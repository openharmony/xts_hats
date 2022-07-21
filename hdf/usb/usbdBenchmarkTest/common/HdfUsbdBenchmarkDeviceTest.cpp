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

#include <iostream>
#include <vector>
#include "usbd_device_test.h"
#include "hdf_log.h"
#include "usbd_client.h"
#include "UsbSubscriberTest.h"
#include "usb_errors.h"

using namespace benchmark::internal;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;

const int SLEEP_TIME = 3;

struct UsbDev HdfUsbdBenchmarkDeviceTest::dev_ = {0, 0};

void HdfUsbdBenchmarkDeviceTest::SetUp(const ::benchmark::State &state)
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
}

void HdfUsbdBenchmarkDeviceTest::TearDown(const ::benchmark::State &state) {}

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0010
 * @tc.desc: Test functions to OpenDevice benchmark test
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Forward test: correct parameters
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HDI_Benchmark_0010)(benchmark::State &st)
{
    struct UsbDev dev = dev_;
    auto ret = 0;
    for (auto _ : st) {
        ret = UsbdClient::GetInstance().OpenDevice(dev);
    }
    ASSERT_TRUE(ret == 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HDI_Benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: SUB_USB_HDI_Benchmark_0020
 * @tc.desc: Test functions to CloseDevice benchmark test
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Forward test: correct parameters
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HDI_Benchmark_0020)(benchmark::State &st)
{
    struct UsbDev dev = dev_;
    auto ret = UsbdClient::GetInstance().OpenDevice(dev);
    ASSERT_TRUE(ret == 0);
    for (auto _ : st) {
        ret = UsbdClient::GetInstance().CloseDevice(dev);
    }
    ASSERT_TRUE(ret == 0);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkDeviceTest, SUB_USB_HDI_Benchmark_0020)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_MAIN();
