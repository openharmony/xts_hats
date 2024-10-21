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

#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/syscall.h>

using namespace testing::ext;

class HatsTkillTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsTkillTest::SetUp()
{
}
void HatsTkillTest::TearDown()
{
}
void HatsTkillTest::SetUpTestCase()
{
}
void HatsTkillTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TKILL_0100
 * @tc.name   : TkillSendSignalToExistingProcessSuccess_0001
 * @tc.desc   : tkill send signal to existing process success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsTkillTest, TkillSendSignalToExistingProcessSuccess_0001, Function | MediumTest | Level1)
{
    bool signalRet = false;
    int signalNumber = 0;

    pid_t childPid = fork();
    if (childPid == 0) {
        usleep(5);
    } else {
        int ret = syscall(SYS_tkill, childPid, SIGTERM);
        EXPECT_EQ(ret, 0);
        int status;
        pid_t waitret = waitpid(childPid, &status, 0);
        EXPECT_EQ(waitret, childPid);
        signalRet = WIFSIGNALED(status);
        signalNumber = WTERMSIG(status);
        EXPECT_TRUE(signalRet && (SIGTERM == signalNumber));
    }
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_TKILL_0200
 * @tc.name   : TkillSendSignalToNonExistingProcessFail_0002
 * @tc.desc   : tkill send signal to nonExisting process fail, return -1, and set errno.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsTkillTest, TkillSendSignalToNonExistingProcessFail_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = syscall(SYS_tkill, -1, SIGTERM);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}
