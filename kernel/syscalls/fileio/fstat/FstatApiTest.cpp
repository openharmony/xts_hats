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
#include "securec.h"

using namespace testing::ext;
using namespace std;

class FstatApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FstatApiTest::SetUp()
{
}
void FstatApiTest::TearDown()
{
}
void FstatApiTest::SetUpTestCase()
{
}
void FstatApiTest::TearDownTestCase()
{
}

static const char *TEST_FILE = "/data/local/tmp/fstat.txt";
mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSTAT_0100
 * @tc.name   : FstatValidFdSuccess_0001
 * @tc.desc   : fstat a valid fd success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FstatApiTest, FstatValidFdSuccess_0001, Function | MediumTest | Level1)
{
    struct stat stat = { 0 };
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    int ret = fstat(fd, &stat);
    EXPECT_EQ(ret, 0);

    close(fd);
    remove(TEST_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FSTAT_0200
 * @tc.name   : FstatInvalidFdFailed_0002
 * @tc.desc   : fstat invalid fd fail, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FstatApiTest, FstatInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    struct stat stat = { 0 };
    errno = 0;
    int ret = fstat(-1, &stat);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}
