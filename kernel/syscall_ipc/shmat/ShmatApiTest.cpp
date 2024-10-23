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

class ShmatApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ShmatApiTest::SetUp()
{
}
void ShmatApiTest::TearDown()
{
}
void ShmatApiTest::SetUpTestCase()
{
}
void ShmatApiTest::TearDownTestCase()
{
}

const int NAME_SIZE = 50;
const int MEM_SIZE = 1024;

typedef struct {
    int id;
    char name[NAME_SIZE];
} TestData;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHMAT_0100
 * @tc.name   : ShmdtAttachShmSuccess_0001
 * @tc.desc   : shmat attaches shared memory segment by shmid success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ShmatApiTest, ShmdtAttachShmSuccess_0001, Function | MediumTest | Level1)
{
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, MEM_SIZE, 0644 | IPC_CREAT | IPC_EXCL);
    EXPECT_TRUE(shmid >= 0);

    void* addr = nullptr;
    addr = shmat(shmid, nullptr, 0);
    EXPECT_TRUE(addr != nullptr);

    int ret = shmctl(shmid, IPC_RMID, nullptr);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHMAT_0200
 * @tc.name   : ShmatUsedInvalidShmidFailed_0002
 * @tc.desc   : shmat attaches shared memory segment by invalid shmid failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ShmatApiTest, ShmatUsedInvalidShmidFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    void* addr = shmat(-1, nullptr, 0);
    EXPECT_TRUE(addr == reinterpret_cast<void*>(-1));
    EXPECT_TRUE(errno == EINVAL);
}