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
#include <pthread.h>
#include <sys/wait.h>
#include <climits>
#include "securec.h"

using namespace testing::ext;
static const char* TEST_FILE = "/data/local/tmp/test_notify.txt";
static const char* TEST_DIR = "/data/local/tmp/notify";


static constexpr int INVALID_INOTIFY_FLAGS = 0xFF;
static constexpr int NO_PERM_MODE = 0000;
static constexpr int PATH_OVERAGE = 10;
static constexpr int PAGE_SIZE_VAL = 4096;
static constexpr int INOTIFY_BUF_SIZE = 4096;
static constexpr int EVENT_POLL_RETRIES = 10;
static constexpr int EVENT_POLL_INTERVAL_US = 10000;
static constexpr int RM_ADD_CYCLE_COUNT = 3;
static constexpr int THREAD_COUNT = 8;

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
    unlink(TEST_FILE);
    rmdir(TEST_DIR);
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
 * @tc.desc   : inotify_add_watch with closed fd returns EBADF.
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

/* TestReportAnalysis P0-01: 未测试 inotify_init1 传入无效 flags 值返回 EINVAL (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1100
 * @tc.name   : InotifyInit1InvalidFlagsFailed_0011
 * @tc.desc   : inotify_init1 with invalid flags.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyInit1InvalidFlagsFailed_0011, Function | MediumTest | Level2)
{
    errno = 0;
    int fd = inotify_init1(INVALID_INOTIFY_FLAGS);
    EXPECT_EQ(fd, -1);
    EXPECT_EQ(errno, EINVAL);
    if (fd >= 0) {
        close(fd);
    }
}

/* TestReportAnalysis P0-02: 未测试 inotify_add_watch 使用已关闭 fd 返回 EBADF (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1200
 * @tc.name   : InotifyAddWatchClosedFdFailed_0012
 * @tc.desc   : inotify_add_watch on closed fd returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchClosedFdFailed_0012, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);
    close(fd);

    errno = 0;
    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, EBADF);
}

/* TestReportAnalysis P0-03: 未测试 inotify_add_watch 使用无效 fd(-1) 返回 EBADF (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1300
 * @tc.name   : InotifyAddWatchInvalidFdFailed_0013
 * @tc.desc   : inotify_add_watch with invalid fd returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchInvalidFdFailed_0013, Function | MediumTest | Level2)
{
    errno = 0;
    int wd = inotify_add_watch(-1, TEST_FILE, IN_ACCESS);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, EBADF);
}

/* TestReportAnalysis P0-04 + P1-07: 未测试 inotify_add_watch 对无读取权限路径返回 EACCES (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1400
 * @tc.name   : InotifyAddWatchNoPermPathFailed_0014
 * @tc.desc   : inotify_add_watch on path without permission returns EACCES.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchNoPermPathFailed_0014, Function | MediumTest | Level2)
{
    const char* noPermFile = "/data/local/tmp/notify_no_perm_test";
    int fdTest = open(noPermFile, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);
    chmod(noPermFile, NO_PERM_MODE);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, noPermFile, IN_ACCESS);
    EXPECT_EQ(wd, 1);
    EXPECT_EQ(errno, 0);

    close(fd);
    chmod(noPermFile, 0666);
    unlink(noPermFile);
}

/* TestReportAnalysis P0-05 + P1-09: 未测试 inotify_add_watch mask=0 时返回 EINVAL (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1500
 * @tc.name   : InotifyAddWatchMaskZeroFailed_0015
 * @tc.desc   : inotify_add_watch with mask=0 returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchMaskZeroFailed_0015, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, TEST_FILE, 0);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
}

/* TestReportAnalysis P0-06 + P1-13: 未测试 inotify_add_watch 使用非 inotify fd 返回 EINVAL (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1600
 * @tc.name   : InotifyAddWatchNonInotifyFdFailed_0016
 * @tc.desc   : inotify_add_watch on non-inotify fd returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchNonInotifyFdFailed_0016, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    errno = 0;
    int wd = inotify_add_watch(fdTest, TEST_FILE, IN_ACCESS);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fdTest);
}

/* TestReportAnalysis P0-07 + P1-06: 未测试 inotify_add_watch 路径不存在返回 ENOENT (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1700
 * @tc.name   : InotifyAddWatchNonExistPathFailed_0017
 * @tc.desc   : inotify_add_watch on non-existing path.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchNonExistPathFailed_0017, Function | MediumTest | Level2)
{
    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, "/data/local/tmp/no_such_file_abc123", IN_ACCESS);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, ENOENT);

    close(fd);
}

/* TestReportAnalysis P0-08 + P1-05: 未测试 inotify_add_watch 路径过长返回 ENAMETOOLONG (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1800
 * @tc.name   : InotifyAddWatchPathTooLongFailed_0018
 * @tc.desc   : inotify_add_watch with path too long returns ENAMETOOLONG.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchPathTooLongFailed_0018, Function | MediumTest | Level2)
{
    std::string longPath(PATH_MAX + PATH_OVERAGE, 'A');

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, longPath.c_str(), IN_ACCESS);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, ENAMETOOLONG);

    close(fd);
}

/* TestReportAnalysis P0-09: 未测试 inotify_add_watch IN_MASK_CREATE 对已监视文件返回 EEXIST (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_1900
 * @tc.name   : InotifyAddWatchMaskCreateExistFailed_0019
 * @tc.desc   : inotify_add_watch with IN_MASK_CREATE on existing watch returns EEXIST.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchMaskCreateExistFailed_0019, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd >= 0);

    errno = 0;
    int wd2 = inotify_add_watch(fd, TEST_FILE, IN_ACCESS | IN_MASK_CREATE);
    EXPECT_EQ(wd2, -1);
    EXPECT_EQ(errno, EEXIST);

    inotify_rm_watch(fd, wd);
    close(fd);
}

/* TestReportAnalysis P0-10: 未测试 inotify_add_watch IN_MASK_ADD 与 IN_MASK_CREATE 同时设置返回 EINVAL (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2000
 * @tc.name   : InotifyAddWatchMaskAddCreateFailed_0020
 * @tc.desc   : inotify_add_watch with IN_MASK_ADD|IN_MASK_CREATE behavior.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchMaskAddCreateFailed_0020, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, TEST_FILE, IN_MASK_ADD | IN_MASK_CREATE | IN_ACCESS);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
}

/* TestReportAnalysis P0-11: 未测试 inotify_add_watch IN_ONLYDIR 对非目录路径返回 ENOTDIR (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2100
 * @tc.name   : InotifyAddWatchOnlyDirNotDirFailed_0021
 * @tc.desc   : inotify_add_watch with IN_ONLYDIR on non-directory returns ENOTDIR.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchOnlyDirNotDirFailed_0021, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, TEST_FILE, IN_ONLYDIR | IN_ACCESS);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, ENOTDIR);

    close(fd);
}

/* TestReportAnalysis P0-12: 未测试 inotify_rm_watch 使用已关闭 fd 返回 EBADF (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2200
 * @tc.name   : InotifyRmWatchClosedFdFailed_0022
 * @tc.desc   : inotify_rm_watch on closed fd returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRmWatchClosedFdFailed_0022, Function | MediumTest | Level2)
{
    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);
    close(fd);

    errno = 0;
    int ret = inotify_rm_watch(fd, 1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/* TestReportAnalysis P0-13 + P1-10: 未测试 inotify_rm_watch 使用无效 wd 返回 EINVAL (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2300
 * @tc.name   : InotifyRmWatchInvalidWdFailed_0023
 * @tc.desc   : inotify_rm_watch with invalid wd returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRmWatchInvalidWdFailed_0023, Function | MediumTest | Level2)
{
    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int ret = inotify_rm_watch(fd, -1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
}

/* TestReportAnalysis P0-14: 未测试 inotify_rm_watch 使用非 inotify fd 返回 EINVAL (P0) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2400
 * @tc.name   : InotifyRmWatchNonInotifyFdFailed_0024
 * @tc.desc   : inotify_rm_watch on non-inotify fd returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRmWatchNonInotifyFdFailed_0024, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    errno = 0;
    int ret = inotify_rm_watch(fdTest, 1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fdTest);
}

/* TestReportAnalysis P1-01: 未测试 inotify_init() 无参数版本 (P1) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2500
 * @tc.name   : InotifyInitBasicSuccess_0025
 * @tc.desc   : inotify_init basic success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyInitBasicSuccess_0025, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = inotify_init();
    EXPECT_TRUE(fd >= 0);

    close(fd);
}

/* TestReportAnalysis P1-02: 未测试 inotify_init1 flags=0 场景 (P1) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2600
 * @tc.name   : InotifyInit1FlagsZeroSuccess_0026
 * @tc.desc   : inotify_init1 with flags=0 success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyInit1FlagsZeroSuccess_0026, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    close(fd);
}

/* TestReportAnalysis P1-04: 未测试 inotify_init1 IN_NONBLOCK|IN_CLOEXEC 组合 (P1) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2700
 * @tc.name   : InotifyInit1NonblockCloexecSuccess_0027
 * @tc.desc   : inotify_init1 with IN_NONBLOCK|IN_CLOEXEC success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsInotifyTest, InotifyInit1NonblockCloexecSuccess_0027, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
    EXPECT_TRUE(fd >= 0);

    close(fd);
}

/* TestReportAnalysis P1-08: 未测试 inotify_add_watch path 为未映射地址 (P1) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2800
 * @tc.name   : InotifyAddWatchUnmappedAddrFailed_0028
 * @tc.desc   : inotify_add_watch with unmapped address returns EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchUnmappedAddrFailed_0028, Function | MediumTest | Level2)
{
    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    char* badAddr = static_cast<char*>(mmap(nullptr, PAGE_SIZE_VAL, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    ASSERT_TRUE(badAddr != MAP_FAILED);
    munmap(badAddr, 4096);

    int wd = inotify_add_watch(fd, badAddr, IN_ACCESS);
    EXPECT_EQ(wd, -1);
    EXPECT_EQ(errno, EFAULT);

    close(fd);
}

/* TestReportAnalysis P1-11: 未测试 inotify_rm_watch 已移除的 wd (P1) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_2900
 * @tc.name   : InotifyRmWatchRemovedWdFailed_0029
 * @tc.desc   : inotify_rm_watch on already removed wd returns EINVAL.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRmWatchRemovedWdFailed_0029, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    errno = 0;
    int ret2 = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret2, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
}

/* TestReportAnalysis P1-12: 未测试 inotify_rm_watch 属于其他 inotify 实例的 wd (P1) */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3000
 * @tc.name   : InotifyRmWatchOtherInstanceWdFailed_0030
 * @tc.desc   : inotify_rm_watch with wd from another instance.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRmWatchOtherInstanceWdFailed_0030, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);

    int fd1 = inotify_init1(0);
    EXPECT_TRUE(fd1 >= 0);
    int fd2 = inotify_init1(0);
    EXPECT_TRUE(fd2 >= 0);

    int wd = inotify_add_watch(fd1, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd >= 0);

    errno = 0;
    int ret = inotify_rm_watch(fd2, wd);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    inotify_rm_watch(fd1, wd);
    close(fd1);
    close(fd2);
}

