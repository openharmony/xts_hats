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

#include "HdfUsbdBenchmarkRequestTest.h"
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
const uint8_t INDEX_0 = 0;
const uint8_t INDEX_1 = 1;
const uint8_t CONFIG_ID_0 = 0;
const uint32_t LENGTH_NUM_255 = 255;
const uint32_t TAG_LENGTH_NUM_1000 = 1000;
const int TAG_NUM_10 = 10;
const uint8_t INTERFACEID_1 = 1;
const uint8_t POINTID_129 = 130;
const uint8_t POINTID_BULK_IN = 0x82;
UsbDev HdfUsbdBenchmarkRequestTest::dev_ = { 0, 0 };

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

void HdfUsbdBenchmarkRequestTest::SetUp(const ::benchmark::State& state)
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

void HdfUsbdBenchmarkRequestTest::TearDown(const ::benchmark::State& state)
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
 * @tc.name: SUB_USB_HDI_Benchmark_0070
 * @tc.desc: Test functions to SetConfig benchmark test
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0070)
(benchmark::State& st)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = dev_;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->SetConfig(dev, configIndex);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0070)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0080
 * @tc.desc: Test functions to GetConfig benchmark test
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0080)
(benchmark::State& st)
{
    uint8_t configIndex = 1;
    struct UsbDev dev = dev_;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->GetConfig(dev, configIndex);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0080)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0090
 * @tc.desc: Test functions to ClaimInterface benchmark test
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0090)
(benchmark::State& st)
{
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = dev_;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0090)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0100
 * @tc.desc: Test functions to SetInterface benchmark test
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0100)
(benchmark::State& st)
{
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    for (auto _ : st) {
        ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0100)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0110
 * @tc.desc: Test functions to GetDeviceDescriptor benchmark test
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0110)
(benchmark::State& st)
{
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->GetDeviceDescriptor(dev, devdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0110)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0120
 * @tc.desc: Test functions to GetStringDescriptor benchmark test
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0120)
(benchmark::State& st)
{
    uint8_t stringId = 0;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->GetStringDescriptor(dev, stringId, devdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0120)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0130
 * @tc.desc: Test functions to GetConfigDescriptor benchmark test
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0130)
(benchmark::State& st)
{
    uint8_t configId = CONFIG_ID_0;
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->GetConfigDescriptor(dev, configId, devdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0130)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0140
 * @tc.desc: Test functions to GetRawDescriptor benchmark test
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0140)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> rawData;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0140)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0150
 * @tc.desc: Test functions to GetFileDescriptor benchmark test
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0150)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    int32_t fd = 0;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->GetFileDescriptor(dev, fd);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0150)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0160
 * @tc.desc: Test functions to RequestQueue benchmark test
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0160)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    for (auto _ : st) {
        ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0160)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0170
 * @tc.desc: Test functions to RequestWait benchmark test
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0170)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    ASSERT_EQ(0, ret);
    uint8_t* clientObj = new uint8_t[10];
    std::vector<uint8_t> waitdata = { clientObj, clientObj + 10 };
    for (auto _ : st) {
        ret = g_usbInterface->RequestWait(dev, waitdata, bufferdata, 10000);
    }
    ASSERT_EQ(0, ret);
    delete[] clientObj;
    clientObj = nullptr;
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0170)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0180
 * @tc.desc: Test functions to RequestCancel benchmark test
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0180)
(benchmark::State& st)
{
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "request001";
    uint32_t length = LENGTH_NUM_255;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    EXPECT_TRUE(ret == 0);
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    ASSERT_EQ(0, ret);
    for (auto _ : st) {
        ret = g_usbInterface->RequestCancel(dev, pipe);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0180)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0190
 * @tc.desc: Test functions to ReleaseInterface benchmark test
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0190)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0190)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0280
 * @tc.desc: Test functions to BulkCancel benchmark test
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0280)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_BULK_IN;
    struct UsbPipe pipe = {interfaceId, pointid};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    ASSERT_EQ(ret, 0);
    for (auto _ : st) {
        ret = g_usbInterface->BulkCancel(dev, pipe);
    }
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    ASSERT_EQ(ret, 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HDI_Benchmark_0280)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();
} // namespace

BENCHMARK_MAIN();
