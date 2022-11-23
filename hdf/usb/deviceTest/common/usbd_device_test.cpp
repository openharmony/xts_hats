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
#include <vector>

#include "UsbSubscriberTest.h"
#include "hdf_log.h"
#include "usbd_device_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

const int SLEEP_TIME = 3;
const uint8_t BUS_NUM_255 = 255;
const uint8_t DEV_ADDR_255 = 255;

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

struct UsbDev UsbdDeviceTest::dev_ = { 0, 0 };

void UsbdDeviceTest::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdDeviceTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }

    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber failed\n", __func__);
        exit(0);
    }
    dev_ = { subscriber->busNum_, subscriber->devAddr_ };

    std::cout << "please connect device, press enter to continue" << std::endl;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void UsbdDeviceTest::TearDownTestCase(void) {}

void UsbdDeviceTest::SetUp(void) {}

void UsbdDeviceTest::TearDown(void) {}

/**
 * @tc.name: SUB_USB_HDI_0010
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HDI_0010, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result =%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0020
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HDI_0020, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0030
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HDI_0030, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0040
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: int32_t OpenDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HDI_0040, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, DEV_ADDR_255 };
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0050
 * @tc.desc: Test functions to CloseDevice
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HDI_0050, Function | MediumTest | Level1)
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
 * @tc.name: SUB_USB_HDI_0060
 * @tc.desc: Test functions to CloseDevice
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HDI_0060, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_255;
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    g_usbInterface->CloseDevice(dev);
}

/**
 * @tc.name: SUB_USB_HDI_0070
 * @tc.desc: Test functions to CloseDevice
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HDI_0070, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_255;
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    g_usbInterface->CloseDevice(dev);
}

/**
 * @tc.name: SUB_USB_HDI_0080
 * @tc.desc: Test functions to CloseDevice
 * @tc.desc: int32_t CloseDevice(const UsbDev &dev);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdDeviceTest, SUB_USB_HDI_0080, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->OpenDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d OpenDevice result=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_255;
    dev.devAddr = DEV_ADDR_255;
    ret = g_usbInterface->CloseDevice(dev);
    HDF_LOGI("UsbdDeviceTest:: Line:%{public}d Close result=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    g_usbInterface->CloseDevice(dev);
}
} // namespace