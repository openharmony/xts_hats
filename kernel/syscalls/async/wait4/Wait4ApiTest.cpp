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
#include <csignal>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <poll.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

static const int DELAY_TIME = 10 * 1000;

class HatsWait4Test : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsWait4Test::SetUp()
{
}

void HatsWait4Test::TearDown()
{
}

void HatsWait4Test::SetUpTestCase()
{
}

void HatsWait4Test::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0100
 * @tc.name   : Wait4Failed_0001
 * @tc.desc   : Wait4 wait continued failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsWait4Test, Wait4Failed_0001, Function | MediumTest | Level2)
{
    int ret;
    pid_t childPid = -1;

    sigset_t sigMask;
    ret = sigemptyset(&sigMask);
    EXPECT_TRUE(ret >= 0);
    ret = sigaddset(&sigMask, SIGCHLD);
    EXPECT_TRUE(ret >= 0);

    struct sigaction sa;
    sa.sa_sigaction = nullptr;
    sa.sa_flags = SA_SIGINFO;
    ret = sigemptyset(&sa.sa_mask);
    EXPECT_TRUE(ret >= 0);
    ret = sigaction(SIGCHLD, &sa, nullptr);
    EXPECT_EQ(ret, 0);

    int wstatus = 0;
    int options = WCONTINUED;
    int retPid = wait4(childPid, &wstatus, options, nullptr);
    EXPECT_EQ(retPid, -1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_WAIT4_0200
 * @tc.name   : Wait4ChildProcessSuccess_0002
 * @tc.desc   : wait4 child process and WIFEXITED success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsWait4Test, Wait4ChildProcessSuccess_0002, Function | MediumTest | Level1)
{
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child process (PID: %d) is running...\n", getpid());
        usleep(DELAY_TIME);
        exit(0);
    }
    int status;
    struct rusage usage;
    pid_t tpid = wait4(pid, &status, 0, &usage);
    EXPECT_TRUE(tpid > 0);
    EXPECT_EQ(WIFEXITED(status), 1);
    EXPECT_EQ(WEXITSTATUS(status), 0);
    EXPECT_EQ(WIFSIGNALED(status), 0);
    EXPECT_EQ(WTERMSIG(status), 0);
    EXPECT_EQ(WCOREDUMP(status), 0);
    EXPECT_EQ(WIFSTOPPED(status), 0);
    EXPECT_EQ(WSTOPSIG(status), 0);
    EXPECT_EQ(WIFCONTINUED(status), 0);
}
