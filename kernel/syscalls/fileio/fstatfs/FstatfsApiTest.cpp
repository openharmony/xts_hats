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
#include <sys/statfs.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class FstatfsApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FstatfsApiTest::SetUp()
{
}
void FstatfsApiTest::TearDown()
{
}
void FstatfsApiTest::SetUpTestCase()
{
}
void FstatfsApiTest::TearDownTestCase()
{
}

static const char* OPEN_API_TEST_FILE = "/data/local/tmp";

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSTATFS_0100
 * @tc.name   : FstatfsValidFdSuccess_0001
 * @tc.desc   : fstatfs valid fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FstatfsApiTest, OpenAndGetStateFileSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    struct statfs fs = { 0 };

    fd = open(OPEN_API_TEST_FILE, O_RDONLY);
    EXPECT_TRUE(fd > 0);

    ret = fstatfs(fd, &fs);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(fs.f_type > 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSTATFS_0200
 * @tc.name   : FstatfsUseInvalidFdFailed_0002
 * @tc.desc   : fstatfs use invalid fd failed, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FstatfsApiTest, FstatfsUseInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    int ret;
    int fd = -1;
    struct statfs fs = { 0 };

    errno = 0;
    ret = fstatfs(fd, &fs);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    close(fd);
}
