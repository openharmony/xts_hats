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
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <linux/capability.h>
#include <sys/capability.h>
#include <sys/mman.h>
#include <sys/types.h>

using namespace testing::ext;
using namespace std;

static const int INVALID_VERSION = 0;
static const int INVALID_PID = -1;
static const int UNUSED_PID = 15464648;
static pid_t g_pid;

class CapGetApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};

void CapGetApiTest::SetUp()
{
    g_pid = getpid();
}

void CapGetApiTest::TearDown()
{
}

void CapGetApiTest::SetUpTestCase()
{
}

void CapGetApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPGET_0100
 * @tc.name   : CapGetVersionSuccess_0001
 * @tc.desc   : capget version 1 2 3 and data success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(CapGetApiTest, CapGetVersionSuccess_0001, Function | MediumTest | Level1)
{
    struct __user_cap_header_struct hdr;
    struct __user_cap_data_struct data[2];
    hdr.pid = g_pid;

    hdr.version = _LINUX_CAPABILITY_VERSION_1;
    int ret = capget(&hdr, data);
    EXPECT_TRUE(ret >= 0);

    hdr.version = _LINUX_CAPABILITY_VERSION_2;
    ret = capget(&hdr, data);
    EXPECT_TRUE(ret >= 0);

    hdr.version = _LINUX_CAPABILITY_VERSION_3;
    ret = capget(&hdr, data);
    EXPECT_TRUE(ret >= 0);

    hdr.version = _LINUX_CAPABILITY_VERSION_2;
    ret = capget(&hdr, nullptr);
    EXPECT_TRUE(ret >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPGET_0200
 * @tc.name   : CapGetInvalidVersionFailed_0002
 * @tc.desc   : capget invalid version return -1 errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CapGetApiTest, CapGetInvalidVersionFailed_0002, Function | MediumTest | Level2)
{
    struct __user_cap_header_struct hdr;
    struct __user_cap_data_struct data[2];
    hdr.pid = g_pid;

    hdr.version = INVALID_VERSION;
    errno = 0;
    int ret = capget(&hdr, data);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPGET_0300
 * @tc.name   : CapGetInvalidPidFailed_0003
 * @tc.desc   : capget invalid g_pid return -1 errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CapGetApiTest, CapGetInvalidPidFailed_0003, Function | MediumTest | Level2)
{
    struct __user_cap_header_struct hdr;
    struct __user_cap_data_struct data[2];
    hdr.pid = INVALID_PID;
    hdr.version = _LINUX_CAPABILITY_VERSION_2;

    errno = 0;
    int ret = capget(&hdr, data);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPGET_0400
 * @tc.name   : CapGetUnUsedPidFailed_0004
 * @tc.desc   : capget unused g_pid return -1 errno ESRCH.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CapGetApiTest, CapGetUnUsedPidFailed_0004, Function | MediumTest | Level2)
{
    struct __user_cap_header_struct hdr;
    struct __user_cap_data_struct data[2];
    hdr.pid = UNUSED_PID;
    hdr.version = _LINUX_CAPABILITY_VERSION_2;

    errno = 0;
    int ret = capget(&hdr, data);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, ESRCH);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPGET_0500
 * @tc.name   : CapGetInvalidAddressFailed_0005
 * @tc.desc   : capget invalid address return -1 errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CapGetApiTest, CapGetInvalidAddressFailed_0005, Function | MediumTest | Level2)
{
    struct __user_cap_data_struct data[2];

    errno = 0;
    int ret = capget(nullptr, data);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}