/* TestReportAnalysis P2-03: 修改现有监视mask (P2) 建议: 验证再次add_watch同一文件可修改mask */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3100
 * @tc.name   : InotifyAddWatchModifyMask_0031
 * @tc.desc   : inotify_add_watch modify mask behavior.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddWatchModifyMask_0031, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd1 = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd1 >= 0);

    errno = 0;
    int wd2 = inotify_add_watch(fd, TEST_FILE, IN_MODIFY);
    EXPECT_EQ(wd2, wd1);

    inotify_rm_watch(fd, wd2);
    close(fd);
}

/* TestReportAnalysis P2-04: IN_MASK_ADD追加事件验证 (P2) 建议: 验证IN_MASK_ADD真正追加事件到已有mask */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3200
 * @tc.name   : InotifyMaskAddAppendEvent_0032
 * @tc.desc   : inotify IN_MASK_ADD appends events to existing watch.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyMaskAddAppendEvent_0032, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(IN_NONBLOCK);
    EXPECT_TRUE(fd >= 0);

    int wd1 = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd1 >= 0);

    errno = 0;
    int wd2 = inotify_add_watch(fd, TEST_FILE, IN_MODIFY | IN_MASK_ADD);
    EXPECT_EQ(wd2, wd1);

    fdTest = open(TEST_FILE, O_WRONLY);
    EXPECT_TRUE(fdTest > 0);
    write(fdTest, "x", 1);
    close(fdTest);

    char buf[INOTIFY_BUF_SIZE];
    errno = 0;
    ssize_t len = read(fd, buf, sizeof(buf));
    EXPECT_GT(len, 0);
    if (len > 0) {
        struct inotify_event* event = reinterpret_cast<struct inotify_event*>(buf);
        EXPECT_EQ(event->wd, wd1);
    }

    inotify_rm_watch(fd, wd1);
    close(fd);
}

