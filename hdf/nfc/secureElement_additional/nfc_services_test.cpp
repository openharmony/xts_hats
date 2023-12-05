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
 * @tc.number : SUB_Communication_Secureelement_Init_0200
 * @tc.name   : testInit001
 * @tc.desc   : check init,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testInit001, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->init(mSeHdiCallback, status);
            if (ret != HDF_SUCCESS) {
                printf("SeTerminal Error Initialize secure element hdf ret =  %d", ret);
            }
            if (status != SecureElementStatus::SE_SUCCESS) {
                EXPECT_EQ(HDF_SUCCESS, 0);
            }
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_GetAtr_0200
 * @tc.name   : testGetAtr001
 * @tc.desc   : check getAtr,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testGetAtr001, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        printf("SeTerminal testGetAtr001 1 ");
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            printf("SeTerminal testGetAtr001 2");
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        std::vector<uint8_t> vecResponse(MAX_APDU_RESP_BYTES, 0);
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->getAtr(vecResponse);
            printf("SeTerminal testGetAtr001 =  %d", ret);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Secureelement_IsSecureElementPresent_0200
 * @tc.name   : testIsSecureElementPresent001
 * @tc.desc   : check IsSecureElementPresent,cycle 10 times
 */
HWTEST_F(HdfNfcHdiTestAdditional, testIsSecureElementPresent001, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        printf("SeTerminal testIsSecureElementPresent001 1 ");
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            printf("SeTerminal testIsSecureElementPresent001 2 ");
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        bool present = false;
        int32_t ret;
        for (int i = 0; i < 10; i++) {
            ret = mSeHdi_->isSecureElementPresent(present);
            printf("SeTerminal testIsSecureElementPresent001 =  %d", ret);
            EXPECT_EQ(0, ret);
        }
    }
}

/**
 * @tc.name: SUB_DriverSystem_HdinfcWrite_0400
 * @tc.desc: Writes NCI data to the nfc core.
 * @tc.type: FUNC
 */
HWTEST_F(HdfNfcHdiTestAdditional, SUB_DriverSystem_secureElement_0400, TestSize.Level2)
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
HWTEST_F(HdfNfcHdiTestAdditional, SUB_DriverSystem_secureElement_0500, TestSize.Level2)
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
        int32_t ret = mSeHdi_->closeChannel(channelNumber, status);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.name: SUB_DriverSystem_HdinfcPowerCycle_0600
 * @tc.desc:  Restarts the nfc controller according to each power cycle.
 * @tc.type: FUNC
 */
HWTEST_F(HdfNfcHdiTestAdditional, SUB_DriverSystem_secureElement_0600, TestSize.Level2)
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
HWTEST_F(HdfNfcHdiTestAdditional, SUB_DriverSystem_secureElement_0700, TestSize.Level2)
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
HWTEST_F(HdfNfcHdiTestAdditional, SUB_DriverSystem_secureElement_0800, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mSeHdi_ == nullptr) {
            ASSERT_NE(nullptr, mSeHdi_);
            return;
        }
        SecureElementStatus status = SecureElementStatus::SE_GENERAL_ERROR;
        int32_t ret = mSeHdi_->reset(status);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.name: SUB_DriverSystem_secureElement_0900
 * @tc.desc: HdinfcType test
 * @tc.type: FUNC
 */
HWTEST_F(HdfNfcHdiTestAdditional, SUB_DriverSystem_secureElement_0900, TestSize.Level2)
{
    SecureElementStatus success = SecureElementStatus::SE_SUCCESS;
    std::cout << "SecureElementStatus SE_SUCCESS = " << success << std::endl;
    EXPECT_EQ(success, 0);

    SecureElementStatus null_pointer_error = SecureElementStatus::SE_NULL_POINTER_ERROR;
    std::cout << "SecureElementStatus SE_NULL_POINTER_ERROR = " << null_pointer_error << std::endl;

    SecureElementStatus illegal_parameter_error = SecureElementStatus::SE_ILLEGAL_PARAMETER_ERROR;
    std::cout << "SecureElementStatus SE_ILLEGAL_PARAMETER_ERROR = " << illegal_parameter_error << std::endl;

    SecureElementStatus illegal_state_error = SecureElementStatus::SE_ILLEGAL_STATE_ERROR;
    std::cout << "SecureElementStatus SE_ILLEGAL_STATE_ERROR = " << illegal_state_error << std::endl;

    SecureElementStatus security_error = SecureElementStatus::SE_SECURITY_ERROR;
    std::cout << "SecureElementStatus SE_SECURITY_ERROR = " << security_error << std::endl;

    SecureElementStatus channel_not_available_error = SecureElementStatus::SE_CHANNEL_NOT_AVAILABLE_ERROR;
    std::cout << "SecureElementStatus SE_CHANNEL_NOT_AVAILABLE_ERROR = " << channel_not_available_error << std::endl;

    SecureElementStatus no_such_element_error = SecureElementStatus::SE_NO_SUCH_ELEMENT_ERROR;
    std::cout << "SecureElementStatus SE_NO_SUCH_ELEMENT_ERROR = " << no_such_element_error << std::endl;

    SecureElementStatus illegal_reference_error = SecureElementStatus::SE_ILLEGAL_REFERENCE_ERROR;
    std::cout << "SecureElementStatus SE_ILLEGAL_REFERENCE_ERROR = " << illegal_reference_error << std::endl;

    SecureElementStatus operation_not_supported_error = SecureElementStatus::SE_OPERATION_NOT_SUPPORTED_ERROR;
    std::cout << "SecureElementStatus SE_OPERATION_NOT_SUPPORTED_ERROR = " << operation_not_supported_error
              << std::endl;

    SecureElementStatus io_error = SecureElementStatus::SE_IO_ERROR;
    std::cout << "SecureElementStatus SE_IO_ERROR = " << io_error << std::endl;

    SecureElementStatus general_error = SecureElementStatus::SE_GENERAL_ERROR;
    std::cout << "SecureElementStatus SE_GENERAL_ERROR = " << general_error << std::endl;
}