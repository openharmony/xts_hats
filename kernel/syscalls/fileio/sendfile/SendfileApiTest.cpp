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

#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

static const char *FIRST_FILE = "/data/local/tmp/first";
static const char *SEC_FILE = "/data/local/tmp/second";
static const char* DATA = "Hello, sendfile!";
static const int STR_COUNT = 256;
static const int LOW_COUNT = -1;

class SendfileApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void SendfileApiTest::SetUp()
{
}
void SendfileApiTest::TearDown()
{
}
void SendfileApiTest::SetUpTestCase()
{
}
void SendfileApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDFILE_0100
 * @tc.name   : SendfileSuccess_0001
 * @tc.desc   : sendfile success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(SendfileApiTest, SendfileSuccess_0001, Function | MediumTest | Level1)
{
    int firFd = open(FIRST_FILE, O_CREAT | O_RDWR, 0644);
    EXPECT_GE(firFd, 0);
    write(firFd, DATA, sizeof(DATA));
    int secFd = open(SEC_FILE, O_CREAT | O_RDWR, 0644);
    EXPECT_GE(secFd, 0);

    ssize_t size = sendfile(secFd, firFd, nullptr, STR_COUNT);
    EXPECT_GE(size, 0);
    size = sendfile(secFd, firFd, nullptr, LOW_COUNT);
    if (sizeof(long) == 8) {
        EXPECT_EQ(size, -1);
    } else {
        EXPECT_EQ(size, 0); 
    }
    unlink(FIRST_FILE);
    unlink(SEC_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SENDFILE_0200
 * @tc.name   : SendfileInvalidFdFailed_0002
 * @tc.desc   : sendfile -1 fd errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(SendfileApiTest, SendfileInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    errno = 0;
    ssize_t size = sendfile(-1, -1, nullptr, STR_COUNT);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
}