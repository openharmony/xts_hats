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

#include "HdfUsbdBenchmarkDeviceTest.h"
#include "UsbSubscriberTest.h"
#include "hdf_log.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

using namespace benchmark::internal;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

const int SLEEP_TIME = 3;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

struct UsbDev HdfUsbdBenchmarkDeviceTest::dev_ = { 0, 0 };

void HdfUsbdBenchmarkDeviceTest::SetUp(const ::benchmark::State& state)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }

    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber) != HDF_SUCCESS) {
        exit(0);
    }
    dev_ = { subscriber->busNum_, subscriber->devAddr_ };
}

void HdfUsbdBenchmarkDeviceTest::TearDown(const ::benchmark::State& state) {}

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0010
 * @tc.desc: Test functions to OpenDevice benchmark test
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HDI_Benchmark_0010)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->OpenDevice(dev);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HDI_Benchmark_0010)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0020
 * @tc.desc: Test functions to CloseDevice benchmark test
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HDI_Benchmark_0020)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    ASSERT_EQ(0, ret);
    for (auto _ : st) {
        ret = g_usbInterface->CloseDevice(dev);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HDI_Benchmark_0020)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();
} // namespace

BENCHMARK_MAIN();
