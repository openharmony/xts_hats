/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

int32_t SwitchErrCode(int32_t ret) { return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret; }

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
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_0900
 * @tc.name: testHdiusbrequestTestClaimInterface001
 * @tc.desc: Claims a USB interface. ({255, 0}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface001, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1000
 * @tc.name: testHdiusbrequestTestClaimInterface002
 * @tc.desc: Claims a USB interface. ({255, 0}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface002, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1100
 * @tc.name: testHdiusbrequestTestClaimInterface003
 * @tc.desc: Claims a USB interface. ({255, 0}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface003, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1200
 * @tc.name: testHdiusbrequestTestClaimInterface004
 * @tc.desc: Claims a USB interface. ({255, 0}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface004, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1300
 * @tc.name: testHdiusbrequestTestClaimInterface005
 * @tc.desc: Claims a USB interface. ({255, 0}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface005, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1400
 * @tc.name: testHdiusbrequestTestClaimInterface006
 * @tc.desc: Claims a USB interface. ({255, 0}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface006, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1500
 * @tc.name: testHdiusbrequestTestClaimInterface007
 * @tc.desc: Claims a USB interface. ({255, 0}, 255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface007, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1600
 * @tc.name: testHdiusbrequestTestClaimInterface008
 * @tc.desc: Claims a USB interface. ({255, 0}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface008, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = BUS_NUM_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1700
 * @tc.name: testHdiusbrequestTestClaimInterface009
 * @tc.desc: Claims a USB interface. ({200, 255}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface009, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {200, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1800
 * @tc.name: testHdiusbrequestTestClaimInterface010
 * @tc.desc: Claims a USB interface. ({200, 255}, 1, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface010, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {200, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_1900
 * @tc.name: testHdiusbrequestTestClaimInterface011
 * @tc.desc: Claims a USB interface. ({100, 0}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface011, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2000
 * @tc.name: testHdiusbrequestTestClaimInterface012
 * @tc.desc: Claims a USB interface. ({200, 255}, 0, 0)
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface012, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {200, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2100
 * @tc.name: testHdiusbrequestTestClaimInterface013
 * @tc.desc: Claims a USB interface. ({200, 255}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface013, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {200, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2200
 * @tc.name: testHdiusbrequestTestClaimInterface014
 * @tc.desc: Claims a USB interface. ({100, 0}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface014, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2300
 * @tc.name: testHdiusbrequestTestClaimInterface015
 * @tc.desc: Claims a USB interface. ({100, 0}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface015, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2400
 * @tc.name: testHdiusbrequestTestClaimInterface016
 * @tc.desc: Claims a USB interface. ({100, 0}, 255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface016, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2500
 * @tc.name: testHdiusbrequestTestClaimInterface017
 * @tc.desc: Claims a USB interface. ({100, 0}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface017, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 100;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2600
 * @tc.name: testHdiusbrequestTestClaimInterface018
 * @tc.desc: Claims a USB interface. ({255, 200}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface018, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {DEV_ADDR_INVALID, 200};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2700
 * @tc.name: testHdiusbrequestTestClaimInterface019
 * @tc.desc: Claims a USB interface. ({255, 200}, 1, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface019, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {DEV_ADDR_INVALID, 200};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2800
 * @tc.name: testHdiusbrequestTestClaimInterface020
 * @tc.desc: Claims a USB interface. ({1, 0}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface020, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_2900
 * @tc.name: testHdiusbrequestTestClaimInterface021
 * @tc.desc: Claims a USB interface. ({255, 200}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface021, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {DEV_ADDR_INVALID, 200};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3000
 * @tc.name: testHdiusbrequestTestClaimInterface022
 * @tc.desc: Claims a USB interface. ({255, 200}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface022, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {DEV_ADDR_INVALID, 200};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3100
 * @tc.name: testHdiusbrequestTestClaimInterface023
 * @tc.desc: Claims a USB interface. ({1, 0}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface023, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3200
 * @tc.name: testHdiusbrequestTestClaimInterface024
 * @tc.desc: Claims a USB interface. ({1, 0}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface024, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3300
 * @tc.name: testHdiusbrequestTestClaimInterface025
 * @tc.desc: Claims a USB interface. ({1, 0}, 255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface025, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3400
 * @tc.name: testHdiusbrequestTestClaimInterface026
 * @tc.desc: Claims a USB interface. ({1, 0}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface026, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = dev_;
    dev.busNum = 1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3500
 * @tc.name: testHdiusbrequestTestClaimInterface027
 * @tc.desc: Claims a USB interface. ({0, 255}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface027, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3600
 * @tc.name: testHdiusbrequestTestClaimInterface028
 * @tc.desc: Claims a USB interface. ({0, 255}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface028, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3700
 * @tc.name: testHdiusbrequestTestClaimInterface029
 * @tc.desc: Claims a USB interface. ({0, 255}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface029, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3800
 * @tc.name: testHdiusbrequestTestClaimInterface030
 * @tc.desc: Claims a USB interface. ({0, 255}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface030, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_3900
 * @tc.name: testHdiusbrequestTestClaimInterface031
 * @tc.desc: Claims a USB interface. ({0, 255}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface031, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4000
 * @tc.name: testHdiusbrequestTestClaimInterface032
 * @tc.desc: Claims a USB interface. ({0, 255}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface032, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4100
 * @tc.name: testHdiusbrequestTestClaimInterface033
 * @tc.desc: Claims a USB interface. ({0, 255}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface033, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4200
 * @tc.name: testHdiusbrequestTestClaimInterface034
 * @tc.desc: Claims a USB interface. ({150, 255}, 1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface034, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {150, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4300
 * @tc.name: testHdiusbrequestTestClaimInterface035
 * @tc.desc: Claims a USB interface. ({150, 255}, 1, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface035, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {150, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4400
 * @tc.name: testHdiusbrequestTestClaimInterface036
 * @tc.desc: Claims a USB interface. ({0, 100}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface036, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_OK;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4500
 * @tc.name: testHdiusbrequestTestClaimInterface037
 * @tc.desc: Claims a USB interface. ({150, 255}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface037, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {150, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4600
 * @tc.name: testHdiusbrequestTestClaimInterface038
 * @tc.desc: Claims a USB interface. ({150, 255}, 0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface038, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {150, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4700
 * @tc.name: testHdiusbrequestTestClaimInterface039
 * @tc.desc: Claims a USB interface. ({0, 100}, 0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface039, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4800
 * @tc.name: testHdiusbrequestTestClaimInterface040
 * @tc.desc: Claims a USB interface. ({0, 100}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface040, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_4900
 * @tc.name: testHdiusbrequestTestClaimInterface041
 * @tc.desc: Claims a USB interface. ({0, 100}, 255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface041, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5000
 * @tc.name: testHdiusbrequestTestClaimInterface042
 * @tc.desc: Claims a USB interface. ({0, 100}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface042, TestSize.Level2)
{
    uint8_t interfaceId = INTERFACEID_INVALID;
    struct UsbDev dev = {dev_.busNum, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5100
 * @tc.name: testHdiusbrequestTestClaimInterface043
 * @tc.desc: Claims a USB interface. ({0, 255}, 0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface043, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {dev_.busNum, DEV_ADDR_INVALID};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5200
 * @tc.name: testHdiusbrequestTestClaimInterface044
 * @tc.desc: Claims a USB interface. ({255, 0}, 100, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface044, TestSize.Level2)
{
    uint8_t interfaceId = 100;
    struct UsbDev dev = {DEV_ADDR_INVALID, dev_.devAddr};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5300
 * @tc.name: testHdiusbrequestTestClaimInterface045
 * @tc.desc: Claims a USB interface. ({255, 0}, 100, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface045, TestSize.Level2)
{
    uint8_t interfaceId = 100;
    struct UsbDev dev = {DEV_ADDR_INVALID, dev_.devAddr};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5400
 * @tc.name: testHdiusbrequestTestClaimInterface046
 * @tc.desc: Claims a USB interface. ({255, 0}, 100, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface046, TestSize.Level2)
{
    uint8_t interfaceId = 100;
    struct UsbDev dev = {DEV_ADDR_INVALID, dev_.devAddr};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5500
 * @tc.name: testHdiusbrequestTestClaimInterface047
 * @tc.desc: Claims a USB interface. ({255, 0}, 1, 100).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface047, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {DEV_ADDR_INVALID, dev_.devAddr};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 100);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5600
 * @tc.name: testHdiusbrequestTestClaimInterface048
 * @tc.desc: Claims a USB interface. ({255, 100}, 1, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface048, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {DEV_ADDR_INVALID, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5700
 * @tc.name: testHdiusbrequestTestClaimInterface049
 * @tc.desc: Claims a USB interface. ({255, 100}, 1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface049, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {DEV_ADDR_INVALID, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5800
 * @tc.name: testHdiusbrequestTestClaimInterface050
 * @tc.desc: Claims a USB interface. ({255, 100}, 255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface050, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {DEV_ADDR_INVALID, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_5900
 * @tc.name: testHdiusbrequestTestClaimInterface051
 * @tc.desc: Claims a USB interface. ({255, 100}, 255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface051, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {DEV_ADDR_INVALID, 100};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_6000
 * @tc.name: testHdiusbrequestTestClaimInterface052
 * @tc.desc: Claims a USB interface. ({255, 255},0, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface052, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_6100
 * @tc.name: testHdiusbrequestTestClaimInterface053
 * @tc.desc: Claims a USB interface. ({255, 255},0, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface053, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_6200
 * @tc.name: testHdiusbrequestTestClaimInterface054
 * @tc.desc: Claims a USB interface. ({255, 255},0, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface054, TestSize.Level2)
{
    uint8_t interfaceId = 0;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_6300
 * @tc.name: testHdiusbrequestTestClaimInterface055
 * @tc.desc: Claims a USB interface. ({255, 255},1, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface055, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_6400
 * @tc.name: testHdiusbrequestTestClaimInterface056
 * @tc.desc: Claims a USB interface. ({255, 255},1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface056, TestSize.Level2)
{
    uint8_t interfaceId = 1;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_6500
 * @tc.name: testHdiusbrequestTestClaimInterface057
 * @tc.desc: Claims a USB interface. ({255, 255},255, 0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface057, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 0);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ClaimInterface_6600
 * @tc.name: testHdiusbrequestTestClaimInterface058
 * @tc.desc: Claims a USB interface. ({255, 255},255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestClaimInterface058, TestSize.Level2)
{
    uint8_t interfaceId = 255;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 255);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_0900
 * @tc.name: testHdiusbrequestTestSetConfig001
 * @tc.desc: Sets the configuration information of a USB device. ({100, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig001, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {100, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1000
 * @tc.name: testHdiusbrequestTestSetConfig002
 * @tc.desc: Sets the configuration information of a USB device. ({100, 255},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig002, TestSize.Level2)
{
    uint8_t configIndex = 1;
    struct UsbDev dev = dev_;
    dev = {100, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1100
 * @tc.name: testHdiusbrequestTestSetConfig003
 * @tc.desc: Sets the configuration information of a USB device. ({100, 255},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig003, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {100, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1200
 * @tc.name: testHdiusbrequestTestSetConfig004
 * @tc.desc: Sets the configuration information of a USB device. ({1, 0},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig004, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {1, 0};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1300
 * @tc.name: testHdiusbrequestTestSetConfig005
 * @tc.desc: Sets the configuration information of a USB device. ({255, 100},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig005, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {255, 100};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1400
 * @tc.name: testHdiusbrequestTestSetConfig006
 * @tc.desc: Sets the configuration information of a USB device. ({255, 100},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig006, TestSize.Level2)
{
    uint8_t configIndex = 1;
    struct UsbDev dev = dev_;
    dev = {255, 100};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1500
 * @tc.name: testHdiusbrequestTestSetConfig007
 * @tc.desc: Sets the configuration information of a USB device. ({0, 1},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig007, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {0, 1};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1600
 * @tc.name: testHdiusbrequestTestSetConfig008
 * @tc.desc: Sets the configuration information of a USB device. ({255, 0},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig008, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {255, 0};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1700
 * @tc.name: testHdiusbrequestTestSetConfig009
 * @tc.desc: Sets the configuration information of a USB device. ({255, 1},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig009, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {255, 1};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1800
 * @tc.name: testHdiusbrequestTestSetConfig010
 * @tc.desc: Sets the configuration information of a USB device. ({255, 1},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig010, TestSize.Level2)
{
    uint8_t configIndex = 1;
    struct UsbDev dev = dev_;
    dev = {255, 1};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_1900
 * @tc.name: testHdiusbrequestTestSetConfig011
 * @tc.desc: Sets the configuration information of a USB device. ({255, 1},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig011, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {255, 1};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_2000
 * @tc.name: testHdiusbrequestTestSetConfig012
 * @tc.desc: Sets the configuration information of a USB device. ({0, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig012, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {0, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_2100
 * @tc.name: testHdiusbrequestTestSetConfig013
 * @tc.desc: Sets the configuration information of a USB device. ({1, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig013, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {1, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_2200
 * @tc.name: testHdiusbrequestTestSetConfig014
 * @tc.desc: Sets the configuration information of a USB device. ({1, 255},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig014, TestSize.Level2)
{
    uint8_t configIndex = 1;
    struct UsbDev dev = dev_;
    dev = {1, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_2300
 * @tc.name: testHdiusbrequestTestSetConfig015
 * @tc.desc: Sets the configuration information of a USB device. ({1, 255},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig015, TestSize.Level2)
{
    uint8_t configIndex = 255;
    struct UsbDev dev = dev_;
    dev = {1, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_SetConfig_2400
 * @tc.name: testHdiusbrequestTestSetConfig016
 * @tc.desc: Sets the configuration information of a USB device. ({255, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestSetConfig016, TestSize.Level2)
{
    uint8_t configIndex = 0;
    struct UsbDev dev = dev_;
    dev = {255, 255};
    auto ret = g_usbInterface->SetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_0900
 * @tc.name: testHdiusbrequestTestReleaseInterface001
 * @tc.desc: Releases a USB interface. ({100, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface001, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {100, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1000
 * @tc.name: testHdiusbrequestTestReleaseInterface002
 * @tc.desc: Releases a USB interface. ({100, 255},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface002, TestSize.Level2)
{
    int32_t interfaceId = 1;
    struct UsbDev dev = {100, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1100
 * @tc.name: testHdiusbrequestTestReleaseInterface003
 * @tc.desc: Releases a USB interface. ({100, 255},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface003, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {100, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1200
 * @tc.name: testHdiusbrequestTestReleaseInterface004
 * @tc.desc: Releases a USB interface. ({1, 0},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface004, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {1, 0};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1300
 * @tc.name: testHdiusbrequestTestReleaseInterface005
 * @tc.desc: Releases a USB interface. ({255, 100},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface005, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {255, 100};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1400
 * @tc.name: testHdiusbrequestTestReleaseInterface006
 * @tc.desc: Releases a USB interface. ({255, 100},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface006, TestSize.Level2)
{
    int32_t interfaceId = 1;
    struct UsbDev dev = {255, 100};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1500
 * @tc.name: testHdiusbrequestTestReleaseInterface007
 * @tc.desc: Releases a USB interface. ({0, 1},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface007, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {0, 1};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1600
 * @tc.name: testHdiusbrequestTestReleaseInterface008
 * @tc.desc: Releases a USB interface. ({255, 0},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface008, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {255, 0};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1700
 * @tc.name: testHdiusbrequestTestReleaseInterface009
 * @tc.desc: Releases a USB interface. ({255, 1},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface009, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {255, 1};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1800
 * @tc.name: testHdiusbrequestTestReleaseInterface010
 * @tc.desc: Releases a USB interface. ({255, 1},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface010, TestSize.Level2)
{
    int32_t interfaceId = 1;
    struct UsbDev dev = {255, 1};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_1900
 * @tc.name: testHdiusbrequestTestReleaseInterface011
 * @tc.desc: Releases a USB interface. ({255, 1},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface011, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {255, 1};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_2000
 * @tc.name: testHdiusbrequestTestReleaseInterface012
 * @tc.desc: Releases a USB interface. ({0, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface012, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {0, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_2100
 * @tc.name: testHdiusbrequestTestReleaseInterface013
 * @tc.desc: Releases a USB interface. ({1, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface013, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {1, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_2200
 * @tc.name: testHdiusbrequestTestReleaseInterface014
 * @tc.desc: Releases a USB interface. ({1, 255},1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface014, TestSize.Level2)
{
    int32_t interfaceId = 1;
    struct UsbDev dev = {1, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_2300
 * @tc.name: testHdiusbrequestTestReleaseInterface015
 * @tc.desc: Releases a USB interface. ({1, 255},255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface015, TestSize.Level2)
{
    int32_t interfaceId = 255;
    struct UsbDev dev = {1, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_ReleaseInterface_2400
 * @tc.name: testHdiusbrequestTestReleaseInterface016
 * @tc.desc: Releases a USB interface. ({255, 255},0).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestReleaseInterface016, TestSize.Level2)
{
    int32_t interfaceId = 0;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->ReleaseInterface(dev, interfaceId);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetDeviceDescriptor_0900
 * @tc.name: testHdiusbrequestTestGetDeviceDescriptor001
 * @tc.desc: Obtains the USB device descriptor dev(255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetDeviceDescriptor001, TestSize.Level2)
{
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetDeviceDescriptor_1000
 * @tc.name: testHdiusbrequestTestGetDeviceDescriptor002
 * @tc.desc: Obtains the USB device descriptor dev(1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetDeviceDescriptor002, TestSize.Level2)
{
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetDeviceDescriptor(dev, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_0900
 * @tc.name: testHdiusbrequestTestGetStringDescriptor001
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(0, 255) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor001, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1000
 * @tc.name: testHdiusbrequestTestGetStringDescriptor002
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(0, 255) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor002, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1100
 * @tc.name: testHdiusbrequestTestGetStringDescriptor003
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(0, 255) stringId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor003, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1200
 * @tc.name: testHdiusbrequestTestGetStringDescriptor004
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 0) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor004, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1300
 * @tc.name: testHdiusbrequestTestGetStringDescriptor005
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 0) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor005, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1400
 * @tc.name: testHdiusbrequestTestGetStringDescriptor006
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 0) stringId =
 * STRING_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor006, TestSize.Level2)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1500
 * @tc.name: testHdiusbrequestTestGetStringDescriptor007
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 0) stringId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor007, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1600
 * @tc.name: testHdiusbrequestTestGetStringDescriptor008
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId = 0.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor008, TestSize.Level2)
{
    uint8_t stringId = 0;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1700
 * @tc.name: testHdiusbrequestTestGetStringDescriptor009
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor009, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1800
 * @tc.name: testHdiusbrequestTestGetStringDescriptor010
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor010, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_1900
 * @tc.name: testHdiusbrequestTestGetStringDescriptor011
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId =
 * STRING_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor011, TestSize.Level2)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_2000
 * @tc.name: testHdiusbrequestTestGetStringDescriptor012
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(1, 255) stringId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor012, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_2100
 * @tc.name: testHdiusbrequestTestGetStringDescriptor013
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId = 0.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor013, TestSize.Level2)
{
    uint8_t stringId = 0;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_2200
 * @tc.name: testHdiusbrequestTestGetStringDescriptor014
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor014, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_2300
 * @tc.name: testHdiusbrequestTestGetStringDescriptor015
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor015, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_2400
 * @tc.name: testHdiusbrequestTestGetStringDescriptor016
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId =
 * STRING_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor016, TestSize.Level2)
{
    uint8_t stringId = STRING_ID_INVALID;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_2500
 * @tc.name: testHdiusbrequestTestGetStringDescriptor017
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 1) stringId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor017, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_2600
 * @tc.name: testHdiusbrequestTestGetStringDescriptor018
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 255) stringId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor018, TestSize.Level2)
{
    uint8_t stringId = 1;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_2700
 * @tc.name: testHdiusbrequestTestGetStringDescriptor019
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 255) stringId =
 * INVALID_NUM.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor019, TestSize.Level2)
{
    uint8_t stringId = INVALID_NUM;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetStringDescriptor_2800
 * @tc.name: testHdiusbrequestTestGetStringDescriptor020
 * @tc.desc: Obtains the string descriptor of a USB device based on the specified string ID. dev(255, 255) stringId =
 * 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetStringDescriptor020, TestSize.Level2)
{
    uint8_t stringId = 255;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetStringDescriptor(dev, stringId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_0900
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor001
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(0, 255)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor001, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1000
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor002
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(0, 255)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor002, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1100
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor003
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(1, 255)
 * configId = 0.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor003, TestSize.Level2)
{
    uint8_t configId = 0;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1200
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor004
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(1, 255)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor004, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1300
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor005
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(1, 255)
 * configId = CONFIG_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor005, TestSize.Level2)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1400
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor006
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(1, 255)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor006, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1500
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor007
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 0)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor007, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1600
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor008
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 0)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor008, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1700
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor009
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 1)
 * configId = 0.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor009, TestSize.Level2)
{
    uint8_t configId = 0;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1800
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor010
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 1)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor010, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_1900
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor011
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 1)
 * configId = CONFIG_ID_INVALID.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor011, TestSize.Level2)
{
    uint8_t configId = CONFIG_ID_INVALID;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_2000
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor012
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 1)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor012, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_2100
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor013
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 255)
 * configId = 1.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor013, TestSize.Level2)
{
    uint8_t configId = 1;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfigDescriptor_2200
 * @tc.name: testHdiusbrequestTestGetConfigDescriptor014
 * @tc.desc: Obtains the configuration descriptor of a USB device based on the specified config ID. dev(255, 255)
 * configId = 255.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfigDescriptor014, TestSize.Level2)
{
    uint8_t configId = 255;
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> devData(MAX_BUFFER_LENGTH);
    auto ret = g_usbInterface->GetConfigDescriptor(dev, configId, devData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetRawDescriptor_0400
 * @tc.name: testHdiusbrequestTestGetRawDescriptor001
 * @tc.desc: Obtains the raw descriptor. dev(1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetRawDescriptor001, TestSize.Level2)
{
    struct UsbDev dev = {1, 255};
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetRawDescriptor_0500
 * @tc.name: testHdiusbrequestTestGetRawDescriptor002
 * @tc.desc: Obtains the raw descriptor. dev(255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetRawDescriptor002, TestSize.Level2)
{
    struct UsbDev dev = {255, 1};
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetRawDescriptor_0600
 * @tc.name: testHdiusbrequestTestGetRawDescriptor003
 * @tc.desc: Obtains the raw descriptor. dev(255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetRawDescriptor003, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> rawData;
    auto ret = g_usbInterface->GetRawDescriptor(dev, rawData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetFileDescriptor_0500
 * @tc.name: testHdiusbrequestTestGetFileDescriptor001
 * @tc.desc: Obtains the file descriptor. dev(1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetFileDescriptor001, TestSize.Level2)
{
    struct UsbDev dev = {1, 255};
    int32_t fd = MAX_BUFFER_LENGTH;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetFileDescriptor_0600
 * @tc.name: testHdiusbrequestTestGetFileDescriptor002
 * @tc.desc: Obtains the file descriptor. dev(255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetFileDescriptor002, TestSize.Level2)
{
    struct UsbDev dev = {255, 1};
    int32_t fd = MAX_BUFFER_LENGTH;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetFileDescriptor_0700
 * @tc.name: testHdiusbrequestTestGetFileDescriptor003
 * @tc.desc: Obtains the file descriptor. dev(255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetFileDescriptor003, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    int32_t fd = MAX_BUFFER_LENGTH;
    auto ret = g_usbInterface->GetFileDescriptor(dev, fd);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfig_0500
 * @tc.name: testHdiusbrequestTestGetConfig001
 * @tc.desc: Obtains the configuration information of a USB device. dev(1, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfig001, TestSize.Level2)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = {1, 255};
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfig_0600
 * @tc.name: testHdiusbrequestTestGetConfig002
 * @tc.desc: Obtains the configuration information of a USB device. dev(255, 1).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfig002, TestSize.Level2)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = {255, 1};
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_requestTest_GetConfig_0700
 * @tc.name: testHdiusbrequestTestGetConfig003
 * @tc.desc: Obtains the configuration information of a USB device. dev(255, 255).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiusbrequestTestGetConfig003, TestSize.Level2)
{
    uint8_t configIndex = INDEX_1;
    struct UsbDev dev = {255, 255};
    auto ret = g_usbInterface->GetConfig(dev, configIndex);
    ASSERT_NE(ret, 0);
}

} // namespace
