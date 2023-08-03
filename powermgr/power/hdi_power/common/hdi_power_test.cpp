/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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
#include "v1_1/ipower_interface.h"
#include "v1_1/ipower_hdi_callback.h"
#include "v1_1/power_types.h"
#include "v1_1/running_lock_types.h"

using namespace OHOS::HDI;
using namespace OHOS::HDI::Power::V1_1;
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
  * @tc.name: HdfPowerHdiTest008
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

/**
  * @tc.name: HdfPowerHdiTest009
  * @tc.desc: HoldRunningLock
  * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest009, TestSize.Level1)
{
    struct RunningLockInfo info = {
        .name = "acts_test",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret) << "HdfPowerHdiTest009 failed";
}

/**
  * @tc.name: HdfPowerHdiTest010
  * @tc.desc: HoldRunningLock, name is null
  * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest010, TestSize.Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_ERR_INVALID_PARAM == ret) << "HdfPowerHdiTest010 failed";
}

/**
  * @tc.name: HdfPowerHdiTest011
  * @tc.desc: HoldRunningLock, type is invaild
  * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest011, TestSize.Level1)
{
    struct RunningLockInfo info = {
        .name = "acts_test",
        .type = static_cast<RunningLockType>(100),
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_ERR_INVALID_PARAM == ret) << "HdfPowerHdiTest011 failed";
}

/**
  * @tc.name: HdfPowerHdiTest012
  * @tc.desc: UnholdRunningLock, name is null
  * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest012, TestSize.Level1)
{
    struct RunningLockInfo info = {
        .name = "",
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_ERR_INVALID_PARAM == ret) << "HdfPowerHdiTest012 failed";
}

/**
  * @tc.name: HdfPowerHdiTest013
  * @tc.desc: UnholdRunningLock, type is invaild
  * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest013, TestSize.Level1)
{
    struct RunningLockInfo info = {
        .name = "acts_test",
        .type = static_cast<RunningLockType>(100),
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_ERR_INVALID_PARAM == ret) << "HdfPowerHdiTest013 failed";
}

/**
  * @tc.name: HdfPowerHdiTest014
  * @tc.desc: UnholdRunningLock, runninglock type not found
  * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest014, TestSize.Level1)
{
    struct RunningLockInfo info = {
        .name = "acts_test",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_ERR_NOT_SUPPORT == ret) << "HdfPowerHdiTest014 failed";
}

/**
  * @tc.name: HdfPowerHdiTest015
  * @tc.desc: UnholdRunningLock, runninglock name not found
  * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest015, TestSize.Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";
    std::string errorLockName = "error_acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    info.name = errorLockName;
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_ERR_NOT_SUPPORT == ret);
    info.name = lockName;
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.name: HdfPowerHdiTest016
 * @tc.desc: Judgment PowerHdfCmd enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest016, TestSize.Level3)
{
    printf("HdfPowerHdiTest016: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfCmd::CMD_REGISTER_CALLBCK == 0);
    printf("HdfPowerHdiTest016: end.");
}

/**
 * @tc.name: HdfPowerHdiTest017
 * @tc.desc: Judgment PowerHdfCmd enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest017, TestSize.Level3)
{
    printf("HdfPowerHdiTest017: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfCmd::CMD_START_SUSPEND == 1);
    printf("HdfPowerHdiTest017: end.");
}

/**
 * @tc.name: HdfPowerHdiTest018
 * @tc.desc: Judgment PowerHdfCmd enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest018, TestSize.Level3)
{
    printf("HdfPowerHdiTest018: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfCmd::CMD_STOP_SUSPEND == 2);
    printf("HdfPowerHdiTest018: end.");
}

/**
 * @tc.name: HdfPowerHdiTest019
 * @tc.desc: Judgment PowerHdfCmd enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest019, TestSize.Level3)
{
    printf("HdfPowerHdiTest019: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfCmd::CMD_FORCE_SUSPEND == 3);
    printf("HdfPowerHdiTest019: end.");
}

/**
 * @tc.name: HdfPowerHdiTest020
 * @tc.desc: Judgment PowerHdfCmd enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest020, TestSize.Level3)
{
    printf("HdfPowerHdiTest020: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfCmd::CMD_SUSPEND_BLOCK == 4);
    printf("HdfPowerHdiTest020: end.");
}

/**
 * @tc.name: HdfPowerHdiTest021
 * @tc.desc: Judgment PowerHdfCmd enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest021, TestSize.Level3)
{
    printf("HdfPowerHdiTest021: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfCmd::CMD_SUSPEND_UNBLOCK == 5);
    printf("HdfPowerHdiTest021: end.");
}

/**
 * @tc.name: HdfPowerHdiTest022
 * @tc.desc: Judgment PowerHdfCmd enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest022, TestSize.Level3)
{
    printf("HdfPowerHdiTest022: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfCmd::CMD_DUMP == 6);
    printf("HdfPowerHdiTest022: end.");
}

/**
 * @tc.name: HdfPowerHdiTest023
 * @tc.desc: Judgment PowerHdfCallbackCmd enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest023, TestSize.Level3)
{
    printf("HdfPowerHdiTest023: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfCallbackCmd::CMD_ON_SUSPEND == 0);
    printf("HdfPowerHdiTest023: end.");
}

/**
 * @tc.name: HdfPowerHdiTest024
 * @tc.desc: Judgment PowerHdfCallbackCmd enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest024, TestSize.Level3)
{
    printf("HdfPowerHdiTest024: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfCallbackCmd::CMD_ON_WAKEUP == 1);
    printf("HdfPowerHdiTest024: end.");
}

/**
 * @tc.name: HdfPowerHdiTest025
 * @tc.desc: Judgment PowerHdfState enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest025, TestSize.Level3)
{
    printf("HdfPowerHdiTest025: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfState::AWAKE == 0);
    printf("HdfPowerHdiTest025: end.");
}

/**
 * @tc.name: HdfPowerHdiTest026
 * @tc.desc: Judgment PowerHdfState enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest026, TestSize.Level3)
{
    printf("HdfPowerHdiTest026: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfState::INACTIVE == 1);
    printf("HdfPowerHdiTest026: end.");
}

/**
 * @tc.name: HdfPowerHdiTest027
 * @tc.desc: Judgment PowerHdfState enum value of the PowerTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest027, TestSize.Level3)
{
    printf("HdfPowerHdiTest027: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::PowerHdfState::SLEEP == 2);
    printf("HdfPowerHdiTest027: end.");
}

/**
 * @tc.name: HdfPowerHdiTest028
 * @tc.desc: Judgment BaseRunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest028, TestSize.Level3)
{
    printf("HdfPowerHdiTest028: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::BaseRunningLockType::RUNNINGLOCK_SCREEN == 0);
    printf("HdfPowerHdiTest028: end.");
}

/**
 * @tc.name: HdfPowerHdiTest029
 * @tc.desc: Judgment BaseRunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest029, TestSize.Level3)
{
    printf("HdfPowerHdiTest029: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::BaseRunningLockType::RUNNINGLOCK_BACKGROUND == 1);
    printf("HdfPowerHdiTest029: end.");
}

/**
 * @tc.name: HdfPowerHdiTest030
 * @tc.desc: Judgment BaseRunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest030, TestSize.Level3)
{
    printf("HdfPowerHdiTest030: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::BaseRunningLockType::RUNNINGLOCK_PROXIMITY_SCREEN_CONTROL == 2);
    printf("HdfPowerHdiTest030: end.");
}

/**
 * @tc.name: HdfPowerHdiTest031
 * @tc.desc: Judgment RunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest031, TestSize.Level3)
{
    printf("HdfPowerHdiTest031: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE == 3);
    printf("HdfPowerHdiTest031: end.");
}

/**
 * @tc.name: HdfPowerHdiTest032
 * @tc.desc: Judgment RunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest032, TestSize.Level3)
{
    printf("HdfPowerHdiTest032: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION == 5);
    printf("HdfPowerHdiTest032: end.");
}

/**
 * @tc.name: HdfPowerHdiTest033
 * @tc.desc: Judgment RunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest033, TestSize.Level3)
{
    printf("HdfPowerHdiTest033: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO == 9);
    printf("HdfPowerHdiTest033: end.");
}

/**
 * @tc.name: HdfPowerHdiTest034
 * @tc.desc: Judgment RunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest034, TestSize.Level3)
{
    printf("HdfPowerHdiTest034: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT == 17);
    printf("HdfPowerHdiTest034: end.");
}

/**
 * @tc.name: HdfPowerHdiTest035
 * @tc.desc: Judgment RunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest035, TestSize.Level3)
{
    printf("HdfPowerHdiTest035: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION == 33);
    printf("HdfPowerHdiTest035: end.");
}

/**
 * @tc.name: HdfPowerHdiTest036
 * @tc.desc: Judgment RunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest036, TestSize.Level3)
{
    printf("HdfPowerHdiTest036: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::RunningLockType::RUNNINGLOCK_BACKGROUND_TASK == 65);
    printf("HdfPowerHdiTest036: end.");
}

/**
 * @tc.name: HdfPowerHdiTest037
 * @tc.desc: Judgment RunningLockType enum value of the RunningLockTypes.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfPowerHdiTest, HdfPowerHdiTest037, TestSize.Level3)
{
    printf("HdfPowerHdiTest037: start.");
    EXPECT_TRUE(OHOS::HDI::Power::V1_1::RunningLockType::RUNNINGLOCK_BUTT == 66);
    printf("HdfPowerHdiTest037: end.");
}
}
