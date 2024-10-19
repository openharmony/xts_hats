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
#include <csignal>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/inotify.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
static const char* TEST_FILE = "/data/local/tmp/test_notify.txt";
static const char* TEST_DIR = "/data/local/tmp/notify";


class HatsInotifyTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsInotifyTest::SetUp()
{
}

void HatsInotifyTest::TearDown()
{
}

void HatsInotifyTest::SetUpTestCase()
{
}

void HatsInotifyTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_0100
 * @tc.name   : InotifyAddWatchAccessSuccess_0001
 * @tc.desc   : Inotify adds watch with mask IN_ACCESS.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchAccessSuccess_0001, Function | MediumTest | Level1)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_0200
 * @tc.name   : InotifyAddWatchModifySuccess_0002
 * @tc.desc   : Inotify adds watch with mask IN_MODIFY、IN_ATTRIB、IN_CLOSE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchModifySuccess_0002, Function | MediumTest | Level1)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(IN_NONBLOCK);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_MODIFY | IN_ATTRIB | IN_CLOSE);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_0300
 * @tc.name   : InotifyAddWatchMoveSuccess_0003
 * @tc.desc   : Inotify adds watch with mask IN_MOVE、IN_OPEN、IN_CLOSE_NOWRITE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchMoveSuccess_0003, Function | MediumTest | Level1)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_MOVE | IN_OPEN | IN_CLOSE_NOWRITE);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_0400
 * @tc.name   : InotifyAddWatchCreateSuccess_0004
 * @tc.desc   : Inotify adds watch with mask IN_CLOSE_WRITE、IN_CREATE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchCreateSuccess_0004, Function | MediumTest | Level1)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_CLOSE_WRITE | IN_CREATE);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_0500
 * @tc.name   : InotifyAddWatchMoveFromSuccess_0005
 * @tc.desc   : Inotify adds watch with mask IN_MOVED_FROM、IN_DELETE_SELF、IN_MOVE_SELF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchMoveFromSuccess_0005, Function | MediumTest | Level1)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_MOVED_FROM | IN_DELETE_SELF | IN_MOVE_SELF);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_0600
 * @tc.name   : InotifyAddWatchMoveToSuccess_0006
 * @tc.desc   : Inotify adds watch with mask IN_MOVED_TO、IN_UNMOUNT、IN_IGNORED、IN_Q_OVERFLOW.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchMoveToSuccess_0006, Function | MediumTest | Level1)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_MOVED_TO | IN_UNMOUNT | IN_IGNORED | IN_Q_OVERFLOW);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_0700
 * @tc.name   : InotifyAddWatchDirSuccess_0007
 * @tc.desc   : Inotify adds watch with mask IN_ONLYDIR、IN_DONT_FOLLOW、IN_EXCL_UNLINK.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchDirSuccess_0007, Function | MediumTest | Level1)
{
    mkdir(TEST_DIR, 0777);

    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_DIR, IN_ONLYDIR | IN_DONT_FOLLOW | IN_EXCL_UNLINK);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_0800
 * @tc.name   : InotifyAddWatchMaskAddSuccess_0008
 * @tc.desc   : Inotify adds watch with mask IN_MASK_ADD、IN_ISDIR、IN_ONESHOT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchMaskAddSuccess_0008, Function | MediumTest | Level1)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_MASK_ADD | IN_ISDIR | IN_ONESHOT);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_0900
 * @tc.name   : InotifyAddWatchCloExecFailed_009
 * @tc.desc   : Inotify adds watch with mask IN_CLOEXEC.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchCloExecFailed_009, Function | MediumTest | Level2)
{
    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, nullptr, IN_DELETE);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, EFAULT);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1000
 * @tc.name   : InotifyAddWatchIN_CLOEXECTestSuccess_0010
 * @tc.desc   : inotify_adds_watch with mask IN_CLOEXEC success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchIN_CLOEXECTestSuccess_0010, Function | MediumTest | Level1)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_MASK_ADD);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    close(fd);
}