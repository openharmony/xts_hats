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
#include <csetjmp>
#include "securec.h"

#define PAGE_SIZE 4096

using namespace testing::ext;
using namespace std;

class MmapApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MmapApiTest::SetUp()
{
}
void MmapApiTest::TearDown()
{
}
void MmapApiTest::SetUpTestCase()
{
}
void MmapApiTest::TearDownTestCase()
{
}

static int g_sigStarted = 0;

static void SigsegvHandler(int signum)
{
    if (g_sigStarted) {
        g_sigStarted = 0;
    } else {
        std::cout << "case failed.\n" << std::endl;
        ASSERT_TRUE(g_sigStarted != 0);
    }
}

static void CreateFile()
{
    FILE *file;
    int ret = -1;
    char *buffer =  static_cast<char *>(malloc(PAGE_SIZE));
    if (buffer != nullptr) {
        ret = memset_s(buffer, PAGE_SIZE, 'a', PAGE_SIZE);
        ASSERT_TRUE(ret == 0);
        file = fopen("output_file.txt", "wb");
        if (fwrite(buffer, 1, PAGE_SIZE, file) != PAGE_SIZE) {
            std::cout << "Error fwrite file.\n" << std::endl;
            ASSERT_TRUE(ret != 0);
        }
        fclose(file);
    }
    free(buffer);
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_0100
 * @tc.name   MremapDontunmap001
 * @tc.desc   mremap use MREMAP_DONTUNMAP with old len == new len
*/
HWTEST_F(MmapApiTest, MremapDontunmap001, Function | MediumTest | Level1)
{
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    void *fixAddr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(fixAddr != MAP_FAILED);
    void *newAddr = mremap(addr, PAGE_SIZE, PAGE_SIZE, MREMAP_MAYMOVE | MREMAP_FIXED | MREMAP_DONTUNMAP, fixAddr);
    ASSERT_TRUE(newAddr != MAP_FAILED);
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = munmap(fixAddr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = munmap(newAddr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    fixAddr = nullptr;
    newAddr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_0200
 * @tc.name   MremapDontunmap002
 * @tc.desc   mremap use MREMAP_DONTUNMAP with MAP_SHARED
*/
HWTEST_F(MmapApiTest, MremapDontunmap002, Function | MediumTest | Level2)
{
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    void *fixAddr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    ASSERT_TRUE(fixAddr != MAP_FAILED);
    void *newAddr = mremap(fixAddr, PAGE_SIZE, PAGE_SIZE, MREMAP_MAYMOVE | MREMAP_FIXED | MREMAP_DONTUNMAP, addr);
    ASSERT_TRUE(newAddr != MAP_FAILED);
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = munmap(fixAddr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = munmap(newAddr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    fixAddr = nullptr;
    newAddr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_0300
 * @tc.name   MremapDontunmap003
 * @tc.desc   mremap use MREMAP_DONTUNMAP with old len != new len
*/
HWTEST_F(MmapApiTest, MremapDontunmap003, Function | MediumTest | Level1)
{
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    void *fixAddr = mmap(nullptr, PAGE_SIZE * 2, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(fixAddr != MAP_FAILED);
    void *newAddr = mremap(addr, PAGE_SIZE, PAGE_SIZE * 2, MREMAP_MAYMOVE | MREMAP_FIXED | MREMAP_DONTUNMAP, fixAddr);
    ASSERT_TRUE(newAddr == MAP_FAILED);
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = munmap(fixAddr, PAGE_SIZE * 2);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    fixAddr = nullptr;
    newAddr = nullptr;
}
/*
 * @tc.number SUB_KERNEL_MEM_MAP_0400
 * @tc.name   MremapDontunmap004
 * @tc.desc   mremap use MREMAP_DONTUNMAP without MREMAP_MAYMOVE
*/
HWTEST_F(MmapApiTest, MremapDontunmap004, Function | MediumTest | Level1)
{
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    void *newAddr = mremap(addr, PAGE_SIZE, PAGE_SIZE,  MREMAP_FIXED | MREMAP_DONTUNMAP);
    ASSERT_TRUE(newAddr == MAP_FAILED);
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    newAddr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_0500
 * @tc.name   MremapDontunmap005
 * @tc.desc   mremap use MREMAP_DONTUNMAP with MREMAP_MAYMOVE
*/
HWTEST_F(MmapApiTest, MremapDontunmap005, Function | MediumTest | Level1)
{
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    void *newAddr = mremap(addr, PAGE_SIZE, PAGE_SIZE,  MREMAP_MAYMOVE | MREMAP_DONTUNMAP);
    ASSERT_TRUE(newAddr != MAP_FAILED);
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = munmap(newAddr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    newAddr = nullptr;
}

/*
 * @tc.number  SUB_KERNEL_MEM_MAP_0800
 * @tc.name    MMAPShardValidate001
 * @tc.desc    mmap use MAP_SHARED_VALIDATE or MAP_SHARED with MAP_SYNC
*/
HWTEST_F(MmapApiTest, MMAPShardValidate001, Function | MediumTest | Level1)
{
    void *va = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON | MAP_SYNC, -1, 0);
    ASSERT_TRUE(va != MAP_FAILED);
    void *newVa = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE | MAP_ANON | MAP_SYNC, -1, 0);
    ASSERT_TRUE(newVa == MAP_FAILED);
    int ret = munmap(va, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    va = nullptr;
    newVa = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_0900
 * @tc.name   MMAPShardValidate002
 * @tc.desc   mmap use MAP_SHARED_VALIDATE  with no error param
*/
HWTEST_F(MmapApiTest, MMAPShardValidate002, Function | MediumTest | Level1)
{
    CreateFile();
    int fd = open("output_file.txt", O_RDWR);
    ASSERT_TRUE(fd > 0);
    void *va = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE, fd, 0);
    ASSERT_TRUE(va != MAP_FAILED);
    int ret = munmap(va, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    va = nullptr;
    close(fd);
    ret = unlink("output_file.txt");
    ASSERT_TRUE(ret == 0);
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_1000
 * @tc.name   MMAPNonBlock001
 * @tc.desc   mmap use MAP_NONBLOCK
*/
HWTEST_F(MmapApiTest, MMAPNonBlock001, Function | MediumTest | Level1)
{
    void *va = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NONBLOCK, -1, 0);
    ASSERT_TRUE(va != MAP_FAILED);
    int ret = munmap(va, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    va = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_1400
 * @tc.name   MMAPGrownsDown001
 * @tc.desc   MAP_GROWSDONW when the pre-vregion is not MAP_GROWSDONW
*/
HWTEST_F(MmapApiTest, MMAPGrownsDown001, Function | MediumTest | Level1)
{
    const size_t lenGdGap = (2UL << 20);
    const size_t lenLeftMap = 0x1000;
    const size_t lenGd = 0x1000;
    const size_t lenReserve = lenLeftMap + lenGdGap + lenGd;

    char *addReserve = nullptr;
    char *addrGd = nullptr;
    int ret = -1;

    addReserve = (char *)mmap(nullptr, lenReserve, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addReserve != MAP_FAILED);
    void *addr1 = mmap(addReserve, lenLeftMap, PROT_READ, MAP_ANON | MAP_PRIVATE | MAP_FIXED, -1, 0);
    ASSERT_TRUE(addr1 != MAP_FAILED);
    addrGd = (char *)mmap(addReserve + lenLeftMap + lenGdGap, lenGd, PROT_READ | PROT_WRITE,
         MAP_ANON | MAP_PRIVATE | MAP_FIXED | MAP_GROWSDOWN, -1, 0);
    ASSERT_TRUE(addrGd != MAP_FAILED);

    g_sigStarted = 1;
    struct sigaction sa;
    sa.sa_handler = SigsegvHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    ret = sigaction(SIGSEGV, &sa, nullptr);
    ASSERT_TRUE(ret == 0);
    int cpage = 256;
    addrGd[cpage * -0x1000] = '1';
    ret = munmap(addReserve, lenReserve);
    ASSERT_TRUE(ret == 0);
    ret = munmap(addr1, lenLeftMap);
    ASSERT_TRUE(ret == 0);
    ret = munmap(addrGd, lenGd);
    ASSERT_TRUE(ret == 0);
    addReserve = nullptr;
    addr1 = nullptr;
    addrGd = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_1500
 * @tc.name   MMAPGrownsDown002
 * @tc.desc   mprotect use PROT_GROWSDONW
*/
HWTEST_F(MmapApiTest, MMAPGrownsDown002, Function | MediumTest | Level1)
{
    const size_t lenGdGap = (2UL << 20);
    const size_t lenLeftMap = 0x1000;
    const size_t lenGd = 0x1000;
    const size_t lenReserve = lenLeftMap + lenGdGap + lenGd;
    char *addReserve = nullptr;
    char *addrGd = nullptr;
    int ret = -1;

    addReserve = (char *)mmap(nullptr, lenReserve, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addReserve != MAP_FAILED);
    void *addr1 = mmap(addReserve, lenLeftMap, PROT_READ, MAP_ANON | MAP_PRIVATE | MAP_FIXED, -1, 0);
    ASSERT_TRUE(addr1 != MAP_FAILED);
    addrGd = (char *)mmap(addReserve + lenLeftMap + lenGdGap, lenGd, PROT_READ,
         MAP_ANON | MAP_PRIVATE | MAP_FIXED | MAP_GROWSDOWN, -1, 0);
    ASSERT_TRUE(addrGd != MAP_FAILED);
    
    ret = mprotect(addrGd, lenGd, PROT_READ | PROT_WRITE | PROT_GROWSDOWN);
    ASSERT_TRUE(ret == 0);
    g_sigStarted = 1;
    struct sigaction sa;
    sa.sa_handler = SigsegvHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    ret = sigaction(SIGSEGV, &sa, nullptr);
    ASSERT_TRUE(ret == 0);
    int cpage = 256;
    addrGd[cpage * -0x1000] = '1';
    ret = munmap(addReserve, lenReserve);
    ASSERT_TRUE(ret == 0);
    ret = munmap(addr1, lenLeftMap);
    ASSERT_TRUE(ret == 0);
    ret = munmap(addrGd, lenGd);
    ASSERT_TRUE(ret == 0);
    addReserve = nullptr;
    addr1 = nullptr;
    addrGd = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_1600
 * @tc.name   MMAPGrownsDown003
 * @tc.desc   MAP_GROWSDONW when MAP_SHARED
*/
HWTEST_F(MmapApiTest, MMAPGrownsDown003, Function | MediumTest | Level1)
{
    void *va = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED | MAP_GROWSDOWN, -1, 0);
    ASSERT_TRUE(va == MAP_FAILED);
    va = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MAP_1700
 * @tc.name   MMAPGrownsDown004
 * @tc.desc   MAP_GROWSDONW when the pre-vregion is  MAP_GROWSDONW
*/
HWTEST_F(MmapApiTest, MMAPGrownsDown004, Function | MediumTest | Level1)
{
    const size_t lenGd1 = 0x2000;
    const size_t lenGd2 = 0x2000;
    const size_t lenReserve = 0x1000 * 6;

    char *addReserve = nullptr;
    char *addrGd1 = nullptr;
    char *addrGd2 = nullptr;
    int ret = -1;

    addReserve = (char *)mmap(nullptr, lenReserve, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addReserve != MAP_FAILED);
    addrGd1 = (char *)mmap(addReserve, lenGd1, PROT_READ  | PROT_WRITE,
         MAP_ANON | MAP_PRIVATE | MAP_FIXED | MAP_GROWSDOWN, -1, 0);
    ASSERT_TRUE(addrGd1 != MAP_FAILED);
    addrGd2 = (char *)mmap(addReserve + (lenReserve - lenGd2), lenGd2, PROT_READ  | PROT_WRITE,
         MAP_ANON | MAP_PRIVATE | MAP_FIXED | MAP_GROWSDOWN, -1, 0);
    ASSERT_TRUE(addrGd2 != MAP_FAILED);
    g_sigStarted = 1;
    struct sigaction sa;
    sa.sa_handler = SigsegvHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    ret = sigaction(SIGSEGV, &sa, nullptr);
    ASSERT_TRUE(ret == 0);
    addrGd2[-0x1000] = '1';
    addrGd2[-0x2000] = '1';
    ret = munmap(addReserve, lenReserve);
    ASSERT_TRUE(ret == 0);
    ret = munmap(addrGd1, lenGd1);
    ASSERT_TRUE(ret == 0);
    ret = munmap(addrGd2, lenGd2);
    ASSERT_TRUE(ret == 0);
    addReserve = nullptr;
    addrGd1 = nullptr;
    addrGd2 = nullptr;
}