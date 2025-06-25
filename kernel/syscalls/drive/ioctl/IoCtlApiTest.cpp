/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include "securec.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include <iterator>
#include <linux/fs.h>
#include <ostream>
#include <string>
#include <vector>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/fsuid.h>
#include <sys/stat.h>
#include <gtest/gtest.h>
#include <linux/if.h>

using namespace testing::ext;

class IoCtlTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void IoCtlTest::SetUp()
{
}

void IoCtlTest::TearDown()
{
}

void IoCtlTest::SetUpTestCase()
{
}

void IoCtlTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_IOCTL_0100
 * @tc.name   : IoctlValidFdSuccess_0001
 * @tc.desc   : Ioctl invalid fd failed.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(IoCtlTest, IoctlValidFdSuccess_0001, Function | MediumTest | Level1)
{
    errno = 0;
    int fd = open("/dev/null", O_RDONLY);
    ASSERT_NE(fd, -1);
    int result = ioctl(fd, 0xffff, 0);
    close(fd);
    EXPECT_TRUE(result == -1);
    EXPECT_EQ(errno, ENOTTY);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_IOCTL_0200
 * @tc.name   : IoctlSiocgifconfSuccess_0002
 * @tc.desc   : Ioctl config SIOCGIFCONF successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(IoCtlTest, IoctlSiocgifconfSuccess_0002, Function | MediumTest | Level1)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifconf ifconfig;
    int ret = ioctl(sockfd,  SIOCGIFCONF, &ifconfig);
    close(sockfd);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_IOCTL_0300
 * @tc.name   : IoctlSiocgifflagsSuccess_0003
 * @tc.desc   : Ioctl config SIOCGIFFLAGS successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(IoCtlTest, IoctlSiocgifflagsSuccess_0003, Function | MediumTest | Level1)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    memset_s(&ifr, sizeof(ifr), 0, sizeof(ifr));
    const char *strValue = "wlan0";
    memcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), strValue, strlen(strValue));
    int ret = ioctl(sockfd,  SIOCGIFFLAGS, &ifr);
    close(sockfd);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_IOCTL_0400
 * @tc.name   : IoctlSiocgifindexSuccess_0004
 * @tc.desc   : Ioctl config SIOCGIFINDEX successfully.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(IoCtlTest, IoctlSiocgifindexSuccess_0004, Function | MediumTest | Level1)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    memset_s(&ifr, sizeof(ifr), 0, sizeof(ifr));
    const char *strValue = "wlan0";
    memcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), strValue, strlen(strValue));
    int ret = ioctl(sockfd,  SIOCGIFINDEX, &ifr);
    close(sockfd);
    EXPECT_EQ(ret, 0);
}
