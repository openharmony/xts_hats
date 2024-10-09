/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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
#include <vector>

#include "hdf_log.h"
#include "usbd_device_test.h"
#include "v1_1/iusb_interface.h"
#include "v1_1/usb_types.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;
using namespace OHOS::HDI::Usb::V1_1;

const int SLEEP_TIME = 3;
const uint8_t BUS_NUM_INVALID = 255;
const uint8_t DEV_ADDR_INVALID = 255;
sptr<UsbSubscriberTest> UsbdDeviceTest::subscriber_ = nullptr;

namespace {
sptr<OHOS::HDI::Usb::V1_1::IUsbInterface> g_usbInterface = nullptr;

struct UsbDev UsbdDeviceTest::dev_ = { 0, 0 };

int32_t SwitchErrCode(int32_t ret)
{
    return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret;
}

void UsbdDeviceTest::SetUpTestCase(void)
{
    g_usbInterface = OHOS::HDI::Usb::V1_1::IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdDeviceTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }

    subscriber_ = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber_) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber_ failed\n", __func__);
        exit(0);
    }
    dev_ = { subscriber_->busNum_, subscriber_->devAddr_ };

    std::cout << "please connect device, press enter to continue" << std::endl;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void UsbdDeviceTest::TearDownTestCase(void)
{
    g_usbInterface->UnbindUsbdSubscriber(subscriber_);
}

void UsbdDeviceTest::SetUp(void) {}

void UsbdDeviceTest::TearDown(void) {}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_0100
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HostManager_HDI_Func_0100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result =%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_0100
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HostManager_HDI_Compatibility_0100, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_0200
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HostManager_HDI_Compatibility_0200, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_0300
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HostManager_HDI_Compatibility_0300, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_1400
 * @tc.desc: Test functions to CloseDevice
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HostManager_HDI_Func_1400, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_7000
 * @tc.desc: Test functions to CloseDevice
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HostManager_HDI_Compatibility_7000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    g_usbInterface->CloseDevice(dev);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_7100
 * @tc.desc: Test functions to CloseDevice
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HostManager_HDI_Compatibility_7100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    g_usbInterface->CloseDevice(dev);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_7200
 * @tc.desc: Test functions to CloseDevice
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HostManager_HDI_Compatibility_7200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    g_usbInterface->CloseDevice(dev);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Func_1900
 * @tc.name     : UsbdResetDevice001
 * @tc.desc     : int32_t ResetDevice(const UsbDev &dev)
 * @tc.desc     : Positive test: parameters correctly
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdDeviceTest, UsbdResetDevice001, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->ResetDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d UsbdResetDevice001 result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_9000
 * @tc.name     : UsbdResetDevice002
 * @tc.desc     : int32_t ResetDevice(const UsbDev &dev)
 * @tc.desc     : Negative test
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdDeviceTest, UsbdResetDevice002, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    ret = g_usbInterface->ResetDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d UsbdResetDevice002 result=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    g_usbInterface->CloseDevice(dev);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_9100
 * @tc.name     : UsbdResetDevice003
 * @tc.desc     : int32_t ResetDevice(const UsbDev &dev)
 * @tc.desc     : Negative test
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdDeviceTest, UsbdResetDevice003, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->ResetDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d UsbdResetDevice003 result=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    g_usbInterface->CloseDevice(dev);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_9200
 * @tc.name     : UsbdResetDevice004
 * @tc.desc     : int32_t ResetDevice(const UsbDev &dev)
 * @tc.desc     : Negative test
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdDeviceTest, UsbdResetDevice004, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->ResetDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d UsbdResetDevice004 result=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    g_usbInterface->CloseDevice(dev);
}
} // namespace