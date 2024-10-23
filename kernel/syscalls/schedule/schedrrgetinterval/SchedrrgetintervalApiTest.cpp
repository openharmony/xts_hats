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
#include <ctime>
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

class HatsSchedrrgetintervalTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsSchedrrgetintervalTest::SetUp()
{
}
void HatsSchedrrgetintervalTest::TearDown()
{
}
void HatsSchedrrgetintervalTest::SetUpTestCase()
{
}
void HatsSchedrrgetintervalTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDRRGETINTERVAL_0100
 * @tc.name   : SchedrrgetintervalGetRRTSuccess_0001
 * @tc.desc   : sched_rr_get_interval get RR timespec of current thread success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSchedrrgetintervalTest, SchedrrgetintervalGetRRTSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    struct timespec timeSpec1;
    ret = sched_rr_get_interval(0, &timeSpec1);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDRRGETINTERVAL_0200
 * @tc.name   : SchedSchedrrgetintervalInvalidPIDFail_0002
 * @tc.desc   : sched_rr_get_interval get RR timespec of an invalid pid fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSchedrrgetintervalTest, SchedSchedrrgetintervalInvalidPIDFail_0002, Function | MediumTest | Level2)
{
    int ret;
    pid_t pid = -1;
    struct timespec timeSpec1;

    errno = 0;
    ret = sched_rr_get_interval(pid, &timeSpec1);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);
}
