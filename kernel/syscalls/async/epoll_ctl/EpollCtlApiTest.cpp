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

class HatsEpollCtlTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsEpollCtlTest::SetUp()
{
}

void HatsEpollCtlTest::TearDown()
{
}

void HatsEpollCtlTest::SetUpTestCase()
{
}

void HatsEpollCtlTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCTL_0100
 * @tc.name   : EpollCtlModeTestSuccess_0001
 * @tc.desc   : epoll_ctl mode EPOLL_CTL_ADDT/EPOLL_CTL_MOD/EPOLL_CTL_DEL test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCtlTest, EpollCtlModeTestSuccess_0001, Function | MediumTest | Level1)
{
    int fds[2];
    const uint64_t expected = 0x11223344;

    int fd = epoll_create(1);
    EXPECT_TRUE(fd > 0);

    int ret = pipe(fds);
    EXPECT_EQ(ret, 0);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.u64 = expected;

    ret = epoll_ctl(fd, EPOLL_CTL_ADD, fds[0], &ev);
    EXPECT_EQ(ret, 0);

    ret = epoll_ctl(fd, EPOLL_CTL_MOD, fds[0], &ev);
    EXPECT_EQ(ret, 0);

    ret = epoll_ctl(fd, EPOLL_CTL_DEL, fds[0], &ev);
    EXPECT_EQ(ret, 0);

    close(fds[0]);
    close(fds[1]);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCTL_0200
 * @tc.name   : EpollCtlUseInvalidFdTestFailed_0002
 * @tc.desc   : epoll_ctl use invalid fd test failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCtlTest, EpollCtlUseInvalidFdTestFailed_0002, Function | MediumTest | Level2)
{
    int fds[2];
    const uint64_t expected = 0x11223344;

    int ret = pipe(fds);
    EXPECT_EQ(ret, 0);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.u64 = expected;

    errno = 0;
    ret = epoll_ctl(-1, EPOLL_CTL_ADD, fds[0], &ev);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);

    close(fds[0]);
    close(fds[1]);
}