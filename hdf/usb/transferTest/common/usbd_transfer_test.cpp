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
#include "securec.h"
#include "usbd_transfer_test.h"
#include "v1_2/iusb_interface.h"
#include "v1_2/usb_types.h"

const int SLEEP_TIME = 3;
const uint8_t BUS_NUM_INVALID = 255;
const uint8_t DEV_ADDR_INVALID = 255;
const uint32_t MAX_BUFFER_LENGTH = 255;
const uint8_t INTERFACEID_OK = 1;
const uint8_t PIPE_ENDPOINTID_INVALID = 244;
const uint8_t PIPE_INTERFACEID_INVALID = 244;
// data interface have 2 point : 1->bulk_out 2->bulk_in
const uint8_t POINTID_BULK_IN = USB_ENDPOINT_DIR_IN | 2;
const uint8_t POINTID_BULK_OUT = USB_ENDPOINT_DIR_OUT | 1;
const int32_t ASHMEM_MAX_SIZE = 1024;
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
using namespace OHOS::HDI::Usb::V1_2;

UsbDev UsbdTransferTest::dev_ = {0, 0};
sptr<UsbSubscriberTest> UsbdTransferTest::subscriber_ = nullptr;

namespace {
sptr<OHOS::HDI::Usb::V1_2::IUsbInterface> g_usbInterface = nullptr;

int32_t InitAshmemOne(sptr<Ashmem> &asmptr, int32_t asmSize, uint8_t rflg)
{
    asmptr = Ashmem::CreateAshmem("ttashmem000", asmSize);
    if (asmptr == nullptr) {
        HDF_LOGE("InitAshmemOne CreateAshmem failed\n");
        return HDF_FAILURE;
    }

    asmptr->MapReadAndWriteAshmem();

    if (rflg == 0) {
        uint8_t tdata[ASHMEM_MAX_SIZE];
        int32_t offset = 0;
        int32_t tlen = 0;

        int32_t retSafe = memset_s(tdata, sizeof(tdata), 'Y', ASHMEM_MAX_SIZE);
        if (retSafe != EOK) {
            HDF_LOGE("InitAshmemOne memset_s failed\n");
            return HDF_FAILURE;
        }
        while (offset < asmSize) {
            tlen = (asmSize - offset) < ASHMEM_MAX_SIZE ? (asmSize - offset) : ASHMEM_MAX_SIZE;
            asmptr->WriteToAshmem(tdata, tlen, offset);
            offset += tlen;
        }
    }
    return HDF_SUCCESS;
}

int32_t SwitchErrCode(int32_t ret)
{
    return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret;
}

void UsbdTransferTest::SetUpTestCase(void)
{
    g_usbInterface = OHOS::HDI::Usb::V1_2::IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    const int32_t DEFAULT_PORT_ID = 1;
    const int32_t DEFAULT_ROLE_HOST = 1;
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, DEFAULT_ROLE_HOST, DEFAULT_ROLE_HOST);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdTransferTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
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
    while ((c = getchar()) != '\n' && c != EOF) {}

