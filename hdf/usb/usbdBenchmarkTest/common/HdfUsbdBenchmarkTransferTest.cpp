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
#include "securec.h"
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
const uint8_t POINTID_BULK_IN = 0x82;
const uint8_t POINTID_BULK_OUT = 0x01;
const int32_t ASHMEM_MAX_SIZE = 1024;
const uint8_t SAMPLE_DATA_1 = 1;
const uint8_t SAMPLE_DATA_2 = 2;
const uint8_t SAMPLE_DATA_3 = 3;
UsbDev HdfUsbdBenchmarkTransferTest::dev_ = { 0, 0 };

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

int32_t InitAshmemOne(sptr<Ashmem>& asmptr, int32_t asmSize, uint8_t rflg)
{
    asmptr = Ashmem::CreateAshmem("ttashmem000", asmSize);
    if (asmptr == nullptr) {
        return HDF_FAILURE;
    }

    asmptr->MapReadAndWriteAshmem();

    if (rflg == 0) {
        uint8_t tdata[ASHMEM_MAX_SIZE];
        int32_t offset = 0;
        int32_t tlen = 0;

        int32_t retSafe = memset_s(tdata, sizeof(tdata), 'Y', ASHMEM_MAX_SIZE);
        if (retSafe != EOK) {
            return HDF_FAILURE;
        }
        while (offset < asmSize) {
            tlen = (asmSize - offset) < ASHMEM_MAX_SIZE ? (asmSize - offset) : ASHMEM_MAX_SIZE;
            asmptr->WriteToAshmem(tdata, tlen, offset);
            offset += tlen;
        }
    }
    return HDF_SUCCESS;
}

void HdfUsbdBenchmarkTransferTest::SetUp(const ::benchmark::State& state)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        exit(0);
    }
    const int32_t DEFAULT_PORT_ID = 1;
    const int32_t DEFAULT_ROLE_HOST = 1;
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, DEFAULT_ROLE_HOST, DEFAULT_ROLE_HOST);
    sleep(SLEEP_TIME);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }

    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (subscriber == nullptr) {
        exit(0);
    }
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
    if (subscriber == nullptr) {
        exit(0);
    }
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
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, standard request: get configuration
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
 * @tc.name: SUB_USB_HDI_Benchmark_0270
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0270)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {0b0001, 8, 0, 0, 1000};
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0270)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0210
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0210)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
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
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0220)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
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
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0230)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
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
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0240)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
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
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0250)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
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
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0260)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
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

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0290
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0290)
(benchmark::State& st)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    for (auto _ : st) {
        ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    }
    ASSERT_EQ(ret, 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0290)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0300
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0300)
(benchmark::State& st)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    for (auto _ : st) {
        ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    }
    ASSERT_EQ(ret, 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0300)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0310
 * @tc.desc: Benchmark test
 * @tc.desc: int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const sptr<IUsbdBulkCallback> &cb)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0310)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_BULK_OUT;
    struct UsbPipe pipe = {interfaceId, pointid};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        exit(0);
    }
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    }
    ASSERT_EQ(ret, 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0310)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0320
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0320)
(benchmark::State& st)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = {interfaceId, pointid};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        exit(0);
    }
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    ASSERT_EQ(ret, 0);
    for (auto _ : st) {
        ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    }
    ASSERT_EQ(ret, 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0320)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0330
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0330)
(benchmark::State& st)
{
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (subscriber == nullptr) {
        exit(0);
    }
    auto ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->BindUsbdSubscriber(subscriber);
    }
    ASSERT_EQ(ret, 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0330)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0340
 * @tc.desc: Benchmark test
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0340)
(benchmark::State& st)
{
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (subscriber == nullptr) {
        exit(0);
    }
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber);
    ASSERT_EQ(0, ret);
    ret = 0;
    for (auto _ : st) {
        ret = g_usbInterface->UnbindUsbdSubscriber(subscriber);
    }
    ASSERT_EQ(ret, 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkTransferTest, SUB_USB_HDI_Benchmark_0340)
    ->Iterations(100)
    ->Repetitions(3)
    ->ReportAggregatesOnly();
} // namespace

BENCHMARK_MAIN();