/* TestReportAnalysis P2-05: IN_DONT_FOLLOW对符号链接的行为 (P2) 建议: 创建符号链接验证IN_DONT_FOLLOW */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3300
 * @tc.name   : InotifyDontFollowSymlink_0033
 * @tc.desc   : inotify IN_DONT_FOLLOW does not follow symlink.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyDontFollowSymlink_0033, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    const char* symlinkPath = "/data/local/tmp/notify_symlink";
    unlink(symlinkPath);
    symlink(TEST_FILE, symlinkPath);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, symlinkPath, IN_ACCESS | IN_DONT_FOLLOW);
    EXPECT_TRUE(wd >= 0);

    if (wd >= 0) {
        inotify_rm_watch(fd, wd);
    }
    close(fd);
    unlink(symlinkPath);
}

/* TestReportAnalysis P2-06: IN_EXCL_UNLINK排除子文件事件 (P2) 建议: 监视目录时验证IN_EXCL_UNLINK */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3400
 * @tc.name   : InotifyExclUnlinkSubFile_0034
 * @tc.desc   : inotify IN_EXCL_UNLINK excludes unlinked subfile events.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyExclUnlinkSubFile_0034, Function | MediumTest | Level2)
{
    mkdir(TEST_DIR, 0777);
    int fd = inotify_init1(IN_NONBLOCK);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, TEST_DIR, IN_CREATE | IN_DELETE | IN_EXCL_UNLINK);
    EXPECT_TRUE(wd >= 0);

    if (wd >= 0) {
        inotify_rm_watch(fd, wd);
    }
    close(fd);
}

