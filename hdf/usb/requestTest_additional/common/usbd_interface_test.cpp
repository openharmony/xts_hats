/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

const int SLEEP_TIME = 3;

class UsbdInterfaceTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

    static UsbDev dev_;
    static OHOS::sptr<OHOS::USB::UsbSubscriberTest> subscriber_;
};
UsbDev UsbdInterfaceTestAdditional::dev_ = {0, 0};
OHOS::sptr<OHOS::USB::UsbSubscriberTest> UsbdInterfaceTestAdditional::subscriber_ = nullptr;

sptr<IUsbInterface> g_usbInterface = nullptr;

int32_t SwitchErrCode(int32_t ret) { return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret; }

void UsbdInterfaceTestAdditional::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdInterfaceTestAdditional::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
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
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};

    ret = g_usbInterface->OpenDevice(dev_);
    ASSERT_EQ(0, ret);
    HDF_LOGI("UsbdInterfaceTestAdditional:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ret = g_usbInterface->ClaimInterface(dev_, 1, 1);
    ASSERT_EQ(0, ret);
}

void UsbdInterfaceTestAdditional::TearDownTestCase(void)
{
    g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};
    auto ret = g_usbInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdInterfaceTestAdditional:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdInterfaceTestAdditional::SetUp(void) {}

