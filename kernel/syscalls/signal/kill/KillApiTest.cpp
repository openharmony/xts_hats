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

class HatsKillTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsKillTest::SetUp()
{
}
void HatsKillTest::TearDown()
{
}
void HatsKillTest::SetUpTestCase()
{
}
void HatsKillTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_KILL_0100
 * @tc.name   : KillNormalTerminateProcessSuccess_0001
 * @tc.desc   : kill normal terminate process success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsKillTest, KillNormalTerminateProcessSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t killRet = 0;
    pid_t waitpidRet = 0;
    bool signalRet = false;

    pid_t childPid = fork();
    if (childPid == 0) {
        usleep(5);
    } else {
        killRet = kill(childPid, SIGTERM);
        EXPECT_EQ(killRet, 0);
        int status;
        waitpidRet = waitpid(childPid, &status, WUNTRACED);
        EXPECT_EQ(waitpidRet, childPid);
        signalRet = WIFSIGNALED(status);
        EXPECT_TRUE(signalRet);
    }
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_KILL_0200
 * @tc.name   : KillSendInvalidSignalFail_0002
 * @tc.desc   : Kill send invalid signal fail, return -1, and set errno.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsKillTest, KillSendInvalidSignalFail_0002, Function | MediumTest | Level2)
{
    ssize_t killRet = 0;

    pid_t pid = getpid();
    errno = 0;
    killRet = kill(pid, -1);
    EXPECT_EQ(killRet, -1);
    EXPECT_EQ(errno, EINVAL);
}
