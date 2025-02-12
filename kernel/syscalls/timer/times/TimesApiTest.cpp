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
#include <sys/times.h>
#include "securec.h"
#include <ctime>
using namespace testing::ext;
class HatsTimesApiTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsTimesApiTest::SetUp()
{
}
void HatsTimesApiTest::TearDown()
{
}
void HatsTimesApiTest::SetUpTestCase()
{
}
void HatsTimesApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TIMES_0100
 * @tc.name   : TimesBasicSuccess_0001
 * @tc.desc   : Test the times basic functionality.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsTimesApiTest, TimesBasicSuccess_0001, Function | MediumTest | Level1)
{
    struct tms tms1, tms2;
    clock_t startTime = 0;
    int counts = 30000000;
    int syscounts = 100000;
    double threshold = 1.0;
    double duration = 0;
    clock_t init = 0;
    clock_t start = times(&tms1);
    EXPECT_GE(start, init);
    clock_t end = 0;
    volatile int j = 0;
    startTime = clock();
    do {
        for (volatile int i = 0; i < counts; i++) {
            j++;
        }
        for (int k = 0; k < syscounts; k++) {
            end = times(&tms2);
        }
        
        if (tms2.tms_utime > tms1.tms_utime && tms2.tms_stime > tms1.tms_stime) {
            break;
        }
    } while ((duration = static_cast<double>(clock() - startTime) / CLOCKS_PER_SEC) < threshold);
    EXPECT_GT(end, start);
    EXPECT_GT(tms2.tms_utime, tms1.tms_utime);
    EXPECT_GT(tms2.tms_stime, tms1.tms_stime);
}