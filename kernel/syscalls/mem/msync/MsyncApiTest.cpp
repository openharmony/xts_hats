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

const int SIZE = 1024;
static const char* TEST_FILE = "/data/local/tmp/test_file.txt";

class HatsMsyncTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsMsyncTest::SetUp()
{
}

void HatsMsyncTest::TearDown()
{
}

void HatsMsyncTest::SetUpTestCase()
{
}

void HatsMsyncTest::TearDownTestCase()
{
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Msync_0100
 * @tc.name MsyncInvalidateSuccess_0001
 * @tc.desc Msync sets flag MS_INVALIDATE successfully.
*/
HWTEST_F(HatsMsyncTest, MsyncInvalidateSuccess_0001, Function | MediumTest | Level1)
{
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fd > 0);

    ftruncate(fd, SIZE);

    const char *initialData = "Hello, msync invalidate!";

    void *addr = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_NE(addr, MAP_FAILED);

    char *data = (char *) addr;
    size_t numToCopy = strlen(initialData);
    int retCpy = memcpy_s(data, SIZE, initialData, numToCopy + 1);
    EXPECT_EQ(retCpy, 0);

    int ret = msync(addr, SIZE, MS_INVALIDATE);
    EXPECT_EQ(ret, 0);

    const char *expectedData = "Hello, msync invalidate!";
    ret = strcmp((char *)addr, expectedData);
    EXPECT_EQ(ret, 0);

    lseek(fd, 0, SEEK_SET);
    char buffer[SIZE];
    read(fd, buffer, SIZE);
    EXPECT_EQ(strcmp(buffer, initialData), 0);

    munmap(addr, SIZE);
    close(fd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Msync_0200
 * @tc.name MsyncAsyncSuccess_0002
 * @tc.desc Msync sets flag MS_ASYNC successfully.
*/
HWTEST_F(HatsMsyncTest, MsyncAsyncSuccess_0002, Function | MediumTest | Level1)
{
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fd > 0);

    ftruncate(fd, SIZE);

    const char *initialData = "Hello, msync async!";
    void *addr = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_NE(addr, MAP_FAILED);

    char *data = (char *) addr;
    size_t numToCopy = strlen(initialData);
    int retCpy = memcpy_s(data, SIZE, initialData, numToCopy + 1);
    EXPECT_EQ(retCpy, 0);

    int ret = msync(addr, SIZE, MS_ASYNC);
    EXPECT_EQ(ret, 0);

    usleep(200000);

    lseek(fd, 0, SEEK_SET);
    char buffer[SIZE];
    read(fd, buffer, SIZE);
    EXPECT_EQ(strcmp(buffer, "Hello, msync async!"), 0);

    munmap(addr, SIZE);
    close(fd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Msync_0300
 * @tc.name MsyncSyncSuccess_0003
 * @tc.desc Msync sets flag MS_SYNC successfully.
*/
HWTEST_F(HatsMsyncTest, MsyncSyncSuccess_0003, Function | MediumTest | Level1)
{
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fd > 0);

    ftruncate(fd, SIZE);

    const char *initialData = "Hello, msync sync!";
    void *addr = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_NE(addr, MAP_FAILED);

    char *data = (char *) addr;
    size_t numToCopy = strlen(initialData);
    int retCpy = memcpy_s(data, SIZE, initialData, numToCopy + 1);
    EXPECT_EQ(retCpy, 0);

    int ret = msync(addr, SIZE, MS_SYNC);
    EXPECT_EQ(ret, 0);

    lseek(fd, 0, SEEK_SET);
    char buffer[SIZE];
    read(fd, buffer, SIZE);
    EXPECT_EQ(strcmp(buffer, "Hello, msync sync!"), 0);

    munmap(addr, SIZE);
    close(fd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Msync_0400
 * @tc.name MsyncInvalidAddrFailed_0004
 * @tc.desc Msync is failed for invalid addr, errno EINVAL.
*/
HWTEST_F(HatsMsyncTest, MsyncInvalidAddrFailed_0004, Function | MediumTest | Level2)
{
    void *invalidAddr = reinterpret_cast<void *>(0x1234);
    int ret = msync(invalidAddr, SIZE, MS_SYNC);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_Msync_0500
 * @tc.name MsyncNullAddrFailed_0005
 * @tc.desc Msync is failed for null addr, errno ENOMEM.
*/
HWTEST_F(HatsMsyncTest, MsyncNullAddrFailed_0005, Function | MediumTest | Level2)
{
    char *data = nullptr;

    int ret = msync(data, SIZE, MS_INVALIDATE);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ENOMEM);
}