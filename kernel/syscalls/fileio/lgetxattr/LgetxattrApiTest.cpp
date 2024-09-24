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

class LgetxattrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void LgetxattrApiTest::SetUp()
{
}
void LgetxattrApiTest::TearDown()
{
}
void LgetxattrApiTest::SetUpTestCase()
{
}
void LgetxattrApiTest::TearDownTestCase()
{
}

static const char* OPEN_API_TEST_FILE = "/data/local/tmp/";
static const int MAX_LEN = 128;

/*
 * @tc.number SUB_KERNEL_SYSCALL_LGETXATTR_0100
 * @tc.name LgetxattrGetValidAttrSuccess_0001
 * @tc.desc lgetxattr get valid attr success.
*/
HWTEST_F(LgetxattrApiTest, LgetxattrGetValidAttrSuccess_0001, Function | MediumTest | Level1)
{
    const char* name = "user.example";
    const char* setValue = "Hello World";
    int ret = setxattr(OPEN_API_TEST_FILE, name, setValue, strlen(setValue), 0);
    EXPECT_EQ(ret, 0);

    char value[MAX_LEN] = {0};
    ssize_t size = lgetxattr(OPEN_API_TEST_FILE, name, value, sizeof(value) - 1);
    EXPECT_EQ(size, strlen(setValue));
    EXPECT_STREQ(value, setValue);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_LGETXATTR_0200
 * @tc.name LgetxattrGetInvalidAttrFailed_0002
 * @tc.desc lgetxattr get invalid attr failed, errno is ENODATA.
*/
HWTEST_F(LgetxattrApiTest, LgetxattrGetInvalidAttrFailed_0002, Function | MediumTest | Level2)
{
    const char* name = "user.example1";
    char value[MAX_LEN] = {0};
    errno = 0;
    ssize_t size = lgetxattr(OPEN_API_TEST_FILE, name, value, sizeof(value) - 1);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, ENODATA);
}