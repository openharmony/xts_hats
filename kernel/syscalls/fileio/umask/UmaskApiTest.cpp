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

#include <cstdio>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const mode_t MASK = 022;

class UmaskApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void UmaskApiTest::SetUp()
{
}
void UmaskApiTest::TearDown()
{
}
void UmaskApiTest::SetUpTestCase()
{
}
void UmaskApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_Umask_0100
 * @tc.name   : UmaskSuccess_0001
 * @tc.desc   : umask success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UmaskApiTest, UmaskSuccess_0001, Function | MediumTest | Level1)
{
    mode_t newMask;
    umask(MASK);
    newMask = umask(MASK);
    EXPECT_EQ(newMask, MASK);
}