/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "nfc_impl.h"
#include <hdf_base.h>
#include <hdf_log.h>
#include <vector>
#include "nfc_vendor_adaptions.h"
#include <gtest/gtest.h>
#include <hdf_device_desc.h>
#include <hdf_sbuf_ipc.h>
#include "v1_0/infc_interface.h"
#include "v1_0/nfc_types.h"
#include "nfc_chip_type_parser.h"

typedef uint8_t tHAL_NFC_STATUS;
typedef void(tHAL_NFC_STATUS_CBACK)(tHAL_NFC_STATUS status);
typedef void(tHAL_NFC_CBACK)(uint8_t event, tHAL_NFC_STATUS status);
typedef void(tHAL_NFC_DATA_CBACK)(uint16_t data_len, uint8_t* p_data);

using namespace OHOS::HDI::Nfc::V1_0;
using namespace testing::ext;
using namespace std;
using namespace OHOS::HDI::Nfc;
using INfcV1_0 = OHOS::HDI::Nfc::V1_0::INfcInterface;
using OHOS::HDI::Nfc::V1_0::NfcStatus;
using OHOS::HDI::Nfc::V1_0::NfcEvent;
using OHOS::HDI::Nfc::V1_0::INfcCallback;
using OHOS::NFC::NfcChipTypeParser;

namespace {
OHOS::sptr<INfcV1_0> mHal = nullptr;
OHOS::sptr<V1_0::INfcCallback> g_callbackV1_0 = nullptr;
OHOS::sptr<INfcCallback> mCallback = nullptr;
} // namespace

class NfcClientCallback : public INfcCallback {
public:
    NfcClientCallback() {}
    virtual ~NfcClientCallback() {}
    NfcClientCallback(tHAL_NFC_CBACK *eventCallback, tHAL_NFC_DATA_CBACK dataCallback)
    {
        mEventCallback = eventCallback;
        mDataCallback = dataCallback;
    };

    int32_t OnData(const std::vector<uint8_t> &data) override
    {
        if (mDataCallback != nullptr && !data.empty()) {
            mDataCallback(data.size(), (uint8_t *)&data[0]);
        }
        return HDF_SUCCESS;
    }

    int32_t OnEvent(NfcEvent event, NfcStatus status) override
    {
        if (mEventCallback != nullptr) {
            mEventCallback((uint8_t)event, (tHAL_NFC_STATUS)status);
        }
        return HDF_SUCCESS;
    }

private:
    tHAL_NFC_CBACK *mEventCallback;
    tHAL_NFC_DATA_CBACK *mDataCallback;
};

class HdfNfcHdiTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void HdfNfcHdiTestAdditional::SetUpTestCase() { mHal = INfcV1_0::Get(); }
void HdfNfcHdiTestAdditional::TearDownTestCase() {}
void HdfNfcHdiTestAdditional::SetUp() {}
void HdfNfcHdiTestAdditional::TearDown() {}

static void EventCallback(uint8_t event, uint8_t status)
{
    if (g_callbackV1_0 != nullptr) {
        printf("EventCallback:%d,%d", event, status);
    }
}

