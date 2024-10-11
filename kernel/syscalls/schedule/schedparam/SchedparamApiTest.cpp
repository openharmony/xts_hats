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

static const int TEST_PRIORITY = 1;

class HatsSchedparamTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsSchedparamTest::SetUp()
{
}
void HatsSchedparamTest::TearDown()
{
}
void HatsSchedparamTest::SetUpTestCase()
{
}
void HatsSchedparamTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDPARAM_0100
 * @tc.name   : SchedparamSetAndGetParametersSuccess_0001
 * @tc.desc   : sched_setparam set param and sched_setparam get priority param success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSchedparamTest, SchedparamSetAndGetParametersSuccess_0001, Function | MediumTest | Level1)
{
    struct sched_param param = {
        .sched_priority = TEST_PRIORITY,
    };

    int ret = sched_setscheduler(0, SCHED_FIFO, &param);

    // test sched_setparam set priority success
    param.sched_priority = SCHED_FIFO;
    ret = sched_setparam(0, &param);
    EXPECT_EQ(ret, 0);

    memset_s(&param, sizeof(struct sched_param), 0, sizeof(struct sched_param));

    // test sched_getparam get priority success
    ret = sched_getparam(0, &param);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(param.sched_priority, SCHED_FIFO);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SCHEDPARAM_0200
 * @tc.name   : SchedparamUseInvalidParmFailed_00002
 * @tc.desc   : sched_setparam and sched_getparam use invalid param failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSchedparamTest, SchedparamUseInvalidParmFailed_00002, Function | MediumTest | Level2)
{
    struct sched_param param = {
        .sched_priority = TEST_PRIORITY,
    };

    // test sched_setparam use pid = -1 failed, errno EINVAL
    errno = 0;
    int ret = sched_getparam(-1, &param);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    // test sched_getparam use pid = -1 pid failed, errno EINVAL
    errno = 0;
    ret = sched_getparam(-1, &param);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    // test sched_setparam use param nullptr failed, errno EINVAL
    errno = 0;
    ret = sched_setparam(0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    // test sched_getparam use param nullptr failed, errno EINVAL
    errno = 0;
    ret = sched_getparam(0, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}
