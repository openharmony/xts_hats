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
#include <sys/sysinfo.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

class HatsSysinfoTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSysinfoTest::SetUp()
{
}

void HatsSysinfoTest::TearDown()
{
}

void HatsSysinfoTest::SetUpTestCase()
{
}

void HatsSysinfoTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSINFO_0100
 * @tc.name   : SysinfoSuccess_0001
 * @tc.desc   : Obtain system statistical information successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSysinfoTest, SysinfoSuccess_0001, Function | MediumTest | Level1)
{
    struct sysinfo info;
    int ret = sysinfo(&info);
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE(info.uptime > 0);
    EXPECT_TRUE(info.mem_unit > 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSINFO_0200
 * @tc.name   : SysinfoNullptrFailed_0002
 * @tc.desc   : sysinfo nullptr failed errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSysinfoTest, SysinfoNullptrFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = sysinfo(nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}