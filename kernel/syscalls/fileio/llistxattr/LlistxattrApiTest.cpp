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

class LlistxattrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void LlistxattrApiTest::SetUp()
{
}
void LlistxattrApiTest::TearDown()
{
}
void LlistxattrApiTest::SetUpTestCase()
{
}
void LlistxattrApiTest::TearDownTestCase()
{
}

static const char* OPEN_API_TEST_FILE = "/data/local/tmp";
const int BUFFER_SIZE = 128;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LLISTXATTR_0100
 * @tc.name   : LlistxattrListExtendedAttrSuccess_0001
 * @tc.desc   : list the extended attributes of the link specified path success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LlistxattrApiTest, LlistxattrListExtendedAttrSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t attrlistLen = llistxattr(OPEN_API_TEST_FILE, nullptr, 0);
    EXPECT_TRUE(attrlistLen > 0);

    char* attrlist = new char[attrlistLen + 1];
    memset_s(attrlist, attrlistLen + 1, 0, attrlistLen + 1);

    attrlistLen = llistxattr(OPEN_API_TEST_FILE, attrlist, attrlistLen);
    EXPECT_TRUE(attrlistLen > 0);

    delete[] attrlist;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LLISTXATTR_0200
 * @tc.name   : LlistxattrListExtendAttrSuccess_0002
 * @tc.desc   : list the extended attributes of the specified link file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LlistxattrApiTest, LlistxattrListExtendAttrSuccess_0002, Function | MediumTest | Level1)
{
    char tmpPath[BUFFER_SIZE] = {0};
    int num = sprintf_s(tmpPath, BUFFER_SIZE, "%s/existing_file.txt", OPEN_API_TEST_FILE);
    EXPECT_TRUE(num > 0);

    int fd = open(tmpPath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    ssize_t attrlistLen = llistxattr(tmpPath, nullptr, 0);
    EXPECT_TRUE(attrlistLen > 0);

    char* attrlist = new char[attrlistLen + 1];
    memset_s(attrlist, attrlistLen + 1, 0, attrlistLen + 1);

    attrlistLen = llistxattr(tmpPath, attrlist, attrlistLen);
    EXPECT_TRUE(attrlistLen > 0);

    delete[] attrlist;
    remove(tmpPath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LLISTXATTR_0300
 * @tc.name   : LlistxattrListExtendAttrNonExistFileFailed_0003
 * @tc.desc   : list the extended attributes for non-existent link file failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LlistxattrApiTest, LlistxattrListExtendAttrNonExistFileFailed_0003, Function | MediumTest | Level2)
{
    char tmpPath[BUFFER_SIZE] = {0};
    int num = sprintf_s(tmpPath, BUFFER_SIZE, "%s/non_existing_file.txt", OPEN_API_TEST_FILE);
    EXPECT_TRUE(num > 0);
    errno = 0;
    ssize_t attrlistLen = llistxattr(tmpPath, nullptr, 0);
    EXPECT_TRUE(attrlistLen == -1);
    EXPECT_TRUE(errno == ENOENT);
}
