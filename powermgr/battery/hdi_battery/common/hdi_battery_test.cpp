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
#include "v2_0/battery_interface_proxy.h"
#include "v2_0/ibattery_callback.h"
#include "v2_0/types.h"

using namespace OHOS::HDI::Battery;
using namespace OHOS::HDI::Battery::V2_0;
using namespace testing::ext;
using namespace OHOS;

namespace {
class BatteryCallback : public HDI::Battery::V2_0::IBatteryCallback {
public:
    BatteryCallback() {};
    ~BatteryCallback() override {};
    int32_t Update(const HDI::Battery::V2_0::BatteryInfo& event) override
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
    OHOS::HDI::Battery::V2_0::BatteryHealthState healthState = OHOS::HDI::Battery::V2_0::BatteryHealthState(0);
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
    OHOS::HDI::Battery::V2_0::BatteryPluggedType pluggedType = OHOS::HDI::Battery::V2_0::BatteryPluggedType(0);
    g_batteryInterface->GetPluggedType(pluggedType);
    EXPECT_TRUE(pluggedType >= OHOS::HDI::Battery::V2_0::BatteryPluggedType::PLUGGED_TYPE_NONE &&
        pluggedType <= OHOS::HDI::Battery::V2_0::BatteryPluggedType::PLUGGED_TYPE_BUTT);

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
    OHOS::HDI::Battery::V2_0::BatteryChargeState chargeState = OHOS::HDI::Battery::V2_0::BatteryChargeState(0);
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
    OHOS::HDI::Battery::V2_0::BatteryInfo event = {
        .capacity = -1,
    };
    g_batteryInterface->GetBatteryInfo(event);
    EXPECT_TRUE(-1 != event.capacity);

