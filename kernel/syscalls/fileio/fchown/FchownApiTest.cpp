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

static const char *TEST_FILE = "/data/local/tmp/fchown.txt";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

class FchownApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FchownApiTest::SetUp()
{
}
void FchownApiTest::TearDown()
{
}
void FchownApiTest::SetUpTestCase()
{
}
void FchownApiTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FCHOWN_0100
 * @tc.name FchownFileSuccess_0001
 * @tc.desc fchown change valid file owner success.
*/
HWTEST_F(FchownApiTest, FchownFileSuccess_0001, Function | MediumTest | Level1)
{
    int ret = -1;
    struct stat stat1;
    struct stat stat2;

    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);
    ret = fstat(fd, &stat1);
    uid_t uid = stat1.st_uid + 1;
    gid_t gid = stat1.st_gid + 1;

    ret = fchown(fd, uid, gid);
    EXPECT_EQ(ret, 0);
    ret = fstat(fd, &stat2);
    EXPECT_EQ(stat2.st_uid, uid);
    EXPECT_EQ(stat2.st_gid, gid);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_FCHOWN_0200
 * @tc.name FchownInvalidFdFail_0002
 * @tc.desc fchown change invalid file owner fail, errno EBADF.
*/
HWTEST_F(FchownApiTest, FchownInvalidFdFail_0002, Function | MediumTest | Level2)
{
    uid_t uid = getuid();
    gid_t gid = getgid();
    errno = 0;
    int ret = fchown(-1, uid, gid);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}
