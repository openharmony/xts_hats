/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
#include <cerrno>
#include <cstring>
#include <gtest/gtest.h>

#define UNIT            16
#define OFF_OFFSET      2
#define FIRST_OFFSET    -4
#define FIRST_OFF_OFFSET 8
#define MALLOC_SIZE_S   40
#define MALLOC_SIZE_L   500
#define TEST_NUM        512

using namespace testing::ext;

class MallocSafeUnlink : public testing::Test {
public:

    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};

void MallocSafeUnlink::SetUp()
{
}
void MallocSafeUnlink::TearDown()
{
}
void MallocSafeUnlink::SetUpTestCase()
{
}
void MallocSafeUnlink::TearDownTestCase()
{
}

struct meta_in {
    struct meta_in *prev, *next;
    uintptr_t *mem;
};

struct group_in {
    struct meta_in *meta;
};

static struct group_in *get_group(volatile const uint8_t *p)
{
    int offset = *(const uint16_t *)((uintptr_t)p - OFF_OFFSET);

    if (p[FIRST_OFFSET]) {
        offset = *(uintptr_t *)((uintptr_t)p - FIRST_OFF_OFFSET);
    }

    struct group_in *base = (struct group_in *)((uintptr_t)p - UNIT*offset - UNIT);
    return base;
}

static void handler(int s)
{
}

volatile void *tmp;

int set_devide_chunk(size_t size)
{
    if (!(tmp = malloc(size))) {
        printf("Malloc failed: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}
extern int g_test_d;

static int child(void)
{
    volatile uint8_t *c1;
    volatile uint8_t *c2;
    uint8_t *c1_tmp;
    uint8_t *c2_tmp;

    struct group_in *g1 = nullptr;
    struct group_in *g2 = nullptr;

    for (int i = 0; i < TEST_NUM; ++i) {
        c1_tmp = (uint8_t *)malloc(MALLOC_SIZE_S + 0x10 * i);
        c2_tmp = (uint8_t *)malloc(MALLOC_SIZE_L + 0x10 * i);

        c1 = (uint8_t *)malloc(MALLOC_SIZE_S + 0x10 * i);
        if ((!c1) || ((uintptr_t)c1 & 15))  {
            printf("Malloc failed: %s\n", strerror(errno));
            return -1;
        }
        g1 = get_group(c1);

        c2 = (uint8_t *)malloc(MALLOC_SIZE_L + 0x10 * i);
        if ((!c2) || ((uintptr_t)c2 & 15))  {
            printf("Malloc failed: %s\n", strerror(errno));
            return -1;
        }

        g2 = get_group(c2);
        g2->meta = g1->meta;

        free((void *)c1);
        free((void *)c2);

    }

    return 0;
}

static pid_t start_child(void)
{
    pid_t pid;
    int ret;
    pid = fork();
    if (pid == 0) {
        ret = child();
        printf("child process normally out with %d\n", ret);
        return ret;
    }
    return pid;
}

/*
 * @tc.number SAFE_UNLINK_TEST_0100
 * @tc.name Apply part of the memory camfree reduction
 * @tc.desc Test the safeunlink
*/
HWTEST_F(MallocSafeUnlink, safeunlinkTest0100, Function | MediumTest | Level1) {
    sigset_t set;
    int status;
    pid_t pid;
    int flag = 0;
    struct timespec time1 = {5, 0};

    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, 0);
    signal(SIGCHLD, handler);

    pid = start_child();
    if (pid == -1) {
        printf("fork failed: %s\n",strerror(errno));
    }
    if (sigtimedwait(&set, 0, &time1)  == -1) { /* Wait for 5 seconds */
        if (errno == EAGAIN) {
            flag = 1;
        }
        else {
            printf("sigtimedwait failed: %s\n",strerror(errno));
        }
        if (kill(pid, SIGKILL) == -1) {
            printf("kill failed: %s\n", strerror(errno));
        }
    }

    if (waitpid(pid, &status, 0) != pid) {
        printf("waitpid failed: %s\n", strerror(errno));
    }

    if (flag) {
        printf("Child process time out\n");
    }

    if (WIFSIGNALED(status)) {
        ASSERT_TRUE(WTERMSIG(status) == SIGSEGV || WTERMSIG(status) == SIGILL) << "child process out with %s\n" <<
        WTERMSIG(status);
    } else {
        ASSERT_TRUE(false) << "child process finished normally\n";
    }
}
