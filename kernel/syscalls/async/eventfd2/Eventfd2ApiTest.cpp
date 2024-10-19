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

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>

using namespace testing::ext;

class HatsEventfd2Test : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsEventfd2Test::SetUp()
{
}

void HatsEventfd2Test::TearDown()
{
}

void HatsEventfd2Test::SetUpTestCase()
{
}

void HatsEventfd2Test::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0100
 * @tc.name   : Eventfd2CreateFdSuccess_0001
 * @tc.desc   : eventfd2 create fd use EFD_NONBLOCK/EFD_SEMAPHORE success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEventfd2Test, Eventfd2CreateFdSuccess_0001, Function | MediumTest | Level1)
{
    int fd = syscall(__NR_eventfd2, 1, EFD_NONBLOCK);
    EXPECT_TRUE(fd > 0);
    close(fd);

    fd = syscall(__NR_eventfd2, 1, EFD_SEMAPHORE);
    EXPECT_TRUE(fd > 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EVENTFD2_0200
 * @tc.name   : Eventfd2InvalidParamFailed_0002
 * @tc.desc   : eventfd2 create fd use invalid param failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEventfd2Test, Eventfd2InvalidParamFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = syscall(__NR_eventfd2, 1, -1);
    EXPECT_TRUE(fd < 0);
    EXPECT_EQ(errno, EINVAL);
}