/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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
#include <csignal>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscall.h>
#include "securec.h"

using namespace testing::ext;

enum SyslogActionType {
    SYSLOG_ACTION_CLOSE,
    SYSLOG_ACTION_OPEN,
    SYSLOG_ACTION_READ,
    SYSLOG_ACTION_READ_ALL,
    SYSLOG_ACTION_READ_CLEAR,
    SYSLOG_ACTION_CLEAR,
    SYSLOG_ACTION_CONSOLE_OFF,
    SYSLOG_ACTION_CONSOLE_ON,
    SYSLOG_ACTION_CONSOLE_LEVEL,
    SYSLOG_ACTION_SIZE_UNREAD,
    SYSLOG_ACTION_SIZE_BUFFER,
};

class HatsSyslogTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSyslogTest::SetUp()
{
}

void HatsSyslogTest::TearDown()
{
}

void HatsSyslogTest::SetUpTestCase()
{
}

void HatsSyslogTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0100
 * @tc.name   : SyslogSuccess_0001
 * @tc.desc   : syslog open and close the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSyslogTest, SyslogSuccess_0001, Function | MediumTest | Level2)
{
    int ret = syscall(__NR_syslog, SYSLOG_ACTION_OPEN, nullptr, 0);
    EXPECT_EQ(ret, 0);

    ret = syscall(__NR_syslog, SYSLOG_ACTION_CLOSE, nullptr, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0200
 * @tc.name   : SyslogSuccess_0002
 * @tc.desc   : syslog  size buffer the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogSuccess_0002, Function | MediumTest | Level1)
{
    int ret = syscall(__NR_syslog, SYSLOG_ACTION_SIZE_BUFFER, nullptr, 0);
    EXPECT_GE(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0300
 * @tc.name   : SyslogSuccess_0003
 * @tc.desc   : syslog clear and readall the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogSuccess_0003, Function | MediumTest | Level1)
{
    char buffer[100];

    int ret = syscall(__NR_syslog, SYSLOG_ACTION_CLEAR, nullptr, 0);
    EXPECT_EQ(ret, 0);

    ret = syscall(__NR_syslog, SYSLOG_ACTION_READ_ALL, buffer, sizeof(buffer));
    EXPECT_GE(ret, strlen(buffer));
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0400
 * @tc.name   : SyslogSuccess_0004
 * @tc.desc   : syslog clear and readclear the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogSuccess_0004, Function | MediumTest | Level1)
{
    char buffer[100];

    int ret = syscall(__NR_syslog, SYSLOG_ACTION_CLEAR, nullptr, 0);
    EXPECT_EQ(ret, 0);

    ret = syscall(__NR_syslog, SYSLOG_ACTION_READ_CLEAR, buffer, sizeof(buffer));
    EXPECT_GE(ret, strlen(buffer));
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0500
 * @tc.name   : SyslogSuccess_0005
 * @tc.desc   : syslog console on and off the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSyslogTest, SyslogSuccess_0005, Function | MediumTest | Level2)
{
    int ret = syscall(__NR_syslog, SYSLOG_ACTION_CONSOLE_ON, nullptr, 0);
    EXPECT_EQ(ret, 0);

    ret = syscall(__NR_syslog, SYSLOG_ACTION_CONSOLE_OFF, nullptr, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0600
 * @tc.name   : SyslogSuccess_0006
 * @tc.desc   : syslog level success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogSuccess_0006, Function | MediumTest | Level1)
{
    int ret;
    int i;

    for (i = 1; i < 9; i++) {
        ret = syscall(__NR_syslog, SYSLOG_ACTION_CONSOLE_LEVEL, nullptr, i);
        EXPECT_EQ(ret, 0);
    }
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SYSLOG_0700
 * @tc.name   : SyslogSuccess_0007
 * @tc.desc   : syslog  unread the log success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSyslogTest, SyslogSuccess_0007, Function | MediumTest | Level1)
{
    int ret = syscall(__NR_syslog, SYSLOG_ACTION_CLEAR, nullptr, 0);
    EXPECT_EQ(ret, 0);

    errno = 0;
    ret = syscall(__NR_syslog, SYSLOG_ACTION_SIZE_UNREAD, nullptr, 0);
    if (ret != -1) {
        EXPECT_GE(ret, 0);
    } else {
        EXPECT_EQ(errno, 38);
    }
}