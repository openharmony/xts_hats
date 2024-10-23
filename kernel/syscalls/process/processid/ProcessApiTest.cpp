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
#include <sched.h>
#include <unistd.h>
#include <bits/syscall.h>
#include <gtest/gtest.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const int STACK_SIZE = 8 * 1024 * 1024;
static const char *TEST_FILE = "/data/local/tmp/exec_test.txt";

struct CloneFlag {
    int id;
    int flag;
};

class ProcessApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ProcessApiTest::SetUp()
{
}
void ProcessApiTest::TearDown()
{
}
void ProcessApiTest::SetUpTestCase()
{
}
void ProcessApiTest::TearDownTestCase()
{
}

static int CloneTest(void *arg)
{
    return 0;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_0100
 * @tc.name   : CloneSuccess_0001
 * @tc.desc   : success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, CloneSuccess_0001, Function | MediumTest | Level1)
{
    int i;
    int pid = -1;
    struct CloneFlag cloneFlag[] = {
        {1, CLONE_VM | CLONE_FS | CLONE_FILES | SIGCHLD},
        {2, CSIGNAL},
        {3, CLONE_VFORK},
        {4, CLONE_PARENT},
        {5, CLONE_NEWNS},
        {6, CLONE_SETTLS},
        {7, CLONE_PARENT_SETTID},
        {8, CLONE_CHILD_CLEARTID},
        {9, CLONE_CHILD_SETTID},
        {10, CLONE_NEWUTS},
        {11, CLONE_NEWIPC},
        {13, CLONE_NEWPID},
        {14, CLONE_NEWNET},
    };
    char *stack = new char[STACK_SIZE];
    for (i = 0; i < sizeof(cloneFlag) / sizeof(cloneFlag[0]); i++) {
        pid = clone(CloneTest, &stack[STACK_SIZE - 1], cloneFlag[i].flag, nullptr);
        EXPECT_TRUE(pid > 0);
        waitpid(pid, nullptr, 0);
    }
    free(stack);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_0200
 * @tc.name   : CloneInvalidFlagFailed_0002
 * @tc.desc   : clone flag invalid failed errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ProcessApiTest, CloneInvalidFlagFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    char *stack = new char[STACK_SIZE];
    pid_t pid = clone(CloneTest, &stack[STACK_SIZE - 1], -1, nullptr);
    EXPECT_EQ(pid, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_0300
 * @tc.name   : ExecveCmdSuccess_0003
 * @tc.desc   : execve exec cmd touch success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, ExecveCmdSuccess_0003, Function | MediumTest | Level1)
{
    int ret;
    pid_t pid = fork();
    if (pid == 0) {
        char cmd[] = "touch";
        char file[] = "/data/local/tmp/exec_test.txt";
        char *argv[] = {cmd, file, nullptr};
        char *envp[] = { nullptr };
        ret = execve("/bin/touch", argv, envp);
        EXPECT_EQ(ret, 0);
        exit(0);
    }
    waitpid(pid, nullptr, 0);
    EXPECT_EQ(access(TEST_FILE, F_OK), 0);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_0400
 * @tc.name   : ExecveInvalidParamFailed_0004
 * @tc.desc   : execve exec cmd touch success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ProcessApiTest, ExecveInvalidParamFailed_0004, Function | MediumTest | Level2)
{
    errno = 0;
    char cmd[] = "touch";
    char file[] = "/data/local/tmp/exec_test.txt";
    char *argv[] = {cmd, file, nullptr};
    char *envp[] = { nullptr };
    int ret = execve(nullptr, argv, envp);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_0500
 * @tc.name   : ExecveatSuccess_0005
 * @tc.desc   : success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, ExecveatSuccess_0005, Function | MediumTest | Level1)
{
    int ret;
    char cmd[] = "ls";
    char *argv[] = {cmd, nullptr};
    char *envp[] = { nullptr };
    int fd = open("/bin", O_RDONLY | O_DIRECTORY);
    EXPECT_TRUE(fd > 0);

    pid_t pid = fork();
    if (pid == 0) {
        ret = syscall(__NR_execveat, fd, cmd, argv, envp, 0);
        EXPECT_EQ(ret, 0);
        exit(0);
    }
    waitpid(pid, nullptr, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_0600
 * @tc.name   : ExecveatInvalidFlagsFailed_0006
 * @tc.desc   : execveat invalid flags -1 failed errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ProcessApiTest, ExecveatInvalidFlagsFailed_0006, Function | MediumTest | Level2)
{
    int ret;
    char cmd[] = "ls";
    char *argv[] = {cmd, nullptr};
    char *envp[] = { nullptr };
    int fd = open("/bin", O_RDONLY | O_DIRECTORY);
    EXPECT_TRUE(fd > 0);

    pid_t pid = fork();
    if (pid == 0) {
        errno = 0;
        ret = syscall(__NR_execveat, fd, cmd, argv, envp, -1);
        EXPECT_EQ(ret, -1);
        EXPECT_EQ(errno, EINVAL);
        exit(0);
    }
    waitpid(pid, nullptr, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_0700
 * @tc.name   : ExitGroupSyscallSuccess_0007
 * @tc.desc   : __NR_exit_group syscall test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, ExitGroupSyscallSuccess_0007, Function | MediumTest | Level1)
{
    int ret;
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        ret = syscall(__NR_exit_group, 0);
        EXPECT_EQ(ret, 0);
        exit(0);
    }
    waitpid(pid, nullptr, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_0800
 * @tc.name   : GetPidSuccess_0008
 * @tc.desc   : success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, GetPidSuccess_0008, Function | MediumTest | Level1)
{
    pid_t pid = getpid();
    EXPECT_TRUE(pid >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_0900
 * @tc.name   : GetppidGetParentPidSuccess_0009
 * @tc.desc   : getppid get parent pid success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, GetppidGetParentPidSuccess_0009, Function | MediumTest | Level1)
{
    pid_t ppid = getppid();
    EXPECT_TRUE(ppid >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_1000
 * @tc.name   : GetSidSuccess_0010
 * @tc.desc   : success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, GetSidSuccess_0010, Function | MediumTest | Level1)
{
    pid_t pid = getpid();
    EXPECT_TRUE(pid >= 0);

    pid_t sid = getsid(pid);
    EXPECT_TRUE(sid >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_1100
 * @tc.name   : GetTidSuccess_0011
 * @tc.desc   : success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, GetTidSuccess_0011, Function | MediumTest | Level1)
{
    pid_t tid = gettid();
    EXPECT_TRUE(tid >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_1200
 * @tc.name   : SetTidAddressSuccess_0012
 * @tc.desc   : success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, SetTidAddressSuccess_0001, Function | MediumTest | Level1)
{
    int newTid = -1;
    int ret = syscall(__NR_set_tid_address, &newTid);
    EXPECT_EQ(ret, getpid());
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESS_1300
 * @tc.name   : UnshareFlagsTest_0013
 * @tc.desc   : unshare success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ProcessApiTest, UnshareFlagsTest_0013, Function | MediumTest | Level1)
{
    int32_t ret = unshare(CLONE_NEWPID);
    EXPECT_EQ(ret, 0);

    ret = unshare(CLONE_FILES);
    EXPECT_EQ(ret, 0);

    ret = unshare(CLONE_FS);
    EXPECT_EQ(ret, 0);

    ret = unshare(CLONE_NEWNS);
    EXPECT_EQ(ret, 0);

    ret = unshare(CLONE_NEWUTS);
    EXPECT_EQ(ret, 0);

    ret = unshare(CLONE_NEWIPC);
    EXPECT_EQ(ret, 0);

    ret = unshare(CLONE_NEWNET);
    EXPECT_EQ(ret, 0);
}
