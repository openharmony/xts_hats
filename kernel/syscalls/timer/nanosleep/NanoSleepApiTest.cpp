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
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <gtest/gtest.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class ClockApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ClockApiTest::SetUp()
{
}
void ClockApiTest::TearDown()
{
}
void ClockApiTest::SetUpTestCase()
{
}
void ClockApiTest::TearDownTestCase()
{
}
void SignalHandler(int sig)
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_NANOSLEEP_0100
 * @tc.name   : NanoSleepBlockedSuccess_0001.
 * @tc.desc   : Blocked by other progress.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, NanoSleepBlockedSuccess_0001, Function | MediumTest | Level1)
{
    struct timespec req = {0, 100000000L};
    timer_t timerId;
    struct timespec rem;
    struct sigevent sev = {
        .sigev_signo = SIGALRM,
        .sigev_notify = SIGEV_SIGNAL,
    };
    struct itimerspec its = {
        .it_interval = {
            .tv_sec = 0,
            .tv_nsec = 0,
        },
        .it_value = {
            .tv_sec = 0,
            .tv_nsec = 50000,
        },
    };
    signal(SIGALRM, SignalHandler);
    timer_create(CLOCK_REALTIME, &sev, &timerId);

    timer_settime(timerId, 0, &its, nullptr);

    errno = 0;
    int result = nanosleep(&req, &rem);

    timer_delete(timerId);

    EXPECT_EQ(result, -1);
    EXPECT_EQ(errno, EINTR);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_NANOSLEEP_0200
 * @tc.name   : NanoSleepSuccess_0002
 * @tc.desc   : Nanosleep successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, NanoSleepSuccess_0002, Function | MediumTest | Level1)
{
    struct timespec req = {0, 100};
    struct timespec rem;

    int result = nanosleep(&req, &rem);
    EXPECT_EQ(result, 0);
}
