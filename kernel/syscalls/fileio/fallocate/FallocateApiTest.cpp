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

static const char *TEST_FILE = "/data/local/tmp/fallocate.txt";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
static off_t g_offset = 0;
static off_t g_length = 1024;

class FallocateApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FallocateApiTest::SetUp()
{
}
void FallocateApiTest::TearDown()
{
}
void FallocateApiTest::SetUpTestCase()
{
}
void FallocateApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FALLOCATE_0100
 * @tc.name   : FallocateValidFdSuccess_0001
 * @tc.desc   : fallocate allocate valid file space success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FallocateApiTest, FallocateValidFdSuccess_0001, Function | MediumTest | Level1)
{
    int ret = -1;
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    ret = fallocate(fd, FALLOC_FL_KEEP_SIZE, g_offset, g_length);
    EXPECT_EQ(ret, 0);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FALLOCATE_0200
 * @tc.name   : FallocateInvalidFdFail_0002
 * @tc.desc   : fallocate allocate invalid fd space fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FallocateApiTest, FallocateInvalidFdFail_0002, Function | MediumTest | Level2)
{
    int ret;
    int fd = -1;

    errno = 0;
    ret = fallocate(fd, FALLOC_FL_KEEP_SIZE, g_offset, g_length);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FALLOCATE_0300
 * @tc.name   : FallocateInvalidBaseOrLenFail_0003
 * @tc.desc   : fallocate allocate file space with invalid offset or invalid length fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FallocateApiTest, FallocateInvalidBaseAndLenFail_0003, Function | MediumTest | Level2)
{
    int ret = -1;
    int invalidOffset = -1;
    int invalidLength = -1;

    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    errno = 0;
    ret = fallocate(fd, FALLOC_FL_KEEP_SIZE, invalidOffset, g_length);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    ret = fallocate(fd, FALLOC_FL_KEEP_SIZE, g_offset, invalidLength);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
    remove(TEST_FILE);
}