/* TestReportAnalysis P2-07+P1-T04: IN_ONESHOT单次触发后自动移除 (P2) 建议: 验证IN_ONESHOT触发后wd自动失效 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3500
 * @tc.name   : InotifyOneshotAutoRemove_0035
 * @tc.desc   : inotify IN_ONESHOT auto-removes after one event.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyOneshotAutoRemove_0035, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS | IN_ONESHOT);
    EXPECT_TRUE(wd >= 0);

    fdTest = open(TEST_FILE, O_RDONLY);
    EXPECT_TRUE(fdTest > 0);
    char c;
    read(fdTest, &c, 1);
    close(fdTest);

    char buf[INOTIFY_BUF_SIZE];
    ssize_t len = read(fd, buf, sizeof(buf));
    EXPECT_GT(len, 0);

    errno = 0;
    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);

    close(fd);
}

/* TestReportAnalysis P2-09+P1-T02+P1-S06+P1-S11+P1-S12: read()读取inotify事件验证 (P2) 建议: 完整读取并验证事件 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3600
 * @tc.name   : InotifyReadEventVerify_0036
 * @tc.desc   : inotify read event data verification.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyReadEventVerify_0036, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_MODIFY);
    EXPECT_TRUE(wd >= 0);

    fdTest = open(TEST_FILE, O_WRONLY);
    EXPECT_TRUE(fdTest > 0);
    errno = 0;
    ssize_t wret = write(fdTest, "hello", 5);
    EXPECT_EQ(wret, 5);
    close(fdTest);

    char buf[INOTIFY_BUF_SIZE];
    errno = 0;
    ssize_t len = read(fd, buf, sizeof(buf));
    EXPECT_GT(len, 0);
    if (len > 0) {
        struct inotify_event* event = reinterpret_cast<struct inotify_event*>(buf);
        EXPECT_EQ(event->wd, wd);
        EXPECT_TRUE(event->mask & IN_MODIFY);
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

/* TestReportAnalysis P2-10: inotify_rm_watch后产生IN_IGNORED事件 (P2) 建议: 验证rm_watch产生IN_IGNORED */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3700
 * @tc.name   : InotifyRmWatchIgnoredEvent_0037
 * @tc.desc   : inotify_rm_watch generates IN_IGNORED event.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRmWatchIgnoredEvent_0037, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd >= 0);

    int ret = inotify_rm_watch(fd, wd);
    EXPECT_EQ(ret, 0);

    char buf[INOTIFY_BUF_SIZE];
    errno = 0;
    ssize_t len = read(fd, buf, sizeof(buf));
    EXPECT_GT(len, 0);
    if (len > 0) {
        struct inotify_event* event = reinterpret_cast<struct inotify_event*>(buf);
        EXPECT_TRUE(event->mask & IN_IGNORED);
    }

    close(fd);
}

/* TestReportAnalysis P2-11: IN_ISDIR输出标志验证 (P2) 建议: 监视目录触发事件验证IN_ISDIR标志 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3800
 * @tc.name   : InotifyIsdirOutputFlag_0038
 * @tc.desc   : inotify IN_ISDIR flag in event output.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyIsdirOutputFlag_0038, Function | MediumTest | Level2)
{
    mkdir(TEST_DIR, 0777);
    const char* subFile = "/data/local/tmp/notify/sub.txt";

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_DIR, IN_CREATE);
    EXPECT_TRUE(wd >= 0);

    int fdSub = open(subFile, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdSub > 0);
    close(fdSub);

    char buf[INOTIFY_BUF_SIZE];
    ssize_t len = read(fd, buf, sizeof(buf));
    EXPECT_GT(len, 0);
    if (len > 0) {
        struct inotify_event* event = reinterpret_cast<struct inotify_event*>(buf);
        EXPECT_TRUE(event->mask & IN_CREATE);
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    unlink(subFile);
}

/* TestReportAnalysis P2-12+P1-S05+P1-S10: 同一文件多次add_watch返回相同wd (P2) 建议: 验证wd复用和幂等性 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_3900
 * @tc.name   : InotifySameFileSameWd_0039
 * @tc.desc   : inotify same file returns same wd.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifySameFileSameWd_0039, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd1 = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd1 >= 0);

    errno = 0;
    int wd2 = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_EQ(wd2, wd1);

    errno = 0;
    int wd3 = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_EQ(wd3, wd1);

    inotify_rm_watch(fd, wd1);
    close(fd);
}

/* TestReportAnalysis P1-M06+P1-S08: 监视后删除重建文件 (P1) 建议: 验证删除重建文件后监视行为 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4000
 * @tc.name   : InotifyWatchFileDeleteRecreate_0040
 * @tc.desc   : inotify watch file delete and recreate.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyWatchFileDeleteRecreate_0040, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(IN_NONBLOCK);
    EXPECT_TRUE(fd >= 0);

    int wd1 = inotify_add_watch(fd, TEST_FILE, IN_DELETE_SELF | IN_MODIFY);
    EXPECT_TRUE(wd1 >= 0);

    unlink(TEST_FILE);

    char buf[INOTIFY_BUF_SIZE];
    ssize_t len = read(fd, buf, sizeof(buf));
    EXPECT_EQ(len, -1);

    fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    errno = 0;
    int wd2 = inotify_add_watch(fd, TEST_FILE, IN_MODIFY);
    EXPECT_TRUE(wd2 >= 0);

    if (wd2 >= 0) {
        inotify_rm_watch(fd, wd2);
    }
    close(fd);
}

/* TestReportAnalysis P1-M07: 监视目录后删除重建目录 (P1) 建议: 验证目录删除重建后监视行为 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4100
 * @tc.name   : InotifyWatchDirDeleteRecreate_0041
 * @tc.desc   : inotify watch directory delete and recreate.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyWatchDirDeleteRecreate_0041, Function | MediumTest | Level2)
{
    mkdir(TEST_DIR, 0777);

    int fd = inotify_init1(IN_NONBLOCK);
    EXPECT_TRUE(fd >= 0);

    int wd1 = inotify_add_watch(fd, TEST_DIR, IN_CREATE | IN_DELETE);
    EXPECT_TRUE(wd1 >= 0);

    rmdir(TEST_DIR);

    char buf[INOTIFY_BUF_SIZE];
    ssize_t len = 0;
    for (int i = 0; i < EVENT_POLL_RETRIES && len <= 0; i++) {
        usleep(EVENT_POLL_INTERVAL_US);
        len = read(fd, buf, sizeof(buf));
        if (len) {
            break;
        }
    }
    if (len <= 0) {
        return;
    }

    mkdir(TEST_DIR, 0777);

    errno = 0;
    int wd2 = inotify_add_watch(fd, TEST_DIR, IN_CREATE);
    EXPECT_TRUE(wd2 >= 0);

    if (wd2 >= 0) {
        inotify_rm_watch(fd, wd2);
    }
    close(fd);
}

/* TestReportAnalysis P1-M09: inotify_rm_watch使用fd=-1返回EBADF (P1) 建议: 验证无效fd值 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4200
 * @tc.name   : InotifyRmWatchFdNegOneFailed_0042
 * @tc.desc   : inotify_rm_watch with fd=-1 returns EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRmWatchFdNegOneFailed_0042, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = inotify_rm_watch(-1, 1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/* TestReportAnalysis P1-S01: 多次init1互不影响 (P1) 建议: 验证多个inotify实例独立工作 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4300
 * @tc.name   : InotifyMultiInit1Independent_0043
 * @tc.desc   : inotify multiple init1 instances are independent.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyMultiInit1Independent_0043, Function | MediumTest | Level2)
{
    errno = 0;
    int fd1 = inotify_init1(0);
    EXPECT_TRUE(fd1 >= 0);

    errno = 0;
    int fd2 = inotify_init1(0);
    EXPECT_TRUE(fd2 >= 0);
    EXPECT_NE(fd1, fd2);

    close(fd1);
    errno = 0;
    int fd3 = inotify_init1(0);
    EXPECT_TRUE(fd3 >= 0);

    close(fd2);
    close(fd3);
}

/* TestReportAnalysis P1-S02: 快速连续创建多个inotify实例 (P1) 建议: 验证快速创建不冲突 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4400
 * @tc.name   : InotifyRapidCreateInstances_0044
 * @tc.desc   : inotify rapid create instances stability.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRapidCreateInstances_0044, Function | MediumTest | Level2)
{
    int fds[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++) {
        errno = 0;
        fds[i] = inotify_init1(0);
        EXPECT_TRUE(fds[i] >= 0);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        for (int j = i + 1; j < THREAD_COUNT; j++) {
            EXPECT_NE(fds[i], fds[j]);
        }
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        close(fds[i]);
    }
}

/* TestReportAnalysis P1-S03: 同一fd重复add_watch不同路径 (P1) 建议: 验证同一fd可监视多个路径 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4500
 * @tc.name   : InotifySameFdMultiPath_0045
 * @tc.desc   : inotify same fd watches multiple paths.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifySameFdMultiPath_0045, Function | MediumTest | Level2)
{
    const char* fileA = "/data/local/tmp/notify_a.txt";
    const char* fileB = "/data/local/tmp/notify_b.txt";
    int fdA = open(fileA, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdA > 0);
    close(fdA);
    int fdB = open(fileB, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdB > 0);
    close(fdB);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wdA = inotify_add_watch(fd, fileA, IN_ACCESS);
    EXPECT_TRUE(wdA >= 0);

    errno = 0;
    int wdB = inotify_add_watch(fd, fileB, IN_MODIFY);
    EXPECT_TRUE(wdB >= 0);
    EXPECT_NE(wdA, wdB);

    inotify_rm_watch(fd, wdA);
    inotify_rm_watch(fd, wdB);
    close(fd);
    unlink(fileA);
    unlink(fileB);
}

/* TestReportAnalysis P1-S04: add_watch与rm_watch时序关系 (P1) 建议: 验证add后rm后可重新add */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4600
 * @tc.name   : InotifyAddRmReaddSequence_0046
 * @tc.desc   : inotify add, remove, re-add watch sequence.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyAddRmReaddSequence_0046, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd1 = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd1 >= 0);

    int ret = inotify_rm_watch(fd, wd1);
    EXPECT_EQ(ret, 0);

    errno = 0;
    int wd2 = inotify_add_watch(fd, TEST_FILE, IN_MODIFY);
    EXPECT_TRUE(wd2 >= 0);

    inotify_rm_watch(fd, wd2);
    close(fd);
}

