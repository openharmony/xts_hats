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
    int uring_fd = io_uring_setup(4, &p);
    EXPECT_TRUE(uring_fd > 0);

    int ret = io_uring_enter(uring_fd, 1, 1, 0);
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
    int uring_fd = io_uring_setup(4, &p);
    EXPECT_TRUE(uring_fd > 0);

    int ret = io_uring_enter(uring_fd, 1, 1, IORING_ENTER_GETEVENTS);
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
    int uring_fd = io_uring_setup(4, &p);
    EXPECT_TRUE(uring_fd > 0);

    int ret = io_uring_enter(uring_fd, 0, 0, IORING_ENTER_GETEVENTS);
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
    int uring_fd = io_uring_setup(4, &p);
    EXPECT_TRUE(uring_fd > 0);
    
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
    int uring_fd = io_uring_setup(4, &p);
    EXPECT_TRUE(uring_fd > 0);
    
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
    int uring_fd = io_uring_setup(4, &p);
    EXPECT_TRUE(uring_fd > 0);
    
    //fd is not normal fd,failed
    int ret = io_uring_enter(uring_fd + 10, 1, 1, 0);
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
    int uring_fd = io_uring_setup(4, &p);
    EXPECT_TRUE(uring_fd > 0);
    
    //flag is invalid -1,failed
    int ret = io_uring_enter(uring_fd, 1, 1, -1);
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

    int uring_fd = io_uring_setup(16, &p);
    EXPECT_TRUE(uring_fd > 0);
    
    //Map SQ and CQ rings
    size_t sq_size = p.sq_off.array + p.sq_entries * sizeof(unsigned);
    size_t cq_size = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);
    EXPECT_TRUE(cq_size > 0);

    void*sq_ptr = mmap(NULL, sq_size, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQ_RING);
    void*cq_ptr = mmap(NULL, sq_size, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_CQ_RING);

    //Map SQES
    struct io_uring_sqe *sqes = (struct io_uring_sqe *)mmap(NULL, p.sq_entries *sizeof(struct io_uring_sqe),  PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQES);
    
    //Prepare data
    const char *write_data = "Hello,linked io_uring!";
    size_t data_len = strlen(write_data);
    char *read_buf = (char*) malloc(data_len +1);
    res = memset_s(read_buf, data_len +1, 0, data_len +1);
    EXPECT_EQ(res, 0);

    //Get SQ ring pointers
    unsigned *sq_head = (unsigned *)((char*)sq_ptr + p.sq_off.head);
    EXPECT_TRUE(sq_head != nullptr);
    unsigned *sq_tail = (unsigned *)((char*)sq_ptr + p.sq_off.tail);
    unsigned *sq_ring_mask = (unsigned *)((char*)sq_ptr + p.sq_off.ring_mask);
    unsigned *sq_array = (unsigned *)((char*)sq_ptr + p.sq_off.array);

    //Get CQ ring pointers
    unsigned *cq_head = (unsigned *)((char*)cq_ptr + p.cq_off.head);
    unsigned *cq_tail = (unsigned *)((char*)cq_ptr + p.cq_off.tail);
    unsigned *cq_ring_mask = (unsigned *)((char*)cq_ptr + p.cq_off.ring_mask);
    struct io_uring_cqe *cqes = (struct io_uring_cqe *)((char *)cq_ptr + p.cq_off.cqes);

    //Prepare write SQE(first operation)
    unsigned sq_index = *sq_tail & *sq_ring_mask;
    struct io_uring_sqe *sqe = &sqes[sq_index];
    res = memset_s(sqe, sizeof(*sqe), 0, sizeof(*sqe));
    EXPECT_EQ(res, 0);

    sqe->opcode = IORING_OP_STATX;
    sqe->fd = fd;
    sqe->addr = (unsigned long)write_data;
    sqe->len = data_len;
    sqe->user_data = 1;
    sqe->flags = IOSQE_ASYNC;

    //Add to submission queue
    sq_array[sq_index] = sq_index;

    //Update tal(submit 2 entries)
    *sq_tail += 1;

    ret = io_uring_enter(uring_fd, 1, 1, 0);
    EXPECT_EQ(ret, 1);

    //Process CQES
    while(*cq_head != *cq_tail) {
        unsigned index = *cq_head &*cq_ring_mask;
        struct io_uring_cqe *cqe = &cqes[index];
        EXPECT_EQ(cqe->res, 0);
        (*cq_head)++;
    }

    free(read_buf);
    munmap(sqes, p.sq_entries * sizeof(struct io_uring_sqe));
    munmap(sq_ptr, sq_size);
    munmap(cq_ptr, cq_size);
    close(uring_fd);
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

    int uring_fd = io_uring_setup(16, &p);
    EXPECT_TRUE(uring_fd > 0);
    
    //Map SQ and CQ rings
    size_t sq_size = p.sq_off.array + p.sq_entries * sizeof(unsigned);
    size_t cq_size = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);
    EXPECT_TRUE(cq_size > 0);

    void*sq_ptr = mmap(NULL, sq_size, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQ_RING);
    void*cq_ptr = mmap(NULL, sq_size, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_CQ_RING);

    //Map SQES
    struct io_uring_sqe *sqes = (struct io_uring_sqe *)mmap(NULL, p.sq_entries *sizeof(struct io_uring_sqe),  PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQES);
    
    //Prepare data
    const char *write_data = "Hello,linked io_uring!";
    size_t data_len = strlen(write_data);
    char *read_buf = (char*) malloc(data_len +1);
    res = memset_s(read_buf, data_len +1, 0, data_len +1);
    EXPECT_EQ(res, 0);

    //Get SQ ring pointers
    unsigned *sq_head = (unsigned *)((char*)sq_ptr + p.sq_off.head);
    EXPECT_TRUE(sq_head != nullptr);
    unsigned *sq_tail = (unsigned *)((char*)sq_ptr + p.sq_off.tail);
    unsigned *sq_ring_mask = (unsigned *)((char*)sq_ptr + p.sq_off.ring_mask);
    unsigned *sq_array = (unsigned *)((char*)sq_ptr + p.sq_off.array);

    //Get CQ ring pointers
    unsigned *cq_head = (unsigned *)((char*)cq_ptr + p.cq_off.head);
    unsigned *cq_tail = (unsigned *)((char*)cq_ptr + p.cq_off.tail);
    unsigned *cq_ring_mask = (unsigned *)((char*)cq_ptr + p.cq_off.ring_mask);
    struct io_uring_cqe *cqes = (struct io_uring_cqe *)((char *)cq_ptr + p.cq_off.cqes);

    //Prepare write SQE(first operation)
    unsigned sq_index = *sq_tail & *sq_ring_mask;
    struct io_uring_sqe *sqe = &sqes[sq_index];
    res = memset_s(sqe, sizeof(*sqe), 0, sizeof(*sqe));
    EXPECT_EQ(res, 0);

    sqe->opcode = IORING_OP_STATX;
    sqe->fd = fd;
    sqe->addr = (unsigned long)write_data;
    sqe->len = data_len;
    sqe->user_data = 1;
    sqe->flags = -1;

    //Add to submission queue
    sq_array[sq_index] = sq_index;

    //Update tal(submit 2 entries)
    *sq_tail += 1;

    ret = io_uring_enter(uring_fd, 1, 1, 0);
    EXPECT_EQ(ret, 1);

    //Process CQES
    while(*cq_head != *cq_tail) {
        unsigned index = *cq_head &*cq_ring_mask;
        struct io_uring_cqe *cqe = &cqes[index];
        EXPECT_EQ(cqe->res, -22);
        (*cq_head)++;
    }

    free(read_buf);
    munmap(sqes, p.sq_entries * sizeof(struct io_uring_sqe));
    munmap(sq_ptr, sq_size);
    munmap(cq_ptr, cq_size);
    close(uring_fd);
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

    int uring_fd = io_uring_setup(16, &p);
    EXPECT_TRUE(uring_fd > 0);
    
    //Map SQ and CQ rings
    size_t sq_size = p.sq_off.array + p.sq_entries * sizeof(unsigned);
    size_t cq_size = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);
    EXPECT_TRUE(cq_size > 0);

    void*sq_ptr = mmap(NULL, sq_size, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQ_RING);
    void*cq_ptr = mmap(NULL, sq_size, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_CQ_RING);

    //Map SQES
    struct io_uring_sqe *sqes = (struct io_uring_sqe *)mmap(NULL, p.sq_entries *sizeof(struct io_uring_sqe),  PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQES);
    
    //Prepare data
    const char *write_data = "Hello,linked io_uring!";
    size_t data_len = strlen(write_data);
    char *read_buf = (char*) malloc(data_len +1);
    res = memset_s(read_buf, data_len +1, 0, data_len +1);
    EXPECT_EQ(res, 0);

    //Get SQ ring pointers
    unsigned *sq_head = (unsigned *)((char*)sq_ptr + p.sq_off.head);
    EXPECT_TRUE(sq_head != nullptr);
    unsigned *sq_tail = (unsigned *)((char*)sq_ptr + p.sq_off.tail);
    unsigned *sq_ring_mask = (unsigned *)((char*)sq_ptr + p.sq_off.ring_mask);
    unsigned *sq_array = (unsigned *)((char*)sq_ptr + p.sq_off.array);

    //Get CQ ring pointers
    unsigned *cq_head = (unsigned *)((char*)cq_ptr + p.cq_off.head);
    unsigned *cq_tail = (unsigned *)((char*)cq_ptr + p.cq_off.tail);
    unsigned *cq_ring_mask = (unsigned *)((char*)cq_ptr + p.cq_off.ring_mask);
    struct io_uring_cqe *cqes = (struct io_uring_cqe *)((char *)cq_ptr + p.cq_off.cqes);

    //Prepare write SQE(first operation)
    unsigned sq_index = *sq_tail & *sq_ring_mask;
    struct io_uring_sqe *sqe = &sqes[sq_index];
    res = memset_s(sqe, sizeof(*sqe), 0, sizeof(*sqe));
    EXPECT_EQ(res, 0);

    int pipeFd[2];
    ret = pipe(pipeFd);
    EXPECT_EQ(ret, 0);

    sqe->fd = pipeFd[1];
    sqe->off = -1;
    sqe->splice_off_in = 0;
    sqe->len = data_len;
    sqe->opcode = IORING_OP_SPLICE;
    sqe->flags = 0xff;
    sqe->splice_fd_in = 0;

    //Add to submission queue
    sq_array[sq_index] = sq_index;

    //Update tal(submit 2 entries)
    *sq_tail += 1;

    ret = io_uring_enter(uring_fd, 1, 1, 0);
    EXPECT_EQ(ret, 1);

    //Process CQES
    while(*cq_head != *cq_tail) {
        unsigned index = *cq_head &*cq_ring_mask;
        struct io_uring_cqe *cqe = &cqes[index];
        EXPECT_EQ(cqe->res, -22);
        (*cq_head)++;
    }

    free(read_buf);
    munmap(sqes, p.sq_entries * sizeof(struct io_uring_sqe));
    munmap(sq_ptr, sq_size);
    munmap(cq_ptr, cq_size);
    close(uring_fd);
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

    int uring_fd = io_uring_setup(4, &p);
    EXPECT_TRUE(uring_fd > 0);
    
    //Map SQ and CQ rings
    size_t sq_size = p.sq_off.array + p.sq_entries * sizeof(unsigned);
    size_t cq_size = p.cq_off.cqes + p.cq_entries * sizeof(struct io_uring_cqe);
    EXPECT_TRUE(cq_size > 0);

    void*sq_ptr = mmap(NULL, sq_size, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQ_RING);
    void*cq_ptr = mmap(NULL, sq_size, PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_CQ_RING);

    //Map SQES
    struct io_uring_sqe *sqes = (struct io_uring_sqe *)mmap(NULL, p.sq_entries *sizeof(struct io_uring_sqe),  PROT_READ | PROT_WRITE, MAP_SHARED, uring_fd, IORING_OFF_SQES);
    
    //Prepare data
    const char *write_data = "Hello,linked io_uring!";
    size_t data_len = strlen(write_data);
    char *read_buf = (char*) malloc(data_len +1);
    res = memset_s(read_buf, data_len +1, 0, data_len +1);
    EXPECT_EQ(res, 0);

    //Get SQ ring pointers
    unsigned *sq_head = (unsigned *)((char*)sq_ptr + p.sq_off.head);
    EXPECT_TRUE(sq_head != nullptr);
    unsigned *sq_tail = (unsigned *)((char*)sq_ptr + p.sq_off.tail);
    unsigned *sq_ring_mask = (unsigned *)((char*)sq_ptr + p.sq_off.ring_mask);
    unsigned *sq_array = (unsigned *)((char*)sq_ptr + p.sq_off.array);

    //Get CQ ring pointers
    unsigned *cq_head = (unsigned *)((char*)cq_ptr + p.cq_off.head);
    unsigned *cq_tail = (unsigned *)((char*)cq_ptr + p.cq_off.tail);
    unsigned *cq_ring_mask = (unsigned *)((char*)cq_ptr + p.cq_off.ring_mask);
    struct io_uring_cqe *cqes = (struct io_uring_cqe *)((char *)cq_ptr + p.cq_off.cqes);

    //Prepare write SQE(first operation)
    unsigned sq_index = *sq_tail & *sq_ring_mask;
    struct io_uring_sqe *write_sqe = &sqes[sq_index];
    res = memset_s(write_sqe, sizeof(*write_sqe), 0, sizeof(*write_sqe));
    EXPECT_EQ(res, 0);

    write_sqe->opcode = IORING_OP_WRITE;
    write_sqe->fd = fd;
    write_sqe->addr = (unsigned long)write_data;
    write_sqe->len = data_len;
    write_sqe->user_data = 1;
    write_sqe->flags = IOSQE_IO_LINK;

    //Prepare read SQE(second operation)
    unsigned next_sq_index = (*sq_tail + 1) & *sq_ring_mask;
    struct io_uring_sqe *read_sqe = &sqes[sq_index];
    res = memset_s(read_sqe, sizeof(*read_sqe), 0, sizeof(*read_sqe));
    EXPECT_EQ(res, 0);

    read_sqe->opcode = IORING_OP_READ;
    read_sqe->fd = fd;
    read_sqe->addr = (unsigned long)read_buf;
    read_sqe->len = data_len;
    read_sqe->user_data = 2;
    read_sqe->off = 0;

    //Add to submission queue
    sq_array[sq_index] = sq_index;
    sq_array[next_sq_index] = next_sq_index;

    //Update tal(submit 2 entries)
    *sq_tail += 2;

    ret = io_uring_enter(uring_fd, 2, 2, 0);
    EXPECT_EQ(ret, 2);

    int attempts = 0;
    while (*cq_head == *cq_tail && attempts++ < 10) {
        usleep(1000);
    }

    //Process CQES
    while(*cq_head != *cq_tail) {
        unsigned index = *cq_head &*cq_ring_mask;
        struct io_uring_cqe *cqe = &cqes[index];
        EXPECT_EQ(cqe->res, 22);
        (*cq_head)++;
    }

    free(read_buf);
    munmap(sqes, p.sq_entries * sizeof(struct io_uring_sqe));
    munmap(sq_ptr, sq_size);
    munmap(cq_ptr, cq_size);
    close(uring_fd);
    close(fd);
    unlink(TEST_READ_FILE);
}