static void DataCallback(uint16_t len, uint8_t *data)
{
    if (g_callbackV1_0 != nullptr) {
        printf("DataCallback:%d,%d", len, data[0]);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_Open_0200
 * @tc.name   : testOpen001
 * @tc.desc   : The first parameter is nullptr
 */
HWTEST_F(HdfNfcHdiTestAdditional, testOpen001, Function | MediumTest | Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(nullptr, nfcbtType);
        EXPECT_NE(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_Close_0200
 * @tc.name   : testClose001
 * @tc.desc   : stability test
 */
HWTEST_F(HdfNfcHdiTestAdditional, testClose001, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        for (int i = 0; i < 1000; i++) {
            ret = mHal->Open(mCallback, nfcbtType);
            EXPECT_EQ(HDF_SUCCESS, ret);
            ret = mHal->Close(nfcbtType);
            EXPECT_EQ(HDF_SUCCESS, ret);
        }
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_CoreInitialized_0200
 * @tc.name   : testCoreInitialized001
 * @tc.desc   : The first parameter is an array (1,1)
 */
HWTEST_F(HdfNfcHdiTestAdditional, testCoreInitialized001, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        std::vector<uint8_t> data(1, 1);
        ret = mHal->CoreInitialized(data, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_CoreInitialized_0300
 * @tc.name   : testCoreInitialized002
 * @tc.desc   : The first parameter is an array (255,1)
 */
HWTEST_F(HdfNfcHdiTestAdditional, testCoreInitialized002, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        std::vector<uint8_t> data(255, 1);
        ret = mHal->CoreInitialized(data, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_CoreInitialized_0400
 * @tc.name   : testCoreInitialized003
 * @tc.desc   : stability test
 */
HWTEST_F(HdfNfcHdiTestAdditional, testCoreInitialized003, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        for (int i = 0; i < 1000; i++) {
            std::vector<uint8_t> data(1, 1);
            ret = mHal->CoreInitialized(data, nfcbtType);
            EXPECT_EQ(HDF_SUCCESS, ret);
        }
        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_Prediscover_0200
 * @tc.name   : testPrediscover001
 * @tc.desc   : stability test
 */
HWTEST_F(HdfNfcHdiTestAdditional, testPrediscover001, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        for (int i = 0; i < 1000; i++) {
            ret = mHal->Prediscover(nfcbtType);
            EXPECT_EQ(HDF_SUCCESS, ret);
        }
        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_Write_0200
 * @tc.name   : testWrite001
 * @tc.desc   : The first parameter is an array (1,1)
 */
HWTEST_F(HdfNfcHdiTestAdditional, testWrite001, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        std::vector<uint8_t> data(1, 1);
        ret = mHal->Write(data, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_Write_0300
 * @tc.name   : testWrite002
 * @tc.desc   : The first parameter is an array (255,1)
 */
HWTEST_F(HdfNfcHdiTestAdditional, testWrite002, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        std::vector<uint8_t> data(255, 1);
        ret = mHal->Write(data, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_Write_0400
 * @tc.name   : testWrite003
 * @tc.desc   : stability test
 */
HWTEST_F(HdfNfcHdiTestAdditional, testWrite003, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        for (int i = 0; i < 1000; i++) {
            std::vector<uint8_t> data(1, 1);
            ret = mHal->Write(data, nfcbtType);
            EXPECT_EQ(HDF_SUCCESS, ret);
        }
        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_ControlGranted_0200
 * @tc.name   : testControlGranted001
 * @tc.desc   : stability test
 */
HWTEST_F(HdfNfcHdiTestAdditional, testControlGranted001, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        for (int i = 0; i < 1000; i++) {
            ret = mHal->ControlGranted(nfcbtType);
            EXPECT_EQ(HDF_SUCCESS, ret);
        }
        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_PowerCycle_0200
 * @tc.name   : testPowerCycle001
 * @tc.desc   : stability test
 */
HWTEST_F(HdfNfcHdiTestAdditional, testPowerCycle001, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        for (int i = 0; i < 1000; i++) {
            ret = mHal->PowerCycle(nfcbtType);
            EXPECT_EQ(HDF_SUCCESS, ret);
        }
        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_Ioctl_0200
 * @tc.name   : testIoctl001
 * @tc.desc   : The first parameter is an array (1,1)
 */
HWTEST_F(HdfNfcHdiTestAdditional, testIoctl001, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        std::vector<uint8_t> v_data(1, 1);
        NfcCommand nfcCommand = NfcCommand::CMD_INVALID;
        ret = mHal->Ioctl(nfcCommand, v_data, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_Ioctl_0300
 * @tc.name   : testIoctl002
 * @tc.desc   : The first parameter is an array (255,1)
 */
HWTEST_F(HdfNfcHdiTestAdditional, testIoctl002, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        std::vector<uint8_t> v_data(255, 1);
        NfcCommand nfcCommand = NfcCommand::CMD_INVALID;
        ret = mHal->Ioctl(nfcCommand, v_data, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Communication_Nfc_Ioctl_0400
 * @tc.name   : testIoctl003
 * @tc.desc   : stability test
 */
HWTEST_F(HdfNfcHdiTestAdditional, testIoctl003, Function | MediumTest | Level1)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    } else {
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        mCallback = new NfcClientCallback(EventCallback, DataCallback);
        if (mCallback == nullptr) {
            ASSERT_NE(nullptr, mCallback);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        NfcCommand nfcCommand = NfcCommand::CMD_INVALID;
        int32_t ret = 0;
        ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);

        for (int i = 0; i < 1000; i++) {
            std::vector<uint8_t> v_data(1, 1);
            ret = mHal->Ioctl(nfcCommand, v_data, nfcbtType);
            EXPECT_EQ(HDF_SUCCESS, ret);
        }
        ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}