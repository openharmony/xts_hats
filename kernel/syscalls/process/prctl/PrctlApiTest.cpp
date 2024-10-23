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
#include <climits>
#include <sys/prctl.h>
#include <gtest/gtest.h>
#include <linux/capability.h>
#include <sys/capability.h>
#include <sys/mman.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const int BUFFER_SIZE = 1024;
static const int NUMBER_PAGE = 3;

class HatsPrctlSyscallTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsPrctlSyscallTest::SetUp()
{
}
void HatsPrctlSyscallTest::TearDown()
{
}
void HatsPrctlSyscallTest::SetUpTestCase()
{
}
void HatsPrctlSyscallTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRCTL_0100
 * @tc.name   : PrctlOptionTestSuccess_0001
 * @tc.desc   : prctl option PR_SET_NAME/PR_GET_NAME/PR_SET_PDEATHSIG/PR_GET_PDEATHSIG test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPrctlSyscallTest, PrctlOptionTestSuccess_0001, Function | MediumTest | Level1)
{
    unsigned long setOption[2];
    unsigned long getOption[2];
    char name[] = "thread_name";
    char buffer[BUFFER_SIZE] = { 0 };

    // prctl set and get PR_SET_NAME test success
    int ret = prctl(PR_SET_NAME, name);
    EXPECT_EQ(ret, 0);

    ret = prctl(PR_GET_NAME, buffer);
    EXPECT_EQ(ret, 0);
    EXPECT_STREQ(buffer, name);

    // prctl set and get PR_SET_PDEATHSIG success
    setOption[0] = SIGUSR2;
    ret = prctl(PR_SET_PDEATHSIG, setOption[0]);
    EXPECT_EQ(ret, 0);

    getOption[0] = 0;
    ret = prctl(PR_GET_PDEATHSIG, &getOption[0]);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(getOption[0], setOption[0]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRCTL_0200
 * @tc.name   : PrctlOptionTestSuccess_0002
 * @tc.desc   : prctl option PR_SET_KEEPCAPS/PR_CAPBSET_READ/PR_CAPBSET_DROP/PR_CAP_AMBIENT test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPrctlSyscallTest, PrctlOptionTestSuccess_0002, Function | MediumTest | Level1)
{
    unsigned long setOption[2];
    unsigned long getOption[2];

    // prctl PR_SET_KEEPCAPS and PR_GET_KEEPCAPS success
    setOption[0] = 1;
    int ret = prctl(PR_SET_KEEPCAPS, setOption[0]);
    EXPECT_EQ(ret, 0);

    ret = prctl(PR_GET_KEEPCAPS, &getOption[0]);
    EXPECT_GE(ret, 0);

    // prctl PR_CAPBSET_READ success
    setOption[0] = 1;
    ret = prctl(PR_CAPBSET_READ, setOption[0]);
    EXPECT_GE(ret, 0);

    // prctl PR_CAPBSET_DROP success
    setOption[0] = 0;
    setOption[0] = 0;
    ret = prctl(PR_CAPBSET_DROP, setOption[0], setOption[1], 0, 0);
    EXPECT_EQ(ret, 0);

    // prctl PR_CAP_AMBIENT success
    setOption[0] = 1;
    setOption[0] = ULONG_MAX;
    ret = prctl(PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, 0, 0, 0);
    EXPECT_EQ(ret, 0);

    size_t pageSize = static_cast<size_t>(sysconf(_SC_PAGESIZE));
    void *addr = nullptr;
    addr = mmap(NULL, pageSize * NUMBER_PAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    ret = prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, addr, pageSize * NUMBER_PAGE, "anonymous map space");
    EXPECT_EQ(ret, 0);
    munmap(addr, pageSize * NUMBER_PAGE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRCTL_0300
 * @tc.name   : PrctlOptionTestSuccess_0003
 * @tc.desc   : prctl option PR_GET_SECUREBITS/PR_GET_GECUREBITS/PR_SET_DUMPABLE/PR_GET_DUMPABLE test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPrctlSyscallTest, PrctlOptionTestSuccess_0003, Function | MediumTest | Level1)
{
    unsigned long setOption[2];
    unsigned long getOption[2];

    // prctl PR_GET_SECUREBITS and PR_GET_GECUREBITS success
    int ret = prctl(PR_SET_SECUREBITS, PR_SET_SECUREBITS, 0, 0, 0);
    EXPECT_EQ(ret, 0);

    getOption[0] = 0;
    ret = prctl(PR_GET_SECUREBITS, &getOption[0]);
    EXPECT_GE(ret, 0);

    // prctl PR_SET_DUMPABLE and PR_GET_DUMPABLE success
    setOption[0] = 1;
    setOption[1] = 0;
    ret = prctl(PR_SET_DUMPABLE, setOption[0], setOption[1], 0, 0);
    EXPECT_EQ(ret, 0);

    ret = prctl(PR_GET_DUMPABLE, &getOption[0]);
    EXPECT_GE(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRCTL_0400
 * @tc.name   : PrctlOptionTestSuccess_0004
 * @tc.desc   : prctl option PR_SET_CHILD_SUBREAPER/PR_GET_CHILD_SUBREAPER test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPrctlSyscallTest, PrctlOptionTestSuccess_0004, Function | MediumTest | Level1)
{
    unsigned long setOption[2];
    unsigned long getOption[2];

    // prctl PR_SET_CHILD_SUBREAPER and PR_GET_CHILD_SUBREAPER success
    setOption[0] = 1;
    int ret = prctl(PR_SET_CHILD_SUBREAPER, setOption[0]);
    EXPECT_EQ(ret, 0);

    getOption[0] = 0;
    ret = prctl(PR_GET_CHILD_SUBREAPER, &getOption[0]);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(getOption[0], setOption[0]);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PRCTL_0500
 * @tc.name   : PrctlOptionTestSuccess_0005
 * @tc.desc   : prctl option PR_SET_NO_NEW_PRIVS/PR_GET_NO_NEW_PRIVS test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsPrctlSyscallTest, PrctlOptionTestSuccess_0005, Function | MediumTest | Level1)
{
    // prctl PR_SET_NO_NEW_PRIVS and PR_GET_NO_NEW_PRIVS success
    int ret = prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);
    EXPECT_EQ(ret, 0);

    ret = prctl(PR_GET_NO_NEW_PRIVS, 0, 0, 0, 0);
    EXPECT_GE(ret, 0);
}
