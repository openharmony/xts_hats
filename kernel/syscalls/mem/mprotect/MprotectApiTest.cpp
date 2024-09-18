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
#include <malloc.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "securec.h"

using namespace testing::ext;

class HatsMprotectTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMprotectTest::SetUp()
{
}

void HatsMprotectTest::TearDown()
{
}

void HatsMprotectTest::SetUpTestCase()
{
}

void HatsMprotectTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MPROTECT_0100
 * @tc.name MprotectSuccess_0001
 * @tc.desc mprotect changes the access protections to PROT_WRITE successfully.
*/
HWTEST_F(HatsMprotectTest, MprotectSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int pagesize;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    EXPECT_NE(sigaction(SIGSEGV, &sa, nullptr), -1);

    pagesize = sysconf(_SC_PAGE_SIZE);
    EXPECT_NE(pagesize, -1);

    // Allocate a buffer aligned on a page boundary; initial protection is PROT_WRITE.
    void *buffer = memalign(pagesize, 4 * pagesize);
    EXPECT_NE(buffer, nullptr);

    ret = mprotect(buffer, pagesize, PROT_WRITE);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MPROTECT_0200
 * @tc.name MprotectSuccess_0002
 * @tc.desc mprotect changes the access protections to PROT_EXEC successfully.
*/
HWTEST_F(HatsMprotectTest, MprotectSuccess_0002, Function | MediumTest | Level1)
{
    int ret;
    int pagesize;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    EXPECT_NE(sigaction(SIGSEGV, &sa, nullptr), -1);

    pagesize = sysconf(_SC_PAGE_SIZE);
    EXPECT_NE(pagesize, -1);

    // Allocate a buffer aligned on a page boundary; initial protection is PROT_EXEC.
    void *buf = memalign(pagesize, 4 * pagesize);
    EXPECT_NE(buf, nullptr);

    ret = mprotect(buf, pagesize, PROT_EXEC);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MPROTECT_0300
 * @tc.name MprotectInvalid_0003
 * @tc.desc mprotect changes the access protections to PROT_READ failed for invalid address,error code is ENOMEM.
*/
HWTEST_F(HatsMprotectTest, MprotectInvalid_0003, Function | MediumTest | Level2)
{
    int ret;
    int pagesize;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    EXPECT_NE(sigaction(SIGSEGV, &sa, nullptr), -1);

    pagesize = sysconf(_SC_PAGE_SIZE);
    EXPECT_NE(pagesize, -1);

    void *buf = nullptr;
    errno = 0;
    ret = mprotect(buf, pagesize, PROT_READ);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOMEM);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MPROTECT_0400
 * @tc.name MprotectInvalid_0004
 * @tc.desc mprotect changes the access protections to PROT_READ failed for flag invalid, error code is EINVAL.
*/
HWTEST_F(HatsMprotectTest, MprotectInvalid_0004, Function | MediumTest | Level2)
{
    int ret;
    int pagesize;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    EXPECT_NE(sigaction(SIGSEGV, &sa, nullptr), -1);

    pagesize = sysconf(_SC_PAGE_SIZE);
    EXPECT_NE(pagesize, -1);

    // Allocate a buffer aligned on a page boundary; initial protection is invalid value of 0xFF.
    void *buf = memalign(pagesize, 4 * pagesize);
    EXPECT_NE(buf, nullptr);

    errno = 0;
    ret = mprotect(buf, pagesize, 0xFF);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_MPROTECT_0500
 * @tc.name MprotectInvalid_0005
 * @tc.desc When both PROT_GROWSUP and PROT_GROWSDOWN were specified, mprotect changes the access protections failed,
   and errno is EINVAL.
*/
HWTEST_F(HatsMprotectTest, MprotectInvalid_0005, Function | MediumTest | Level2)
{
    int ret;
    int pagesize;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    EXPECT_NE(sigaction(SIGSEGV, &sa, nullptr), -1);

    pagesize = sysconf(_SC_PAGE_SIZE);
    EXPECT_NE(pagesize, -1);

    // Allocate a buffer aligned on a page boundary; initial protection are PROT_GROWSUP and PROT_GROWSDOWN.
    void *buf = memalign(pagesize, 4 * pagesize);
    EXPECT_NE(buf, nullptr);

    errno = 0;
    ret = mprotect(buf, pagesize, PROT_GROWSUP | PROT_GROWSDOWN);

    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}
