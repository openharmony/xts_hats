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

#include  <cstdio>
#include  <cstdlib>
#include  <fcntl.h>
#include  <string>
#include  <unistd.h>
#include  <vector>
#include  <gtest/gtest.h>
#include  <sys/stat.h>
#include  <sys/types.h>
#include  <sys/xattr.h>
#include  "securec.h"
#include  <linux/io_uring.h>
#include  <sys/mman.h>
#include  <sys/syscall.h>  

static int io_uring_setup(unsigned entries,struct io_uring_params *p){
        return syscall(__NR_io_uring_setup,entries,p);
}


using namespace testing::ext;

static const char  *TEST_VALID_PATH = "/data/local/tmp/attr_test_dir";
static const char  *TEST_VALID_FILE = "/data/local/tmp/attr_test_dir/attr_test.txt";


class HatsSetupTest:public testing::Test{
public:
static void SetUpTestCase();
static void TearDownTestCase();
void SetUp();
void TearDown();
private:

};

void HatsSetupTest::SetUp()
{
int fd = -1
if(access(TEST_VALID_PATH,F_OK) == 0)
{
    (void)remove(TEST_VALID_PATH);
}
(void)mkdir(TEST_VALID_PATH,S_IWUSR | S_IRUSR | S_IXUSR);
fd = open(TEST_VALID_FILE,O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
close(fd);

}

void HatsSetupTest::TearDown()
{
(void)remove(   TEST_VALID_FILE);
if(access(TEST_VALID_PATH,F_OK) == 0){
    (void)remove(TEST_VALID_PATH);
}
}

void HatsSetupTest::SetUpTestCase()
{
    
}

void HatsSetupTest::TearDownTestCase()
{
    
}


/**
* @tc.number : SUB_BASIC_FM_USE_IOURING_SETUP_0100
* @tc.name   : SetupApiTestSuccess_0001
* @tc.desc   : RdbStore backup and restore callback test
* @tc.size   : MediumTest
* @tc.type   : Function
* @tc.level  : Level 1
*/

HWTEST_F(HatsSetupTest,SetupApiTestSuccess_0001,Function | MediumTest | Level)
{

    static io_uring_params p = {0};

    int uring_fd01 = io_uring_setup(1,&p);
    EXPECT_TRUE(uring_fd01 > 0);

    int uring_fd02 = io_uring_setup(2,&p);
    EXPECT_TRUE(uring_fd02 > 0);

    int uring_fd03 = io_uring_setup(8,&p);
    EXPECT_TRUE(uring_fd03 > 0);

    int uring_fd04 = io_uring_setup(32767,&p);
    EXPECT_TRUE(uring_fd04 > 0);

    int uring_fd05 = io_uring_setup(32768,&p);
    EXPECT_TRUE(uring_fd05 > 0);

}



/**
* @tc.number : SUB_BASIC_FM_USE_IOURING_SETUP_0200
* @tc.name   : SetupApiTest_params_null_0002
* @tc.desc   : RdbStore backup and restore callback test
* @tc.size   : MediumTest
* @tc.type   : Function
* @tc.level  : Level 1
*/

HWTEST_F(HatsSetupTest,SetupApiTest_params_null_0002,Function | MediumTest | Level)
{

    int uring_fd01 = io_uring_setup(1,NULL);
    EXPECT_EQ(errno, 14);

}


/**
* @tc.number : SUB_BASIC_FM_USE_IOURING_SETUP_0300
* @tc.name   : SetupApiTestFail_0003
* @tc.desc   : RdbStore backup and restore callback test
* @tc.size   : MediumTest
* @tc.type   : Function
* @tc.level  : Level 1
*/

HWTEST_F(HatsSetupTest,SetupApiTestFail_0003,Function | MediumTest | Level)
{

     static io_uring_params p = {0};

    int uring_fd01 = io_uring_setup(-1, &p);
    EXPECT_EQ(uring_fd01, -1);
    EXPECT_EQ(errno, 22);

    int uring_fd02 = io_uring_setup(0, &p);
    EXPECT_EQ(uring_fd02, -1);
    EXPECT_EQ(errno, 22);

    int uring_fd03 = io_uring_setup(32769, &p);
    EXPECT_EQ(uring_fd03, -1);
    EXPECT_EQ(errno, 22);

}

/**
* @tc.number : SUB_BASIC_FM_USE_IOURING_SETUP_0400
* @tc.name   : SetupApiTest_params_int_0004
* @tc.desc   : RdbStore backup and restore callback test
* @tc.size   : MediumTest
* @tc.type   : Function
* @tc.level  : Level 1
*/

HWTEST_F(HatsSetupTest,SetupApiTest_params_int_0004,Function | MediumTest | Level)
{

     static io_uring_params params;
     memset(&params, 0, sizeof(int));

    int uring_fd01 = io_uring_setup(8, &params);
    EXPECT_EQ(errno, 2);

}