/*
 * Copyright (C) 2024 HiHope Open Source Organization.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace testing::ext;

class HatsSyncfsTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsSyncfsTest::SetUp()
{
}
void HatsSyncfsTest::TearDown()
{
}
void HatsSyncfsTest::SetUpTestCase()
{
}
void HatsSyncfsTest::TearDownTestCase()
{
}

static const char *SYNCFS_TEST_FILE = "/data/local/tmp/trySyncfs.txt";
static const char *TEST_DATA = "Hello syncfs!";
static const int TEST_DATA_LEN = strlen(TEST_DATA);

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYNCFS_0100
 * @tc.name   : SyncfsFileToDiskSuccess_0001
 * @tc.desc   : syncfs write file from buffer to disk success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyncfsTest, SyncfsFileToDiskSuccess_0001, Function | MediumTest | Level1)
{
    int fd;
    int ret;

    fd = open(SYNCFS_TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);
    ret = write(fd, TEST_DATA, TEST_DATA_LEN);
    EXPECT_TRUE(ret <= TEST_DATA_LEN);

    ret = syncfs(fd);
    EXPECT_TRUE(ret == 0);
    close(fd);

    remove(SYNCFS_TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYNCFS_0200
 * @tc.name   : SyncfsInvalidFdFail_0002
 * @tc.desc   : syncfs write file from buffer to disk using invalid fd fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSyncfsTest, SyncfsInvalidFdFail_0002, Function | MediumTest | Level2)
{
    int fd;
    int ret;

    fd = -1;
    errno = 0;
    ret = syncfs(fd);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EBADF);

    remove(SYNCFS_TEST_FILE);
}