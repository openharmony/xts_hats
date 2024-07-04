/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include <fcntl.h>
#include <cinttypes>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <sys/mman.h>
#include <sys/utsname.h>
#include <string>
#include <memory.h>
#include "securec.h"

#define PAGE_SIZE 4096
#define NR_HUGEPAGES 10U
#define NR_OVERCOMMIT_HUGEPAGES 20U
#define HUGETLB_SHIFT 21
#define OVERCOMMIT_RATIO 1000000000
#define KB_SHIFT 10U
#define NUM 1024
using namespace testing::ext;
using namespace std;

class MmapVApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MmapVApiTest::SetUp()
{
}
void MmapVApiTest::TearDown()
{
}
void MmapVApiTest::SetUpTestCase()
{
}
void MmapVApiTest::TearDownTestCase()
{
}


enum {
    OVERCOMMIT_GUESS,
    OVERCOMMIT_NEVER,
};

static int g_overcommitPolicy;
static int g_overcommitRatio;

static void SetProc(string entry, int input)
{
    FILE *fp = fopen(entry.c_str(), "w");
    if (fp != nullptr) {
        (void)fprintf(fp, "%d", input);
        (void)fclose(fp);
    }
}

static void FetchProc(string entry, int *output)
{
    char buffer[NUM];
    FILE *fp = fopen(entry.c_str(), "r");
    if (fp != nullptr) {
        int ret = snprintf_s(buffer, sizeof(buffer),  sizeof(buffer) - 1, "%d", *output);
        if (ret == 0) {
            if (fputs(buffer, fp) == EOF) {
                std::cout << "Error fputs .\n" << std::endl;
            }
        }
    }
}

static void SetOvercommitMemory(int input)
{
    SetProc("/proc/sys/vm/overcommit_memory", input);
}

static void FetchOvercommitMemory(int *output)
{
    FetchProc("/proc/sys/vm/overcommit_memory", output);
}

static void SetOvercommitRatio(int input)
{
    SetProc("/proc/sys/vm/overcommit_ratio", input);
}

static void FetchOvercommitRatio(int *output)
{
    FetchProc("/proc/sys/vm/overcommit_ratio", output);
}

static int TstSmaps(void *va)
{
    FILE *fp;
    char cmd[NUM];
    char line[NUM];
    char *p;
    int rss = 0;
    unsigned long ptr = reinterpret_cast<unsigned long>(va);
    int ret = snprintf_s(cmd, sizeof(cmd), sizeof(cmd) - 1,
         "cat /proc/%d/smaps |grep -A 10 '%lx-' |grep 'Rss:'", getpid(), ptr);
    if (ret < 0) {
        std::cout << "Error snprintf_s open .\n" << std::endl;
        return ret;
    }
    fp = popen(cmd, "r");
    if (fp == nullptr) {
        std::cout << "Error popen open .\n" << std::endl;
        return -1;
    }
    while (fgets(line, NUM, fp) != nullptr) {
        p = strstr(line, "Rss:");
        if (p != nullptr) {
            int err = sscanf_s(p, "Rss: %d KB", &rss);
            if (err < 0) {
                std::cout << "Error sscanf_s open .\n" << std::endl;
                return err;
            }
            break;
        }
    }
    pclose(fp);
    return rss;
}

