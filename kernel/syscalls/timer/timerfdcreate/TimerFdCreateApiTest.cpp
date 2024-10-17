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
#include <sys/timerfd.h>
#include "securec.h"

using namespace testing::ext;

class TimerFdCreateApiTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void TimerFdCreateApiTest::SetUp()
{
}
void TimerFdCreateApiTest::TearDown()
{
}
void TimerFdCreateApiTest::SetUpTestCase()
{
}
void TimerFdCreateApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMERFD_SETTIME_0100
 * @tc.name   : TimerfdSetTimeClockRealTimeSuccess_0001
 * @tc.desc   : Test the timerfd_settime with CLOCK_REALTIME.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerFdCreateApiTest, TimerfdSetTimeClockRealTimeSuccess_0001, Function | MediumTest | Level1)
{
    int timerfd = timerfd_create(CLOCK_REALTIME, 0);
    EXPECT_NE(timerfd, -1);

    struct itimerspec newValue;
    struct itimerspec oldValue;
    newValue.it_value.tv_sec = 0;
    newValue.it_value.tv_nsec = 100;
    newValue.it_interval.tv_sec = 0;

    EXPECT_EQ(timerfd_settime(timerfd, 0, &newValue, nullptr), 0);

    uint64_t expirations;
    EXPECT_EQ(read(timerfd, &expirations, sizeof(expirations)), sizeof(expirations));
    EXPECT_EQ(expirations, 1);

    int ret = timerfd_gettime(timerfd, &oldValue);
    EXPECT_GE(ret, 0);
    EXPECT_GE(newValue.it_value.tv_nsec, 0);

    close(timerfd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMERFD_SETTIME_0200
 * @tc.name   : TimerfdSetTimeClockMonotonicSuccess_0002
 * @tc.desc   : Test the timerfd_settime with CLOCK_MONOTONIC.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerFdCreateApiTest, TimerfdCreateClockMonotonicSuccess_0002, Function | MediumTest | Level1)
{
    int timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
    EXPECT_NE(timerfd, -1);

    struct itimerspec newValue;
    newValue.it_value.tv_nsec = 100;
    newValue.it_interval.tv_nsec = 0;

    EXPECT_EQ(timerfd_settime(timerfd, 0, &newValue, nullptr), 0);

    uint64_t expirations;
    EXPECT_EQ(read(timerfd, &expirations, sizeof(expirations)), sizeof(expirations));
    EXPECT_EQ(expirations, 1);

    close(timerfd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMERFD_SETTIME_0300
 * @tc.name   : TimerfdSetTimeClockBoottimeSuccess_0003
 * @tc.desc   : Test the timerfd_settime with CLOCK_BOOTTIME.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerFdCreateApiTest, TimerfdSetTimeClockBoottimeSuccess_0003, Function | MediumTest | Level1)
{
    int timerfd = timerfd_create(CLOCK_BOOTTIME, 0);
    EXPECT_NE(timerfd, -1);

    struct itimerspec newValue;
    newValue.it_value.tv_sec = 0;
    newValue.it_value.tv_nsec = 100;
    newValue.it_interval.tv_sec = 0;

    EXPECT_EQ(timerfd_settime(timerfd, 0, &newValue, nullptr), 0);

    uint64_t expirations;
    EXPECT_EQ(read(timerfd, &expirations, sizeof(expirations)), sizeof(expirations));
    EXPECT_EQ(expirations, 1);

    close(timerfd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMERFD_SETTIME_0400
 * @tc.name   : TimerfdSetTimeClockRealTimeAlarmSuccess_0004
 * @tc.desc   : Test the timerfd_settime with CLOCK_REALTIME_ALARM.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerFdCreateApiTest, TimerfdSetTimeClockRealTimeAlarmSuccess_0004, Function | MediumTest | Level1)
{
    int timerfd = timerfd_create(CLOCK_REALTIME_ALARM, 0);
    EXPECT_NE(timerfd, -1);

    struct itimerspec newValue;
    newValue.it_value.tv_sec = 0;
    newValue.it_value.tv_nsec = 100;
    newValue.it_interval.tv_nsec = 0;

    EXPECT_EQ(timerfd_settime(timerfd, 0, &newValue, nullptr), 0);

    uint64_t expirations;
    EXPECT_EQ(read(timerfd, &expirations, sizeof(expirations)), sizeof(expirations));
    EXPECT_EQ(expirations, 1);

    close(timerfd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMERFD_SETTIME_0500
 * @tc.name   : TimerfdSetTimeClockBoottimeAlarmSuccess_0005
 * @tc.desc   : Test the timerfd_settime with CLOCK_BOOTTIME_ALARM.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerFdCreateApiTest, TimerfdSetTimeClockBoottimeAlarmSuccess_0005, Function | MediumTest | Level1)
{
    int timerfd = timerfd_create(CLOCK_BOOTTIME_ALARM, 0);
    EXPECT_NE(timerfd, -1);

    struct itimerspec newValue;
    newValue.it_value.tv_nsec = 500;
    newValue.it_interval.tv_nsec = 0;

    EXPECT_EQ(timerfd_settime(timerfd, 0, &newValue, nullptr), 0);

    uint64_t expirations;
    EXPECT_EQ(read(timerfd, &expirations, sizeof(expirations)), sizeof(expirations));
    EXPECT_EQ(expirations, 1);

    close(timerfd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMERFD_CREATE_0600
 * @tc.name   : TimerfdCreateTFdNonBlockTest_0006
 * @tc.desc   : Test the timerfd_create with TFD_NONBLOCK.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(TimerFdCreateApiTest, TimerfdCreateTFdNonBlockTest_0006, Function | MediumTest | Level2)
{
    int timerfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    EXPECT_NE(timerfd, -1);
    errno = 0;

    struct itimerspec newValue;
    newValue.it_value.tv_sec = 0;
    newValue.it_value.tv_nsec = 10000;
    newValue.it_interval.tv_sec = 0;

    EXPECT_EQ(timerfd_settime(timerfd, 0, &newValue, nullptr), 0);

    uint64_t expirations;
    ssize_t result = read(timerfd, &expirations, sizeof(expirations));
    EXPECT_EQ(result, -1);
    EXPECT_EQ(errno, EAGAIN);

    close(timerfd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMERFD_SETTIME_0700
 * @tc.name   : TimerfdSetTimeTFdTimerAbstimeSuccess_0007
 * @tc.desc   : Test the timerfd_settime with TFD_TIMER_ABSTIME.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(TimerFdCreateApiTest, TimerfdSetTimeTFdTimerAbstimeSuccess_0007, Function | MediumTest | Level1)
{
    int timerfd = timerfd_create(CLOCK_REALTIME, 0);
    EXPECT_NE(timerfd, -1);

    struct itimerspec newValue;
    newValue.it_value.tv_sec = 1;
    newValue.it_value.tv_nsec = 0;
    newValue.it_interval.tv_sec = 0;

    EXPECT_EQ(timerfd_settime(timerfd, TFD_TIMER_ABSTIME, &newValue, nullptr), 0);

    uint64_t expirations;
    EXPECT_EQ(read(timerfd, &expirations, sizeof(expirations)), sizeof(expirations));
    EXPECT_EQ(expirations, 1);

    close(timerfd);
}