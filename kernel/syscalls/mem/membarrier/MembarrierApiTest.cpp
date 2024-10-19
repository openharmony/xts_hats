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
#include <unistd.h>
#include <gtest/gtest.h>
#include <linux/membarrier.h>
#include <sys/syscall.h>

using namespace testing::ext;
using namespace std;

class MembarrierApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MembarrierApiTest::SetUp()
{
}
void MembarrierApiTest::TearDown()
{
}
void MembarrierApiTest::SetUpTestCase()
{
}
void MembarrierApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MEMBARRIER_0100
 * @tc.name   : MembarrierValidCmdTestSuccess_0001
 * @tc.desc   : Membarrier MEMBARRIER_CMD_QUERY and MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED cmd test success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MembarrierApiTest, MembarrierValidCmdTestSuccess_0001, Function | MediumTest | Level1)
{
    int ret = syscall(__NR_membarrier, MEMBARRIER_CMD_QUERY, 0);
    EXPECT_TRUE(ret >= 0);

    ret = syscall(__NR_membarrier, MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MEMBARRIER_0200
 * @tc.name   : MembarrierInvalidCmdFailed_0002
 * @tc.desc   : membarrier verify memory masking by invalid cmd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MembarrierApiTest, MembarrierFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = syscall(__NR_membarrier, -1, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MEMBARRIER_0300
 * @tc.name   : MembarrierMEMBARRIER_CMD_PRIVATE_EXPEDITEDCmdSuccess_0003
 * @tc.desc   : membarrier MEMBARRIER_CMD_PRIVATE_EXPEDITED cmd test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MembarrierApiTest, MembarrierMEMBARRIER_CMD_PRIVATE_EXPEDITEDCmdSuccess_0003, Function | MediumTest | Level1)
{
    int ret = syscall(__NR_membarrier, MEMBARRIER_CMD_PRIVATE_EXPEDITED, 0);
    EXPECT_EQ(ret, 0);
}