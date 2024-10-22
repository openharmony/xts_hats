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

static const int MAX_LEN = 1024;
static const int MSG_TYPE = 1;

struct MsgBuf {
    long type;
    char text[MAX_LEN];
};

class HatsMsgHandleTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMsgHandleTest::SetUp()
{
}
void HatsMsgHandleTest::TearDown()
{
}
void HatsMsgHandleTest::SetUpTestCase()
{
}
void HatsMsgHandleTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MSG_HANDLE_0100
 * @tc.name   : MsgsndSendValueAndMsgrcvSuccess_0001
 * @tc.desc   : msgsnd send message queue and msgrcv message success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMsgHandleTest, MsgsndSendValueAndMsgrcvSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int msqid;

    struct MsgBuf buffer = {
        .type = MSG_TYPE,
        .text = "Hello, world!",
    };

    msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0777);
    EXPECT_TRUE(msqid >= 0);

    ret = msgsnd(msqid, &buffer, MAX_LEN, IPC_NOWAIT);
    EXPECT_EQ(ret, 0);

    memset_s(&buffer, sizeof(buffer), 0, sizeof(buffer));

    ret = msgrcv(msqid, &buffer, MAX_LEN, MSG_TYPE, IPC_NOWAIT);
    EXPECT_TRUE(ret >= 0);
    EXPECT_STREQ(buffer.text, "Hello, world!");

    ret = msgctl(msqid, IPC_RMID, nullptr);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MSG_HANDLE_0200
 * @tc.name   : MsgsndAndMsgrcvUseInvalidMsqidFail_0002
 * @tc.desc   : msgsnd and msgrcv use a invalid msqid fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsMsgHandleTest, MsgsndAndMsgrcvUseInvalidMsqidFail_0002, Function | MediumTest | Level2)
{
    int ret;

    struct MsgBuf buffer = {
        .type = MSG_TYPE,
        .text = "Hello, world!",
    };

    errno = 0;
    ret = msgsnd(-1, &buffer, MAX_LEN, IPC_NOWAIT);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    ret = msgrcv(-1, &buffer, MAX_LEN, MSG_TYPE, IPC_NOWAIT);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MSG_HANDLE_0300
 * @tc.name   : MsgsndAndMsgrcvUseInvalidLenFail_0003
 * @tc.desc   : msgsnd and msgrcv use invalid message size given fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsMsgHandleTest, MsgsndAndMsgrcvUseInvalidLenFail_0003, Function | MediumTest | Level2)
{
    int ret;
    int msqid;

    struct MsgBuf buffer = {
        .type = MSG_TYPE,
        .text = "Hello, world!",
    };

    msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0777);
    EXPECT_TRUE(msqid >= 0);

    errno = 0;
    ret = msgsnd(msqid, &buffer, -1, IPC_NOWAIT);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    errno = 0;
    ret = msgrcv(msqid, &buffer, -1, MSG_TYPE, IPC_NOWAIT);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    ret = msgctl(msqid, IPC_RMID, nullptr);
    EXPECT_EQ(ret, 0);
}
