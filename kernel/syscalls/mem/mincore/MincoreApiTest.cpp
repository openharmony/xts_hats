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
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"


using namespace testing::ext;
using namespace std;

static const int PAGE_SIZE = 4096;
static const int PAGE_NUM = 4;

class MincoreApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void MincoreApiTest::SetUp()
{
}
void MincoreApiTest::TearDown()
{
}
void MincoreApiTest::SetUpTestCase()
{
}
void MincoreApiTest::TearDownTestCase()
{
}

static int  CheckStatus(unsigned char *vec)
{
    int ret = 4;
    for (int i = 0; i < PAGE_NUM; i++) {
        int a = vec[i] & 0x1;
        if (a == 0) {
            ret = ret - 1;
        }
    }
    return ret;
}

static int  Touch(void *addr)
{
    char *buffer1 = static_cast<char *>(addr);
    int ret1 = memset_s(buffer1, PAGE_SIZE, 'A', PAGE_SIZE);
    if (ret1 != EOK) {
        return ret1;
    }

    char *buffer2 = static_cast<char *>(addr) + 2 * PAGE_SIZE;
    int ret2 = memset_s(buffer2, PAGE_SIZE, 'A', PAGE_SIZE);
    if (ret2 != EOK) {
        return ret2;
    }

    int ret = ret1 + ret2;
    return ret;
}


/*
 * @tc.number : SUB_KERNEL_SYSCALL_MINCORE_0100
 * @tc.name   : MincoreCheckPageInMemorySuccess_0001
 * @tc.desc   : Mincore determine whether page are resident in memory.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(MincoreApiTest, MincoreCheckPageInMemorySuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    size_t length = PAGE_SIZE * PAGE_NUM;
    unsigned char *vec = new unsigned char[(length / PAGE_SIZE) + 1]();

    void *addr = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(addr, MAP_FAILED);

    ret = mincore(addr, length, vec);
    EXPECT_EQ(ret, 0);
    ret = CheckStatus(vec);
    EXPECT_EQ(ret, 0);

    ret = Touch(addr);
    EXPECT_EQ(ret, 0);

    ret = mincore(addr, length, vec);
    EXPECT_EQ(ret, 0);
    ret = CheckStatus(vec);
    EXPECT_EQ(ret, 2);

    munmap(addr, length);
    delete [] vec;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MINCORE_0200
 * @tc.name   : MincoreCheckPageInvalidAddrFail_0002
 * @tc.desc   : Mincore determine whether page are resident in invalid memory addr fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MincoreApiTest, MincoreCheckPageInMemoryFail_0002, Function | MediumTest | Level2)
{
    int ret;
    size_t length = PAGE_SIZE * PAGE_NUM;
    unsigned char *vec = new unsigned char[(length / PAGE_SIZE) + 1]();

    errno = 0;
    ret = mincore(MAP_FAILED, length, vec);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
    delete [] vec;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MINCORE_0300
 * @tc.name   : MincoreCheckPageInvalidLengthFail_0003
 * @tc.desc   : Mincore determine whether page are resident in invalid memory addr fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MincoreApiTest, MincoreCheckPageInvalidLengthFail_0003, Function | MediumTest | Level2)
{
    int ret;
    size_t length = PAGE_SIZE * PAGE_NUM;
    unsigned char *vec = new unsigned char[(length / PAGE_SIZE) + 1]();
    void *addr = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(addr, MAP_FAILED);

    errno = 0;
    ret = mincore(MAP_FAILED, 0, vec);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
    munmap(addr, length);
    delete [] vec;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MINCORE_0400
 * @tc.name   : MincoreCheckPageInMemorySuccess_0004
 * @tc.desc   : Mincore MAP_SHARED.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MincoreApiTest, MincoreCheckPageInMemorySuccess_0004, Function | MediumTest | Level2)
{
    int ret;
    size_t length = PAGE_SIZE * PAGE_NUM;
    unsigned char *vec = new unsigned char[(length / PAGE_SIZE) + 1]();

    void *addr = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    EXPECT_NE(addr, MAP_FAILED);

    ret = mincore(addr, length, vec);
    EXPECT_EQ(ret, 0);
    ret = CheckStatus(vec);
    EXPECT_EQ(ret, 0);

    ret = Touch(addr);
    EXPECT_EQ(ret, 0);

    ret = mincore(addr, length, vec);
    EXPECT_EQ(ret, 0);
    ret = CheckStatus(vec);
    EXPECT_EQ(ret, 2);

    munmap(addr, length);
    delete [] vec;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MINCORE_0500
 * @tc.name   : MincoreCheckPageInMemorySuccess_0005
 * @tc.desc   : Mincore MAP_PRIVATE FILE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MincoreApiTest, MincoreCheckPageInMemorySuccess_0005, Function | MediumTest | Level2)
{
    int ret;
    const char *filename = "testfile";
    size_t length = PAGE_SIZE * PAGE_NUM;
    unsigned char *vec = new unsigned char[(length / PAGE_SIZE) + 1]();

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_GE(fd, 0);

    ret = ftruncate(fd, PAGE_SIZE * PAGE_NUM);
    EXPECT_NE(ret, -1);

    void *addr = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    EXPECT_NE(addr, MAP_FAILED);

    ret = mincore(addr, length, vec);
    EXPECT_EQ(ret, 0);
    ret = CheckStatus(vec);
    EXPECT_EQ(ret, 0);

    ret = Touch(addr);
    EXPECT_EQ(ret, 0);

    ret = mincore(addr, length, vec);
    EXPECT_EQ(ret, 0);
    ret = CheckStatus(vec);
    EXPECT_EQ(ret, 4);

    munmap(addr, length);
    close(fd);
    unlink(filename);
    delete [] vec;
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_MINCORE_0600
 * @tc.name   : MincoreCheckPageInMemorySuccess_0006
 * @tc.desc   : Mincore MAP_SHARED FILE.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(MincoreApiTest, MincoreCheckPageInMemorySuccess_0006, Function | MediumTest | Level2)
{
    int ret;
    const char *filename = "testfile";
    size_t length = PAGE_SIZE * PAGE_NUM;
    unsigned char *vec = new unsigned char[(length / PAGE_SIZE) + 1]();

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_GE(fd, 0);

    ret = ftruncate(fd, PAGE_SIZE * PAGE_NUM);
    EXPECT_NE(ret, -1);

    void *addr = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_NE(addr, MAP_FAILED);

    ret = mincore(addr, length, vec);
    EXPECT_EQ(ret, 0);
    ret = CheckStatus(vec);
    EXPECT_EQ(ret, 0);

    ret = Touch(addr);
    EXPECT_EQ(ret, 0);

    ret = mincore(addr, length, vec);
    EXPECT_EQ(ret, 0);
    ret = CheckStatus(vec);
    EXPECT_EQ(ret, 4);

    munmap(addr, length);
    close(fd);
    unlink(filename);
    delete [] vec;
}