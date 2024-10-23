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

class HatsSemopTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSemopTest::SetUp()
{
}
void HatsSemopTest::TearDown()
{
}
void HatsSemopTest::SetUpTestCase()
{
}
void HatsSemopTest::TearDownTestCase()
{
}

static const int SEMPAHORE_NUM = 1;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SEMOP_0100
 * @tc.name   : SemopAddSemvalSuccess_0001
 * @tc.desc   : semop add semaphore value success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSemopTest, SemopAddSemvalSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int semid;
    struct sembuf sop;
    sop.sem_op = 1;

    semid = semget(IPC_PRIVATE, SEMPAHORE_NUM, IPC_CREAT | 0666);
    EXPECT_TRUE(semid >= 0);

    ret = semop(semid, &sop, 1);
    EXPECT_TRUE(ret == 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SEMOP_0200
 * @tc.name   : SemopInvalidSemidFail_0002
 * @tc.desc   : semop add semaphore value of a invalid sephamore id fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSemopTest, SemopInvalidSemidFail_0002, Function | MediumTest | Level2)
{
    int ret;
    int semid;
    struct sembuf sop;
    sop.sem_op = 1;
    semid = -1;

    errno = 0;
    ret = semop(semid, &sop, 1);
    EXPECT_TRUE(ret == -1);
    EXPECT_EQ(errno, EINVAL);
}