void UsbdInterfaceTestAdditional::TearDown(void) {}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_0900
 * @tc.name: testHdiusbrequestTestSetInterface001
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface001, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1000
 * @tc.name: testHdiusbrequestTestSetInterface002
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface002, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1100
 * @tc.name: testHdiusbrequestTestSetInterface003
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface003, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1200
 * @tc.name: testHdiusbrequestTestSetInterface004
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface004, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1300
 * @tc.name: testHdiusbrequestTestSetInterface005
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface005, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1400
 * @tc.name: testHdiusbrequestTestSetInterface006
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface006, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1500
 * @tc.name: testHdiusbrequestTestSetInterface007
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface007, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1600
 * @tc.name: testHdiusbrequestTestSetInterface008
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface008, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1700
 * @tc.name: testHdiusbrequestTestSetInterface009
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface009, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1800
 * @tc.name: testHdiusbrequestTestSetInterface010
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface010, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_1900
 * @tc.name: testHdiusbrequestTestSetInterface011
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},1,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface011, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2000
 * @tc.name: testHdiusbrequestTestSetInterface012
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface012, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2100
 * @tc.name: testHdiusbrequestTestSetInterface013
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,0},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface013, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2200
 * @tc.name: testHdiusbrequestTestSetInterface014
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,0},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface014, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2300
 * @tc.name: testHdiusbrequestTestSetInterface015
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,0},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface015, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2400
 * @tc.name: testHdiusbrequestTestSetInterface016
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,0},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface016, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2500
 * @tc.name: testHdiusbrequestTestSetInterface017
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,10).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface017, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 10;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2600
 * @tc.name: testHdiusbrequestTestSetInterface018
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},10,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface018, TestSize.Level2)
{
    int32_t interfaceId = 10;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2700
 * @tc.name: testHdiusbrequestTestSetInterface019
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface019, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2800
 * @tc.name: testHdiusbrequestTestSetInterface020
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},100,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface020, TestSize.Level2)
{
    int32_t interfaceId = 100;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_2900
 * @tc.name: testHdiusbrequestTestSetInterface021
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,100).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface021, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 100;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3000
 * @tc.name: testHdiusbrequestTestSetInterface022
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface022, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3100
 * @tc.name: testHdiusbrequestTestSetInterface023
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface023, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3200
 * @tc.name: testHdiusbrequestTestSetInterface024
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface024, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3300
 * @tc.name: testHdiusbrequestTestSetInterface025
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface025, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3400
 * @tc.name: testHdiusbrequestTestSetInterface026
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},1,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface026, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3500
 * @tc.name: testHdiusbrequestTestSetInterface027
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface027, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3600
 * @tc.name: testHdiusbrequestTestSetInterface028
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface028, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3700
 * @tc.name: testHdiusbrequestTestSetInterface029
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface029, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3800
 * @tc.name: testHdiusbrequestTestSetInterface030
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface030, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_3900
 * @tc.name: testHdiusbrequestTestSetInterface031
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface031, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4000
 * @tc.name: testHdiusbrequestTestSetInterface032
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface032, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4100
 * @tc.name: testHdiusbrequestTestSetInterface033
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface033, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4200
 * @tc.name: testHdiusbrequestTestSetInterface034
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface034, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4300
 * @tc.name: testHdiusbrequestTestSetInterface035
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface035, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4400
 * @tc.name: testHdiusbrequestTestSetInterface036
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface036, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4500
 * @tc.name: testHdiusbrequestTestSetInterface037
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface037, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4600
 * @tc.name: testHdiusbrequestTestSetInterface038
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface038, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4700
 * @tc.name: testHdiusbrequestTestSetInterface039
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface039, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4800
 * @tc.name: testHdiusbrequestTestSetInterface040
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,100).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface040, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 100;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_4900
 * @tc.name: testHdiusbrequestTestSetInterface041
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},100,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface041, TestSize.Level2)
{
    int32_t interfaceId = 100;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5000
 * @tc.name: testHdiusbrequestTestSetInterface042
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface042, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5100
 * @tc.name: testHdiusbrequestTestSetInterface043
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},10,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface043, TestSize.Level2)
{
    int32_t interfaceId = 10;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5200
 * @tc.name: testHdiusbrequestTestSetInterface044
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,10).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface044, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 10;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5300
 * @tc.name: testHdiusbrequestTestSetInterface045
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface045, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5400
 * @tc.name: testHdiusbrequestTestSetInterface046
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface046, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5500
 * @tc.name: testHdiusbrequestTestSetInterface047
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface047, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5600
 * @tc.name: testHdiusbrequestTestSetInterface048
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface048, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5700
 * @tc.name: testHdiusbrequestTestSetInterface049
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,100).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface049, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 100;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    dev.devAddr = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5800
 * @tc.name: testHdiusbrequestTestSetInterface050
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},100,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface050, TestSize.Level2)
{
    int32_t interfaceId = 100;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    dev.devAddr = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_5900
 * @tc.name: testHdiusbrequestTestSetInterface051
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface051, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    dev.devAddr = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6000
 * @tc.name: testHdiusbrequestTestSetInterface052
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},10,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface052, TestSize.Level2)
{
    int32_t interfaceId = 10;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    dev.devAddr = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6100
 * @tc.name: testHdiusbrequestTestSetInterface053
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,10).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface053, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 10;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    dev.devAddr = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6200
 * @tc.name: testHdiusbrequestTestSetInterface054
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface054, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    dev.devAddr = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6300
 * @tc.name: testHdiusbrequestTestSetInterface055
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface055, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    dev.devAddr = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6400
 * @tc.name: testHdiusbrequestTestSetInterface056
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface056, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    dev.devAddr = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6500
 * @tc.name: testHdiusbrequestTestSetInterface057
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface057, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    dev.devAddr = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6600
 * @tc.name: testHdiusbrequestTestSetInterface058
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface058, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6700
 * @tc.name: testHdiusbrequestTestSetInterface059
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface059, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6800
 * @tc.name: testHdiusbrequestTestSetInterface060
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface060, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_6900
 * @tc.name: testHdiusbrequestTestSetInterface061
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},1,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface061, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_7000
 * @tc.name: testHdiusbrequestTestSetInterface062
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface062, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_7100
 * @tc.name: testHdiusbrequestTestSetInterface063
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface063, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_7200
 * @tc.name: testHdiusbrequestTestSetInterface064
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface064, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_7300
 * @tc.name: testHdiusbrequestTestSetInterface065
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface065, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetInterface_7400
 * @tc.name: testHdiusbrequestTestSetInterface066
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiusbrequestTestSetInterface066, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

} // namespace