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

#include <cmath>
#include <cstdio>
#include <fcntl.h>
#include <functional>
#include <gtest/gtest.h>
#include <hdf_core_log.h>
#include <mutex>
#include <securec.h>
#include <unistd.h>

#include "hdf_base.h"
#include "osal_time.h"
#include "v1_1/ifan_callback.h"
#include "v1_1/ithermal_callback.h"
#include "v1_1/ithermal_interface.h"
#include "v1_1/thermal_types.h"

using namespace OHOS::HDI;
using namespace OHOS::HDI::Thermal::V1_1;
using namespace testing::ext;

namespace {
int g_onFanDataEventCount = 0;
int g_onThermalDataEventCount = 0;
class ThermalCallbackMock : public IThermalCallback {
public:
    virtual ~ThermalCallbackMock() {}
    using ThermalEventCallback = std::function<int32_t(const HdfThermalCallbackInfo &event)>;
    static int32_t RegisterThermalEvent(const ThermalEventCallback &eventCb)
    {
        (void)eventCb;
        return 0;
    }
    int32_t OnThermalDataEvent(const HdfThermalCallbackInfo &event) override
    {
        (void)event;
        g_onThermalDataEventCount++;
        return 0;
    }
};

class FanCallbackMock : public IFanCallback {
public:
    virtual ~FanCallbackMock() {}
    using FanEventCallback = std::function<int32_t(const HdfThermalCallbackInfo &event)>;
    static int32_t RegisterFanEvent(const FanEventCallback &eventCb)
    {
        (void)eventCb;
        return 0;
    }
    int32_t OnFanDataEvent(const HdfThermalCallbackInfo &event) override
    {
        (void)event;
        g_onFanDataEventCount++;
        return 0;
    }
};

sptr<IThermalInterface> g_thermalInterface = nullptr;
sptr<IThermalCallback> g_callback = new ThermalCallbackMock();
sptr<IFanCallback> g_fanCallback = new FanCallbackMock();
std::mutex g_mutex;

class HdfThermalHdiTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfThermalHdiTestAdditional::SetUpTestCase() { g_thermalInterface = IThermalInterface::Get(true); }

void HdfThermalHdiTestAdditional::TearDownTestCase() {}

void HdfThermalHdiTestAdditional::SetUp(void)
{
    const auto* test_info = testing::UnitTest::GetInstance()->current_test_info();
    HDF_LOGI("%{public}s.%{public}s start", test_info->test_suite_name(), test_info->name());
}

void HdfThermalHdiTestAdditional::TearDown(void)
{
    const auto* test_info = testing::UnitTest::GetInstance()->current_test_info();
    HDF_LOGI("%{public}s.%{public}s end", test_info->test_suite_name(), test_info->name());
} // namespace

namespace {
/**
 * @tc.number  SUB_Powermgr_Thermal_SetCpuFreq_0200
 * @tc.name  testSetCpuFreq001
 * @tc.desc  Reliability of function(SetCpuFreq)
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetCpuFreq001, Function | MediumTest | Level1)
{
    int32_t cpuFreq = 1994100;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret |= g_thermalInterface->SetCpuFreq(cpuFreq);
    }
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetCpuFreq_0300
 * @tc.name  testSetCpuFreq002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetCpuFreq002, Function | MediumTest | Level2)
{
    int32_t cpuFreq = -1;
    int32_t ret = g_thermalInterface->SetCpuFreq(cpuFreq);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetCpuFreq_0400
 * @tc.name  testSetCpuFreq003
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetCpuFreq003, Function | MediumTest | Level1)
{
    int32_t cpuFreq = 0x7fffffff;
    int32_t ret = g_thermalInterface->SetCpuFreq(cpuFreq);
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetCpuFreq_0500
 * @tc.name  testSetCpuFreq004
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetCpuFreq004, Function | MediumTest | Level1)
{
    int32_t cpuFreq = 1024;
    int32_t ret = g_thermalInterface->SetCpuFreq(cpuFreq);
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetCpuFreq_0600
 * @tc.name  testSetCpuFreq005
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetCpuFreq005, Function | MediumTest | Level2)
{
    int32_t cpuFreq = 0;
    int32_t ret = g_thermalInterface->SetCpuFreq(cpuFreq);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetGpuFreq_0200
 * @tc.name  testSetGpuFreq001
 * @tc.desc  Reliability of function(SetGpuFreq)
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetGpuFreq001, Function | MediumTest | Level1)
{
    int32_t gpuFreq = 1994100;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret |= g_thermalInterface->SetGpuFreq(gpuFreq);
    }
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetGpuFreq_0300
 * @tc.name  testSetGpuFreq002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetGpuFreq002, Function | MediumTest | Level2)
{
    int32_t gpuFreq = -1;
    int32_t ret = g_thermalInterface->SetGpuFreq(gpuFreq);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetGpuFreq_0400
 * @tc.name  testSetGpuFreq003
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetGpuFreq003, Function | MediumTest | Level1)
{
    int32_t gpuFreq = 0x7fffffff;
    int32_t ret = g_thermalInterface->SetGpuFreq(gpuFreq);
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetGpuFreq_0500
 * @tc.name  testSetGpuFreq004
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetGpuFreq004, Function | MediumTest | Level1)
{
    int32_t gpuFreq = 1024;
    int32_t ret = g_thermalInterface->SetGpuFreq(gpuFreq);
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetGpuFreq_0600
 * @tc.name  testSetGpuFreq005
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetGpuFreq005, Function | MediumTest | Level2)
{
    int32_t gpuFreq = 0;
    int32_t ret = g_thermalInterface->SetGpuFreq(gpuFreq);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetBatteryCurrent_0200
 * @tc.name  testSetBatteryCurrent001
 * @tc.desc  Reliability of function(SetBatteryCurrent)
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetBatteryCurrent001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int32_t batteryCurrent = 1000;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret |= g_thermalInterface->SetBatteryCurrent(batteryCurrent);
    }
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetBatteryCurrent_0300
 * @tc.name  testSetBatteryCurrent002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetBatteryCurrent002, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    int32_t batteryCurrent = -1;
    ret = g_thermalInterface->SetBatteryCurrent(batteryCurrent);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetBatteryCurrent_0400
 * @tc.name  testSetBatteryCurrent003
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetBatteryCurrent003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int32_t batteryCurrent = 0x7fffffff;
    ret = g_thermalInterface->SetBatteryCurrent(batteryCurrent);
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetBatteryCurrent_0500
 * @tc.name  testSetBatteryCurrent004
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetBatteryCurrent004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int32_t batteryCurrent = 1024;
    ret = g_thermalInterface->SetBatteryCurrent(batteryCurrent);
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_SetBatteryCurrent_0600
 * @tc.name  testSetBatteryCurrent005
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testSetBatteryCurrent005, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    int32_t batteryCurrent = 0;
    ret = g_thermalInterface->SetBatteryCurrent(batteryCurrent);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_GetThermalZoneInfo_0200
 * @tc.name  testGetThermalZoneInfo001
 * @tc.desc  Reliability of function(GetThermalZoneInfo)
 */
HWTEST_F(HdfThermalHdiTestAdditional, testGetThermalZoneInfo001, Function | MediumTest | Level1)
{
    HdfThermalCallbackInfo event;
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 30; i++) {
        ret |= g_thermalInterface->GetThermalZoneInfo(event);
    }
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_IsolateCpu_0200
 * @tc.name  testIsolateCpu001
 * @tc.desc  Reliability of function(IsolateCpu)
 */
HWTEST_F(HdfThermalHdiTestAdditional, testIsolateCpu001, Function | MediumTest | Level1)
{
#ifdef PRODUCT_SUPPORT
    int i = 0;
    int32_t isolateNum = 2;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret |= g_thermalInterface->IsolateCpu(isolateNum);
    }
    EXPECT_EQ(0, ret);
#endif
}
/**
 * @tc.number  SUB_Powermgr_Thermal_IsolateCpu_0300
 * @tc.name  testIsolateCpu002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testIsolateCpu002, Function | MediumTest | Level2)
{
    int32_t isolateNum = -1;
    int32_t ret = g_thermalInterface->IsolateCpu(isolateNum);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_IsolateCpu_0400
 * @tc.name  testIsolateCpu003
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testIsolateCpu003, Function | MediumTest | Level2)
{
    int32_t isolateNum = 0;
    int32_t ret = g_thermalInterface->IsolateCpu(isolateNum);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_IsolateCpu_0500
 * @tc.name  testIsolateCpu004
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testIsolateCpu004, Function | MediumTest | Level1)
{
#ifdef PRODUCT_SUPPORT
    int32_t isolateNum = 0x7fffffff;
    int32_t ret = g_thermalInterface->IsolateCpu(isolateNum);
    EXPECT_EQ(0, ret);
#endif
}
/**
 * @tc.number  SUB_Powermgr_Thermal_IsolateCpu_0600
 * @tc.name  testIsolateCpu005
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testIsolateCpu005, Function | MediumTest | Level1)
{
#ifdef PRODUCT_SUPPORT
    int32_t isolateNum = 3;
    int32_t ret = g_thermalInterface->IsolateCpu(isolateNum);
    EXPECT_EQ(0, ret);
#endif
}
/**
 * @tc.number  SUB_Powermgr_Thermal_Register_0200
 * @tc.name  testRegister001
 * @tc.desc  Reliability of function(Register)
 */
HWTEST_F(HdfThermalHdiTestAdditional, testRegister001, Function | MediumTest | Level1)
{
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret |= g_thermalInterface->Register(g_callback);
        g_thermalInterface->Unregister();
    }
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_Register_0200
 * @tc.name  testRegister002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testRegister002, Function | MediumTest | Level2)
{
    int32_t ret;
    ret = g_thermalInterface->Register(nullptr);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_Unregister_0200
 * @tc.name  testUnregister001
 * @tc.desc  Reliability of function(Unregister)
 */
HWTEST_F(HdfThermalHdiTestAdditional, testUnregister001, Function | MediumTest | Level1)
{
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        g_thermalInterface->Register(g_callback);
        ret |= g_thermalInterface->Unregister();
    }
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_Unregister_0300
 * @tc.name  testUnregister002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testUnregister002, Function | MediumTest | Level2)
{
    int32_t ret = g_thermalInterface->Unregister();
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_RegisterFanCallback_0200
 * @tc.name  testRegisterFanCallback001
 * @tc.desc  Reliability of function(RegisterFanCallback)
 */
HWTEST_F(HdfThermalHdiTestAdditional, testRegisterFanCallback001, Function | MediumTest | Level1)
{
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret |= g_thermalInterface->RegisterFanCallback(g_fanCallback);
        g_thermalInterface->UnregisterFanCallback();
    }
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_RegisterFanCallback_0300
 * @tc.name  testRegisterFanCallback002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testRegisterFanCallback002, Function | MediumTest | Level2)
{
    int32_t ret = g_thermalInterface->RegisterFanCallback(nullptr);
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_UnregisterFanCallback_0200
 * @tc.name  testUnregisterFanCallback001
 * @tc.desc  Test input param
 */
HWTEST_F(HdfThermalHdiTestAdditional, testUnregisterFanCallback001, Function | MediumTest | Level1)
{
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        g_thermalInterface->RegisterFanCallback(g_fanCallback);
        ret |= g_thermalInterface->UnregisterFanCallback();
    }
    EXPECT_EQ(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_UnregisterFanCallback_0300
 * @tc.name  testUnregisterFanCallback002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(HdfThermalHdiTestAdditional, testUnregisterFanCallback002, Function | MediumTest | Level2)
{
    int32_t ret = g_thermalInterface->UnregisterFanCallback();
    EXPECT_NE(0, ret);
}
/**
 * @tc.number  SUB_Powermgr_Thermal_OnThermalDataEvent_0100
 * @tc.name  testOnThermalDataEvent001
 * @tc.desc  Test callback
 */
HWTEST_F(HdfThermalHdiTestAdditional, testOnThermalDataEvent001, Function | MediumTest | Level1)
{
    int32_t ret = g_thermalInterface->Register(g_callback);
    EXPECT_EQ(0, ret);
    sleep(35);
    EXPECT_EQ(true, (g_onThermalDataEventCount != 0));
    g_thermalInterface->Unregister();
    g_onThermalDataEventCount = 0;
}
/**
 * @tc.number  SUB_Powermgr_Thermal_OnThermalDataEvent_0200
 * @tc.name  testOnThermalDataEvent002
 * @tc.desc  Test callback
 */
HWTEST_F(HdfThermalHdiTestAdditional, testOnThermalDataEvent002, Function | MediumTest | Level2)
{
    sleep(35);
    EXPECT_EQ(false, (g_onThermalDataEventCount != 0));
    g_onThermalDataEventCount = 0;
}
} // namespace
