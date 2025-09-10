/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include "v2_0/iusb_host_interface.h"
#include "v2_0/iusb_port_interface.h"

const int SLEEP_TIME = 3;
const int CLAZZ_NUM = 9;
const uint8_t INTERFACEID_OK = 1;
const uint32_t MAX_BUFFER_LENGTH = 255;
const uint8_t POINTID_DIR_IN = USB_ENDPOINT_DIR_IN | 2;
const uint8_t POINTID_DIR_OUT = USB_ENDPOINT_DIR_OUT | 1;

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V2_0;

UsbDev UsbdRequestTestAdditional::dev_ = {0, 0};
sptr<UsbSubscriberTest> UsbdRequestTestAdditional::subscriber_ = nullptr;

namespace {
sptr<IUsbHostInterface> g_usbHostInterface = nullptr;
sptr<IUsbPortInterface> g_usbPortInterface = nullptr;

void UsbdRequestTestAdditional::SetUpTestCase(void)
{
    g_usbHostInterface = IUsbHostInterface::Get(true);
    g_usbPortInterface = IUsbPortInterface::Get();
    if (g_usbHostInterface == nullptr || g_usbPortInterface == nullptr) {
        HDF_LOGE("%{public}s:g_usbHostInterface or g_usbPortInterface is nullptr.", __func__);
        exit(0);
    }
    auto ret = g_usbPortInterface->SetPortRole(1, 1, 1);
    HDF_LOGI("UsbdRequestTestAdditional::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    if (ret != 0) {
        ASSERT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    } else {
        ASSERT_EQ(0, ret);
    }

    subscriber_ = new UsbSubscriberTest();
    if (g_usbHostInterface->BindUsbdHostSubscriber(subscriber_) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber_ failed", __func__);
        exit(0);
    }
    sleep(SLEEP_TIME);
    for (size_t i = 0; i < subscriber_->busInfos.size(); i++) {
        struct UsbDev dev = {subscriber_->busInfos[i].busNum, subscriber_->busInfos[i].devAddr};
        g_usbHostInterface->OpenDevice(dev);
        std::vector<uint8_t> rawData;
        auto ret = g_usbHostInterface->GetRawDescriptor(dev, rawData);
        ASSERT_EQ(0, ret);
        uint32_t deviceDescriptorSize = sizeof(UsbdDeviceDescriptor);
        if (rawData.size() < deviceDescriptorSize) {
            HDF_LOGE("%{public}s: rawData failed", __func__);
            exit(0);
            return ;
        }
    
        UsbdDeviceDescriptor deviceDescriptor = *(reinterpret_cast<const UsbdDeviceDescriptor *>(rawData.data()));
        if (deviceDescriptor.bLength != deviceDescriptorSize) {
            HDF_LOGE("UsbdDeviceDescriptor size error");
            exit(0);
        }
        if (deviceDescriptor.bDeviceClass != CLAZZ_NUM) {
            dev_ = dev;
        }
    }

    std::cout << "please connect device, press enter to continue" << std::endl;

    ret = g_usbHostInterface->OpenDevice(dev_);
    HDF_LOGI("UsbdRequestTestAdditional:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTestAdditional::TearDownTestCase(void)
{
    g_usbHostInterface->UnbindUsbdHostSubscriber(subscriber_);
    dev_ = {subscriber_->busNum_, subscriber_->devAddr_};
    auto ret = g_usbHostInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdRequestTestAdditional:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdRequestTestAdditional::SetUp(void) {}

void UsbdRequestTestAdditional::TearDown(void) {}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_0100
 * @tc.name: testHdiUsbRequestTestRequestWait001
 * @tc.desc: Waits for the operation result of the isochronous transfer request in <b>RequestQueue</b>.
 * (dev, waitData, bufferData, 1000).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestWait001, TestSize.Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> waitData(10);
    ret = g_usbHostInterface->RequestWait(dev, waitData, bufferData, 1000);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_0200
 * @tc.name: testHdiUsbRequestTestRequestWait002
 * @tc.desc: Waits for the operation result of the isochronous transfer request in <b>RequestQueue</b>.
 * ({1, 255}, waitData, bufferData, 1000).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestWait002, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> waitData(10);
    dev.busNum = 1;
    dev.devAddr = 255;
    ret = g_usbHostInterface->RequestWait(dev, waitData, bufferData, 1000);
    ASSERT_NE(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_0300
 * @tc.name: testHdiUsbRequestTestRequestWait003
 * @tc.desc: Waits for the operation result of the isochronous transfer request in <b>RequestQueue</b>.
 * ({255, 1}, waitData, bufferData, 1000).
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestWait003, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_IN;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    std::vector<uint8_t> waitData(10);
    dev.busNum = 255;
    dev.devAddr = 1;
    ret = g_usbHostInterface->RequestWait(dev, waitData, bufferData, 1000);
    ASSERT_NE(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_0400
 * @tc.name: testHdiUsbRequestTestRequestQueue001
 * @tc.desc: Sends or receives requests for isochronous transfer on a specified endpoint.
 * clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd', ','}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestQueue001, TestSize.Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd', ','};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_0500
 * @tc.name: testHdiUsbRequestTestRequestQueue002
 * @tc.desc: Sends or receives requests for isochronous transfer on a specified endpoint.
 * clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd', '!'}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestQueue002, TestSize.Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd', '!'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_0600
 * @tc.name: testHdiUsbRequestTestRequestQueue003
 * @tc.desc: Sends or receives requests for isochronous transfer on a specified endpoint.
 * clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd', '~'}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestQueue003, TestSize.Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd', '~'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_0700
 * @tc.name: testHdiUsbRequestTestRequestQueue004
 * @tc.desc: Sends or receives requests for isochronous transfer on a specified endpoint.
 * dev = {1, 255}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestQueue004, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    dev.busNum = 1;
    dev.devAddr = 255;
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_NE(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_0800
 * @tc.name: testHdiUsbRequestTestRequestQueue005
 * @tc.desc: Sends or receives requests for isochronous transfer on a specified endpoint.
 * dev = {255, 1}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestQueue005, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    dev.busNum = 255;
    dev.devAddr = 1;
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_NE(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_0900
 * @tc.name: testHdiUsbRequestTestRequestCancel001
 * @tc.desc: Cancels the data transfer requests to be processed.
 * pipe = {1, 255}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestCancel001, TestSize.Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    pipe.intfId = 1;
    pipe.endpointId = 255;
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
    pipe = {interfaceId, pointId};
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1000
 * @tc.name: testHdiUsbRequestTestRequestCancel002
 * @tc.desc: Cancels the data transfer requests to be processed.
 * pipe = {255, 1}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestCancel002, TestSize.Level1)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    pipe.intfId = 255;
    pipe.endpointId = 1;
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_NE(0, ret);
    pipe = {interfaceId, pointId};
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1100
 * @tc.name: testHdiUsbRequestTestRequestCancel003
 * @tc.desc: Cancels the data transfer requests to be processed.
 * dev = {255, 1}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestCancel003, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    dev.busNum = 255;
    dev.devAddr = 1;
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1200
 * @tc.name: testHdiUsbRequestTestRequestCancel004
 * @tc.desc: Cancels the data transfer requests to be processed.
 * dev = {1, 255}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestCancel004, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    dev.busNum = 1;
    dev.devAddr = 255;
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1300
 * @tc.name: testHdiUsbRequestTestRequestCancel005
 * @tc.desc: Cancels the data transfer requests to be processed.
 * dev = {1, 255}. pipe = {1, 255}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestCancel005, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    dev.busNum = 1;
    dev.devAddr = 255;
    pipe.intfId = 1;
    pipe.endpointId = 255;
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_NE(0, ret);
    dev = dev_;
    pipe = {interfaceId, pointId};
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1400
 * @tc.name: testHdiUsbRequestTestRequestCancel006
 * @tc.desc: Cancels the data transfer requests to be processed.
 * dev = {255, 1}. pipe = {255, 1}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestCancel006, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    dev.busNum = 255;
    dev.devAddr = 1;
    pipe.intfId = 255;
    pipe.endpointId = 1;
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_NE(0, ret);
    dev = dev_;
    pipe = {interfaceId, pointId};
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1500
 * @tc.name: testHdiUsbRequestTestRequestCancel007
 * @tc.desc: Cancels the data transfer requests to be processed.
 * dev = {255, 255}. pipe = {255, 255}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestRequestCancel007, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t pointId = POINTID_DIR_OUT;
    uint8_t interfaceId = INTERFACEID_OK;
    auto ret = g_usbHostInterface->ClaimInterface(dev, interfaceId, 1);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = {interfaceId, pointId};
    std::vector<uint8_t> clientData = {'q', 'u', 'e', 'u', 'e', 'r', 'e', 'a', 'd'};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbHostInterface->RequestQueue(dev, pipe, clientData, bufferData);
    ASSERT_EQ(0, ret);
    dev.busNum = 255;
    dev.devAddr = 255;
    pipe.intfId = 255;
    pipe.endpointId = 255;
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_NE(0, ret);
    dev = dev_;
    pipe = {interfaceId, pointId};
    ret = g_usbHostInterface->RequestCancel(dev, pipe);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1600
 * @tc.name: testHdiUsbRequestTestBulkCancel001
 * @tc.desc: Cancels the isochronous bulk transfer. dev ={1, 255}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestBulkCancel001, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbHostInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    ASSERT_EQ(ret, 0);
    dev = {1, 255};
    ret = g_usbHostInterface->BulkCancel(dev, pipe);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbHostInterface->UnRegBulkCallback(dev, pipe);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1700
 * @tc.name: testHdiUsbRequestTestBulkCancel002
 * @tc.desc: Cancels the isochronous bulk transfer. dev ={255, 1}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestBulkCancel002, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbHostInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    ASSERT_EQ(ret, 0);
    dev = {255, 1};
    ret = g_usbHostInterface->BulkCancel(dev, pipe);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbHostInterface->UnRegBulkCallback(dev, pipe);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1800
 * @tc.name: testHdiUsbRequestTestBulkCancel003
 * @tc.desc: Cancels the isochronous bulk transfer. dev ={255, 100}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestBulkCancel003, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbHostInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    ASSERT_EQ(ret, 0);
    dev = {255, 100};
    ret = g_usbHostInterface->BulkCancel(dev, pipe);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbHostInterface->UnRegBulkCallback(dev, pipe);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_USB_HostManager_HDI_CompatibilityRequestAddTest_1900
 * @tc.name: testHdiUsbRequestTestBulkCancel004
 * @tc.desc: Cancels the isochronous bulk transfer. dev ={100, 255}.
 */
HWTEST_F(UsbdRequestTestAdditional, testHdiUsbRequestTestBulkCancel004, TestSize.Level2)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_DIR_IN;
    struct UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbHostInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    ASSERT_EQ(ret, 0);
    dev = {100, 255};
    ret = g_usbHostInterface->BulkCancel(dev, pipe);
    ASSERT_NE(0, ret);
    dev = dev_;
    ret = g_usbHostInterface->UnRegBulkCallback(dev, pipe);
    ASSERT_EQ(ret, 0);
}
} // namespace
