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
#include <sys/types.h>
#include "securec.h"


using namespace testing::ext;

static const char *TEST_FILE = "/data/local/tmp/mmap_test";

class HatsMadvisecallTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMadvisecallTest::SetUp()
{
}

void HatsMadvisecallTest::TearDown()
{
}

void HatsMadvisecallTest::SetUpTestCase()
{
}

void HatsMadvisecallTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_0100
 * @tc.name   : MadvisecallForkSuccess_0001
 * @tc.desc   : Madvise sets advise MADV_DOFORK successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallForkSuccess_0001, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 1024;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_DOFORK);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_0200
 * @tc.name   : MadvisecallNormalSuccess_0002
 * @tc.desc   : Madvise sets advise MADV_NORMAL successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallNormalSuccess_0002, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 2048;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_NORMAL);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_0300
 * @tc.name   : MadvisecallWillneedSuccess_0003
 * @tc.desc   : Madvise sets advise MADV_WILLNEED successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallWillneedSuccess_0003, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 4096;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_WILLNEED);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_0400
 * @tc.name   : MadvisecallDontforkSuccess_0004
 * @tc.desc   : Madvise sets advise MADV_DONTFORK successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallDontforkSuccess_0004, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 1024;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_DONTFORK);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_0500
 * @tc.name   : MadvisecallSequentialSuccess_0005
 * @tc.desc   : Madvise sets advise MADV_SEQUENTIAL successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallSequentialSuccess_0005, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 2048;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_SEQUENTIAL);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_0600
 * @tc.name   : MadvisecallRandomSuccess_0006
 * @tc.desc   : Madvise sets advise MADV_RANDOM successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallRandomSuccess_0006, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 4096;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_RANDOM);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_0700
 * @tc.name   : MadvisecallDontneedSuccess_0007
 * @tc.desc   : Madvise sets advise MADV_DONTNEED successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallDontneedSuccess_0007, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 8192;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_DONTNEED);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_0800
 * @tc.name   : MadvisecallDontdumpSuccess_0008
 * @tc.desc   : Madvise sets advise MADV_DONTDUMP successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallDontdumpSuccess_0008, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 1024;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_DONTDUMP);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_0900
 * @tc.name   : MadvisecallDodumpSuccess_0009
 * @tc.desc   : Madvise sets advise MADV_DODUMP successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallDodumpSuccess_0009, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 2048;

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_DODUMP);
    EXPECT_EQ(ret, 0);
    munmap(ptr, size);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MADVISECALL_1000
 * @tc.name   : MadvisecallRemoveSuccess_0010
 * @tc.desc   : Madvise sets advise MADV_REMOVE successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsMadvisecallTest, MadvisecallRemoveSuccess_0010, Function | MediumTest | Level1)
{
    void *ptr;
    int ret;
    size_t size = 2048;

    int fd = open(TEST_FILE, O_CREAT | O_RDWR, 0664);

    ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_NE(ptr, MAP_FAILED);

    ret = madvise(ptr, size, MADV_REMOVE);
    EXPECT_EQ(ret, 0);

    ret = munmap(ptr, size);
    EXPECT_EQ(ret, 0);
    close(fd);
}