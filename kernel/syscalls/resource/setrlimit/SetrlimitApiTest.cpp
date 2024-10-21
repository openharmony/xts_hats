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
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>


using namespace testing::ext;
using namespace std;

class SetrlimitApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void SetrlimitApiTest::SetUp()
{
}
void SetrlimitApiTest::TearDown()
{
}
void SetrlimitApiTest::SetUpTestCase()
{
}
void SetrlimitApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETRLIMIT_0100
 * @tc.name   : SetrlimitGetResourceLimitsSuccess_0001
 * @tc.desc   : Setrlimit set resource limits success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(SetrlimitApiTest, SetrlimitGetResourceLimitsSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    struct rlimit limit;
    ret = setrlimit(RLIMIT_CORE, &limit);
    EXPECT_EQ(ret, 0);
    ret = setrlimit(RLIMIT_LOCKS, &limit);
    EXPECT_EQ(ret, 0);
    ret = setrlimit(RLIMIT_MEMLOCK, &limit);
    EXPECT_EQ(ret, 0);
    ret = setrlimit(RLIMIT_MSGQUEUE, &limit);
    EXPECT_EQ(ret, 0);
    ret = setrlimit(RLIMIT_NPROC, &limit);
    EXPECT_EQ(ret, 0);
    ret = setrlimit(RLIMIT_RTPRIO, &limit);
    EXPECT_EQ(ret, 0);
    ret = setrlimit(RLIMIT_RTTIME, &limit);
    EXPECT_EQ(ret, 0);
    ret = setrlimit(RLIMIT_SIGPENDING, &limit);
    EXPECT_EQ(ret, 0);
    ret = setrlimit(RLIMIT_STACK, &limit);
    EXPECT_EQ(ret, 0);
    ret = setrlimit(RLIMIT_NICE, &limit);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETRLIMIT_0200
 * @tc.name   : SetrlimitGetResourceLimitsFail_0002
 * @tc.desc   : Setrlimit get resource limits fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(SetrlimitApiTest, SetrlimitGetResourceLimitsFail_0002, Function | MediumTest | Level2)
{
    struct rlimit limit;
    int ret = setrlimit(-1, &limit);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}
