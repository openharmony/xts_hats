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
#include <string>
#include <vector>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>


using namespace testing::ext;
using namespace std;

class Prlimit64ApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void Prlimit64ApiTest::SetUp()
{
}
void Prlimit64ApiTest::TearDown()
{
}
void Prlimit64ApiTest::SetUpTestCase()
{
}
void Prlimit64ApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRLIMIT64_0100
 * @tc.name   : PrlimitSetGetPIDResourceLimitsSuccess_0001
 * @tc.desc   : Prlimit64 get and set pid resource limits success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(Prlimit64ApiTest, PrlimitSetGetPIDResourceLimitsSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    pid_t pid = getpid();
    struct rlimit limit;
    struct rlimit limit2;
    ret = prlimit64(pid, RLIMIT_NPROC, nullptr, &limit);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(pid, RLIMIT_NPROC, &limit, nullptr);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_NPROC, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(pid, RLIMIT_NOFILE, nullptr, &limit);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_CORE, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_LOCKS, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_MEMLOCK, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_MSGQUEUE, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_NOFILE, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_RTPRIO, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_RTTIME, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_SIGPENDING, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_STACK, &limit, &limit2);
    EXPECT_EQ(ret, 0);
    ret = prlimit64(0, RLIMIT_NICE, &limit, &limit2);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRLIMIT64_0200
 * @tc.name   : PrlimitSetGetPIDResourceLimitsFail_0002
 * @tc.desc   : Prlimit64 set and get pid resource limits fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(Prlimit64ApiTest, PrlimitSetGetPIDResourceLimitsFail_0002, Function | MediumTest | Level2)
{
    int ret;
    struct rlimit limit;
    pid_t pid = getpid();
    
    errno = 0;
    ret = prlimit64(-2, RLIMIT_NPROC, nullptr, &limit);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESRCH);

    errno = 0;
    ret = prlimit64(pid, -1, nullptr, &limit);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}
