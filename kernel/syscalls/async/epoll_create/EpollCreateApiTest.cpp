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

class HatsEpollCreateTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsEpollCreateTest::SetUp()
{
}

void HatsEpollCreateTest::TearDown()
{
}

void HatsEpollCreateTest::SetUpTestCase()
{
}

void HatsEpollCreateTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE_0100
 * @tc.name   : EpollCreateFdSuccess_0001
 * @tc.desc   : epoll_create epoll fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreateFdSuccess_0001, Function | MediumTest | Level1)
{
    int fd = epoll_create(1);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE1_0200
 * @tc.name   : EpollCreate1FdSuccess_0002
 * @tc.desc   : epoll_create1 epoll fd and EPOLL_CLOEXEC test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEpollCreateTest, EpollCreatelFdSuccess_0002, Function | MediumTest | Level1)
{
    int fd = epoll_create1(EPOLL_CLOEXEC);
    EXPECT_TRUE(fd > 0);
    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_EPOLLCREATE1_0300
 * @tc.name   : EpollCreate1UseInvalidParamFailed_0003
 * @tc.desc   : epoll_create1 epoll fd use invalid param failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEpollCreateTest, EpollCreate1UseInvalidParamFailed_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = epoll_create1(-1);
    EXPECT_TRUE(fd < 0);
    EXPECT_EQ(errno, EINVAL);
    close(fd);
}
