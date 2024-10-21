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
#include <gtest/gtest.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/xattr.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *TEST_HOST_NAME = "TestHostName";
static const int HOST_NAME_MAX_LEN = 256;

class SetHostNameApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
    char hostName[HOST_NAME_MAX_LEN] = { 0 };
};
void SetHostNameApiTest::SetUp()
{
    (void)gethostname(hostName, sizeof(hostName));
}
void SetHostNameApiTest::TearDown()
{
    (void)sethostname(hostName, sizeof(hostName));
}
void SetHostNameApiTest::SetUpTestCase()
{
}
void SetHostNameApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETHOSTNAME_0100
 * @tc.name   : SethostnameChangeHostNameSuccess_0001
 * @tc.desc   : sethostname change the system hostname success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(SetHostNameApiTest, SethostnameChangeHostNameSuccess_0001, Function | MediumTest | Level1)
{
    char buffer[HOST_NAME_MAX_LEN] = { 0 };
    int ret = sethostname(TEST_HOST_NAME, strlen(TEST_HOST_NAME));
    EXPECT_EQ(ret, 0);
    ret = gethostname(buffer, sizeof(buffer));
    EXPECT_EQ(ret, 0);
    EXPECT_STREQ(buffer, TEST_HOST_NAME);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETHOSTNAME_0200
 * @tc.name   : SethostnameUseInvalidLenFailed_0002
 * @tc.desc   : sethostname use the length larger than 256 failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(SetHostNameApiTest, SethostnameUseInvalidLenFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = sethostname(TEST_HOST_NAME, HOST_NAME_MAX_LEN + 1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETHOSTNAME_0300
 * @tc.name   : SethostnameUseInvalidNameFailed_0003
 * @tc.desc   : sethostname use invalid name failed, errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(SetHostNameApiTest, SethostnameUseInvalidNameFailed_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = sethostname(nullptr, strlen(TEST_HOST_NAME));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}
