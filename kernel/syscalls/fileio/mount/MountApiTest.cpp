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
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

const char *SOURCE = "/mnt/source01";
const char *TARGET = "/mnt/target01";
mode_t MODE_0755 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

class MountApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MountApiTest::SetUp()
{
}
void MountApiTest::TearDown()
{
}
void MountApiTest::SetUpTestCase()
{
    mkdir(SOURCE, MODE_0755);
    mkdir(TARGET, MODE_0755);
}
void MountApiTest::TearDownTestCase()
{
    rmdir(SOURCE);
    rmdir(TARGET);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_0100
 * @tc.name   : MountSuccess_0001
 * @tc.desc   : Mount dir success by data.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0001, Function | MediumTest | Level1)
{
    const char *filesystemtype = "ext4";
    const char *data = "rw";

    int ret = mount(SOURCE, TARGET, filesystemtype, MS_BIND, data);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_0200
 * @tc.name   : MountSuccess_0002
 * @tc.desc   : Mount dir success by MS_BIND.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0002, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_0300
 * @tc.name   : MountSuccess_0003
 * @tc.desc   : Mount dir success by MS_REC.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0003, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_REC, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_0400
 * @tc.name   : Mountsuccess_0004
 * @tc.desc   : Mount dir success by MS_NOATIME.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, Mountsuccess_0004, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_NOATIME, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_0500
 * @tc.name   : MountSuccess_0005
 * @tc.desc   : Mount dir success by MS_NODIRATIME.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0005, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_NODIRATIME, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_0600
 * @tc.name   : MountSuccess_0006
 * @tc.desc   : Mount dir success by MS_MOVE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0006, Function | MediumTest | Level1)
{
    const char *TARGET1 = "/mnt/target60";
    const char *TARGET2 = "/mnt/target61";

    mkdir(TARGET1, MODE_0755);
    mkdir(TARGET2, MODE_0755);

    int ret = mount(SOURCE, TARGET1, nullptr, MS_BIND, nullptr);
    EXPECT_EQ(ret, 0);
    ret = mount(TARGET1, TARGET2, nullptr, MS_MOVE, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET2, flags);
    EXPECT_EQ(ret, 0);

    rmdir(TARGET1);
    rmdir(TARGET2);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_0700
 * @tc.name   : MountSuccess_0007
 * @tc.desc   : Mount dir success by MS_PRIVATE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0007, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_PRIVATE, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_0800
 * @tc.name   : MountSuccess_0008
 * @tc.desc   : Mount dir success by MS_RELATIME.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0008, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_RELATIME, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_0900
 * @tc.name   : MountSuccess_0009
 * @tc.desc   : Mount dir success by MS_RDONLY.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0009, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_RDONLY, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_1000
 * @tc.name   : MountSuccess_0010
 * @tc.desc   : Mount dir success by MS_NOSUID.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0010, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_NOSUID, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_1100
 * @tc.name   : MountSuccess_0011
 * @tc.desc   : Mount dir success by MS_NODEV.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0011, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_NODEV, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_1200
 * @tc.name   : MountSuccess_0012
 * @tc.desc   : Mount dir success by MS_NOEXEC.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0012, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_NOEXEC, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_1300
 * @tc.name   : MountSuccess_0013
 * @tc.desc   : Mount dir success by MS_MANDLOCK.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0013, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_MANDLOCK, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_1400
 * @tc.name   : MountFail_0014
 * @tc.desc   : Mount dir fail by MS_REMOUNT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountFail_0014, Function | MediumTest | Level1)
{
    errno = 0;
    int ret = mount(SOURCE, TARGET, nullptr, MS_REMOUNT | MS_RDONLY, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_1500
 * @tc.name   : MountSuccess_0015
 * @tc.desc   : Mount dir success by MS_REMOUNT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0015, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_NOEXEC, nullptr);
    EXPECT_EQ(ret, 0);
    ret = mount(nullptr, TARGET, nullptr, MS_REMOUNT, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MOUNT_1600
 * @tc.name   : MountSuccess_0016
 * @tc.desc   : Umount2 dir success by MS_REMOUNT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MountApiTest, MountSuccess_0016, Function | MediumTest | Level1)
{
    int ret = mount(SOURCE, TARGET, nullptr, MS_BIND | MS_NOEXEC, nullptr);
    EXPECT_EQ(ret, 0);
    ret = mount(nullptr, TARGET, nullptr, MS_REMOUNT, nullptr);
    EXPECT_EQ(ret, 0);

    int flags = MNT_FORCE;
    ret = umount2(TARGET, flags);
    EXPECT_EQ(ret, 0);
}