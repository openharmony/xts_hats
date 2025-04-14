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
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace testing::ext;

class HatsTruncateTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsTruncateTest::SetUp()
{
}
void HatsTruncateTest::TearDown()
{
}
void HatsTruncateTest::SetUpTestCase()
{
}
void HatsTruncateTest::TearDownTestCase()
{
}

const char *TRUNCATE_TEST_FILE = "/data/local/tmp/tryTruncate.txt";
const char *TEST_DATA = "1234567890";
const int TEST_DATA_LEN = strlen(TEST_DATA);


/*
 * @tc.number : SUB_KERNEL_SYSCALL_TRUNCATE_0100
 * @tc.name   : TruncateFileSuccess_0001
 * @tc.desc   : Truncate file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsTruncateTest, TruncateFileSuccess_0001, Function | MediumTest | Level1)
{
    int fd;
    int ret;
    char *buf = nullptr;
    buf = new char[TEST_DATA_LEN / 2 + 1];

    fd = open(TRUNCATE_TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);
    ret = write(fd, TEST_DATA, TEST_DATA_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN);
    close(fd);

    ret = truncate(TRUNCATE_TEST_FILE, TEST_DATA_LEN / 2);
    EXPECT_TRUE(ret == 0);

    fd = open(TRUNCATE_TEST_FILE, O_RDONLY, 0644);
    EXPECT_TRUE(fd >= 3);
    ret = read(fd, buf, TEST_DATA_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN / 2);
    buf[TEST_DATA_LEN / 2] = '\0';
    ret = strcmp(buf, "12345");
    EXPECT_TRUE(ret == 0);
    close(fd);

    delete[] buf;
    remove(TRUNCATE_TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TRUNCATE_0200
 * @tc.name   : TruncateNotExistFileFail_0002
 * @tc.desc   : Truncate file that does not exist fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsTruncateTest, TruncateNotExistFileFail_0002, Function | MediumTest | Level2)
{
    int ret;
    errno = 0;
    ret = truncate(TRUNCATE_TEST_FILE, TEST_DATA_LEN / 2);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TRUNCATE_0300
 * @tc.name   : TruncateExtendFileSuccess_0003
 * @tc.desc   : Truncate extend file success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsTruncateTest, TruncateExtendFileSuccess_0003, Function | MediumTest | Level1)
{
    int fd;
    int ret;
    struct stat fileStat;
    ssize_t fileSize1;
    ssize_t fileSize2;

    fd = open(TRUNCATE_TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);
    ret = write(fd, TEST_DATA, TEST_DATA_LEN);
    EXPECT_TRUE(ret == TEST_DATA_LEN);
    close(fd);

    ret = stat(TRUNCATE_TEST_FILE, &fileStat);
    EXPECT_TRUE(ret == 0);
    fileSize1 = fileStat.st_size;

    ret = truncate(TRUNCATE_TEST_FILE, fileSize1 * 2);
    EXPECT_TRUE(ret == 0);

    ret = stat(TRUNCATE_TEST_FILE, &fileStat);
    EXPECT_TRUE(ret == 0);
    fileSize2 = fileStat.st_size;

    ret = (fileSize1 * 2 == fileSize2);
    EXPECT_TRUE(ret);
    remove(TRUNCATE_TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TRUNCATE_0400
 * @tc.name   : TruncateInvaliLenFail_0004
 * @tc.desc   : Truncate using invalid length fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsTruncateTest, TruncateInvaliLenFail_0004, Function | MediumTest | Level2)
{
    int ret;
    int truncateLen;

    truncateLen = -1;
    errno = 0;
    ret = truncate(TRUNCATE_TEST_FILE, truncateLen);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);
}