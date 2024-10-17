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

class UnameApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void UnameApiTest::SetUp()
{
}
void UnameApiTest::TearDown()
{
}
void UnameApiTest::SetUpTestCase()
{
}
void UnameApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UNAME_0100
 * @tc.name   : UnameAcquireInformationSuccess_0001
 * @tc.desc   : Uname acquire system information in the valid struct success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UnameApiTest, UnameAcquireInformationSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    struct utsname buffer;
    ret = uname(&buffer);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_UNAME_0200
 * @tc.name   : UnameAcquireInformationFail_0002
 * @tc.desc   : Uname acquire system information in the invalid struct fail, errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UnameApiTest, UnameAcquireInformationFail_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = uname(nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}
