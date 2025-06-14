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
#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "securec.h"
#include <linux/io_uring.h>
#include <sys/syscall.h>
#include <sys/mman.h>

//Simplified io_uring_enter
static int io_uring_enter(int fd, unsigned to_submit, unsigned min_complete, unsigned flags) {
    return syscall(__NR_io_uring_enter, fd, to_submit, min_complete, flags, NULL, 0);
}

static int io_uring_setup(unsigned entries, struct io_uring_params *p) {
    return syscall(__NR_io_uring_setup, entries, p);
}

using namespace testing::ext;

static const char *TEST_READ_FILE = "/data/local/tmp/splice_read_file.txt";
static const char *TEST_WRITE_FILE = "/data/local/tmp/splice_write_file.txt";
static const char *TEST_DATA = "Hello World!";
static const int TEST_DATA_LEN = strlen(TEST_DATA);
//static const int MAX_LEN = 128;

class HatsEnterTest : public testing::Test {
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:
};
void HatsEnterTest::SetUp()
{
    int fd = open(TEST_READ_FILE, O_WRONLY | O_CREAT, 0644);
    write(fd, TEST_DATA, TEST_DATA_LEN);
    close(fd);
}
void HatsEnterTest::TearDown()
{
    (void)remove(TEST_READ_FILE);
    (void)remove(TEST_WRITE_FILE);
}
void HatsEnterTest::SetUpTestCase()
{
    static io_uring_params p = {0};
    io_uring_setup(1, &p);
    if (errno == ENOSYS) {
        GTEST_SKIP() << "Not support wearable, skip testCase";
    }
}
void HatsEnterTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_0100
 * @tc.name   : EnterSqeSuccess_0001
 * @tc.desc   : Enter submit sqe success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEnterTest, EnterSqeSuccess_0001, Function | MediumTest | Level1)
{
    struct io_uring_params p = {0};
    int uringFd = io_uring_setup(4, &p);
    EXPECT_TRUE(uringFd > 0);

    int ret = io_uring_enter(uringFd, 1, 1, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_0200
 * @tc.name   : EnterSqeSuccess_0002
 * @tc.desc   : Enter submit sqe success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEnterTest, EnterSqeSuccess_0002, Function | MediumTest | Level1)
{
    struct io_uring_params p = {0};
    int uringFd = io_uring_setup(4, &p);
    EXPECT_TRUE(uringFd > 0);

    int ret = io_uring_enter(uringFd, 1, 1, IORING_ENTER_GETEVENTS);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_0300
 * @tc.name   : EnterSqeSuccess_0003
 * @tc.desc   : Enter submit sqe success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(HatsEnterTest, EnterSqeSuccess_0003, Function | MediumTest | Level1)
{
    struct io_uring_params p = {0};
    int uringFd = io_uring_setup(4, &p);
    EXPECT_TRUE(uringFd > 0);

    int ret = io_uring_enter(uringFd, 0, 0, IORING_ENTER_GETEVENTS);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_0400
 * @tc.name   : EnterSqeInvalidFdFailed_0004
 * @tc.desc   : Enter was provide invaild file fd return failed,errno EBADF
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEnterTest, EnterSqeInvalidFdFailed_0004, Function | MediumTest | Level2)
{
    struct io_uring_params p = {0};
    int uringFd = io_uring_setup(4, &p);
    EXPECT_TRUE(uringFd > 0);
    
    //fd is invalid fd,failed
    int ret = io_uring_enter(-1, 1, 1, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_0500
 * @tc.name   : EnterSqeNotSupportFdFailed_0005
 * @tc.desc   : Enter was provide not support file fd return failed,errno EOPNOTSUPP
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEnterTest, EnterSqeNotSupportFdFailed_0005, Function | MediumTest | Level2)
{
    struct io_uring_params p = {0};
    int uringFd = io_uring_setup(4, &p);
    EXPECT_TRUE(uringFd > 0);
    
    //fd is 0,failed
    int ret = io_uring_enter(0, 1, 1, 0);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EOPNOTSUPP);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_0600
 * @tc.name   : EnterSqeAbnormalFdFailed_0006
 * @tc.desc   : Enter was provide notabnormal file fd return failed
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEnterTest, EnterSqeAbnormalFdFailed_0006, Function | MediumTest | Level2)
{
    struct io_uring_params p = {0};
    int uringFd = io_uring_setup(4, &p);
    EXPECT_TRUE(uringFd > 0);
    
    //fd is not normal fd,failed
    int ret = io_uring_enter(uringFd + 10, 1, 1, 0);
    EXPECT_EQ(ret, -1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_0700
 * @tc.name   : EnterSqeInvalidFlagFailed_0007
 * @tc.desc   : Enter was provide invalid flag return failed, errno EINVAL
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEnterTest, EnterSqeInvalidFlagFailed_0007, Function | MediumTest | Level2)
{
    struct io_uring_params p = {0};
    int uringFd = io_uring_setup(4, &p);
    EXPECT_TRUE(uringFd > 0);
    
    //flag is invalid -1,failed
    int ret = io_uring_enter(uringFd, 1, 1, -1);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_0800
 * @tc.name   : EnterSqeInvalidStatxFailed_0008
 * @tc.desc   : Enter was provide invalid statx
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEnterTest, EnterSqeInvalidStatxFailed_0008, Function | MediumTest | Level2)
{
    int ret;
    int res;
    struct io_uring_params p = {0};
    int fd = open(TEST_READ_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd > 0);

    int uringFd = io_uring_setup(16, &p);
    EXPECT_TRUE(uringFd > 0);
    
    //Map SQ and CQ rings
    size_t sqSize = p.sq_off.array + p.sq_entries * sizeof(unsigned);
    size_t cqSize = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);
    EXPECT_TRUE(cqSize > 0);

    void*sqPtr = mmap(NULL, sqSize, PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_SQ_RING);
    void*cqPtr = mmap(NULL, sqSize, PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_CQ_RING);

    //Map SQES
    struct io_uring_sqe *sqes = (struct io_uring_sqe *)mmap(NULL, p.sq_entries *sizeof(struct io_uring_sqe),
    PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_SQES);
    
    //Prepare data
    const char *writeData = "Hello,linked io_uring!";
    size_t dataLen = strlen(writeData);
    char *readBuf = (char*) malloc(dataLen + 1);
    res = memset_s(readBuf, dataLen + 1, 0, dataLen + 1);
    EXPECT_EQ(res, 0);

    //Get SQ ring pointers
    unsigned *sqHead = (unsigned *)((char*)sqPtr + p.sq_off.head);
    EXPECT_TRUE(sqHead != nullptr);
    unsigned *sqTail = (unsigned *)((char*)sqPtr + p.sq_off.tail);
    unsigned *sqRingMask = (unsigned *)((char*)sqPtr + p.sq_off.ring_mask);
    unsigned *sqArray = (unsigned *)((char*)sqPtr + p.sq_off.array);

    //Get CQ ring pointers
    unsigned *cqHead = (unsigned *)((char*)cqPtr + p.cq_off.head);
    unsigned *cqTail = (unsigned *)((char*)cqPtr + p.cq_off.tail);
    unsigned *cqRingMask = (unsigned *)((char*)cqPtr + p.cq_off.ring_mask);
    struct io_uring_cqe *cqes = (struct io_uring_cqe *)((char *)cqPtr + p.cq_off.cqes);

    //Prepare write SQE(first operation)
    unsigned sqIndex = *sqTail & *sqRingMask;
    struct io_uring_sqe *sqe = &sqes[sqIndex];
    res = memset_s(sqe, sizeof(*sqe), 0, sizeof(*sqe));
    EXPECT_EQ(res, 0);

    sqe->opcode = IORING_OP_STATX;
    sqe->fd = fd;
    sqe->addr = static_cast<unsigned long>(writeData);
    sqe->len = dataLen;
    sqe->user_data = 1;
    sqe->flags = IOSQE_ASYNC;

    //Add to submission queue
    sqArray[sqIndex] = sqIndex;

    //Update tal(submit 2 entries)
    *sqTail += 1;

    ret = io_uring_enter(uringFd, 1, 1, 0);
    EXPECT_EQ(ret, 1);

    //Process CQES
    while (*cqHead != *cqTail) {
        unsigned index = *cqHead & *cqRingMask;
        struct io_uring_cqe *cqe = &cqes[index];
        EXPECT_EQ(cqe->res, 0);
        (*cqHead)++;
    }

    free(readBuf);
    munmap(sqes, p.sq_entries * sizeof(struct io_uring_sqe));
    munmap(sqPtr, sqSize);
    munmap(cqPtr, cqSize);
    close(uringFd);
    close(fd);
    unlink(TEST_READ_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_0900
 * @tc.name   : EnterSqeIlleagelFlagStatxFailed_0009
 * @tc.desc   : Enter was provide illeagel statx
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEnterTest, EnterSqeIlleagelFlagStatxFailed_0009, Function | MediumTest | Level2)
{
    int ret;
    int res;
    struct io_uring_params p = {0};
    int fd = open(TEST_READ_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd > 0);

    int uringFd = io_uring_setup(16, &p);
    EXPECT_TRUE(uringFd > 0);
    
    //Map SQ and CQ rings
    size_t sqSize = p.sq_off.array + p.sq_entries * sizeof(unsigned);
    size_t cqSize = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);
    EXPECT_TRUE(cqSize > 0);

    void*sqPtr = mmap(NULL, sqSize, PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_SQ_RING);
    void*cqPtr = mmap(NULL, sqSize, PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_CQ_RING);

    //Map SQES
    struct io_uring_sqe *sqes = (struct io_uring_sqe *)mmap(NULL, p.sq_entries *sizeof(struct io_uring_sqe),
    PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_SQES);
    
    //Prepare data
    const char *writeData = "Hello,linked io_uring!";
    size_t dataLen = strlen(writeData);
    char *readBuf = (char*) malloc(dataLen + 1);
    res = memset_s(readBuf, dataLen + 1, 0, dataLen + 1);
    EXPECT_EQ(res, 0);

    //Get SQ ring pointers
    unsigned *sqHead = (unsigned *)((char*)sqPtr + p.sq_off.head);
    EXPECT_TRUE(sqHead != nullptr);
    unsigned *sqTail = (unsigned *)((char*)sqPtr + p.sq_off.tail);
    unsigned *sqRingMask = (unsigned *)((char*)sqPtr + p.sq_off.ring_mask);
    unsigned *sqArray = (unsigned *)((char*)sqPtr + p.sq_off.array);

    //Get CQ ring pointers
    unsigned *cqHead = (unsigned *)((char*)cqPtr + p.cq_off.head);
    unsigned *cqTail = (unsigned *)((char*)cqPtr + p.cq_off.tail);
    unsigned *cqRingMask = (unsigned *)((char*)cqPtr + p.cq_off.ring_mask);
    struct io_uring_cqe *cqes = (struct io_uring_cqe *)((char *)cqPtr + p.cq_off.cqes);

    //Prepare write SQE(first operation)
    unsigned sqIndex = *sqTail & *sqRingMask;
    struct io_uring_sqe *sqe = &sqes[sqIndex];
    res = memset_s(sqe, sizeof(*sqe), 0, sizeof(*sqe));
    EXPECT_EQ(res, 0);

    sqe->opcode = IORING_OP_STATX;
    sqe->fd = fd;
    sqe->addr = static_cast<unsigned long>(writeData);
    sqe->len = dataLen;
    sqe->user_data = 1;
    sqe->flags = -1;

    //Add to submission queue
    sqArray[sqIndex] = sqIndex;

    //Update tal(submit 2 entries)
    *sqTail += 1;

    ret = io_uring_enter(uringFd, 1, 1, 0);
    EXPECT_EQ(ret, 1);

    //Process CQES
    while (*cqHead != *cqTail) {
        unsigned index = *cqHead & *cqRingMask;
        struct io_uring_cqe *cqe = &cqes[index];
        EXPECT_EQ(cqe->res, -22);
        (*cqHead)++;
    }

    free(readBuf);
    munmap(sqes, p.sq_entries * sizeof(struct io_uring_sqe));
    munmap(sqPtr, sqSize);
    munmap(cqPtr, cqSize);
    close(uringFd);
    close(fd);
    unlink(TEST_READ_FILE);
}


/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_1000
 * @tc.name   : EnterSqeFlagSpliceFailed_0010
 * @tc.desc   : Enter was provide error flag splice
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEnterTest, EnterSqeFlagSpliceFailed_0010, Function | MediumTest | Level2)
{
    int ret;
    int res;
    struct io_uring_params p = {0};
    int fd = open(TEST_READ_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd > 0);

    int uringFd = io_uring_setup(16, &p);
    EXPECT_TRUE(uringFd > 0);
    
    //Map SQ and CQ rings
    size_t sqSize = p.sq_off.array + p.sq_entries * sizeof(unsigned);
    size_t cqSize = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);
    EXPECT_TRUE(cqSize > 0);

    void*sqPtr = mmap(NULL, sqSize, PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_SQ_RING);
    void*cqPtr = mmap(NULL, sqSize, PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_CQ_RING);

    //Map SQES
    struct io_uring_sqe *sqes = (struct io_uring_sqe *)mmap(NULL, p.sq_entries *sizeof(struct io_uring_sqe),
    PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_SQES);
    
    //Prepare data
    const char *writeData = "Hello,linked io_uring!";
    size_t dataLen = strlen(writeData);
    char *readBuf = (char*) malloc(dataLen + 1);
    res = memset_s(readBuf, dataLen + 1, 0, dataLen + 1);
    EXPECT_EQ(res, 0);

    //Get SQ ring pointers
    unsigned *sqHead = (unsigned *)((char*)sqPtr + p.sq_off.head);
    EXPECT_TRUE(sqHead != nullptr);
    unsigned *sqTail = (unsigned *)((char*)sqPtr + p.sq_off.tail);
    unsigned *sqRingMask = (unsigned *)((char*)sqPtr + p.sq_off.ring_mask);
    unsigned *sqArray = (unsigned *)((char*)sqPtr + p.sq_off.array);

    //Get CQ ring pointers
    unsigned *cqHead = (unsigned *)((char*)cqPtr + p.cq_off.head);
    unsigned *cqTail = (unsigned *)((char*)cqPtr + p.cq_off.tail);
    unsigned *cqRingMask = (unsigned *)((char*)cqPtr + p.cq_off.ring_mask);
    struct io_uring_cqe *cqes = (struct io_uring_cqe *)((char *)cqPtr + p.cq_off.cqes);

    //Prepare write SQE(first operation)
    unsigned sqIndex = *sqTail & *sqRingMask;
    struct io_uring_sqe *sqe = &sqes[sqIndex];
    res = memset_s(sqe, sizeof(*sqe), 0, sizeof(*sqe));
    EXPECT_EQ(res, 0);

    int pipeFd[2];
    ret = pipe(pipeFd);
    EXPECT_EQ(ret, 0);

    sqe->fd = pipeFd[1];
    sqe->off = -1;
    sqe->splice_off_in = 0;
    sqe->len = dataLen;
    sqe->opcode = IORING_OP_SPLICE;
    sqe->flags = 0xff;
    sqe->splice_fd_in = 0;

    //Add to submission queue
    sqArray[sqIndex] = sqIndex;

    //Update tal(submit 2 entries)
    *sqTail += 1;

    ret = io_uring_enter(uringFd, 1, 1, 0);
    EXPECT_EQ(ret, 1);

    //Process CQES
    while (*cqHead != *cqTail) {
        unsigned index = *cqHead & *cqRingMask;
        struct io_uring_cqe *cqe = &cqes[index];
        EXPECT_EQ(cqe->res, -22);
        (*cqHead)++;
    }

    free(readBuf);
    munmap(sqes, p.sq_entries * sizeof(struct io_uring_sqe));
    munmap(sqPtr, sqSize);
    munmap(cqPtr, cqSize);
    close(uringFd);
    close(fd);
    unlink(TEST_READ_FILE);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_ENTER_1100
 * @tc.name   : EnterSqeFlagIOSQE_IO_LINK_0011
 * @tc.desc   : Enter was provide SQE flag is IOSQE_IO_LINK
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(HatsEnterTest, EnterSqeFlagIOSQE_IO_LINK_0011, Function | MediumTest | Level2)
{
    int ret;
    int res;
    struct io_uring_params p = {0};
    int fd = open(TEST_READ_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    EXPECT_TRUE(fd > 0);

    int uringFd = io_uring_setup(4, &p);
    EXPECT_TRUE(uringFd > 0);
    
    //Map SQ and CQ rings
    size_t sqSize = p.sq_off.array + p.sq_entries * sizeof(unsigned);
    size_t cqSize = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);
    EXPECT_TRUE(cqSize > 0);

    void*sqPtr = mmap(NULL, sqSize, PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_SQ_RING);
    void*cqPtr = mmap(NULL, sqSize, PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_CQ_RING);

    //Map SQES
    struct io_uring_sqe *sqes = (struct io_uring_sqe *)mmap(NULL, p.sq_entries *sizeof(struct io_uring_sqe),
    PROT_READ | PROT_WRITE, MAP_SHARED, uringFd, IORING_OFF_SQES);
    
    //Prepare data
    const char *writeData = "Hello,linked io_uring!";
    size_t dataLen = strlen(writeData);
    char *readBuf = (char*) malloc(dataLen + 1);
    res = memset_s(readBuf, dataLen + 1, 0, dataLen + 1);
    EXPECT_EQ(res, 0);

    //Get SQ ring pointers
    unsigned *sqHead = (unsigned *)((char*)sqPtr + p.sq_off.head);
    EXPECT_TRUE(sqHead != nullptr);
    unsigned *sqTail = (unsigned *)((char*)sqPtr + p.sq_off.tail);
    unsigned *sqRingMask = (unsigned *)((char*)sqPtr + p.sq_off.ring_mask);
    unsigned *sqArray = (unsigned *)((char*)sqPtr + p.sq_off.array);

    //Get CQ ring pointers
    unsigned *cqHead = (unsigned *)((char*)cqPtr + p.cq_off.head);
    unsigned *cqTail = (unsigned *)((char*)cqPtr + p.cq_off.tail);
    unsigned *cqRingMask = (unsigned *)((char*)cqPtr + p.cq_off.ring_mask);
    struct io_uring_cqe *cqes = (struct io_uring_cqe *)((char *)cqPtr + p.cq_off.cqes);

    //Prepare write SQE(first operation)
    unsigned sqIndex = *sqTail & *sqRingMask;
    struct io_uring_sqe *writeSqe = &sqes[sqIndex];
    res = memset_s(writeSqe, sizeof(*writeSqe), 0, sizeof(*writeSqe));
    EXPECT_EQ(res, 0);

    writeSqe->opcode = IORING_OP_WRITE;
    writeSqe->fd = fd;
    writeSqe->addr = static_cast<unsigned long>(writeData);
    writeSqe->len = dataLen;
    writeSqe->user_data = 1;
    writeSqe->flags = IOSQE_IO_LINK;

    //Prepare read SQE(second operation)
    unsigned nextSqIndex = (*sqTail + 1) & *sqRingMask;
    struct io_uring_sqe *readSqe = &sqes[nextSqIndex];
    res = memset_s(read_sqe, sizeof(*readSqe), 0, sizeof(*readSqe));
    EXPECT_EQ(res, 0);

    readSqe->opcode = IORING_OP_READ;
    readSqe->fd = fd;
    readSqe->addr = static_cast<unsigned long>(readBuf);
    readSqe->len = dataLen;
    readSqe->user_data = 2;
    readSqe->off = 0;

    //Add to submission queue
    sqArray[sqIndex] = sqIndex;
    sqArray[nextSqIndex] = nextSqIndex;

    //Update tal(submit 2 entries)
    *sqTail += 2;

    ret = io_uring_enter(uringFd, 2, 2, 0);
    EXPECT_EQ(ret, 2);

    int attempts = 0;
    while (*cqHead == *cqTail && attempts++ < 10) {
        usleep(1000);
    }

    //Process CQES
    while (*cqHead != *cqTail) {
        unsigned index = *cqHead & *cqRingMask;
        struct io_uring_cqe *cqe = &cqes[index];
        EXPECT_EQ(cqe->res, 22);
        (*cqHead)++;
    }

    free(readBuf);
    munmap(sqes, p.sq_entries * sizeof(struct io_uring_sqe));
    munmap(sqPtr, sqSize);
    munmap(cqPtr, cqSize);
    close(uringFd);
    close(fd);
    unlink(TEST_READ_FILE);
}