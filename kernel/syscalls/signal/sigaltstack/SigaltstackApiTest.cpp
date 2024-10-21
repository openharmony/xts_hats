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
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/wait.h>

using namespace testing::ext;

static char g_stack[SIGSTKSZ];

class HatsSigaltstackTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSigaltstackTest::SetUp()
{
}
void HatsSigaltstackTest::TearDown()
{
}
void HatsSigaltstackTest::SetUpTestCase()
{
}
void HatsSigaltstackTest::TearDownTestCase()
{
}

static void Handle(int sig)
{
    int ret;
    uintptr_t i;
    uintptr_t* ptr;
    stack_t ss;

    ptr = reinterpret_cast<uintptr_t*>(&i);
    EXPECT_TRUE(ptr >= reinterpret_cast<uintptr_t*>(g_stack));
    EXPECT_TRUE(ptr < reinterpret_cast<uintptr_t*>(g_stack + SIGSTKSZ));
    ret = sigaltstack(nullptr, &ss);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(ss.ss_flags, SS_ONSTACK);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SIGALTSTACK_0100
 * @tc.name   : SigaltstackParameterNormalProcessSuccess_0001
 * @tc.desc   : sigaltstack parameter normal process success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSigaltstackTest, SigaltstackParameterNormalProcessSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    stack_t ss;
    struct sigaction sa;

    ss.ss_sp = g_stack;
    ss.ss_size = sizeof(g_stack);
    ss.ss_flags = 0;
    sa.sa_handler = Handle;
    sa.sa_flags = SA_ONSTACK;
    ret = sigaltstack(&ss, nullptr);
    EXPECT_EQ(ret, 0);
    ret = sigfillset(&sa.sa_mask);
    EXPECT_EQ(ret, 0);
    ret = sigaction(SIGUSR1, &sa, nullptr);
    EXPECT_EQ(ret, 0);
    ret = raise(SIGUSR1);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SIGALTSTACK_0200
 * @tc.name   : SigaltstackParameterAbnormalProcessFail_0002
 * @tc.desc   : Sigaltstack paramter set fail, return -1, and set errno.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSigaltstackTest, SigaltstackParameterAbnormalProcessFail_0002, Function | MediumTest | Level2)
{
    int ret;
    stack_t ss;
    struct sigaction sa;
    
    ss.ss_sp = g_stack;
    ss.ss_size = sizeof(g_stack);
    ss.ss_flags = 0;
    sa.sa_handler = Handle;
    sa.sa_flags = SA_ONSTACK;
    errno = 0;
    ss.ss_size = MINSIGSTKSZ - 1;
    ret = sigaltstack(&ss, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOMEM);
    
    errno = 0;
    ss.ss_flags = -1;
    ss.ss_size = MINSIGSTKSZ;
    ret =  sigaltstack(&ss, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}
