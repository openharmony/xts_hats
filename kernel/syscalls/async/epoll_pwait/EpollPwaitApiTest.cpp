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
#include <gtest/gtest.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

class HatsEpollPwaitTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsEpollPwaitTest::SetUp()
{
}

void HatsEpollPwaitTest::TearDown()
{
}

void HatsEpollPwaitTest::SetUpTestCase()
{
}

void HatsEpollPwaitTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLPWAIT_0100
 * @tc.name   : EpollPwaitWithNoSigsetSuccess_0001
 * @tc.desc   : epoll_pwait with no sigset test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollPwaitTest, EpollPwaitWithNoSigsetSuccess_0001, Function | MediumTest | Level1)
{
    int fd = epoll_create(1);
    EXPECT_TRUE(fd > 0);

    struct epoll_event events[1];
    int ret = epoll_pwait(fd, events, 1, 1, nullptr);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLPWAIT_0200
 * @tc.name   : EpollPwaitWithSIGPIPESigsetSuccess_0002
 * @tc.desc   : epoll_pwait with SIGPIPE sigset test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollPwaitTest, EpollPwaitWithSIGPIPESigsetSuccess_0002, Function | MediumTest | Level1)
{
    int ret;
    int fd = epoll_create(1);
    EXPECT_TRUE(fd > 0);

    struct epoll_event events[1];
    sigset_t ss;
    ret = sigemptyset(&ss);
    EXPECT_EQ(ret, 0);
    ret = sigaddset(&ss, SIGPIPE);
    EXPECT_EQ(ret, 0);

    ret = epoll_pwait(fd, events, 1, 1, &ss);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLPWAIT_0300
 * @tc.name   : EpollPwaitUseInvalidFdFailed_0003
 * @tc.desc   : epoll_pwait use invalid fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollPwaitTest, EpollPwaitUseInvalidFdFailed_0003, Function | MediumTest | Level2)
{
    struct epoll_event events[1];
    errno = 0;
    int ret = epoll_pwait(-1, events, 1, 1, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLPWAIT_0400
 * @tc.name   : EpollPwaitEINVALFailed_0004
 * @tc.desc   : epoll_pwait with EINVAL failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollPwaitTest, EpollPwaitEINVALFailed_0004, Function | MediumTest | Level2)
{
    struct epoll_event events[1];
    errno = 0;
    int ret = epoll_pwait(-1, events, -1, 1, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}