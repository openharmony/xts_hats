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

class HatsSemgetTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSemgetTest::SetUp()
{
}
void HatsSemgetTest::TearDown()
{
}
void HatsSemgetTest::SetUpTestCase()
{
}
void HatsSemgetTest::TearDownTestCase()
{
}

static const int SEMPAHORE_NUM = 1;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SEMGET_0100
 * @tc.name   : SemgetCreateSemaphoreSuccess_0001
 * @tc.desc   : semget create a semaphore set success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSemgetTest, SemgetCreateSemaphoreSuccess_0001, Function | MediumTest | Level1)
{
    int semid  = semget(IPC_PRIVATE, SEMPAHORE_NUM, IPC_CREAT | 0777);
    EXPECT_TRUE(semid >= 0);

    int ret = semctl(semid, SEMPAHORE_NUM, IPC_RMID);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SEMGET_0200
 * @tc.name   : SemgetCreateSempahoreSetUsingSameKeyFail_0002
 * @tc.desc   : semget create a sempahore set using a valid but used key fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSemgetTest, SemgetCreateSempahoreSetUsingSameKeyFail_0002, Function | MediumTest | Level2)
{
    int semid1;
    int semid2;
    int key = 1234;

    semid1 = semget(key, SEMPAHORE_NUM, IPC_CREAT | 0777);
    EXPECT_TRUE(semid1 >= 0);
    errno = 0;
    semid2 = semget(key, SEMPAHORE_NUM, IPC_CREAT | IPC_EXCL | 0777);
    EXPECT_TRUE(semid2 == -1);
    EXPECT_EQ(errno, EEXIST);

    int ret = semctl(semid1, SEMPAHORE_NUM, IPC_RMID);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SEMGET_0300
 * @tc.name   : SemgetInvalidSemnumFail_0003
 * @tc.desc   : semget create a sempahore set with invalid semaphore number fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSemgetTest, SemgetInvalidSemnumFail_0003, Function | MediumTest | Level2)
{
    errno = 0;
    int semid = semget(IPC_PRIVATE, -1, IPC_CREAT | 0777);
    EXPECT_TRUE(semid == -1);
    EXPECT_EQ(errno, EINVAL);
}
