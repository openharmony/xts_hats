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
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;

class HatsProcessvmTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsProcessvmTest::SetUp()
{
}

void HatsProcessvmTest::TearDown()
{
}

void HatsProcessvmTest::SetUpTestCase()
{
}

void HatsProcessvmTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESSVM_0100
 * @tc.name   : ProcessvmReadSuccess_0001
 * @tc.desc   : process_vm_readv reads data successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsProcessvmTest, ProcessvmReadSuccess_0001, Function | MediumTest | Level1)
{
    pid_t pid = getpid();
    struct iovec local[2];
    struct iovec remote[1];

    char buf1[1024];
    char buf2[1024];
    local[0].iov_base = buf1;
    local[0].iov_len = sizeof(buf1);
    local[1].iov_base = buf2;
    local[1].iov_len = sizeof(buf2);

    char remoteData[] = "Hello, World!";
    remote[0].iov_base = remoteData;
    remote[0].iov_len = sizeof(remoteData);

    ssize_t nread = process_vm_readv(pid, local, 2, remote, 1, 0);
    EXPECT_NE(nread, -1);
    EXPECT_EQ(memcmp(buf1, remoteData, sizeof(remoteData)), 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_PROCESSVM_0200
 * @tc.name   : ProcessvmWriteSuccess_0002
 * @tc.desc   : process_vm_writev writes data successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsProcessvmTest, ProcessvmWriteSuccess_0002, Function | MediumTest | Level1)
{
    pid_t pid = getpid();
    struct iovec local[2];
    struct iovec remote[1];

    char writeData1[] = "Hello, ";
    char writeData2[] = "World!";
    local[0].iov_base = writeData1;
    local[0].iov_len = sizeof(writeData1) - 1;
    local[1].iov_base = writeData2;
    local[1].iov_len = sizeof(writeData2) - 1;

    char remoteData[1024] = {0};
    remote[0].iov_base = remoteData;
    remote[0].iov_len = sizeof(remoteData);

    ssize_t len = process_vm_writev(pid, local, 2, remote, 1, 0);
    EXPECT_NE(len, -1);
    EXPECT_EQ(strcmp(remoteData, "Hello, World!"), 0);
}
