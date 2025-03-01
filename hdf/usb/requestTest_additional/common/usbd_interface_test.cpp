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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_0900
 * @tc.name: testHdiUsbRequestTestSetInterface001
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface001, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1000
 * @tc.name: testHdiUsbRequestTestSetInterface002
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface002, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1100
 * @tc.name: testHdiUsbRequestTestSetInterface003
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface003, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1200
 * @tc.name: testHdiUsbRequestTestSetInterface004
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface004, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1300
 * @tc.name: testHdiUsbRequestTestSetInterface005
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface005, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1400
 * @tc.name: testHdiUsbRequestTestSetInterface006
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface006, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1500
 * @tc.name: testHdiUsbRequestTestSetInterface007
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,0},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface007, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1600
 * @tc.name: testHdiUsbRequestTestSetInterface008
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface008, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1700
 * @tc.name: testHdiUsbRequestTestSetInterface009
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface009, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1800
 * @tc.name: testHdiUsbRequestTestSetInterface010
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface010, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_1900
 * @tc.name: testHdiUsbRequestTestSetInterface011
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},1,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface011, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2000
 * @tc.name: testHdiUsbRequestTestSetInterface012
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,255},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface012, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2100
 * @tc.name: testHdiUsbRequestTestSetInterface013
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,0},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface013, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2200
 * @tc.name: testHdiUsbRequestTestSetInterface014
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,0},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface014, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2300
 * @tc.name: testHdiUsbRequestTestSetInterface015
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,0},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface015, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2400
 * @tc.name: testHdiUsbRequestTestSetInterface016
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({100,0},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface016, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2500
 * @tc.name: testHdiUsbRequestTestSetInterface017
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,10).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface017, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 10;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2600
 * @tc.name: testHdiUsbRequestTestSetInterface018
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},10,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface018, TestSize.Level2)
{
    int32_t interfaceId = 10;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2700
 * @tc.name: testHdiUsbRequestTestSetInterface019
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface019, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2800
 * @tc.name: testHdiUsbRequestTestSetInterface020
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},100,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface020, TestSize.Level2)
{
    int32_t interfaceId = 100;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_2900
 * @tc.name: testHdiUsbRequestTestSetInterface021
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,100).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface021, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 100;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3000
 * @tc.name: testHdiUsbRequestTestSetInterface022
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface022, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3100
 * @tc.name: testHdiUsbRequestTestSetInterface023
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface023, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3200
 * @tc.name: testHdiUsbRequestTestSetInterface024
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface024, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3300
 * @tc.name: testHdiUsbRequestTestSetInterface025
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,100},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface025, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 100;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3400
 * @tc.name: testHdiUsbRequestTestSetInterface026
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},1,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface026, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3500
 * @tc.name: testHdiUsbRequestTestSetInterface027
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface027, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3600
 * @tc.name: testHdiUsbRequestTestSetInterface028
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface028, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3700
 * @tc.name: testHdiUsbRequestTestSetInterface029
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface029, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3800
 * @tc.name: testHdiUsbRequestTestSetInterface030
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface030, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_3900
 * @tc.name: testHdiUsbRequestTestSetInterface031
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface031, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4000
 * @tc.name: testHdiUsbRequestTestSetInterface032
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({0,255},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface032, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.devAddr = 255;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4100
 * @tc.name: testHdiUsbRequestTestSetInterface033
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface033, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4200
 * @tc.name: testHdiUsbRequestTestSetInterface034
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface034, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4300
 * @tc.name: testHdiUsbRequestTestSetInterface035
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface035, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4400
 * @tc.name: testHdiUsbRequestTestSetInterface036
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface036, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4500
 * @tc.name: testHdiUsbRequestTestSetInterface037
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface037, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4600
 * @tc.name: testHdiUsbRequestTestSetInterface038
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface038, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4700
 * @tc.name: testHdiUsbRequestTestSetInterface039
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,255},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface039, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4800
 * @tc.name: testHdiUsbRequestTestSetInterface040
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,100).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface040, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 100;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_4900
 * @tc.name: testHdiUsbRequestTestSetInterface041
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},100,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface041, TestSize.Level2)
{
    int32_t interfaceId = 100;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5000
 * @tc.name: testHdiUsbRequestTestSetInterface042
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface042, TestSize.Level2)
{
    int32_t interfaceId = 0;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5100
 * @tc.name: testHdiUsbRequestTestSetInterface043
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},10,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface043, TestSize.Level2)
{
    int32_t interfaceId = 10;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5200
 * @tc.name: testHdiUsbRequestTestSetInterface044
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,10).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface044, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 10;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5300
 * @tc.name: testHdiUsbRequestTestSetInterface045
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface045, TestSize.Level2)
{
    int32_t interfaceId = 1;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5400
 * @tc.name: testHdiUsbRequestTestSetInterface046
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface046, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5500
 * @tc.name: testHdiUsbRequestTestSetInterface047
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface047, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 1;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5600
 * @tc.name: testHdiUsbRequestTestSetInterface048
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,0},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface048, TestSize.Level2)
{
    int32_t interfaceId = 255;
    uint8_t altIndex = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->SetInterface(dev, interfaceId, altIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5700
 * @tc.name: testHdiUsbRequestTestSetInterface049
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,100).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface049, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5800
 * @tc.name: testHdiUsbRequestTestSetInterface050
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},100,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface050, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_5900
 * @tc.name: testHdiUsbRequestTestSetInterface051
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface051, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6000
 * @tc.name: testHdiUsbRequestTestSetInterface052
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},10,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface052, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6100
 * @tc.name: testHdiUsbRequestTestSetInterface053
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,10).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface053, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6200
 * @tc.name: testHdiUsbRequestTestSetInterface054
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface054, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6300
 * @tc.name: testHdiUsbRequestTestSetInterface055
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface055, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6400
 * @tc.name: testHdiUsbRequestTestSetInterface056
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface056, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6500
 * @tc.name: testHdiUsbRequestTestSetInterface057
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({1,1},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface057, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6600
 * @tc.name: testHdiUsbRequestTestSetInterface058
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},0,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface058, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6700
 * @tc.name: testHdiUsbRequestTestSetInterface059
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},0,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface059, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6800
 * @tc.name: testHdiUsbRequestTestSetInterface060
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},0,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface060, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_6900
 * @tc.name: testHdiUsbRequestTestSetInterface061
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},1,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface061, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_7000
 * @tc.name: testHdiUsbRequestTestSetInterface062
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},1,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface062, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_7100
 * @tc.name: testHdiUsbRequestTestSetInterface063
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},1,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface063, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_7200
 * @tc.name: testHdiUsbRequestTestSetInterface064
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},255,0).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface064, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_7300
 * @tc.name: testHdiUsbRequestTestSetInterface065
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},255,1).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface065, TestSize.Level2)
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
 * @tc.number: SUB_Driver_Usb_RequestTest_SetInterface_7400
 * @tc.name: testHdiUsbRequestTestSetInterface066
 * @tc.desc: Sets the alternate settings for the specified USB interface. ({255,100},255,255).
 */
HWTEST_F(UsbdInterfaceTestAdditional, testHdiUsbRequestTestSetInterface066, TestSize.Level2)
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