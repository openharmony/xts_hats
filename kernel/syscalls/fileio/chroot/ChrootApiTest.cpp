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

#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gtest/gtest.h>

using namespace testing::ext;
using namespace std;

static const char *INVALID_FILE_PATH = "/data/invalid";
static const char *TEST_FILE = "/data/local/tmp/chroot_file";
mode_t MODE_0755 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

class ChrootApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ChrootApiTest::SetUp()
{
}
void ChrootApiTest::TearDown()
{
}
void ChrootApiTest::SetUpTestCase()
{
}
void ChrootApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CHDIR_0200
 * @tc.name   : ChrootNullptrPathFailed_0002
 * @tc.desc   : Chroot nullptr failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ChrootApiTest, ChrootNullptrPathFailed_0002, Function | MediumTest | Level2)
{
    int ret;
    errno = 0;
    ret = chroot(nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CHDIR_0300
 * @tc.name   : ChrootInvalidPathFailed_0003
 * @tc.desc   : Chroot invalid dir failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ChrootApiTest, ChrootInvalidPathFailed_0003, Function | MediumTest | Level2)
{
    int ret;
    errno = 0;
    ret = chroot(INVALID_FILE_PATH);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CHDIR_0400
 * @tc.name   : ChrootFileFailed_0004
 * @tc.desc   : Chroot file failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ChrootApiTest, ChrootFileFailed_0004, Function | MediumTest | Level2)
{
    int ret;
    int fd;
    fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0755);
    close(fd);

    errno = 0;
    ret = chroot(TEST_FILE);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOTDIR);
    unlink(TEST_FILE);
}