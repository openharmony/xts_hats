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
#include <iostream>
#include <string>
#include <vector>

#include "HdfUsbdBenchmarkDeviceTest.h"
#include "hdf_log.h"
#include "v1_0/iusb_interface.h"

using namespace benchmark::internal;
using namespace std;
using namespace OHOS;
using namespace OHOS::USB;
using namespace OHOS::HDI::Usb::V1_0;

const int SLEEP_TIME = 3;
constexpr int32_t ITERATION_FREQUENCY = 100;
constexpr int32_t REPETITION_FREQUENCY = 3;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

struct UsbDev HdfUsbdBenchmarkDeviceTest::dev_ = { 0, 0 };

int32_t SwitchErrCode(int32_t ret)
{
    return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret;
}

void HdfUsbdBenchmarkDeviceTest::SetUp(const ::benchmark::State& state)
{
    g_usbInterface = IUsbInterface::Get();
    ASSERT_NE(g_usbInterface, nullptr);
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    sleep(SLEEP_TIME);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
}

void HdfUsbdBenchmarkDeviceTest::TearDown(const ::benchmark::State& state){}

void HdfUsbdBenchmarkDeviceTest::InitPara(const sptr<UsbSubscriberTest> &subscriber)
{
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber);
    ASSERT_EQ(0, ret);
    dev_ = {subscriber->busNum_, subscriber->devAddr_};
    ret = g_usbInterface->OpenDevice(dev_);
    ASSERT_EQ(0, ret);
}

void HdfUsbdBenchmarkDeviceTest::ReleasePara(const sptr<UsbSubscriberTest> &subscriber)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    auto ret = g_usbInterface->UnbindUsbdSubscriber(subscriber);
    EXPECT_EQ(0, ret);
    ret = g_usbInterface->CloseDevice(dev_);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_1100
 * @tc.desc: Test functions to OpenDevice benchmark test
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HostManager_HDI_Performance_1100)
(benchmark::State& st)
{
    ASSERT_NE(g_usbInterface, nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_NE(subscriber, nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->OpenDevice(dev);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HostManager_HDI_Performance_1100)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_1200
 * @tc.desc: Test functions to CloseDevice benchmark test
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HostManager_HDI_Performance_1200)
(benchmark::State& st)
{
    ASSERT_NE(g_usbInterface, nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_NE(subscriber, nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    int32_t ret;
    for (auto _ : st) {
        ret = g_usbInterface->OpenDevice(dev);
        ASSERT_EQ(0, ret);
        ret = g_usbInterface->CloseDevice(dev);
        ASSERT_EQ(0, ret);
    }
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HostManager_HDI_Performance_1200)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();
} // namespace

BENCHMARK_MAIN();
