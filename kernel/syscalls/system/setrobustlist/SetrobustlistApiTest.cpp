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
#include <pthread.h>
#include <syscall.h>
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include "securec.h"

using namespace testing::ext;

struct RobustList {
    struct RobustList *next;
};

int SetRobustList(int tid, void *list, size_t len)
{
    return syscall(__NR_set_robust_list, tid, list, len);
}

class HatsSetrobustlistTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void HatsSetrobustlistTest::SetUp()
{
}

void HatsSetrobustlistTest::TearDown()
{
}

void HatsSetrobustlistTest::SetUpTestCase()
{
}

void HatsSetrobustlistTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_SETROBUSTLIST_0100
 * @tc.name   : SetrobustlistFailed_0001
 * @tc.desc   : Set robust_list failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsSetrobustlistTest, SetrobustlistFailed_0001, Function | MediumTest | Level2)
{
    pid_t tid = pthread_self();
    struct RobustList *setRobustList = (struct RobustList *)malloc(sizeof(struct RobustList));
    setRobustList->next = nullptr;

    int ret = SetRobustList(tid, setRobustList, sizeof(struct RobustList));
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}
