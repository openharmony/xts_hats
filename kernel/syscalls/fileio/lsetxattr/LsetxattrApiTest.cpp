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

class LsetxattrApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void LsetxattrApiTest::SetUp()
{
}
void LsetxattrApiTest::TearDown()
{
}
void LsetxattrApiTest::SetUpTestCase()
{
}
void LsetxattrApiTest::TearDownTestCase()
{
}

int SetExtendedAttribute(const char* linkPath, const char* attrName, const char* attrValue, int flags = 0)
{
    size_t attrSize = strlen(attrValue) + 1;
    int ret = lsetxattr(linkPath, attrName, attrValue, attrSize, flags);
    if (ret == -1) {
        return -1;
    }
    return 0;
}

int GetExtendedAttribute(const char* linkPath, const char* attrName, char* buffer, size_t bufferSize)
{
    ssize_t ret = lgetxattr(linkPath, attrName, buffer, bufferSize);
    if (ret == -1) {
        return -1;
    }
    buffer[ret] = '\0';
    return 0;
}

int RemoveExtendedAttribute(const char* linkPath, const char* attrName)
{
    int ret = lremovexattr(linkPath, attrName);
    if (ret == -1) {
        return -1;
    }
    return 0;
}

static const char* OPEN_API_TEST_FILE = "/data/local/tmp";
const int BUFFER_SIZE = 128;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_0100
 * @tc.name   : LsetxattrSetExtAttrSuccess_0001
 * @tc.desc   : set file extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrSetExtAttrSuccess_0001, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    const char* attrName = "user.myattr";
    const char* attrValue = "Hello, xattr!";
    int ret = SetExtendedAttribute(targetFilePath, attrName, attrValue);
    EXPECT_TRUE(ret == 0);

    char buffer[64] = {0};
    ret = GetExtendedAttribute(targetFilePath, attrName, buffer, sizeof(buffer));
    EXPECT_TRUE(ret == 0);

    ret = strncmp(attrValue, buffer, strlen(attrValue));
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_0200
 * @tc.name   : LsetxattrSetMoreExtAttrSuccess_0002
 * @tc.desc   : set file more extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrSetMoreExtAttrSuccess_0002, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    const char* attrName1 = "user.myattr1";
    const char* attrValue1 = "Hello, xattr1!";
    int ret = SetExtendedAttribute(targetFilePath, attrName1, attrValue1);
    EXPECT_TRUE(ret == 0);
    char buffer[64] = {0};
    ret = GetExtendedAttribute(targetFilePath, attrName1, buffer, sizeof(buffer));
    EXPECT_TRUE(ret == 0);

    ret = strncmp(attrValue1, buffer, strlen(attrValue1));
    EXPECT_TRUE(ret == 0);

    const char* attrName2 = "user.myattr2";
    const char* attrValue2 = "Hello, xattr2!";
    ret = SetExtendedAttribute(targetFilePath, attrName2, attrValue2);
    EXPECT_TRUE(ret == 0);

    memset_s(&buffer, sizeof(buffer), 0, sizeof(buffer));
    ret = GetExtendedAttribute(targetFilePath, attrName2, buffer, sizeof(buffer));
    EXPECT_TRUE(ret == 0);

    ret = strncmp(attrValue2, buffer, strlen(attrValue2));
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_0300
 * @tc.name   : LsetxattrCreateExtAttrSuccess_0003
 * @tc.desc   : create file extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrCreateExtAttrSuccess_0003, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    const char* attrName = "user.myattr";
    const char* attrValue = "Hello, xattr!";
    int ret = SetExtendedAttribute(targetFilePath, attrName, attrValue, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    char buffer[64] = {0};
    ret = GetExtendedAttribute(targetFilePath, attrName, buffer, sizeof(buffer));
    EXPECT_TRUE(ret == 0);

    ret = strncmp(attrValue, buffer, strlen(attrValue));
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_0400
 * @tc.name   : LsetxattrCreateMoreExtAttrSuccess_0004
 * @tc.desc   : create file more extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrCreateMoreExtAttrSuccess_0004, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    const char* attrName1 = "user.myattr1";
    const char* attrValue1 = "Hello, xattr1!";
    int ret = SetExtendedAttribute(targetFilePath, attrName1, attrValue1, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    const char* attrName2 = "user.myattr2";
    const char* attrValue2 = "Hello, xattr2!";
    ret = SetExtendedAttribute(targetFilePath, attrName2, attrValue2, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_0500
 * @tc.name   : LsetxattrReplaceExtAttrSuccess_0005
 * @tc.desc   : replace file more extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrReplaceExtAttrSuccess_0005, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    const char* attrName = "user.myattr";
    const char* attrValue1 = "Hello, xattr1!";
    int ret = SetExtendedAttribute(targetFilePath, attrName, attrValue1, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    const char* attrValue2 = "Hello, xattr2!";
    ret = SetExtendedAttribute(targetFilePath, attrName, attrValue2, XATTR_REPLACE);
    EXPECT_TRUE(ret == 0);

    char buffer[64] = {0};
    ret = GetExtendedAttribute(targetFilePath, attrName, buffer, sizeof(buffer));
    EXPECT_TRUE(ret == 0);

    ret = strncmp(attrValue2, buffer, strlen(attrValue2));
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_0600
 * @tc.name   : LsetxattrSetLExtAttrSuccess_0006
 * @tc.desc   : set link file extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrSetLExtAttrSuccess_0006, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    const char* linkPath = "link_to_file.txt";

    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    char linkFilePath[BUFFER_SIZE] = {0};
    num = sprintf_s(linkFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, linkPath);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    int newFd = open(OPEN_API_TEST_FILE, O_RDONLY, 0644);
    EXPECT_TRUE(newFd > 0);

    int ret = linkat(newFd, targetFile, newFd, linkPath, 0);
    EXPECT_TRUE(ret == 0);

    const char* attrName = "user.myattr";
    const char* attrValue = "Hello, xattr!";
    ret = SetExtendedAttribute(linkFilePath, attrName, attrValue, 0);
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
    remove(linkFilePath);
    close(newFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_0700
 * @tc.name   : LsetxattrSetLMoreExtAttrSuccess_0007
 * @tc.desc   : set link file more extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrSetLMoreExtAttrSuccess_0007, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    const char* linkPath = "link_to_file.txt";

    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    char linkFilePath[BUFFER_SIZE] = {0};
    num = sprintf_s(linkFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, linkPath);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    int newFd = open(OPEN_API_TEST_FILE, O_RDONLY, 0644);
    EXPECT_TRUE(newFd > 0);

    int ret = linkat(newFd, targetFile, newFd, linkPath, 0);
    EXPECT_TRUE(ret == 0);

    const char* attrName1 = "user.myattr1";
    const char* attrValue1 = "Hello, xattr1!";
    ret = SetExtendedAttribute(linkFilePath, attrName1, attrValue1);
    EXPECT_TRUE(ret == 0);

    const char* attrName2 = "user.myattr2";
    const char* attrValue2 = "Hello, xattr2!";
    ret = SetExtendedAttribute(linkFilePath, attrName2, attrValue2);
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
    remove(linkFilePath);
    close(newFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_0800
 * @tc.name   : LsetxattrCreateLExtAttrSuccess_0008
 * @tc.desc   : create link file extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrCreateLExtAttrSuccess_0008, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    const char* linkPath = "link_to_file.txt";

    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    char linkFilePath[BUFFER_SIZE] = {0};
    num = sprintf_s(linkFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, linkPath);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    int newFd = open(OPEN_API_TEST_FILE, O_RDONLY, 0644);
    EXPECT_TRUE(newFd > 0);

    int ret = linkat(newFd, targetFile, newFd, linkPath, 0);
    EXPECT_TRUE(ret == 0);

    const char* attrName = "user.myattr";
    const char* attrValue = "Hello, xattr!";
    ret = SetExtendedAttribute(linkFilePath, attrName, attrValue, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
    remove(linkFilePath);
    close(newFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_0900
 * @tc.name   : LsetxattrCreateLMoreExtAttrSuccess_0009
 * @tc.desc   : create link file more extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrCreateLMoreExtAttrSuccess_0009, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    const char* linkPath = "link_to_file.txt";

    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    char linkFilePath[BUFFER_SIZE] = {0};
    num = sprintf_s(linkFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, linkPath);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    int newFd = open(OPEN_API_TEST_FILE, O_RDONLY, 0644);
    EXPECT_TRUE(newFd > 0);

    int ret = linkat(newFd, targetFile, newFd, linkPath, 0);
    EXPECT_TRUE(ret == 0);

    const char* attrName1 = "user.myattr1";
    const char* attrValue1 = "Hello, xattr1!";
    ret = SetExtendedAttribute(linkFilePath, attrName1, attrValue1, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    const char* attrName2 = "user.myattr2";
    const char* attrValue2 = "Hello, xattr2!";
    ret = SetExtendedAttribute(linkFilePath, attrName2, attrValue2, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
    remove(linkFilePath);
    close(newFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_1000
 * @tc.name   : LsetxattrReplaceLExtAttrSuccess_0010
 * @tc.desc   : replace link file more extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrReplaceLExtAttrSuccess_0010, Function | MediumTest | Level1)
{
    const char* targetFile = "target_file.txt";
    const char* linkPath = "link_to_file.txt";

    char targetFilePath[BUFFER_SIZE] = {0};
    int num = sprintf_s(targetFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, targetFile);
    EXPECT_TRUE(num > 0);

    char linkFilePath[BUFFER_SIZE] = {0};
    num = sprintf_s(linkFilePath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, linkPath);
    EXPECT_TRUE(num > 0);

    int fd = open(targetFilePath, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);
    close(fd);

    int newFd = open(OPEN_API_TEST_FILE, O_RDONLY, 0644);
    EXPECT_TRUE(newFd > 0);

    int ret = linkat(newFd, targetFile, newFd, linkPath, 0);
    EXPECT_TRUE(ret == 0);

    const char* attrName = "user.myattr";
    const char* attrValue1 = "Hello, xattr1!";
    ret = SetExtendedAttribute(linkFilePath, attrName, attrValue1, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    const char* attrValue2 = "Hello, xattr2!";
    ret = SetExtendedAttribute(linkFilePath, attrName, attrValue2, XATTR_REPLACE);
    EXPECT_TRUE(ret == 0);

    remove(targetFilePath);
    remove(linkFilePath);
    close(newFd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_1100
 * @tc.name   : LsetxattrSetPExtAttrSuccess_0011
 * @tc.desc   : set path extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrSetPExtAttrSuccess_0011, Function | MediumTest | Level1)
{
    const char* attrName = "user.myattr";
    const char* attrValue = "Hello, xattr!";
    int ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName, attrValue);
    EXPECT_TRUE(ret == 0);

    ret = RemoveExtendedAttribute(OPEN_API_TEST_FILE, attrName);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_1200
 * @tc.name   : LsetxattrSetPMoreExtAttrSuccess_0012
 * @tc.desc   : set path more extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrSetPMoreExtAttrSuccess_0012, Function | MediumTest | Level1)
{
    const char* attrName1 = "user.myattr1";
    const char* attrValue1 = "Hello, xattr1!";
    int ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName1, attrValue1);
    EXPECT_TRUE(ret == 0);

    const char* attrName2 = "user.myattr2";
    const char* attrValue2 = "Hello, xattr2!";
    ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName2, attrValue2);
    EXPECT_TRUE(ret == 0);

    ret = RemoveExtendedAttribute(OPEN_API_TEST_FILE, attrName1);
    EXPECT_TRUE(ret == 0);

    ret = RemoveExtendedAttribute(OPEN_API_TEST_FILE, attrName2);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_1300
 * @tc.name   : LsetxattrCreatePExtAttrSuccess_0013
 * @tc.desc   : create path extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrCreatePExtAttrSuccess_0013, Function | MediumTest | Level1)
{
    const char* attrName = "user.myattr";
    const char* attrValue = "Hello, xattr!";
    int ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName, attrValue, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    ret = RemoveExtendedAttribute(OPEN_API_TEST_FILE, attrName);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_1400
 * @tc.name   : LsetxattrCreatePMoreExtAttrSuccess_0014
 * @tc.desc   : create path more extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrCreatePMoreExtAttrSuccess_0014, Function | MediumTest | Level1)
{
    const char* attrName1 = "user.myattr1";
    const char* attrValue1 = "Hello, xattr1!";
    int ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName1, attrValue1, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    const char* attrName2 = "user.myattr2";
    const char* attrValue2 = "Hello, xattr2!";
    ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName2, attrValue2, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    ret = RemoveExtendedAttribute(OPEN_API_TEST_FILE, attrName1);
    EXPECT_TRUE(ret == 0);

    ret = RemoveExtendedAttribute(OPEN_API_TEST_FILE, attrName2);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_1500
 * @tc.name   : LsetxattrReplacePExtAttrSuccess_0015
 * @tc.desc   : replace path more extended attribute success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(LsetxattrApiTest, LsetxattrReplacePExtAttrSuccess_0015, Function | MediumTest | Level1)
{
    const char* attrName = "user.myattr";
    const char* attrValue1 = "Hello, xattr1!";
    int ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName, attrValue1, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    char buffer[64] = {0};
    ret = GetExtendedAttribute(OPEN_API_TEST_FILE, attrName, buffer, sizeof(buffer));
    EXPECT_TRUE(ret == 0);

    ret = strncmp(attrValue1, buffer, strlen(attrValue1));
    EXPECT_TRUE(ret == 0);

    const char* attrValue2 = "Hello, xattr2!";
    ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName, attrValue2, XATTR_REPLACE);
    EXPECT_TRUE(ret == 0);

    memset_s(&buffer, sizeof(buffer), 0, sizeof(buffer));
    ret = GetExtendedAttribute(OPEN_API_TEST_FILE, attrName, buffer, sizeof(buffer));
    EXPECT_TRUE(ret == 0);

    ret = strncmp(attrValue2, buffer, strlen(attrValue2));
    EXPECT_TRUE(ret == 0);

    ret = RemoveExtendedAttribute(OPEN_API_TEST_FILE, attrName);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_1600
 * @tc.name   : LsetxattrCreateExtAttrFailed_0016
 * @tc.desc   : create non path extended attribute failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LsetxattrApiTest, LsetxattrCreateExtAttrFailed_0016, Function | MediumTest | Level2)
{
    const char* nonDirPath = "non_existing_dir";
    char tmpNonDirPath[BUFFER_SIZE] = {0};
    int num = sprintf_s(tmpNonDirPath, BUFFER_SIZE, "%s/%s", OPEN_API_TEST_FILE, nonDirPath);
    EXPECT_TRUE(num > 0);

    errno = 0;
    const char* attrName = "user.myattr";
    const char* attrValue = "Hello, xattr!";
    int ret = SetExtendedAttribute(tmpNonDirPath, attrName, attrValue, XATTR_CREATE);
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(errno == ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_LSETXATTR_1700
 * @tc.name   : LsetxattrCreateSameExtAttrFailed_0017
 * @tc.desc   : create file same extended attribute failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(LsetxattrApiTest, LsetxattrCreateSameExtAttrFailed_0017, Function | MediumTest | Level2)
{
    const char* attrName = "user.myattr";
    const char* attrValue = "Hello, xattr1!";
    int ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName, attrValue, XATTR_CREATE);
    EXPECT_TRUE(ret == 0);

    errno = 0;
    ret = SetExtendedAttribute(OPEN_API_TEST_FILE, attrName, attrValue, XATTR_CREATE);
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(errno == EEXIST);

    ret = RemoveExtendedAttribute(OPEN_API_TEST_FILE, attrName);
    EXPECT_TRUE(ret == 0);
}