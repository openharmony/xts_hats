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

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static time_t g_time;
static struct timespec g_timeTs;
static const int DELAY_TIME = 100;

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
    g_time = time(NULL);
    g_timeTs.tv_sec = g_time;
    g_timeTs.tv_nsec = 0;
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

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCKGETTIME_0100
 * @tc.name   : GetRealtimeClockSuccess_0001
 * @tc.desc   : Get the realtime clock successfully and compare with time function.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, GetRealtimeClockSuccess_0001, Function | MediumTest | Level1)
{
    struct timespec ts;
    ts.tv_sec = 0;
    int ret = clock_gettime(CLOCK_REALTIME, &ts);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(ts.tv_sec > 0);

    long diff = abs(ts.tv_sec - g_timeTs.tv_sec);
    EXPECT_TRUE(diff <= 1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCKGETTIME_0200
 * @tc.name   : ClockGetTimeGetRealTimeCoarseSuccess_0002
 * @tc.desc   : Get the realtime coarse clock successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, ClockGetTimeGetRealTimeCoarseSuccess_0002, Function | MediumTest | Level1)
{
    struct timespec ts;
    int ret = clock_gettime(CLOCK_REALTIME_COARSE, &ts);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(ts.tv_sec >= 0);

    long diff = abs(ts.tv_sec - g_timeTs.tv_sec);
    EXPECT_TRUE(diff <= 1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCKGETTIME_0300
 * @tc.name   : ClockGetTimeMonotonicSuccess_0003
 * @tc.desc   : Get the monotonic clock successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, ClockGetTimeMonotonicSuccess_0003, Function | MediumTest | Level1)
{
    struct timespec ts;
    struct timespec tsSec;
    int ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(ts.tv_sec >= 0);

    usleep(DELAY_TIME);
    clock_gettime(CLOCK_MONOTONIC, &tsSec);
    long diff = abs(ts.tv_sec - tsSec.tv_sec);
    EXPECT_TRUE(diff <= 1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCKGETTIME_0400
 * @tc.name   : ClockGetTimeMonotonicCoarseSuccess_0004
 * @tc.desc   : Get the monotonic coarse clock successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, ClockGetTimeMonotonicCoarseSuccess_0004, Function | MediumTest | Level1)
{
    struct timespec ts;
    struct timespec tsSec;
    int ret = clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(ts.tv_sec >= 0);

    usleep(DELAY_TIME);
    clock_gettime(CLOCK_MONOTONIC_COARSE, &tsSec);
    long diff = abs(ts.tv_sec - tsSec.tv_sec);
    EXPECT_TRUE(diff <= 10);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCKGETTIME_0500
 * @tc.name   : ClockGetTimeBoottimeSuccess_0005
 * @tc.desc   : Get the boottime clock successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, ClockGetTimeBoottimeSuccess_0005, Function | MediumTest | Level1)
{
    struct timespec ts;
    struct timespec tsSec;
    int ret = clock_gettime(CLOCK_BOOTTIME, &ts);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(ts.tv_sec >= 0);

    usleep(DELAY_TIME);
    clock_gettime(CLOCK_BOOTTIME, &tsSec);
    long diff = abs(ts.tv_sec - tsSec.tv_sec);
    EXPECT_TRUE(diff <= 10);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCKGETTIME_0600
 * @tc.name   : ClockGetTimeProcessCputimeSuccess_0006
 * @tc.desc   : Get the process cputime clock successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, ClockGetTimeProcessCputimeSuccess_0006, Function | MediumTest | Level1)
{
    struct timespec ts;
    struct timespec tsSec;
    int ret = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(ts.tv_sec >= 0);

    usleep(DELAY_TIME);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsSec);
    long diff = abs(ts.tv_sec - tsSec.tv_sec);
    EXPECT_TRUE(diff <= 1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCKGETTIME_0700
 * @tc.name   : ClockGetTimeThreadCputimeSuccess_0007
 * @tc.desc   : Get the thread cputime clock successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, ClockGetTimeThreadCputimeSuccess_0007, Function | MediumTest | Level1)
{
    struct timespec ts;
    struct timespec tsSec;
    int ret = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    EXPECT_TRUE(ret == 0);
    EXPECT_TRUE(ts.tv_sec >= 0);

    usleep(DELAY_TIME);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tsSec);
    long diff = abs(ts.tv_sec - tsSec.tv_sec);
    EXPECT_TRUE(diff <= 1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCKGETTIME_0800
 * @tc.name   : ClockSetTimeThreadCputimeSuccess_0008
 * @tc.desc   : Set time clock successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockApiTest, ClockSetTimeThreadCputimeSuccess_0008, Function | MediumTest | Level1)
{
    struct timespec newTime;
    time_t rawtime;

    int ret = time(&rawtime);
    EXPECT_NE(ret, -1);

    newTime.tv_sec = rawtime;
    newTime.tv_nsec = 0;

    ret = clock_settime(CLOCK_REALTIME, &newTime);
    EXPECT_EQ(ret, 0);
}

