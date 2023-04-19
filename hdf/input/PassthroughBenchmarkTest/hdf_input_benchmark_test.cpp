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
#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include <cmath>
#include <cstdio>
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#include <hdf_base.h>
#include <hdf_log.h>
#include "osal_time.h"
#include "v1_0/iinput_interfaces.h"
#include "input_type.h"
#include "input_callback_impl.h"

using namespace std;
using namespace OHOS::HDI::Input::V1_0;
using namespace testing::ext;

namespace {
namespace {
    sptr<IInputInterfaces>  g_inputInterfaces = nullptr;
    sptr<IInputCallback> g_callback = nullptr;
    sptr<IInputCallback> g_hotplugCb = nullptr;

    constexpr int32_t INIT_DEFAULT_VALUE = 255;
    constexpr int32_t KEEP_ALIVE_TIME_MS = 3000;
    constexpr int32_t TOUCH_INDEX = 1;
    constexpr int32_t TEST_RESULT_LEN = 32;
}

class PassthroughBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};

void PassthroughBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    g_inputInterfaces = IInputInterfaces::Get(true);
    if (g_inputInterfaces != nullptr) {
        g_callback = new InputCallbackImpl(g_inputInterfaces, nullptr);
        g_hotplugCb = new InputCallbackImpl(g_inputInterfaces, g_callback);
    }
}
    
void PassthroughBenchmarkTest::TearDown(const ::benchmark::State &state)
{
}

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0010
  * @tc.name: ScanInputDevice001
  * @tc.desc: scan input device test-benchmark
  * @tc.type: FUNC
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, ScanInputDevice)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    std::vector<DevDesc> sta;

    HDF_LOGI("%s: [Hdi-Input] ScanInputDevice001 enter", __func__);
    int32_t ret;

    for (auto _ : st) {
        ret = g_inputInterfaces->ScanInputDevice(sta);
    }
    if (ret == INPUT_SUCCESS) {
        HDF_LOGE("%s: %d, %d, %d, %d", __func__, sta[0].devType, sta[0].devIndex, sta[1].devType, sta[1].devIndex);
    }
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, ScanInputDevice)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0020
  * @tc.name: OpenInputDev001
  * @tc.desc: open input device test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, OpenInputDevice)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [Hdi-Input] OpenInputDevice001 enter", __func__);
    int32_t ret;
    for (auto _ : st) {
        ret = g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: open device failed, ret %d", __func__, ret);
    }
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, OpenInputDevice)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0030
  * @tc.name: CloseInputDevice001
  * @tc.desc: close input device test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, CloseInputDevice)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] CloseInputDevice001 enter", __func__);
    int32_t ret;
    for (auto _ : st) {
        ret = g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: close device failed, ret %d", __func__, ret);
    }
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, CloseInputDevice)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();


