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

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "securec.h"

using namespace testing::ext;
using namespace std;

class FtruncateApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void FtruncateApiTest::SetUp()
{
}
void FtruncateApiTest::TearDown()
{
}
void FtruncateApiTest::SetUpTestCase()
{
}
void FtruncateApiTest::TearDownTestCase()
{
}

static const char* TEST_FILE = "/data/local/tmp/test.txt";

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FTRUNCATE_0100
 * @tc.name   : FtruncateModifyFileSizeSuccess_0001
 * @tc.desc   : modify file size to 50 success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(FtruncateApiTest, FtruncateModifyFileSizeSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    int fd = -1;
    off_t len = 50;
    struct stat stat;

    fd = open(TEST_FILE, O_WRONLY | O_CREAT, 0644);
    EXPECT_TRUE(fd > 0);

    ret = ftruncate(fd, len);
    EXPECT_EQ(ret, 0);
    ret = fstat(fd, &stat);
    EXPECT_EQ(ret, 0);

    // Check whether the file size is len
    EXPECT_EQ(stat.st_size, len);

    close(fd);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_FTRUNCATE_0200
 * @tc.name   : FtruncateUseInvalidFdFailed_0002
 * @tc.desc   : ftruncate modify illegal fd size failed, errno EBADF.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(FtruncateApiTest, FtruncateUseInvalidFdFailed_0002, Function | MediumTest | Level2)
{
    int ret;
    off_t len = 20;
    errno = 0;
    ret = ftruncate(-1, len);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EBADF);
}