    printf("HdfBatteryHdiTest017: return.");
}

/**
 * @tc.name: HdfBatteryHdiTest018
 * @tc.desc: GetChargeType
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest018, TestSize.Level1)
{
    printf("HdfBatteryHdiTest018: start.");
    OHOS::HDI::Battery::V2_0::ChargeType chargeType = OHOS::HDI::Battery::V2_0::ChargeType(0);
    g_batteryInterface->GetChargeType(chargeType);
    EXPECT_TRUE(chargeType >= OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_NONE &&
        chargeType <= OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_WIRELESS_SUPER_QUICK);

    printf("HdfBatteryHdiTest018: return.");
}

/**
 * @tc.name: HdfBatteryHdiTest019
 * @tc.desc: Judgment ChargeType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest019, TestSize.Level1)
{
    printf("HdfBatteryHdiTest019: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_NONE == 0);
    printf("HdfBatteryHdiTest019: return.");
}

/**
 * @tc.name: HdfBatteryHdiTest020
 * @tc.desc: Test limit charging current
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest020, TestSize.Level1)
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
 * @tc.name: HdfBatteryHdiTest021
 * @tc.desc: Test limit charging voltage
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest021, TestSize.Level1)
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

/**
 * @tc.name: HdfBatteryHdiTest022
 * @tc.desc: Judgment BatteryHealthState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest022, TestSize.Level3)
{
    printf("HdfBatteryHdiTest022: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryHealthState::BATTERY_HEALTH_UNKNOWN == 0);
    printf("HdfBatteryHdiTest022: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest023
 * @tc.desc: Judgment BatteryHealthState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest023, TestSize.Level3)
{
    printf("HdfBatteryHdiTest023: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryHealthState::BATTERY_HEALTH_GOOD == 1);
    printf("HdfBatteryHdiTest023: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest024
 * @tc.desc: Judgment BatteryHealthState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest024, TestSize.Level3)
{
    printf("HdfBatteryHdiTest024: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryHealthState::BATTERY_HEALTH_OVERHEAT == 2);
    printf("HdfBatteryHdiTest024: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest025
 * @tc.desc: Judgment BatteryHealthState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest025, TestSize.Level3)
{
    printf("HdfBatteryHdiTest025: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryHealthState::BATTERY_HEALTH_OVERVOLTAGE == 3);
    printf("HdfBatteryHdiTest025: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest026
 * @tc.desc: Judgment BatteryHealthState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest026, TestSize.Level3)
{
    printf("HdfBatteryHdiTest026: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryHealthState::BATTERY_HEALTH_COLD == 4);
    printf("HdfBatteryHdiTest026: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest027
 * @tc.desc: Judgment BatteryHealthState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest027, TestSize.Level3)
{
    printf("HdfBatteryHdiTest027: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryHealthState::BATTERY_HEALTH_DEAD == 5);
    printf("HdfBatteryHdiTest027: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest028
 * @tc.desc: Judgment BatteryHealthState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest028, TestSize.Level3)
{
    printf("HdfBatteryHdiTest028: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryHealthState::BATTERY_HEALTH_RESERVED == 6);
    printf("HdfBatteryHdiTest028: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest029
 * @tc.desc: Judgment BatteryChargeState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest029, TestSize.Level3)
{
    printf("HdfBatteryHdiTest029: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryChargeState::CHARGE_STATE_NONE == 0);
    printf("HdfBatteryHdiTest029: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest030
 * @tc.desc: Judgment BatteryChargeState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest030, TestSize.Level3)
{
    printf("HdfBatteryHdiTest030: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryChargeState::CHARGE_STATE_ENABLE == 1);
    printf("HdfBatteryHdiTest030: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest031
 * @tc.desc: Judgment BatteryChargeState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest031, TestSize.Level3)
{
    printf("HdfBatteryHdiTest031: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryChargeState::CHARGE_STATE_DISABLE == 2);
    printf("HdfBatteryHdiTest031: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest032
 * @tc.desc: Judgment BatteryChargeState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest032, TestSize.Level3)
{
    printf("HdfBatteryHdiTest032: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryChargeState::CHARGE_STATE_FULL == 3);
    printf("HdfBatteryHdiTest032: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest033
 * @tc.desc: Judgment BatteryChargeState enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest033, TestSize.Level3)
{
    printf("HdfBatteryHdiTest033: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryChargeState::CHARGE_STATE_RESERVED == 4);
    printf("HdfBatteryHdiTest033: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest034
 * @tc.desc: Judgment BatteryPluggedType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest034, TestSize.Level3)
{
    printf("HdfBatteryHdiTest034: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryPluggedType::PLUGGED_TYPE_NONE == 0);
    printf("HdfBatteryHdiTest034: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest035
 * @tc.desc: Judgment BatteryPluggedType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest035, TestSize.Level3)
{
    printf("HdfBatteryHdiTest035: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryPluggedType::PLUGGED_TYPE_AC == 1);
    printf("HdfBatteryHdiTest035: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest036
 * @tc.desc: Judgment BatteryPluggedType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest036, TestSize.Level3)
{
    printf("HdfBatteryHdiTest036: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryPluggedType::PLUGGED_TYPE_USB == 2);
    printf("HdfBatteryHdiTest036: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest037
 * @tc.desc: Judgment BatteryPluggedType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest037, TestSize.Level3)
{
    printf("HdfBatteryHdiTest037: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryPluggedType::PLUGGED_TYPE_WIRELESS == 3);
    printf("HdfBatteryHdiTest037: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest038
 * @tc.desc: Judgment BatteryPluggedType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest038, TestSize.Level3)
{
    printf("HdfBatteryHdiTest038: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::BatteryPluggedType::PLUGGED_TYPE_BUTT == 4);
    printf("HdfBatteryHdiTest038: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest039
 * @tc.desc: Judgment ChargingLimitType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest039, TestSize.Level3)
{
    printf("HdfBatteryHdiTest039: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::ChargingLimitType::TYPE_CURRENT == 0);
    printf("HdfBatteryHdiTest039: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest040
 * @tc.desc: Judgment ChargingLimitType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest040, TestSize.Level3)
{
    printf("HdfBatteryHdiTest040: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::ChargingLimitType::TYPE_VOLTAGE == 1);
    printf("HdfBatteryHdiTest040: end.");
}


/**
 * @tc.name: HdfBatteryHdiTest041
 * @tc.desc: Judgment ChargeType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest041, TestSize.Level3)
{
    printf("HdfBatteryHdiTest041: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_WIRED_NORMAL == 1);
    printf("HdfBatteryHdiTest041: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest042
 * @tc.desc: Judgment ChargeType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest042, TestSize.Level3)
{
    printf("HdfBatteryHdiTest042: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_WIRED_QUICK == 2);
    printf("HdfBatteryHdiTest042: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest043
 * @tc.desc: Judgment ChargeType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest043, TestSize.Level3)
{
    printf("HdfBatteryHdiTest043: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_WIRED_SUPER_QUICK == 3);
    printf("HdfBatteryHdiTest043: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest044
 * @tc.desc: Judgment ChargeType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest044, TestSize.Level3)
{
    printf("HdfBatteryHdiTest044: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_WIRELESS_NORMAL == 4);
    printf("HdfBatteryHdiTest044: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest045
 * @tc.desc: Judgment ChargeType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest045, TestSize.Level3)
{
    printf("HdfBatteryHdiTest045: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_WIRELESS_QUICK == 5);
    printf("HdfBatteryHdiTest045: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest046
 * @tc.desc: Judgment ChargeType enum value of the Types.idl
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest046, TestSize.Level3)
{
    printf("HdfBatteryHdiTest046: start.");
    EXPECT_TRUE(OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_WIRELESS_SUPER_QUICK == 6);
    printf("HdfBatteryHdiTest046: end.");
}

/**
 * @tc.name: HdfBatteryHdiTest047
 * @tc.desc: Test SetBatteryConfig
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest047, TestSize.Level1)
{
    std::string sceneName = "xxx";
    std::string value = "";
    int32_t result = g_batteryInterface->SetBatteryConfig(sceneName, value);
    EXPECT_EQ(true, result != 0);
}

/**
 * @tc.name: HdfBatteryHdiTest048
 * @tc.desc: Test GetBatteryConfig
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest048, TestSize.Level1)
{
    std::string sceneName = "xxx";
    std::string value = "";
    int32_t result = g_batteryInterface->GetBatteryConfig(sceneName, value);
    EXPECT_EQ(true, value == "");
    EXPECT_EQ(true, result != 0);
}

/**
 * @tc.name: HdfBatteryHdiTest049
 * @tc.desc: Test IsBatteryConfigSupported
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryHdiTest, HdfBatteryHdiTest049, TestSize.Level1)
{
    std::string sceneName = "xxx";
    bool value = false;
    int32_t result = g_batteryInterface->IsBatteryConfigSupported(sceneName, value);
    EXPECT_EQ(true, value == false);
    EXPECT_EQ(true, result != 0);
}
}