static void ForceReclaim(void)
{
    FILE *fp = fopen("/proc/self/reclaim", "w");
    if (fp != nullptr) {
        fputs("5", fp);
        fclose(fp);
    } else {
        std::cout << "Error ForceReclaim open .\n" << std::endl;
        ASSERT_TRUE(fp != nullptr);
    }
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_0600
 * @tc.name   MMAPNoreServe001
 * @tc.desc   mmap use MAP_NORESERVE without OVERCOMMIT_NEVER
*/
HWTEST_F(MmapVApiTest, MMAPNoreServe001, Function | MediumTest | Level1)
{
    FetchOvercommitMemory(&g_overcommitPolicy);
    ASSERT_TRUE(g_overcommitPolicy >= 0);
    FetchOvercommitRatio(&g_overcommitRatio);
    ASSERT_TRUE(g_overcommitRatio >= 0);
    SetOvercommitRatio(OVERCOMMIT_RATIO);
    SetOvercommitMemory(OVERCOMMIT_GUESS);
    void *va = mmap(nullptr, (NR_OVERCOMMIT_HUGEPAGES << HUGETLB_SHIFT), PROT_READ | PROT_WRITE,
         MAP_ANON | MAP_PRIVATE | MAP_HUGETLB | MAP_NORESERVE, -1, 0);
    ASSERT_TRUE(va != MAP_FAILED);
    int ret = munmap(va, (NR_OVERCOMMIT_HUGEPAGES << HUGETLB_SHIFT));
    ASSERT_TRUE(ret == 0);
    va = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_0700
 * @tc.name   MMAPNoreServe002
 * @tc.desc   mmap use MAP_NORESERVE with OVERCOMMIT_NEVER
*/
HWTEST_F(MmapVApiTest, MMAPNoreServe002, Function | MediumTest | Level1)
{
    FetchOvercommitMemory(&g_overcommitPolicy);
    ASSERT_TRUE(g_overcommitPolicy >= 0);
    FetchOvercommitRatio(&g_overcommitRatio);
    ASSERT_TRUE(g_overcommitRatio >= 0);
    SetOvercommitRatio(OVERCOMMIT_RATIO);
    SetOvercommitMemory(OVERCOMMIT_NEVER);

    void *va = mmap(nullptr, (NR_OVERCOMMIT_HUGEPAGES << HUGETLB_SHIFT), PROT_READ | PROT_WRITE,
         MAP_ANON | MAP_PRIVATE | MAP_HUGETLB | MAP_NORESERVE, -1, 0);
    ASSERT_TRUE(va != MAP_FAILED);
    int ret = munmap(va, (NR_OVERCOMMIT_HUGEPAGES << HUGETLB_SHIFT));
    ASSERT_TRUE(ret == 0);
    va = nullptr;
}


/*
 * @tc.number SUB_KERNEL_MEM_MAP_1100
 * @tc.name   MMAPNonBlock002
 * @tc.desc   mmap use MAP_NONBLOCK with MAP_POPULATE
*/
HWTEST_F(MmapVApiTest, MMAPNonBlock002, Function | MediumTest | Level1)
{
    void *va = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE,
         MAP_ANON | MAP_PRIVATE | MAP_NONBLOCK | MAP_POPULATE, -1, 0);
    ASSERT_TRUE(va != MAP_FAILED);
    int ret = munmap(va, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    va = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_1200
 * @tc.name   mlockall001
 * @tc.desc   mlockall current pthread
*/
HWTEST_F(MmapVApiTest, mlockall001, Function | MediumTest | Level1)
{
    void *va = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(va != MAP_FAILED);
    int rc = mlockall(MCL_CURRENT | MCL_ONFAULT);
    ASSERT_TRUE(rc == 0);
    int64_t rss = TstSmaps(va);
    ASSERT_TRUE(rss == 0LL);
    *(char *)va = 1;
    ForceReclaim();
    rss = TstSmaps(va);
    ASSERT_TRUE(rss == (PAGE_SIZE >> KB_SHIFT));
    rc = munlockall();
    ASSERT_TRUE(rc == 0);
    int ret = munmap(va, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    va = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_1300
 * @tc.name   mlockall002
 * @tc.desc   create vregion after mlockall
*/
HWTEST_F(MmapVApiTest, mlockall002, Function | MediumTest | Level1)
{
    int rc = mlockall(MCL_CURRENT | MCL_ONFAULT);
    ASSERT_TRUE(rc == 0);
    void *va = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(va != MAP_FAILED);
    int64_t rss = TstSmaps(va);
    ASSERT_TRUE(rss == 0LL);
    *(char *)va = 1;
    ForceReclaim();
    rss = TstSmaps(va);
    ASSERT_TRUE(rss == (PAGE_SIZE >> KB_SHIFT));
    rc = munlockall();
    ASSERT_TRUE(rc == 0);
    int ret = munmap(va, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    va = nullptr;
}