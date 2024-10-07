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
#include <string>
#include <vector>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class Getdents64ApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void Getdents64ApiTest::SetUp()
{
}
void Getdents64ApiTest::TearDown()
{
}
void Getdents64ApiTest::SetUpTestCase()
{
}
void Getdents64ApiTest::TearDownTestCase()
{
}

static const char* TEST_FILE = "/data/local/tmp";

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETDENTS64_0100
 * @tc.name   : Getdents64GetDirInfoSuccess_0001
 * @tc.desc   : open and get directory entry information success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(Getdents64ApiTest, Getdents64GetDirInfoSuccess_0001, Function | MediumTest | Level1)
{
    struct dirent buf = { 0 };
    int fd = open(TEST_FILE, O_RDONLY);
    EXPECT_TRUE(fd > 0);

    ssize_t size = getdents64(fd, &buf, sizeof(buf));
    EXPECT_TRUE(size > 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETDENTS64_0200
 * @tc.name   : Getdents64GetDirectoryEntryInfoFailed_0002
 * @tc.desc   : get illegal file handle directory entry information failed, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(Getdents64ApiTest, Getdents64GetDirectoryEntryInfoFailed_0002, Function | MediumTest | Level2)
{
    struct dirent buf = { 0 };
    errno = 0;
    ssize_t size = getdents64(-1, &buf, sizeof(buf));
    EXPECT_TRUE(size <= 0);
    EXPECT_EQ(errno, EBADF);
}