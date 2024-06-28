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
}

class NfcClientCallback : public INfcCallback {
    public:
        NfcClientCallback()
    {}
    virtual ~NfcClientCallback()
    {}
    NfcClientCallback(tHAL_NFC_CBACK* eventCallback, tHAL_NFC_DATA_CBACK dataCallback)
    {
        mEventCallback = eventCallback;
        mDataCallback = dataCallback;
    };

    int32_t OnData(const std::vector<uint8_t>& data) override
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
    tHAL_NFC_CBACK* mEventCallback;
    tHAL_NFC_DATA_CBACK* mDataCallback;
};

class HdfNfcHdiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void HdfNfcHdiTest::SetUpTestCase()
{
    mHal = INfcV1_0::Get();
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
* @tc.name: SUB_DriverSystem_Hdinfcopen_0100
* @tc.desc: Enables the nfc controller and initialize the nfc core.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_Hdinfcopen_0100, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
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
        int32_t ret = mHal->Open(mCallback, nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcCoreInitialized_0200
* @tc.desc: Configures the nfc chip after initializing the nfc core.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_HdinfcCoreInitialized_0200, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        std::vector<uint8_t> data(0);
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = mHal->CoreInitialized(data, nfcbtType);
        EXPECT_EQ(HDF_ERR_INVALID_PARAM, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcPrediscover_0300
* @tc.desc: Specifically configures the nfc chip before starting RF discovering.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_HdinfcPrediscover_0300, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = mHal->Prediscover(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcWrite_0400
* @tc.desc: Writes NCI data to the nfc core.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_HdinfcWrite_0400, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        std::vector<uint8_t> data;
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = mHal->Write(data, nfcbtType);
        EXPECT_EQ(HDF_ERR_INVALID_PARAM, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcControlGranted_0500
* @tc.desc: Sets the HDF to allow to send NCI data.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_HdinfcControlGranted_0500, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = mHal->ControlGranted(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcPowerCycle_0600
* @tc.desc:  Restarts the nfc controller according to each power cycle.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_HdinfcPowerCycle_0600, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = mHal->PowerCycle(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcIoctl_0700
* @tc.desc: Sends I/O control commands and data from the nfc stack to HDI.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_HdinfcIoctl_0700, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        uint8_t p_core_init_rsp_params = 0;
        uint16_t data_len = sizeof(uint8_t);
        std::vector<uint8_t> v_data(p_core_init_rsp_params,
                                    p_core_init_rsp_params + data_len / sizeof(uint8_t));
        NfcStatus nfcbtType = NfcStatus::OK;
        NfcCommand nfcCommand = NfcCommand::CMD_INVALID ;
        int32_t ret = mHal->Ioctl(nfcCommand, v_data, nfcbtType);
        EXPECT_EQ(HDF_ERR_INVALID_PARAM, ret);
    }
}

/**
* @tc.name: SUB_DriverSystem_HdinfcClose_0800
* @tc.desc: Disables the nfc controller and releases the resource.
* @tc.type: FUNC
*/
HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_HdinfcClose_0800, TestSize.Level2)
{
    if (!NfcChipTypeParser::IsSupportedChipType()) {
        EXPECT_EQ(HDF_SUCCESS, 0);
    }
    else{
        if (mHal == nullptr) {
            ASSERT_NE(nullptr, mHal);
            return;
        }
        NfcStatus nfcbtType = NfcStatus::OK;
        int32_t ret = mHal->Close(nfcbtType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
  * @tc.name: SUB_DriverSystem_HdinfcClose_0900
  * @tc.desc: the HdiNfc enum types test
  * @tc.type: FUNC
  */

HWTEST_F(HdfNfcHdiTest, SUB_DriverSystem_HdinfcClose_0900, TestSize.Level2)
{
    NfcEvent open_cplt = NfcEvent::OPEN_CPLT;
    std::cout << "NfcEvent OPEN_CPLT = " << open_cplt << std::endl;
    EXPECT_EQ(open_cplt, 0);

    NfcEvent close_cplt = NfcEvent::CLOSE_CPLT;
    std::cout << "NfcEvent CLOSE_CPLT = " << close_cplt << std::endl;
    EXPECT_EQ(close_cplt, 1);

    NfcEvent post_init_cplt = NfcEvent::POST_INIT_CPLT;
    std::cout << "NfcEvent POST_INIT_CPLT = " << post_init_cplt << std::endl;
    EXPECT_EQ(post_init_cplt, 2);

    NfcEvent per_discover_cplt = NfcEvent::PRE_DISCOVER_CPLT;
    std::cout << "NfcEvent PRE_DISCOVER_CPLT = " << per_discover_cplt << std::endl;
    EXPECT_EQ(per_discover_cplt, 3);

    NfcEvent request_control = NfcEvent::REQUEST_CONTROL;
    std::cout << "NfcEvent REQUEST_CONTROL = " << request_control << std::endl;
    EXPECT_EQ(request_control, 4);

    NfcEvent release_control = NfcEvent::RELEASE_CONTROL;
    std::cout << "NfcEvent RELEASE_CONTROL = " << release_control << std::endl;
    EXPECT_EQ(release_control, 5);

    NfcEvent error = NfcEvent::ERROR;
    std::cout << "NfcEvent ERROR = " << error << std::endl;
    EXPECT_EQ(error, 6);

    NfcEvent hci_network_reset = NfcEvent::HCI_NETWORK_RESET;
    std::cout << "NfcEvent HCI_NETWORK_RESET = " << hci_network_reset << std::endl;
    EXPECT_EQ(hci_network_reset, 7);

    NfcStatus ok = NfcStatus::OK;
    std::cout << "NfcStatus OK = " << ok << std::endl;
    EXPECT_EQ(ok, 0);

    NfcStatus failed = NfcStatus::FAILED;
    std::cout << "NfcStatus FAILED = " << failed << std::endl;
    EXPECT_EQ(failed, 1);

    NfcStatus err_transport = NfcStatus::ERR_TRANSPORT;
    std::cout << "NfcStatus ERR_TRANSPORT = " << err_transport << std::endl;
    EXPECT_EQ(err_transport, 2);

    NfcStatus err_cmd_timeout = NfcStatus::ERR_CMD_TIMEOUT;
    std::cout << "NfcStatus ERR_CMD_TIMEOUT = " << err_cmd_timeout << std::endl;
    EXPECT_EQ(err_cmd_timeout, 3);

    NfcStatus refused = NfcStatus::REFUSED;
    std::cout << "NfcStatus REFUSED = " << refused << std::endl;
    EXPECT_EQ(refused, 4);

    NfcCommand cmd_invalid = NfcCommand::CMD_INVALID;
    std::cout << "NfcCommand CMD_INVALID = " << cmd_invalid << std::endl;
    EXPECT_EQ(cmd_invalid, 0);
}