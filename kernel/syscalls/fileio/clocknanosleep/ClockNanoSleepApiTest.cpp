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

using namespace testing::ext;
using namespace std;

class ClockNanosleepApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ClockNanosleepApiTest::SetUp()
{
}
void ClockNanosleepApiTest::TearDown()
{
}
void ClockNanosleepApiTest::SetUpTestCase()
{
}
void ClockNanosleepApiTest::TearDownTestCase()
{
}
void SignalHandler(int sig)
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCK_NANOSLEEP_0100
 * @tc.name   : ClockNanosleepBlockedSuccess_0001
 * @tc.desc   : Blocked by other progress.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockNanosleepApiTest, ClockNanosleepBlockedSuccess_0001, Function | MediumTest | Level1)
{
    struct timespec req = {1, 0};
    struct timespec rem;
    timer_t timerId;
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
    int ret = clock_nanosleep(CLOCK_REALTIME, 0, &req, &rem);

    timer_delete(timerId);

    EXPECT_EQ(ret, EINTR);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCK_NANOSLEEP_0200
 * @tc.name   : ClockNanosleepSuccess_0002
 * @tc.desc   : Clock nanosleep successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockNanosleepApiTest, ClockNanosleepSuccess_0002, Function | MediumTest | Level1)
{
    struct timespec req = {0, 100};

    int ret = clock_nanosleep(CLOCK_REALTIME, 0, &req, nullptr);
    EXPECT_EQ(ret, 0);
}