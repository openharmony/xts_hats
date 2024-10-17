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

static const char *TEST_VALID_PATH = "/data/local/tmp/attr_test_dir";
static const char *TEST_VALID_FILE = "/data/local/tmp/attr_test_dir/attr_test.txt";
static const char *TEST_INVALID_PATH = "/data/local/abcd";
static const int MAX_LEN = 128;

class AttrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void AttrApiTest::SetUp()
{
    int fd = -1;
    if (access(TEST_VALID_PATH, F_OK) == 0) {
        (void)remove(TEST_VALID_PATH);
    }
    (void)mkdir(TEST_VALID_PATH, S_IWUSR | S_IRUSR | S_IXUSR);
    fd = open(TEST_VALID_FILE, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    close(fd);
}
void AttrApiTest::TearDown()
{
    (void)remove(TEST_VALID_FILE);
    if (access(TEST_VALID_PATH, F_OK) == 0) {
        (void)remove(TEST_VALID_PATH);
    }
}
void AttrApiTest::SetUpTestCase()
{
}
void AttrApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_XATTR_0100
 * @tc.name   : LsetAndListxattrValidFileTestSuccess_0001
 * @tc.desc   : lsetxattr and listxattr test valid path and file extend properties success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(AttrApiTest, ListxattrFileExtendPropertiesTest_0001, Function | MediumTest | Level1)
{
    char list[MAX_LEN] = { 0 };
    const char *name = "user.path_attr";
    const char *value = "path_attr_value";
    size_t size = strlen(value);

    // set path xattr, success
    int ret = lsetxattr(TEST_VALID_PATH, name, value, size, 0);
    EXPECT_EQ(ret, 0);

    // get path xattr list, success
    size = listxattr(TEST_VALID_PATH, list, sizeof(list));
    EXPECT_TRUE(size > 0);

    // set file xattr, success
    name = "user.file_attr";
    value = "file_attr_value";
    size = strlen(value);
    ret = lsetxattr(TEST_VALID_FILE, name, value, size, 0);
    EXPECT_EQ(ret, 0);

    // get file xattr list, success
    size = listxattr(TEST_VALID_FILE, list, sizeof(list));
    EXPECT_TRUE(size >= 0);

    // only get xattr size, success
    size = listxattr(TEST_VALID_FILE, nullptr, 0);
    EXPECT_TRUE(size >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_XATTR_0200
 * @tc.name   : LsetxattrInvalidPropertyFailed_0002
 * @tc.desc   : lsetxattr set invalid xattr failed, errno EOPNOTSUPP.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(AttrApiTest, LsetxattrInvalidPropertyFailed_0002, Function | MediumTest | Level2)
{
    const char *name = "invalid_attr";
    const char *value = "invalid_attr_value";
    size_t size = strlen(value);

    // set invalid property
    errno = 0;
    int ret = lsetxattr(TEST_VALID_PATH, name, value, size, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EOPNOTSUPP);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_XATTR_0300
 * @tc.name   : LsetAndListxattrInvalidFileFailed_0003
 * @tc.desc   : lsetxattr and listxattr test use invalid path or file failed, errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(AttrApiTest, LsetAndListxattrInvalidFileFailed_0003, Function | MediumTest | Level2)
{
    char list[MAX_LEN] = { 0 };
    const char *name = "user.path_attr";
    const char *value = "path_attr_value";
    size_t size = strlen(value);

    errno = 0;
    int ret = lsetxattr(TEST_INVALID_PATH, name, value, size, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);

    errno = 0;
    size = listxattr(TEST_INVALID_PATH, list, sizeof(list));
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_XATTR_0400
 * @tc.name   : RemovexattrRemoveNotExistAttrFailed_0004
 * @tc.desc   : Removexattr removes attribute that does not exist failed, errno ENOENT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(AttrApiTest, RemovexattrRemoveNotExistAttrFailed_0004, Function | MediumTest | Level2)
{
    const char *name = "user.invalid_attr";
    errno = 0;
    int ret = removexattr(TEST_VALID_PATH, name);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENODATA);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_XATTR_0500
 * @tc.name   : RemovexattrExistAttrSuccess_0005
 * @tc.desc   : Removexattr removes exist attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(AttrApiTest, RemovexattrExistAttrSuccess_0005, Function | MediumTest | Level1)
{
    const char *name = "user.path_attr";
    const char *value = "path_attr_value";
    size_t size = strlen(value);

    // set path xattr, success
    int ret = lsetxattr(TEST_VALID_PATH, name, value, size, 0);
    EXPECT_EQ(ret, 0);

    // remove exist xattr success
    ret = removexattr(TEST_VALID_PATH, name);
    EXPECT_EQ(ret, 0);
}