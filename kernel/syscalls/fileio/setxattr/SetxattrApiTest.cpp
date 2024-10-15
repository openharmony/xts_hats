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

#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *TEST_FILE_PATH = "/data/local/tmp/test_file";
static const char *TEST_ATTRIBUTE = "user.testattribute";
static const char *TEST_ATTRIBUTE_VALUE = "testvalue";

class SetxattrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void SetxattrApiTest::SetUp()
{
    int fd = open(TEST_FILE_PATH, O_CREAT | O_WRONLY, 0644);
    close(fd);
}
void SetxattrApiTest::TearDown()
{
    unlink(TEST_FILE_PATH);
}
void SetxattrApiTest::SetUpTestCase()
{
}
void SetxattrApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETXATTR_0100
 * @tc.name   : SetxattrSuccess_0001
 * @tc.desc   : Setxattr success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(SetxattrApiTest, SetxattrSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    ret = setxattr(TEST_FILE_PATH, TEST_ATTRIBUTE, TEST_ATTRIBUTE_VALUE, strlen(TEST_ATTRIBUTE_VALUE) + 1, 0);
    EXPECT_EQ(ret, 0);

    char value[256];
    ssize_t size = getxattr(TEST_FILE_PATH, TEST_ATTRIBUTE, value, sizeof(value));
    EXPECT_GT(size, 0);
    EXPECT_STREQ(TEST_ATTRIBUTE_VALUE, value);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_STATX_0200
 * @tc.name   : SetxattrInvalidPath_0002
 * @tc.desc   : statx invalid path.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(SetxattrApiTest, SetxattrInvalidPath_0002, Function | MediumTest | Level2)
{
    int ret;
    ret = setxattr("/non/existent/path", TEST_ATTRIBUTE, TEST_ATTRIBUTE_VALUE, strlen(TEST_ATTRIBUTE_VALUE) + 1, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
}