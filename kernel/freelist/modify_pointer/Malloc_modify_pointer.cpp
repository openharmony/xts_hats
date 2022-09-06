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
#include <sys/wait.h>
#include <sys/time.h>

static void handler(int s)
{
}

#define SIZE_ALIGN (4 * sizeof(size_t))
#define THRESHOLD (0x1c00 * SIZE_ALIGN)
#define ITER_TIME 20
 

using namespace testing::ext;
using namespace std;

class MallocModifyPointer : public testing::Test {
public:

    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};

void MallocModifyPointer::SetUp()
{
}
void MallocModifyPointer::TearDown()
{
}
void MallocModifyPointer::SetUpTestCase()
{
}
void MallocModifyPointer::TearDownTestCase()
{
}
const int MAX_NUMBER_VALUE = 512;
volatile uintptr_t *g_p0;
volatile uintptr_t *g_p1;
volatile void *g_tmp[MAX_NUMBER_VALUE];

static int child(void)
{
    g_p0 = (uintptr_t *)malloc(10 * sizeof(uintptr_t));
    if (!g_p0) {
        printf("Malloc failed:%s\n", strerror(errno));
    }
    /* Malloc a dividing chunk to avoid combination of neighbouring freed chunk */
    g_tmp[0] = (uintptr_t *)malloc(10 * sizeof(uintptr_t));
    /* Malloc another chunk to get a key */
    g_p1 = (uintptr_t *)malloc(10 * sizeof(uintptr_t));
    if (!g_p1) {
        printf("Malloc failed:%s\n", strerror(errno));
    }
    /* Malloc a dividing chunk to avoid combination of neighbouring freed chunk */
    g_tmp[0] = (uintptr_t *)malloc(10 * sizeof(uintptr_t));

    if (g_p0 != nullptr) {
        free(static_cast<void *>(const_cast<uintptr_t *>(g_p0)));
        g_p0 = nullptr;
    }

    if (g_p1 != nullptr) {
        free(static_cast<void *>(const_cast<uintptr_t *>(g_p1)));
        g_p1 = nullptr;
    }
    
    uintptr_t *fake_ptr = reinterpret_cast<uintptr_t *>(reinterpret_cast<uintptr_t>(reinterpret_cast<char *>
    (const_cast<uintptr_t *>(g_p1)) - sizeof(size_t) * 2) ^ reinterpret_cast<uintptr_t>(g_p0[0]));
    g_p0[0] = reinterpret_cast<uintptr_t>(fake_ptr);
    g_p1[0] = reinterpret_cast<uintptr_t>(fake_ptr);

    for (int i = 0; i < MAX_NUMBER_VALUE; ++i) {
        /*
         * The init procedure makes the freelist unpredictable. To make sure to trigger the ivalid ptr
         * acess, here we create as many chunks as possible to make sure there are enough chunks in
         * bin[j] of size "10 * sizeof(uintptr_t)". Basically this is heap spray.
         */
        g_tmp[i] = (uintptr_t *)malloc(10 * sizeof(uintptr_t));
    }
    for (int i = 0; i < MAX_NUMBER_VALUE; ++i) {
        free(const_cast<void *>(g_tmp[i]));
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
HWTEST_F(MallocModifyPointer, modifyPointerTest0100, Function | MediumTest | Level1)
{
    sigset_t set;
    int status;
    int flag = 0;
    struct timespec g_time1 = {5, 0};

    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, 0);
    signal(SIGCHLD, handler);

    pid_t pid = start_child();
    if (pid == -1) {
        printf("fork failed: %s\n", strerror(errno));
    }
    if (sigtimedwait(&set, 0, &g_time1) == -1) { /* Wait for 5 seconds */
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