/* TestReportAnalysis P1-S07: 对符号链接等特殊文件的add_watch (P1) 建议: 验证符号链接监视 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4700
 * @tc.name   : InotifyWatchSymlink_0047
 * @tc.desc   : inotify watch symlink behavior.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyWatchSymlink_0047, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    const char* symlinkPath = "/data/local/tmp/notify_sym_test";
    unlink(symlinkPath);
    int sret = symlink(TEST_FILE, symlinkPath);
    EXPECT_EQ(sret, 0);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    errno = 0;
    int wd = inotify_add_watch(fd, symlinkPath, IN_ACCESS);
    EXPECT_TRUE(wd >= 0);

    if (wd >= 0) {
        inotify_rm_watch(fd, wd);
    }
    close(fd);
    unlink(symlinkPath);
}

/* TestReportAnalysis P1-S13: rm_watch后再add_watch再rm_watch循环 (P1) 建议: 验证循环操作稳定性 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4800
 * @tc.name   : InotifyRmAddRmCycle_0048
 * @tc.desc   : inotify rm/add cycle test.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRmAddRmCycle_0048, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    for (int i = 0; i < RM_ADD_CYCLE_COUNT; i++) {
        errno = 0;
        int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
        EXPECT_TRUE(wd >= 0);

        int ret = inotify_rm_watch(fd, wd);
        EXPECT_EQ(ret, 0);
    }

    close(fd);
}

/* TestReportAnalysis P1-S14: 快速连续rm_watch多个wd (P1) 建议: 验证批量移除稳定性 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_4900
 * @tc.name   : InotifyRapidRmMultiWd_0049
 * @tc.desc   : inotify rapid remove multiple watch descriptors.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyRapidRmMultiWd_0049, Function | MediumTest | Level2)
{
    const char* base = "/data/local/tmp/notify_rapid_";
    std::vector<int> wds;
    std::vector<std::string> paths;

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    for (int i = 0; i < 5; i++) {
        std::string path = std::string(base) + std::to_string(i) + ".txt";
        int fdTmp = open(path.c_str(), O_RDWR | O_CREAT, 0666);
        EXPECT_TRUE(fdTmp > 0);
        close(fdTmp);
        paths.push_back(path);

        int wd = inotify_add_watch(fd, path.c_str(), IN_ACCESS);
        EXPECT_TRUE(wd >= 0);
        wds.push_back(wd);
    }

    for (size_t i = 0; i < wds.size(); i++) {
        errno = 0;
        int ret = inotify_rm_watch(fd, wds[i]);
        EXPECT_EQ(ret, 0);
    }

    close(fd);
    for (size_t i = 0; i < paths.size(); i++) {
        unlink(paths[i].c_str());
    }
}

/* TestReportAnalysis P1-S16: add_watch后立即rm_watch (P1) 建议: 验证快速操作不异常 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5000
 * @tc.name   : InotifyImmediateRmAfterAdd_0050
 * @tc.desc   : inotify immediate rm_watch after add_watch.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyImmediateRmAfterAdd_0050, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    for (int i = 0; i < 5; i++) {
        errno = 0;
        int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
        EXPECT_TRUE(wd >= 0);

        errno = 0;
        int ret = inotify_rm_watch(fd, wd);
        EXPECT_EQ(ret, 0);
    }

    close(fd);
}

/* TestReportAnalysis P1-T03: 被监视文件删除触发IN_DELETE_SELF事件 (P1) 建议: 验证删除触发事件 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5100
 * @tc.name   : InotifyDeleteSelfEvent_0051
 * @tc.desc   : inotify IN_DELETE_SELF event on watched file.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyDeleteSelfEvent_0051, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_DELETE_SELF);
    EXPECT_TRUE(wd >= 0);

    unlink(TEST_FILE);

    char buf[INOTIFY_BUF_SIZE];
    errno = 0;
    ssize_t len = read(fd, buf, sizeof(buf));
    EXPECT_GT(len, 0);
    if (len > 0) {
        struct inotify_event* event = reinterpret_cast<struct inotify_event*>(buf);
        EXPECT_TRUE(event->mask & IN_DELETE_SELF);
    }

    close(fd);
}

/* TestReportAnalysis P1-T06: 添加多个wd的状态转换 (P1) 建议: 验证多wd独立工作 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5200
 * @tc.name   : InotifyMultiWdState_0052
 * @tc.desc   : inotify multiple watch descriptor state.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyMultiWdState_0052, Function | MediumTest | Level2)
{
    mkdir(TEST_DIR, 0777);
    const char* fileA = "/data/local/tmp/notify/mwa.txt";
    const char* fileB = "/data/local/tmp/notify/mwb.txt";

    int fdA = open(fileA, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdA > 0);
    close(fdA);
    int fdB = open(fileB, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdB > 0);
    close(fdB);

    int fd = inotify_init1(IN_NONBLOCK);
    EXPECT_TRUE(fd >= 0);

    int wdDir = inotify_add_watch(fd, TEST_DIR, IN_CREATE);
    EXPECT_TRUE(wdDir >= 0);
    int wdFile = inotify_add_watch(fd, fileA, IN_MODIFY);
    EXPECT_TRUE(wdFile >= 0);

    EXPECT_NE(wdDir, wdFile);

    inotify_rm_watch(fd, wdDir);
    errno = 0;
    int ret = inotify_rm_watch(fd, wdFile);
    EXPECT_EQ(ret, 0);

    close(fd);
    unlink(fileA);
    unlink(fileB);
}

/* TestReportAnalysis P1-C01: 多线程同时调用inotify_init1 (P1) 建议: 验证多线程并发init安全 */
static void* ThreadInit1Func(void* arg)
{
    int* result = static_cast<int*>(arg);
    errno = 0;
    *result = inotify_init1(0);
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5300
 * @tc.name   : InotifyMultiThreadInit1_0053
 * @tc.desc   : inotify init1 in multi-thread environment.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyMultiThreadInit1_0053, Function | MediumTest | Level2)
{
    const int THREAD_COUNT_0053 = 4;
    pthread_t threads[THREAD_COUNT_0053];
    int results[THREAD_COUNT_0053];

    for (int i = 0; i < THREAD_COUNT_0053; i++) {
        int ret = pthread_create(&threads[i], nullptr, ThreadInit1Func, &results[i]);
        EXPECT_EQ(ret, 0);
    }
    for (int i = 0; i < THREAD_COUNT_0053; i++) {
        pthread_join(threads[i], nullptr);
    }
    for (int i = 0; i < THREAD_COUNT_0053; i++) {
        EXPECT_TRUE(results[i] >= 0);
        if (results[i] >= 0) {
            close(results[i]);
        }
    }
}

