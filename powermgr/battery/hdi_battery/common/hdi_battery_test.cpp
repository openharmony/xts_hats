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
#include <fcntl.h>
#include <functional>
#include <securec.h>
#include <unistd.h>

#include <fstream>
#include "v1_1/battery_interface_proxy.h"
#include "v1_1/ibattery_callback.h"
#include "v1_1/types.h"

using namespace OHOS::HDI::Battery;
using namespace OHOS::HDI::Battery::V1_1;
using namespace testing::ext;
using namespace OHOS;

namespace {
class BatteryCallback : public HDI::Battery::V1_1::IBatteryCallback {
public:
    BatteryCallback() {};
    ~BatteryCallback() override {};
    int32_t Update(const HDI::Battery::V1_1::BatteryInfo& event) override
    {
        (void)event;
        return 0;
    };
};

sptr<IBatteryInterface> g_batteryInterface = nullptr;
sptr<IBatteryCallback> g_callback = new BatteryCallback();
}

class HdfBatteryHdiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    static int32_t ReadFile(const char *path, char *buf, size_t size);
    static int32_t ConvertInt(const std::string &value);
};

void HdfBatteryHdiTest::SetUpTestCase(void)
{
    g_batteryInterface = IBatteryInterface::Get(true);
}

void HdfBatteryHdiTest::TearDownTestCase(void)
{
}

void HdfBatteryHdiTest::SetUp(void)
{
}

void HdfBatteryHdiTest::TearDown(void)
{
}

namespace {
/**
  * @tc.name: HdfBatteryHdiTest001
  * @tc.desc: Get a client and check whether the client is empty.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest001, TestSize.Level1)
{
    EXPECT_TRUE(nullptr != g_batteryInterface);
}

/**
  * @tc.name: HdfBatteryHdiTest002
  * @tc.desc: Register
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest002, TestSize.Level1)
{
    if (false) {
        printf("HdfBatteryHdiTest002: start.");
        int32_t ret = g_batteryInterface->Register(g_callback);
        EXPECT_EQ(0, ret) << "HdfBatteryHdiTest002 failed";

        printf("HdfBatteryHdiTest002: return.");
    }
}

/**
  * @tc.name: HdfBatteryHdiTest003
  * @tc.desc: UnRegister
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest003, TestSize.Level1)
{
    if (false) {
        printf("HdfBatteryHdiTest003: start.");
        int32_t ret = g_batteryInterface->UnRegister();
        EXPECT_EQ(0, ret) << "HdfBatteryHdiTest002 failed";

        printf("HdfBatteryHdiTest003: return.");
    }
}

/**
  * @tc.name: HdfBatteryHdiTest004
  * @tc.desc: ChangePath
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest004, TestSize.Level1)
{
    printf("HdfBatteryHdiTest004: start.");
    std::string path;
    EXPECT_TRUE(HDF_SUCCESS == g_batteryInterface->ChangePath(path));

    printf("HdfBatteryHdiTest004: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest005
  * @tc.desc: GetCapacity
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest005, TestSize.Level1)
{
    printf("HdfBatteryHdiTest005: start.");
    int capacity;
    g_batteryInterface->GetCapacity(capacity);
    EXPECT_TRUE(capacity <= 100 && capacity >= 0);

    printf("HdfBatteryHdiTest005: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest006
  * @tc.desc: GetVoltage
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest006, TestSize.Level1)
{
    printf("HdfBatteryHdiTest006: start.");
    int voltage;
    g_batteryInterface->GetVoltage(voltage);
    EXPECT_TRUE(voltage >= 0);

    printf("HdfBatteryHdiTest006: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest007
  * @tc.desc: GetTemperature
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest007, TestSize.Level1)
{
    printf("HdfBatteryHdiTest007: start.");
    int temperature;
    g_batteryInterface->GetTemperature(temperature);
    EXPECT_TRUE(temperature >= 0 && temperature <= 600);

    printf("HdfBatteryHdiTest007: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest008
  * @tc.desc: GetHealthState
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest008, TestSize.Level1)
{
    printf("HdfBatteryHdiTest007: start.");
    OHOS::HDI::Battery::V1_1::BatteryHealthState healthState = OHOS::HDI::Battery::V1_1::BatteryHealthState(0);
    g_batteryInterface->GetHealthState(healthState);
    EXPECT_TRUE(healthState >= 0 && healthState <= 6);

    printf("HdfBatteryHdiTest007: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest009
  * @tc.desc: GetPluggedType
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest009, TestSize.Level1)
{
    printf("HdfBatteryHdiTest009: start.");
    OHOS::HDI::Battery::V1_1::BatteryPluggedType pluggedType = OHOS::HDI::Battery::V1_1::BatteryPluggedType(0);
    g_batteryInterface->GetPluggedType(pluggedType);
    EXPECT_TRUE(pluggedType >= OHOS::HDI::Battery::V1_1::BatteryPluggedType::PLUGGED_TYPE_NONE &&
        pluggedType <= OHOS::HDI::Battery::V1_1::BatteryPluggedType::PLUGGED_TYPE_BUTT);

    printf("HdfBatteryHdiTest009: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest010
  * @tc.desc: GetChargeState
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest010, TestSize.Level1)
{
    printf("HdfBatteryHdiTest010: start.");
    OHOS::HDI::Battery::V1_1::BatteryChargeState chargeState = OHOS::HDI::Battery::V1_1::BatteryChargeState(0);
    g_batteryInterface->GetChargeState(chargeState);
    EXPECT_TRUE(chargeState >= 0 && chargeState <= 4);

    printf("HdfBatteryHdiTest010: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest011
  * @tc.desc: GetPresent
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest011, TestSize.Level1)
{
    printf("HdfBatteryHdiTest011: start.");
    bool present = false;
    EXPECT_TRUE(HDF_SUCCESS == g_batteryInterface->GetPresent(present));
    printf("HdfBatteryHdiTest011: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest012
  * @tc.desc: GetTechnology
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest012, TestSize.Level1)
{
    printf("HdfBatteryHdiTest012: start.");
    std::string technology;
    g_batteryInterface->GetTechnology(technology);
    EXPECT_FALSE(technology.empty());

    printf("HdfBatteryHdiTest012: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest013
  * @tc.desc: GetTotalEnergy
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest013, TestSize.Level1)
{
    printf("HdfBatteryHdiTest013: start.");
    int32_t totalEnergy = -1;
    g_batteryInterface->GetTotalEnergy(totalEnergy);
    EXPECT_TRUE(totalEnergy >= 0);

    printf("HdfBatteryHdiTest013: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest014
  * @tc.desc: GetCurrentAverage
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest014, TestSize.Level1)
{
    printf("HdfBatteryHdiTest014: start.");
    int curAverage = -1;
    g_batteryInterface->GetCurrentAverage(curAverage);
    EXPECT_TRUE(curAverage != 0);

    printf("HdfBatteryHdiTest014: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest015
  * @tc.desc: GetCurrentNow
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest015, TestSize.Level1)
{
    printf("HdfBatteryHdiTest015: start.");
    int32_t nowCurr = -1;
    g_batteryInterface->GetCurrentNow(nowCurr);
    EXPECT_TRUE(nowCurr != -1);

    printf("HdfBatteryHdiTest015: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest016
  * @tc.desc: GetRemainEnergy
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest016, TestSize.Level1)
{
    printf("HdfBatteryHdiTest016: start.");
    int32_t remainEnergy = -1;
    g_batteryInterface->GetRemainEnergy(remainEnergy);
    EXPECT_TRUE(remainEnergy >= 0);

    printf("HdfBatteryHdiTest016: return.");
}

/**
  * @tc.name: HdfBatteryHdiTest017
  * @tc.desc: GetBatteryInfo
  * @tc.type: FUNC
  */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest017, TestSize.Level1)
{
    printf("HdfBatteryHdiTest017: start.");
    OHOS::HDI::Battery::V1_1::BatteryInfo event = {
        .capacity = -1,
    };
    g_batteryInterface->GetBatteryInfo(event);
    EXPECT_TRUE(-1 != event.capacity);

    printf("HdfBatteryHdiTest017: return.");
}

