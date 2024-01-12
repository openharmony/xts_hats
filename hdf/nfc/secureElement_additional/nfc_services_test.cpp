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

#include "common_utils.h"
#include "nfc_chip_type_parser.h"
#include "se_impl.h"
#include "v1_0/infc_interface.h"
#include "v1_0/isecure_element_callback.h"
#include "v1_0/isecure_element_interface.h"
#include "v1_0/nfc_types.h"
#include "v1_0/secure_element_types.h"
#include <gtest/gtest.h>
#include <hdf_base.h>
#include <hdf_device_desc.h>
#include <hdf_log.h>
#include <hdf_sbuf_ipc.h>
#include <vector>

using namespace OHOS::HDI::SecureElement::V1_0;
using namespace testing::ext;
using namespace std;
using namespace OHOS::HDI::Nfc;
using ISeHdiV1_0 = OHOS::HDI::SecureElement::V1_0::ISecureElementInterface;
using OHOS::HDI::SecureElement::V1_0::ISecureElementCallback;
using OHOS::HDI::SecureElement::V1_0::SecureElementStatus;
using namespace OHOS::NFC;
using namespace OHOS::SE;
std::vector<uint8_t> vecCommand = {128, 202, 159, 127, 0};
std::recursive_mutex mLock_{};

class SeClientCallback : public ISecureElementCallback {
public:
    explicit SeClientCallback() {}

    int32_t OnSeStateChanged(bool connected) override { return HDF_SUCCESS; }
};

OHOS::sptr<ISeHdiV1_0> mSeHdi_ = nullptr;
OHOS::sptr<ISecureElementCallback> mSeHdiCallback = nullptr;

class HdfNfcHdiTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void HdfNfcHdiTestAdditional::SetUpTestCase()
{
    mSeHdi_ = ISeHdiV1_0::Get();
    mSeHdiCallback = new SeClientCallback();
}
void HdfNfcHdiTestAdditional::TearDownTestCase() {}
void HdfNfcHdiTestAdditional::SetUp() {}
void HdfNfcHdiTestAdditional::TearDown() {}

/**
 * @tc.number : SUB_Communication_Secureelement_GetAtr_0200
 * @tc.name   : testgetAtr001
 * @tc.desc   : Enables the nfc controller and initialize the nfc core.Cycle 10 times.
 */
