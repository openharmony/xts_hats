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

#include "HdfUsbdBenchmarkManagerInterfaceTest.h"
#include "hdf_log.h"
#include "v2_0/iusb_host_interface.h"
#include "v2_0/iusb_port_interface.h"

using namespace benchmark::internal;
using namespace std;
using namespace OHOS;
using namespace OHOS::USB;
using namespace OHOS::HDI::Usb::V2_0;

const int SLEEP_TIME = 3;
const uint8_t INTERFACEID_OK_NEW = 0;
const uint8_t INTERFACEID_INVALID = 255;
constexpr int32_t ITERATION_FREQUENCY = 100;
constexpr int32_t REPETITION_FREQUENCY = 3;

namespace {
UsbDev HdfUsbdBenchmarkManagerInterfaceTest::dev_ = {0, 0};
sptr<UsbSubscriberTest> HdfUsbdBenchmarkManagerInterfaceTest::subscriber_ = nullptr;
sptr<IUsbHostInterface> g_usbHostInterface = nullptr;
sptr<IUsbPortInterface> g_usbPortInterface = nullptr;

void HdfUsbdBenchmarkManagerInterfaceTest::SetUp(const ::benchmark::State& state)
{
    g_usbHostInterface = IUsbHostInterface::Get(true);
    g_usbPortInterface = IUsbPortInterface::Get();
    if (g_usbHostInterface == nullptr || g_usbPortInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbPortInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    sleep(SLEEP_TIME);
    HDF_LOGI("HdfUsbdBenchmarkManagerInterfaceTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    if (ret != 0) {
        ASSERT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    } else {
        ASSERT_EQ(0, ret);
    }

    subscriber_ = new UsbSubscriberTest();
    if (g_usbHostInterface->BindUsbdHostSubscriber(subscriber_) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber_ failed", __func__);
        exit(0);
    }

    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};

    ret = g_usbHostInterface->OpenDevice(dev_);
    HDF_LOGI("UsbdManageInterfaceTest:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void HdfUsbdBenchmarkManagerInterfaceTest::TearDown(const ::benchmark::State& state)
{
    g_usbHostInterface->UnbindUsbdHostSubscriber(subscriber_);
    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};
    auto ret = g_usbHostInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdManageInterfaceTest:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Performance_2800
 * @tc.desc: Test functions to OpenDevice benchmark test
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
BENCHMARK_F(HdfUsbdBenchmarkManagerInterfaceTest, SUB_USB_HostManager_HDI_Performance_2800)
(benchmark::State& st)
{
    uint8_t interfaceId = INTERFACEID_OK_NEW;
    struct UsbDev dev = dev_;
    int32_t ret = -1;
    for (auto _ : st) {
        for (; interfaceId < INTERFACEID_INVALID; interfaceId++) {
            ret = g_usbHostInterface->ManageInterface(dev, interfaceId, true);
            if (ret == 0) {
                break;
            }
        }
    }
    ASSERT_EQ(0, ret);
}

BENCHMARK_REGISTER_F(HdfUsbdBenchmarkManagerInterfaceTest, SUB_USB_HostManager_HDI_Performance_2800)
    ->Iterations(ITERATION_FREQUENCY)
    ->Repetitions(REPETITION_FREQUENCY)
    ->ReportAggregatesOnly();

} // namespace

BENCHMARK_MAIN();
