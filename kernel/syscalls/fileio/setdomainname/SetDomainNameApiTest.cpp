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

static const char *TEST_DOMAIN_NAME = "domainname.com";
static const int DOMAIN_NAME_MAX_LEN = 256;
static const int MIN_LEN = 1;

class SetDomainNameApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
    char domainName[DOMAIN_NAME_MAX_LEN] = { 0 };
};
void SetDomainNameApiTest::SetUp()
{
    (void)getdomainname(domainName, sizeof(domainName));
}
void SetDomainNameApiTest::TearDown()
{
    (void)setdomainname(domainName, strlen(domainName));
}
void SetDomainNameApiTest::SetUpTestCase()
{
}
void SetDomainNameApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETDOMAINNAME_0100
 * @tc.name   : SetdomainnameChangeNameSuccess_0001
 * @tc.desc   : setdomainname change the system domainname success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(SetDomainNameApiTest, SetdomainnameChangeNameSuccess_0001, Function | MediumTest | Level1)
{
    char buffer[DOMAIN_NAME_MAX_LEN] = { 0 };
    int ret = setdomainname(TEST_DOMAIN_NAME, strlen(TEST_DOMAIN_NAME));
    EXPECT_EQ(ret, 0);
    ret = getdomainname(buffer, sizeof(buffer));
    EXPECT_EQ(ret, 0);
    EXPECT_STREQ(buffer, TEST_DOMAIN_NAME);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETDOMAINNAME_0200
 * @tc.name   : SetdomainnameUseInvalidLenFailed_0002
 * @tc.desc   : setdomainname use the length larger than 256 failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(SetDomainNameApiTest, SetdomainnameUseInvalidLenFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = setdomainname(TEST_DOMAIN_NAME, DOMAIN_NAME_MAX_LEN + 1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETDOMAINNAME_0300
 * @tc.name   : SetdomainnameUseInvalidNameFailed_0003
 * @tc.desc   : setdomainname use invalid name failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(SetDomainNameApiTest, SetdomainnameUseInvalidNameFailed_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = setdomainname(nullptr, strlen(TEST_DOMAIN_NAME));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETDOMAINNAME_0400
 * @tc.name   : GetdomainnameMinLenFailed_0004
 * @tc.desc   : getdomainname use too min len failed return EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(SetDomainNameApiTest, GetdomainnameMinLenFailed_0004, Function | MediumTest | Level2)
{
    int ret;
    char buffer[MIN_LEN] = { 0 };
    errno = 0;
    ret = getdomainname(buffer, sizeof(buffer));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}