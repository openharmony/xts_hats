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
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>


using namespace testing::ext;
using namespace std;

class GetrusageApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void GetrusageApiTest::SetUp()
{
}
void GetrusageApiTest::TearDown()
{
}
void GetrusageApiTest::SetUpTestCase()
{
}
void GetrusageApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETRUSAGE_0100
 * @tc.name   : GetrusageSuccess_0001
 * @tc.desc   : Getrusage flags and get usage success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(GetrusageApiTest, GetrusageSuccess_0001, Function | MediumTest | Level1)
{
    struct rusage usage;
    int ret = getrusage(RUSAGE_SELF, &usage);
    EXPECT_EQ(ret, 0);
    ret = getrusage(RUSAGE_CHILDREN, &usage);
    EXPECT_EQ(ret, 0);
    ret = getrusage(RUSAGE_THREAD, &usage);
    EXPECT_EQ(ret, 0);
    EXPECT_GE(usage.ru_utime.tv_sec, 0);
    EXPECT_GE(usage.ru_maxrss, 0);
    EXPECT_GE(usage.ru_ixrss, 0);
    EXPECT_GE(usage.ru_minflt, 0);
    EXPECT_GE(usage.ru_majflt, 0);
    EXPECT_GE(usage.ru_nvcsw, 0);
    EXPECT_GE(usage.ru_nivcsw, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETRUSAGE_0200
 * @tc.name   : GetrusageNullUsageStructFail_0002
 * @tc.desc   : Getrusage failed and return -1 errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(GetrusageApiTest, GetrusageNullUsageStructFail_0002, Function | MediumTest | Level2)
{
    int ret = getrusage(RUSAGE_SELF, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}