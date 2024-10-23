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

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class ShmgetApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ShmgetApiTest::SetUp()
{
}
void ShmgetApiTest::TearDown()
{
}
void ShmgetApiTest::SetUpTestCase()
{
}
void ShmgetApiTest::TearDownTestCase()
{
}

const int MEM_SIZE = 1024;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHMGET_0100
 * @tc.name   : ShmgetCreateSharedMemoryObjectAndReturnIdentifierSuccess_0001
 * @tc.desc   : Create a shared memory object and return the shared memory identifier success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ShmgetApiTest, ShmgetCreateSharedMemoryObjectAndReturnIdentifierSuccess_0001, Function | MediumTest | Level1)
{
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, MEM_SIZE, 0666 | IPC_CREAT);
    EXPECT_TRUE(shmid != 0);

    int ret = shmctl(shmid, IPC_RMID, nullptr);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHMGET_0200
 * @tc.name   : ShmgetCreateSharedMemoryObjectAndReturnIdentifierSuccess_0002
 * @tc.desc   : Create a shared memory object and return the shared memory identifier success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ShmgetApiTest, ShmgetCreateSharedMemoryObjectAndReturnIdentifierSuccess_0002, Function | MediumTest | Level1)
{
    char* ptrSharedData = nullptr;
    const char* data = "Hello, shared memory!";
    key_t key = ftok("shmfile", 66);

    int shmid = shmget(key, MEM_SIZE, 0666 | IPC_CREAT);
    EXPECT_TRUE(shmid != 0);

    void* ptrData = shmat(shmid, nullptr, 0);
    EXPECT_TRUE(ptrData != reinterpret_cast<void*>(-1));

    ptrSharedData = (char*)ptrData;
    strncpy_s(ptrSharedData, MEM_SIZE, data, strlen(data));
    ptrSharedData[strlen(data)] = '\0';

    int ret = shmdt(ptrSharedData);
    EXPECT_TRUE(ret == 0);

    ret = shmctl(shmid, IPC_RMID, nullptr);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHMGET_0300
 * @tc.name   : ShmgetCreateSharedMemoryObjectAndReturnIdentifierSuccess_0003
 * @tc.desc   : Create a shared memory object and return the shared memory identifier success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ShmgetApiTest, ShmgetCreateSharedMemoryObjectAndReturnIdentifierSuccess_0003, Function | MediumTest | Level1)
{
    key_t key1 = ftok("shmfile1", 65);
    key_t key2 = ftok("shmfile2", 65);

    int shmid1 = shmget(key1, MEM_SIZE, IPC_CREAT | 0666);
    EXPECT_TRUE(shmid1 != -1);

    int shmid2 = shmget(key2, MEM_SIZE, IPC_CREAT | 0666);
    EXPECT_TRUE(shmid2 != -1);

    EXPECT_TRUE(shmid1 == shmid2);

    if (shmid1 != -1) {
        shmctl(shmid1, IPC_RMID, nullptr);
    }

    if (shmid2 != -1) {
        shmctl(shmid2, IPC_RMID, nullptr);
    }
}