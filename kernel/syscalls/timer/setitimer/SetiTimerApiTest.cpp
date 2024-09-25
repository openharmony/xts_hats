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

#include <csignal>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <gtest/gtest.h>
#include "securec.h"

using namespace testing::ext;

static bool g_timerExpired = false;
static const int DELAY_TIME = 100;

void TimerHandler(int sig)
{
    g_timerExpired = true;
}

class SetiTimerApiTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void SetiTimerApiTest::SetUp()
{
    g_timerExpired = false;
    sighandler_t oldHandler = signal(SIGALRM, TimerHandler);
    EXPECT_NE(oldHandler, SIG_ERR);
}
void SetiTimerApiTest::TearDown()
{
    sighandler_t oldHandler = signal(SIGALRM, SIG_DFL);
    EXPECT_NE(oldHandler, SIG_ERR);
}
void SetiTimerApiTest::SetUpTestCase()
{
}
void SetiTimerApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETITIMER_0100
 * @tc.name   : SetiTimerGetRealTimeSuccess_0001
 * @tc.desc   : Test the ITIMER_REAL timer.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(SetiTimerApiTest, SetiTimerGetRealTimeSuccess_0001, Function | MediumTest | Level1)
{
    struct itimerval timer_val = {
        .it_interval = {
            .tv_sec = 0,
            .tv_usec = 0,
        },
        .it_value = {
            .tv_sec = 0,
            .tv_usec = 50,
        },
    };
    struct itimerval remaining;

    EXPECT_EQ(setitimer(ITIMER_REAL, &timer_val, &remaining), 0);

    usleep(DELAY_TIME);
    EXPECT_TRUE(g_timerExpired);
    EXPECT_EQ(signal(SIGALRM, SIG_DFL), TimerHandler);
}
