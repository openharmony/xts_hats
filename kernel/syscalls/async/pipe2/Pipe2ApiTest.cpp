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
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

class HatsPipe2Test : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsPipe2Test::SetUp()
{
}

void HatsPipe2Test::TearDown()
{
}

void HatsPipe2Test::SetUpTestCase()
{
}

void HatsPipe2Test::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0100
 * @tc.name   : Pipe2Success_0001
 * @tc.desc   : Pipe2 create pipe fds with flag O_CLOEXEC and O_NONBLOCK successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPipe2Test, Pipe2Success_0001, Function | MediumTest | Level1)
{
    int pipeFds[2];

    int ret = pipe2(pipeFds, O_CLOEXEC | O_NONBLOCK);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(pipeFds[0] > 0 && pipeFds[1] > 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PIPE2_0200
 * @tc.name   : Pipe2Failed_0002
 * @tc.desc   : Pipe2 fails for invalid Fd array.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsPipe2Test, Pipe2Failed_0002, Function | MediumTest | Level2)
{
    int* pipeFds = nullptr;

    errno = 0;
    int ret = pipe2(pipeFds, O_CLOEXEC);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}
