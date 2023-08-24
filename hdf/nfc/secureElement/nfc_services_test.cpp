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

#include "se_impl.h"
#include <hdf_base.h>
#include <hdf_log.h>
#include <vector>
#include <gtest/gtest.h>
#include <hdf_device_desc.h>
#include <hdf_sbuf_ipc.h>
#include "v1_0/infc_interface.h"
#include "v1_0/nfc_types.h"
#include "v1_0/isecure_element_interface.h"
#include "v1_0/secure_element_types.h"
#include "v1_0/isecure_element_callback.h"
#include "nfc_chip_type_parser.h"
#include "common_utils.h"
// #include "se_terminal.h"

using namespace OHOS::HDI::SecureElement::V1_0;
using namespace testing::ext;
using namespace std;
using namespace OHOS::HDI::Nfc;
using ISeHdiV1_0 = OHOS::HDI::SecureElement::V1_0::ISecureElementInterface;
using OHOS::HDI::SecureElement::V1_0::SecureElementStatus;
using OHOS::HDI::SecureElement::V1_0::ISecureElementCallback;
using namespace OHOS::NFC;
using namespace OHOS::SE;
std::vector<uint8_t> vecCommand = {128, 202, 159, 127, 0};
// int MAX_APDU_RESP_BYTES = 512;
std::recursive_mutex mLock_{};
// tHAL_NFC_CBACK* NfcAdaptation::mSeHdi_Callback = nullptr;
// tHAL_NFC_DATA_CBACK* NfcAdaptation::mSeHdi_DataCallback = nullptr;

class SeClientCallback : public ISecureElementCallback {
public:
    explicit SeClientCallback() {

    }

    int32_t OnSeStateChanged(bool connected) override
    {
        return HDF_SUCCESS;
    }

};

OHOS::sptr<ISeHdiV1_0> mSeHdi_ = nullptr;
OHOS::sptr<ISecureElementCallback> mSeHdiCallback = nullptr;

class HdfNfcHdiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void HdfNfcHdiTest::SetUpTestCase()
{
    mSeHdi_ = ISeHdiV1_0::Get();
    mSeHdiCallback = new SeClientCallback();
}
void HdfNfcHdiTest::TearDownTestCase()
{
}
void HdfNfcHdiTest::SetUp()
{
}
void HdfNfcHdiTest::TearDown()
{
}

/**
* @tc.name: SUB_DriverSystem_Hdinfcopen_0100
* @tc.desc: Enables the nfc controller and initialize the nfc core.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_secureElement_0100, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSn110()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret = mSeHdi_->init(mSeHdiCallback, status);
        if (ret != HDF_SUCCESS) {
            printf("SeTerminal Error Initialize secure element hdf ret =  %d", ret);
        }
        if (status != SecureElementStatus::SE_SUCCESS) {
            // printf("SeTerminal::Error Initialize secure element hdf status = ", status);
            EXPECT_EQ(HDF_SUCCESS, 0);
        }
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcCoreInitialized_0200
* @tc.desc: Configures the nfc chip after initializing the nfc core.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_secureElement_0200, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSn110()) {
        printf("SeTerminal SUB_DriverSystem_secureElement_0200 1 ");
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mSeHdi_ == nullptr) {
        printf("SeTerminal SUB_DriverSystem_secureElement_0200 2");
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::vector<uint8_t> vecResponse(MAX_APDU_RESP_BYTES, 0);
        int32_t ret = mSeHdi_->getAtr(vecResponse);
        printf("SeTerminal SUB_DriverSystem_secureElement_0200 =  %d", ret);
        EXPECT_EQ(0, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcPrediscover_0300
* @tc.desc: Specifically configures the nfc chip before starting RF discovering.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_secureElement_0300, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSn110()) {
        printf("SeTerminal SUB_DriverSystem_secureElement_0300 1 ");
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mSeHdi_ == nullptr) {
        printf("SeTerminal SUB_DriverSystem_secureElement_0300 2 ");
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        bool present = false;
        int32_t ret = mSeHdi_->isSecureElementPresent(present);
        printf("SeTerminal SUB_DriverSystem_secureElement_0300 =  %d", ret);
        EXPECT_EQ(0, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcWrite_0400
* @tc.desc: Writes NCI data to the nfc core.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_secureElement_0400, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSn110()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        uint8_t channelNumber = 0;
        std::vector<uint8_t> vecAid;
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecResponse(MAX_APDU_RESP_BYTES, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret = mSeHdi_->openLogicalChannel(vecAid, p2, vecResponse, channelNumber, status);
        EXPECT_EQ(0, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcControlGranted_0500
* @tc.desc: Sets the HDF to allow to send NCI data.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_secureElement_0500, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSn110()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        uint8_t channelNumber = 0;
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret = mSeHdi_->closeChannel(channelNumber, status);
        EXPECT_EQ(0, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcPowerCycle_0600
* @tc.desc:  Restarts the nfc controller according to each power cycle.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_secureElement_0600, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSn110()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecAid;
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        std::vector<uint8_t> vecResponse(MAX_APDU_RESP_BYTES, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret = mSeHdi_->openBasicChannel(vecAid, p2, vecResponse, status);
        EXPECT_EQ(0, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcIoctl_0700
* @tc.desc: Sends I/O control commands and data from the nfc stack to HDI.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_secureElement_0700, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSn110()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecCommand;
        std::vector<uint8_t> vecResponse(MAX_APDU_RESP_BYTES, 0);
        CommonUtils::HexStringToBytesArray(aid, vecCommand);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret = mSeHdi_->transmit(vecCommand, vecResponse, status);
        EXPECT_EQ(0, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcClose_0800
* @tc.desc: Disables the nfc controller and releases the resource.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_secureElement_0800, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSn110()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret = mSeHdi_->reset(status);
        EXPECT_EQ(0, ret);
    }
}

