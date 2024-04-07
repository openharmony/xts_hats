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

#include "cmath"
#include "condition_variable"
#include "cstdio"
#include "hdf_base.h"
#include "hdf_log.h"
#include "map"
#include "mutex"
#include "securec.h"
#include "cstdlib"
#include "unistd.h"
#include "v1_3/iril.h"
#include "gtest/gtest.h"
#include "hdf_ril_callback_common.h"

using namespace OHOS::HDI::Ril::V1_3;
using namespace testing::ext;

class HdfRilHdiTestAdditional5th : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
/**
** HdfRilHdiTestAdditional5th implement
**/
void HdfRilHdiTestAdditional5th::SetUpTestCase()
{
    g_rilInterface = OHOS::HDI::Ril::V1_3::IRil::Get();
    if (g_rilInterface != nullptr) {
        g_rilInterface->SetCallback1_3(&g_callback);
        g_rilInterface->SendRilAck();
        g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
        g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    }
}

void HdfRilHdiTestAdditional5th::TearDownTestCase() {}
void HdfRilHdiTestAdditional5th::SetUp() {}
void HdfRilHdiTestAdditional5th::TearDown() {}

/**
**  Xts Test
**/
/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessage_0500
 * @tc.name: testV1DelCdmaSimMessage003
 * @tc.desc: DelCdmaSimMessage, index is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelCdmaSimMessage003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t index = -1;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_1, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessage_0600
 * @tc.name: testV1DelCdmaSimMessage004
 * @tc.desc: DelCdmaSimMessage, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelCdmaSimMessage004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_2, -1, index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessage_0700
 * @tc.name: testV1DelCdmaSimMessage005
 * @tc.desc: DelCdmaSimMessage, index is 255
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelCdmaSimMessage005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t index = 255;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_2, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessage_0800
 * @tc.name: testV1DelCdmaSimMessage006
 * @tc.desc: DelCdmaSimMessage, index is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelCdmaSimMessage006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t index = -1;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_2, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessage_0900
 * @tc.name: testV1DelCdmaSimMessage007
 * @tc.desc: DelCdmaSimMessage, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelCdmaSimMessage007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->DelCdmaSimMessage(SLOTID_1, GetSerialId(), index);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_0300
 * @tc.name: testV1UpdateCdmaSimMessage001
 * @tc.desc: UpdateCdmaSimMessage, smscPdu is TEST_CDMA_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_0400
 * @tc.name: testV1UpdateCdmaSimMessage002
 * @tc.desc: UpdateCdmaSimMessage, smscPdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_0500
 * @tc.name: testV1UpdateCdmaSimMessage003
 * @tc.desc: UpdateCdmaSimMessage, smscPdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "@";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_0600
 * @tc.name: testV1UpdateCdmaSimMessage004
 * @tc.desc: UpdateCdmaSimMessage, pdu is TEST_SEND_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_SEND_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_0700
 * @tc.name: testV1UpdateCdmaSimMessage005
 * @tc.desc: UpdateCdmaSimMessage, pdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage005, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_0800
 * @tc.name: testV1UpdateCdmaSimMessage006
 * @tc.desc: UpdateCdmaSimMessage, pdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "@";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_0900
 * @tc.name: testV1UpdateCdmaSimMessage007
 * @tc.desc: UpdateCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNREAD
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNREAD);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1000
 * @tc.name: testV1UpdateCdmaSimMessage008
 * @tc.desc: UpdateCdmaSimMessage, state is SIM_MESSAGE_STATUS_READ
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_READ);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1100
 * @tc.name: testV1UpdateCdmaSimMessage009
 * @tc.desc: UpdateCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1200
 * @tc.name: testV1UpdateCdmaSimMessage010
 * @tc.desc: UpdateCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 255;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1300
 * @tc.name: testV1UpdateCdmaSimMessage011
 * @tc.desc: UpdateCdmaSimMessage, smscPdu is TEST_CDMA_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1400
 * @tc.name: testV1UpdateCdmaSimMessage012
 * @tc.desc: UpdateCdmaSimMessage, smscPdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage012, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1500
 * @tc.name: testV1UpdateCdmaSimMessage013
 * @tc.desc: UpdateCdmaSimMessage, smscPdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "@";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1600
 * @tc.name: testV1UpdateCdmaSimMessage014
 * @tc.desc: UpdateCdmaSimMessage, pdu is TEST_SEND_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_SEND_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1700
 * @tc.name: testV1UpdateCdmaSimMessage015
 * @tc.desc: UpdateCdmaSimMessage, pdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage015, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1800
 * @tc.name: testV1UpdateCdmaSimMessage016
 * @tc.desc: UpdateCdmaSimMessage, pdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "@";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_1900
 * @tc.name: testV1UpdateCdmaSimMessage017
 * @tc.desc: UpdateCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNREAD
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNREAD);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_2000
 * @tc.name: testV1UpdateCdmaSimMessage018
 * @tc.desc: UpdateCdmaSimMessage, state is SIM_MESSAGE_STATUS_READ
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_READ);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_2100
 * @tc.name: testV1UpdateCdmaSimMessage019
 * @tc.desc: UpdateCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_2200
 * @tc.name: testV1UpdateCdmaSimMessage020
 * @tc.desc: UpdateCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 255;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_2300
 * @tc.name: testV1UpdateCdmaSimMessage021
 * @tc.desc: UpdateCdmaSimMessage, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_2400
 * @tc.name: testV1UpdateCdmaSimMessage022
 * @tc.desc: UpdateCdmaSimMessage, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage022, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessage_2500
 * @tc.name: testV1UpdateCdmaSimMessage023
 * @tc.desc: UpdateCdmaSimMessage, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateCdmaSimMessage023, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSearchInformation_0300
 * @tc.name: testV1GetNetworkSearchInformation001
 * @tc.desc: GetNetworkSearchInformation, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetNetworkSearchInformation001, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSearchInformation_0400
 * @tc.name: testV1GetNetworkSearchInformation002
 * @tc.desc: GetNetworkSearchInformation, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetNetworkSearchInformation002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSearchInformation_0500
 * @tc.name: testV1GetNetworkSearchInformation003
 * @tc.desc: GetNetworkSearchInformation, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetNetworkSearchInformation003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSelectionMode_0300
 * @tc.name: testV1GetNetworkSelectionMode001
 * @tc.desc: GetNetworkSelectionMode, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetNetworkSelectionMode001, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSelectionMode_0400
 * @tc.name: testV1GetNetworkSelectionMode002
 * @tc.desc: GetNetworkSelectionMode, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetNetworkSelectionMode002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSelectionMode_0500
 * @tc.name: testV1GetNetworkSelectionMode003
 * @tc.desc: GetNetworkSelectionMode, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetNetworkSelectionMode003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_0300
 * @tc.name: testV1SetNetworkSelectionMode001
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_AUTO
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_0400
 * @tc.name: testV1SetNetworkSelectionMode002
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_0500
 * @tc.name: testV1SetNetworkSelectionMode003
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_0600
 * @tc.name: testV1SetNetworkSelectionMode004
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_0700
 * @tc.name: testV1SetNetworkSelectionMode005
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_0800
 * @tc.name: testV1SetNetworkSelectionMode006
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_0900
 * @tc.name: testV1SetNetworkSelectionMode007
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1000
 * @tc.name: testV1SetNetworkSelectionMode008
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1100
 * @tc.name: testV1SetNetworkSelectionMode009
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_EVDO
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_EVDO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1200
 * @tc.name: testV1SetNetworkSelectionMode010
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1300
 * @tc.name: testV1SetNetworkSelectionMode011
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1400
 * @tc.name: testV1SetNetworkSelectionMode012
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1500
 * @tc.name: testV1SetNetworkSelectionMode013
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1600
 * @tc.name: testV1SetNetworkSelectionMode014
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1700
 * @tc.name: testV1SetNetworkSelectionMode015
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1800
 * @tc.name: testV1SetNetworkSelectionMode016
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_1900
 * @tc.name: testV1SetNetworkSelectionMode017
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2000
 * @tc.name: testV1SetNetworkSelectionMode018
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2100
 * @tc.name: testV1SetNetworkSelectionMode019
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2200
 * @tc.name: testV1SetNetworkSelectionMode020
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2300
 * @tc.name: testV1SetNetworkSelectionMode021
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2400
 * @tc.name: testV1SetNetworkSelectionMode022
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2500
 * @tc.name: testV1SetNetworkSelectionMode023
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2600
 * @tc.name: testV1SetNetworkSelectionMode024
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2700
 * @tc.name: testV1SetNetworkSelectionMode025
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode025, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2800
 * @tc.name: testV1SetNetworkSelectionMode026
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode026, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_2900
 * @tc.name: testV1SetNetworkSelectionMode027
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode027, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3000
 * @tc.name: testV1SetNetworkSelectionMode028
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode028, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3100
 * @tc.name: testV1SetNetworkSelectionMode029
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode029, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3200
 * @tc.name: testV1SetNetworkSelectionMode030
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode030, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3300
 * @tc.name: testV1SetNetworkSelectionMode031
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode031, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3400
 * @tc.name: testV1SetNetworkSelectionMode032
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode032, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3500
 * @tc.name: testV1SetNetworkSelectionMode033
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode033, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3600
 * @tc.name: testV1SetNetworkSelectionMode034
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode034, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3700
 * @tc.name: testV1SetNetworkSelectionMode035
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_MAX_VALUE
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode035, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_MAX_VALUE);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3800
 * @tc.name: testV1SetNetworkSelectionMode036
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_AUTO
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode036, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_3900
 * @tc.name: testV1SetNetworkSelectionMode037
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode037, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4000
 * @tc.name: testV1SetNetworkSelectionMode038
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode038, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4100
 * @tc.name: testV1SetNetworkSelectionMode039
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode039, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4200
 * @tc.name: testV1SetNetworkSelectionMode040
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode040, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4300
 * @tc.name: testV1SetNetworkSelectionMode041
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode041, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4400
 * @tc.name: testV1SetNetworkSelectionMode042
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode042, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4500
 * @tc.name: testV1SetNetworkSelectionMode043
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode043, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4600
 * @tc.name: testV1SetNetworkSelectionMode044
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_EVDO
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode044, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_EVDO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4700
 * @tc.name: testV1SetNetworkSelectionMode045
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode045, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4800
 * @tc.name: testV1SetNetworkSelectionMode046
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode046, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_4900
 * @tc.name: testV1SetNetworkSelectionMode047
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode047, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5000
 * @tc.name: testV1SetNetworkSelectionMode048
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode048, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5100
 * @tc.name: testV1SetNetworkSelectionMode049
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode049, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5200
 * @tc.name: testV1SetNetworkSelectionMode050
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode050, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5300
 * @tc.name: testV1SetNetworkSelectionMode051
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode051, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5400
 * @tc.name: testV1SetNetworkSelectionMode052
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode052, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5500
 * @tc.name: testV1SetNetworkSelectionMode053
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode053, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5600
 * @tc.name: testV1SetNetworkSelectionMode054
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode054, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5700
 * @tc.name: testV1SetNetworkSelectionMode055
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode055, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5800
 * @tc.name: testV1SetNetworkSelectionMode056
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode056, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_5900
 * @tc.name: testV1SetNetworkSelectionMode057
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode057, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6000
 * @tc.name: testV1SetNetworkSelectionMode058
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode058, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6100
 * @tc.name: testV1SetNetworkSelectionMode059
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode059, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6200
 * @tc.name: testV1SetNetworkSelectionMode060
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode060, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6300
 * @tc.name: testV1SetNetworkSelectionMode061
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode061, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6400
 * @tc.name: testV1SetNetworkSelectionMode062
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode062, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6500
 * @tc.name: testV1SetNetworkSelectionMode063
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode063, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6600
 * @tc.name: testV1SetNetworkSelectionMode064
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode064, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6700
 * @tc.name: testV1SetNetworkSelectionMode065
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode065, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6800
 * @tc.name: testV1SetNetworkSelectionMode066
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode066, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_6900
 * @tc.name: testV1SetNetworkSelectionMode067
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode067, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_7000
 * @tc.name: testV1SetNetworkSelectionMode068
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode068, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_7100
 * @tc.name: testV1SetNetworkSelectionMode069
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode069, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode =
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_7200
 * @tc.name: testV1SetNetworkSelectionMode070
 * @tc.desc: SetNetworkSelectionMode, selectMode is CORE_NETWORK_MODE_MAX_VALUE
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode070, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_MAX_VALUE);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_7300
 * @tc.name: testV1SetNetworkSelectionMode071
 * @tc.desc: SetNetworkSelectionMode, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode071, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, -1, setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_7400
 * @tc.name: testV1SetNetworkSelectionMode072
 * @tc.desc: SetNetworkSelectionMode, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode072, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, -1, setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionMode_7500
 * @tc.name: testV1SetNetworkSelectionMode073
 * @tc.desc: SetNetworkSelectionMode, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNetworkSelectionMode073, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNeighboringCellInfoList_0300
 * @tc.name: testV1GetNeighboringCellInfoList001
 * @tc.desc: GetNeighboringCellInfoList, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetNeighboringCellInfoList001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNeighboringCellInfoList_0400
 * @tc.name: testV1GetNeighboringCellInfoList002
 * @tc.desc: GetNeighboringCellInfoList, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetNeighboringCellInfoList002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNeighboringCellInfoList_0500
 * @tc.name: testV1GetNeighboringCellInfoList003
 * @tc.desc: GetNeighboringCellInfoList, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetNeighboringCellInfoList003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfo_0300
 * @tc.name: testV1GetCurrentCellInfo001
 * @tc.desc: GetCurrentCellInfo, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetCurrentCellInfo001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfo_0400
 * @tc.name: testV1GetCurrentCellInfo002
 * @tc.desc: GetCurrentCellInfo, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetCurrentCellInfo002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfo_0500
 * @tc.name: testV1GetCurrentCellInfo003
 * @tc.desc: GetCurrentCellInfo, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetCurrentCellInfo003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_0300
 * @tc.name: testV1SetDataPermitted001
 * @tc.desc: SetDataPermitted, dataPermitted is 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_0400
 * @tc.name: testV1SetDataPermitted002
 * @tc.desc: SetDataPermitted, dataPermitted is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_0500
 * @tc.name: testV1SetDataPermitted003
 * @tc.desc: SetDataPermitted, dataPermitted is 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, GetSerialId(), 2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_0600
 * @tc.name: testV1SetDataPermitted004
 * @tc.desc: SetDataPermitted, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, -1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_0700
 * @tc.name: testV1SetDataPermitted005
 * @tc.desc: SetDataPermitted, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_0800
 * @tc.name: testV1SetDataPermitted006
 * @tc.desc: SetDataPermitted, dataPermitted is 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_0900
 * @tc.name: testV1SetDataPermitted007
 * @tc.desc: SetDataPermitted, dataPermitted is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_1000
 * @tc.name: testV1SetDataPermitted008
 * @tc.desc: SetDataPermitted, dataPermitted is 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, GetSerialId(), 2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_1100
 * @tc.name: testV1SetDataPermitted009
 * @tc.desc: SetDataPermitted, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, -1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_1200
 * @tc.name: testV1SetDataPermitted010
 * @tc.desc: SetDataPermitted, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermitted_1300
 * @tc.name: testV1SetDataPermitted011
 * @tc.desc: SetDataPermitted, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataPermitted011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetDataPermitted(SLOTID_1, GetSerialId(), 1);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_0300
 * @tc.name: testV1SetDataProfileInfo001
 * @tc.desc: SetDataProfileInfo, isRoaming is 0, profileId is 0, authenticationType is 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 0;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_1, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_0400
 * @tc.name: testV1SetDataProfileInfo002
 * @tc.desc: SetDataProfileInfo, isRoaming is 1, profileId is 1, authenticationType is 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 1;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 1;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_1, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_0500
 * @tc.name: testV1SetDataProfileInfo003
 * @tc.desc: SetDataProfileInfo, isRoaming is 1, profileId is 1, authenticationType is 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 1;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 2;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_1, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_0600
 * @tc.name: testV1SetDataProfileInfo004
 * @tc.desc: SetDataProfileInfo, userName is "@", password is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 1;
    dataProfileInfo.password = "@";
    dataProfileInfo.authenticationType = 2;
    dataProfileInfo.userName = "@";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_1, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_0700
 * @tc.name: testV1SetDataProfileInfo005
 * @tc.desc: SetDataProfileInfo, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 1;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 2;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_1, -1, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_0800
 * @tc.name: testV1SetDataProfileInfo006
 * @tc.desc: SetDataProfileInfo, isRoaming is 0, profileId is 0, authenticationType is 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 0;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_2, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_0900
 * @tc.name: testV1SetDataProfileInfo007
 * @tc.desc: SetDataProfileInfo, isRoaming is 1, profileId is 1, authenticationType is 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 1;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 1;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_2, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_1000
 * @tc.name: testV1SetDataProfileInfo008
 * @tc.desc: SetDataProfileInfo, isRoaming is 1, profileId is 1, authenticationType is 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 1;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 2;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_2, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_1100
 * @tc.name: testV1SetDataProfileInfo009
 * @tc.desc: SetDataProfileInfo, userName is "@", password is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 1;
    dataProfileInfo.password = "@";
    dataProfileInfo.authenticationType = 2;
    dataProfileInfo.userName = "@";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_2, serialId, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_1200
 * @tc.name: testV1SetDataProfileInfo010
 * @tc.desc: SetDataProfileInfo, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 1;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 1;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 2;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_2, -1, dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataProfileInfo_1300
 * @tc.name: testV1SetDataProfileInfo011
 * @tc.desc: SetDataProfileInfo, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDataProfileInfo011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    dataProfilesInfo.isRoaming = 0;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetDataProfileInfo(SLOTID_1, serialId, dataProfilesInfo);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_0300
 * @tc.name  : testV1GetSimLockStatus001
 * @tc.desc  : GetSimLockStatus, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus001, Function | MediumTest | Level2)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC";
    simLockInfo.mode = 2;
    simLockInfo.classx = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, -1, simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_0400
 * @tc.name  : testV1GetSimLockStatus002
 * @tc.desc  : GetSimLockStatus, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus002, Function | MediumTest | Level2)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC";
    simLockInfo.mode = 2;
    simLockInfo.classx = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, -1, simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_0500
 * @tc.name  : testV1GetSimLockStatus003
 * @tc.desc  : GetSimLockStatus, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus003, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC";
    simLockInfo.mode = 2;
    simLockInfo.classx = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, 2147483647, simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_0600
 * @tc.name  : testV1GetSimLockStatus004
 * @tc.desc  : GetSimLockStatus, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus004, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC";
    simLockInfo.mode = 2;
    simLockInfo.classx = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, 2147483647, simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_0700
 * @tc.name  : testV1GetSimLockStatus005
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AO, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus005, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_0800
 * @tc.name  : testV1GetSimLockStatus006
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= OI, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus006, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_0900
 * @tc.name  : testV1GetSimLockStatus007
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= OX, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus007, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OX";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1000
 * @tc.name  : testV1GetSimLockStatus008
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AI, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus008, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AI";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1100
 * @tc.name  : testV1GetSimLockStatus009
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= IR, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus009, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1200
 * @tc.name  : testV1GetSimLockStatus010
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AB, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus010, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AB";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1300
 * @tc.name  : testV1GetSimLockStatus011
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AG, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus011, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AG";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1400
 * @tc.name  : testV1GetSimLockStatus012
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AC, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus012, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AC";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1500
 * @tc.name  : testV1GetSimLockStatus013
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= FD, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus013, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "FD";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1600
 * @tc.name  : testV1GetSimLockStatus014
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= PN, mode = 0, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus014, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PN";
    simLockInfo.mode = 0;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1700
 * @tc.name  : testV1GetSimLockStatus015
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= PU, mode = 0, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus015, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PU";
    simLockInfo.mode = 0;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1800
 * @tc.name  : testV1GetSimLockStatus016
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= PP, mode = 0, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus016, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PP";
    simLockInfo.mode = 0;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_1900
 * @tc.name  : testV1GetSimLockStatus017
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AO, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus017, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2000
 * @tc.name  : testV1GetSimLockStatus018
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= OI, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus018, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2100
 * @tc.name  : testV1GetSimLockStatus019
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= OX, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus019, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OX";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2200
 * @tc.name  : testV1GetSimLockStatus020
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AI, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus020, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AI";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2300
 * @tc.name  : testV1GetSimLockStatus021
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= IR, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus021, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2400
 * @tc.name  : testV1GetSimLockStatus022
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AB, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus022, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AB";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2500
 * @tc.name  : testV1GetSimLockStatus023
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AG, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus023, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AG";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2600
 * @tc.name  : testV1GetSimLockStatus024
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AC, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus024, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AC";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2700
 * @tc.name  : testV1GetSimLockStatus025
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= FD, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus025, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "FD";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2800
 * @tc.name  : testV1GetSimLockStatus026
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= PN, mode = 1, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus026, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PN";
    simLockInfo.mode = 1;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_2900
 * @tc.name  : testV1GetSimLockStatus027
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= PU, mode = 1, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus027, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PU";
    simLockInfo.mode = 1;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3000
 * @tc.name  : testV1GetSimLockStatus028
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= PP, mode = 1, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus028, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PP";
    simLockInfo.mode = 1;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3100
 * @tc.name  : testV1GetSimLockStatus029
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AO, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus029, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3200
 * @tc.name  : testV1GetSimLockStatus030
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= OI, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus030, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3300
 * @tc.name  : testV1GetSimLockStatus031
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= OX, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus031, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OX";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3400
 * @tc.name  : testV1GetSimLockStatus032
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AI, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus032, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AI";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3500
 * @tc.name  : testV1GetSimLockStatus033
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= IR, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus033, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3600
 * @tc.name  : testV1GetSimLockStatus034
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AB, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus034, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AB";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3700
 * @tc.name  : testV1GetSimLockStatus035
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AG, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus035, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AG";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3800
 * @tc.name  : testV1GetSimLockStatus036
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= AC, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus036, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AC";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_3900
 * @tc.name  : testV1GetSimLockStatus037
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= FD, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus037, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "FD";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4000
 * @tc.name  : testV1GetSimLockStatus038
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= PN, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus038, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PN";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4100
 * @tc.name  : testV1GetSimLockStatus039
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= PU, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus039, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PU";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4200
 * @tc.name  : testV1GetSimLockStatus040
 * @tc.desc  : GetSimLockStatus, slotId is 0, classx = 255, fac= PP, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus040, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PP";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4300
 * @tc.name  : testV1GetSimLockStatus041
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AO, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus041, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4400
 * @tc.name  : testV1GetSimLockStatus042
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= OI, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus042, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4500
 * @tc.name  : testV1GetSimLockStatus043
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= OX, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus043, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OX";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4600
 * @tc.name  : testV1GetSimLockStatus044
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AI, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus044, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AI";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4700
 * @tc.name  : testV1GetSimLockStatus045
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= IR, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus045, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4800
 * @tc.name  : testV1GetSimLockStatus046
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AB, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus046, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AB";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_4900
 * @tc.name  : testV1GetSimLockStatus047
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AG, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus047, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AG";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5000
 * @tc.name  : testV1GetSimLockStatus048
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AC, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus048, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AC";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5100
 * @tc.name  : testV1GetSimLockStatus049
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= FD, mode = 0
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus049, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "FD";
    simLockInfo.mode = 0;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5200
 * @tc.name  : testV1GetSimLockStatus050
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= PN, mode = 0, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus050, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PN";
    simLockInfo.mode = 0;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5300
 * @tc.name  : testV1GetSimLockStatus051
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= PU, mode = 0, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus051, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PU";
    simLockInfo.mode = 0;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5400
 * @tc.name  : testV1GetSimLockStatus052
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= PP, mode = 0, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus052, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PP";
    simLockInfo.mode = 0;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5500
 * @tc.name  : testV1GetSimLockStatus053
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AO, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus053, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5600
 * @tc.name  : testV1GetSimLockStatus054
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= OI, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus054, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5700
 * @tc.name  : testV1GetSimLockStatus055
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= OX, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus055, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OX";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5800
 * @tc.name  : testV1GetSimLockStatus056
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AI, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus056, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AI";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_5900
 * @tc.name  : testV1GetSimLockStatus057
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= IR, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus057, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6000
 * @tc.name  : testV1GetSimLockStatus058
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AB, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus058, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AB";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6100
 * @tc.name  : testV1GetSimLockStatus059
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AG, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus059, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AG";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6200
 * @tc.name  : testV1GetSimLockStatus060
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AC, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus060, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AC";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6300
 * @tc.name  : testV1GetSimLockStatus061
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= FD, mode = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus061, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "FD";
    simLockInfo.mode = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6400
 * @tc.name  : testV1GetSimLockStatus062
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= PN, mode = 1, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus062, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PN";
    simLockInfo.mode = 1;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6500
 * @tc.name  : testV1GetSimLockStatus063
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= PU, mode = 1, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus063, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PU";
    simLockInfo.mode = 1;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6600
 * @tc.name  : testV1GetSimLockStatus064
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= PP, mode = 1, status = 1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus064, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PP";
    simLockInfo.mode = 1;
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6700
 * @tc.name  : testV1GetSimLockStatus065
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AO, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus065, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6800
 * @tc.name  : testV1GetSimLockStatus066
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= OI, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus066, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_6900
 * @tc.name  : testV1GetSimLockStatus067
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= OX, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus067, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OX";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_7000
 * @tc.name  : testV1GetSimLockStatus068
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AI, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus068, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AI";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_7100
 * @tc.name  : testV1GetSimLockStatus069
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= IR, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus069, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_7200
 * @tc.name  : testV1GetSimLockStatus070
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AB, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus070, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AB";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_7300
 * @tc.name  : testV1GetSimLockStatus071
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AG, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus071, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AG";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_7400
 * @tc.name  : testV1GetSimLockStatus072
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= AC, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus072, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AC";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_7500
 * @tc.name  : testV1GetSimLockStatus073
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= FD, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus073, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "FD";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_7600
 * @tc.name  : testV1GetSimLockStatus074
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= PN, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus074, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PN";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_7700
 * @tc.name  : testV1GetSimLockStatus075
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= PU, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus075, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PU";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimLockStatus_7800
 * @tc.name  : testV1GetSimLockStatus076
 * @tc.desc  : GetSimLockStatus, slotId is 1, classx = 255, fac= PP, mode = 2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSimLockStatus076, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "PP";
    simLockInfo.mode = 2;

    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_0300
 * @tc.name  : testV1ChangeSimPassword001
 * @tc.desc  : ChangeSimPassword, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword001, Function | MediumTest | Level2)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, -1, simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_0400
 * @tc.name  : testV1ChangeSimPassword002
 * @tc.desc  : ChangeSimPassword, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword002, Function | MediumTest | Level2)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, -1, simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_0500
 * @tc.name  : testV1ChangeSimPassword003
 * @tc.desc  : ChangeSimPassword, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword003, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, 2147483647, simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_0600
 * @tc.name  : testV1ChangeSimPassword004
 * @tc.desc  : ChangeSimPassword, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword004, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, 2147483647, simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_0700
 * @tc.name  : testV1ChangeSimPassword005
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = AO
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword005, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AO";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_0800
 * @tc.name  : testV1ChangeSimPassword006
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = OI
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword006, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "OI";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_0900
 * @tc.name  : testV1ChangeSimPassword007
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = OX
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword007, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "OX";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1000
 * @tc.name  : testV1ChangeSimPassword008
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = AI
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword008, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AI";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1100
 * @tc.name  : testV1ChangeSimPassword009
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = IR
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword009, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "IR";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1200
 * @tc.name  : testV1ChangeSimPassword010
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = AB
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword010, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AB";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1300
 * @tc.name  : testV1ChangeSimPassword011
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = AG
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword011, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AG";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1400
 * @tc.name  : testV1ChangeSimPassword012
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = AC
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword012, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AC";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1500
 * @tc.name  : testV1ChangeSimPassword013
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = FD
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword013, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "FD";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1600
 * @tc.name  : testV1ChangeSimPassword014
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = PN
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword014, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "PN";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1700
 * @tc.name  : testV1ChangeSimPassword015
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = PU
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword015, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "PU";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1800
 * @tc.name  : testV1ChangeSimPassword016
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = PP
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword016, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "PP";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_1900
 * @tc.name  : testV1ChangeSimPassword017
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = AO,Password testing
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword017, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AO";
    simPassword.oldPassword = "1qaz!QAZ";
    simPassword.newPassword = "1qaz!QAZ";
    simPassword.passwordLength = 9;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2000
 * @tc.name  : testV1ChangeSimPassword018
 * @tc.desc  : ChangeSimPassword, slotId is 0, fac = AO,Password testing
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword018, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AO";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1qaz!QAZ";
    simPassword.passwordLength = 9;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2100
 * @tc.name  : testV1ChangeSimPassword019
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = AO
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword019, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AO";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2200
 * @tc.name  : testV1ChangeSimPassword020
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = OI
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword020, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "OI";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2300
 * @tc.name  : testV1ChangeSimPassword021
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = OX
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword021, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "OX";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2400
 * @tc.name  : testV1ChangeSimPassword022
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = AI
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword022, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AI";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2500
 * @tc.name  : testV1ChangeSimPassword023
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = IR
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword023, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "IR";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2600
 * @tc.name  : testV1ChangeSimPassword024
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = AB
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword024, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AB";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2700
 * @tc.name  : testV1ChangeSimPassword025
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = AG
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword025, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AG";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2800
 * @tc.name  : testV1ChangeSimPassword026
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = AC
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword026, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AC";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_2900
 * @tc.name  : testV1ChangeSimPassword027
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = FD
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword027, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "FD";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_3000
 * @tc.name  : testV1ChangeSimPassword028
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = PN
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword028, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "PN";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_3100
 * @tc.name  : testV1ChangeSimPassword029
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = PU
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword029, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "PU";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_3200
 * @tc.name  : testV1ChangeSimPassword030
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = PP
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword030, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "PP";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_3300
 * @tc.name  : testV1ChangeSimPassword031
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = AO,Password testing
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword031, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AO";
    simPassword.oldPassword = "1qaz!QAZ";
    simPassword.newPassword = "1qaz!QAZ";
    simPassword.passwordLength = 9;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPassword_3400
 * @tc.name  : testV1ChangeSimPassword032
 * @tc.desc  : ChangeSimPassword, slotId is 1, fac = AO,Password testing
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1ChangeSimPassword032, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "AO";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1qaz!QAZ";
    simPassword.passwordLength = 9;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdatesResponse_0100
 * @tc.name  : testV1SetLocateUpdatesResponse001
 * @tc.desc  : Call function SetLocateUpdatesResponse slotId is SLOTID_1 Notification mode is REG_NOT_NOTIFY
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetLocateUpdatesResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setLocateUpdatesResponseFlag = false;
    int32_t ret = 0;
    ret = g_rilInterface->SetLocateUpdates(SLOTID_1, GetSerialId(), RilRegNotifyMode::REG_NOT_NOTIFY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(true, g_setLocateUpdatesResponseFlag);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdatesResponse_0200
 * @tc.name  : testV1SetLocateUpdatesResponse002
 * @tc.desc  : Call function SetLocateUpdatesResponse slotId is SLOTID_2 Notification mode is REG_NOT_NOTIFY
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetLocateUpdatesResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setLocateUpdatesResponseFlag = false;
    int32_t ret = 0;
    ret = g_rilInterface->SetLocateUpdates(SLOTID_2, GetSerialId(), RilRegNotifyMode::REG_NOT_NOTIFY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(true, g_setLocateUpdatesResponseFlag);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdatesResponse_0300
 * @tc.name  : testV1SetLocateUpdatesResponse003
 * @tc.desc  : Call function SetLocateUpdatesResponse slotId is SLOTID_1 Notification mode is REG_NOTIFY_STAT_ONLY
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetLocateUpdatesResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setLocateUpdatesResponseFlag = false;
    int32_t ret = 0;
    ret = g_rilInterface->SetLocateUpdates(SLOTID_1, GetSerialId(), RilRegNotifyMode::REG_NOTIFY_STAT_ONLY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(true, g_setLocateUpdatesResponseFlag);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdatesResponse_0400
 * @tc.name  : testV1SetLocateUpdatesResponse004
 * @tc.desc  : Call function SetLocateUpdatesResponse slotId is SLOTID_2 SerialId is -1 Notification mode is
 * REG_NOT_NOTIFY
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetLocateUpdatesResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setLocateUpdatesResponseFlag = false;
    int32_t ret = 0;
    ret = g_rilInterface->SetLocateUpdates(SLOTID_2, -1, RilRegNotifyMode::REG_NOT_NOTIFY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(true, g_setLocateUpdatesResponseFlag);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdatesResponse_0500
 * @tc.name  : testV1SetLocateUpdatesResponse005
 * @tc.desc  : Call function SetLocateUpdatesResponse slotId is SLOTID_1 SerialId is -1 Notification mode is
 * REG_NOT_NOTIFY
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetLocateUpdatesResponse005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setLocateUpdatesResponseFlag = false;
    int32_t ret = 0;
    ret = g_rilInterface->SetLocateUpdates(SLOTID_1, -1, RilRegNotifyMode::REG_NOT_NOTIFY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(true, g_setLocateUpdatesResponseFlag);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdatesResponse_0600
 * @tc.name  : testV1SetLocateUpdatesResponse006
 * @tc.desc  : Call function SetLocateUpdatesResponse slotId is SLOTID_2 SerialId is -1 Notification mode is
 * REG_NOTIFY_STAT_ONLY
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetLocateUpdatesResponse006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setLocateUpdatesResponseFlag = false;
    int32_t ret = 0;
    ret = g_rilInterface->SetLocateUpdates(SLOTID_2, -1, RilRegNotifyMode::REG_NOTIFY_STAT_ONLY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(true, g_setLocateUpdatesResponseFlag);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilterResponse_0100
 * @tc.name  : testV1SetNotificationFilterResponse001
 * @tc.desc  : Call function SetNotificationFilterResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNotificationFilterResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setNotificationFilterResponseFlag = false;
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_NONE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setNotificationFilterResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilterResponse_0200
 * @tc.name  : testV1SetNotificationFilterResponse002
 * @tc.desc  : Call function SetNotificationFilterResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNotificationFilterResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setNotificationFilterResponseFlag = false;
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_NONE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setNotificationFilterResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilterResponse_0300
 * @tc.name  : testV1SetNotificationFilterResponse003
 * @tc.desc  : Call function SetNotificationFilterResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNotificationFilterResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setNotificationFilterResponseFlag = false;
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, -1, static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_NONE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setNotificationFilterResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilterResponse_0400
 * @tc.name  : testV1SetNotificationFilterResponse004
 * @tc.desc  : Call function SetNotificationFilterResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetNotificationFilterResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setNotificationFilterResponseFlag = false;
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, -1, static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_NONE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setNotificationFilterResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceStateResponse_0100
 * @tc.name  : testV1SetDeviceStateResponse001
 * @tc.desc  : Call function SetDeviceStateResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDeviceStateResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setDeviceStateResponseFlag = false;
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_1, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setDeviceStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceStateResponse_0200
 * @tc.name  : testV1SetDeviceStateResponse002
 * @tc.desc  : Call function SetDeviceStateResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDeviceStateResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setDeviceStateResponseFlag = false;
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_2, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setDeviceStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceStateResponse_0300
 * @tc.name  : testV1SetDeviceStateResponse003
 * @tc.desc  : Call function SetDeviceStateResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDeviceStateResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setDeviceStateResponseFlag = false;
    int32_t ret =
        g_rilInterface->SetDeviceState(SLOTID_1, -1, static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setDeviceStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceStateResponse_0400
 * @tc.name  : testV1SetDeviceStateResponse004
 * @tc.desc  : Call function SetDeviceStateResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SetDeviceStateResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setDeviceStateResponseFlag = false;
    int32_t ret =
        g_rilInterface->SetDeviceState(SLOTID_2, -1, static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setDeviceStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSmsResponse_0100
 * @tc.name  : testV1SendGsmSmsResponse001
 * @tc.desc  : Call function SendGsmSmsResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SendGsmSmsResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_sendGsmSmsResponseFlag = false;
    GsmSmsMessageInfo msg;
    msg.smscPdu = "+-*%";
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendGsmSmsResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSmsResponse_0200
 * @tc.name  : testV1SendGsmSmsResponse002
 * @tc.desc  : Call function SendGsmSmsResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SendGsmSmsResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_sendGsmSmsResponseFlag = false;
    GsmSmsMessageInfo msg;
    msg.smscPdu = "+-*%";
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendGsmSmsResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSmsResponse_0300
 * @tc.name  : testV1SendGsmSmsResponse003
 * @tc.desc  : Call function SendGsmSmsResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SendGsmSmsResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_sendGsmSmsResponseFlag = false;
    GsmSmsMessageInfo msg;
    msg.smscPdu = "";
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, -1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendGsmSmsResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSmsResponse_0400
 * @tc.name  : testV1SendGsmSmsResponse004
 * @tc.desc  : Call function SendGsmSmsResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SendGsmSmsResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_sendGsmSmsResponseFlag = false;
    GsmSmsMessageInfo msg;
    msg.smscPdu = "";
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_2, -1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendGsmSmsResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSmsResponse_0100
 * @tc.name  : testV1SendCdmaSmsResponse001
 * @tc.desc  : Call function SendCdmaSmsResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SendCdmaSmsResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_sendCdmaSmsResponseFlag = false;
    SendCdmaSmsMessageInfo msg;
    msg.serial = 2;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendCdmaSmsResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSmsResponse_0200
 * @tc.name  : testV1SendCdmaSmsResponse002
 * @tc.desc  : Call function SendCdmaSmsResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SendCdmaSmsResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_sendCdmaSmsResponseFlag = false;
    SendCdmaSmsMessageInfo msg;
    msg.serial = 2;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendCdmaSmsResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSmsResponse_0300
 * @tc.name  : testV1SendCdmaSmsResponse003
 * @tc.desc  : Call function SendCdmaSmsResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SendCdmaSmsResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_sendCdmaSmsResponseFlag = false;
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = "";
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, -1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendCdmaSmsResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSmsResponse_0400
 * @tc.name  : testV1SendCdmaSmsResponse004
 * @tc.desc  : Call function SendCdmaSmsResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1SendCdmaSmsResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_sendCdmaSmsResponseFlag = false;
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = "";
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_2, -1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendCdmaSmsResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessageResponse_0100
 * @tc.name  : testV1AddSimMessageResponse001
 * @tc.desc  : Call function AddSimMessageResponse slotId is SLOTID_1 SerialId is 100
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1AddSimMessageResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_addSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "!&#";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_addSimMessageResponseFlag);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessageResponse_0200
 * @tc.name  : testV1AddSimMessageResponse002
 * @tc.desc  : Call function AddSimMessageResponse slotId is SLOTID_2 SerialId is 100
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1AddSimMessageResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_addSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_addSimMessageResponseFlag);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessageResponse_0300
 * @tc.name  : testV1AddSimMessageResponse003
 * @tc.desc  : Call function AddSimMessageResponse slotId is SLOTID_1 SerialId is 1-
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1AddSimMessageResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_addSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = -1;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_addSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessageResponse_0400
 * @tc.name  : testV1AddSimMessageResponse004
 * @tc.desc  : Call function AddSimMessageResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1AddSimMessageResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_addSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_addSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelSimMessageResponse_0100
 * @tc.name  : testV1DelSimMessageResponse001
 * @tc.desc  : Call function DelSimMessageResponse slotId is SLOTID_1 SerialId is 100
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelSimMessageResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_delSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "!&#";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
    EXPECT_EQ(true, g_delSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelSimMessageResponse_0200
 * @tc.name  : testV1DelSimMessageResponse002
 * @tc.desc  : Call function DelSimMessageResponse slotId is SLOTID_2 SerialId is 100
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelSimMessageResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_delSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
    EXPECT_EQ(true, g_delSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelSimMessageResponse_0300
 * @tc.name  : testV1DelSimMessageResponse003
 * @tc.desc  : Call function DelSimMessageResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelSimMessageResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_delSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = -1;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelSimMessageResponse_0400
 * @tc.name  : testV1DelSimMessageResponse004
 * @tc.desc  : Call function DelSimMessageResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelSimMessageResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_delSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessageResponse_0100
 * @tc.name  : testV1UpdateSimMessageResponse001
 * @tc.desc  : Call function UpdateSimMessageResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateSimMessageResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_updateSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_updateSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessageResponse_0200
 * @tc.name  : testV1UpdateSimMessageResponse002
 * @tc.desc  : Call function UpdateSimMessageResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateSimMessageResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_updateSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_updateSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessageResponse_0300
 * @tc.name  : testV1UpdateSimMessageResponse003
 * @tc.desc  : Call function UpdateSimMessageResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateSimMessageResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_updateSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_updateSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessageResponse_0400
 * @tc.name  : testV1UpdateSimMessageResponse004
 * @tc.desc  : Call function UpdateSimMessageResponse slotId is SLOTID2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1UpdateSimMessageResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_updateSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_updateSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessageResponse_0100
 * @tc.name  : testV1AddCdmaSimMessageResponse001
 * @tc.desc  : Call function AddCdmaSimMessageResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1AddCdmaSimMessageResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_addCdmaSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_addCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessageResponse_0200
 * @tc.name  : testV1AddCdmaSimMessageResponse002
 * @tc.desc  : Call function AddCdmaSimMessageResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1AddCdmaSimMessageResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_addCdmaSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_addCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessageResponse_0300
 * @tc.name  : testV1AddCdmaSimMessageResponse003
 * @tc.desc  : Call function AddCdmaSimMessageResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1AddCdmaSimMessageResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_addCdmaSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_addCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessageResponse_0400
 * @tc.name  : testV1AddCdmaSimMessageResponse004
 * @tc.desc  : Call function AddCdmaSimMessageResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1AddCdmaSimMessageResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_addCdmaSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_addCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessageResponse_0100
 * @tc.name  : testV1DelCdmaSimMessageResponse001
 * @tc.desc  : Call function DelCdmaSimMessageResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelCdmaSimMessageResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_delCdmaSimMessageResponseFlag = false;
    int32_t index = 255;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_1, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_delCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessageResponse_0200
 * @tc.name  : testV1DelCdmaSimMessageResponse002
 * @tc.desc  : Call function DelCdmaSimMessageResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelCdmaSimMessageResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_delCdmaSimMessageResponseFlag = false;
    int32_t index = 255;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_2, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_delCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessageResponse_0300
 * @tc.name  : testV1DelCdmaSimMessageResponse003
 * @tc.desc  : Call function DelCdmaSimMessageResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelCdmaSimMessageResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_delCdmaSimMessageResponseFlag = false;
    int32_t index = -1;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_1, -1, index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_delCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessageResponse_0400
 * @tc.name  : testV1DelCdmaSimMessageResponse004
 * @tc.desc  : Call function DelCdmaSimMessageResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1DelCdmaSimMessageResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_delCdmaSimMessageResponseFlag = false;
    int32_t index = -1;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_2, -1, index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_delCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrengthResponse_0100
 * @tc.name  : testV1GetSignalStrengthResponse001
 * @tc.desc  : Call function GetSignalStrengthResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSignalStrengthResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getSignalStrengthResponseFlag = false;
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSignalStrengthResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrengthResponse_0200
 * @tc.name  : testV1GetSignalStrengthResponse002
 * @tc.desc  : Call function GetSignalStrengthResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSignalStrengthResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getSignalStrengthResponseFlag = false;
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSignalStrengthResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrengthResponse_0300
 * @tc.name  : testV1GetSignalStrengthResponse003
 * @tc.desc  : Call function GetSignalStrengthResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSignalStrengthResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getSignalStrengthResponseFlag = false;
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSignalStrengthResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrengthResponse_0400
 * @tc.name  : testV1GetSignalStrengthResponse004
 * @tc.desc  : Call function GetSignalStrengthResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetSignalStrengthResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getSignalStrengthResponseFlag = false;
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSignalStrengthResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatusResponse_0100
 * @tc.name  : testV1GetCsRegStatusResponse001
 * @tc.desc  : Call function GetCsRegStatusResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetCsRegStatusResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCsRegStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCsRegStatusResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatusResponse_0200
 * @tc.name  : testV1GetCsRegStatusResponse002
 * @tc.desc  : Call function GetCsRegStatusResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetCsRegStatusResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCsRegStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCsRegStatusResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatusResponse_0300
 * @tc.name  : testV1GetCsRegStatusResponse003
 * @tc.desc  : Call function GetCsRegStatusResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetCsRegStatusResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCsRegStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCsRegStatusResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatusResponse_0400
 * @tc.name  : testV1GetCsRegStatusResponse004
 * @tc.desc  : Call function GetCsRegStatusResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetCsRegStatusResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCsRegStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCsRegStatusResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatusResponse_0100
 * @tc.name  : testV1GetPsRegStatusResponse001
 * @tc.desc  : Call function GetPsRegStatusResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetPsRegStatusResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getPsRegStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPsRegStatusResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatusResponse_0200
 * @tc.name  : testV1GetPsRegStatusResponse002
 * @tc.desc  : Call function GetPsRegStatusResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetPsRegStatusResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getPsRegStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPsRegStatusResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatusResponse_0300
 * @tc.name  : testV1GetPsRegStatusResponse003
 * @tc.desc  : Call function GetPsRegStatusResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetPsRegStatusResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getPsRegStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPsRegStatusResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatusResponse_0400
 * @tc.name  : testV1GetPsRegStatusResponse004
 * @tc.desc  : Call function GetPsRegStatusResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetPsRegStatusResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getPsRegStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPsRegStatusResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfoResponse_0100
 * @tc.name  : testV1GetOperatorInfoResponse001
 * @tc.desc  : Call function GetOperatorInfoResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetOperatorInfoResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getOperatorInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getOperatorInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfoResponse_0200
 * @tc.name  : testV1GetOperatorInfoResponse002
 * @tc.desc  : Call function GetOperatorInfoResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetOperatorInfoResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getOperatorInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getOperatorInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfoResponse_0300
 * @tc.name  : testV1GetOperatorInfoResponse003
 * @tc.desc  : Call function GetOperatorInfoResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional5th, testV1GetOperatorInfoResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getOperatorInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getOperatorInfoResponseFlag);
}