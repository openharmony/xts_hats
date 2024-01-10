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

#include <iostream>
#include <vector>

#include "hdf_log.h"
#include "usbd_request_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

const int SLEEP_TIME = 3;
const uint8_t BUS_NUM_INVALID = 255;
const uint8_t DEV_ADDR_INVALID = 255;
const uint8_t INTERFACEID_OK = 1;
const uint8_t INTERFACEID_INVALID = 255;
const uint8_t INVALID_NUM = 222;
const uint8_t STRING_ID_INVALID = 233;
const uint32_t MAX_BUFFER_LENGTH = 255;
const uint8_t CONFIG_ID_INVALID = 222;
const uint8_t INDEX_1 = 1;

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

UsbDev UsbdRequestTestAdditional::dev_ = {0, 0};
sptr<UsbSubscriberTest> UsbdRequestTestAdditional::subscriber_ = nullptr;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

int32_t SwitchErrCode(int32_t ret)
{
    return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret;
}

void UsbdRequestTestAdditional::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(1, 1, 1);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdRequestTestAdditional::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
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
    HDF_LOGI("UsbdRequestTestAdditional:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTestAdditional::TearDownTestCase(void)
{
    g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};
    auto ret = g_usbInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdRequestTestAdditional:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTestAdditional::SetUp(void) {}

void UsbdRequestTestAdditional::TearDown(void) {}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_0900
 * @tc.name: testHdiUsbRequestTestClaimInterface001
 * @tc.desc: Claims a USB interface. ({255, 0}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface001, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1000
 * @tc.name: testHdiUsbRequestTestClaimInterface002
 * @tc.desc: Claims a USB interface. ({255, 0}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface002, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1100
 * @tc.name: testHdiUsbRequestTestClaimInterface003
 * @tc.desc: Claims a USB interface. ({255, 0}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface003, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1200
 * @tc.name: testHdiUsbRequestTestClaimInterface004
 * @tc.desc: Claims a USB interface. ({255, 0}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface004, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1300
 * @tc.name: testHdiUsbRequestTestClaimInterface005
 * @tc.desc: Claims a USB interface. ({255, 0}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface005, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1400
 * @tc.name: testHdiUsbRequestTestClaimInterface006
 * @tc.desc: Claims a USB interface. ({255, 0}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface006, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1500
 * @tc.name: testHdiUsbRequestTestClaimInterface007
 * @tc.desc: Claims a USB interface. ({255, 0}, 255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface007, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1600
 * @tc.name: testHdiUsbRequestTestClaimInterface008
 * @tc.desc: Claims a USB interface. ({255, 0}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface008, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1700
 * @tc.name: testHdiUsbRequestTestClaimInterface009
 * @tc.desc: Claims a USB interface. ({200, 255}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface009, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {200, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1800
 * @tc.name: testHdiUsbRequestTestClaimInterface010
 * @tc.desc: Claims a USB interface. ({200, 255}, 1, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface010, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {200, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_1900
 * @tc.name: testHdiUsbRequestTestClaimInterface011
 * @tc.desc: Claims a USB interface. ({100, 0}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface011, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2000
 * @tc.name: testHdiUsbRequestTestClaimInterface012
 * @tc.desc: Claims a USB interface. ({200, 255}, 0, 0)
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface012, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {200, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2100
 * @tc.name: testHdiUsbRequestTestClaimInterface013
 * @tc.desc: Claims a USB interface. ({200, 255}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface013, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {200, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2200
 * @tc.name: testHdiUsbRequestTestClaimInterface014
 * @tc.desc: Claims a USB interface. ({100, 0}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface014, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2300
 * @tc.name: testHdiUsbRequestTestClaimInterface015
 * @tc.desc: Claims a USB interface. ({100, 0}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface015, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2400
 * @tc.name: testHdiUsbRequestTestClaimInterface016
 * @tc.desc: Claims a USB interface. ({100, 0}, 255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface016, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2500
 * @tc.name: testHdiUsbRequestTestClaimInterface017
 * @tc.desc: Claims a USB interface. ({100, 0}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface017, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2600
 * @tc.name: testHdiUsbRequestTestClaimInterface018
 * @tc.desc: Claims a USB interface. ({255, 200}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface018, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {DEV_ADDR_INVALID, 200};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2700
 * @tc.name: testHdiUsbRequestTestClaimInterface019
 * @tc.desc: Claims a USB interface. ({255, 200}, 1, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface019, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {DEV_ADDR_INVALID, 200};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2800
 * @tc.name: testHdiUsbRequestTestClaimInterface020
 * @tc.desc: Claims a USB interface. ({1, 0}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface020, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_2900
 * @tc.name: testHdiUsbRequestTestClaimInterface021
 * @tc.desc: Claims a USB interface. ({255, 200}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface021, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {DEV_ADDR_INVALID, 200};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3000
 * @tc.name: testHdiUsbRequestTestClaimInterface022
 * @tc.desc: Claims a USB interface. ({255, 200}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface022, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {DEV_ADDR_INVALID, 200};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3100
 * @tc.name: testHdiUsbRequestTestClaimInterface023
 * @tc.desc: Claims a USB interface. ({1, 0}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface023, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3200
 * @tc.name: testHdiUsbRequestTestClaimInterface024
 * @tc.desc: Claims a USB interface. ({1, 0}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface024, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3300
 * @tc.name: testHdiUsbRequestTestClaimInterface025
 * @tc.desc: Claims a USB interface. ({1, 0}, 255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface025, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3400
 * @tc.name: testHdiUsbRequestTestClaimInterface026
 * @tc.desc: Claims a USB interface. ({1, 0}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface026, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3500
 * @tc.name: testHdiUsbRequestTestClaimInterface027
 * @tc.desc: Claims a USB interface. ({0, 255}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface027, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3600
 * @tc.name: testHdiUsbRequestTestClaimInterface028
 * @tc.desc: Claims a USB interface. ({0, 255}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface028, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3700
 * @tc.name: testHdiUsbRequestTestClaimInterface029
 * @tc.desc: Claims a USB interface. ({0, 255}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface029, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3800
 * @tc.name: testHdiUsbRequestTestClaimInterface030
 * @tc.desc: Claims a USB interface. ({0, 255}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface030, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_3900
 * @tc.name: testHdiUsbRequestTestClaimInterface031
 * @tc.desc: Claims a USB interface. ({0, 255}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface031, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4000
 * @tc.name: testHdiUsbRequestTestClaimInterface032
 * @tc.desc: Claims a USB interface. ({0, 255}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface032, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4100
 * @tc.name: testHdiUsbRequestTestClaimInterface033
 * @tc.desc: Claims a USB interface. ({0, 255}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface033, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4200
 * @tc.name: testHdiUsbRequestTestClaimInterface034
 * @tc.desc: Claims a USB interface. ({150, 255}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface034, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {150, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4300
 * @tc.name: testHdiUsbRequestTestClaimInterface035
 * @tc.desc: Claims a USB interface. ({150, 255}, 1, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface035, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {150, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4400
 * @tc.name: testHdiUsbRequestTestClaimInterface036
 * @tc.desc: Claims a USB interface. ({0, 100}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface036, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4500
 * @tc.name: testHdiUsbRequestTestClaimInterface037
 * @tc.desc: Claims a USB interface. ({150, 255}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface037, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {150, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4600
 * @tc.name: testHdiUsbRequestTestClaimInterface038
 * @tc.desc: Claims a USB interface. ({150, 255}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface038, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {150, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4700
 * @tc.name: testHdiUsbRequestTestClaimInterface039
 * @tc.desc: Claims a USB interface. ({0, 100}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface039, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4800
 * @tc.name: testHdiUsbRequestTestClaimInterface040
 * @tc.desc: Claims a USB interface. ({0, 100}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface040, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_4900
 * @tc.name: testHdiUsbRequestTestClaimInterface041
 * @tc.desc: Claims a USB interface. ({0, 100}, 255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface041, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5000
 * @tc.name: testHdiUsbRequestTestClaimInterface042
 * @tc.desc: Claims a USB interface. ({0, 100}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface042, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5100
 * @tc.name: testHdiUsbRequestTestClaimInterface043
 * @tc.desc: Claims a USB interface. ({0, 255}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface043, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5200
 * @tc.name: testHdiUsbRequestTestClaimInterface044
 * @tc.desc: Claims a USB interface. ({255, 0}, 100, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface044, TestSize.Level2)
{
    uint8_t interfaceId = 100;
    struct UsbDev dev = {DEV_ADDR_INVALID, dev_.devAddr};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5300
 * @tc.name: testHdiUsbRequestTestClaimInterface045
 * @tc.desc: Claims a USB interface. ({255, 0}, 100, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface045, TestSize.Level2)
{
    uint8_t interfaceId = 100;
    struct UsbDev dev = {DEV_ADDR_INVALID, dev_.devAddr};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5400
 * @tc.name: testHdiUsbRequestTestClaimInterface046
 * @tc.desc: Claims a USB interface. ({255, 0}, 100, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface046, TestSize.Level2)
{
    uint8_t interfaceId = 100;
    struct UsbDev dev = {DEV_ADDR_INVALID, dev_.devAddr};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5500
 * @tc.name: testHdiUsbRequestTestClaimInterface047
 * @tc.desc: Claims a USB interface. ({255, 0}, 1, 100).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface047, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {DEV_ADDR_INVALID, dev_.devAddr};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 100);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5600
 * @tc.name: testHdiUsbRequestTestClaimInterface048
 * @tc.desc: Claims a USB interface. ({255, 100}, 1, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface048, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {DEV_ADDR_INVALID, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5700
 * @tc.name: testHdiUsbRequestTestClaimInterface049
 * @tc.desc: Claims a USB interface. ({255, 100}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface049, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {DEV_ADDR_INVALID, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5800
 * @tc.name: testHdiUsbRequestTestClaimInterface050
 * @tc.desc: Claims a USB interface. ({255, 100}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface050, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {DEV_ADDR_INVALID, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_5900
 * @tc.name: testHdiUsbRequestTestClaimInterface051
 * @tc.desc: Claims a USB interface. ({255, 100}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface051, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {DEV_ADDR_INVALID, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_6000
 * @tc.name: testHdiUsbRequestTestClaimInterface052
 * @tc.desc: Claims a USB interface. ({255, 255},0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface052, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_6100
 * @tc.name: testHdiUsbRequestTestClaimInterface053
 * @tc.desc: Claims a USB interface. ({255, 255},0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface053, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_6200
 * @tc.name: testHdiUsbRequestTestClaimInterface054
 * @tc.desc: Claims a USB interface. ({255, 255},0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface054, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_6300
 * @tc.name: testHdiUsbRequestTestClaimInterface055
 * @tc.desc: Claims a USB interface. ({255, 255},1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface055, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_6400
 * @tc.name: testHdiUsbRequestTestClaimInterface056
 * @tc.desc: Claims a USB interface. ({255, 255},1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface056, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_6500
 * @tc.name: testHdiUsbRequestTestClaimInterface057
 * @tc.desc: Claims a USB interface. ({255, 255},255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface057, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ClaimInterface_6600
 * @tc.name: testHdiUsbRequestTestClaimInterface058
 * @tc.desc: Claims a USB interface. ({255, 255},255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestClaimInterface058, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_0900
 * @tc.name: testHdiUsbRequestTestSetConfig001
 * @tc.desc: Sets the configuration information of a USB device. ({100, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig001, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {100, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1000
 * @tc.name: testHdiUsbRequestTestSetConfig002
 * @tc.desc: Sets the configuration information of a USB device. ({100, 255},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig002, TestSize.Level2)
{
    uint8_t configIndex = 1;
    struct UsbDev dev = dev_;
    dev = {100, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1100
 * @tc.name: testHdiUsbRequestTestSetConfig003
 * @tc.desc: Sets the configuration information of a USB device. ({100, 255},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig003, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {100, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1200
 * @tc.name: testHdiUsbRequestTestSetConfig004
 * @tc.desc: Sets the configuration information of a USB device. ({1, 0},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig004, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {1, 0};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1300
 * @tc.name: testHdiUsbRequestTestSetConfig005
 * @tc.desc: Sets the configuration information of a USB device. ({255, 100},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig005, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {255, 100};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1400
 * @tc.name: testHdiUsbRequestTestSetConfig006
 * @tc.desc: Sets the configuration information of a USB device. ({255, 100},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig006, TestSize.Level2)
{
    uint8_t configIndex = 1;
    struct UsbDev dev = dev_;
    dev = {255, 100};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1500
 * @tc.name: testHdiUsbRequestTestSetConfig007
 * @tc.desc: Sets the configuration information of a USB device. ({0, 1},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig007, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {0, 1};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1600
 * @tc.name: testHdiUsbRequestTestSetConfig008
 * @tc.desc: Sets the configuration information of a USB device. ({255, 0},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig008, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {255, 0};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1700
 * @tc.name: testHdiUsbRequestTestSetConfig009
 * @tc.desc: Sets the configuration information of a USB device. ({255, 1},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig009, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {255, 1};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1800
 * @tc.name: testHdiUsbRequestTestSetConfig010
 * @tc.desc: Sets the configuration information of a USB device. ({255, 1},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig010, TestSize.Level2)
{
    uint8_t configIndex = 1;
    struct UsbDev dev = dev_;
    dev = {255, 1};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_1900
 * @tc.name: testHdiUsbRequestTestSetConfig011
 * @tc.desc: Sets the configuration information of a USB device. ({255, 1},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig011, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {255, 1};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_2000
 * @tc.name: testHdiUsbRequestTestSetConfig012
 * @tc.desc: Sets the configuration information of a USB device. ({0, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig012, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {0, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_2100
 * @tc.name: testHdiUsbRequestTestSetConfig013
 * @tc.desc: Sets the configuration information of a USB device. ({1, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig013, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {1, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_2200
 * @tc.name: testHdiUsbRequestTestSetConfig014
 * @tc.desc: Sets the configuration information of a USB device. ({1, 255},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig014, TestSize.Level2)
{
    uint8_t configIndex = 1;
    struct UsbDev dev = dev_;
    dev = {1, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_2300
 * @tc.name: testHdiUsbRequestTestSetConfig015
 * @tc.desc: Sets the configuration information of a USB device. ({1, 255},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig015, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {1, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_SetConfig_2400
 * @tc.name: testHdiUsbRequestTestSetConfig016
 * @tc.desc: Sets the configuration information of a USB device. ({255, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestSetConfig016, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {255, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_0900
 * @tc.name: testHdiUsbRequestTestReleaseInterface001
 * @tc.desc: Releases a USB interface. ({100, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface001, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {100, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1000
 * @tc.name: testHdiUsbRequestTestReleaseInterface002
 * @tc.desc: Releases a USB interface. ({100, 255},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface002, TestSize.Level2)
{
    int32_t interfaceId = 1;
    struct UsbDev dev = {100, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1100
 * @tc.name: testHdiUsbRequestTestReleaseInterface003
 * @tc.desc: Releases a USB interface. ({100, 255},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface003, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {100, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1200
 * @tc.name: testHdiUsbRequestTestReleaseInterface004
 * @tc.desc: Releases a USB interface. ({1, 0},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface004, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {1, 0};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1300
 * @tc.name: testHdiUsbRequestTestReleaseInterface005
 * @tc.desc: Releases a USB interface. ({255, 100},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface005, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {255, 100};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1400
 * @tc.name: testHdiUsbRequestTestReleaseInterface006
 * @tc.desc: Releases a USB interface. ({255, 100},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface006, TestSize.Level2)
{
    int32_t interfaceId = 1;
    struct UsbDev dev = {255, 100};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1500
 * @tc.name: testHdiUsbRequestTestReleaseInterface007
 * @tc.desc: Releases a USB interface. ({0, 1},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface007, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {0, 1};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1600
 * @tc.name: testHdiUsbRequestTestReleaseInterface008
 * @tc.desc: Releases a USB interface. ({255, 0},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface008, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {255, 0};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1700
 * @tc.name: testHdiUsbRequestTestReleaseInterface009
 * @tc.desc: Releases a USB interface. ({255, 1},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface009, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {255, 1};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1800
 * @tc.name: testHdiUsbRequestTestReleaseInterface010
 * @tc.desc: Releases a USB interface. ({255, 1},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface010, TestSize.Level2)
{
    int32_t interfaceId = 1;
    struct UsbDev dev = {255, 1};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_1900
 * @tc.name: testHdiUsbRequestTestReleaseInterface011
 * @tc.desc: Releases a USB interface. ({255, 1},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface011, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {255, 1};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_2000
 * @tc.name: testHdiUsbRequestTestReleaseInterface012
 * @tc.desc: Releases a USB interface. ({0, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface012, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {0, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_2100
 * @tc.name: testHdiUsbRequestTestReleaseInterface013
 * @tc.desc: Releases a USB interface. ({1, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface013, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {1, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_2200
 * @tc.name: testHdiUsbRequestTestReleaseInterface014
 * @tc.desc: Releases a USB interface. ({1, 255},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface014, TestSize.Level2)
{
    int32_t interfaceId = 1;
    struct UsbDev dev = {1, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_2300
 * @tc.name: testHdiUsbRequestTestReleaseInterface015
 * @tc.desc: Releases a USB interface. ({1, 255},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface015, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {1, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_ReleaseInterface_2400
 * @tc.name: testHdiUsbRequestTestReleaseInterface016
 * @tc.desc: Releases a USB interface. ({255, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestReleaseInterface016, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetDeviceDescriptor_0900
 * @tc.name: testHdiUsbRequestTestGetDeviceDescriptor001
 * @tc.desc: Obtains the USB device descriptor dev(255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetDeviceDescriptor001, TestSize.Level2)
{
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetDeviceDescriptor_1000
 * @tc.name: testHdiUsbRequestTestGetDeviceDescriptor002
 * @tc.desc: Obtains the USB device descriptor dev(1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetDeviceDescriptor002, TestSize.Level2)
{
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_0900
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor001
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(0, 255) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor001, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1000
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor002
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(0, 255) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor002, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1100
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor003
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(0, 255) stringId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor003, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1200
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor004
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 0) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor004, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1300
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor005
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 0) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor005, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1400
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor006
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 0) stringId =
 * STRING_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor006, TestSize.Level2)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1500
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor007
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 0) stringId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor007, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1600
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor008
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId = 0.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor008, TestSize.Level2)
{
    uint8_t stringId = 0;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1700
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor009
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor009, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1800
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor010
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor010, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_1900
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor011
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId =
 * STRING_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor011, TestSize.Level2)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_2000
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor012
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor012, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_2100
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor013
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId = 0.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor013, TestSize.Level2)
{
    uint8_t stringId = 0;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_2200
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor014
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor014, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_2300
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor015
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor015, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_2400
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor016
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId =
 * STRING_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor016, TestSize.Level2)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_2500
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor017
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor017, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_2600
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor018
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 255) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor018, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_2700
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor019
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 255) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor019, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetStringDescriptor_2800
 * @tc.name: testHdiUsbRequestTestGetStringDescriptor020
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 255) stringId =
 * 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetStringDescriptor020, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_0900
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor001
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(0, 255)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor001, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1000
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor002
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(0, 255)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor002, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1100
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor003
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(1, 255)
 * configId = 0.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor003, TestSize.Level2)
{
    uint8_t configId = 0;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1200
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor004
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(1, 255)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor004, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1300
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor005
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(1, 255)
 * configId = CONFIG_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor005, TestSize.Level2)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1400
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor006
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(1, 255)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor006, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1500
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor007
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 0)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor007, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1600
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor008
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 0)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor008, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1700
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor009
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 1)
 * configId = 0.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor009, TestSize.Level2)
{
    uint8_t configId = 0;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1800
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor010
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 1)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor010, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_1900
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor011
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 1)
 * configId = CONFIG_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor011, TestSize.Level2)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_2000
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor012
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 1)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor012, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_2100
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor013
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 255)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor013, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfigDescriptor_2200
 * @tc.name: testHdiUsbRequestTestGetConfigDescriptor014
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 255)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfigDescriptor014, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetRawDescriptor_0400
 * @tc.name: testHdiUsbRequestTestGetRawDescriptor001
 * @tc.desc: Obtains the raw descriptor. dev(1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetRawDescriptor001, TestSize.Level2)
{
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetRawDescriptor_0500
 * @tc.name: testHdiUsbRequestTestGetRawDescriptor002
 * @tc.desc: Obtains the raw descriptor. dev(255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetRawDescriptor002, TestSize.Level2)
{
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetRawDescriptor_0600
 * @tc.name: testHdiUsbRequestTestGetRawDescriptor003
 * @tc.desc: Obtains the raw descriptor. dev(255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetRawDescriptor003, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetFileDescriptor_0500
 * @tc.name: testHdiUsbRequestTestGetFileDescriptor001
 * @tc.desc: Obtains the file descriptor. dev(1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetFileDescriptor001, TestSize.Level2)
{
    struct UsbDev dev = {1, 255};
    int32_t fd = MAX_BUFFER_LENGTH;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetFileDescriptor_0600
 * @tc.name: testHdiUsbRequestTestGetFileDescriptor002
 * @tc.desc: Obtains the file descriptor. dev(255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetFileDescriptor002, TestSize.Level2)
{
    struct UsbDev dev = {255, 1};
    int32_t fd = MAX_BUFFER_LENGTH;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetFileDescriptor_0700
 * @tc.name: testHdiUsbRequestTestGetFileDescriptor003
 * @tc.desc: Obtains the file descriptor. dev(255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetFileDescriptor003, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    int32_t fd = MAX_BUFFER_LENGTH;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfig_0500
 * @tc.name: testHdiUsbRequestTestGetConfig001
 * @tc.desc: Obtains the configuration information of a USB device. dev(1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfig001, TestSize.Level2)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = {1, 255};
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfig_0600
 * @tc.name: testHdiUsbRequestTestGetConfig002
 * @tc.desc: Obtains the configuration information of a USB device. dev(255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfig002, TestSize.Level2)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = {255, 1};
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_RequestTest_GetConfig_0700
 * @tc.name: testHdiUsbRequestTestGetConfig003
 * @tc.desc: Obtains the configuration information of a USB device. dev(255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestGetConfig003, TestSize.Level2)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

} // namespace