HWTEST_F(HdfNfcHdiTestAdditional, testgetAtr001, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        printf("SeTerminal SUB_DriverSystem_secureElement_0200 1 ");
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            printf("SeTerminal SUB_DriverSystem_secureElement_0200 2");
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::vector<uint8_t> vecResponse(255, 0);
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->getAtr(vecResponse);
            printf("SeTerminal SUB_DriverSystem_secureElement_0200 =  %d", ret);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_OpenLogicalChannel_0200
 * @tc.name   : testOpenLogicalChannel001
 * @tc.desc   : check openLogicalChannel,vecAid = 0
 */
HWTEST_F(HdfNfcHdiTestAdditional, testOpenLogicalChannel001, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        uint8_t channelNumber = 0;
        std::vector<uint8_t> vecAid;
        vecAid.push_back(0);
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret = mSeHdi_->openLogicalChannel(vecAid, p2, vecResponse, channelNumber, status);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_OpenLogicalChannel_0300
 * @tc.name   : testOpenLogicalChannel002
 * @tc.desc   : check openLogicalChannel,vecAid = 1
 */
HWTEST_F(HdfNfcHdiTestAdditional, testOpenLogicalChannel002, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        uint8_t channelNumber = 0;
        std::vector<uint8_t> vecAid;
        vecAid.push_back(1);
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret = mSeHdi_->openLogicalChannel(vecAid, p2, vecResponse, channelNumber, status);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_OpenLogicalChannel_0400
 * @tc.name   : testOpenLogicalChannel003
 * @tc.desc   : check openLogicalChannel,vecAid = 0,cycle 10 times.
 */
HWTEST_F(HdfNfcHdiTestAdditional, testOpenLogicalChannel003, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        uint8_t channelNumber = 0;
        std::vector<uint8_t> vecAid;
        vecAid.push_back(0);
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->openLogicalChannel(vecAid, p2, vecResponse, channelNumber, status);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_OpenLogicalChannel_0300
 * @tc.name   : testOpenLogicalChannel004
 * @tc.desc   : check openLogicalChannel,vecAid = 1,cycle 10 times.
 */
HWTEST_F(HdfNfcHdiTestAdditional, testOpenLogicalChannel004, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        uint8_t channelNumber = 0;
        std::vector<uint8_t> vecAid;
        vecAid.push_back(1);
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->openLogicalChannel(vecAid, p2, vecResponse, channelNumber, status);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_OpenBasicChannel_0200
 * @tc.name   : testOpenBasicChannel001
 * @tc.desc   : check openBasicChannel,vecAid = 0
 */
HWTEST_F(HdfNfcHdiTestAdditional, testOpenBasicChannel001, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecAid;
        vecAid.push_back(0);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret = mSeHdi_->openBasicChannel(vecAid, p2, vecResponse, status);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_OpenBasicChannel_0300
 * @tc.name   : testOpenBasicChannel002
 * @tc.desc   : check openBasicChannel,vecAid = 1
 */
HWTEST_F(HdfNfcHdiTestAdditional, testOpenBasicChannel002, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecAid;
        vecAid.push_back(1);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret = mSeHdi_->openBasicChannel(vecAid, p2, vecResponse, status);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_OpenBasicChannel_0400
 * @tc.name   : testOpenBasicChannel003
 * @tc.desc   : check openBasicChannel,vecAid = 0,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testOpenBasicChannel003, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecAid;
        vecAid.push_back(0);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->openBasicChannel(vecAid, p2, vecResponse, status);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_OpenBasicChannel_0500
 * @tc.name   : testOpenBasicChannel004
 * @tc.desc   : check openBasicChannel,vecAid = 1,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testOpenBasicChannel004, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::lock_guard<std::recursive_mutex> lock(mLock_);
        int p2 = 0x00;
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecAid;
        vecAid.push_back(1);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecAid);
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->openBasicChannel(vecAid, p2, vecResponse, status);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_IsSecureElementPresent_0200
 * @tc.name   : testIsSecureElementPresent001
 * @tc.desc   : Specifically configures the nfc chip before starting RF discovering.
 */
HWTEST_F(HdfNfcHdiTestAdditional, testIsSecureElementPresent001, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        printf("SeTerminal SUB_DriverSystem_secureElement_0300 1 ");
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            printf("SeTerminal SUB_DriverSystem_secureElement_0300 2 ");
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        bool present = false;
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->isSecureElementPresent(present);
            printf("SeTerminal SUB_DriverSystem_secureElement_0300 =  %d", ret);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_CloseChannel_0200
 * @tc.name   : testCloseChannel001
 * @tc.desc   : check closeChannel,channelNumber = 1
 */
HWTEST_F(HdfNfcHdiTestAdditional, testCloseChannel001, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        uint8_t channelNumber = 1;
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret = mSeHdi_->closeChannel(channelNumber, status);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_CloseChannel_0300
 * @tc.name   : testCloseChannel002
 * @tc.desc   : check closeChannel,channelNumber = 0,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testCloseChannel002, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        uint8_t channelNumber = 0;
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->closeChannel(channelNumber, status);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_CloseChannel_0400
 * @tc.name   : testCloseChannel003
 * @tc.desc   : check closeChannel,channelNumber = 1,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testCloseChannel003, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        uint8_t channelNumber = 1;
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->closeChannel(channelNumber, status);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_Transmit_0200
 * @tc.name   : testTransmit001
 * @tc.desc   : check transmit,vecCommand = 0
 */
HWTEST_F(HdfNfcHdiTestAdditional, testTransmit001, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecCommand;
        vecCommand.push_back(0);
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecCommand);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret = mSeHdi_->transmit(vecCommand, vecResponse, status);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_Transmit_0300
 * @tc.name   : testTransmit002
 * @tc.desc   : check transmit,vecCommand = 1
 */
HWTEST_F(HdfNfcHdiTestAdditional, testTransmit002, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecCommand;
        vecCommand.push_back(1);
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecCommand);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret = mSeHdi_->transmit(vecCommand, vecResponse, status);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_Transmit_0400
 * @tc.name   : testTransmit003
 * @tc.desc   : check transmit,vecCommand = 0,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testTransmit003, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecCommand;
        vecCommand.push_back(0);
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecCommand);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->transmit(vecCommand, vecResponse, status);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_Transmit_0500
 * @tc.name   : testTransmit004
 * @tc.desc   : check transmit,vecCommand = 0,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testTransmit004, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        static const std::string aid = "A000000151000000";
        std::vector<uint8_t> vecCommand;
        vecCommand.push_back(1);
        std::vector<uint8_t> vecResponse(255, 0);
        CommonUtils::HexStringToBytesArray(aid, vecCommand);
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->transmit(vecCommand, vecResponse, status);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_Reset_0200
 * @tc.name   : testReset001
 * @tc.desc   : Disables the nfc controller and releases the resource,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testReset001, TestSize.Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->reset(status);
            EXPECT_EQ(0, ret);
        }
    }
}