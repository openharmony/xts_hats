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
#include <csignal>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>
#include <syscall.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <linux/futex.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include "securec.h"

using namespace testing::ext;

static const int INVALID_FLAGS = -1;

class HatsGetrobustlistTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsGetrobustlistTest::SetUp()
{
}

void HatsGetrobustlistTest::TearDown()
{
}

void HatsGetrobustlistTest::SetUpTestCase()
{
}

void HatsGetrobustlistTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETROBUSTLIST_0100
 * @tc.name   : GetrobustlistSuccess_0001
 * @tc.desc   : Get robust_list success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsGetrobustlistTest, GetrobustlistSuccess_0001, Function | MediumTest | Level1)
{
    struct robust_list_head head;
    int len = sizeof(struct robust_list_head);
    int ret = syscall(__NR_get_robust_list, 0, &head, &len);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETROBUSTLIST_0200
 * @tc.name   : GetrobustlistInvalidFlagFailed_0002
 * @tc.desc   : Get robust_list invalid Flags failed errno ESRCH.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsGetrobustlistTest, GetrobustlistInvalidFlagFailed_0002, Function | MediumTest | Level2)
{
    struct robust_list_head head;
    int len = sizeof(struct robust_list_head);
    errno = 0;
    int ret = syscall(__NR_get_robust_list, INVALID_FLAGS, &head, &len);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESRCH);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETROBUSTLIST_0300
 * @tc.name   : GetrobustlistNullptrFailed_0003
 * @tc.desc   : Get robust_list nullptr failed errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsGetrobustlistTest, GetrobustlistNullptrFailed_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = syscall(__NR_get_robust_list, 0, nullptr, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}