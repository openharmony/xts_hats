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
#include "securec.h"
#include "usbd_transfer_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

const int SLEEP_TIME = 3;
uint8_t BUS_NUM_1 = 0;
uint8_t DEV_ADDR_2 = 0;
const uint8_t BUS_NUM_255 = 255;
const uint8_t DEV_ADDR_255 = 255;
const uint8_t BUS_NUM_222 = 222;
const uint32_t LENGTH_NUM_255 = 255;
const uint8_t INTERFACEID_1 = 1;
const uint8_t POINTID_1 = 1;
const uint8_t POINTID_130 = 130;
const uint32_t ASHMEM_MAX_SIZE = 1024;
const uint8_t SAMPLE_DATA_1 = 1;
const uint8_t SAMPLE_DATA_2 = 2;
const uint8_t SAMPLE_DATA_3 = 3;

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::USB;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;
}

struct UsbDev UsbdTransferTest::dev_ = { 0, 0 };

int32_t InitAshmemOne(sptr<Ashmem>& asmptr, int32_t asmSize, uint8_t rflg)
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
    return 0;
}

void UsbdTransferTest::SetUpTestCase(void)
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
    HDF_LOGI("UsbdTransferTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
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
    HDF_LOGI("%{public}s: busNum:%{public}d, devAddr:%{public}d", __func__, subscriber->busNum_, subscriber->devAddr_);
    ret = g_usbInterface->OpenDevice(dev_);
    HDF_LOGI("UsbdTransferTest:: %{public}d OpenDevice=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdTransferTest::TearDownTestCase(void)
{
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    if (g_usbInterface->BindUsbdSubscriber(subscriber) != HDF_SUCCESS) {
        HDF_LOGE("%{public}s: bind usbd subscriber failed", __func__);
        exit(0);
    }
    dev_ = { subscriber->busNum_, subscriber->devAddr_ };
    auto ret = g_usbInterface->CloseDevice(dev_);
    HDF_LOGI("UsbdTransferTest:: %{public}d Close=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

void UsbdTransferTest::SetUp(void) {}

void UsbdTransferTest::TearDown(void) {}

/**
 * @tc.name: SUB_USB_HDI_0450
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.desc: Forward test: correct parameters
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0450, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 8, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0450 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0460
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.desc: Reverse test: BUS_NUM_255
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0460, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 8, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0460 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0470
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.desc: Reverse test: DEV_ADDR_255
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0470, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 8, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0470 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0480
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.desc: Forward test: correct parameters
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0480, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 6, 0x100, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0480 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0490
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0490, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 6, 0x100, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0490 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0500
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0500, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 6, 0x100, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0500 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0510
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0510, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t intercafeidex = 0;
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000001, 0X0A, 0, intercafeidex, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0510 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0520
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0520, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    int32_t intercafeidex = 0;
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000001, 0X0A, 0, intercafeidex, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0520 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0530
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0530, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    int32_t intercafeidex = 0;
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000001, 0X0A, 0, intercafeidex, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0530 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0540
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0540, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0540 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0550
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0550, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0550 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0560
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0560, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000000, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0560 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0570
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0570, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000001, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0570 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0580
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0580, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000001, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0580 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0590
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0590, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000001, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0590 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0600
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000010, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0600 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0610
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0610, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000010, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0610 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0620
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0620, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000010, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0620 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0630
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0630, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = {};
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000010, 0X0C, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0630 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0640
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0640, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = {};
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000010, 0X0C, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0640 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0650
 * @tc.desc: Test functions to ControlTransferRead(const UsbDev &dev, UsbCtrlTransfer &ctrl,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0650, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    uint32_t length = LENGTH_NUM_255;
    uint8_t buffer[LENGTH_NUM_255] = {};
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    struct UsbCtrlTransfer ctrlparmas = { 0b10000010, 0X0C, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferRead(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0650 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1780
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.desc: 正向测试：参数正确
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1780, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0001, 8, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1780 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1790
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.desc: 反向测试：BUS_NUM_255
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1790, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0001, 8, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1790 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1800
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.desc: 反向测试：DEV_ADDR_255
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1800, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0001, 8, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1800 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1810
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.desc: 反向测试：参数正确
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1810, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0001, 6, 0x100, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1810 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1820
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1820, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0001, 6, 0x100, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1820 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1830
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1830, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0001, 6, 0x100, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1830 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1840
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1840, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    int32_t intercafeidex = 0;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0, 0X0A, 0, intercafeidex, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1840 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1850
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1850, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    int32_t intercafeidex = 0;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0, 0X0A, 0, intercafeidex, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1850 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1860
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1860, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    int32_t intercafeidex = 0;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0, 0X0A, 0, intercafeidex, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1860 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1870
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1870, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0001, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1870 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1880
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1880, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0001, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1880 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1890
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1890, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0001, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1890 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1900
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1900, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1900 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1910
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1910, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1910 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1920
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1920, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1920 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1930
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1930, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0010, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1930 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1940
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1940, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0010, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1940 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1950
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1950, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0010, 0, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1950 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1960
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1960, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0010, 0X0C, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1960 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1970
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1970, Function | MediumTest | Level1)
{
    struct UsbDev dev = { BUS_NUM_255, dev_.devAddr };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0010, 0X0C, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1970 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1980
 * @tc.desc: Test functions to ControlTransferWrite(const UsbDev &dev, const UsbCtrlTransfer &ctrl,
 * const std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1980, Function | MediumTest | Level1)
{
    struct UsbDev dev = { dev_.busNum, DEV_ADDR_255 };
    std::vector<uint8_t> bufferdata(LENGTH_NUM_255);
    bufferdata.push_back(SAMPLE_DATA_1);
    bufferdata.push_back(SAMPLE_DATA_2);
    bufferdata.push_back(SAMPLE_DATA_3);
    struct UsbCtrlTransfer ctrlparmas = { 0b0010, 0X0C, 0, 0, 1000 };
    auto ret = g_usbInterface->ControlTransferWrite(dev, ctrlparmas, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1980 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0660
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0660, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0660 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0660 %{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0670
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0670, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0670 %{public}d ReleaseInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_222;
    uint32_t length = 100;
    uint8_t buffer[100] = { 0 };
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0670 %{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0680
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0680, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0680 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = 244;
    uint32_t length = 100;
    uint8_t buffer[100] = { 0 };
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0680 %{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0690
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0690, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0690 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t buffer[100] = { 0 };
    uint32_t length = 100;
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.intfId = 244;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0690 %{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0700
 * @tc.desc: Test functions to BulkTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0700, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0700 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint32_t length = 100;
    uint8_t buffer[100] = {};
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.endpointId = 244;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0700 %{public}d UsbdBulkTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0710
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0710, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0710 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ01";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0710 %{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_0720
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0720, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0720 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 99;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ02";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0720 %{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0730
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0730, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0730 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = 244;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ03";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0730 %{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0740
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0740, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0740 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.intfId = 255;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ04";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0740 %{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0750
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0750, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0750 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.endpointId = 255;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ05";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0750 %{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0760
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0760, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0760 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 99;
    dev.devAddr = 99;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ06";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0760 %{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0770
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0770, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = 99;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0770 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ07";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0770 %{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_0780
 * @tc.desc: Test functions to BulkTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_0780, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0780 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world bulk writ08";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->BulkTransferWrite(dev, pipe, -1, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_0780 %{public}d BulkTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1440
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1440, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1440 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1440 %{public}d UsbdInterruptTransferRead=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1450
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1450, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1450 %{public}d ReleaseInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_222;
    uint32_t length = 100;
    uint8_t buffer[100] = { 0 };
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1450 %{public}d UsbdInterruptTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1460
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1460, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1460 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = 244;
    uint32_t length = 100;
    uint8_t buffer[100] = { 0 };
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, 1000, bufferdata);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1470
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1470, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1470 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t buffer[100] = { 0 };
    uint32_t length = 100;
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.intfId = 244;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1470 %{public}d UsbdInterruptTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1480
 * @tc.desc: Test functions to InterruptTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1480, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1480 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint32_t length = 100;
    uint8_t buffer[100] = {};
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.endpointId = 244;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1480 %{public}d UsbdInterruptTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1490
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1490, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1490 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ01";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1490 %{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1500
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1500, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1500 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 99;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ02";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1500 %{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1510
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1510, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1510 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = 244;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ03";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1510 %{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1520
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1520, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1520 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.intfId = 255;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ04";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1520 %{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1530
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1530, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1530 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.endpointId = 255;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ05";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1530 %{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1540
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1540, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1540 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 99;
    dev.devAddr = 99;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ06";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1540 %{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1550
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1550, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = 99;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1550 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ07";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1550 %{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1560
 * @tc.desc: Test functions to InterruptTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1560, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1560 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Interrupt writ08";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->InterruptTransferWrite(dev, pipe, -1, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1560 %{public}d InterruptTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1570
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1570, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1570 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t buffer[LENGTH_NUM_255] = { 0 };
    uint32_t length = LENGTH_NUM_255;
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1570 %{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1580
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1580, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1580 %{public}d interfaceId=%{public}d", __LINE__, interfaceId);
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1580 %{public}d ReleaseInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.busNum = BUS_NUM_222;
    uint32_t length = 100;
    uint8_t buffer[100] = { 0 };
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1580 %{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1590
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1590, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1590 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = 244;
    uint32_t length = 100;
    uint8_t buffer[100] = { 0 };
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1590 %{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1600
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1600, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1600 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint8_t buffer[100] = { 0 };
    uint32_t length = 100;
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.intfId = 244;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1600 %{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1610
 * @tc.desc: Test functions to IsoTransferRead(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1610, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1610 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint32_t length = 100;
    uint8_t buffer[100] = {};
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.endpointId = 244;
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferRead(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1610 %{public}d UsbdIsoTransferRead=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1620
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1620, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1620 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ01";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1620 %{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1630
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1630, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1630 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 99;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ02";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1630 %{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1640
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1640, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1640 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    dev.devAddr = 244;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ03";
    struct UsbPipe pipe = { interfaceId, pointid };
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1640 %{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1650
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1650, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1650 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.intfId = 255;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ04";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1650 %{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1660
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1660, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1660 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.endpointId = 255;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ05";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1660 %{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1670
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1670, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1670 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 99;
    dev.devAddr = 99;
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ06";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1670 %{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1680
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1680, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = 99;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1680 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ07";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, 1000, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1680 %{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1690
 * @tc.desc: Test functions to IsoTransferWrite(const UsbDev &dev, const UsbPipe &pipe, int32_t timeout,
 * std::vector<uint8_t> &data);
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_1690, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1690 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    uint32_t length = 100;
    uint8_t buffer[100] = "hello world Iso writ08";
    std::vector<uint8_t> bufferdata = { buffer, buffer + length };
    ret = g_usbInterface->IsoTransferWrite(dev, pipe, -1, bufferdata);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_1690 %{public}d IsoTransferWrite=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2040
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2040, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2040 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2040 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2050
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2050, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2050 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 99;
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2050 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2060
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2060, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2060 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.devAddr = 244;
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2060 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2070
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2070, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2070 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.intfId = 255;
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2070 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2080
 * @tc.desc: Test functions to int32_t BulkWrite(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2080, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2080 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.endpointId = 255;
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkWrite(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2080 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2090
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2090, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_130;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2090 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2090 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2100
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2100, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2100 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 99;
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2100 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2110
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2110, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2110 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.devAddr = 244;
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2110 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2120
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2120, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2120 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.intfId = 255;
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2120 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2130
 * @tc.desc: Test functions to int32_t BulkRead(const UsbDev &dev, const UsbPipe &pipe, const sptr<Ashmem> &ashmem)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2130, Function | MediumTest | Level1)
{
    sptr<Ashmem> ashmem;
    uint8_t rflg = 0;
    int32_t asmSize = LENGTH_NUM_255;
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    auto ret = g_usbInterface->ClaimInterface(dev, interfaceId, true);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2130 %{public}d ClaimInterface=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.endpointId = 255;
    InitAshmemOne(ashmem, asmSize, rflg);
    ret = g_usbInterface->BulkRead(dev, pipe, ashmem);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2130 %{public}d BulkWrite=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2140
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2140, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(1, 100, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2140 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->BindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2140 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2150
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2150, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(100, 1, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2150 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->BindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2150 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2160
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2160, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(100, 100, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2160 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->BindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2160 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2170
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2170, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(100, 100, 100);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2170 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->BindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2170 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2180
 * @tc.desc: Test functions to int32_t BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2180, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(100, 1, 100);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2180 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->BindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2180 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2190
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2190, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(1, 100, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2190 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2190 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2200
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2200, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(100, 1, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2200 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2200 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2210
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2210, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(100, 100, 1);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2210 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2210 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2220
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2220, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(100, 100, 100);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2220 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2220 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2230
 * @tc.desc: Test functions to int32_t UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2230, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(100, 1, 100);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2230 [Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
    sptr<UsbSubscriberTest> subscriber = new UsbSubscriberTest();
    ret = g_usbInterface->UnbindUsbdSubscriber(subscriber);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2230 %{public}d BindUsbdSubscriber=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2240
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const
 * sptr<IUsbdBulkCallback> &cb)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2240, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.endpointId = 255;
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2240 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2250
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const
 * sptr<IUsbdBulkCallback> &cb)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2250, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 99;
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2250 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2260
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const
 * sptr<IUsbdBulkCallback> &cb)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2260, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.devAddr = 244;
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2260 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2270
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const
 * sptr<IUsbdBulkCallback> &cb)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2270, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = { interfaceId, pointid };
    pipe.intfId = 255;
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2270 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2280
 * @tc.desc: Test functions to int32_t RegBulkCallback(const UsbDev &dev, const UsbPipe &pipe, const
 * sptr<IUsbdBulkCallback> &cb)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2280, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = 99;
    struct UsbPipe pipe = { interfaceId, pointid };
    sptr<UsbdBulkCallbackTest> usbdBulkCallback = new UsbdBulkCallbackTest();
    auto ret = g_usbInterface->RegBulkCallback(dev, pipe, usbdBulkCallback);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2280 %{public}d RegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2290
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2290, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = BUS_NUM_222;
    auto ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2290 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2300
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2300, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.devAddr = 244;
    auto ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2300 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2310
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2310, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = 244;
    auto ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2310 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2320
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2320, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.devAddr = 99;
    auto ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2320 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_2330
 * @tc.desc: Test functions to int32_t UnRegBulkCallback(const UsbDev &dev, const UsbPipe &pipe)
 * @tc.type: FUNC
 */
HWTEST_F(UsbdTransferTest, SUB_USB_HDI_2330, Function | MediumTest | Level1)
{
    struct UsbDev dev = dev_;
    uint8_t interfaceId = INTERFACEID_1;
    uint8_t pointid = POINTID_1;
    struct UsbPipe pipe = { interfaceId, pointid };
    dev.busNum = BUS_NUM_222;
    dev.devAddr = 99;
    auto ret = g_usbInterface->UnRegBulkCallback(dev, pipe);
    HDF_LOGI("UsbdTransferTest::SUB_USB_HDI_2330 %{public}d UnRegBulkCallback=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}