/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0040
  * @tc.name: GetInputDeviceList001
  * @tc.desc: get input device list info test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, GetInputDeviceList)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetInputDeviceList001 enter", __func__);
    int32_t ret;
    uint32_t num = 0;
    std::vector<DeviceInfo> dev;
    for (auto _ : st) {
        ret = g_inputInterfaces->GetInputDeviceList(num, dev, MAX_INPUT_DEV_NUM);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device list failed, ret %d", __func__, ret);
    }
    ret = num <= MAX_INPUT_DEV_NUM ? HDF_SUCCESS : HDF_FAILURE;  /* num <= MAX_INPUT_DEV_NUM return true */
    ASSERT_EQ(ret, HDF_SUCCESS);


    for (uint32_t i = 0; i < num; i++) {
        HDF_LOGI("%s: num = %u, device[%u]'s info is:", __func__, num, i);
        HDF_LOGI("%s: index = %u, devType = %u", __func__, dev[i].devIndex, dev[i].devType);
        HDF_LOGI("%s: chipInfo = %s, vendorName = %s, chipName = %s",
            __func__, dev[i].chipInfo.c_str(), dev[i].vendorName.c_str(), dev[i].chipName.c_str());
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, GetInputDeviceList)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0050
  * @tc.name: GetDeviceType001
  * @tc.desc: get input device type test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, GetDeviceType)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetDeviceType001 enter", __func__);
    int32_t ret;
    uint32_t devType = INIT_DEFAULT_VALUE;

    for (auto _ : st) {
        ret = g_inputInterfaces->GetDeviceType(TOUCH_INDEX, devType);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's type failed, ret %d", __func__, ret);
    }

    HDF_LOGI("%s: device's type is %u", __func__, devType);
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, GetDeviceType)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0060
  * @tc.name: GetChipInfo001
  * @tc.desc: get input device chip info test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, GetChipInfo)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetChipInfo001 enter", __func__);
    int32_t ret;
    std::string chipInfo;

    for (auto _ : st) {
        ret = g_inputInterfaces->GetChipInfo(TOUCH_INDEX, chipInfo);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's chip info failed, ret %d", __func__, ret);
    }

    HDF_LOGI("%s: device's chip info is %s", __func__, chipInfo.c_str());
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, GetChipInfo)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0070
  * @tc.name: SetPowerStatus001
  * @tc.desc: set device power status test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, SetPowerStatus)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] SetPowerStatus001 enter", __func__);
    int32_t ret;
    uint32_t setStatus = INPUT_LOW_POWER;

    for (auto _ : st) {
        ret = g_inputInterfaces->SetPowerStatus(TOUCH_INDEX, setStatus);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: set device's power status failed, ret %d", __func__, ret);
    }
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, SetPowerStatus)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0080
  * @tc.name: GetPowerStatus001
  * @tc.desc: get device power status test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, GetPowerStatus)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetPowerStatus001 enter", __func__);
    int32_t ret;
    uint32_t getStatus = 0;

    for (auto _ : st) {
        ret = g_inputInterfaces->GetPowerStatus(TOUCH_INDEX, getStatus);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's power status failed, ret %d", __func__, ret);
    }

    HDF_LOGI("%s: device's power status is %u:", __func__, getStatus);
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, GetPowerStatus)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0090
  * @tc.name: GetVendorName001
  * @tc.desc: get device vendor name test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, GetVendorName)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetVendorName001 enter", __func__);
    int32_t ret;
    std::string vendorName;

    for (auto _ : st) {
        ret = g_inputInterfaces->GetVendorName(TOUCH_INDEX, vendorName);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's vendor name failed, ret %d", __func__, ret);
    }

    HDF_LOGI("%s: device's vendor name is %s:", __func__, vendorName.c_str());
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, GetVendorName)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0100
  * @tc.name: GetChipName001
  * @tc.desc: get device chip name test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, GetChipName)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetChipName001 enter", __func__);
    int32_t ret;
    std::string chipName;

    for (auto _ : st) {
        ret = g_inputInterfaces->GetChipName(TOUCH_INDEX, chipName);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's chip name failed, ret %d", __func__, ret);
    }

    HDF_LOGI("%s: device's chip name is %s", __func__, chipName.c_str());
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, GetChipName)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0110
  * @tc.name: SetGestureMode001
  * @tc.desc: set device gesture mode test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, SetGestureMode)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] SetGestureMode001 enter", __func__);
    int32_t ret;
    uint32_t gestureMode = 1;

    for (auto _ : st) {
        ret = g_inputInterfaces->SetGestureMode(TOUCH_INDEX, gestureMode);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: set device's gestureMode failed, ret %d", __func__, ret);
    }
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, SetGestureMode)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0120
  * @tc.name: RunCapacitanceTest001
  * @tc.desc: run capacitanceTest test-benchmark
  * @tc.type: FUNC
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, RunCapacitanceTest)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RunCapacitanceTest001 enter", __func__);
    int32_t ret;
    std::string result;
    uint32_t testType = MMI_TEST;

    for (auto _ : st) {
        ret = g_inputInterfaces->RunCapacitanceTest(TOUCH_INDEX, testType, result, TEST_RESULT_LEN);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: run capacitanceTest failed, ret %d", __func__, ret);
    }
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, RunCapacitanceTest)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0130
  * @tc.name: RunExtraCommand001
  * @tc.desc: run extra command test-benchmark
  * @tc.type: FUNC
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, RunExtraCommand)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RunExtraCommand001 enter", __func__);
    int32_t ret;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";

    for (auto _ : st) {
        ret = g_inputInterfaces->RunExtraCommand(TOUCH_INDEX, extraCmd);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: run extraCommand failed, ret %d", __func__, ret);
    }
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, RunExtraCommand)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0140
  * @tc.name: RegisterCallbackAndReportData001
  * @tc.desc: register callback and report data test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, RegisterReportCallback)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RegisterCallbackAndReportData001 enter", __func__);
    int32_t ret;

    for (auto _ : st) {
        ret = g_inputInterfaces->RegisterReportCallback(TOUCH_INDEX, g_callback);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: register callback failed for device 1, ret %d", __func__, ret);
    }
    OsalMSleep(KEEP_ALIVE_TIME_MS);
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, RegisterReportCallback)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_HdiInputPassthrough_0150
  * @tc.name: UnregisterReportCallback001
  * @tc.desc: unregister reportCallback test-benchmark
  * @tc.type: func
  * @tc.require:
  */
BENCHMARK_F(PassthroughBenchmarkTest, UnregisterReportCallback)(benchmark::State &st)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] UnregisterReportCallback001 enter", __func__);
    int32_t ret;

    for (auto _ : st) {
        ret  = g_inputInterfaces->UnregisterReportCallback(TOUCH_INDEX);
    }
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: unregister callback failed for device, ret %d", __func__, ret);
    }

    ret = g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: close device failed, ret %d", __func__, ret);
    }
}
BENCHMARK_REGISTER_F(PassthroughBenchmarkTest, UnregisterReportCallback)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

}
BENCHMARK_MAIN();