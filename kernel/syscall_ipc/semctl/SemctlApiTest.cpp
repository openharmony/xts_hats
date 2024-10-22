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
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

union Semun {
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

static const int MAX_LEN = 1024;

class HatsSemctlTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSemctlTest::SetUp()
{
}
void HatsSemctlTest::TearDown()
{
}
void HatsSemctlTest::SetUpTestCase()
{
}
void HatsSemctlTest::TearDownTestCase()
{
}

static const int SEMPAHORE_NUM = 1;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SEMCTL_0100
 * @tc.name   : SemctlCmdTestSuccess_0001
 * @tc.desc   : semctl command test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSemctlTest, SemctlCmdTestSuccess_0001, Function | MediumTest | Level1)
{
    union Semun sem = {
        .val = 0,
    };
    char buffer[MAX_LEN] = { 0 };

    int semid  = semget(IPC_PRIVATE, SEMPAHORE_NUM, IPC_CREAT | 0777);
    EXPECT_TRUE(semid >= 0);

    int ret = semctl(semid, SEMPAHORE_NUM, IPC_SET, &sem);
    EXPECT_TRUE(ret == 0);

    ret = semctl(semid, SEMPAHORE_NUM, IPC_STAT, &buffer);
    EXPECT_TRUE(ret == 0);

    ret = semctl(semid, SEMPAHORE_NUM, IPC_RMID);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SEMCTL_0200
 * @tc.name   : SemctlUseInvalidIdFail_0002
 * @tc.desc   : semctl use valid id fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSemctlTest, SemctlUseInvalidIdFail_0002, Function | MediumTest | Level2)
{
    union Semun sem = {
        .val = 0,
    };
    errno = 0;
    int ret = semctl(-1, SEMPAHORE_NUM, IPC_STAT, &sem);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SEMCTL_0300
 * @tc.name   : SemctlUsdInvalidCmdFail_0003
 * @tc.desc   : semctl use valid cmd fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSemctlTest, SemctlUsdInvalidCmdFail_0003, Function | MediumTest | Level2)
{
    int semid  = semget(IPC_PRIVATE, SEMPAHORE_NUM, IPC_CREAT | 0777);
    EXPECT_TRUE(semid >= 0);

    errno = 0;
    int ret = semctl(semid, SEMPAHORE_NUM, 999);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    ret = semctl(semid, SEMPAHORE_NUM, IPC_RMID);
    EXPECT_TRUE(ret == 0);
}
