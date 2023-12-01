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
#include <fcntl.h>
#include <functional>
#include <gtest/gtest.h>
#include <mutex>
#include <securec.h>
#include <unistd.h>

#include "hdf_base.h"
#include "osal_time.h"
#include "v1_1/ipower_hdi_callback.h"
#include "v1_1/ipower_interface.h"
#include "v1_1/power_types.h"
#include "v1_1/running_lock_types.h"

using namespace OHOS::HDI;
using namespace OHOS::HDI::Power::V1_1;
using namespace testing::ext;

namespace {
class PowerHdiCallback : public IPowerHdiCallback {
public:
    ~PowerHdiCallback() override{};
    int32_t OnSuspend() override { return 0; };

    int32_t OnWakeup() override { return 0; };
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

class HdfPowerHdiTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    static int32_t ReadFile(const std::string path, std::string &buf, size_t size);
};

void HdfPowerHdiTestAdditional::SetUpTestCase() { g_powerInterface = IPowerInterface::Get(true); }

void HdfPowerHdiTestAdditional::TearDownTestCase() {}

void HdfPowerHdiTestAdditional::SetUp() {}

void HdfPowerHdiTestAdditional::TearDown() {}

int32_t HdfPowerHdiTestAdditional::ReadFile(const std::string path, std::string &buf, size_t size)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    int32_t ret;
    char readbuf[size];
    int32_t fd = open(path.c_str(), O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH);
    if (fd < HDF_SUCCESS) {
        return HDF_FAILURE;
    }

