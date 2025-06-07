/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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
#include <csignal>
#include <cstring>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>
#include "securec.h"

using namespace testing::ext;

static const int EXIT_VALUE = 123;

class HatsWaitidTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsWaitidTest::SetUp()
{
}

void HatsWaitidTest::TearDown()
{
}

void HatsWaitidTest::SetUpTestCase()
{
}

void HatsWaitidTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAITID_0100
 * @tc.name   : WaitidAllChildProcessSuccess_0001
 * @tc.desc   : Waitid wait all child process and WEXITED success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWaitidTest, WaitidAllChildProcessSuccess_0001, Function | MediumTest | Level2)
{
    siginfo_t info;

    pid_t pid = fork();
    if (pid == 0) {
        exit(EXIT_VALUE);
    }

    int ret = waitid(P_ALL, 0, &info, WEXITED);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(info.si_pid, pid);
    EXPECT_EQ(info.si_status, EXIT_VALUE);
    EXPECT_EQ(info.si_signo, SIGCHLD);
    EXPECT_EQ(info.si_code, CLD_EXITED);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAITID_0200
 * @tc.name   : WaitidGroupChildProcessFailed_0002
 * @tc.desc   : waitid group child process and WEXITED failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWaitidTest, WaitidGroupChildProcessFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    siginfo_t info;
    pid_t pidGroup;

    pidGroup = getpgid(0);

    int ret = waitid(P_PGID, pidGroup + 1, &info, WEXITED);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ECHILD);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAITID_0300
 * @tc.name   : WaitidGroupChildProcessSuccess_0003
 * @tc.desc   : waitid group child process and WEXITED success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWaitidTest, WaitidGroupChildProcessSuccess_0003, Function | MediumTest | Level2)
{
    siginfo_t info;
    pid_t pid;
    pid_t pidChild;
    pid_t pidGroup;

    pid = fork();
    if (pid == 0) {
        pidChild = getpid();
        exit(0);
    }

    pidGroup = getpgid(0);

    int ret = waitid(P_PGID, pidGroup, &info, WEXITED);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(info.si_pid, pid);
    EXPECT_EQ(info.si_status, 0);
    EXPECT_EQ(info.si_signo, SIGCHLD);
    EXPECT_EQ(info.si_code, CLD_EXITED);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAITID_0400
 * @tc.name   : WaitidChildProcessSuccess_0004
 * @tc.desc   : waitid child process and WEXITED success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWaitidTest, WaitidChildProcessSuccess_0004, Function | MediumTest | Level2)
{
    siginfo_t info;

    pid_t pid = fork();
    if (pid == 0) {
        exit(0);
    }

    int ret = waitid(P_PID, pid, &info, WEXITED);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(info.si_pid, pid);
    EXPECT_EQ(info.si_status, 0);
    EXPECT_EQ(info.si_signo, SIGCHLD);
    EXPECT_EQ(info.si_code, CLD_EXITED);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAITID_0500
 * @tc.name   : WaitidChildProcessSuccess_0005
 * @tc.desc   : waitid child process and WSTOPPED success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWaitidTest, WaitidChildProcessSuccess_0005, Function | MediumTest | Level2)
{
    siginfo_t info;

    pid_t pid = fork();
    if (pid == 0) {
        kill(getpid(), SIGSTOP);
        wait(nullptr);
        return;
    }

    int ret = waitid(P_PID, pid, &info, WSTOPPED | WNOWAIT);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(info.si_pid, pid);
    EXPECT_EQ(info.si_status, SIGSTOP);
    EXPECT_EQ(info.si_signo, SIGCHLD);
    EXPECT_EQ(info.si_code, CLD_STOPPED);

    kill(pid, SIGCONT);

    memset_s(&info, sizeof(info), 0, sizeof(info));

    ret = waitid(P_PID, pid, &info, WCONTINUED);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(info.si_pid, pid);
    EXPECT_EQ(info.si_status, SIGCONT);
    EXPECT_EQ(info.si_signo, SIGCHLD);
    EXPECT_EQ(info.si_code, CLD_CONTINUED);
    kill(pid, SIGQUIT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAITID_0600
 * @tc.name   : WaitidChildProcessFailed_0006
 * @tc.desc   : waitid child process and WNOHANG failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWaitidTest, WaitidChildProcessFailed_0006, Function | MediumTest | Level2)
{
    siginfo_t info;
    errno = 0;
    int ret = waitid(P_ALL, 0, &info, WNOHANG);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

