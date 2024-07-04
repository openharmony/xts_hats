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
#include <regex>
#include "securec.h"

#define PAGE_SIZE 4096


using namespace testing::ext;
using namespace std;

class MadviseApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MadviseApiTest::SetUp()
{
}
void MadviseApiTest::TearDown()
{
}
void MadviseApiTest::SetUpTestCase()
{
}
void MadviseApiTest::TearDownTestCase()
{
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
 * @tc.number SUB_KERNEL_MEM_MADVISE_0100
 * @tc.name   MadviseMadvCold001
 * @tc.desc   MADV_COLD with MAP_PRIVATE
*/
HWTEST_F(MadviseApiTest, MadviseMadvCold001, Function | MediumTest | Level1)
{
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    int err = madvise(addr, PAGE_SIZE, MADV_COLD);
    ASSERT_TRUE(err == 0);
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_0200
 * @tc.name   MadviseMadvCold002
 * @tc.desc   MADV_COLD with MAP_SHARED
*/
HWTEST_F(MadviseApiTest, MadviseMadvCold002, Function | MediumTest | Level1)
{
    CreateFile();
    int fd = open("output_file.txt", O_RDWR);
    ASSERT_TRUE(fd > 0);
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED | MAP_FILE, fd, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    int ret = madvise(addr, PAGE_SIZE, MADV_COLD);
    ASSERT_TRUE(ret == 0);
    ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    close(fd);
    ret = unlink("output_file.txt");
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_0300
 * @tc.name   MadviseMadvCold003
 * @tc.desc   MADV_COLD with mlock MAP_PRIVATE FILE
*/
HWTEST_F(MadviseApiTest, MadviseMadvCold003, Function | MediumTest | Level1)
{
    CreateFile();
    int fd = open("output_file.txt", O_RDWR);
    ASSERT_TRUE(fd > 0);
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED | MAP_FILE, fd, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    int ret = mlock(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = madvise(addr, PAGE_SIZE, MADV_COLD);
    ASSERT_TRUE(ret == -1);
    ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    close(fd);
    ret = unlink("output_file.txt");
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_0400
 * @tc.name   MadviseMadvCold004
 * @tc.desc   MADV_COLD with mlock MAP_PRIVATE anon
*/
HWTEST_F(MadviseApiTest, MadviseMadvCold004, Function | MediumTest | Level1)
{
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    int ret = mlock(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = madvise(addr, PAGE_SIZE, MADV_COLD);
    ASSERT_TRUE(ret == -1);
    ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_0500
 * @tc.name MadviseMadvPageOut001
 * @tc.desc MADV_PAGEOUT with  MAP_PRIVATE anon
*/
HWTEST_F(MadviseApiTest, MadviseMadvPageOut001, Function | MediumTest | Level1)
{
    int ret = -1;
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    ret = memset_s(addr, PAGE_SIZE, 'a', PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = madvise(addr, PAGE_SIZE, MADV_PAGEOUT);
    ASSERT_TRUE(ret == 0);
    ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_0600
 * @tc.name MadviseMadvPageOut002
 * @tc.desc MADV_PAGEOUT with  MAP_SHARED FILE
*/
HWTEST_F(MadviseApiTest, MadviseMadvPageOut002, Function | MediumTest | Level1)
{
    CreateFile();
    int fd = open("output_file.txt", O_RDWR);
    ASSERT_TRUE(fd > 0);
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED | MAP_FILE, fd, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    int ret = memset_s(addr, PAGE_SIZE, 'a', PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = madvise(addr, PAGE_SIZE, MADV_PAGEOUT);
    ASSERT_TRUE(ret == 0);
    ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    close(fd);
    ret = unlink("output_file.txt");
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_0700
 * @tc.name   MadviseMadvPageOut003
 * @tc.desc   MADV_PAGEOUT with mlock MAP_SHARED FILE
*/
HWTEST_F(MadviseApiTest, MadviseMadvPageOut003, Function | MediumTest | Level1)
{
    CreateFile();
    int fd = open("output_file.txt", O_RDWR);
    ASSERT_TRUE(fd > 0);
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED | MAP_FILE, fd, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    int ret = memset_s(addr, PAGE_SIZE, 'a', PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = mlock(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = madvise(addr, PAGE_SIZE, MADV_PAGEOUT);
    ASSERT_TRUE(ret == -1);
    ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    close(fd);
    ret = unlink("output_file.txt");
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_0800
 * @tc.name   MadviseMadvPageOut004
 * @tc.desc   MADV_PAGEOUT with mlock MAP_PRIVATE anon
*/
HWTEST_F(MadviseApiTest, MadviseMadvPageOut004, Function | MediumTest | Level1)
{
    int ret = -2;
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    ret = memset_s(addr, PAGE_SIZE, 'a', PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = mlock(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = madvise(addr, PAGE_SIZE, MADV_PAGEOUT);
    ASSERT_TRUE(ret == -1);
    ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_0900
 * @tc.name   MadviseMadvWipeOnFork001
 * @tc.desc   MADV_WIPEONFORK with  MAP_PRIVATE anon
*/
HWTEST_F(MadviseApiTest, MadviseMadvWipeOnFork001, Function | MediumTest | Level1)
{
    int status = 0;
    char *addr = static_cast<char*>(mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE,
         MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
    ASSERT_TRUE(addr != MAP_FAILED);
    *addr = 'A';
    int err = madvise(addr, PAGE_SIZE, MADV_WIPEONFORK);
    ASSERT_TRUE(err == 0);
    pid_t pid = fork();
    ASSERT_TRUE(pid >= 0);
    if (pid == 0)
    {
        ASSERT_TRUE(*addr == 0);
        exit(0);
    } else {
        ASSERT_TRUE(*addr == 'A');
        waitpid(0, &status, 0);
    }
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number  SUB_KERNEL_MEM_MADVISE_1000
 * @tc.name    MadviseMadvWipeOnFork002
 * @tc.desc    MADV_WIPEONFORK with  MAP_SHARED
*/
HWTEST_F(MadviseApiTest, MadviseMadvWipeOnFork002, Function | MediumTest | Level1)
{
    void *addr = mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    ASSERT_TRUE(addr != MAP_FAILED);
    int err = madvise(addr, PAGE_SIZE, MADV_WIPEONFORK);
    ASSERT_TRUE(err == -1);
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_1100
 * @tc.name   MadviseMadvWipeOnFork003
 * @tc.desc   MADV_KEEPONFORK after MADV_WIPEONFORK
*/
HWTEST_F(MadviseApiTest, MadviseMadvWipeOnFork003, Function | MediumTest | Level1)
{
    int status = 0;
    char *addr = static_cast<char*>(mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE,
         MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
    ASSERT_TRUE(addr != MAP_FAILED);
    *addr = 'A';
    int err = madvise(addr, PAGE_SIZE, MADV_WIPEONFORK);
    ASSERT_TRUE(err == 0);
    err = madvise(addr, PAGE_SIZE, MADV_KEEPONFORK);
    ASSERT_TRUE(err == 0);
    pid_t pid = fork();
    ASSERT_TRUE(pid >= 0);
    if (pid == 0)
    {
        ASSERT_TRUE(*addr == 'A');
        exit(0);
    } else {
        ASSERT_TRUE(*addr == 'A');
        waitpid(0, &status, 0);
    }
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_1200
 * @tc.name   MadviseMadvWipeOnFork004
 * @tc.desc   next vregion is no MADV_WIPEONFORK
*/
HWTEST_F(MadviseApiTest, MadviseMadvWipeOnFork004, Function | MediumTest | Level1)
{
    int status = 0;
    char *addr = static_cast<char*>(mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE,
         MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
    ASSERT_TRUE(addr != MAP_FAILED);
    char *nextAddr = static_cast<char*>(mmap(addr, PAGE_SIZE, PROT_READ | PROT_WRITE,
         MAP_ANONYMOUS | MAP_PRIVATE, -1, PAGE_SIZE));
    ASSERT_TRUE(nextAddr != MAP_FAILED);
    *addr = 'A';
    *nextAddr = 'B';
    int err = madvise(addr, PAGE_SIZE, MADV_WIPEONFORK);
    ASSERT_TRUE(err == 0);
    pid_t pid = fork();
    ASSERT_TRUE(pid >= 0);
    if (pid == 0)
    {
        ASSERT_TRUE(*addr == 0);
        ASSERT_TRUE(*nextAddr == 'B');
        exit(0);
    } else {
        ASSERT_TRUE(*addr == 'A');
        ASSERT_TRUE(*nextAddr == 'B');
        waitpid(0, &status, 0);
    }
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = munmap(nextAddr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    nextAddr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_1300
 * @tc.name   MadviseMadvWipeOnFork005
 * @tc.desc   next vregion is MADV_WIPEONFORK
*/
HWTEST_F(MadviseApiTest, MadviseMadvWipeOnFork005, Function | MediumTest | Level1)
{
    int status = 0;
    char *addr = static_cast<char*>(mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE,
         MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
    ASSERT_TRUE(addr != MAP_FAILED);
    char *nextAddr = static_cast<char*>(mmap(addr, PAGE_SIZE, PROT_READ | PROT_WRITE,
         MAP_ANONYMOUS | MAP_PRIVATE, -1, PAGE_SIZE));
    ASSERT_TRUE(nextAddr != MAP_FAILED);
    *addr = 'A';
    *nextAddr = 'B';
    int err = madvise(addr, PAGE_SIZE, MADV_WIPEONFORK);
    ASSERT_TRUE(err == 0);
    err = madvise(nextAddr, PAGE_SIZE, MADV_WIPEONFORK);
    ASSERT_TRUE(err == 0);
    pid_t pid = fork();
    ASSERT_TRUE(pid >= 0);
    if (pid == 0)
    {
        ASSERT_TRUE(*addr == 0);
        ASSERT_TRUE(*nextAddr == 0);
        exit(0);
    } else {
        ASSERT_TRUE(*addr == 'A');
        ASSERT_TRUE(*nextAddr == 'B');
        waitpid(0, &status, 0);
    }
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = munmap(nextAddr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    nextAddr = nullptr;
}

/*
 * @tc.number SUB_KERNEL_MEM_MADVISE_1400
 * @tc.name   MadviseMadvWipeOnFork006
 * @tc.desc   next vregion is MADV_KEEPONFORK
*/
HWTEST_F(MadviseApiTest, MadviseMadvWipeOnFork006, Function | MediumTest | Level1)
{
    int status = 0;
    char *addr = static_cast<char*>(mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE,
         MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
    ASSERT_TRUE(addr != MAP_FAILED);
    char *nextAddr = static_cast<char*>(mmap(addr, PAGE_SIZE, PROT_READ | PROT_WRITE,
         MAP_ANONYMOUS | MAP_PRIVATE, -1, PAGE_SIZE));
    ASSERT_TRUE(nextAddr != MAP_FAILED);
    *addr = 'A';
    *nextAddr = 'B';
    int err = madvise(addr, PAGE_SIZE, MADV_WIPEONFORK);
    ASSERT_TRUE(err == 0);
    err = madvise(nextAddr, PAGE_SIZE, MADV_WIPEONFORK);
    ASSERT_TRUE(err == 0);
    err = madvise(addr, PAGE_SIZE, MADV_KEEPONFORK);
    ASSERT_TRUE(err == 0);
    err = madvise(nextAddr, PAGE_SIZE, MADV_KEEPONFORK);
    ASSERT_TRUE(err == 0);
    pid_t pid = fork();
    ASSERT_TRUE(pid >= 0);
    if (pid == 0)
    {
        ASSERT_TRUE(*addr == 'A');
        ASSERT_TRUE(*nextAddr == 'B');
        exit(0);
    } else {
        ASSERT_TRUE(*addr == 'A');
        ASSERT_TRUE(*nextAddr == 'B');
        waitpid(0, &status, 0);
    }
    int ret = munmap(addr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    ret = munmap(nextAddr, PAGE_SIZE);
    ASSERT_TRUE(ret == 0);
    addr = nullptr;
    nextAddr = nullptr;
}