    ret = read(fd, readbuf, size);
    if (ret < HDF_SUCCESS) {
        close(fd);
        return HDF_FAILURE;
    }
    buf = readbuf;
    if(readbuf[0] == '\0'){
        EXPECT_EQ(0, 1);
    }
    EXPECT_FALSE(buf.empty());
    close(fd);
    buf.push_back('\0');
    return HDF_SUCCESS;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_0500
 * @tc.name   : testHoldRunningLock001
 * @tc.desc   : check HoldRunningLock,type = RUNNINGLOCK_BACKGROUND_PHONE
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock001, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_0600
 * @tc.name   : testHoldRunningLock002
 * @tc.desc   : check HoldRunningLock,type = RUNNINGLOCK_BACKGROUND_NOTIFICATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock002, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_0700
 * @tc.name   : testHoldRunningLock003
 * @tc.desc   : check HoldRunningLock,type = RUNNINGLOCK_BACKGROUND_AUDIO
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock003, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_0800
 * @tc.name   : testHoldRunningLock004
 * @tc.desc   : check HoldRunningLock,type = RUNNINGLOCK_BACKGROUND_SPORT
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock004, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_0900
 * @tc.name   : testHoldRunningLock005
 * @tc.desc   : check HoldRunningLock,type = RUNNINGLOCK_BACKGROUND_NAVIGATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock005, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1000
 * @tc.name   : testHoldRunningLock006
 * @tc.desc   : check HoldRunningLock,type = RUNNINGLOCK_BACKGROUND_TASK
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock006, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1100
 * @tc.name   : testHoldRunningLock007
 * @tc.desc   : Cycle 100 times,type = RUNNINGLOCK_BACKGROUND_PHONE
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock007, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1200
 * @tc.name   : testHoldRunningLock008
 * @tc.desc   : Cycle 100 times,type = RUNNINGLOCK_BACKGROUND_NOTIFICATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock008, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1300
 * @tc.name   : testHoldRunningLock009
 * @tc.desc   : Cycle 100 times,type = RUNNINGLOCK_BACKGROUND_AUDIO
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock009, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1400
 * @tc.name   : testHoldRunningLock010
 * @tc.desc   : Cycle 100 times,type = RUNNINGLOCK_BACKGROUND_SPORT
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock010, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1500
 * @tc.name   : testHoldRunningLock011
 * @tc.desc   : Cycle 100 times,type = RUNNINGLOCK_BACKGROUND_NAVIGATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock011, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1600
 * @tc.name   : testHoldRunningLock012
 * @tc.desc   : Cycle 100 times,type = RUNNINGLOCK_BACKGROUND_TASK
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock012, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "test_HoldRunningLock",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1700
 * @tc.name   : testHoldRunningLock013
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_NOTIFICATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock013, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1800
 * @tc.name   : testHoldRunningLock014
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_AUDIO
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock014, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_1900
 * @tc.name   : testHoldRunningLock015
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_SPORT
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock015, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2000
 * @tc.name   : testHoldRunningLock016
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_NAVIGATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock016, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2100
 * @tc.name   : testHoldRunningLock017
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_TASK
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock017, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2200
 * @tc.name   : testHoldRunningLock018
 * @tc.desc   : name is null,type = RUNNINGLOCK_BUTT
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock018, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BUTT,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2300
 * @tc.name   : testHoldRunningLock019
 * @tc.desc   : name is null,type = static_cast<RunningLockType>(100)
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock019, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = static_cast<RunningLockType>(100),
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2400
 * @tc.name   : testHoldRunningLock020
 * @tc.desc   : Cycle 100 times,name is null,type = RUNNINGLOCK_BACKGROUND_PHONE
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock020, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_FALSE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2500
 * @tc.name   : testHoldRunningLock021
 * @tc.desc   : Cycle 100 times,name is null,type = RUNNINGLOCK_BACKGROUND_NOTIFICATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock021, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_FALSE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2600
 * @tc.name   : testHoldRunningLock022
 * @tc.desc   : Cycle 100 times,name is null,type = RUNNINGLOCK_BACKGROUND_AUDIO
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock022, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_FALSE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2700
 * @tc.name   : testHoldRunningLock023
 * @tc.desc   : Cycle 100 times,name is null,type = RUNNINGLOCK_BACKGROUND_SPORT
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock023, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_FALSE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2800
 * @tc.name   : testHoldRunningLock024
 * @tc.desc   : Cycle 100 times,name is null,type = RUNNINGLOCK_BACKGROUND_NAVIGATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock024, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_FALSE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_2900
 * @tc.name   : testHoldRunningLock025
 * @tc.desc   : Cycle 100 times,name is null,type = RUNNINGLOCK_BACKGROUND_TASK
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock025, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_FALSE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3000
 * @tc.name   : testHoldRunningLock026
 * @tc.desc   : Cycle 100 times,name is null,type = RUNNINGLOCK_BUTT
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock026, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BUTT,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_FALSE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3100
 * @tc.name   : testHoldRunningLock027
 * @tc.desc   : Cycle 100 times,name is null,type = static_cast<RunningLockType>(100)
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock027, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = static_cast<RunningLockType>(100),
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_FALSE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3200
 * @tc.name   : testHoldRunningLock028
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_PHONE,timeoutMs = -1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock028, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3300
 * @tc.name   : testHoldRunningLock029
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_AUDIO,timeoutMs = -1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock029, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3400
 * @tc.name   : testHoldRunningLock030
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_SPORT,timeoutMs = -1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock030, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3500
 * @tc.name   : testHoldRunningLock031
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_NAVIGATION,timeoutMs = -1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock031, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3600
 * @tc.name   : testHoldRunningLock032
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_TASK,timeoutMs = -1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock032, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3700
 * @tc.name   : testHoldRunningLock033
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_PHONE,timeoutMs = -1,pid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock033, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = -1,
        .pid = 1,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3800
 * @tc.name   : testHoldRunningLock034
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_AUDIO,timeoutMs = -1,pid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock034, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = -1,
        .pid = 1,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_3900
 * @tc.name   : testHoldRunningLock035
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_SPORT,timeoutMs = -1,pid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock035, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = -1,
        .pid = 1,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4000
 * @tc.name   : testHoldRunningLock036
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_NAVIGATION,timeoutMs = -1,pid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock036, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = -1,
        .pid = 1,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4100
 * @tc.name   : testHoldRunningLock037
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_TASK,timeoutMs = -1,pid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock037, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = -1,
        .pid = 1,
        .uid = 0,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4200
 * @tc.name   : testHoldRunningLock038
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_PHONE,timeoutMs = -1,pid = 1,.uid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock038, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = -1,
        .pid = 1,
        .uid = 1,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4300
 * @tc.name   : testHoldRunningLock039
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_AUDIO,timeoutMs = -1,pid = 1,.uid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock039, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = 1,
        .pid = 1,
        .uid = 1,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4400
 * @tc.name   : testHoldRunningLock040
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_SPORT,timeoutMs = -1,pid = 1,.uid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock040, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = 1,
        .pid = 1,
        .uid = 1,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4500
 * @tc.name   : testHoldRunningLock041
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_NAVIGATION,timeoutMs = -1,pid = 1,.uid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock041, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = 1,
        .pid = 1,
        .uid = 1,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4600
 * @tc.name   : testHoldRunningLock042
 * @tc.desc   : name is null,type = RUNNINGLOCK_BACKGROUND_TASK,timeoutMs = -1,pid = 1,.uid = 1
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock042, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = 1,
        .pid = 1,
        .uid = 1,
    };

    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4700
 * @tc.name   : testHoldRunningLock043
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_PHONE
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock043, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4800
 * @tc.name   : testHoldRunningLock044
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_NOTIFICATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock044, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_4900
 * @tc.name   : testHoldRunningLock045
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_AUDIO
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock045, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5000
 * @tc.name   : testHoldRunningLock046
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_SPORT
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock046, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5100
 * @tc.name   : testHoldRunningLock047
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_NAVIGATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock047, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5200
 * @tc.name   : testHoldRunningLock048
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_TASK
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock048, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5300
 * @tc.name   : testHoldRunningLock049
 * @tc.desc   : name = "//,,",type = static_cast<RunningLockType>(100)
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock049, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = static_cast<RunningLockType>(100),
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5400
 * @tc.name   : testHoldRunningLock050
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_PHONE,Cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock050, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5500
 * @tc.name   : testHoldRunningLock051
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_NOTIFICATION,Cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock051, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5600
 * @tc.name   : testHoldRunningLock052
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_AUDIO,Cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock052, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5700
 * @tc.name   : testHoldRunningLock053
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_SPORT,Cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock053, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5800
 * @tc.name   : testHoldRunningLock054
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_NAVIGATION,Cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock054, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_5900
 * @tc.name   : testHoldRunningLock055
 * @tc.desc   : name = "//,,",type = RUNNINGLOCK_BACKGROUND_TASK,Cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock055, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
        ret = g_powerInterface->UnholdRunningLock(info);
        EXPECT_TRUE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_HoldRunningLock_6000
 * @tc.name   : testHoldRunningLock056
 * @tc.desc   : name = "//,,",type = static_cast<RunningLockType>(100),Cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testHoldRunningLock056, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "//,,",
        .type = static_cast<RunningLockType>(100),
        .timeoutMs = 3000,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret;
    for (int i = 0; i < 100; i++) {
        ret = g_powerInterface->HoldRunningLock(info);
        EXPECT_FALSE(HDF_SUCCESS == ret);
    }
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_0300
 * @tc.name   : testSuspendUnblock001
 * @tc.desc   : check SuspendUnblock,testName = "testSuspendUnblock001"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock001, Function | MediumTest | Level1)
{
    std::string testName = "testSuspendUnblock001";
    g_powerInterface->SuspendBlock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendUnblock(testName);
    EXPECT_EQ(0, ret);

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_0400
 * @tc.name   : testSuspendUnblock002
 * @tc.desc   : check SuspendUnblock,testName = "0"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock002, Function | MediumTest | Level1)
{
    std::string testName = "0";
    g_powerInterface->SuspendBlock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendUnblock(testName);
    EXPECT_EQ(0, ret);

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_0500
 * @tc.name   : testSuspendUnblock003
 * @tc.desc   : check SuspendUnblock,testName = "QWER"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock003, Function | MediumTest | Level1)
{
    std::string testName = "QWER";
    g_powerInterface->SuspendBlock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendUnblock(testName);
    EXPECT_EQ(0, ret);

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_0600
 * @tc.name   : testSuspendUnblock004
 * @tc.desc   : check SuspendUnblock,testName = ""
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock004, Function | MediumTest | Level1)
{
    std::string testName = "";
    g_powerInterface->SuspendBlock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendUnblock(testName);
    EXPECT_NE(0, ret);

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_0700
 * @tc.name   : testSuspendUnblock005
 * @tc.desc   : check SuspendUnblock,testName = "//,,"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock005, Function | MediumTest | Level1)
{
    std::string testName = "//,,";
    g_powerInterface->SuspendBlock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendUnblock(testName);
    EXPECT_EQ(0, ret);

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_0800
 * @tc.name   : testSuspendUnblock006
 * @tc.desc   : check SuspendUnblock,testName = "a@%"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock006, Function | MediumTest | Level1)
{
    std::string testName = "a@%";
    g_powerInterface->SuspendBlock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendUnblock(testName);
    EXPECT_EQ(0, ret);

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_0900
 * @tc.name   : testSuspendUnblock007
 * @tc.desc   : check SuspendUnblock,testName = "testSuspendUnblock001",cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock007, Function | MediumTest | Level1)
{
    std::string testName = "testSuspendUnblock001";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendBlock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendUnblock(testName);
        EXPECT_EQ(0, ret);
    }

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_1000
 * @tc.name   : testSuspendUnblock008
 * @tc.desc   : check SuspendUnblock,testName = "0",cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock008, Function | MediumTest | Level1)
{
    std::string testName = "0";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendBlock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendUnblock(testName);
        EXPECT_EQ(0, ret);
    }

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_1100
 * @tc.name   : testSuspendUnblock009
 * @tc.desc   : check SuspendUnblock,testName = "QWER",cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock009, Function | MediumTest | Level1)
{
    std::string testName = "QWER";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendBlock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendUnblock(testName);
        EXPECT_EQ(0, ret);
    }

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_1200
 * @tc.name   : testSuspendUnblock010
 * @tc.desc   : check SuspendUnblock,testName = "",cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock010, Function | MediumTest | Level1)
{
    std::string testName = "";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendBlock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendUnblock(testName);
        EXPECT_NE(0, ret);
    }

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_1300
 * @tc.name   : testSuspendUnblock011
 * @tc.desc   : check SuspendUnblock,testName = "//,,",cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock011, Function | MediumTest | Level1)
{
    std::string testName = "//,,";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendBlock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendUnblock(testName);
        EXPECT_EQ(0, ret);
    }

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendUnblock_1400
 * @tc.name   : testSuspendUnblock012
 * @tc.desc   : check SuspendUnblock,testName = "a@%",cycle 100 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendUnblock012, Function | MediumTest | Level1)
{
    std::string testName = "a@%";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendBlock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendUnblock(testName);
        EXPECT_EQ(0, ret);
    }

    char unLockBuf[MAX_PATH] = {0};
    std::string  unLockBuf_s;
    std::string  unLockValue;

    ret = snprintf_s(unLockBuf, MAX_PATH, sizeof(unLockBuf) - 1, UNLOCK_PATH.c_str());
    unLockBuf_s = unLockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(unLockBuf_s, unLockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(unLockValue.empty());
    auto pos = unLockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendUnblock001 failed unLock: " << unLockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_0300
 * @tc.name   : testSuspendBlock001
 * @tc.desc   : check SuspendBlock,testName = "testSuspendBlock001"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock001, Function | MediumTest | Level1)
{
    std::string testName = "testSuspendBlock001";
    g_powerInterface->SuspendUnblock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendBlock(testName);
    EXPECT_EQ(0, ret);

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_0400
 * @tc.name   : testSuspendBlock002
 * @tc.desc   : check SuspendBlock,testName = "0"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock002, Function | MediumTest | Level1)
{
    std::string testName = "0";
    g_powerInterface->SuspendUnblock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendBlock(testName);
    EXPECT_EQ(0, ret);

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_0500
 * @tc.name   : testSuspendBlock003
 * @tc.desc   : check SuspendBlock,testName = "QWER"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock003, Function | MediumTest | Level1)
{
    std::string testName = "QWER";
    g_powerInterface->SuspendUnblock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendBlock(testName);
    EXPECT_EQ(0, ret);

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_0600
 * @tc.name   : testSuspendBlock004
 * @tc.desc   : check SuspendBlock,testName = ""
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock004, Function | MediumTest | Level1)
{
    std::string testName = "";
    g_powerInterface->SuspendUnblock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendBlock(testName);
    EXPECT_NE(0, ret);

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_0700
 * @tc.name   : testSuspendBlock005
 * @tc.desc   : check SuspendBlock,testName = "//,,"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock005, Function | MediumTest | Level1)
{
    std::string testName = "//,,";
    g_powerInterface->SuspendUnblock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendBlock(testName);
    EXPECT_EQ(0, ret);

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_0800
 * @tc.name   : testSuspendBlock006
 * @tc.desc   : check SuspendBlock,testName = "a@%"
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock006, Function | MediumTest | Level1)
{
    std::string testName = "a@%";
    g_powerInterface->SuspendUnblock(testName);
    sleep(WAIT_TIME);
    int32_t ret = g_powerInterface->SuspendBlock(testName);
    EXPECT_EQ(0, ret);

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_0900
 * @tc.name   : testSuspendBlock007
 * @tc.desc   : check SuspendBlock,testName = "testSuspendBlock001",cycle 5 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock007, Function | MediumTest | Level1)
{
    std::string testName = "testSuspendBlock001";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendUnblock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendBlock(testName);
        EXPECT_EQ(0, ret);
    }

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_1000
 * @tc.name   : testSuspendBlock008
 * @tc.desc   : check SuspendBlock,testName = "0",cycle 5 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock008, Function | MediumTest | Level1)
{
    std::string testName = "0";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendUnblock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendBlock(testName);
        EXPECT_EQ(0, ret);
    }

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_1100
 * @tc.name   : testSuspendBlock009
 * @tc.desc   : check SuspendBlock,testName = "QWER",cycle 5 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock009, Function | MediumTest | Level1)
{
    std::string testName = "QWER";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendUnblock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendBlock(testName);
        EXPECT_EQ(0, ret);
    }

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_1200
 * @tc.name   : testSuspendBlock010
 * @tc.desc   : check SuspendBlock,testName = "",cycle 5 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock010, Function | MediumTest | Level1)
{
    std::string testName = "";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendUnblock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendBlock(testName);
        EXPECT_NE(0, ret);
    }

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_1300
 * @tc.name   : testSuspendBlock011
 * @tc.desc   : check SuspendBlock,testName = "//,,",cycle 5 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock011, Function | MediumTest | Level1)
{
    std::string testName = "//,,";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendUnblock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendBlock(testName);
        EXPECT_EQ(0, ret);
    }

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_SuspendBlock_1400
 * @tc.name   : testSuspendBlock012
 * @tc.desc   : check SuspendBlock,testName = "a@%",cycle 5 times
 */
