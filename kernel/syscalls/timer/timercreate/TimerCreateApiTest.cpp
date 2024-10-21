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
static bool g_sigalRm = false;
static const int DELAY_TIME = 100;

void TimeHandler(union sigval sv)
{
    g_timerExpired = true;
}
void SigalrmHandler(int sig)
{
    g_sigalRm = true;
}

class TimerCreateApiTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void TimerCreateApiTest::SetUp()
{
    g_timerExpired = false;
    g_sigalRm = false;
}
void TimerCreateApiTest::TearDown()
{
    sighandler_t oldHandler = signal(SIGRTMIN, SIG_DFL);
    EXPECT_NE(oldHandler, SIG_ERR);
}
void TimerCreateApiTest::SetUpTestCase()
{
}
void TimerCreateApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMER_CREATE_0100
 * @tc.name   : TimerCreateRealtimeAndGetoverrunSuccess_0001
 * @tc.desc   : Test the timer_create and timer_getoverrun with CLOCK_REALTIME.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerCreateApiTest, TimerCreateRealtimeAndGetoverrunSuccess_0001, Function | MediumTest | Level1)
{
    timer_t timerId;
    struct sigevent sev;
    const clockid_t CLOCK_ID = CLOCK_REALTIME;
    struct itimerspec its = {
        .it_interval = {
            .tv_sec = 0,
            .tv_nsec = 4000,
        },
        .it_value = {
            .tv_sec = 0,
            .tv_nsec = 5000,
        },
    };

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_notify_function = TimeHandler;

    EXPECT_EQ(timer_create(CLOCK_ID, &sev, &timerId), 0);
    EXPECT_EQ(timer_settime(timerId, 0, &its, nullptr), 0);

    usleep(DELAY_TIME);
    EXPECT_TRUE(g_timerExpired);

    int overruns = timer_getoverrun(timerId);
    EXPECT_TRUE(overruns >= 0);

    int timerRes = timer_delete(timerId);
    EXPECT_EQ(timerRes, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMER_CREATE_0200
 * @tc.name   : TimerCreateMonotonicSuccess_0002
 * @tc.desc   : Test the timer_create with CLOCK_MONOTONIC.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerCreateApiTest, TimerCreateMonotonicSuccess_0002, Function | MediumTest | Level1)
{
    timer_t timerId;
    struct itimerspec currValue;
    const clockid_t CLOCK_ID = CLOCK_MONOTONIC;
    signal(SIGALRM, SigalrmHandler);
    struct itimerspec its = {
        .it_interval = {
            .tv_sec = 0,
            .tv_nsec = 0,
        },
        .it_value = {
            .tv_sec = 0,
            .tv_nsec = 5000,
        },
    };

    struct sigevent sev = {
        .sigev_signo = SIGALRM,
        .sigev_notify = SIGEV_SIGNAL,
        .sigev_notify_function = TimeHandler,
    };

    EXPECT_EQ(timer_create(CLOCK_ID, &sev, &timerId), 0);
    EXPECT_EQ(timer_settime(timerId, 0, &its, nullptr), 0);

    usleep(DELAY_TIME);
    EXPECT_EQ(timer_gettime(timerId, &currValue), 0);
    EXPECT_TRUE(currValue.it_value.tv_sec < 1 && currValue.it_value.tv_nsec < 5000);

    int timerRes = timer_delete(timerId);
    EXPECT_EQ(timerRes, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMER_CREATE_0300
 * @tc.name   : TimerCreateRealTimeAlarmSuccess_0003
 * @tc.desc   : Test the timer_create with CLOCK_REALTIME_ALARM.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerCreateApiTest, TimerCreateRealTimeAlarmSuccess_0003, Function | MediumTest | Level1)
{
    timer_t timerId;
    const clockid_t CLOCK_ID = CLOCK_REALTIME_ALARM;
    sighandler_t oldHandler = signal(SIGALRM, SigalrmHandler);
    EXPECT_NE(oldHandler, SIG_ERR);
    struct sigevent sev = {
        .sigev_signo = SIGALRM,
        .sigev_notify = SIGEV_SIGNAL,
        .sigev_notify_function = TimeHandler,
    };
    struct itimerspec its = {
        .it_interval = {
            .tv_sec = 0,
            .tv_nsec = 0,
        },
        .it_value = {
            .tv_sec = 0,
            .tv_nsec = 50,
        },
    };

    EXPECT_EQ(timer_create(CLOCK_ID, &sev, &timerId), 0);
    EXPECT_EQ(timer_settime(timerId, 0, &its, nullptr), 0);

    usleep(DELAY_TIME);

    EXPECT_FALSE(g_timerExpired);
    EXPECT_TRUE(g_sigalRm);

    int timerRes = timer_delete(timerId);
    EXPECT_EQ(timerRes, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMER_CREATE_0400
 * @tc.name   : TimeSetTimeOldValueSuccess_0004
 * @tc.desc   : Test the timer_settime get the old value.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerCreateApiTest, TimeSetTimeOldValueSuccess_0004, Function | MediumTest | Level1)
{
    timer_t timerId;
    struct itimerspec oldValue;
    struct itimerspec oldValue2;
    struct itimerspec newValue = {
        .it_interval = {
            .tv_sec = 1,
            .tv_nsec = 0,
        },
        .it_value = {
            .tv_sec = 0,
            .tv_nsec = 50,
        },
    };
    EXPECT_EQ(timer_create(CLOCK_REALTIME, nullptr, &timerId), 0);

    EXPECT_EQ(timer_settime(timerId, 0, &newValue, &oldValue), 0);
    EXPECT_EQ(oldValue.it_value.tv_sec, 0);
    EXPECT_EQ(oldValue.it_value.tv_nsec, 0);
    EXPECT_EQ(oldValue.it_interval.tv_sec, 0);
    EXPECT_EQ(oldValue.it_interval.tv_nsec, 0);

    newValue.it_value.tv_sec = 0;
    newValue.it_value.tv_nsec = 50;
    newValue.it_interval.tv_sec = 1;
    newValue.it_interval.tv_nsec = 0;
    EXPECT_EQ(timer_settime(timerId, 0, &newValue, &oldValue2), 0);
    EXPECT_EQ(oldValue2.it_interval.tv_sec, 1);
    EXPECT_EQ(oldValue2.it_interval.tv_nsec, 0);

    int timerRes = timer_delete(timerId);
    EXPECT_EQ(timerRes, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMER_SETTIME_0500
 * @tc.name   : TimerSetTimeAbstimeSuccess_0005
 * @tc.desc   : Test the timer_settime with TIMER_ABSTIME.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerCreateApiTest, TimerSetTimeAbstimeSuccess_0005, Function | MediumTest | Level1)
{
    timer_t timerId;
    struct itimerspec newValue;
    struct itimerspec oldValue;
    const clockid_t CLOCK_ID = CLOCK_REALTIME;

    // Set the timer current time plus 50 nsec
    struct timespec now;
    clock_gettime(CLOCK_ID, &now);
    newValue.it_value.tv_sec = now.tv_sec;
    newValue.it_value.tv_nsec = now.tv_nsec + 50;
    if (newValue.it_value.tv_nsec >= 1000000000) {
        newValue.it_value.tv_nsec -= 1000000000;
        newValue.it_value.tv_sec += 1;
    }
    newValue.it_interval.tv_sec = 0;
    newValue.it_interval.tv_nsec = 0;

    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = TimeHandler;
    sev.sigev_value.sival_ptr = &g_timerExpired;

    EXPECT_EQ(timer_create(CLOCK_ID, &sev, &timerId), 0);

    EXPECT_EQ(timer_settime(timerId, TIMER_ABSTIME, &newValue, &oldValue), 0);

    usleep(DELAY_TIME);
    EXPECT_TRUE(g_timerExpired);

    int timerRes = timer_delete(timerId);
    EXPECT_EQ(timerRes, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMER_CREATE_0600
 * @tc.name   : TimerCreateBoottimeAlarmSuccess_0006
 * @tc.desc   : Test the timer_create with CLOCK_BOOTTIME_ALARM.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerCreateApiTest, TimerCreateBoottimeAlarmSuccess_0006, Function | MediumTest | Level1)
{
    timer_t timerId;
    const clockid_t CLOCK_ID = CLOCK_BOOTTIME_ALARM;
    sighandler_t oldHandler = signal(SIGALRM, SigalrmHandler);
    EXPECT_NE(oldHandler, SIG_ERR);
    struct sigevent sev = {
        .sigev_notify = SIGEV_SIGNAL,
        .sigev_signo = SIGALRM,
    };
    struct itimerspec its = {
        .it_interval = {
            .tv_sec = 0,
            .tv_nsec = 0,
        },
        .it_value = {
            .tv_sec = 0,
            .tv_nsec = 50,
        },
    };

    EXPECT_EQ(timer_create(CLOCK_ID, &sev, &timerId), 0);
    EXPECT_EQ(timer_settime(timerId, 0, &its, nullptr), 0);

    usleep(DELAY_TIME);

    EXPECT_TRUE(g_sigalRm);

    int timerRes = timer_delete(timerId);
    EXPECT_EQ(timerRes, 0);

    signal(SIGALRM, oldHandler);
}
