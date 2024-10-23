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
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class HatsMsgctlTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMsgctlTest::SetUp()
{
}
void HatsMsgctlTest::TearDown()
{
}
void HatsMsgctlTest::SetUpTestCase()
{
}
void HatsMsgctlTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MSGCTL_0100
 * @tc.name   : MsgctlGetStatusSuccess_0001
 * @tc.desc   : msgctl get message queue status success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMsgctlTest, MsgctlGetStatusSuccess_0001, Function | MediumTest | Level1)
{
    struct msqid_ds msqBuf;

    int msqid = msgget(IPC_PRIVATE, O_CREAT | 0777);
    EXPECT_TRUE(msqid >= 0);

    int ret = msgctl(msqid, IPC_STAT, &msqBuf);
    EXPECT_EQ(ret, 0);

    ret = msgctl(msqid, IPC_SET, &msqBuf);
    EXPECT_EQ(ret, 0);

    ret = msgctl(msqid, IPC_RMID, nullptr);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MSGCTL_0200
 * @tc.name   : MsgctlInvalidMsqidFail_0002
 * @tc.desc   : msgctl perform control operation on an invalid message queue id fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsMsgctlTest, MsgctlInvalidMsqidFail_0002, Function | MediumTest | Level2)
{
    int ret;
    int msqid = -1;
    struct msqid_ds msqBuf;

    errno = 0;
    ret = msgctl(msqid, IPC_STAT, &msqBuf);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    ret = msgctl(msqid, IPC_RMID, nullptr);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MSGCTL_0300
 * @tc.name   : MsgctlInvalidCmdFail_0003
 * @tc.desc   : msgctl perform invalid command failed, errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsMsgctlTest, MsgctlInvalidOperationFail_0003, Function | MediumTest | Level2)
{
    int ret;
    int msqid;
    int cmd = -1;
    struct msqid_ds msqBuf;

    msqid = msgget(IPC_PRIVATE, O_CREAT | 0777);
    EXPECT_TRUE(msqid >= 0);

    errno = 0;
    ret = msgctl(msqid, cmd, &msqBuf);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);

    ret = msgctl(msqid, IPC_RMID, nullptr);
    EXPECT_EQ(ret, 0);
}