/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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
#include <gtest/gtest.h>
#include <cinttypes>
#include <climits>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <sys/time.h>

#define SIZE_ALIGN (4 * sizeof(size_t))
#define THRESHOLD (0x1c00 * SIZE_ALIGN)
#define ITER_TIME 20
const int MALLOC_SIZE_ONE = 1024;
const int MALLOC_SIZE_TWO = 2048;

static void handler(int s)
{
}

using namespace testing::ext;
using namespace std;

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

volatile void *g_ptr;

int set_devide_chunk(int size)
{
    if (!(g_ptr = malloc(size))) {
        printf("Malloc failed: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

static int child(void)
{
    int *c;
	int *d;

    /* Set first dividing chunk */
    if (set_devide_chunk(MALLOC_SIZE_TWO)) {
        return -1;
    }

    /*
     * The init procedure makes the freelist unpredictable. To make sure trigger the safe-unlink
     * check, Here we create as many chunks as possible to make sure there are enough chunks in
     * bin[0] and malloc again. Basically this is heap spray.
     */
    for (int i = 0; i < 512; ++i) {

        c = (int *)malloc(MALLOC_SIZE_ONE);
        if (!c) {
            printf("Malloc failed: %s\n", strerror(errno));
            return -1;
        }
        if (set_devide_chunk(MALLOC_SIZE_TWO)) {
            return -1;
        }
        d = (int *)malloc(MALLOC_SIZE_ONE);
        if (!d) {
            printf("Malloc failed: %s\n", strerror(errno));
            return -1;
        }
        if (set_devide_chunk(MALLOC_SIZE_TWO)) {
            return -1;
        }
        free(d);
        free(c);
        /* exchange the prev and next pointer */
        uintptr_t temp = c[0];
        c[0] = c[1];
        c[1] = temp;
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
 * @tc.number CAM_FREE_FUN_0100
 * @tc.name Apply part of the memory camfree reduction
 * @tc.desc Test the basic features of CamFree
*/
HWTEST_F(MallocSafeUnlink, safeUnlinkTest0100, Function | MediumTest | Level1)
{
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
        printf("fork failed: %s\n", strerror(errno));
    }
    if (sigtimedwait(&set, 0, &time1) == -1) { /* Wait for 5 seconds */
        if (errno == EAGAIN) {
            flag = 1;
        } else {
            printf("sigtimedwait failed: %s\n", strerror(errno));
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