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
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *TEST_FILE = "/data/local/tmp/test.txt";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
mode_t MODE_0755 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
struct stat g_statbuf;

class FchmodApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FchmodApiTest::SetUp()
{
}
void FchmodApiTest::TearDown()
{
}
void FchmodApiTest::SetUpTestCase()
{
}
void FchmodApiTest::TearDownTestCase()
{
    unlink(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHMOD_0100
 * @tc.name   : FchmodFileModeSuccess_0001
 * @tc.desc   : fchmod change file mode bits success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FchmodApiTest, FchmodFileModeSuccess_0001, Function | MediumTest | Level1)
{
    int ret = -1;

    int fd = open(TEST_FILE, O_CREAT | O_RDWR, MODE_0644);
    EXPECT_TRUE(fd > 0);

    ret = fchmod(fd, MODE_0755);
    EXPECT_EQ(ret, 0);

    ret = fstat(fd, &g_statbuf);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ((g_statbuf.st_mode & S_IXUSR), S_IXUSR);
    EXPECT_EQ((g_statbuf.st_mode & S_IXGRP), S_IXGRP);
    EXPECT_EQ((g_statbuf.st_mode & S_IXOTH), S_IXOTH);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FCHMOD_0200
 * @tc.name   : FchmodInvalidFdModeFail_0002
 * @tc.desc   : fchmod change invalid fd mode bits fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FchmodApiTest, FchmodInvalidFdModeFail_0002, Function | MediumTest | Level2)
{
    int ret = -1;
    int invalidFd = -1;
    errno = 0;
    ret = fchmod(invalidFd, MODE_0644);
    EXPECT_NE(ret, 0);
    EXPECT_EQ(errno, EBADF);
}
