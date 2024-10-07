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
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char* TEST_FILE = "/data/local/tmp/test_file.txt";
static const int BUFFER_SIZE = 128;
static const int PATH_MAX_LEN = 128;

class GetxattrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void GetxattrApiTest::SetUp()
{
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    close(fd);
}
void GetxattrApiTest::TearDown()
{
    (void)remove(TEST_FILE);
}
void GetxattrApiTest::SetUpTestCase()
{
}
void GetxattrApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETXATTR_0100
 * @tc.name   : GetxattrGetExistAttrSuccess_0001
 * @tc.desc   : Get the exist file attribute value success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(GetxattrApiTest, GetxattrGetExistAttrSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t size;
    const char* name = "user.example";
    const char* setValue = "Hello World";
    char value[PATH_MAX_LEN] = { 0 };
    int ret = setxattr(TEST_FILE, name, setValue, strlen(setValue), 0);
    EXPECT_EQ(ret, 0);

    size = getxattr(TEST_FILE, name, value, sizeof(value) - 1);
    EXPECT_EQ(size, strlen(setValue));
    EXPECT_STREQ(value, setValue);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETXATTR_0200
 * @tc.name   : GetxattrGetUnknownAttrValueFailed_0002
 * @tc.desc   : Get the unknown file attribute value failed, errno ENODATA.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(GetxattrApiTest, GetxattrGetUnknownAttrValueFailed_0002, Function | MediumTest | Level2)
{
    const char* name = "user.example1";
    char value[BUFFER_SIZE] = {0};
    errno = 0;
    ssize_t size = getxattr(TEST_FILE, name, value, sizeof(value) - 1);
    EXPECT_TRUE(size == -1);
    EXPECT_TRUE(errno == ENODATA);
}