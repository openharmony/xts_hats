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
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace testing::ext;

class HatsSyncFileRangeTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsSyncFileRangeTest::SetUp()
{
}
void HatsSyncFileRangeTest::TearDown()
{
}
void HatsSyncFileRangeTest::SetUpTestCase()
{
}
void HatsSyncFileRangeTest::TearDownTestCase()
{
}

static const char *SYNC_RANGE_TEST_FILE = "/data/local/tmp/trySync.txt";
static const char *TEST_DATA = "Hello sync!";
static const int TEST_DATA_LEN = strlen(TEST_DATA);
static const int FILE_OFFSET = 0;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYNCFILERANGE_0100
 * @tc.name   : SyncFileRangeFileToDiskSuccess_0001
 * @tc.desc   : sync_file_range write file from buffer to disk success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyncFileRangeTest, SyncFileRangeFileToDiskSuccess_0001, Function | MediumTest | Level1)
{
    int fd;
    int ret;

    fd = open(SYNC_RANGE_TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd > 0);
    
    ret = sync_file_range(fd, FILE_OFFSET, TEST_DATA_LEN, 0);
    EXPECT_EQ(ret, 0);

    close(fd);
    remove(SYNC_RANGE_TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYNCFILERANGE_0200
 * @tc.name   : SyncFileRangeInvalidFdFail_0002
 * @tc.desc   : sync_file_range sync invalid fd to disk fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSyncFileRangeTest, SyncFileRangeInvalidFdFail_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = sync_file_range(-1, FILE_OFFSET, TEST_DATA_LEN, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYNCFILERANGE_0300
 * @tc.name   : SyncFileRangeUsingInvalidFlagFail_0003
 * @tc.desc   : sync_file_range sync file to disk using invalid flag fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSyncFileRangeTest, SyncFileRangeInvalidOffsetFail_0003, Function | MediumTest | Level2)
{
    int ret;
    int fd = open(SYNC_RANGE_TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd > 0);
    
    errno = 0;
    ret = sync_file_range(fd, FILE_OFFSET, TEST_DATA_LEN, -1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
    remove(SYNC_RANGE_TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYNCFILERANGE_0400
 * @tc.name   : SyncFileRangeInvalidNbytesFail_0004
 * @tc.desc   : sync_file_range sync file to disk when nbytes is invalid fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSyncFileRangeTest, SyncFileRangeInvalidNbytesFail_0004, Function | MediumTest | Level2)
{
    int fd = open(SYNC_RANGE_TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd > 0);
    ssize_t size = write(fd, TEST_DATA, TEST_DATA_LEN);
    EXPECT_EQ(size, TEST_DATA_LEN);

    errno = 0;
    int ret = sync_file_range(fd, FILE_OFFSET, -1, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
    remove(SYNC_RANGE_TEST_FILE);
}