HWTEST_F(HdfPowerHdiTestAdditional, testSuspendBlock012, Function | MediumTest | Level1)
{
    std::string testName = "a@%";
    int32_t ret;
    for (int i = 0; i < 5; i++) {
        g_powerInterface->SuspendUnblock(testName);
        sleep(WAIT_TIME);
        ret = g_powerInterface->SuspendBlock(testName);
        EXPECT_EQ(0, ret);
    }

    char lockBuf[MAX_PATH] = {0};
    std::string LockBuf_s;
    std::string lockValue;

    ret = snprintf_s(lockBuf, MAX_PATH, sizeof(lockBuf) - 1, LOCK_PATH.c_str());
    LockBuf_s = lockBuf;
    sleep(WAIT_TIME);
    ret = HdfPowerHdiTestAdditional::ReadFile(LockBuf_s, lockValue, MAX_PATH);
    EXPECT_EQ(0, ret);
    EXPECT_FALSE(lockValue.empty());
    auto pos = lockValue.find(testName);
    EXPECT_TRUE(pos != std::string::npos) << "testSuspendBlock001 failed lock: " << lockValue;
}

/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_0500
 * @tc.name   : testUnholdRunningLock001
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_PHONE
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock001, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_0600
 * @tc.name   : testUnholdRunningLock002
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_AUDIO
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock002, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_0700
 * @tc.name   : testUnholdRunningLock003
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_SPORT
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock003, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_0800
 * @tc.name   : testUnholdRunningLock004
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_NAVIGATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock004, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_0900
 * @tc.name   : testUnholdRunningLock005
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_TASK
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock005, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1000
 * @tc.name   : testUnholdRunningLock006
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_PHONE
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock006, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = 3000,
        .pid = -1,
        .uid = -1,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1100
 * @tc.name   : testUnholdRunningLock007
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_NOTIFICATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock007, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION,
        .timeoutMs = 3000,
        .pid = -1,
        .uid = -1,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1200
 * @tc.name   : testUnholdRunningLock008
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_AUDIO
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock008, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = 3000,
        .pid = -1,
        .uid = -1,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1300
 * @tc.name   : testUnholdRunningLock009
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_SPORT
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock009, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = 3000,
        .pid = -1,
        .uid = -1,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1400
 * @tc.name   : testUnholdRunningLock010
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_NAVIGATION
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock010, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = 3000,
        .pid = -1,
        .uid = -1,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1500
 * @tc.name   : testUnholdRunningLock011
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_TASK
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock011, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = 3000,
        .pid = -1,
        .uid = -1,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);

    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1600
 * @tc.name   : testUnholdRunningLock012
 * @tc.desc   : UnholdRunningLock type is RUNNINGLOCK_BACKGROUND_PHONE
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock012, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    info.type = static_cast<RunningLockType>(100);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
    info.type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE;
    ret = g_powerInterface->UnholdRunningLock(info);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1700
 * @tc.name   : testUnholdRunningLock013
 * @tc.desc   : UnholdRunningLock type is invaild
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock013, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    info.type = static_cast<RunningLockType>(100);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
    info.type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO;
    ret = g_powerInterface->UnholdRunningLock(info);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1800
 * @tc.name   : testUnholdRunningLock014
 * @tc.desc   : UnholdRunningLock type is invaild
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock014, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    info.type = static_cast<RunningLockType>(100);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
    info.type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT;
    ret = g_powerInterface->UnholdRunningLock(info);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_1900
 * @tc.name   : testUnholdRunningLock015
 * @tc.desc   : UnholdRunningLock type is invaild
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock015, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    info.type = static_cast<RunningLockType>(100);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
    info.type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION;
    ret = g_powerInterface->UnholdRunningLock(info);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2000
 * @tc.name   : testUnholdRunningLock016
 * @tc.desc   : UnholdRunningLock type is invaild
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock016, Function | MediumTest | Level1)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    info.type = static_cast<RunningLockType>(100);
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
    info.type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK;
    ret = g_powerInterface->UnholdRunningLock(info);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2100
 * @tc.name   : testUnholdRunningLock017
 * @tc.desc   : UnholdRunningLock different names
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock017, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
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
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2200
 * @tc.name   : testUnholdRunningLock018
 * @tc.desc   : UnholdRunningLock different names
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock018, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    std::string lockName = "111";
    std::string errorLockName = "error_acts_test";

    info.name = lockName;
    int32_t ret = g_powerInterface->HoldRunningLock(info);
    EXPECT_TRUE(HDF_SUCCESS == ret);
    info.name = errorLockName;
    ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2300
 * @tc.name   : testUnholdRunningLock019
 * @tc.desc   : UnholdRunningLock different names
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock019, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
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
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2400
 * @tc.name   : testUnholdRunningLock020
 * @tc.desc   : UnholdRunningLock different names
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock020, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
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
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2500
 * @tc.name   : testUnholdRunningLock021
 * @tc.desc   : UnholdRunningLock different names
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock021, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
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
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2600
 * @tc.name   : testUnholdRunningLock022
 * @tc.desc   : UnholdRunningLock name is null
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock022, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_PHONE,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2700
 * @tc.name   : testUnholdRunningLock023
 * @tc.desc   : UnholdRunningLock name is null
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock023, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NOTIFICATION,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2800
 * @tc.name   : testUnholdRunningLock024
 * @tc.desc   : UnholdRunningLock name is null
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock024, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_AUDIO,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_2900
 * @tc.name   : testUnholdRunningLock025
 * @tc.desc   : UnholdRunningLock name is null
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock025, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_SPORT,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_3000
 * @tc.name   : testUnholdRunningLock026
 * @tc.desc   : UnholdRunningLock name is null
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock026, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_NAVIGATION,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_3100
 * @tc.name   : testUnholdRunningLock027
 * @tc.desc   : UnholdRunningLock name is null
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock027, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "",
        .type = RunningLockType::RUNNINGLOCK_BACKGROUND_TASK,
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_3200
 * @tc.name   : testUnholdRunningLock028
 * @tc.desc   : UnholdRunningLock type is invaild
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock028, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .name = "acts_test",
        .type = static_cast<RunningLockType>(100),
        .timeoutMs = -1,
        .pid = 0,
        .uid = 0,
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
/**
 * @tc.number : SUB_Powermgr_Power_HDI_UnholdRunningLock_3300
 * @tc.name   : testUnholdRunningLock029
 * @tc.desc   : UnholdRunningLock only type in info
 */
HWTEST_F(HdfPowerHdiTestAdditional, testUnholdRunningLock029, Function | MediumTest | Level2)
{
    struct RunningLockInfo info = {
        .type = static_cast<RunningLockType>(100),
    };
    int32_t ret = g_powerInterface->UnholdRunningLock(info);
    EXPECT_FALSE(HDF_SUCCESS == ret);
}
} // namespace