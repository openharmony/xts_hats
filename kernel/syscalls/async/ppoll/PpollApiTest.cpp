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
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

class HatsPpollTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsPpollTest::SetUp()
{
}

void HatsPpollTest::TearDown()
{
}

void HatsPpollTest::SetUpTestCase()
{
}

void HatsPpollTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PPOLL_0100
 * @tc.name   : PpollSuccess_0001
 * @tc.desc   : Ppoll poll fds successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPpollTest, PpollSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    struct pollfd pfdArray[2];
    int numFds = 2;

    pfdArray[0].fd = 0;
    pfdArray[0].events = POLLIN;

    pfdArray[1].fd = 1;
    pfdArray[1].events = POLLOUT;

    struct timespec timeout;
    timeout.tv_sec = 0;
    timeout.tv_nsec = 200 * 1000 * 1000;

    sigset_t sigMask;
    ret = sigemptyset(&sigMask);
    EXPECT_TRUE(ret >= 0);

    int retNum = ppoll(pfdArray, numFds, &timeout, &sigMask);
    EXPECT_TRUE(retNum > 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PPOLL_0200
 * @tc.name   : PpollFdsNoReadyFailed_0002
 * @tc.desc   : Ppoll poll is failed for no file descriptors were ready.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPpollTest, PpollFdsNoReadyFailed_0002, Function | MediumTest | Level2)
{
    struct pollfd* fds = nullptr;
    struct timespec timeout;
    timeout.tv_sec = 0;
    timeout.tv_nsec = 200 * 1000 * 1000;

    int retNum = ppoll(fds, 0, &timeout, nullptr);
    EXPECT_EQ(retNum, 0);
}
