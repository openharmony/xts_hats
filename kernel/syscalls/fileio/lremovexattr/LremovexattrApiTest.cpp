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

class LremovexattrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void LremovexattrApiTest::SetUp()
{
}
void LremovexattrApiTest::TearDown()
{
}
void LremovexattrApiTest::SetUpTestCase()
{
}
void LremovexattrApiTest::TearDownTestCase()
{
}

static const char* OPEN_API_TEST_FILE = "/data/local/tmp";
const int BUFFER_SIZE = 128;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LREMOVEXATTR_0100
 * @tc.name   : LremovexattrDeleteExtentAttSuccess_0001
 * @tc.desc   : delete the specified path extension attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LremovexattrApiTest, LremovexattrDeleteExtentAttSuccess_0001, Function | MediumTest | Level1)
{
    const char* name = "user.example";
    const char* setValue = "Hello World";
    ssize_t size = strlen(setValue) + 1;
    int result = setxattr(OPEN_API_TEST_FILE, name, setValue, size, 0);
    EXPECT_TRUE(result != -1);

    int ret = lremovexattr(OPEN_API_TEST_FILE, name);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LREMOVEXATTR_0200
 * @tc.name   : LremovexattrDeleteNonExtentAttrFailed_0002
 * @tc.desc   : delete the specified path non extension attribute failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LremovexattrApiTest, LremovexattrDeleteNonExtentAttrFailed_0002, Function | MediumTest | Level2)
{
    const char* name = "user.example";
    errno = 0;
    int ret = lremovexattr(OPEN_API_TEST_FILE, name);
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(errno == ENODATA);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LREMOVEXATTR_0300
 * @tc.name   : LremovexattrDeleteNonPathExtensionAttributeFailed_0003
 * @tc.desc   : delete the non path extension attribute failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LremovexattrApiTest, LremovexattrDeleteNonPathExtensionAttributeFailed_0003, Function | MediumTest | Level2)
{
    const char* name = "user.example";
    char tmpPath[BUFFER_SIZE] = {0};
    int num = sprintf_s(tmpPath, BUFFER_SIZE, "%s/non_existing_dir", OPEN_API_TEST_FILE);
    EXPECT_TRUE(num > 0);

    errno = 0;
    int ret = lremovexattr(tmpPath, name);
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(errno == ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LREMOVEXATTR_0400
 * @tc.name   : LremovexattrDeleteFileExtentAttrSuccess_0004
 * @tc.desc   : delete the specified file extension attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LremovexattrApiTest, LremovexattrDeleteFileExtentAttrSuccess_0004, Function | MediumTest | Level1)
{
    char tmpPath[BUFFER_SIZE] = {0};
    int num = sprintf_s(tmpPath, BUFFER_SIZE, "%s/existing_file.txt", OPEN_API_TEST_FILE);
    EXPECT_TRUE(num > 0);

    int fd = open(tmpPath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    const char* name = "user.example";
    const char* setValue = "Hello World";
    ssize_t size = strlen(setValue) + 1;
    int result = setxattr(tmpPath, name, setValue, size, 0);
    EXPECT_TRUE(result != -1);

    int ret = lremovexattr(tmpPath, name);
    EXPECT_TRUE(ret == 0);

    remove(tmpPath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LREMOVEXATTR_0500
 * @tc.name   : LremovexattrDeleteSNonExtentAttrFailed_0005
 * @tc.desc   : delete the specified file non extension attribute failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LremovexattrApiTest, LremovexattrDeleteSNonExtentAttrFailed_0005, Function | MediumTest | Level2)
{
    const char* name = "user.example";

    char tmpPath[BUFFER_SIZE] = {0};
    int num = sprintf_s(tmpPath, BUFFER_SIZE, "%s/existing_file.txt", OPEN_API_TEST_FILE);
    EXPECT_TRUE(num > 0);

    int fd = open(tmpPath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    errno = 0;
    int ret = lremovexattr(tmpPath, name);
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(errno == ENODATA);
    remove(tmpPath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LREMOVEXATTR_0600
 * @tc.name   : LremovexattrDeleteNonFileExtensionAttributeFailed_0006
 * @tc.desc   : delete the non file extension attribute failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LremovexattrApiTest, LremovexattrDeleteNonFileExtensionAttributeFailed_0006, Function | MediumTest | Level2)
{
    const char* name = "user.example";
    char tmpPath[BUFFER_SIZE] = {0};
    int num = sprintf_s(tmpPath, BUFFER_SIZE, "%s/non_existing_file.txt", OPEN_API_TEST_FILE);
    EXPECT_TRUE(num > 0);

    errno = 0;
    int ret = lremovexattr(tmpPath, name);
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(errno == ENOENT);
}

