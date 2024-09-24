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

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class FsyncApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FsyncApiTest::SetUp()
{
}
void FsyncApiTest::TearDown()
{
}
void FsyncApiTest::SetUpTestCase()
{
}
void FsyncApiTest::TearDownTestCase()
{
}

static const char* TEST_FILE = "/data/local/tmp/test.txt";

/*
 * @tc.number SUB_KERNEL_SYSCALL_FSYNC_0100
 * @tc.name FsyncValidFdSuccess_0001
 * @tc.desc sync valid fd after write success.
*/
HWTEST_F(FsyncApiTest, FsyncValidFdSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    const char* data = "Hello, fsync!";

    fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);

    ret = write(fd, data, strlen(data));
    EXPECT_TRUE(ret >= 0);

    ret = fsync(fd);
    EXPECT_EQ(ret, 0);
    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FSYNC_0200
 * @tc.name FsyncUseInvalidFdFailed_0002
 * @tc.desc fsync use invalid fd failed, errno EBADF.
*/
HWTEST_F(FsyncApiTest, FsyncUseInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    int fd = -1;
    errno = 0;
    int ret = fsync(fd);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}