/* TestReportAnalysis P1-C02: 多线程同时add_watch同一路径 (P1) 建议: 验证并发add_watch安全 */
struct ThreadAddWatchArgs {
    int fd;
    const char* path;
    uint32_t mask;
    int wd;
};

static void* ThreadAddWatchFunc(void* arg)
{
    struct ThreadAddWatchArgs* a = static_cast<struct ThreadAddWatchArgs*>(arg);
    errno = 0;
    a->wd = inotify_add_watch(a->fd, a->path, a->mask);
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5400
 * @tc.name   : InotifyMultiThreadAddWatchSamePath_0054
 * @tc.desc   : inotify multi-thread add watch same path.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyMultiThreadAddWatchSamePath_0054, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    const int THREAD_COUNT_0054 = 4;
    pthread_t threads[THREAD_COUNT_0054];
    struct ThreadAddWatchArgs args[THREAD_COUNT_0054];
    for (int i = 0; i < THREAD_COUNT_0054; i++) {
        args[i].fd = fd;
        args[i].path = TEST_FILE;
        args[i].mask = IN_ACCESS;
        args[i].wd = -1;
        int ret = pthread_create(&threads[i], nullptr, ThreadAddWatchFunc, &args[i]);
        EXPECT_EQ(ret, 0);
    }
    for (int i = 0; i < THREAD_COUNT_0054; i++) {
        pthread_join(threads[i], nullptr);
    }
    for (int i = 0; i < THREAD_COUNT_0054; i++) {
        EXPECT_TRUE(args[i].wd >= 0);
    }

    if (args[0].wd >= 0) {
        inotify_rm_watch(fd, args[0].wd);
    }
    close(fd);
}

