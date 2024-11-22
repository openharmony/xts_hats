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

class ShmctlApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void ShmctlApiTest::SetUp()
{
}
void ShmctlApiTest::TearDown()
{
}
void ShmctlApiTest::SetUpTestCase()
{
}
void ShmctlApiTest::TearDownTestCase()
{
}

const int MEM_SIZE = 1024;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHMCTL_0100
 * @tc.name   : ShmctlCmdTestSuccess_0001
 * @tc.desc   : shmctl command test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(ShmctlApiTest, ShmctlCmdTestSuccess_0001, Function | MediumTest | Level1)
{
    key_t key = 1234;

    int shmid = shmget(key, MEM_SIZE, 0666 | IPC_CREAT);
    EXPECT_TRUE(shmid != -1);

    struct shmid_ds ds;
    int ret = shmctl(shmid, IPC_STAT, &ds);
    EXPECT_TRUE(ret == 0);

    ret = shmctl(shmid, IPC_SET, &ds);
    EXPECT_TRUE(ret == 0);

    ret = shmctl(shmid, IPC_RMID, nullptr);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHMCTL_0200
 * @tc.name   : ShmctlInvalidCommandFailed_0002
 * @tc.desc   : shmctl use invalid commands failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ShmctlApiTest, ShmctlInvalidCommandFailed_0002, Function | MediumTest | Level2)
{
    key_t key = 1234;

    int shmid = shmget(key, MEM_SIZE, 0666 | IPC_CREAT);
    EXPECT_TRUE(shmid != -1);

    errno = 0;
    int ret = shmctl(shmid, 999, nullptr);
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(errno == EINVAL);

    ret = shmctl(shmid, IPC_RMID, nullptr);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SHMCTL_0300
 * @tc.name   : ShmctlInvalidIdFailed_0003
 * @tc.desc   : shmctl use invalid id failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(ShmctlApiTest, ShmctlInvalidIdFailed_0003, Function | MediumTest | Level2)
{
    key_t key = 1234;

    int shmid = shmget(key, MEM_SIZE, 0666 | IPC_CREAT);
    EXPECT_TRUE(shmid != -1);

    errno = 0;
    int ret = shmctl(-1, IPC_STAT, nullptr);
    EXPECT_TRUE(ret == -1);
    EXPECT_TRUE(errno == EINVAL);

    ret = shmctl(shmid, IPC_RMID, nullptr);
    EXPECT_TRUE(ret == 0);
}
