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
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class GetcwdApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void GetcwdApiTest::SetUp()
{
}
void GetcwdApiTest::TearDown()
{
}
void GetcwdApiTest::SetUpTestCase()
{
}
void GetcwdApiTest::TearDownTestCase()
{
}

const int BUFFER_SIZE = 128;
/*
 * @tc.number : SUB_KERNEL_SYSCALL_GETCWD_0100
 * @tc.name   : GetcwdGetCurrentWorkingDirectorySuccess_0001
 * @tc.desc   : get path of the current working directory success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(GetcwdApiTest, GetcwdGetCurrentWorkingDirectorySuccess_0001, Function | MediumTest | Level1)
{
    char buff[BUFFER_SIZE] = {0};
    char* path = getcwd(buff, sizeof(buff));
    EXPECT_TRUE(path != nullptr);
    EXPECT_EQ(strlen(path), strlen(buff));
}