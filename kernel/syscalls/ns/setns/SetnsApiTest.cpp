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
#include <sched.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include "securec.h"

using namespace testing::ext;

static const int PATH_MAX_SIZE = 128;

class HatsSetnsTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSetnsTest::SetUp()
{
}

void HatsSetnsTest::TearDown()
{
}

void HatsSetnsTest::SetUpTestCase()
{
}

void HatsSetnsTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETNS_0100
 * @tc.name   : SetnsTest_0001
 * @tc.desc   : setns success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsSetnsTest, SetnsTest_0001, Function | MediumTest | Level1)
{
    char path[PATH_MAX_SIZE] = { 0 };
    pid_t pid = getpid();

    // test CLONE_NEWIPC success
    int ret = sprintf_s(path, PATH_MAX_SIZE - 1, "/proc/%d/ns/ipc", pid);
    EXPECT_TRUE(ret > 0);
    int fd = open(path, O_RDONLY);
    EXPECT_TRUE(fd > 0);

    ret = setns(fd, CLONE_NEWIPC);
    EXPECT_EQ(ret, 0);
    close(fd);

    // test CLONE_NEWNET success
    memset_s(path, PATH_MAX_SIZE, 0, sizeof(path));
    ret = sprintf_s(path, PATH_MAX_SIZE - 1, "/proc/%d/ns/net", pid);
    EXPECT_TRUE(ret > 0);
    fd = open(path, O_RDONLY);
    EXPECT_TRUE(fd > 0);

    ret = setns(fd, CLONE_NEWNET);
    EXPECT_EQ(ret, 0);
    close(fd);

    // test CLONE_NEWPID success
    memset_s(path, PATH_MAX_SIZE, 0, sizeof(path));
    ret = sprintf_s(path, PATH_MAX_SIZE - 1, "/proc/%d/ns/pid", pid);
    EXPECT_TRUE(ret > 0);
    fd = open(path, O_RDONLY);
    EXPECT_TRUE(fd > 0);

    ret = setns(fd, CLONE_NEWPID);
    EXPECT_EQ(ret, 0);
    close(fd);

    // test CLONE_NEWUTS success
    memset_s(path, PATH_MAX_SIZE, 0, sizeof(path));
    ret = sprintf_s(path, PATH_MAX_SIZE - 1, "/proc/%d/ns/uts", pid);
    EXPECT_TRUE(ret > 0);
    fd = open(path, O_RDONLY);
    EXPECT_TRUE(fd > 0);

    ret = setns(fd, CLONE_NEWUTS);
    EXPECT_EQ(ret, 0);

    close(fd);
}
