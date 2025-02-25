/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include <iostream>

#include "UsbSubscriberTest.h"
#include "hdf_log.h"
#include "usbd_request_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

using OHOS::HDI::Usb::V1_0::UsbDev;
using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

namespace {
const uint8_t INDEX_0 = 0;
const uint8_t INDEX_1 = 1;
const uint8_t INDEX_INVALID = 255;
const uint8_t BUS_NUM_INVALID = 255;
const uint8_t DEV_ADDR_INVALID = 255;
const uint8_t INTERFACEID_OK = 1;
const uint8_t INTERFACEID_INVALID = 255;
const int SLEEP_TIME = 3;

class UsbdInterfaceTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

    static UsbDev dev_;
    static OHOS::sptr<OHOS::USB::UsbSubscriberTest> subscriber_;
};
UsbDev UsbdInterfaceTest::dev_ = {0, 0};
OHOS::sptr<OHOS::USB::UsbSubscriberTest> UsbdInterfaceTest::subscriber_ = nullptr;

sptr<IUsbInterface> g_usbInterface = nullptr;

void UsbdInterfaceTest::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdInterfaceTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    if (ret != 0) {
        ASSERT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    } else {
        ASSERT_EQ(0, ret);
    }

    subscriber_ = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber_) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber_ failed", __func__);
        exit(0);
    }

    std::cout << "please connect device, press enter to continue" << std::endl;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};

    ret = g_usbInterface->OpenDevice(dev_);
    ASSERT_EQ(0, ret);
    HDF_LOGI("UsbdInterfaceTest:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ret = g_usbInterface->ClaimInterface(dev_, 1, 1);
    ASSERT_EQ(0, ret);
}

void UsbdInterfaceTest::TearDownTestCase(void)
{
    g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};
    auto ret = g_usbInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdInterfaceTest:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdInterfaceTest::SetUp(void) {}

void UsbdInterfaceTest::TearDown(void) {}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_0900
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdInterfaceTest, SUB_USB_HostManager_HDI_Func_0900, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdInterfaceTest::SUB_USB_HostManager_HDI_Func_0900 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4700
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdInterfaceTest, SUB_USB_HostManager_HDI_Compatibility_4700, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    ;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdInterfaceTest::SUB_USB_HostManager_HDI_Compatibility_4700 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4800
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdInterfaceTest, SUB_USB_HostManager_HDI_Compatibility_4800, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_INVALID;
    struct UsbDev dev = dev_;
    dev.devAddr = DEV_ADDR_INVALID;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdInterfaceTest::SUB_USB_HostManager_HDI_Compatibility_4800 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4900
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdInterfaceTest, SUB_USB_HostManager_HDI_Compatibility_4900, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    uint8_t altIndex = INDEX_INVALID;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdInterfaceTest::SUB_USB_HostManager_HDI_Compatibility_4900 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5000
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdInterfaceTest, SUB_USB_HostManager_HDI_Compatibility_5000, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdInterfaceTest::SUB_USB_HostManager_HDI_Compatibility_5000 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5100
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdInterfaceTest, SUB_USB_HostManager_HDI_Compatibility_5100, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_INVALID;
    uint8_t altIndex = INDEX_1;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdInterfaceTest::SUB_USB_HostManager_HDI_Compatibility_5100 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5200
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdInterfaceTest, SUB_USB_HostManager_HDI_Compatibility_5200, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_INVALID;
    uint8_t altIndex = INDEX_INVALID;
    struct UsbDev dev = dev_;
    dev.devAddr = DEV_ADDR_INVALID;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdInterfaceTest::SUB_USB_HostManager_HDI_Compatibility_5200 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5300
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdInterfaceTest, SUB_USB_HostManager_HDI_Compatibility_5300, Function | MediumTest | Level1)
{
    uint8_t altIndex = INDEX_INVALID;
    int32_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdInterfaceTest::SUB_USB_HostManager_HDI_Compatibility_5300 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}
} // namespace