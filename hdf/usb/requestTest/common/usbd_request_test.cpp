/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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
#include "usbd_request_test.h"
#include "v1_2/iusb_interface.h"
#include "v1_2/usb_types.h"

const int SLEEP_TIME = 3;
const uint8_t INDEX_1 = 1;
const uint8_t INDEX_INVALID = 255;
const uint8_t CONFIG_ID_0 = 0;
const uint8_t CONFIG_ID_INVALID = 222;
const uint8_t BUS_NUM_INVALID = 255;
const uint8_t DEV_ADDR_INVALID = 255;
const uint8_t STRING_ID_INVALID = 233;
const uint32_t MAX_BUFFER_LENGTH = 255;
const int TAG_NUM_10 = 10;
const uint8_t INTERFACEID_OK = 1;
const uint8_t INTERFACEID_INVALID = 255;
const uint8_t POINTID_INVALID = 158;
// data interface have 2 point : 1->bulk_out 2->bulk_in
const uint8_t POINTID_DIR_IN = USB_ENDPOINT_DIR_IN | 2;
const uint8_t POINTID_DIR_OUT = USB_ENDPOINT_DIR_OUT | 1;
const uint8_t INVALID_NUM = 222;
const uint32_t TIME_WAIT = 10000;

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;
using namespace OHOS::HDI::Usb::V1_2;

UsbDev UsbdRequestTest::dev_ = {0, 0};
sptr<UsbSubscriberTest> UsbdRequestTest::subscriber_ = nullptr;

namespace {
sptr<OHOS::HDI::Usb::V1_2::IUsbInterface> g_usbInterface = nullptr;

int32_t SwitchErrCode(int32_t ret)
{
    return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret;
}

void UsbdRequestTest::SetUpTestCase(void)
{
    g_usbInterface = OHOS::HDI::Usb::V1_2::IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdRequestTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }

