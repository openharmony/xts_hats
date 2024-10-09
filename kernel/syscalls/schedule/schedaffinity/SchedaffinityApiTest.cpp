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

class HatsSchedaffinityTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsSchedaffinityTest::SetUp()
{
}
void HatsSchedaffinityTest::TearDown()
{
}
void HatsSchedaffinityTest::SetUpTestCase()
{
}
void HatsSchedaffinityTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDAFFINITY_0100
 * @tc.name   : SchedaffinitySetAndGetCurrentThreadSuccess_0001
 * @tc.desc   : sched_setaffinity CPU 0 to current thread and sched_getaffinity success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSchedaffinityTest, SchedsetaffinityOfCurrentThreadSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    pid_t pid = 0;
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    ret = sched_setaffinity(pid, sizeof(cpu_set_t), &set);
    EXPECT_EQ(ret, 0);

    ret = sched_getaffinity(pid, sizeof(cpu_set_t), &set);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDAFFINITY_0200
 * @tc.name   : SchedsetaffinityInvalidCPUFail_0002
 * @tc.desc   : sched_setaffinity use invalid CPU to current thread fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSchedaffinityTest, SchedsetaffinityInvalidCPUFail_0002, Function | MediumTest | Level2)
{
    int ret;
    pid_t pid = 0;
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(-1, &set);
    errno = 0;
    ret = sched_setaffinity(pid, sizeof(cpu_set_t), &set);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDAFFINITY_0300
 * @tc.name   : SchedaffinitySetAndGetUseInvalidPIDFail_0003
 * @tc.desc   : sched_setaffinity and sched_getaffinity use invalid PID fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSchedaffinityTest, SchedaffinitySetAndGetUseInvalidPIDFail_0003, Function | MediumTest | Level2)
{
    int ret;
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    // pid = -1 test
    pid_t pid = -1;
    errno = 0;
    ret = sched_setaffinity(pid, sizeof(cpu_set_t), &set);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESRCH);

    errno = 0;
    ret = sched_getaffinity(pid, sizeof(cpu_set_t), &set);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESRCH);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDAFFINITY_0400
 * @tc.name   : SchedaffinitySetAndGetUseInvalidCPUSetFail_0004
 * @tc.desc   : sched_setaffinity and sched_getaffinity use invalid cpu_set fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSchedaffinityTest, SchedaffinitySetAndGetUseInvalidCPUSetFail_0004, Function | MediumTest | Level2)
{
    int ret;
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    // invalid size test
    errno = 0;
    ret = sched_setaffinity(0, 0, &set);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    ret = sched_getaffinity(0, 0, &set);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    // invalid cpu_set_t test
    errno = 0;
    ret = sched_setaffinity(0, sizeof(cpu_set_t), nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);

    errno = 0;
    ret = sched_getaffinity(0, sizeof(cpu_set_t), nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}