/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <mutex>
#include <fcntl.h>
#include <functional>
#include <securec.h>
#include <unistd.h>

#include "hdf_base.h"
#include "osal_time.h"
#include "v1_0/ipower_interface.h"
#include "v1_0/ipower_hdi_callback.h"
#include "v1_0/power_types.h"

using namespace OHOS::HDI;
using namespace OHOS::HDI::Power::V1_0;
using namespace testing::ext;

namespace {
class PowerHdiCallback : public IPowerHdiCallback {
public:
    ~PowerHdiCallback() override {};
    int32_t OnSuspend() override
    {
        return 0;
    };

    int32_t OnWakeup() override
    {
        return 0;
    };
};

sptr<IPowerHdiCallback> g_callback = new PowerHdiCallback();
sptr<IPowerInterface> g_powerInterface = nullptr;
std::mutex g_mutex;
const uint32_t MAX_PATH = 256;
const uint32_t WAIT_TIME = 1;
const std::string SUSPEND_STATE = "mem";
const std::string SUSPEND_STATE_PATH = "/sys/power/state";
const std::string LOCK_PATH = "/sys/power/wake_lock";
const std::string UNLOCK_PATH = "/sys/power/wake_unlock";
const std::string WAKEUP_COUNT_PATH = "/sys/power/wakeup_count";

class HdfPowerHdiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    static int32_t ReadFile(const char *path, char *buf, size_t size);
};

void HdfPowerHdiTest::SetUpTestCase()
{
    g_powerInterface = IPowerInterface::Get(true);
}

void HdfPowerHdiTest::TearDownTestCase()
{
}

void HdfPowerHdiTest::SetUp()
{
}

void HdfPowerHdiTest::TearDown()
{
}

int32_t HdfPowerHdiTest::ReadFile(const char *path, char *buf, size_t size)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    int32_t ret;

    int32_t fd = open(path, O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH);
    if (fd < HDF_SUCCESS) {
        return HDF_FAILURE;
    }

    ret = read(fd, buf, size);
    if (ret < HDF_SUCCESS) {
        close(fd);
        return HDF_FAILURE;
    }

    close(fd);
    buf[size - 1] = '\0';
    return HDF_SUCCESS;
}
}

namespace {
/**
  * @tc.name: HdfPowerHdiTest001
  * @tc.desc: Get a client and check whether the client is empty.
  * @tc.type: FUNC
  */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest001, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_powerInterface);
}

/**
  * @tc.name: HdfPowerHdiTest002
  * @tc.desc: RegisterCallback
  * @tc.type: FUNC
  */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest002, TestSize.Level1)
{
    if (false) {
        EXPECT_TRUE(HDF_SUCCESS ==
            g_powerInterface->RegisterCallback(g_callback))<< "HdfPowerHdiTest002 failed";
    }
}

/**
  * @tc.name: HdfPowerHdiTest003
  * @tc.desc: check startsuspend
  * @tc.type: FUNC
  */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest003, TestSize.Level1)
{
    if (false) {
      int32_t ret = g_powerInterface->StartSuspend();
      EXPECT_EQ(0, ret);
  
      char stateBuf[MAX_PATH] = {0};
      char stateValue[MAX_PATH] = {0};
  
      ret = snprintf_s(stateBuf, MAX_PATH, sizeof(stateBuf) - 1, SUSPEND_STATE_PATH.c_str());
      EXPECT_FALSE(ret < EOK);
      sleep(WAIT_TIME);
      ret = HdfPowerHdiTest::ReadFile(stateBuf, stateValue, sizeof(stateValue));
      EXPECT_EQ(0, ret);
      std::string state = stateValue;
      EXPECT_FALSE(state.empty());
      auto pos = state.find(SUSPEND_STATE);
      EXPECT_TRUE(pos != std::string::npos) << "HdfPowerHdiTest003 failed state: " << state;
    }
}

/**
  * @tc.name: HdfPowerHdiTest004
  * @tc.desc: check StopSuspend
  * @tc.type: FUNC
  */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest004, TestSize.Level1)
{
    int32_t ret = g_powerInterface->StopSuspend();
    EXPECT_EQ(0, ret) << "HdfPowerHdiTest004 failed";
}

/**
  * @tc.name: HdfPowerHdiTest005
  * @tc.desc: Force Suspend
  * @tc.type: FUNC
  */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest005, TestSize.Level1)
{
    if (false) {
        int32_t ret = g_powerInterface->ForceSuspend();
        EXPECT_EQ(0, ret) << "HdfPowerHdiTest005 failed";
    }
}

/**
  * @tc.name: HdfPowerHdiTest006
  * @tc.desc: check SuspendBlock
  * @tc.type: FUNC
  */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest006, TestSize.Level1)
{
    std::string testName = "HdfPowerHdiTest006";
    g_powerInterface->SuspendUnblock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendBlock(testName);
    EXPECT_EQ(0, ret);

    char lockBuf[MAX_PATH] = {0};
    char lockValue[MAX_PATH] = {0};

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    EXPECT_FALSE(ret < EOK);

    sleep(WAIT_TIME);
    ret = HdfPowerHdiTest::ReadFile(lockBuf, lockValue, sizeof(lockValue));
    EXPECT_EQ(0, ret);
    std::string lock = lockValue;
    EXPECT_FALSE(lock.empty());
    auto pos = lock.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "HdfPowerHdiTest006 failed lock: " << lock;
}

/**
  * @tc.name: HdfPowerHdiTest007
  * @tc.desc: check SuspendUnblock
  * @tc.type: FUNC
  */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest007, TestSize.Level1)
{
    std::string testName = "HdfPowerHdiTest007";
    g_powerInterface->SuspendBlock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendUnblock(testName);
    EXPECT_EQ(0, ret);

    char unLockBuf[MAX_PATH] = {0};
    char unLockValue[MAX_PATH] = {0};

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    EXPECT_FALSE(ret < EOK);

    sleep(WAIT_TIME);
    ret = HdfPowerHdiTest::ReadFile(unLockBuf, unLockValue, sizeof(unLockValue));
    EXPECT_EQ(0, ret);
    std::string unLock = unLockValue;
    EXPECT_FALSE(unLock.empty());
    auto pos = unLock.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "HdfPowerHdiTest007 failed unLock: " << unLock;
}

/**
  * @tc.name: HdfPowerHdiTest007
  * @tc.desc: PowerDump
  * @tc.type: FUNC
  */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest008, TestSize.Level1)
{
    if (false) {
        std::string dump;
        EXPECT_TRUE(HDF_SUCCESS ==
        g_powerInterface->PowerDump(dump))<< "HdfPowerHdiTest008 failed";
    }
}
}