    subscriber_ = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber_) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber_ failed", __func__);
        exit(0);
    }

    std::cout << "please connect device, press enter to continue" << std::endl;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    dev_ = { subscriber_->busNum_, subscriber_->devAddr_ };

    ret = g_usbInterface->OpenDevice(dev_);
    HDF_LOGI("UsbdRequestTest:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTest::TearDownTestCase(void)
{
    g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    dev_ = { subscriber_->busNum_, subscriber_->devAddr_ };
    auto ret = g_usbInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdRequestTest:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTest::SetUp(void) {}

void UsbdRequestTest::TearDown(void) {}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_0200
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Func_0200, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0200 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_0400
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_0400, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_0400 %{public}d SetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_0500
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_0500, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_0500 %{public}d SetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_0600
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_0600, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_INVALID;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
    configIndex = INDEX_1;
    ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_0600 %{public}d SetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_0700
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_0700, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_0700 %{public}d SetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_0800
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_0800, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_0800 %{public}d SetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_0900
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, devAddr && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_0900, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_0900 %{public}d SetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1000
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1000, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1000 %{public}d SetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_0300
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Func_0300, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0300 %{public}d GetConfig=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1100
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1100, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1100 %{public}d GetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1200
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1200, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1200 %{public}d GetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1300
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1300, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1300 %{public}d GetConfig=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_0700
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Func_0700, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0700 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3300
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3300, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3300 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3400
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3400, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3400 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3500
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3500, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    interfaceId = INTERFACEID_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3500 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3600
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3600, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3600 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3700
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3700, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3700 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3800
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3800, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3800 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3900
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3900, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3900 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Func_0100
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Func_0100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Func_0100 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_0100
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_0100, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_0100 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_0200
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_0200, Function | MediumTest | Level1)
{
    uint8_t devAddr = DEV_ADDR_INVALID;
    struct UsbDev dev = { dev_.busNum, devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_0200 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_0300
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_0300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_0300 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_0400
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_0400, Function | MediumTest | Level1)
{
    uint8_t busNum = BUS_NUM_INVALID;
    uint8_t devAddr = DEV_ADDR_INVALID;
    struct UsbDev dev = { busNum, devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_0400 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_0500
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_0500, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_0500 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_0600
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr && length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_0600, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_0600 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_0700
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Reverse test: busNum、devAddr、length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_0700, Function | MediumTest | Level1)
{
    uint8_t busNum = BUS_NUM_INVALID;
    uint8_t devAddr = DEV_ADDR_INVALID;
    struct UsbDev dev = { busNum, devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_0700 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Func_0200
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Func_0200, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI(
        "UsbdRequestTest::SUB_USB_DeviceManager_HDI_Func_0200 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_0800
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_0800, Function | MediumTest | Level1)
{
    uint8_t stringId = 1;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_0800 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_0900
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, stringId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_0900, Function | MediumTest | Level1)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_0900 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_1000
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_1000, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_1000 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_1100
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_1100, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_1100 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_1200
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_1200, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_1200 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_1300
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr && stringID error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_1300, Function | MediumTest | Level1)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_1300 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_1400
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && stringID error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_1400, Function | MediumTest | Level1)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_1400 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_0400
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Func_0400, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI(
        "UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0400 length=%{public}zu buffer=%{public}zu ret=%{public}d",
        devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1400
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1400, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1400 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1500
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1500, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1500 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1600
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1600, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1600 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1700
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1700, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1700 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1800
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum、configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1800, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1800 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_1900
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr && configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_1900, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_1900 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2000
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2000, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2000 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", devData.size(), sizeof(devData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Func_1200
 * @tc.desc: Test functions to GetRawDescriptor
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Func_1200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Func_1200 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", rawData.size(), sizeof(rawData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_1600
 * @tc.desc: Test functions to GetRawDescriptor
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_1600, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_1600 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", rawData.size(), sizeof(rawData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_1700
 * @tc.desc: Test functions to GetRawDescriptor
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_1700, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_1700 "
        "length=%{public}zu buffer=%{public}zu ret=%{public}d", rawData.size(), sizeof(rawData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Func_1300
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Func_1300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t fd = 0;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Func_1300 %{public}d fd=%{public}d ret=%{public}d",
        __LINE__, fd, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_1800
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_1800, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    int32_t fd = 0;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_1800 %{public}d fd=%{public}d ret=%{public}d",
        __LINE__, fd, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_1900
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_1900, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    int32_t fd = 0;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_1900 %{public}d fd=%{public}d ret=%{public}d",
        __LINE__, fd, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_DeviceManager_HDI_Compatibility_2000
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Negative test: parameters exception, fd error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_DeviceManager_HDI_Compatibility_2000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t fd = MAX_BUFFER_LENGTH;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_DeviceManager_HDI_Compatibility_2000 %{public}d fd=%{public}d ret=%{public}d",
        __LINE__, fd, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Func_2100
 * @tc.name     : GetDeviceFileDescriptor001
 * @tc.desc     : int32_t GetDeviceFileDescriptor(const UsbDev &dev, int32_t &fd)
 * @tc.desc     : Positive test: parameters correctly
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdRequestTest, GetDeviceFileDescriptor001, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t fd = 0;
    auto ret = g_usbInterface->GetDeviceFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::GetDeviceFileDescriptor001 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_0110
 * @tc.name     : GetDeviceFileDescriptor002
 * @tc.desc     : int32_t GetDeviceFileDescriptor(const UsbDev &dev, int32_t &fd)
 * @tc.desc     : Negative test: parameters exception, busNum error
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdRequestTest, GetDeviceFileDescriptor002, Function | MediumTest | Level1)
{
    struct UsbDev dev = {BUS_NUM_INVALID, dev_.devAddr};
    int32_t fd = 0;
    auto ret = g_usbInterface->GetDeviceFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::GetDeviceFileDescriptor002 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_0120
 * @tc.name     : GetDeviceFileDescriptor003
 * @tc.desc     : int32_t GetDeviceFileDescriptor(const UsbDev &dev, int32_t &fd)
 * @tc.desc     : Negative test: parameters exception, devAddr error
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdRequestTest, GetDeviceFileDescriptor003, Function | MediumTest | Level1)
{
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    int32_t fd = 0;
    auto ret = g_usbInterface->GetDeviceFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::GetDeviceFileDescriptor003 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_0130
 * @tc.name     : GetDeviceFileDescriptor004
 * @tc.desc     : int32_t GetDeviceFileDescriptor(const UsbDev &dev, int32_t &fd)
 * @tc.desc     : Negative test: parameters exception, fd error
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdRequestTest, GetDeviceFileDescriptor004, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t fd = MAX_BUFFER_LENGTH;
    auto ret = g_usbInterface->GetDeviceFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::GetDeviceFileDescriptor004 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_0500
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Func_0500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0500 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0500 "
        "interfaceId=%{public}d pointId=%{public}d ret=%{public}d", interfaceId, pointId, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2100
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2100 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2100 "
        "interfaceId=%{public}d pointId=%{public}d ret=%{public}d", interfaceId, pointId, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2200
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2200, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2200 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2200 "
        "interfaceId=%{public}d pointId=%{public}d ret=%{public}d", interfaceId, pointId, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2300
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2300 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    interfaceId = INVALID_NUM;
    dev.busNum = BUS_NUM_INVALID;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2300 "
        "interfaceId=%{public}d pointId=%{public}d ret=%{public}d", interfaceId, pointId, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2400
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2400, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2400 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2400 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2500
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceId && pointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2500 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    interfaceId = INVALID_NUM;
    pointId = INVALID_NUM;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2500 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2600
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2600 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'w', 'r', 'i', 't', 'e'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '7'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2600 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2700
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2700, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2700 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    interfaceId = INVALID_NUM;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'w', 'r', 'i', 't', 'e'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '8'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2700 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2800
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, interfaceId && pointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2800 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    interfaceId = INVALID_NUM;
    pointId = INVALID_NUM;
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'w', 'r', 'i', 't', 'e'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '9'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2800 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_0600
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Func_0600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0600 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0600 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> waitData(TAG_NUM_10);
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0600 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_2900
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_2900, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2900 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbPipe pipe = { interfaceId, pointId };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2900 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    std::vector<uint8_t> waitData(TAG_NUM_10);
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_2900 %{public}d RequestWait=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3000
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3000, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3000 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbPipe pipe = { interfaceId, pointId };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3000 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    std::vector<uint8_t> waitData(TAG_NUM_10);
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3000 %{public}d RequestWait=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3100
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, timeout error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3100, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3100 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3100 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> waitData(TAG_NUM_10);
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, -TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3100 %{public}d RequestWait=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_3200
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_3200, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3200 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3200 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> waitData(TAG_NUM_10);
    dev.devAddr = DEV_ADDR_INVALID;
    dev.busNum = BUS_NUM_INVALID;
    ret = g_usbInterface->RequestWait(dev, waitData, bufferData, TIME_WAIT);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_3200 %{public}d RequestWait=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_0300
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranFunc_0300, Function | MediumTest | Level1)
{
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranFunc_0300 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '1'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranFunc_0300 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranFunc_0300 %{public}d RequestCancel=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3200
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_3200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3200 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '2'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3200 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3200 %{public}d RequestCancel=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI(
        "UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3200 again %{public}d RequestCancel=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3300
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_3300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3300 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '3'};
    struct UsbPipe pipe = { interfaceId, pointId };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3300 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3300 %{public}d RequestCancel=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::UsbdRequestCancel003 again %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3400
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: intfId && endpointId error in pipe but not used
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_3400, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3400 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '4'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3400 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    pipe.intfId = INTERFACEID_INVALID;
    pipe.endpointId = POINTID_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3400 %{public}d RequestCancel=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    pipe = {interfaceId, pointId};
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::UsbdRequestCancel004 %{public}d again RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3500
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: call twice
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_3500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3500 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '5'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3500 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3500 %{public}d RequestCancel=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev = dev_;
    pipe = {interfaceId, pointId};
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3600
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error && interfaceid ignore
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_3600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3600 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '6'};
    struct UsbPipe pipe = { interfaceId, pointId };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3600 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    pipe.intfId = 224;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3600 %{public}d RequestCancel=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev.busNum = dev_.busNum;
    pipe.intfId = INTERFACEID_OK;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3700
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, devAddr error && interfaceid ignore
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_3700, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3700 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '7'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3700 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    pipe.intfId = INTERFACEID_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3700 %{public}d RequestCancel=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev.devAddr = dev_.devAddr;
    pipe.intfId = INTERFACEID_OK;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3800
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error && devAddr error && interfaceid ignore
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_3800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3800 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData = {'r', 'e', 'q', 'u', 'e', 's', 't', '0', '0', '8'};
    ret = g_usbInterface->RequestQueue(dev, pipe, clientData, bufferData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3800 %{public}d RequestQueue=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    pipe.intfId = INTERFACEID_INVALID;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_3800 %{public}d RequestCancel=%{public}d",
        __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    pipe.intfId = INTERFACEID_OK;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_0800
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Func_0800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Func_0800 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4000
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_4000, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_4000 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4100
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_4100, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_4100 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4200
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_4200, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_4200 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4300
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_4300, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_4300 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4400
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_4400, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { DEV_ADDR_INVALID, dev_.devAddr };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_4400 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4500
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_4500, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_4500 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_4600
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_Compatibility_4600, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = { BUS_NUM_INVALID, DEV_ADDR_INVALID };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_Compatibility_4600 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_0700
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranFunc_0700, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranFunc_0700 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0700 %{public}d RegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranFunc_0700 %{public}d BulkCancel=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0700 %{public}d UnRegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8100
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_8100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_8100 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI(
        "UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8100 %{public}d RegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.busNum = BUS_NUM_INVALID;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI(
        "UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_8100 %{public}d BulkCancel=%{public}d",
        __LINE__, ret);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI(
        "UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8100 %{public}d UnRegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8200
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_8200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_8200 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI(
        "UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8200 %{public}d RegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI(
        "UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_8200 %{public}d BulkCancel=%{public}d",
        __LINE__, ret);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI(
        "UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8200 %{public}d UnRegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8300
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_8300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_8300 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI(
        "UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8300 %{public}d RegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    pipe.intfId = POINTID_INVALID;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI(
        "UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_8300 %{public}d BulkCancel=%{public}d",
        __LINE__, ret);
    ASSERT_NE(0, ret);
    dev = dev_;
    pipe = {interfaceId, pointId};
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI(
        "UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8300 %{public}d UnRegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8400
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HostManager_HDI_TranCompatibility_8400, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_8400 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI(
        "UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8400 %{public}d RegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
    pipe.intfId = POINTID_INVALID;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI(
        "UsbdRequestTest::SUB_USB_HostManager_HDI_TranCompatibility_8400 %{public}d BulkCancel=%{public}d",
        __LINE__, ret);
    ASSERT_NE(0, ret);
    pipe = {interfaceId, pointId};
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI(
        "UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8400 %{public}d UnRegBulkCallback=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(ret, 0);
}
} // namespace
