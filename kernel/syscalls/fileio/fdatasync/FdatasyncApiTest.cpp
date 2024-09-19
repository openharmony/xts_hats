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
#include <gtest/gtest.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *TEST_FILE = "/data/local/tmp/fdatasync.txt";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

class FdatasyncApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FdatasyncApiTest::SetUp()
{
}
void FdatasyncApiTest::TearDown()
{
}
void FdatasyncApiTest::SetUpTestCase()
{
}
void FdatasyncApiTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FDATASYNC_0100
 * @tc.name FdatasyncValidFdSuccess_0001
 * @tc.desc fdatasync sync valid file success.
*/
HWTEST_F(FdatasyncApiTest, FdatasyncValidFdSuccess_0001, Function | MediumTest | Level1)
{
    int ret = -1;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, MODE_0644);
    EXPECT_TRUE(fd > 0);
    
    const char *data = "fdatasync test data.";
    ssize_t bytesWritten = write(fd, data, strlen(data) + 1);
    EXPECT_NE(bytesWritten, -1);

    ret = fdatasync(fd);
    EXPECT_EQ(ret, 0);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FDATASYNC_0200
 * @tc.name FdatasyncInvalidFdFail_0002
 * @tc.desc fdatasync sync invalid file fail, errno EBADF.
*/
HWTEST_F(FdatasyncApiTest, FdatasyncInvalidFdFail_0002, Function | MediumTest | Level2)
{
    int invalidFd = -1;
    errno = 0;
    int ret = fdatasync(invalidFd);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}
