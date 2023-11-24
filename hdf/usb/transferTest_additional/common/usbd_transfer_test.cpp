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
#include "securec.h"
#include "usbd_transfer_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

const int SLEEP_TIME = 3;
const uint32_t MAX_BUFFER_LENGTH = 255;
const uint8_t SAMPLE_DATA_1 = 1;
const uint8_t SAMPLE_DATA_2 = 2;
const uint8_t SAMPLE_DATA_3 = 3;
const int32_t TRANSFER_TIME_OUT = 1000;
const int32_t CTL_VALUE = 0x100;

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

UsbDev UsbdTransferTestAdditional::dev_ = {0, 0};
sptr<UsbSubscriberTest> UsbdTransferTestAdditional::subscriber_ = nullptr;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

int32_t SwitchErrCode(int32_t ret) { return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret; }

void UsbdTransferTestAdditional::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    const int32_t DEFAULT_PORT_ID = 1;
    const int32_t DEFAULT_ROLE_HOST = 1;
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, DEFAULT_ROLE_HOST, DEFAULT_ROLE_HOST);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdTransferTestAdditional::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }

    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    if (g_usbInterface->BindUsbdSubscriber(subscriber_) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber_ failed\n", __func__);
        exit(0);
    }

    std::cout << "please connect device, press enter to continue" << std::endl;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }

    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};
    ret = g_usbInterface->OpenDevice(dev_);
    HDF_LOGI("UsbdTransferTestAdditional:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdTransferTestAdditional::TearDownTestCase(void)
{
    g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};
    auto ret = g_usbInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdTransferTestAdditional:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdTransferTestAdditional::SetUp(void) {}

void UsbdTransferTestAdditional::TearDown(void) {}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_2100
 * @tc.name: testHdiusbtransferTestControlTransferWrite001
 * @tc.desc: Performs control transfer for endpoint 0 of the device.
 * dev = (0,255). ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite001, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_2200
 * @tc.name: testHdiusbtransferTestControlTransferWrite002
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (0,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite002, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR,
                                         0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_2300
 * @tc.name: testHdiusbtransferTestControlTransferWrite003
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (0,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_CONFIGURATION, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite003, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE,
                                         USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_2400
 * @tc.name: testHdiusbtransferTestControlTransferWrite004
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (0,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite004, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE, 0,
                                         0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_2500
 * @tc.name: testHdiusbtransferTestControlTransferWrite005
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (0,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite005, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME, 0,
                                         0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_2600
 * @tc.name: testHdiusbtransferTestControlTransferWrite006
 * @tc.desc: Performs control transfer for endpoint 0 of the device.
 * dev = (255,0). ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite006, TestSize.Level2)
{
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_2700
 * @tc.name: testHdiusbtransferTestControlTransferWrite007
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,0).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite007, TestSize.Level2)
{
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR,
                                         0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_2800
 * @tc.name: testHdiusbtransferTestControlTransferWrite008
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,0).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_CONFIGURATION, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite008, TestSize.Level2)
{
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE,
                                         USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_2900
 * @tc.name: testHdiusbtransferTestControlTransferWrite009
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,0).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite009, TestSize.Level2)
{
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE, 0,
                                         0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3000
 * @tc.name: testHdiusbtransferTestControlTransferWrite010
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,0).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite010, TestSize.Level2)
{
    struct UsbDev dev = {255, 0};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME, 0,
                                         0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3100
 * @tc.name: testHdiusbtransferTestControlTransferWrite011
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, 0, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite011, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, 0, 0, 0,
                                         TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3200
 * @tc.name: testHdiusbtransferTestControlTransferWrite012
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_GET_DESCRIPTOR, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite012, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE,
                                         USB_DDK_REQ_GET_DESCRIPTOR, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3300
 * @tc.name: testHdiusbtransferTestControlTransferWrite013
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_GET_CONFIGURATION, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite013, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE,
                                         USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3400
 * @tc.name: testHdiusbtransferTestControlTransferWrite014
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_GET_INTERFACE, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite014, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_GET_INTERFACE,
                                         0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3500
 * @tc.name: testHdiusbtransferTestControlTransferWrite015
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_SYNCH_FRAME, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite015, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_SYNCH_FRAME,
                                         0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3600
 * @tc.name: testHdiusbtransferTestControlTransferWrite016
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, 0, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite016, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, 0, 0, 0,
                                         TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3700
 * @tc.name: testHdiusbtransferTestControlTransferWrite017
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_DESCRIPTOR, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite017, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_DESCRIPTOR,
                                         0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3800
 * @tc.name: testHdiusbtransferTestControlTransferWrite018
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_CONFIGURATION, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite018, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT,
                                         USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_3900
 * @tc.name: testHdiusbtransferTestControlTransferWrite019
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_INTERFACE, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite019, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_INTERFACE,
                                         0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4000
 * @tc.name: testHdiusbtransferTestControlTransferWrite020
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite020, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME, 0,
                                         0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4100
 * @tc.name: testHdiusbtransferTestControlTransferWrite021
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, 0, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite021, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, 0, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4200
 * @tc.name: testHdiusbtransferTestControlTransferWrite022
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_DESCRIPTOR, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite022, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_DESCRIPTOR, 0,
                                         0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4300
 * @tc.name: testHdiusbtransferTestControlTransferWrite023
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_CONFIGURATION, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite023, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_CONFIGURATION,
                                         0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4400
 * @tc.name: testHdiusbtransferTestControlTransferWrite024
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_INTERFACE, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite024, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_INTERFACE, 0,
                                         0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4500
 * @tc.name: testHdiusbtransferTestControlTransferWrite025
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_SYNCH_FRAME, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite025, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_SYNCH_FRAME, 0, 0,
                                         TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4600
 * @tc.name: testHdiusbtransferTestControlTransferWrite026
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite026, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4700
 * @tc.name: testHdiusbtransferTestControlTransferWrite027
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR, 0, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite027, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR,
                                         0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4800
 * @tc.name: testHdiusbtransferTestControlTransferWrite028
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_CONFIGURATION, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite028, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE,
                                         USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_4900
 * @tc.name: testHdiusbtransferTestControlTransferWrite029
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite029, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE, 0,
                                         0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5000
 * @tc.name: testHdiusbtransferTestControlTransferWrite030
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME, 0, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite030, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME, 0,
                                         0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5100
 * @tc.name: testHdiusbtransferTestControlTransferWrite031
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, CTL_VALUE, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite031, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, CTL_VALUE, 0,
                                         TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5200
 * @tc.name: testHdiusbtransferTestControlTransferWrite032
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite032, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5300
 * @tc.name: testHdiusbtransferTestControlTransferWrite033
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_CONFIGURATION, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite033, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE,
                                         USB_DDK_REQ_GET_CONFIGURATION, CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5400
 * @tc.name: testHdiusbtransferTestControlTransferWrite034
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite034, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5500
 * @tc.name: testHdiusbtransferTestControlTransferWrite035
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite035, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5600
 * @tc.name: testHdiusbtransferTestControlTransferWrite036
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, 0, CTL_VALUE, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite036, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, 0, CTL_VALUE, 0,
                                         TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5700
 * @tc.name: testHdiusbtransferTestControlTransferWrite037
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite037, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE,
                                         USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5800
 * @tc.name: testHdiusbtransferTestControlTransferWrite038
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_GET_CONFIGURATION, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite038, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE,
                                         USB_DDK_REQ_GET_CONFIGURATION, CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_5900
 * @tc.name: testHdiusbtransferTestControlTransferWrite039
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_GET_INTERFACE, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite039, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_GET_INTERFACE,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6000
 * @tc.name: testHdiusbtransferTestControlTransferWrite040
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_SYNCH_FRAME, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite040, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, USB_DDK_REQ_SYNCH_FRAME,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6100
 * @tc.name: testHdiusbtransferTestControlTransferWrite041
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, 0, CTL_VALUE, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite041, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, 0, CTL_VALUE, 0,
                                         TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6200
 * @tc.name: testHdiusbtransferTestControlTransferWrite042
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite042, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_DESCRIPTOR,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6300
 * @tc.name: testHdiusbtransferTestControlTransferWrite043
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_CONFIGURATION, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite043, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT,
                                         USB_DDK_REQ_GET_CONFIGURATION, CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6400
 * @tc.name: testHdiusbtransferTestControlTransferWrite044
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_INTERFACE, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite044, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_GET_INTERFACE,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6500
 * @tc.name: testHdiusbtransferTestControlTransferWrite045
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite045, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6600
 * @tc.name: testHdiusbtransferTestControlTransferWrite046
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, 0, CTL_VALUE, 0,TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite046, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, 0, CTL_VALUE, 0,
                                         TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6700
 * @tc.name: testHdiusbtransferTestControlTransferWrite047
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE,
 * 0,TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite047, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_DESCRIPTOR,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6800
 * @tc.name: testHdiusbtransferTestControlTransferWrite048
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_CONFIGURATION, CTL_VALUE,
 * 0,TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite048, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_CONFIGURATION,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_6900
 * @tc.name: testHdiusbtransferTestControlTransferWrite049
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_INTERFACE, CTL_VALUE,
 * 0,TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite049, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_GET_INTERFACE,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_7000
 * @tc.name: testHdiusbtransferTestControlTransferWrite050
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (255,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_SYNCH_FRAME, CTL_VALUE,
 * 0,TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite050, TestSize.Level2)
{
    struct UsbDev dev = {255, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_OTHER, USB_DDK_REQ_SYNCH_FRAME,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_7100
 * @tc.name: testHdiusbtransferTestControlTransferWrite051
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (0,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, CTL_VALUE, 0, TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite051, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, 0, CTL_VALUE, 0,
                                         TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_7200
 * @tc.name: testHdiusbtransferTestControlTransferWrite052
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (0,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite052, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_DESCRIPTOR,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_7300
 * @tc.name: testHdiusbtransferTestControlTransferWrite053
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (0,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_CONFIGURATION, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite053, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE,
                                         USB_DDK_REQ_GET_CONFIGURATION, CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_7400
 * @tc.name: testHdiusbtransferTestControlTransferWrite054
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (0,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite054, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_GET_INTERFACE,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_usb_transferTest_ControlTransferWrite_7500
 * @tc.name: testHdiusbtransferTestControlTransferWrite055
 * @tc.desc: Performs control transfer for endpoint 0 of the device. dev = (0,255).
 * ctrlparmas = (USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME, CTL_VALUE, 0,
 * TRANSFER_TIME_OUT).
 */
HWTEST_F(UsbdTransferTestAdditional, testHdiusbtransferTestControlTransferWrite055, TestSize.Level2)
{
    struct UsbDev dev = {0, 255};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_DEVICE, USB_DDK_REQ_SYNCH_FRAME,
                                         CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    ASSERT_NE(ret, 0);
}

} // namespace
