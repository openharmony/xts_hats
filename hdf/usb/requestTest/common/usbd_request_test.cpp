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
#include "usbd_request_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

const int SLEEP_TIME = 3;
const uint8_t INDEX_0 = 0;
const uint8_t INDEX_1 = 1;
const uint8_t INDEX_255 = 255;
const uint8_t CONFIG_ID_0 = 0;
const uint8_t CONFIG_ID_1 = 1;
const uint8_t CONFIG_ID_222 = 222;
const uint8_t BUS_NUM_255 = 255;
const uint8_t DEV_ADDR_255 = 255;
const uint8_t BUS_NUM_222 = 222;
const uint8_t STRING_ID_233 = 233;
const uint8_t DEV_ADDR_222 = 222;
const uint32_t LENGTH_NUM_255 = 255;
const uint32_t TAG_LENGTH_NUM_1000 = 1000;
const int TAG_NUM_10 = 10;
const int TAG_NUM_11 = 11;
const uint8_t INTERFACEID_1 = 1;
const uint8_t INTERFACEID_255 = 255;
const int32_t INT32_INTERFACEID_1 = 1;
const int32_t INT32_INTERFACEID_255 = 255;
const uint8_t POINTID_1 = 1;
const uint8_t POINTID_129 = 130;
const uint8_t POINTID_158 = 158;
const uint8_t POINTID_255 = 255;
const uint8_t POINTID_BULK_IN = 0x82;
const uint8_t POINTID_BULK_OUT = 0x01;
UsbDev UsbdRequestTest::dev_ = {0, 0};

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

