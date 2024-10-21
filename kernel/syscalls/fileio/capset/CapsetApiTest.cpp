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

class CapSetApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};

void CapSetApiTest::SetUp()
{
    g_pid = getpid();
}

void CapSetApiTest::TearDown()
{
}

void CapSetApiTest::SetUpTestCase()
{
}

void CapSetApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPSET_0100
 * @tc.name   : CapSetVersionSuccess_0001
 * @tc.desc   : capset version 1 2 3 success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(CapSetApiTest, CapSetVersionSuccess_0001, Function | MediumTest | Level1)
{
    struct __user_cap_header_struct hdr;
    struct __user_cap_data_struct data[2];
    hdr.pid = g_pid;

    hdr.version = _LINUX_CAPABILITY_VERSION_1;
    int ret = capset(&hdr, data);
    EXPECT_TRUE(ret >= 0);

    ret = -1;
    hdr.version = _LINUX_CAPABILITY_VERSION_2;
    ret = capset(&hdr, data);
    EXPECT_TRUE(ret >= 0);

    ret = -1;
    hdr.version = _LINUX_CAPABILITY_VERSION_3;
    ret = capset(&hdr, data);
    EXPECT_TRUE(ret >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPSET_0200
 * @tc.name   : CapSetInvalidVersionFailed_0002
 * @tc.desc   : capset invalid version return -1 errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CapSetApiTest, CapSetInvalidVersionFailed_0002, Function | MediumTest | Level2)
{
    struct __user_cap_header_struct hdr;
    struct __user_cap_data_struct data[2];
    hdr.pid = g_pid;

    hdr.version = INVALID_VERSION;
    errno = 0;
    int ret = capset(&hdr, data);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPSET_0300
 * @tc.name   : CapSetInvalidPidFailed_0003
 * @tc.desc   : capset invalid g_pid return -1 errno EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CapSetApiTest, CapSetInvalidPidFailed_0003, Function | MediumTest | Level2)
{
    struct __user_cap_header_struct hdr;
    struct __user_cap_data_struct data[2];
    hdr.pid = INVALID_PID;
    hdr.version = _LINUX_CAPABILITY_VERSION_2;

    errno = 0;
    int ret = capset(&hdr, data);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EPERM);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPSET_0400
 * @tc.name   : CapSetUnUsedPidFailed_0004
 * @tc.desc   : capset unused g_pid return -1 errno ESRCH.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CapSetApiTest, CapSetUnUsedPidFailed_0004, Function | MediumTest | Level2)
{
    struct __user_cap_header_struct hdr;
    struct __user_cap_data_struct data[2];
    hdr.pid = UNUSED_PID;
    hdr.version = _LINUX_CAPABILITY_VERSION_2;

    errno = 0;
    int ret = capset(&hdr, data);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EPERM);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_CAPSET_0500
 * @tc.name   : CapSetInvalidAddressFailed_0005
 * @tc.desc   : capset invalid address return -1 errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(CapSetApiTest, CapSetInvalidAddressFailed_0005, Function | MediumTest | Level2)
{
    struct __user_cap_data_struct data[2];

    errno = 0;
    int ret = capset(nullptr, data);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}