    dev_ = { subscriber_->busNum_, subscriber_->devAddr_ };
    ret = g_usbInterface->OpenDevice(dev_);
    HDF_LOGI("UsbdTransferTest:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdTransferTest::TearDownTestCase(void)
{
    g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    dev_ = { subscriber_->busNum_, subscriber_->devAddr_ };
    auto ret = g_usbInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdTransferTest:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdTransferTest::SetUp(void) {}

void UsbdTransferTest::TearDown(void) {}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_0100
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, standard request: get configuration
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_0100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN, USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0100 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_0100
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error, standard request: get configuration
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_0100, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN, USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_0100 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_0200
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error, standard request: get configuration
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_0200, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN, USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_0200 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_0300
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, standard request: get descriptor(device)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_0300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN, USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_0300 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_0400
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error, standard request: get descriptor(device)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_0400, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN, USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_0400 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_0500
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error, standard request: get descriptor(device)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_0500, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN, USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_0500 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_0600
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, standard request: get interface
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_0600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t intercafeidex = 0;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_INTERFACE,
        USB_DDK_REQ_GET_INTERFACE, 0, intercafeidex, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_0600 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_0700
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error, standard request: get interface
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_0700, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    int32_t intercafeidex = 0;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_INTERFACE,
        USB_DDK_REQ_GET_INTERFACE, 0, intercafeidex, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_0700 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_0800
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error, standard request: get interface
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_0800, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    int32_t intercafeidex = 0;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_INTERFACE,
        USB_DDK_REQ_GET_INTERFACE, 0, intercafeidex, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_0800 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_0900
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, standard request: get status(recipient device)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_0900, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN, 0, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_0900 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1000
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error, standard request: get status(device)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1000, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN, 0, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1000 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1100
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error, standard request: get status(device)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1100, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN, 0, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1100 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1200
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, standard request: get status(interface)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_INTERFACE, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1200 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1300
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error, standard request: get status(interface)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1300, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_INTERFACE, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1300 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1400
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error, standard request: get status(interface)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1400, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_INTERFACE, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1400 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1500
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, standard request: get status(endpoint)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_ENDPOINT, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1500 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1600
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error, standard request: get status(endpoint)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1600, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_ENDPOINT, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1600 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1700
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error, standard request: get status(endpoint)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1700, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_ENDPOINT, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1700 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1800
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, standard request: sync frame
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME,
        0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1800 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_1900
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error, standard request: sync frame
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_1900, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME,
        0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_1900 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2000
 * @tc.desc: Test functions to ControlTransferRead
 * @tc.desc: int32_t ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error, standard request: sync frame
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2000, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_IN | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME,
        0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2000 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_0600
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_0600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT,
        USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0600 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6100
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6100, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE,
        USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6100 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6200
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6200, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE,
        USB_DDK_REQ_GET_CONFIGURATION, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6200 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6300
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT,
        USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6300 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6400
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6400, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE,
        USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6400 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6500
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6500, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE,
        USB_DDK_REQ_GET_DESCRIPTOR, CTL_VALUE, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6500 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6600
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t intercafeidex = 0;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {0, USB_DDK_REQ_GET_INTERFACE, 0, intercafeidex, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6600 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6700
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6700, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    int32_t intercafeidex = 0;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {0, USB_DDK_REQ_GET_INTERFACE, 0, intercafeidex, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6700 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6800
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6800, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    int32_t intercafeidex = 0;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {0, USB_DDK_REQ_GET_INTERFACE, 0, intercafeidex, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6800 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6900
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6900, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT, USB_DDK_REQ_GET_DESCRIPTOR, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6900 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7000
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7000, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7000 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7100
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7100, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_INTERFACE, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7100 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7200
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT, USB_DDK_REQ_GET_INTERFACE, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7200 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7300
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7300, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT, 0, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7300 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7400
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7400, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT, 0, 0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7400 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7500
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT, USB_DDK_REQ_GET_CONFIGURATION, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7500 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7600
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7600, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7600 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7700
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7700, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, 0, 0, 0,
        TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7700 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7800
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME,
        0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7800 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_7900
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_7900, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_INVALID, dev_.devAddr };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME,
        0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_7900 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8000
 * @tc.desc: Test functions to ControlTransferWrite
 * @tc.desc: int32_t ControlTransferWrite(const UsbDev &dev, UsbCtrlTransfer &ctrl, std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_8000, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_INVALID };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    bufferData.push_back(SAMPLE_DATA_1);
    bufferData.push_back(SAMPLE_DATA_2);
    bufferData.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = {USB_ENDPOINT_DIR_OUT | USB_REQUEST_TARGET_ENDPOINT, USB_DDK_REQ_SYNCH_FRAME,
        0, 0, TRANSFER_TIME_OUT};
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8000 "
        "%{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_0200
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_0200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0200 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0200 "
        "%{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2100
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2100 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2100 "
        "%{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2200
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2200 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2200 "
        "%{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2300
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2300 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2300 "
        "%{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2400
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, endpointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2400, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2400 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.endpointId = PIPE_ENDPOINTID_INVALID;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2400 "
        "%{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Func_2200
 * @tc.name     : UsbdBulkTransferReadwithLength001
 * @tc.desc     : Test functions to BulkTransferReadwithLength(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 *                std::vector<uint8_t> &data)
 * @tc.desc     : Positive test: parameters correctly
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdTransferTest, UsbdBulkTransferReadwithLength001, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointid = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength001 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointid};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferReadwithLength(dev, pipe, TRANSFER_TIME_OUT, bufferData.size(), bufferData);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength001 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_0140
 * @tc.name     : UsbdBulkTransferReadwithLength002
 * @tc.desc     : Test functions to BulkTransferReadwithLength(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 *                std::vector<uint8_t> &data)
 * @tc.desc     : Negative test: parameters exception, busNum error
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdTransferTest, UsbdBulkTransferReadwithLength002, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointid = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength002 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointid};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferReadwithLength(dev, pipe, TRANSFER_TIME_OUT, bufferData.size(), bufferData);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength002 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_0150
 * @tc.name     : UsbdBulkTransferReadwithLength003
 * @tc.desc     : Test functions to BulkTransferReadwithLength(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 *                std::vector<uint8_t> &data)
 * @tc.desc     : Negative test: parameters exception, devAddr error
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdTransferTest, UsbdBulkTransferReadwithLength003, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointid = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength003 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointid};
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferReadwithLength(dev, pipe, TRANSFER_TIME_OUT, bufferData.size(), bufferData);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength003 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_0160
 * @tc.name     : UsbdBulkTransferReadwithLength004
 * @tc.desc     : Test functions to BulkTransferReadwithLength(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 *                std::vector<uint8_t> &data)
 * @tc.desc     : Negative test: parameters exception, intfId error
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdTransferTest, UsbdBulkTransferReadwithLength004, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointid = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength004 %{public}d ClaimInterface=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointid};
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferReadwithLength(dev, pipe, TRANSFER_TIME_OUT, bufferData.size(), bufferData);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength004 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number   : SUB_USB_HostManager_HDI_Compatibility_0170
 * @tc.name     : UsbdBulkTransferReadwithLength005
 * @tc.desc     : Test functions to BulkTransferReadwithLength(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 *                std::vector<uint8_t> &data)
 * @tc.desc     : Negative test: parameters exception, endpointId error
 * @tc.size     : MediumTest
 * @tc.type     : Function
 * @tc.level    : Level 3
 */
HWTEST_F(UsbdTransferTest, UsbdBulkTransferReadwithLength005, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointid = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength005 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointid};
    pipe.endpointId = PIPE_ENDPOINTID_INVALID;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->BulkTransferReadwithLength(dev, pipe, TRANSFER_TIME_OUT, bufferData.size(), bufferData);
    HDF_LOGI("UsbdTransferTest::UsbdBulkTransferReadwithLength005 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_1100
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_1100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_1100 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'b', 'u', 'l', 'k', 'w', 'r', 'i', 't', 'e', '0', '1'};
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_1100 "
        "%{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2500
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2500 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.busNum = BUS_NUM_INVALID;
    std::vector<uint8_t> bufferData = {'b', 'u', 'l', 'k', 'w', 'r', 'i', 't', 'e', '0', '2'};
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2500 "
        "%{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2600
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2600 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'b', 'u', 'l', 'k', 'w', 'r', 'i', 't', 'e', '0', '3'};
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2600 "
        "%{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2700
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2700, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2700 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    std::vector<uint8_t> bufferData = {'b', 'u', 'l', 'k', 'w', 'r', 'i', 't', 'e', '0', '4'};
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2700 "
        "%{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2800
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, endpointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2800 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.endpointId = PIPE_ENDPOINTID_INVALID;
    std::vector<uint8_t> bufferData = {'b', 'u', 'l', 'k', 'w', 'r', 'i', 't', 'e', '0', '5'};
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2800 "
        "%{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_2900
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_2900, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2900 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    std::vector<uint8_t> bufferData = {'b', 'u', 'l', 'k', 'w', 'r', 'i', 't', 'e', '0', '6'};
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_2900 "
        "%{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3000
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, ClaimInterface failed first
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_3000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = PIPE_ENDPOINTID_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_3000 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'b', 'u', 'l', 'k', 'w', 'r', 'i', 't', 'e', '0', '7'};
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_3000 "
        "%{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3100
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, different in timeout
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_3100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_3100 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'b', 'u', 'l', 'k', 'w', 'r', 'i', 't', 'e', '0', '8'};
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, -1, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_3100 "
        "%{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_0400
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_0400, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0400 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0400 "
        "%{public}d UsbdInterruptTransferRead=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_3900
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_3900, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_3900 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_3900 "
        "%{public}d UsbdInterruptTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4000
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4000 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4100
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4100 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4100 "
        "%{public}d UsbdInterruptTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4200
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, endpointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4200 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.endpointId = PIPE_ENDPOINTID_INVALID;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4200 "
        "%{public}d UsbdInterruptTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_1000
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_1000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_1000 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'i', 'n', 't', 'w', 'r', 'i', 't', 'e', '0', '1'};
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_1000 "
        "%{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4300
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4300 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.busNum = BUS_NUM_INVALID;
    std::vector<uint8_t> bufferData = {'i', 'n', 't', 'w', 'r', 'i', 't', 'e', '0', '2'};
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4300 "
        "%{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4400
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4400, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4400 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'i', 'n', 't', 'w', 'r', 'i', 't', 'e', '0', '3'};
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4400 "
        "%{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4500
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4500 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    std::vector<uint8_t> bufferData = {'i', 'n', 't', 'w', 'r', 'i', 't', 'e', '0', '4'};
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4500 "
        "%{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4600
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, endpointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4600 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.endpointId = PIPE_ENDPOINTID_INVALID;
    std::vector<uint8_t> bufferData = {'i', 'n', 't', 'w', 'r', 'i', 't', 'e', '0', '5'};
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4600 "
        "%{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4700
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4700, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4700 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    std::vector<uint8_t> bufferData = {'i', 'n', 't', 'w', 'r', 'i', 't', 'e', '0', '6'};
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4700 "
        "%{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4800
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, ClaimInterface failed first
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = PIPE_ENDPOINTID_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4800 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'i', 'n', 't', 'w', 'r', 'i', 't', 'e', '0', '7'};
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4800 "
        "%{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_4900
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * Positive test: parameters correctly, different in timeout
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_4900, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4900 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'i', 'n', 't', 'w', 'r', 'i', 't', 'e', '0', '8'};
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, -1, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_4900 "
        "%{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_0500
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_0500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0500 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->IsoTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0500 "
        "%{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5000
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5000 "
        "%{public}d interfaceId=%{public}d", __LINE__, interfaceId);
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5000 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->IsoTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5000 "
        "%{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5100
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5100, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5100 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->IsoTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5100 "
        "%{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5200
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5200 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->IsoTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5200 "
        "%{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5300
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, endpointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5300 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.endpointId = PIPE_ENDPOINTID_INVALID;
    std::vector<uint8_t> bufferData(MAX_BUFFER_LENGTH);
    ret = g_usbInterface->IsoTransferRead(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5300 "
        "%{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_1200
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_1200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_1200 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'i', 's', 'o', 'w', 'r', 'i', 't', 'e', '0', '1'};
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_1200 "
        "%{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5400
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5400, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5400 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.busNum = BUS_NUM_INVALID;
    std::vector<uint8_t> bufferData = {'i', 's', 'o', 'w', 'r', 'i', 't', 'e', '0', '2'};
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5400 "
        "%{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5500
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5500 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = DEV_ADDR_INVALID;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ03";
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5500 "
        "%{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5600
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5600 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    std::vector<uint8_t> bufferData = {'i', 's', 'o', 'w', 'r', 'i', 't', 'e', '0', '4'};
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5600 "
        "%{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5700
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, endpointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5700, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5700 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.endpointId = PIPE_ENDPOINTID_INVALID;
    std::vector<uint8_t> bufferData = {'i', 's', 'o', 'w', 'r', 'i', 't', 'e', '0', '5'};
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5700 "
        "%{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5800
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, busNum && devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5800 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.busNum = BUS_NUM_INVALID;
    dev.devAddr = DEV_ADDR_INVALID;
    std::vector<uint8_t> bufferData = {'i', 's', 'o', 'w', 'r', 'i', 't', 'e', '0', '6'};
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5800 "
        "%{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_5900
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Negative test: parameters exception, endpointId error, ClaimInterface failed first
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_5900, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = PIPE_ENDPOINTID_INVALID;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5900 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'i', 's', 'o', 'w', 'r', 'i', 't', 'e', '0', '7'};
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, TRANSFER_TIME_OUT, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_5900 "
        "%{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_6000
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.desc: Positive test: parameters correctly, different in timeout
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_6000, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6000 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    std::vector<uint8_t> bufferData = {'i', 's', 'o', 'w', 'r', 'i', 't', 'e', '0', '8'};
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, -1, bufferData);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_6000 "
        "%{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_0800
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_0800, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0800 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0800 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8500
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_8500, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8500 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.busNum = BUS_NUM_INVALID;
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8500 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8600
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_8600, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8600 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.devAddr = DEV_ADDR_INVALID;
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8600 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8700
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_8700, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8700 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8700 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8800
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Negative test: parameters exception, endpointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_8800, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8800 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.endpointId = PIPE_ENDPOINTID_INVALID;
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8800 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranFunc_0900
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranFunc_0900, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_IN;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0900 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranFunc_0900 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_8900
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_8900, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8900 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.busNum = BUS_NUM_INVALID;
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_8900 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_9000
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_9000, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_9000 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    dev.devAddr = DEV_ADDR_INVALID;
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_9000 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_9100
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_9100, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_9100 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_9100 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_TranCompatibility_9200
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.desc: Negative test: parameters exception, endpointId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_TranCompatibility_9200, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = MAX_BUFFER_LENGTH;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_9200 "
        "%{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = { interfaceId, pointId };
    pipe.endpointId = PIPE_ENDPOINTID_INVALID;
    (void)InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_TranCompatibility_9200 "
        "%{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_1200
 * @tc.desc: Test functions to RegBulkCallback
 * @tc.desc: int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const sptr<IUsbdBulkCallback> &cb)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Func_1200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        HDF_LOGE("%{public}s:UsbdBulkCallbackTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Func_1200 "
        "%{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6200
 * @tc.desc: Test functions to RegBulkCallback
 * @tc.desc: int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const sptr<IUsbdBulkCallback> &cb)
 * @tc.desc: Negative test: parameters exception, busNum error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6200, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    dev.busNum = BUS_NUM_INVALID;
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        HDF_LOGE("%{public}s:UsbdBulkCallbackTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6200 "
        "%{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6300
 * @tc.desc: Test functions to RegBulkCallback
 * @tc.desc: int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const sptr<IUsbdBulkCallback> &cb)
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    dev.devAddr = DEV_ADDR_INVALID;
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        HDF_LOGE("%{public}s:UsbdBulkCallbackTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6300 "
        "%{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6400
 * @tc.desc: Test functions to RegBulkCallback
 * @tc.desc: int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const sptr<IUsbdBulkCallback> &cb)
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6400, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        HDF_LOGE("%{public}s:UsbdBulkCallbackTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::RegBulkCallback004 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6500
 * @tc.desc: Test functions to RegBulkCallback
 * @tc.desc: int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const sptr<IUsbdBulkCallback> &cb)
 * @tc.desc: Negative test: parameters exception, cb error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, nullptr);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6500 "
        "%{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_1300
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Func_1300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        HDF_LOGE("%{public}s:UsbdBulkCallbackTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Func_1300 "
        "%{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Func_1300 "
        "%{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6600
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.desc: Negative test: parameters exception, devAddr error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        HDF_LOGE("%{public}s:UsbdBulkCallbackTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6600 "
        "%{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    dev.devAddr = DEV_ADDR_INVALID;
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6600 "
        "%{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    dev = dev_;
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6600 "
        "%{public}d again UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6700
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.desc: Negative test: parameters exception, intfId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6700, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        HDF_LOGE("%{public}s:UsbdBulkCallbackTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6700 "
        "%{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    pipe.intfId = PIPE_INTERFACEID_INVALID;
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6700 "
        "%{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    pipe = {interfaceId, pointId};
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6700 "
        "%{public}d again UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6800
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.desc: Positive test: call again
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6800, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    if (usbdBulkCallback == nullptr) {
        HDF_LOGE("%{public}s:UsbdBulkCallbackTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6800 "
        "%{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6800 "
        "%{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
    ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6800 "
        "%{public}d again UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6900
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.desc: Positive test: no register
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6900, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_OK;
    uint8_t pointId = POINTID_BULK_OUT;
    OHOS::HDI::Usb::V1_0::UsbPipe pipe = {interfaceId, pointId};
    auto ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::UnRegBulkCallback005 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_1000
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Func_1000, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Func_1000 "
        "%{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5400
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: bind different
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_5400, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5400 "
        "%{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    sptr<UsbSubscriberTest> subscriber2 = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    ret = g_usbInterface->BindUsbdSubscriber(subscriber2);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5400 "
        "%{public}d again BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5500
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: bind same
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_5500, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5500 "
        "%{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5500 "
        "%{public}d again BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5600
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: bind and unbind, then bind another
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_5600, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5600 "
        "%{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5600 "
        "%{public}d UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    sptr<UsbSubscriberTest> subscriber2 = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    ret = g_usbInterface->BindUsbdSubscriber(subscriber2);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5600 "
        "again %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5700
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: bind again after unbind
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_5700, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5700 "
        "%{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5700 "
        "%{public}d UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5700 "
        "%{public}d again BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Func_1100
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Func_1100, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Func_1100 "
        "%{public}d first BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Func_1100 "
        "%{public}d UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5800
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Negative test: no bind first
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_5800, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5800 "
        "%{public}d UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_5900
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: no bind first, unbind failed; then bind, unbind success
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_5900, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5900 "
        "%{public}d UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
    ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI(
        "UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5900 %{public}d first BindUsbdSubscriber=%{public}d",
        __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_5900 "
        "%{public}d again UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6000
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Negative test: call twice
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6000, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6000 "
        "%{public}d first BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6000 "
        "%{public}d first UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6000 "
        "%{public}d again UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
}

/**
 * @tc.name: SUB_USB_HostManager_HDI_Compatibility_6100
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.desc: Positive test: test repeatedly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HostManager_HDI_Compatibility_6100, Function | MediumTest | Level1)
{
    subscriber_ = new UsbSubscriberTest();
    if (subscriber_ == nullptr) {
        HDF_LOGE("%{public}s:UsbSubscriberTest new failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6100 "
        "%{public}d first BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6100 "
        "%{public}d first UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->BindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6100 "
        "%{public}d again BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber_);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HostManager_HDI_Compatibility_6100 "
        "%{public}d again UnbindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}
} // namespace