/**
 * @tc.name: HdfBatteryHdiTest018
 * @tc.desc: Test limit charging current
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest018, TestSize.Level1)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_CURRENT;
    scLimit.protocol = "sc";
    scLimit.value = 1000;
    ChargingLimit buckLimit;
    buckLimit.type = TYPE_CURRENT;
    buckLimit.protocol = "buck";
    buckLimit.value = 1100;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    chargeLimitList.push_back(buckLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == ERR_OK);

    std::string currentPath = "/data/service/el0/battery/current_limit";
    std::string line;
    std::string chargeLimitStr;
    std::string writeChargeInfo = scLimit.protocol + " " + std::to_string(scLimit.value) + "\n" +
        buckLimit.protocol + " " + std::to_string(buckLimit.value) + "\n";
    std::ifstream fin(currentPath.c_str());
    if (fin) {
        while (getline(fin, line)) {
            chargeLimitStr += line + "\n";
        }
    }
    EXPECT_EQ(true, chargeLimitStr == writeChargeInfo);
}

/**
 * @tc.name: HdfBatteryHdiTest019
 * @tc.desc: Test limit charging voltage
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest019, TestSize.Level1)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_VOLTAGE;
    scLimit.protocol = "sc";
    scLimit.value = 2000;
    ChargingLimit buckLimit;
    buckLimit.type = TYPE_VOLTAGE;
    buckLimit.protocol = "buck";
    buckLimit.value = 3000;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    chargeLimitList.push_back(buckLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == ERR_OK);

    std::string voltagePath = "/data/service/el0/battery/voltage_limit";
    std::string line;
    std::string voltageLimitStr;
    std::string writeVoltageInfo = scLimit.protocol + " " + std::to_string(scLimit.value) + "\n" +
        buckLimit.protocol + " " + std::to_string(buckLimit.value) + "\n";
    std::ifstream fin(voltagePath.c_str());
    if (fin) {
        while (getline(fin, line)) {
            voltageLimitStr += line + "\n";
        }
    }
    EXPECT_EQ(true, voltageLimitStr == writeVoltageInfo);
}
}
