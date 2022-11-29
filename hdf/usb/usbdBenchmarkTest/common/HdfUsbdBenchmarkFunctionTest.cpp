/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

const int SLEEP_TIME = 3;
const int TEST_PORT_ID = 1;
const int TEST_POWER_ROLE = 2;
const int TEST_DATAR_ROLE = 2;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

void HdfUsbdBenchmarkFunctionTest::SetUp(const ::benchmark::State& state)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(TEST_PORT_ID, TEST_POWER_ROLE, TEST_DATAR_ROLE);
    sleep(SLEEP_TIME);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }
}

void HdfUsbdBenchmarkFunctionTest::TearDown(const ::benchmark::State& state) {}

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0030
 * @tc.desc: Test functions to GetCurrentFunctions benchmark test
 * @tc.desc: int32_t GetCurrentFunctions(int32_t &funcs);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_HDI_Benchmark_0030)
(benchmark::State& st)
{
    int32_t funcs = 0;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->GetCurrentFunctions(funcs);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_HDI_Benchmark_0030)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0040
 * @tc.desc: Test functions to SetCurrentFunctions benchmark test
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_HDI_Benchmark_0040)
(benchmark::State& st)
{
    int32_t funcs = 1;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->SetCurrentFunctions(funcs);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_HDI_Benchmark_0040)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0050
 * @tc.desc: Test functions to SetPortRole benchmark test
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_HDI_Benchmark_0050)
(benchmark::State& st)
{
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->SetPortRole(1, 1, 1);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_HDI_Benchmark_0050)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0060
 * @tc.desc: Test functions to QueryPort benchmark test
 * @tc.desc: int32_t QueryPort(int32_t &portId, int32_t &powerRole, int32_t &dataRole, int32_t &mode);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_HDI_Benchmark_0060)
(benchmark::State& st)
{
    int32_t portId = 0;
    int32_t powerRole = 0;
    int32_t dataRole = 0;
    int32_t mode = 0;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkFunctionTest, SUB_USB_HDI_Benchmark_0060)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();
} // namespace

BENCHMARK_MAIN();
