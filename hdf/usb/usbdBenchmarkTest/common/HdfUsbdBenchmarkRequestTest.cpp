/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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
#include "hdf_log.h"
#include "v1_2/iusb_interface.h"

using namespace benchmark::internal;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;
using namespace OHOS::HDI::Usb::V1_2;

const int SLEEP_TIME = 3;
const uint8_t INDEX_0 = 0;
const uint8_t INDEX_1 = 1;
const uint8_t CONFIG_ID_0 = 0;
const uint32_t MAX_BUFFER_LENGTH = 255;
const int TAG_NUM_10 = 10;
const uint8_t INTERFACEID_OK = 1;
// data interface have 2 point : 1->bulk_out 2->bulk_in
const uint8_t POINTID_DIR_IN = USB_ENDPOINT_DIR_IN | 2;
const uint32_t TIME_WAIT = 10000;
constexpr int32_t ITERATION_FREQUENCY = 100;
constexpr int32_t REPETITION_FREQUENCY = 3;
UsbDev HdfUsbdBenchmarkRequestTest::dev_ = { 0, 0 };

namespace {
sptr<OHOS::HDI::Usb::V1_2::IUsbInterface> g_usbInterface = nullptr;

int32_t SwitchErrCode(int32_t ret)
{
    return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret;
}

void HdfUsbdBenchmarkRequestTest::SetUp(const ::benchmark::State& state)
{
    g_usbInterface = OHOS::HDI::Usb::V1_2::IUsbInterface::Get();
    ASSERT_TRUE(g_usbInterface != nullptr);
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    sleep(SLEEP_TIME);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
}

void HdfUsbdBenchmarkRequestTest::TearDown(const ::benchmark::State& state){}

void HdfUsbdBenchmarkRequestTest::InitPara(const sptr<UsbSubscriberTest> &subscriber)
{
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber);
    ASSERT_EQ(0, ret);
    dev_ = {subscriber->busNum_, subscriber->devAddr_};
    ret = g_usbInterface->OpenDevice(dev_);
    ASSERT_EQ(0, ret);
}

void HdfUsbdBenchmarkRequestTest::ReleasePara(const sptr<UsbSubscriberTest> &subscriber)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    auto ret = g_usbInterface->UnbindUsbdSubscriber(subscriber);
    EXPECT_EQ(0, ret);
    ret = g_usbInterface->CloseDevice(dev_);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_0100
 * @tc.desc: Test functions to SetConfig benchmark test
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0100)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    uint8_t configIndex = INDEX_1;
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->SetConfig(dev, configIndex);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0100)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_0200
 * @tc.desc: Test functions to GetConfig benchmark test
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0200)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    uint8_t configIndex = INDEX_1;
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->GetConfig(dev, configIndex);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0200)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_0300
 * @tc.desc: Test functions to ClaimInterface benchmark test
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0300)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    uint8_t interfaceId = INTERFACEID_OK;
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0300)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_0400
 * @tc.desc: Test functions to SetInterface benchmark test
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0400)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    for (auto _ : st) {
        ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0400)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Performance_0300
 * @tc.desc: Test functions to GetDeviceDescriptor benchmark test
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_DeviceManager_HDI_Performance_0300)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_DeviceManager_HDI_Performance_0300)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_0500
 * @tc.desc: Test functions to GetStringDescriptor benchmark test
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0500)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    uint8_t stringId = 0;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0500)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_0600
 * @tc.desc: Test functions to GetConfigDescriptor benchmark test
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0600)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0600)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Performance_0400
 * @tc.desc: Test functions to GetRawDescriptor benchmark test
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_DeviceManager_HDI_Performance_0400)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    std::vector<uint8_t> rawData;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_DeviceManager_HDI_Performance_0400)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Performance_0500
 * @tc.desc: Test functions to GetFileDescriptor benchmark test
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_DeviceManager_HDI_Performance_0500)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    int32_t fd = 0;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->GetFileDescriptor(dev, fd);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_DeviceManager_HDI_Performance_0500)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_0700
 * @tc.desc: Test functions to RequestQueue benchmark test
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0700)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    auto req = -1;
    for (auto _ : st) {
        ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
        req = g_usbInterface->RequestCancel(dev, pipe);
    }
    ASSERT_EQ(0, ret);
    ASSERT_EQ(0, req);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0700)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_0800
 * @tc.desc: Test functions to RequestWait benchmark test
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0800)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    std::vector<uint8_t> waitData(TAG_NUM_10);
    auto req = -1;
    for (auto _ : st) {
        ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
        ret = g_usbInterface->RequestWait(dev, waitData, bufferData, TIME_WAIT);
        req = g_usbInterface->RequestCancel(dev, pipe);
    }
    ASSERT_EQ(0, ret);
    ASSERT_EQ(0, req);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0800)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_0900
 * @tc.desc: Test functions to RequestCancel benchmark test
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0900)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '1'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    EXPECT_EQ(0, ret);
    for (auto _ : st) {
        ret = g_usbInterface->RequestCancel(dev, pipe);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_0900)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_1000
 * @tc.desc: Test functions to ReleaseInterface benchmark test
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_1000)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
        ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    }
    ASSERT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_1000)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_2100
 * @tc.desc: Test functions to BulkCancel benchmark test
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_2100)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    for (auto _ : st) {
        ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
        ret = g_usbInterface->BulkCancel(dev, pipe);
    }
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    ASSERT_EQ(ret, 0);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, SUB_USB_HostManager_HDI_Performance_2100)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_2900
 * @tc.desc: Test functions to ClearHalt benchmark test
 * @tc.desc: int32_t ClearHalt(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, ClearHaltBenchmarkTest)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev_, interfaceId, 1);
    EXPECT_EQ(ret, 0);
    uint8_t pointId = POINTID_DIR_IN;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    EXPECT_EQ(0, ret);
    for (auto _ : st) {
        ret = g_usbInterface->ClearHalt(dev_, pipe);
    }
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, ClearHaltBenchmarkTest)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_3000
 * @tc.desc: Test functions to ResetDevice benchmark test
 * @tc.desc: int32_t ResetDevice(const UsbDev &dev)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, ResetDeviceBenchmarkTest)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->ResetDevice(dev_);
    }
    EXPECT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, ResetDeviceBenchmarkTest)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Performance_3100
 * @tc.name     : GetDeviceFileDescriptorBenchmarkTest
 * @tc.desc     : Test functions to GetDeviceFileDescriptor benchmark test
 * @tc.desc     : int32_t GetDeviceFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc     : Positive test: parameters correctly
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
BENCHMARK_F(HdfUsbdBenchmarkRequestTest, GetDeviceFileDescriptorBenchmarkTest)
(benchmark::State& st)
{
    ASSERT_TRUE(g_usbInterface != nullptr);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ASSERT_TRUE(subscriber != nullptr);
    InitPara(subscriber);
    struct UsbDev dev = dev_;
    int32_t fd = 0;
    auto ret = -1;
    for (auto _ : st) {
        ret = g_usbInterface->GetDeviceFileDescriptor(dev, fd);
    }
    EXPECT_EQ(0, ret);
    ReleasePara(subscriber);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkRequestTest, GetDeviceFileDescriptorBenchmarkTest)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

} // namespace

BENCHMARK_MAIN();
