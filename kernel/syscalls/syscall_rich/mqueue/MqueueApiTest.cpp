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
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "securec.h"
#include <mqueue.h>
#include <ctime>
#include <unistd.h>

using namespace testing::ext;
using namespace std;

#define QUEUE_NAME    "test_queue"
#define MAX_SIZE      1024
#define MSG_STOP      "exit"

class MqueueApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MqueueApiTest::SetUp()
{
}
void MqueueApiTest::TearDown()
{
}
void MqueueApiTest::SetUpTestCase()
{
}
void MqueueApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_0100
 * @tc.name   : MqueueApiTest_0001
 * @tc.desc   : remove and open message queue success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0001, Function | MediumTest | Level1)
{
    mqd_t mq;

    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_RDWR, 0644, nullptr);
    EXPECT_TRUE(mq > 0);

    int ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_0200
 * @tc.name   : MqueueApiTest_0002
 * @tc.desc   : open the exists message name to failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0002, Function | MediumTest | Level1)
{
    mqd_t mq;

    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_RDWR, 0644, nullptr);
    EXPECT_TRUE(mq > 0);

    errno = 0;
    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_EXCL, 0644, nullptr);
    EXPECT_EQ(errno, EEXIST);
    EXPECT_TRUE(mq == -1);

    int ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_0300
 * @tc.name   : MqueueApiTest_0003
 * @tc.desc   : the O_RDONLY to send message will failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0003, Function | MediumTest | Level1)
{
    mqd_t mq;
    const char *buffer = "hello world";
    struct timespec ts;

    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_RDONLY, 0644, nullptr);
    EXPECT_TRUE(mq > 0);

    errno = 0;
    int ret = syscall(__NR_mq_timedsend, mq, buffer, strlen(buffer) + 1, 0, &ts);
    EXPECT_EQ(errno, EBADF);
    EXPECT_TRUE(ret == -1);

    ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_0400
 * @tc.name   : MqueueApiTest_0004
 * @tc.desc   : the O_WRONLY to recv message will failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0004, Function | MediumTest | Level1)
{
    mqd_t mq;
    char recvBuf[MAX_SIZE] = { 0 };
    struct timespec ts;

    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_WRONLY, 0644, nullptr);
    EXPECT_TRUE(mq > 0);

    errno = 0;
    int ret = syscall(__NR_mq_timedreceive, mq, recvBuf, MAX_SIZE, nullptr, &ts);
    EXPECT_EQ(errno, EBADF);
    EXPECT_TRUE(ret == -1);

    ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_0500
 * @tc.name   : MqueueApiTest_0005
 * @tc.desc   : the O_NONBLOCK to recv message will failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0005, Function | MediumTest | Level1)
{
    mqd_t mq;
    char recvBuf[MAX_SIZE] = { 0 };
    struct timespec ts;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attr);
    EXPECT_TRUE(mq > 0);

    errno = 0;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 2;
    int bytesRead = syscall(__NR_mq_timedreceive, mq, recvBuf, MAX_SIZE, nullptr, &ts);
    EXPECT_EQ(errno, EAGAIN);
    EXPECT_TRUE(bytesRead == -1);

    int ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_0600
 * @tc.name   : MqueueApiTest_0006
 * @tc.desc   : send message buffer size low mq_maxmsg size will failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0006, Function | MediumTest | Level2)
{
    int ret;
    mqd_t mq;
    const char *buffer = "hello world";
    struct timespec ts;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = 2;
    attr.mq_curmsgs = 0;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 2;

    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attr);
    EXPECT_TRUE(mq > 0);

    errno = 0;
    ret = syscall(__NR_mq_timedsend, mq, buffer, strlen(buffer) + 1, 0, &ts);
    EXPECT_EQ(errno, EMSGSIZE);
    EXPECT_EQ(ret, -1);

    ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_0700
 * @tc.name   : MqueueApiTest_0007
 * @tc.desc   : mqueue send message success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0007, Function | MediumTest | Level2)
{
    int ret;
    mqd_t mq;
    const char *buffer = "hello world";
    struct timespec ts;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 2;

    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    EXPECT_TRUE(mq > 0);

    ret = syscall(__NR_mq_timedsend, mq, buffer, strlen(buffer) + 1, 0, &ts);
    EXPECT_EQ(ret, 0);

    ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_0800
 * @tc.name   : MqueueApiTest_0008
 * @tc.desc   : recv message timeout and return failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0008, Function | MediumTest | Level1)
{
    mqd_t mq;
    char buffer[MAX_SIZE] = { 0 };
    struct timespec ts;
    ssize_t bytesRead;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    EXPECT_TRUE(mq > 0);

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 2;

    errno = 0;
    bytesRead = syscall(__NR_mq_timedreceive, mq, buffer, MAX_SIZE, NULL, &ts);
    EXPECT_EQ(errno, ETIMEDOUT);
    EXPECT_EQ(bytesRead, -1);

    int ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_0900
 * @tc.name   : MqueueApiTest_0009
 * @tc.desc   : remove message queue failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0009, Function | MediumTest | Level1)
{
    errno = 0;
    int ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, -1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_1000
 * @tc.name   : MqueueApiTest_0010
 * @tc.desc   : remove message queue success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0010, Function | MediumTest | Level2)
{
    mqd_t mq = syscall(__NR_mq_open, QUEUE_NAME, O_RDWR | O_CREAT, 0644, NULL);
    EXPECT_TRUE(mq > 0);

    int ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MQUEUE_1100
 * @tc.name   : MqueueApiTest_0011
 * @tc.desc   : mqueue send and receive success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MqueueApiTest, MqueueApiTest_0011, Function | MediumTest | Level2)
{
    int ret;
    mqd_t mq;
    const char *sendBuf = "hello world";
    char recvBuf[MAX_SIZE] = { 0 };
    struct timespec ts;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 2;

    mq = syscall(__NR_mq_open, QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    EXPECT_TRUE(mq > 0);

    ret = syscall(__NR_mq_timedsend, mq, sendBuf, strlen(sendBuf) + 1, 0, &ts);
    EXPECT_EQ(ret, 0);

    errno = 0;
    int bytesRead = syscall(__NR_mq_timedreceive, mq, recvBuf, MAX_SIZE, nullptr, &ts);
    EXPECT_TRUE(bytesRead > 0);

    ret = memcmp(sendBuf, recvBuf, strlen(sendBuf));
    EXPECT_EQ(ret, 0);

    ret = syscall(__NR_mq_unlink, QUEUE_NAME);
    EXPECT_EQ(ret, 0);
}