/* TestReportAnalysis P1-C03: 多线程同时add_watch不同路径 (P1) 建议: 验证并发add_watch不同文件 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5500
 * @tc.name   : InotifyMultiThreadAddWatchDiffPath_0055
 * @tc.desc   : inotify multi-thread add watch different paths.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyMultiThreadAddWatchDiffPath_0055, Function | MediumTest | Level2)
{
    const char* paths[] = {
        "/data/local/tmp/notify_c3_0.txt",
        "/data/local/tmp/notify_c3_1.txt",
        "/data/local/tmp/notify_c3_2.txt",
        "/data/local/tmp/notify_c3_3.txt"
    };
    const int THREAD_COUNT_0055 = 4;
    for (int i = 0; i < THREAD_COUNT_0055; i++) {
        int fdTmp = open(paths[i], O_RDWR | O_CREAT, 0666);
        EXPECT_TRUE(fdTmp > 0);
        close(fdTmp);
    }

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    pthread_t threads[THREAD_COUNT_0055];
    struct ThreadAddWatchArgs args[THREAD_COUNT_0055];
    for (int i = 0; i < THREAD_COUNT_0055; i++) {
        args[i].fd = fd;
        args[i].path = paths[i];
        args[i].mask = IN_ACCESS;
        args[i].wd = -1;
        int ret = pthread_create(&threads[i], nullptr, ThreadAddWatchFunc, &args[i]);
        EXPECT_EQ(ret, 0);
    }
    for (int i = 0; i < THREAD_COUNT_0055; i++) {
        pthread_join(threads[i], nullptr);
    }
    for (int i = 0; i < THREAD_COUNT_0055; i++) {
        EXPECT_TRUE(args[i].wd >= 0);
        if (args[i].wd >= 0) {
            inotify_rm_watch(fd, args[i].wd);
        }
    }
    close(fd);
    for (int i = 0; i < THREAD_COUNT_0055; i++) {
        unlink(paths[i]);
    }
}

/* TestReportAnalysis P1-C04: 多线程同时rm_watch (P1) 建议: 验证并发rm_watch安全 */
struct ThreadRmWatchArgs {
    int fd;
    int wd;
    int ret;
};

static void* ThreadRmWatchFunc(void* arg)
{
    struct ThreadRmWatchArgs* a = static_cast<struct ThreadRmWatchArgs*>(arg);
    errno = 0;
    a->ret = inotify_rm_watch(a->fd, a->wd);
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5600
 * @tc.name   : InotifyMultiThreadRmWatch_0056
 * @tc.desc   : inotify multi-thread rm_watch.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyMultiThreadRmWatch_0056, Function | MediumTest | Level2)
{
    const char* paths[] = {
        "/data/local/tmp/notify_c4_0.txt",
        "/data/local/tmp/notify_c4_1.txt",
        "/data/local/tmp/notify_c4_2.txt",
        "/data/local/tmp/notify_c4_3.txt"
    };
    const int THREAD_COUNT_0056 = 4;
    int wds[THREAD_COUNT_0056];

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    for (int i = 0; i < THREAD_COUNT_0056; i++) {
        int fdTmp = open(paths[i], O_RDWR | O_CREAT, 0666);
        EXPECT_TRUE(fdTmp > 0);
        close(fdTmp);
        wds[i] = inotify_add_watch(fd, paths[i], IN_ACCESS);
        EXPECT_TRUE(wds[i] >= 0);
    }

    pthread_t threads[THREAD_COUNT_0056];
    struct ThreadRmWatchArgs args[THREAD_COUNT_0056];
    for (int i = 0; i < THREAD_COUNT_0056; i++) {
        args[i].fd = fd;
        args[i].wd = wds[i];
        args[i].ret = 0;
        int ret = pthread_create(&threads[i], nullptr, ThreadRmWatchFunc, &args[i]);
        EXPECT_EQ(ret, 0);
    }
    for (int i = 0; i < THREAD_COUNT_0056; i++) {
        pthread_join(threads[i], nullptr);
        EXPECT_EQ(args[i].ret, 0);
    }
    close(fd);
    for (int i = 0; i < THREAD_COUNT_0056; i++) {
        unlink(paths[i]);
    }
}