void UsbdRequestTest::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdRequestTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }

    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber failed", __func__);
        exit(0);
    }

    std::cout << "please connect device, press enter to continue" << std::endl;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    dev_ = { subscriber->busNum_, subscriber->devAddr_ };

    ret = g_usbInterface->OpenDevice(dev_);
    HDF_LOGI("UsbdRequestTest:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTest::TearDownTestCase(void)
{
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber failed", __func__);
        exit(0);
    }
    dev_ = { subscriber->busNum_, subscriber->devAddr_ };
    auto ret = g_usbInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdRequestTest:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTest::SetUp(void) {}

void UsbdRequestTest::TearDown(void) {}

/**
 * @tc.name: SUB_USB_HDI_0090
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0090, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0090 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0100
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0100, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0100 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0110
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0110, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_222 };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0110 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0120
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0120, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_255;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_TRUE(ret != 0);
    configIndex = INDEX_1;
    ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0120 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_TRUE(ret == 0);
}

/**
 * @tc.name: SUB_USB_HDI_0130
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0130, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_222, DEV_ADDR_222 };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0130 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0140
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0140, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_255;
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0140 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0150
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, devAddr && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0150, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_255;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_222 };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0150 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0160
 * @tc.desc: Test functions to SetConfig
 * @tc.desc: int32_t SetConfig(const UsbDev &dev, uint8_t configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0160, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_255;
    struct UsbDev dev = { BUS_NUM_222, DEV_ADDR_222 };
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0160 %{public}d SetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0170
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0170, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0170 %{public}d GetConfig=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0180
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0180, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0180 %{public}d GetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0190
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0190, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_222 };
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0190 %{public}d GetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0200
 * @tc.desc: Test functions to GetConfig
 * @tc.desc: int32_t GetConfig(const UsbDev &dev, uint8_t &configIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0200, Function | MediumTest | Level1)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = { BUS_NUM_222, DEV_ADDR_222 };
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0200 %{public}d GetConfig=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1010
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1010, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1010 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1020
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1020, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = dev_;
    dev.busNum = 20;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1020 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1030
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1030, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1030 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1040
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1040, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = dev_;
    interfaceId = INTERFACEID_255;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1040 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1050
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1050, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = { BUS_NUM_255, DEV_ADDR_255 };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1050 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1060
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1060, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_255;
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1060 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1070
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1070, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_255;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1070 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1080
 * @tc.desc: Test functions to ClaimInterface
 * @tc.desc: int32_t  ClaimInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1080, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_255;
    struct UsbDev dev = { BUS_NUM_255, DEV_ADDR_255 };
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1080 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_1170
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1170, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1170 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1170 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1180
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1180, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1180 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_222;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1180 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1190
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1190, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t altIndex = INDEX_255;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1190 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_222;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1190 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1200
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1200, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t altIndex = INDEX_255;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1200 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    interfaceId = INTERFACEID_255;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1200 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1210
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1210, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t altIndex = INDEX_0;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1210 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_222;
    dev.devAddr = DEV_ADDR_222;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1210 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1220
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1220, Function | MediumTest | Level1)
{
    int32_t interfaceId = INT32_INTERFACEID_1;
    uint8_t altIndex = INDEX_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1220 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_255;
    interfaceId = INT32_INTERFACEID_255;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1220 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1230
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1230, Function | MediumTest | Level1)
{
    int32_t interfaceId = INT32_INTERFACEID_1;
    uint8_t altIndex = INDEX_255;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1230 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_255;
    interfaceId = INT32_INTERFACEID_255;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1230 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1240
 * @tc.desc: Test functions to SetInterface
 * @tc.desc: int32_t SetInterface(const UsbDev &dev, uint8_t interfaceId, uint8_t altIndex);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1240, Function | MediumTest | Level1)
{
    uint8_t altIndex = INDEX_255;
    int32_t interfaceId = INT32_INTERFACEID_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1240 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_255;
    dev.devAddr = DEV_ADDR_255;
    interfaceId = INT32_INTERFACEID_255;
    ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1240 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0210
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0210, Function | MediumTest | Level1)
{
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0210 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0220
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0220, Function | MediumTest | Level1)
{
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0220 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0230
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0230, Function | MediumTest | Level1)
{
    uint8_t devAddr = DEV_ADDR_222;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { dev_.busNum, devAddr };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0230 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0240
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0240, Function | MediumTest | Level1)
{
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = 0;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0240 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0250
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0250, Function | MediumTest | Level1)
{
    uint8_t busNum = BUS_NUM_222;
    uint8_t devAddr = DEV_ADDR_222;
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { busNum, devAddr };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0250 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0260
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0260, Function | MediumTest | Level1)
{
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = 0;
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0260 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0270
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr && length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0270, Function | MediumTest | Level1)
{
    uint8_t buffer[] = {};
    uint32_t length = 0;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_222 };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0270 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0280
 * @tc.desc: Test functions to GetDeviceDescriptor
 * @tc.desc: int32_t GetDeviceDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Reverse test: busNum、devAddr、length error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0280, Function | MediumTest | Level1)
{
    uint8_t busNum = BUS_NUM_222;
    uint8_t devAddr = DEV_ADDR_222;
    uint8_t buffer[] = {};
    uint32_t length = 0;
    struct UsbDev dev = { busNum, devAddr };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0280 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0290
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0290, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0290 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0300
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0300, Function | MediumTest | Level1)
{
    uint8_t stringId = 1;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0300 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0310
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, stringId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0310, Function | MediumTest | Level1)
{
    uint8_t stringId = 222;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0310 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0320
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0320, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = 8;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0320 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0330
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0330, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = 8;
    struct UsbDev dev = { BUS_NUM_222, DEV_ADDR_222 };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0330 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0340
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0340, Function | MediumTest | Level1)
{
    uint8_t stringId = 0;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0340 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0350
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr && stringID error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0350, Function | MediumTest | Level1)
{
    uint8_t stringId = STRING_ID_233;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_222 };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0350 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0360
 * @tc.desc: Test functions to GetStringDescriptor
 * @tc.desc: int32_t GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && stringID error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0360, Function | MediumTest | Level1)
{
    uint8_t stringId = STRING_ID_233;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { BUS_NUM_222, DEV_ADDR_222 };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0360 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0370
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0370, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_0;
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0370 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0380
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0380, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_1;
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0380 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0390
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0390, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_1;
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_222 };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0390 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0400
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0400, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_1;
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0400 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0410
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0410, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_1;
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { BUS_NUM_222, DEV_ADDR_222 };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0410 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0420
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum、configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0420, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_222;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0420 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0430
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr && configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0430, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_222;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_222 };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0430 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0440
 * @tc.desc: Test functions to GetConfigDescriptor
 * @tc.desc: int32_t GetConfigDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && configId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0440, Function | MediumTest | Level1)
{
    uint8_t configId = CONFIG_ID_222;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = { BUS_NUM_222, DEV_ADDR_222 };
    std::vector<uint8_t> devdata(buffer, buffer + length);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0440 length=%{public}zu buffer=%{public}zu ret=%{public}d", devdata.size(),
        sizeof(devdata), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1710
 * @tc.desc: Test functions to GetRawDescriptor
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1710, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1710 length=%{public}zu buffer=%{public}zu ret=%{public}d", rawData.size(),
        sizeof(rawData), ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1720
 * @tc.desc: Test functions to GetRawDescriptor
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1720, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1720 length=%{public}zu buffer=%{public}zu ret=%{public}d", rawData.size(),
        sizeof(rawData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1730
 * @tc.desc: Test functions to GetRawDescriptor
 * @tc.desc: int32_t GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1730, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_222 };
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1730 length=%{public}zu buffer=%{public}zu ret=%{public}d", rawData.size(),
        sizeof(rawData), ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1740
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1740, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t fd = 0;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1740 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1750
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1750, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_222, dev_.devAddr };
    int32_t fd = 0;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1750 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1760
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1760, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_222 };
    int32_t fd = 0;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1760 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1770
 * @tc.desc: Test functions to GetFileDescriptor
 * @tc.desc: int32_t GetFileDescriptor(const UsbDev &dev, int32_t &fd);
 * @tc.desc: Negative test: parameters exception, fd error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1770, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t fd = LENGTH_NUM_255;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1770 %{public}d fd=%{public}d ret=%{public}d", __LINE__, fd, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0790
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0790, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_129;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0790 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0790 interfaceId=%{public}d pointid=%{public}d ret=%{public}d", interfaceId,
        pointid, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0800
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0800 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    dev = { BUS_NUM_222, DEV_ADDR_222 };
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0800 interfaceId=%{public}d pointid=%{public}d ret=%{public}d", interfaceId,
        pointid, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0810
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0810, Function | MediumTest | Level1)
{
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    struct UsbDev dev = dev_;
    uint32_t length = LENGTH_NUM_255;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0810 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    dev.devAddr = DEV_ADDR_222;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0810 interfaceId=%{public}d pointid=%{public}d ret=%{public}d", interfaceId,
        pointid, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0820
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum && configIndex error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0820, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0820 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    interfaceId = 222;
    dev.busNum = BUS_NUM_222;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0820 interfaceId=%{public}d pointid=%{public}d ret=%{public}d", interfaceId,
        pointid, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0830
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0830, Function | MediumTest | Level1)
{
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0830 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    dev.busNum = BUS_NUM_222;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0830 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0840
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceId && pointid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0840, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    uint32_t length = LENGTH_NUM_255;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0840 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    dev.busNum = BUS_NUM_222;
    interfaceId = 222;
    pointid = 222;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0840 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0850
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0850, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "request 007";
    uint32_t length = LENGTH_NUM_255;
    uint8_t pointid = POINTID_1;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0850 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue write";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_11 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0850 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0860
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0860, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "reuquest008";
    uint32_t length = LENGTH_NUM_255;
    uint8_t pointid = POINTID_1;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0860 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue write";
    interfaceId = 222;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_11 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0860 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0870
 * @tc.desc: Test functions to RequestQueue
 * @tc.desc: int32_t RequestQueue(const UsbDev &dev, const UsbPipe &pipe, std::vector<uint8_t> &clientData,
        std::vector<uint8_t> &buffer);
 * @tc.desc: Negative test: parameters exception, interfaceId && pointid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0870, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "request 009";
    uint32_t length = LENGTH_NUM_255;
    uint8_t pointid = POINTID_1;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0870 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue write";
    interfaceId = 222;
    pointid = 222;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_11 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0870 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0880
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0880, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0880 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0880 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t* clientObj = new uint8_t[10];
    std::vector<uint8_t> waitdata = { clientObj, clientObj + 10 };
    ret = g_usbInterface->RequestWait(dev, waitdata, bufferdata, 10000);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0880 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    delete[] clientObj;
    clientObj = nullptr;
}

/**
 * @tc.name: SUB_USB_HDI_0890
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0890, Function | MediumTest | Level1)
{
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0890 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbPipe pipe = { interfaceId, pointid };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0890 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_222;
    uint8_t* clientObj = new uint8_t[10];
    std::vector<uint8_t> waitdata = { clientObj, clientObj + 10 };
    ret = g_usbInterface->RequestWait(dev, waitdata, bufferdata, 10000);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0890 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    delete[] clientObj;
    clientObj = nullptr;
}

/**
 * @tc.name: SUB_USB_HDI_0900
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0900, Function | MediumTest | Level1)
{
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0900 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint32_t length = LENGTH_NUM_255;
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0900 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t* clientObj = new uint8_t[10];
    dev.devAddr = DEV_ADDR_222;
    std::vector<uint8_t> waitdata = { clientObj, clientObj + 10 };
    ret = g_usbInterface->RequestWait(dev, waitdata, bufferdata, 10000);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0900 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    delete[] clientObj;
    clientObj = nullptr;
}

/**
 * @tc.name: SUB_USB_HDI_0910
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, timeout error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0910, Function | MediumTest | Level1)
{
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0910 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = {};
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    uint32_t length = LENGTH_NUM_255;
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0910 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t* clientObj = new uint8_t[10];
    std::vector<uint8_t> waitdata = { clientObj, clientObj + 10 };
    ret = g_usbInterface->RequestWait(dev, waitdata, bufferdata, -10000);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0910 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    delete[] clientObj;
    clientObj = nullptr;
}

/**
 * @tc.name: SUB_USB_HDI_0920
 * @tc.desc: Test functions to RequestWait
 * @tc.desc: int32_t RequestWait(const UsbDev &dev, std::vector<uint8_t> &clientData, std::vector<uint8_t> &buffer,
 * int32_t timeout);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0920, Function | MediumTest | Level1)
{
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0920 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint32_t length = LENGTH_NUM_255;
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    uint8_t buffer[LENGTH_NUM_255] = {};
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0920 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t* clientObj = new uint8_t[10];
    std::vector<uint8_t> waitdata = { clientObj, clientObj + 10 };
    dev.devAddr = DEV_ADDR_255;
    dev.busNum = BUS_NUM_255;
    ret = g_usbInterface->RequestWait(dev, waitdata, bufferdata, 10000);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0920 %{public}d RequestWait=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    delete[] clientObj;
    clientObj = nullptr;
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_0930
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0930, Function | MediumTest | Level1)
{
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "request001";
    uint32_t length = LENGTH_NUM_255;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0930 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0930 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0930 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0940
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0940, Function | MediumTest | Level1)
{
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    struct UsbDev dev = dev_;
    uint32_t length = LENGTH_NUM_255;
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t buffer[LENGTH_NUM_255] = "request002";
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0940 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0940 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_222;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0940 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0940 again %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0950
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0950, Function | MediumTest | Level1)
{
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "request003";
    uint32_t length = LENGTH_NUM_255;
    uint8_t pointid = POINTID_129;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0950 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbPipe pipe = { interfaceId, pointid };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0950 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_222;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0950 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0960
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: intfId && endpointId error in pipe but not used
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0960, Function | MediumTest | Level1)
{
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue read";
    struct UsbDev dev = dev_;
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = "request004";
    uint8_t pointid = POINTID_BULK_OUT;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0960 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_10 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0960 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    pipe.intfId = INTERFACEID_255;
    pipe.endpointId = POINTID_255;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0960 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    pipe = {interfaceId, pointid};
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::UsbdRequestCancel004 %{public}d again RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0970
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: call twice
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0970, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "request005";
    uint32_t length = LENGTH_NUM_255;
    uint8_t pointid = POINTID_1;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0970 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue Write";
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_11 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0970 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0970 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev = dev_;
    pipe = {interfaceId, pointid};
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0980
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error && interfaceid ignore
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0980, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "request006";
    uint32_t length = LENGTH_NUM_255;
    uint8_t pointid = POINTID_1;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0980 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue Write";
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_11 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbPipe pipe = { interfaceId, pointid };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0980 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_255;
    pipe.intfId = 224;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0980 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev.busNum = dev_.busNum;
    pipe.intfId = INTERFACEID_1;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0990
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, devAddr error && interfaceid ignore
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_0990, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "request007";
    uint32_t length = LENGTH_NUM_255;
    uint8_t pointid = POINTID_1;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0990 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue Write";
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_11 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0990 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_222;
    pipe.intfId = INTERFACEID_255;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_0990 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev.devAddr = dev_.devAddr;
    pipe.intfId = INTERFACEID_1;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1000
 * @tc.desc: Test functions to RequestCancel
 * @tc.desc: int32_t RequestCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error && devAddr error && interfaceid ignore
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = "request008";
    uint8_t pointid = POINTID_1;
    uint8_t interfaceId = INTERFACEID_1;
    uint32_t length = LENGTH_NUM_255;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1000 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t tag[TAG_LENGTH_NUM_1000] = "queue Write";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> clientdata = { tag, tag + TAG_NUM_11 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->RequestQueue(dev, pipe, clientdata, bufferdata);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1000 %{public}d RequestQueue=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_222;
    dev.devAddr = DEV_ADDR_222;
    pipe.intfId = INTERFACEID_255;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1000 %{public}d RequestCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    pipe.intfId = INTERFACEID_1;
    ret = g_usbInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_1090
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1090, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1090 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1100
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1100, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1100 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1110
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1110, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1110 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1120
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, interfaceId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1120, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_255;
    struct UsbDev dev = dev_;
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1120 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1130
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1130, Function | MediumTest | Level1)
{
    uint8_t interfaceId = INTERFACEID_1;
    struct UsbDev dev = { BUS_NUM_255, DEV_ADDR_255 };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1130 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1140
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1140, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_255;
    struct UsbDev dev = { DEV_ADDR_255, dev_.devAddr };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1140 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1150
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, devAddr && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1150, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_255;
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1150 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1160
 * @tc.desc: Test functions to ReleaseInterface
 * @tc.desc: int32_t ReleaseInterface(const UsbDev &dev, uint8_t interfaceId);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1160, Function | MediumTest | Level1)
{
    int32_t interfaceId = INTERFACEID_255;
    struct UsbDev dev = { BUS_NUM_255, DEV_ADDR_255 };
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1160 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1990
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_1990, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_BULK_IN;
    struct UsbPipe pipe = {interfaceId, pointid};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1990 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_1990 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1990 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2000
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_2000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_BULK_IN;
    struct UsbPipe pipe = {interfaceId, pointid};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2000 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.busNum = BUS_NUM_222;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_2000 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2000 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2010
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_2010, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_BULK_IN;
    struct UsbPipe pipe = {interfaceId, pointid};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2010 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.devAddr = DEV_ADDR_222;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_2010 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2010 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2020
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr && interfaceid error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_2020, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_BULK_IN;
    struct UsbPipe pipe = {interfaceId, pointid};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2020 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.busNum = BUS_NUM_222;
    dev.devAddr = DEV_ADDR_222;
    pipe.intfId = POINTID_255;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_2020 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    dev = dev_;
    pipe = {interfaceId, pointid};
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2020 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2030
 * @tc.desc: Test functions to BulkCancel
 * @tc.desc: int32_t BulkCancel(const UsbDev &dev, const UsbPipe &pipe);
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdRequestTest, SUB_USB_HDI_2030, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_BULK_IN;
    struct UsbPipe pipe = {interfaceId, pointid};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2030 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    pipe.intfId = POINTID_158;
    ret = g_usbInterface->BulkCancel(dev, pipe);
    HDF_LOGI("UsbdRequestTest::SUB_USB_HDI_2030 %{public}d BulkCancel=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    pipe = {interfaceId, pointid};
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2030 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}
} // namespace