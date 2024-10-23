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

static const int TEST_PRIO = 5;

class HatsPriorityTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsPriorityTest::SetUp()
{
}
void HatsPriorityTest::TearDown()
{
}
void HatsPriorityTest::SetUpTestCase()
{
}
void HatsPriorityTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRIORITY_0100
 * @tc.name   : PrioritySetAndGetTestSuccess_0001
 * @tc.desc   : priority set and get test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPriorityTest, PrioritySetAndGetTestSuccess_0001, Function | MediumTest | Level1)
{
    int pid = 0;
    int ret = setpriority(PRIO_PROCESS, pid, TEST_PRIO);
    EXPECT_EQ(ret, 0);

    int prio = getpriority(PRIO_PROCESS, pid);
    EXPECT_EQ(prio, TEST_PRIO);

    ret = setpriority(PRIO_PGRP, pid, TEST_PRIO);
    EXPECT_EQ(ret, 0);

    prio = getpriority(PRIO_PGRP, pid);
    EXPECT_EQ(prio, TEST_PRIO);

    ret = setpriority(PRIO_USER, getuid(), TEST_PRIO);
    EXPECT_EQ(ret, 0);

    prio = getpriority(PRIO_USER, getuid());
    EXPECT_EQ(prio, TEST_PRIO);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRIORITY_0200
 * @tc.name   : PrioritySetAndGetUseInvalidWhichFailed_0002
 * @tc.desc   : setpriority and getpriority use invalid which test failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPriorityTest, PrioritySetAndGetUseInvalidWhichFailed_0002, Function | MediumTest | Level2)
{
    int pid = getpid();
    errno = 0;
    int ret = setpriority(-1, pid, TEST_PRIO);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    int prio = getpriority(-1, pid);
    EXPECT_EQ(prio, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRIORITY_0300
 * @tc.name   : PrioritySetAndGetUseInvalidWhoFailed_0003
 * @tc.desc   : setpriority and getpriority use invalid Who test failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPriorityTest, PrioritySetAndGetUseInvalidWhichFailed_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = setpriority(PRIO_PROCESS, -1, TEST_PRIO);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESRCH);

    errno = 0;
    int prio = getpriority(PRIO_PROCESS, -1);
    EXPECT_EQ(prio, -1);
    EXPECT_EQ(errno, ESRCH);
}
