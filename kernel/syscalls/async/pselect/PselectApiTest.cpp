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
#include <sys/select.h>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

class HatsPselectTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsPselectTest::SetUp()
{
}

void HatsPselectTest::TearDown()
{
}

void HatsPselectTest::SetUpTestCase()
{
}

void HatsPselectTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PSELECT_0100
 * @tc.name   : PselectSuccess_0001
 * @tc.desc   : Pselect monitors multiple file descriptors successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPselectTest, PselectSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int numFds = 2;
    fd_set readFds, writeFds;
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);

    FD_SET(0, &readFds);
    FD_SET(1, &writeFds);

    timespec timeout;
    timeout.tv_sec = 0;
    timeout.tv_nsec = 800 * 1000 * 1000;

    sigset_t sigMask;
    ret = sigemptyset(&sigMask);
    EXPECT_TRUE(ret >= 0);

    int retNum = pselect(numFds + 1, &readFds, &writeFds, nullptr, &timeout, &sigMask);
    EXPECT_TRUE(retNum > 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PSELECT_0200
 * @tc.name   : PselectTimeoutFailed_0002
 * @tc.desc   : Pselect return 0 for no data ready within the timeout period.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPselectTest, PselectTimeoutFailed_0002, Function | MediumTest | Level2)
{
    fd_set readFds;
    FD_ZERO(&readFds);

    timespec timeout = {0, 0};

    int invalidFdSet[10];
    memset_s(invalidFdSet, sizeof(invalidFdSet), 0, sizeof(invalidFdSet));

    int retNum = pselect(10, reinterpret_cast<fd_set*>(invalidFdSet), nullptr, nullptr, &timeout, nullptr);
    EXPECT_EQ(retNum, 0);
    EXPECT_EQ(errno, ENOENT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PSELECT_0300
 * @tc.name   : PselectInvalidArgFailed_0003
 * @tc.desc   : Pselect is failed for invalid argument.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPselectTest, PselectInvalidArgFailed_0003, Function | MediumTest | Level2)
{
    fd_set readFds;
    FD_ZERO(&readFds);

    timespec timeout = {0, 0};

    int invalidFdSet[10];
    memset_s(invalidFdSet, sizeof(invalidFdSet), 0, sizeof(invalidFdSet));

    int retNum = pselect(-1, reinterpret_cast<fd_set*>(invalidFdSet), nullptr, nullptr, &timeout, nullptr);
    EXPECT_EQ(retNum, -1);
    EXPECT_EQ(errno, EINVAL);
}