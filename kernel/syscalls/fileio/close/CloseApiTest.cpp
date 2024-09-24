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

class CloseApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void CloseApiTest::SetUp()
{
}
void CloseApiTest::TearDown()
{
}
void CloseApiTest::SetUpTestCase()
{
}
void CloseApiTest::TearDownTestCase()
{
}

static const char *TEST_FILE = "/data/local/tmp/close.txt";
static const mode_t MODE_0644 = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

/*
 * @tc.number SUB_KERNEL_SYSCALL_CLOSE_0100
 * @tc.name CloseFileSuccess_0001
 * @tc.desc close file descriptor success.
*/
HWTEST_F(CloseApiTest, CloseFileSuccess_0001, Function | MediumTest | Level1)
{
    int fd = open(TEST_FILE, O_RDWR | O_CREAT, MODE_0644);
    EXPECT_TRUE(fd > 0);

    int ret = close(fd);
    EXPECT_EQ(ret, 0);

    remove(TEST_FILE);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_CLOSE_0200
 * @tc.name CloseFileFail_0002
 * @tc.desc close invalid file descriptor failed.
*/
HWTEST_F(CloseApiTest, CloseFileFail_0002, Function | MediumTest | Level2)
{
    int fd = -1;
    errno = 0;
    int ret = close(fd);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}
