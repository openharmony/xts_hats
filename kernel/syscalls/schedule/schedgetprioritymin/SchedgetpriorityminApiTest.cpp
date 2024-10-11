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
#include "securec.h"

using namespace testing::ext;

class HatsSchedgetpriorityminTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsSchedgetpriorityminTest::SetUp()
{
}
void HatsSchedgetpriorityminTest::TearDown()
{
}
void HatsSchedgetpriorityminTest::SetUpTestCase()
{
}
void HatsSchedgetpriorityminTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDGETPRIORITYMIN_0100
 * @tc.name   : SchedgetpriorityminOfPolicySuccess_0001
 * @tc.desc   : sched_get_priority_min of scheduling policy success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSchedgetpriorityminTest, SchedgetpriorityminOfPolicySuccess_0001, Function | MediumTest | Level1)
{
    int ret = sched_get_priority_min(SCHED_OTHER); // SCHED_OTHER is 0
    EXPECT_TRUE(ret >= 0);

    ret = sched_get_priority_min(SCHED_FIFO);
    EXPECT_TRUE(ret >= 0);

    ret = sched_get_priority_min(SCHED_RR);
    EXPECT_TRUE(ret >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDGETPRIORITYMIN_0200
 * @tc.name   : SchedgetpriorityminInvalidPolicyFail_0002
 * @tc.desc   : sched_get_priority_min of a invalid policy fail, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSchedgetpriorityminTest, SchedgetpriorityminInvalidPolicyFail_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = sched_get_priority_min(-1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}