/* TestReportAnalysis P1-C05: 一线程add_watch另一线程rm_watch (P1) 建议: 验证交叉并发安全 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5700
 * @tc.name   : InotifyCrossThreadAddRm_0057
 * @tc.desc   : inotify cross-thread add and remove watch.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyCrossThreadAddRm_0057, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd >= 0);

    struct ThreadRmWatchArgs rmArgs;
    rmArgs.fd = fd;
    rmArgs.wd = wd;
    rmArgs.ret = 0;

    pthread_t rmThread;
    int pret = pthread_create(&rmThread, nullptr, ThreadRmWatchFunc, &rmArgs);
    EXPECT_EQ(pret, 0);
    pthread_join(rmThread, nullptr);
    EXPECT_EQ(rmArgs.ret, 0);

    errno = 0;
    int wd2 = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd2 >= 0);

    if (wd2 >= 0) {
        inotify_rm_watch(fd, wd2);
    }
    close(fd);
}

/* TestReportAnalysis P1-C06+P1-C10: fork后子进程继承inotify fd (P1) 建议: 验证fork继承和子进程操作 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5800
 * @tc.name   : InotifyForkInheritFd_0058
 * @tc.desc   : inotify fd inherited across fork.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyForkInheritFd_0058, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd >= 0);

    pid_t pid = fork();
    EXPECT_TRUE(pid >= 0);
    if (pid == 0) {
        errno = 0;
        int ret = inotify_rm_watch(fd, wd);
        close(fd);
        _exit(ret == 0 ? 0 : 1);
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        close(fd);
    }
}

/* TestReportAnalysis P1-C07: 多进程同时操作同一inotify实例 (P1) 建议: 验证多进程并发安全 */
/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_5900
 * @tc.name   : InotifyMultiProcessSameFd_0059
 * @tc.desc   : inotify multi-process same fd access.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyMultiProcessSameFd_0059, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    pid_t pid = fork();
    EXPECT_TRUE(pid >= 0);
    if (pid == 0) {
        errno = 0;
        int wd = inotify_add_watch(fd, TEST_FILE, IN_MODIFY);
        if (wd >= 0) {
            inotify_rm_watch(fd, wd);
        }
        close(fd);
        _exit(wd >= 0 ? 0 : 1);
    } else {
        errno = 0;
        int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
        EXPECT_TRUE(wd >= 0);
        if (wd >= 0) {
            inotify_rm_watch(fd, wd);
        }
        int status = 0;
        waitpid(pid, &status, 0);
        close(fd);
    }
}

/* TestReportAnalysis P1-C08: add_watch与文件删除并发 (P1) 建议: 验证并发不崩溃 */
struct ThreadDelFileArgs {
    const char* path;
    int ret;
};

static void* ThreadDelFileFunc(void* arg)
{
    struct ThreadDelFileArgs* a = static_cast<struct ThreadDelFileArgs*>(arg);
    a->ret = unlink(a->path);
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_6000
 * @tc.name   : InotifyConcurrentAddWatchAndDelete_0060
 * @tc.desc   : inotify concurrent add_watch and file delete.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyConcurrentAddWatchAndDelete_0060, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    struct ThreadDelFileArgs delArgs;
    delArgs.path = TEST_FILE;
    delArgs.ret = 0;

    pthread_t delThread;
    int pret = pthread_create(&delThread, nullptr, ThreadDelFileFunc, &delArgs);
    EXPECT_EQ(pret, 0);

    errno = 0;
    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);

    pthread_join(delThread, nullptr);

    if (wd >= 0) {
        inotify_rm_watch(fd, wd);
    }
    close(fd);
}

/* TestReportAnalysis P1-C09: rm_watch与事件读取并发 (P1) 建议: 验证并发不崩溃 */
struct ThreadReadEventArgs {
    int fd;
    ssize_t len;
};

static void* ThreadReadEventFunc(void* arg)
{
    struct ThreadReadEventArgs* a = static_cast<struct ThreadReadEventArgs*>(arg);
    char buf[INOTIFY_BUF_SIZE];
    errno = 0;
    a->len = read(a->fd, buf, sizeof(buf));
    return nullptr;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_INOTIFY_6100
 * @tc.name   : InotifyConcurrentRmWatchAndRead_0061
 * @tc.desc   : inotify concurrent rm_watch and read.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsInotifyTest, InotifyConcurrentRmWatchAndRead_0061, Function | MediumTest | Level2)
{
    int fdTest = open(TEST_FILE, O_RDWR | O_CREAT, 0666);
    EXPECT_TRUE(fdTest > 0);
    close(fdTest);

    int fd = inotify_init1(0);
    EXPECT_TRUE(fd >= 0);

    int wd = inotify_add_watch(fd, TEST_FILE, IN_ACCESS);
    EXPECT_TRUE(wd >= 0);

    struct ThreadReadEventArgs readArgs;
    readArgs.fd = fd;
    readArgs.len = 0;

    pthread_t readThread;
    int pret = pthread_create(&readThread, nullptr, ThreadReadEventFunc, &readArgs);
    EXPECT_EQ(pret, 0);

    errno = 0;
    inotify_rm_watch(fd, wd);

    pthread_join(readThread, nullptr);

    close(fd);
}