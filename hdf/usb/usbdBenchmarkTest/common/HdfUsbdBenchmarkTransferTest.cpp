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

#include "UsbSubscriberTest.h"
#include "hdf_log.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"
#include "HdfUsbdBenchmarkTransferTest.h"

using namespace benchmark::internal;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

const int SLEEP_TIME = 3;
uint8_t BUS_NUM_1 = 0;
uint8_t DEV_ADDR_2 = 0;
const uint32_t LENGTH_NUM_255 = 255;
const uint8_t INTERFACEID_1 = 1;
const uint8_t POINTID_1 = 1;
const uint8_t POINTID_129 = 130;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;
}

struct UsbDev HdfUsbdBenchmarkTransferTest::dev_ = { 0, 0 };

void HdfUsbdBenchmarkTransferTest::SetUp(const ::benchmark::State& state)
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
    ret = g_usbInterface->OpenDevice(dev_);
    ASSERT_EQ(0, ret);
}

void HdfUsbdBenchmarkTransferTest::TearDown(const ::benchmark::State& state)
{
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber) != HDF_SUCCESS) {
        exit(0);
    }
    dev_ = { subscriber->busNum_, subscriber->devAddr_ };
    auto ret = g_usbInterface->CloseDevice(dev_);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0200
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Forward test: correct parameters
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0200)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 8, 0, 0, 1000 };
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0200)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0210
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0210)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    for (auto _ : st) {
        ret = g_usbInterface->BulkTransferRead(dev, pipe, 1000, bufferdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0210)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0220
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0220)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    ASSERT_EQ(0, ret);
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ01";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    for (auto _ : st) {
        ret = g_usbInterface->BulkTransferWrite(dev, pipe, 1000, bufferdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0220)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0230
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0230)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    for (auto _ : st) {
        ret = g_usbInterface->InterruptTransferRead(dev, pipe, 1000, bufferdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0230)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0240
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0240)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    ASSERT_EQ(0, ret);
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ01";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    for (auto _ : st) {
        ret = g_usbInterface->InterruptTransferWrite(dev, pipe, 1000, bufferdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0240)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0250
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0250)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    for (auto _ : st) {
        ret = g_usbInterface->IsoTransferRead(dev, pipe, 1000, bufferdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0250)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0260
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0260)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    ASSERT_EQ(0, ret);
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ01";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    for (auto _ : st) {
        ret = g_usbInterface->IsoTransferWrite(dev, pipe, 1000, bufferdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0260)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

BENCHMARK_MAIN();