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
#include <sys/xattr.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *TEST_FILE = "/data/local/tmp/test_file.txt";
static const char *TEST_DATA = "Hello, world!";
static const size_t TEST_LEN = strlen(TEST_DATA);

class LseekApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void LseekApiTest::SetUp()
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    write(fd, TEST_DATA, TEST_LEN);
    close(fd);
}
void LseekApiTest::TearDown()
{
    (void)remove(TEST_FILE);
}
void LseekApiTest::SetUpTestCase()
{
}
void LseekApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSEEK_0100
 * @tc.name   : LseekSetOffsetTestSuccess_0001
 * @tc.desc   : lseek set the offset of the file test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LseekApiTest, LseekSetOffsetTestSuccess_0001, Function | MediumTest | Level1)
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);

    // file middle position test
    off_t offset = TEST_LEN / 2;
    off_t pos = lseek(fd, offset, SEEK_SET);
    EXPECT_EQ(pos, offset);

    // file start position test
    offset = 0;
    pos = lseek(fd, offset, SEEK_SET);
    EXPECT_EQ(pos, offset);

    // file end position test
    offset = TEST_LEN;
    pos = lseek(fd, offset, SEEK_SET);
    EXPECT_EQ(pos, offset);

    // position larger than file length test
    offset = TEST_LEN * 2;
    errno = 0;
    pos = lseek(fd, offset, SEEK_SET);
    EXPECT_EQ(pos, offset);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSEEK_0200
 * @tc.name   : LseekSetNegativePositionFailed_0002
 * @tc.desc   : lseek sets a negative position failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LseekApiTest, LseekSetNegativePositionFailed_0002, Function | MediumTest | Level2)
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);

    // -1 position test failed, errno EINVAL
    off_t offset = -1;
    errno = 0;
    off_t pos = lseek(fd, offset, SEEK_SET);
    EXPECT_EQ(pos, -1);
    EXPECT_EQ(errno, EINVAL);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSEEK_0300
 * @tc.name   : LseekSEEK_CURTestSuccess_0003
 * @tc.desc   : lseek set position from current pos success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LseekApiTest, LseekSEEK_CURTestSuccess_0003, Function | MediumTest | Level1)
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);

    // set pos to middle position
    off_t start = TEST_LEN / 2;
    off_t initPos = lseek(fd, start, SEEK_SET);
    EXPECT_EQ(initPos, start);

    // set positive pos to offset from current position
    off_t offset = TEST_LEN / 2;
    off_t pos = lseek(fd, offset, SEEK_CUR);
    EXPECT_EQ(pos, initPos + offset);

    // set negative pos to offset from current position
    offset = (TEST_LEN / 2) * (-1);
    initPos = pos;
    pos = lseek(fd, offset, SEEK_CUR);
    EXPECT_EQ(pos, initPos + offset);

    // set positive pos to offset from current position
    offset = TEST_LEN * 2;
    initPos = pos;
    pos = lseek(fd, offset, SEEK_CUR);
    EXPECT_EQ(pos, initPos + offset);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSEEK_0400
 * @tc.name   : LseekSEEK_CURNegativeTestFailed_0004
 * @tc.desc   : lseek set negative position from current pos failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LseekApiTest, LseekSEEK_CURNegativeTestFailed_0004, Function | MediumTest | Level2)
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);

    // set negative pos to offset from current position
    off_t offset = -1;
    errno = 0;
    off_t pos = lseek(fd, offset, SEEK_CUR);
    EXPECT_EQ(pos, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSEEK_0500
 * @tc.name   : LseekSEEK_ENDTestSuccess_0005
 * @tc.desc   : lseek use SEEK_END set the position to file end success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LseekApiTest, LseekSEEK_ENDTestSuccess_0005, Function | MediumTest | Level1)
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);

    off_t pos = lseek(fd, 0, SEEK_END);
    EXPECT_EQ(pos, TEST_LEN);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSEEK_0600
 * @tc.name   : LseekInvalidFdTestFailed_0006
 * @tc.desc   : lseek used invalid fd test failed, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LseekApiTest, LseekInvalidFdTestFailed_0006, Function | MediumTest | Level2)
{
    errno = 0;
    off_t pos = lseek(-1, 0, SEEK_SET);
    EXPECT_EQ(pos, -1);
    EXPECT_EQ(errno, EBADF);
}