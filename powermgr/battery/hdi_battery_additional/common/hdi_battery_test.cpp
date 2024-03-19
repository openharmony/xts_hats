/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
    BatteryCallback(){};
    ~BatteryCallback() override{};
    int32_t Update(const HDI::Battery::V2_0::BatteryInfo &event) override
    {
        (void)event;
        return 0;
    };
};

sptr<IBatteryInterface> g_batteryInterface = nullptr;
sptr<IBatteryCallback> g_callback = new BatteryCallback();
} // namespace

class HdfBatteryHdiTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfBatteryHdiTestAdditional::SetUpTestCase(void) { g_batteryInterface = IBatteryInterface::Get(true); }

void HdfBatteryHdiTestAdditional::TearDownTestCase(void) {}

void HdfBatteryHdiTestAdditional::SetUp(void) {}

void HdfBatteryHdiTestAdditional::TearDown(void) {}

namespace {
/**
 * @tc.number: SUB_Powermgr_Battery_HDI_ChangePath_0200
 * @tc.name: testChangePath001
 * @tc.desc: Verify the reliability of the ChangePath function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testChangePath001, TestSize.Level2)
{
    std::string path = "";
    EXPECT_EQ(HDF_SUCCESS, g_batteryInterface->ChangePath(path));
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_ChangePath_0300
 * @tc.name: testChangePath002
 * @tc.desc: Verify the reliability of the ChangePath function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testChangePath002, TestSize.Level2)
{
    EXPECT_EQ(HDF_SUCCESS, g_batteryInterface->ChangePath(nullptr));
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_ChangePath_0400
 * @tc.name: testChangePath003
 * @tc.desc: Verify the reliability of the ChangePath function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testChangePath003, TestSize.Level2)
{
    std::string path = "test test test";
    EXPECT_EQ(HDF_SUCCESS, g_batteryInterface->ChangePath(path));
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetCapacity_0200
 * @tc.name: testGetCapacity001
 * @tc.desc: Verify the stability of the GetCapacity function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetCapacity001, TestSize.Level1)
{
    int capacity;
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetCapacity(capacity);
        EXPECT_TRUE(capacity <= 100 && capacity >= 0);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetVoltage_0200
 * @tc.name: testGetVoltage001
 * @tc.desc: Verify the stability of the GetVoltage function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetVoltage001, TestSize.Level1)
{
    int voltage;
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetVoltage(voltage);
        EXPECT_TRUE(voltage >= 0);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetTemperature_0200
 * @tc.name: testGetTemperature001
 * @tc.desc: Verify the stability of the GetTemperature function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetTemperature001, TestSize.Level1)
{
    int temperature;
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetTemperature(temperature);
        EXPECT_TRUE(temperature >= 0 && temperature <= 600);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetHealthState_0200
 * @tc.name: testGetHealthState001
 * @tc.desc: Verify the stability of the GetHealthState function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetHealthState001, TestSize.Level1)
{
    OHOS::HDI::Battery::V2_0::BatteryHealthState healthState = OHOS::HDI::Battery::V2_0::BatteryHealthState(0);
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetHealthState(healthState);
        EXPECT_TRUE(healthState >= 0 && healthState <= 6);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetPluggedType_0200
 * @tc.name: testGetPluggedType001
 * @tc.desc: Verify the stability of the GetPluggedType function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetPluggedType001, TestSize.Level1)
{
    OHOS::HDI::Battery::V2_0::BatteryPluggedType pluggedType = OHOS::HDI::Battery::V2_0::BatteryPluggedType(0);
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetPluggedType(pluggedType);
        EXPECT_TRUE(pluggedType >= OHOS::HDI::Battery::V2_0::BatteryPluggedType::PLUGGED_TYPE_NONE &&
                    pluggedType <= OHOS::HDI::Battery::V2_0::BatteryPluggedType::PLUGGED_TYPE_BUTT);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetChargeState_0200
 * @tc.name: testGetChargeState001
 * @tc.desc: Verify the stability of the GetChargeState function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetChargeState001, TestSize.Level1)
{
    OHOS::HDI::Battery::V2_0::BatteryChargeState chargeState = OHOS::HDI::Battery::V2_0::BatteryChargeState(0);
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetChargeState(chargeState);
        EXPECT_TRUE(chargeState >= 0 && chargeState <= 4);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetPresent_0200
 * @tc.name: testGetPresent001
 * @tc.desc: Verify the stability of the GetPresent function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetPresent001, TestSize.Level1)
{
    bool present = false;
    for (uint32_t i = 0; i < 1000; i++) {
        EXPECT_TRUE(HDF_SUCCESS == g_batteryInterface->GetPresent(present));
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetTechnology_0200
 * @tc.name: testGetTechnology001
 * @tc.desc: Verify the stability of the GetTechnology function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetTechnology001, TestSize.Level1)
{
    std::string technology;
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetTechnology(technology);
        EXPECT_FALSE(technology.empty());
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetTotalEnergy_0200
 * @tc.name: testGetTotalEnergy001
 * @tc.desc: Verify the stability of the GetTotalEnergy function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetTotalEnergy001, TestSize.Level1)
{
    int32_t totalEnergy = -1;
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetTotalEnergy(totalEnergy);
        EXPECT_TRUE(totalEnergy >= 0);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetCurrentAverage_0200
 * @tc.name: testGetCurrentAverage001
 * @tc.desc: Verify the stability of the GetCurrentAverage function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetCurrentAverage001, TestSize.Level1)
{
    int32_t curAverage = -1;
    int32_t ret = 0;
    for (uint32_t i = 0; i < 1000; i++) {
        ret = g_batteryInterface->GetCurrentAverage(curAverage);
#ifdef HATS_DRIVERS_PERIPHERAL_BATTERY_PC_MACRO_ISOLATION
        EXPECT_NE(0, curAverage);
#else
        EXPECT_EQ(ret, HDF_SUCCESS);
#endif
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetCurrentNow_0200
 * @tc.name: testGetCurrentNow001
 * @tc.desc: Verify the stability of the GetCurrentNow function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetCurrentNow001, TestSize.Level1)
{
    int32_t nowCurr = -1;
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetCurrentNow(nowCurr);
        EXPECT_TRUE(nowCurr != -1);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetRemainEnergy_0200
 * @tc.name: testGetRemainEnergy001
 * @tc.desc: Verify the stability of the GetRemainEnergy function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetRemainEnergy001, TestSize.Level1)
{
    int32_t remainEnergy = -1;
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetRemainEnergy(remainEnergy);
        EXPECT_TRUE(remainEnergy >= 0);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetBatteryInfo_0200
 * @tc.name: testGetBatteryInfo001
 * @tc.desc: Verify the stability of the GetBatteryInfo function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetBatteryInfo001, TestSize.Level1)
{
    OHOS::HDI::Battery::V2_0::BatteryInfo event = {
        .capacity = -1,
    };
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetBatteryInfo(event);
        EXPECT_TRUE(-1 != event.capacity);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_GetChargeType_0200
 * @tc.name: testGetChargeType001
 * @tc.desc: Verify the stability of the GetChargeType function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testGetChargeType001, TestSize.Level1)
{
    OHOS::HDI::Battery::V2_0::ChargeType chargeType = OHOS::HDI::Battery::V2_0::ChargeType(0);
    for (uint32_t i = 0; i < 1000; i++) {
        g_batteryInterface->GetChargeType(chargeType);
        EXPECT_TRUE(chargeType >= OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_NONE &&
                    chargeType <= OHOS::HDI::Battery::V2_0::ChargeType::CHARGE_TYPE_WIRELESS_SUPER_QUICK);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_Register_0200
 * @tc.name: testBatteryRegister001
 * @tc.desc: Verify the reliability of the Register function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testBatteryRegister001, TestSize.Level2)
{
    EXPECT_NE(0, g_batteryInterface->Register(nullptr));
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_UnRegister_0200
 * @tc.name: testBatteryUnRegister001
 * @tc.desc: Verify the stability of the UnRegister function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testBatteryUnRegister001, TestSize.Level2)
{
    EXPECT_EQ(0, g_batteryInterface->UnRegister());
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_0300
 * @tc.name: testSetChargingLimit001
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit001, TestSize.Level1)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_CURRENT;
    scLimit.protocol = "sc";
    scLimit.value = 1100;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_0400
 * @tc.name: testSetChargingLimit002
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit002, TestSize.Level1)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_VOLTAGE;
    scLimit.protocol = "sc";
    scLimit.value = 3000;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_0500
 * @tc.name: testSetChargingLimit003
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit003, TestSize.Level1)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_CURRENT;
    scLimit.protocol = "sc";
    scLimit.value = 1000;
    ChargingLimit buckLimit;
    buckLimit.type = TYPE_CURRENT;
    buckLimit.protocol = "buck";
    buckLimit.value = 1100;
    ChargingLimit testLimit;
    testLimit.type = TYPE_CURRENT;
    testLimit.protocol = "test";
    testLimit.value = 1200;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    chargeLimitList.push_back(buckLimit);
    chargeLimitList.push_back(testLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);

    std::string currentPath = "/data/service/el0/battery/current_limit";
    std::string line;
    std::string chargeLimitStr;
    std::string writeChargeInfo = scLimit.protocol + " " + std::to_string(scLimit.value) + "\n" + buckLimit.protocol +
                                  " " + std::to_string(buckLimit.value) + "\n" + testLimit.protocol + " " +
                                  std::to_string(testLimit.value) + "\n";
    std::ifstream fin(currentPath.c_str());
    if (fin) {
        while (getline(fin, line)) {
            chargeLimitStr += line + "\n";
        }
    }
    EXPECT_EQ(true, chargeLimitStr == writeChargeInfo);
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_0600
 * @tc.name: testSetChargingLimit004
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit004, TestSize.Level1)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_VOLTAGE;
    scLimit.protocol = "sc";
    scLimit.value = 2000;
    ChargingLimit buckLimit;
    buckLimit.type = TYPE_VOLTAGE;
    buckLimit.protocol = "buck";
    buckLimit.value = 3000;
    ChargingLimit testLimit;
    testLimit.type = TYPE_VOLTAGE;
    testLimit.protocol = "test";
    testLimit.value = 4000;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    chargeLimitList.push_back(buckLimit);
    chargeLimitList.push_back(testLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);

    std::string voltagePath = "/data/service/el0/battery/voltage_limit";
    std::string line;
    std::string voltageLimitStr;
    std::string writeVoltageInfo = scLimit.protocol + " " + std::to_string(scLimit.value) + "\n" + buckLimit.protocol +
                                   " " + std::to_string(buckLimit.value) + "\n" + testLimit.protocol + " " +
                                   std::to_string(testLimit.value) + "\n";
    std::ifstream fin(voltagePath.c_str());
    if (fin) {
        while (getline(fin, line)) {
            voltageLimitStr += line + "\n";
        }
    }
    EXPECT_EQ(true, voltageLimitStr == writeVoltageInfo);
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_0700
 * @tc.name: testSetChargingLimit005
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit005, TestSize.Level1)
{
    int32_t result = HDF_SUCCESS;
    ChargingLimit scLimit;
    scLimit.type = TYPE_CURRENT;
    scLimit.protocol = "sc";
    scLimit.value = 1100;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    for (uint32_t i = 0; i < 1000; i++) {
        result = g_batteryInterface->SetChargingLimit(chargeLimitList);
        EXPECT_EQ(true, result == HDF_SUCCESS);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_0800
 * @tc.name: testSetChargingLimit006
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit006, TestSize.Level1)
{
    int32_t result = HDF_SUCCESS;
    ChargingLimit scLimit;
    scLimit.type = TYPE_VOLTAGE;
    scLimit.protocol = "sc";
    scLimit.value = 3000;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    for (uint32_t i = 0; i < 1000; i++) {
        result = g_batteryInterface->SetChargingLimit(chargeLimitList);
        EXPECT_EQ(true, result == HDF_SUCCESS);
    }
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_0900
 * @tc.name: testSetChargingLimit007
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit007, TestSize.Level2)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_CURRENT;
    scLimit.protocol = "sc";
    scLimit.value = 0;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_1000
 * @tc.name: testSetChargingLimit008
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit008, TestSize.Level2)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_VOLTAGE;
    scLimit.protocol = "sc";
    scLimit.value = 0;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_1100
 * @tc.name: testSetChargingLimit009
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit009, TestSize.Level2)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_CURRENT;
    scLimit.protocol = "sc";
    scLimit.value = -1;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_1200
 * @tc.name: testSetChargingLimit010
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit010, TestSize.Level2)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_VOLTAGE;
    scLimit.protocol = "sc";
    scLimit.value = -1;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_1300
 * @tc.name: testSetChargingLimit011
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit011, TestSize.Level2)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_CURRENT;
    scLimit.protocol = "";
    scLimit.value = 1000;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Powermgr_Battery_HDI_SetChargingLimit_1400
 * @tc.name: testSetChargingLimit012
 * @tc.desc: Verify the reliability of the SetChargingLimit function.
 */
HWTEST_F(HdfBatteryHdiTestAdditional, testSetChargingLimit012, TestSize.Level2)
{
    ChargingLimit scLimit;
    scLimit.type = TYPE_VOLTAGE;
    scLimit.protocol = "";
    scLimit.value = 3000;
    std::vector<ChargingLimit> chargeLimitList;
    chargeLimitList.push_back(scLimit);
    int32_t result = g_batteryInterface->SetChargingLimit(chargeLimitList);
    EXPECT_EQ(true, result == HDF_SUCCESS);
}
} // namespace
