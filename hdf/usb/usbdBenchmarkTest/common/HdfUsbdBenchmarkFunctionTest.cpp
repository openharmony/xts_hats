/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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
#include <iostream>
#include <string>
#include <vector>

#include "HdfUsbdBenchmarkFunctionTest.h"
#include "hdf_log.h"
#include "if_system_ability_manager.h"
#include "system_ability_definition.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

using namespace benchmark::internal;
using namespace OHOS;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

constexpr int32_t SLEEP_TIME = 3;
constexpr int32_t ITERATION_FREQUENCY = 100;
constexpr int32_t REPETITION_FREQUENCY = 3;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

int32_t SwitchErrCode(int32_t ret)
{
    return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret;
}

void HdfUsbdBenchmarkFunctionTest::SetUp(const ::benchmark::State& state)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SINK, DATA_ROLE_DEVICE);
    sleep(SLEEP_TIME);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }
}

void HdfUsbdBenchmarkFunctionTest::TearDown(const ::benchmark::State& state) {}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Performance_0100
 * @tc.desc: Test functions to GetCurrentFunctions benchmark test
 * @tc.desc: int32_t GetCurrentFunctions(int32_t &funcs);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_DeviceManager_HDI_Performance_0100)
(benchmark::State& st)
{
    int32_t funcs = USB_FUNCTION_NONE;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->GetCurrentFunctions(funcs);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_DeviceManager_HDI_Performance_0100)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Performance_0200
 * @tc.desc: Test functions to SetCurrentFunctions benchmark test
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_DeviceManager_HDI_Performance_0200)
(benchmark::State& st)
{
    int32_t funcs = USB_FUNCTION_ACM;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->SetCurrentFunctions(funcs);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_DeviceManager_HDI_Performance_0200)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_PortManager_HDI_Performance_0100
 * @tc.desc: Test functions to SetPortRole benchmark test
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_PortManager_HDI_Performance_0100)
(benchmark::State& st)
{
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    }
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_PortManager_HDI_Performance_0100)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_PortManager_HDI_Performance_0200
 * @tc.desc: Test functions to QueryPort benchmark test
 * @tc.desc: int32_t QueryPort(int32_t &portId, int32_t &powerRole, int32_t &dataRole, int32_t &mode);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_PortManager_HDI_Performance_0200)
(benchmark::State& st)
{
    int32_t portId = DEFAULT_PORT_ID;
    int32_t powerRole = POWER_ROLE_NONE;
    int32_t dataRole = DATA_ROLE_NONE;
    int32_t mode = PORT_MODE_NONE;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_PortManager_HDI_Performance_0200)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();
} // namespace

BENCHMARK_MAIN();
