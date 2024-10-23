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

class HatsMsggetTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMsggetTest::SetUp()
{
}
void HatsMsggetTest::TearDown()
{
}
void HatsMsggetTest::SetUpTestCase()
{
}
void HatsMsggetTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MSGGET_0100
 * @tc.name   : MsggetCreateMessageQueueSuccess_0001
 * @tc.desc   : msgget create a message queue success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMsggetTest, MsggetCreateMessageQueueSuccess_0001, Function | MediumTest | Level1)
{
    // create IPC_PRIVATE ipc
    int msgId = msgget(IPC_PRIVATE, IPC_CREAT | 0777);
    EXPECT_TRUE(msgId >= 0);
    int ret = msgctl(msgId, IPC_RMID, nullptr);
    EXPECT_EQ(ret, 0);

    // create common ipc
    key_t key = 1234;
    msgId = msgget(key, IPC_CREAT | 0777);
    EXPECT_TRUE(msgId >= 0);
    ret = msgctl(msgId, IPC_RMID, nullptr);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MSGGET_0200
 * @tc.name   : MsggetTwiceFailed_0002
 * @tc.desc   : msgget create a message queue twice failed, errno EEXIST.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsMsggetTest, MsggetTwiceFailed_0002, Function | MediumTest | Level2)
{
    key_t key = 1234;
    int msgId = msgget(key, IPC_CREAT | 0777);
    EXPECT_TRUE(msgId >= 0);

    errno = 0;
    int newMsgId = msgget(key, IPC_CREAT | IPC_EXCL | 0777);
    EXPECT_TRUE(newMsgId == -1);
    EXPECT_EQ(errno, EEXIST);

    int ret = msgctl(msgId, IPC_RMID, nullptr);
    EXPECT_EQ(ret, 0);
}
