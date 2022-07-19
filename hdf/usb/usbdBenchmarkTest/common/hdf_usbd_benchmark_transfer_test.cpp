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
#include <string>
#include <vector>

#include "usbd_transfer_test.h"
#include <iostream>
#include <vector>
#include "hdf_log.h"
#include "usbd_client.h"
#include "UsbSubscriberTest.h"
#include "usb_errors.h"
#include "usb_param.h"

using namespace benchmark::internal;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;

const int SLEEP_TIME = 3;
uint8_t BUS_NUM_1 = 0;
uint8_t DEV_ADDR_2 = 0;
const uint32_t LENGTH_NUM_255 = 255;
const uint8_t INTERFACEID_1 = 1;
const uint8_t POINTID_1 = 1;
const uint8_t POINTID_129 = 130;

struct UsbDev HdfUsbdBenchmarkTransferTest::dev_ = {0, 0};

void HdfUsbdBenchmarkTransferTest::SetUp(const ::benchmark::State &state)
{
    auto ret = UsbdClient::GetInstance().SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    ASSERT_TRUE(ret == 0);
    if (ret != 0) {
        exit(0);
    }

    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (UsbdClient::GetInstance().BindUsbdSubscriber(subscriber) != UEC_OK) {
        exit(0);
    }
    dev_ = {subscriber->busNum_, subscriber->devAddr_};
    ret = UsbdClient::GetInstance().OpenDevice(dev_);
    ASSERT_TRUE(ret == 0);
}

void HdfUsbdBenchmarkTransferTest::TearDown(const ::benchmark::State &state) 
{
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (UsbdClient::GetInstance().BindUsbdSubscriber(subscriber) != UEC_OK) {
        exit(0);
    }
    dev_ = {subscriber->busNum_, subscriber->devAddr_};
    auto ret = UsbdClient::GetInstance().CloseDevice(dev_);
    ASSERT_TRUE(ret == 0);
}

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0200
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to ControlTransfer(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Forward test: correct parameters
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0200)(benchmark::State &st)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = {0};
    uint32_t length = LENGTH_NUM_255;
    std::vector<uint8_t> bufferdata = {buffer, buffer + length};
    struct UsbCtrlTransfer ctrlparmas = {0b10000000, 8, 0, 0, 1000};
    auto ret = 0;
    for (auto _ : st) {
        ret = UsbdClient::GetInstance().ControlTransfer(dev, ctrlparmas, bufferdata);
    }
    ASSERT_TRUE(ret == 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0200)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0210
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0210)(benchmark::State &st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = UsbdClient::GetInstance().ClaimInterface(dev, interfaceId, true);
    ASSERT_TRUE(ret == 0);
    uint8_t buffer[LENGTH_NUM_255] = {0};
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = {interfaceId, pointid};
    std::vector<uint8_t> bufferdata = {buffer, buffer + length};
    for (auto _ : st) {
        ret = UsbdClient::GetInstance().BulkTransferRead(dev, pipe, 1000, bufferdata);
    }
    ASSERT_TRUE(ret == 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0210)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0220
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0220)(benchmark::State &st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = UsbdClient::GetInstance().ClaimInterface(dev, interfaceId, true);
    ASSERT_TRUE(ret == 0);
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ01";
    struct UsbPipe pipe = {interfaceId, pointid};
    std::vector<uint8_t> bufferdata = {buffer, buffer + length};
    for (auto _ : st) {
        ret = UsbdClient::GetInstance().BulkTransferWrite(dev, pipe, 1000, bufferdata);
    }
    ASSERT_TRUE(ret == 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0220)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0230
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0230)(benchmark::State &st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = UsbdClient::GetInstance().ClaimInterface(dev, interfaceId, true);
    ASSERT_TRUE(ret == 0);
    uint8_t buffer[LENGTH_NUM_255] = {0};
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = {interfaceId, pointid};
    std::vector<uint8_t> bufferdata = {buffer, buffer + length};
    for (auto _ : st) {
        ret = UsbdClient::GetInstance().InterruptTransferRead(dev, pipe, 1000, bufferdata);
    }
    ASSERT_TRUE(ret == 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0230)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0240
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0240)(benchmark::State &st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = UsbdClient::GetInstance().ClaimInterface(dev, interfaceId, true);
    ASSERT_TRUE(ret == 0);
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ01";
    struct UsbPipe pipe = {interfaceId, pointid};
    std::vector<uint8_t> bufferdata = {buffer, buffer + length};
    for (auto _ : st) {
        ret = UsbdClient::GetInstance().InterruptTransferWrite(dev, pipe, 1000, bufferdata);
    }
    ASSERT_TRUE(ret == 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0240)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0250
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0250)(benchmark::State &st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = UsbdClient::GetInstance().ClaimInterface(dev, interfaceId, true);
    ASSERT_TRUE(ret == 0);
    uint8_t buffer[LENGTH_NUM_255] = {0};
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = {interfaceId, pointid};
    std::vector<uint8_t> bufferdata = {buffer, buffer + length};
    for (auto _ : st) {
        ret = UsbdClient::GetInstance().IsoTransferRead(dev, pipe, 1000, bufferdata);
    }
    ASSERT_TRUE(ret == 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0250)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0260
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0260)(benchmark::State &st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = UsbdClient::GetInstance().ClaimInterface(dev, interfaceId, true);
    ASSERT_TRUE(ret == 0);
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ01";
    struct UsbPipe pipe = {interfaceId, pointid};
    std::vector<uint8_t> bufferdata = {buffer, buffer + length};
    for (auto _ : st) {
        ret = UsbdClient::GetInstance().IsoTransferWrite(dev, pipe, 1000, bufferdata);
    }
    ASSERT_TRUE(ret == 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0260)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_MAIN();