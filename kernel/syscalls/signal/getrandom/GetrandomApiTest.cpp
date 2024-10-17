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

#include <gtest/gtest.h>
#include <sys/random.h>

using namespace testing::ext;

class HatsGetrandomTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsGetrandomTest::SetUp()
{
}
void HatsGetrandomTest::TearDown()
{
}
void HatsGetrandomTest::SetUpTestCase()
{
}
void HatsGetrandomTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETRANDOM_0100
 * @tc.name   : GetrandomFunctionalityTestSuccess_0001
 * @tc.desc   : getrandom functionality test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsGetrandomTest, GetrandomFunctionalityTestSuccess_0001, Function | MediumTest | Level1)
{
    size_t len = 256;
    char buffer[len];
    ssize_t ret = getrandom(buffer, len, 0);
    EXPECT_EQ(ret, static_cast<ssize_t>(len));
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETRANDOM_0200
 * @tc.name   : GetrandomMultipleCallsConsistencySuccess_0002
 * @tc.desc   : getrandom multiple calls consistency success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsGetrandomTest, GetrandomMultipleCallsConsistencySuccess_0002, Function | MediumTest | Level1)
{
    size_t len = 8;
    char buffer1[len];
    char buffer2[len];
    ssize_t ret = 0;
    ssize_t ret1 = 0;
    ssize_t ret2 = 0;

    ret1 = getrandom(buffer1, len, 0);
    EXPECT_EQ(ret1, static_cast<ssize_t>(len));
    ret2 = getrandom(buffer2, len, 0);
    EXPECT_EQ(ret2, static_cast<ssize_t>(len));
    ret = memcmp(buffer1, buffer2, len);
    EXPECT_NE(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETRANDOM_0300
 * @tc.name   : GetrandomInvalidParametersBuffFail_0003
 * @tc.desc   : getrandom invalid parameters buffer address fail, return -1, and set errno.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsGetrandomTest, GetrandomInvalidParametersBuffFail_0003, Function | MediumTest | Level2)
{
    size_t len = 10;

    errno = 0;
    ssize_t ret = getrandom(nullptr, len, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETRANDOM_0400
 * @tc.name   : GetrandomInvalidParametersFlagFail_0004
 * @tc.desc   : getrandom invalid parameters flag fail, return -1, and set errno.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsGetrandomTest, GetrandomInvalidParametersFlagFail_0004, Function | MediumTest | Level2)
{
    size_t len = 32;
    char buffer[len];
    ssize_t invalidFlag = -1;

    errno = 0;
    ssize_t ret = getrandom(buffer, len, invalidFlag);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETRANDOM_0500
 * @tc.name   : GetrandomGRND_RANDOMFlagSuccess_0005
 * @tc.desc   : getrandom use GRND_RANDOM flag success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsGetrandomTest, GetrandomGRND_RANDOMFlagSuccess_0005, Function | MediumTest | Level1)
{
    size_t len = 256;
    char buffer[len];
    ssize_t ret = getrandom(buffer, len, GRND_RANDOM | GRND_NONBLOCK);
    EXPECT_EQ(ret, static_cast<ssize_t>(len));
}
