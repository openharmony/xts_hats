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

#include <iostream>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/timex.h>
#include <sys/times.h>
#include "securec.h"

using namespace testing::ext;

class HatsAdjTimexApiTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsAdjTimexApiTest::SetUp()
{
}
void HatsAdjTimexApiTest::TearDown()
{
}
void HatsAdjTimexApiTest::SetUpTestCase()
{
}
void HatsAdjTimexApiTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_ADJTIMEX_0100
 * @tc.name AdjTimexGetCurrentTimeSuccess_0001
 * @tc.desc Test the adjtimex get current time success.
 */
HWTEST_F(HatsAdjTimexApiTest, AdjTimexGetCurrentTimeSuccess_0001, Function | MediumTest | Level1)
{
    struct timex tx;
    tx.modes = 0;
    int result = adjtimex(&tx);
    EXPECT_GE(result, 0);

    struct timeval now;
    gettimeofday(&now, nullptr);

    EXPECT_NEAR(tx.time.tv_sec, now.tv_sec, 1);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_CLOCK_ADJTIMEX_0200
 * @tc.name ClockAdjTimexGetCurrentTimeSuccess_0002
 * @tc.desc Test the clock_adjtimex get current time success.
 */
HWTEST_F(HatsAdjTimexApiTest, ClockAdjTimexGetCurrentTimeSuccess_0002, Function | MediumTest | Level1)
{
    struct timex tx;
    tx.modes = 0;
    clockid_t clock_id = CLOCK_REALTIME;
    int result = clock_adjtime(clock_id, &tx);
    EXPECT_GE(result, 0);

    struct timespec now;
    clock_gettime(clock_id, &now);

    EXPECT_NEAR(tx.time.tv_sec, now.tv_sec, 1);
}