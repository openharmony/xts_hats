/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include "v2_0/iusb_host_interface.h"
#include "v2_0/iusb_port_interface.h"

using namespace benchmark::internal;
using namespace std;
using namespace OHOS;
using namespace OHOS::USB;
using namespace OHOS::HDI::Usb::V2_0;

const int SLEEP_TIME = 3;
constexpr int32_t ITERATION_FREQUENCY = 100;
constexpr int32_t REPETITION_FREQUENCY = 3;
const uint8_t INTERFACEID_INVALID = 255;
const uint8_t INTERFACEID_OK_NEW = 0;

namespace {
sptr<IUsbHostInterface> g_usbHostInterface = nullptr;
sptr<IUsbPortInterface> g_usbPortInterface = nullptr;

struct UsbDev HdfUsbdBenchmarkDeviceTest::dev_ = { 0, 0 };

void HdfUsbdBenchmarkDeviceTest::SetUp(const ::benchmark::State& state)
{
    g_usbHostInterface = IUsbHostInterface::Get(true);
    g_usbPortInterface = IUsbPortInterface::Get();
    ASSERT_NE(g_usbHostInterface, nullptr);
    ASSERT_NE(g_usbPortInterface, nullptr);
    auto ret = g_usbPortInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    sleep(SLEEP_TIME);
    if (ret != 0) {
        ASSERT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    } else {
        ASSERT_EQ(0, ret);
    }
}

void HdfUsbdBenchmarkDeviceTest::TearDown(const ::benchmark::State& state){}

void HdfUsbdBenchmarkDeviceTest::InitPara(const sptr<UsbSubscriberTest> &subscriber)
{
    auto ret = g_usbHostInterface->BindUsbdHostSubscriber(subscriber);
    ASSERT_EQ(0, ret);
    dev_ = {subscriber->busNum_, subscriber->devAddr_};
    ret = g_usbHostInterface->OpenDevice(dev_);
    ASSERT_EQ(0, ret);
}

void HdfUsbdBenchmarkDeviceTest::ReleasePara(const sptr<UsbSubscriberTest> &subscriber)
{
    ASSERT_TRUE(g_usbHostInterface != nullptr);
    auto ret = g_usbHostInterface->UnbindUsbdHostSubscriber(subscriber);
    EXPECT_EQ(0, ret);
    ret = g_usbHostInterface->CloseDevice(dev_);
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
    ASSERT_NE(g_usbHostInterface, nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_NE(subscriber, nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbHostInterface->OpenDevice(dev);
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
    ASSERT_NE(g_usbHostInterface, nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_NE(subscriber, nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    int32_t ret;
    for (auto _ : st) {
        ret = g_usbHostInterface->OpenDevice(dev);
        ASSERT_EQ(0, ret);
        ret = g_usbHostInterface->CloseDevice(dev);
        ASSERT_EQ(0, ret);
    }
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HostManager_HDI_Performance_1200)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Performance_3100
 * @tc.name     : GetDeviceSpeedBenchmarkTest
 * @tc.desc     : int32_t GetDeviceSpeed(const UsbDev &dev, uint8_t interfaceId, uint8_t speed)
 * @tc.desc     : Positive test: parameters correctly
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
BENCHMARK_F(HdfUsbdBenchmarkDeviceTest, GetDeviceSpeedBenchmarkTest)
(benchmark::State& st)
{
    ASSERT_NE(g_usbHostInterface, nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_NE(subscriber, nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    int32_t ret = -1;
    uint8_t speed = 0;
    for (auto _ : st) {
        ret = g_usbHostInterface->GetDeviceSpeed(dev, speed);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkDeviceTest, GetDeviceSpeedBenchmarkTest)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Performance_3200
 * @tc.name     : GetInterfaceActiveStatusBenchmarkTest
 * @tc.desc     : int32_t GetInterfaceActiveStatus(const UsbDev &dev, uint8_t interfaceId, bool unactived)
 * @tc.desc     : Positive test: parameters correctly
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
BENCHMARK_F(HdfUsbdBenchmarkDeviceTest, GetInterfaceActiveStatusBenchmarkTest)
(benchmark::State& st)
{
    ASSERT_NE(g_usbHostInterface, nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_NE(subscriber, nullptr);
    InitPara(subscriber);
    uint8_t interfaceId = INTERFACEID_OK_NEW;
    struct UsbDev dev = dev_;
    int32_t ret = -1;
    bool unactived = 1;
    ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    for (auto _ : st) {
        for (; interfaceId < INTERFACEID_INVALID; interfaceId++) {
            ret = g_usbHostInterface->GetInterfaceActiveStatus(dev, interfaceId, unactived);
            if (ret == 0) {
                break;
            }
        }
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkDeviceTest, GetInterfaceActiveStatusBenchmarkTest)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

} // namespace

BENCHMARK_MAIN();
