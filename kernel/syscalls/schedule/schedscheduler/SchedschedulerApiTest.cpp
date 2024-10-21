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
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

static const int TEST_INVALID_POLICY = 8888;

class HatsSchedschedulerTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsSchedschedulerTest::SetUp()
{
}
void HatsSchedschedulerTest::TearDown()
{
}
void HatsSchedschedulerTest::SetUpTestCase()
{
}
void HatsSchedschedulerTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDGSCHEDULER_0100
 * @tc.name   : SchedschedulerSetAndGetSchedulingPolicySuccess_0001
 * @tc.desc   : sched_setscheduler and sched_getscheduler scheduling policy of current thread success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSchedschedulerTest, SchedschedulerSetAndGetSchedulingPolicySuccess_0001, Function | MediumTest | Level1)
{
    struct sched_param param = { 0 };

    // scheduler SCHED_FIFO set and get test, success
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    int ret = sched_setscheduler(0, SCHED_FIFO, &param);
    EXPECT_EQ(ret, 0);

    int policy = sched_getscheduler(0);
    EXPECT_EQ(policy, SCHED_FIFO);

    // scheduler SCHED_RR set and get test, success
    param.sched_priority = sched_get_priority_max(SCHED_RR);
    ret = sched_setscheduler(0, SCHED_RR, &param);
    EXPECT_EQ(ret, 0);

    policy = sched_getscheduler(0);
    EXPECT_EQ(policy, SCHED_RR);

    // scheduler SCHED_OTHER set and get test, success
    param.sched_priority = sched_get_priority_max(SCHED_OTHER);
    ret = sched_setscheduler(0, SCHED_OTHER, &param);
    EXPECT_EQ(ret, 0);

    policy = sched_getscheduler(0);
    EXPECT_EQ(policy, SCHED_OTHER);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDGSCHEDULER_0200
 * @tc.name   : SchedschedulerUseInvalidPidFailed_0002
 * @tc.desc   : sched_getscheduler and sched_setscheduler use invalid pid failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSchedschedulerTest, SchedschedulerUseInvalidParamFailed_0002, Function | MediumTest | Level2)
{
    struct sched_param param = { 0 };

    param.sched_priority = sched_get_priority_max(SCHED_FIFO);

    // negative pid test, errno EINVAL
    errno = 0;
    int ret = sched_setscheduler(-1, SCHED_FIFO, &param);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    int policy = sched_getscheduler(-1);
    EXPECT_EQ(policy, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDGSCHEDULER_0300
 * @tc.name   : SchedschedulerUseInvalidPolicyFailed_0003
 * @tc.desc   : sched_setscheduler use invalid Policy failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSchedschedulerTest, SchedschedulerUseInvalidPolicyFailed_0003, Function | MediumTest | Level2)
{
    struct sched_param param = { 0 };
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);

    errno = 0;
    int ret = sched_setscheduler(0, TEST_INVALID_POLICY, &param);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDGSCHEDULER_0400
 * @tc.name   : SchedschedulerUseInvalidParamFailed_0004
 * @tc.desc   : sched_setscheduler use nullptr param failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSchedschedulerTest, SchedschedulerUseInvalidParamFailed_0004, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = sched_setscheduler(0, SCHED_FIFO, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}