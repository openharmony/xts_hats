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
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>


using namespace testing::ext;

static const char *UTIMENSAT_TEST_FILE = "/data/local/tmp/tryUtimensat.txt";
static const int ACCESS_TIME_SEC = 10;
static const int ACCESS_TIME_NSEC = 0;
static const int MODIFICATION_TIME_SEC = 11;
static const int MODIFICATION_TIME_NSEC = 0;

class HatsUtimensatTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsUtimensatTest::SetUp()
{
}
void HatsUtimensatTest::TearDown()
{
}
void HatsUtimensatTest::SetUpTestCase()
{
}
void HatsUtimensatTest::TearDownTestCase()
{
    unlink(UTIMENSAT_TEST_FILE);
}


/*
 * @tc.number : SUB_KERNEL_SYSCALL_UTIMENSAT_0100
 * @tc.name   : UtimensatChangeFileTimeSuccess_0001
 * @tc.desc   : utimensat change file access time and modification time success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsUtimensatTest, UtimensatChangeFileTimeSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int fd;
    struct timespec newTime[2];
    struct stat fileStat1;
    struct stat fileStat2;

    fd = open(UTIMENSAT_TEST_FILE, O_RDONLY);
    if (fd != -1) {
        ret = remove(UTIMENSAT_TEST_FILE);
        EXPECT_TRUE(ret == 0);
    }
    close(fd);

    fd = open(UTIMENSAT_TEST_FILE, O_RDONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);
    close(fd);
    
    ret = stat(UTIMENSAT_TEST_FILE, &fileStat1);
    EXPECT_TRUE(ret == 0);

    //access time
    newTime[0].tv_sec = ACCESS_TIME_SEC;
    newTime[0].tv_nsec = ACCESS_TIME_NSEC;
    //modification time
    newTime[1].tv_sec = MODIFICATION_TIME_SEC;
    newTime[1].tv_nsec = MODIFICATION_TIME_NSEC;

    ret = utimensat(AT_FDCWD, UTIMENSAT_TEST_FILE, newTime, 0);
    EXPECT_TRUE(ret == 0);

    ret = stat(UTIMENSAT_TEST_FILE, &fileStat2);
    EXPECT_TRUE(ret == 0);

    ret = (fileStat1.st_atime != fileStat2.st_atime);
    EXPECT_TRUE(ret == 1);
    ret = (fileStat1.st_mtime != fileStat2.st_mtime);
    EXPECT_TRUE(ret == 1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UTIMENSAT_0200
 * @tc.name   : UtimensatInvalidTimeFail_0002
 * @tc.desc   : utimensat set time using invalid time value fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsUtimensatTest, UtimensatInvalidTimeFail_0002, Function | MediumTest | Level2)
{
    int ret;
    int fd;
    struct timespec newTime[2];
    struct stat fileStat1;

    fd = open(UTIMENSAT_TEST_FILE, O_RDONLY);
    if (fd != -1) {
        ret = remove(UTIMENSAT_TEST_FILE);
        EXPECT_TRUE(ret == 0);
    }
    close(fd);

    fd = open(UTIMENSAT_TEST_FILE, O_RDONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);
    close(fd);
    
    ret = stat(UTIMENSAT_TEST_FILE, &fileStat1);
    EXPECT_TRUE(ret == 0);

    //access time
    newTime[0].tv_sec = -1;
    newTime[0].tv_nsec = -1;
    //modification time
    newTime[1].tv_sec = -1;
    newTime[1].tv_nsec = -1;

    errno = 0;
    ret = utimensat(AT_FDCWD, UTIMENSAT_TEST_FILE, newTime, 0);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UTIMENSAT_0300
 * @tc.name   : UtimensatSetTimeOfNonexistFileFail_0003
 * @tc.desc   : utimensat set time of a non-exist file fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsUtimensatTest, UtimensatSetTimeOfNonexistFileFail_0003, Function | MediumTest | Level2)
{
    int ret;
    int fd;
    struct timespec newTime[2];

    fd = open(UTIMENSAT_TEST_FILE, O_RDONLY);
    if (fd != -1) {
        ret = remove(UTIMENSAT_TEST_FILE);
        EXPECT_TRUE(ret == 0);
    }
    close(fd);

    //access time
    newTime[0].tv_sec = ACCESS_TIME_SEC;
    newTime[0].tv_nsec = ACCESS_TIME_NSEC;
    //modification time
    newTime[1].tv_sec = MODIFICATION_TIME_SEC;
    newTime[1].tv_nsec = MODIFICATION_TIME_NSEC;

    errno = 0;
    ret = utimensat(AT_FDCWD, UTIMENSAT_TEST_FILE, newTime, 0);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UTIMENSAT_0400
 * @tc.name   : UtimensatInvalidFlagFail_0004
 * @tc.desc   : utimensat change time with invalid flag fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsUtimensatTest, UtimensatInvalidFlagFail_0004, Function | MediumTest | Level2)
{
    int ret;
    int fd;
    struct timespec newTime[2];

    fd = open(UTIMENSAT_TEST_FILE, O_RDONLY);
    if (fd != -1) {
        ret = remove(UTIMENSAT_TEST_FILE);
        EXPECT_TRUE(ret == 0);
    }
    close(fd);

    fd = open(UTIMENSAT_TEST_FILE, O_RDONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd >= 3);
    close(fd);

    //access time
    newTime[0].tv_sec = ACCESS_TIME_SEC;
    newTime[0].tv_nsec = ACCESS_TIME_NSEC;
    //modification time
    newTime[1].tv_sec = MODIFICATION_TIME_SEC;
    newTime[1].tv_nsec = MODIFICATION_TIME_NSEC;

    errno = 0;
    ret = utimensat(AT_FDCWD, UTIMENSAT_TEST_FILE, newTime, -1);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);
}
