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

#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace testing::ext;
using namespace std;

class Pread64Test : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

private:
};

static const char* TEST_DATA = "Hello, world!";
static const size_t TEST_LEN = strlen(TEST_DATA);
static const char* TEST_FILE = "/data/local/tmp/test_file.txt";

void Pread64Test::SetUpTestCase()
{
}

void Pread64Test::TearDownTestCase()
{
}

void Pread64Test::SetUp()
{
    int fd = -1;
    if (access(TEST_FILE, F_OK) == 0) {
        remove(TEST_FILE);
    }

    fd = open(TEST_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    write(fd, TEST_DATA, strlen(TEST_DATA));
    close(fd);
}

void Pread64Test::TearDown()
{
    (void)remove(TEST_FILE);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_PREAD64_0100
 * @tc.name Pread64ReadSuccess_0001
 * @tc.desc Test basic read functionality of pread64.
*/
HWTEST_F(Pread64Test, Pread64ReadSuccess_0001, Function | MediumTest | Level1)
{
    ssize_t size;
    int midLen = TEST_LEN / 2;
    char buf[20] = {0};
    int fd = open(TEST_FILE, O_RDONLY);

    // pread64 from file start
    size = pread64(fd, buf, TEST_LEN, 0);
    EXPECT_EQ(size, TEST_LEN);
    EXPECT_STREQ(buf, TEST_DATA);

    // pread64 from file middle
    std::fill_n(buf, sizeof(buf), 0);
    size = pread64(fd, buf, TEST_LEN, midLen);
    EXPECT_EQ(size, TEST_LEN - midLen);
    EXPECT_STREQ(buf, &TEST_DATA[midLen]);

    // pread64 from file end
    std::fill_n(buf, sizeof(buf), 0);
    size = pread64(fd, buf, TEST_LEN, TEST_LEN);
    EXPECT_EQ(size, 0);

    close(fd);
}

/*
 * @tc.number SUB_KERNEL_SYSCALL_PREAD64_0200
 * @tc.name Pread64InvalidFdFailed_0002
 * @tc.desc Test using an invalid file descriptor, errno EBADF.
*/
HWTEST_F(Pread64Test, Pread64InvalidFdFailed_0002, Function | MediumTest | Level2)
{
    char buf[20] = {0};
    ssize_t size;
    errno = 0;
    size = pread64(-1, buf, TEST_LEN, 0);
    EXPECT_EQ(size, -1);
    EXPECT_EQ(errno, EBADF);
}
