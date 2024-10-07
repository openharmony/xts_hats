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

class ClockGetresApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ClockGetresApiTest::SetUp()
{
}
void ClockGetresApiTest::TearDown()
{
}
void ClockGetresApiTest::SetUpTestCase()
{
}
void ClockGetresApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCK_GETRES_0100
 * @tc.name   : ClockGetresInvalidClockId_0001
 * @tc.desc   : Test clock_getres with an invalid clock_id.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ClockGetresApiTest, ClockGetresInvalidClockId_0001, Function | MediumTest | Level2)
{
    struct timespec res;
    clockid_t invalidClockId = -1;
    int ret = clock_getres(invalidClockId, &res);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CLOCK_GETRES_0200
 * @tc.name   : ClockGetresSuccess_0002
 * @tc.desc   : Test clock_getres successfully gets the resolution of a valid clock.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ClockGetresApiTest, ClockGetresSuccess_0002, Function | MediumTest | Level1)
{
    struct timespec res = { 0 };
    clockid_t validClockId = CLOCK_REALTIME;

    int ret = clock_getres(validClockId, &res);
    EXPECT_EQ(ret, 0);

    EXPECT_GE(res.tv_sec, 0);
    EXPECT_GE(res.tv_nsec, 0);
}