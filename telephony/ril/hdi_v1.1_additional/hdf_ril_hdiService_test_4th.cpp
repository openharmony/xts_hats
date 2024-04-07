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
#include "cstdlib"
#include "hdf_base.h"
#include "hdf_log.h"
#include "hdf_ril_callback_common.h"
#include "map"
#include "mutex"
#include "securec.h"
#include "unistd.h"
#include "v1_3/iril.h"
#include "gtest/gtest.h"

using namespace OHOS::HDI::Ril::V1_3;
using namespace testing::ext;
class HdfRilHdiTestAdditional4th : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
/**
** HdfRilHdiTestAdditional4th implement
**/
void HdfRilHdiTestAdditional4th::SetUpTestCase()
{
    g_rilInterface = OHOS::HDI::Ril::V1_3::IRil::Get();
    if (g_rilInterface != nullptr) {
        g_rilInterface->SetCallback1_3(&g_callback);
        g_rilInterface->SendRilAck();
        g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
        g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    }
}

void HdfRilHdiTestAdditional4th::TearDownTestCase() {}
void HdfRilHdiTestAdditional4th::SetUp() {}
void HdfRilHdiTestAdditional4th::TearDown() {}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfigResponse_0100
 * @tc.name  : testV1SetCdmaCBConfigResponse001
 * @tc.desc  : Call function SetCdmaCBConfigResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetCdmaCBConfigResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setCdmaCBConfigResponseFlag = false;
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.serial = 1;
    broadcastInfoList.size = 1;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(g_setCdmaCBConfigResponseFlag, true);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfigResponse_0100
 * @tc.name  : testV1GetCdmaCBConfigResponse001
 * @tc.desc  : Call function GetCdmaCBConfigResponse
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetCdmaCBConfigResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCdmaCBConfigResponseFlag = false;
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(g_getCdmaCBConfigResponseFlag, true);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreModeResponse_0100
 * @tc.name: testV1SendSmsMoreModeResponse001
 * @tc.desc: Call function SendSmsMoreModeResponse
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SendSmsMoreModeResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_sendSmsMoreModeResponseFlag = false;
    GsmSmsMessageInfo msg;
    msg.serial = 1;
    msg.state = 1;
    msg.smscPdu = TEST_STORAGE_PDU;
    msg.pdu = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(g_sendSmsMoreModeResponseFlag, true);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAckResponse_0100
 * @tc.name  : testV1SendSmsAckResponse001
 * @tc.desc  : Call function SendSmsAckResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SendSmsAckResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_sendSmsAckResponseFlag = false;
    ModeData data;
    data.serial = 1;
    data.result = true;
    data.mode = 1000000001;
    data.pdu = "abc";
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_1, GetSerialId(), data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(g_sendSmsAckResponseFlag, true);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNrOptionModeResponse_0100
 * @tc.name  : testV1SetNrOptionModeResponse001
 * @tc.desc  : Call function SetNrOptionModeResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNrOptionModeResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setNrOptionModeResponseFlag = false;
    int32_t ret = g_rilInterface->SetNrOptionMode(SLOTID_1, GetSerialId(), 31);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(g_setNrOptionModeResponseFlag, true);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNrOptionModeResponse_0100
 * @tc.name  : testV1GetNrOptionModeResponse001
 * @tc.desc  : Call function GetNrOptionModeResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetNrOptionModeResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getNrOptionModeResponseFlag = false;
    int32_t ret = g_rilInterface->SetNrOptionMode(SLOTID_1, GetSerialId(), 31);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = 0;
    ret1 = g_rilInterface->GetNrOptionMode(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret1);
    EXPECT_EQ(g_getNrOptionModeResponseFlag, true);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_0300
 * @tc.name  : testV1SetNotificationFilter001
 * @tc.desc  : test SetNotificationFilter indicates that the slot1 filter is disabled
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_NONE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_0400
 * @tc.name  : testV1SetNotificationFilter002
 * @tc.desc  : test SetNotificationFilter indicates that the slot2 filter is disabled
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_NONE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_0500
 * @tc.name  : testV1SetNotificationFilter003
 * @tc.desc  : test SetNotificationFilter indicates that the slot1 signal strength
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_SIGNAL_STRENGTH));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_0600
 * @tc.name  : testV1SetNotificationFilter004
 * @tc.desc  : test SetNotificationFilter indicates that the slot2 signal strength
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_SIGNAL_STRENGTH));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_0700
 * @tc.name  : testV1SetNotificationFilter005
 * @tc.desc  : test SetNotificationFilter indicates that the slot1 network registration status
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_NETWORK_STATE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_0800
 * @tc.name  : testV1SetNotificationFilter006
 * @tc.desc  : test SetNotificationFilter indicates that the slot2 network registration status
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_NETWORK_STATE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_0900
 * @tc.name  : testV1SetNotificationFilter007
 * @tc.desc  : test SetNotificationFilter indicates that the slot1 data connection status
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_DATA_CALL));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1000
 * @tc.name  : testV1SetNotificationFilter008
 * @tc.desc  : test SetNotificationFilter indicates that the slot2 data connection status
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_DATA_CALL));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1100
 * @tc.name  : testV1SetNotificationFilter009
 * @tc.desc  : test SetNotificationFilter indicates that the slot1 link capacity
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_LINK_CAPACITY));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1200
 * @tc.name  : testV1SetNotificationFilter010
 * @tc.desc  : test SetNotificationFilter indicates that the slot2 link capacity
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_LINK_CAPACITY));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1300
 * @tc.name  : testV1SetNotificationFilter011
 * @tc.desc  : test SetNotificationFilter indicates that the slot1 physical channel configuration
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_PHYSICAL_CHANNEL_CONFIG));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1400
 * @tc.name  : testV1SetNotificationFilter012
 * @tc.desc  : test SetNotificationFilter indicates that the slot2 physical channel configuration
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_PHYSICAL_CHANNEL_CONFIG));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1500
 * @tc.name  : testV1SetNotificationFilter013
 * @tc.desc  : test SetNotificationFilter slot1 bit 1 and bit 2
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 3);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1600
 * @tc.name  : testV1SetNotificationFilter014
 * @tc.desc  : test SetNotificationFilter slot 2 bit 1 and bit 2
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 3);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1700
 * @tc.name  : testV1SetNotificationFilter015
 * @tc.desc  : test SetNotificationFilter slot1 bit 1 and bit 3
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 5);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1800
 * @tc.name  : testV1SetNotificationFilter016
 * @tc.desc  : test SetNotificationFilter slot2 bit 1 and bit 3
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 5);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_1900
 * @tc.name  : testV1SetNotificationFilter017
 * @tc.desc  : test SetNotificationFilter slot1 bit 2 and bit 3
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 6);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2000
 * @tc.name  : testV1SetNotificationFilter018
 * @tc.desc  : test SetNotificationFilter slot2 bit 2 and bit 3
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 6);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2100
 * @tc.name  : testV1SetNotificationFilter019
 * @tc.desc  : test SetNotificationFilter slot1 bit 1 bit 2 and bit 3
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 7);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2200
 * @tc.name  : testV1SetNotificationFilter020
 * @tc.desc  : test SetNotificationFilter slot2 bit 1 bit 2 and bit 3
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 7);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2300
 * @tc.name  : testV1SetNotificationFilter021
 * @tc.desc  : test SetNotificationFilter slot1 bit 1 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 9);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2400
 * @tc.name  : testV1SetNotificationFilter022
 * @tc.desc  : test SetNotificationFilter slot2 bit 1 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 9);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2500
 * @tc.name  : testV1SetNotificationFilter023
 * @tc.desc  : test SetNotificationFilter slot1 bit 2 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 10);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2600
 * @tc.name  : testV1SetNotificationFilter024
 * @tc.desc  : test SetNotificationFilter slot2 bit 2 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 10);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2700
 * @tc.name  : testV1SetNotificationFilter025
 * @tc.desc  : test SetNotificationFilter slot1 bit 1 bit 2 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter025, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 11);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2800
 * @tc.name  : testV1SetNotificationFilter026
 * @tc.desc  : test SetNotificationFilter slot2 bit 1 bit 2 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter026, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 11);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_2900
 * @tc.name  : testV1SetNotificationFilter027
 * @tc.desc  : test SetNotificationFilter slot1 bit 3 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter027, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 12);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_3000
 * @tc.name  : testV1SetNotificationFilter028
 * @tc.desc  : test SetNotificationFilter slot2 bit 3 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter028, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 12);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_3100
 * @tc.name  : testV1SetNotificationFilter029
 * @tc.desc  : test SetNotificationFilter slot1 bit 1 bit 3 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter029, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 13);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_3200
 * @tc.name  : testV1SetNotificationFilter030
 * @tc.desc  : test SetNotificationFilter slot2 bit 1 bit 3 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter030, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 13);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_3300
 * @tc.name  : testV1SetNotificationFilter031
 * @tc.desc  : test SetNotificationFilter slot1 bit 2 bit 3 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter031, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 14);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_3400
 * @tc.name  : testV1SetNotificationFilter032
 * @tc.desc  : test SetNotificationFilter slot2 bit 2 bit 3 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter032, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 14);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_3500
 * @tc.name  : testV1SetNotificationFilter033
 * @tc.desc  : test SetNotificationFilter slot1 bit 1 bit 2 bit 3 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter033, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_1, GetSerialId(), 15);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_3600
 * @tc.name  : testV1SetNotificationFilter034
 * @tc.desc  : test SetNotificationFilter slot2 bit 1 bit 2 bit 3 and bit 4
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter034, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(SLOTID_2, GetSerialId(), 15);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_3700
 * @tc.name  : testV1SetNotificationFilter035
 * @tc.desc  : test SetNotificationFilter slot1 Serial Id is invalid.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter035, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, -1, static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_ALL));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNotificationFilter_3800
 * @tc.name  : testV1SetNotificationFilter036
 * @tc.desc  : test SetNotificationFilter slot2 Serial Id is invalid.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNotificationFilter036, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, -1, static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_ALL));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_0300
 * @tc.name  : testV1SetDeviceState001
 * @tc.desc  : test SetDeviceState the slot1 low data mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_1, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_0400
 * @tc.name  : testV1SetDeviceState002
 * @tc.desc  : test SetDeviceState the slot2 low data mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_2, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_0500
 * @tc.name  : testV1SetDeviceState003
 * @tc.desc  : test SetDeviceState the slot1 charging mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_1, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::CHARGING_STATE), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_0600
 * @tc.name  : testV1SetDeviceState004
 * @tc.desc  : test SetDeviceState the slot2 charging mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_2, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::CHARGING_STATE), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_0700
 * @tc.name  : testV1SetDeviceState005
 * @tc.desc  : test SetDeviceState the slot1 charging mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_1, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::CHARGING_STATE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_0800
 * @tc.name  : testV1SetDeviceState006
 * @tc.desc  : test SetDeviceState the slot2 charging mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_2, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::CHARGING_STATE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_0900
 * @tc.name  : testV1SetDeviceState007
 * @tc.desc  : test SetDeviceState the slot1 power saving mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_1, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::POWER_SAVE_MODE), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_1000
 * @tc.name  : testV1SetDeviceState008
 * @tc.desc  : test SetDeviceState the slot2 power saving mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_2, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::POWER_SAVE_MODE), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_1100
 * @tc.name  : testV1SetDeviceState009
 * @tc.desc  : test SetDeviceState the slot1 power saving mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_1, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::POWER_SAVE_MODE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_1200
 * @tc.name  : testV1SetDeviceState010
 * @tc.desc  : test SetDeviceState the slot2 power saving mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_2, GetSerialId(),
                                                 static_cast<int32_t>(DeviceStateType::POWER_SAVE_MODE), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_1300
 * @tc.name  : testV1SetDeviceState011
 * @tc.desc  : test SetDeviceState the slot1 Invalid mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState011, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_1, GetSerialId(), 3, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDeviceState_1400
 * @tc.name  : testV1SetDeviceState012
 * @tc.desc  : test SetDeviceState the slot2 Invalid mode.
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDeviceState012, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(SLOTID_2, GetSerialId(), 3, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdates_0300
 * @tc.name  : testV1SetLocateUpdates001
 * @tc.desc  : test SetLocateUpdates Notification mode is REG_NOT_NOTIFY
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetLocateUpdates001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetLocateUpdates(SLOTID_1, GetSerialId(), RilRegNotifyMode::REG_NOT_NOTIFY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdates_0400
 * @tc.name  : testV1SetLocateUpdates002
 * @tc.desc  : test SetLocateUpdates Notification mode is REG_NOT_NOTIFY
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetLocateUpdates002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetLocateUpdates(SLOTID_2, GetSerialId(), RilRegNotifyMode::REG_NOT_NOTIFY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdates_0500
 * @tc.name  : testV1SetLocateUpdates003
 * @tc.desc  : test SetLocateUpdates Notification mode is REG_NOTIFY_STAT_ONLY
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetLocateUpdates003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetLocateUpdates(SLOTID_1, GetSerialId(), RilRegNotifyMode::REG_NOTIFY_STAT_ONLY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdates_0600
 * @tc.name  : testV1SetLocateUpdates004
 * @tc.desc  : test SetLocateUpdates Notification mode is REG_NOTIFY_STAT_ONLY
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetLocateUpdates004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetLocateUpdates(SLOTID_2, GetSerialId(), RilRegNotifyMode::REG_NOTIFY_STAT_ONLY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdates_0700
 * @tc.name  : testV1SetLocateUpdates005
 * @tc.desc  : test SetLocateUpdates slot1 Serial Id is invalid
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetLocateUpdates005, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetLocateUpdates(SLOTID_1, -1, RilRegNotifyMode::REG_NOTIFY_STAT_ONLY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLocateUpdates_0800
 * @tc.name  : testV1SetLocateUpdates006
 * @tc.desc  : test SetLocateUpdates slot2 Serial Id is invalid
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetLocateUpdates006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetLocateUpdates(SLOTID_2, -1, RilRegNotifyMode::REG_NOTIFY_STAT_ONLY);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_0300
 * @tc.name  : testV1SetPreferredNetwork001
 * @tc.desc  : test SetPreferredNetwork NetworkType is automatic
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_0400
 * @tc.name  : testV1SetPreferredNetwork002
 * @tc.desc  : test SetPreferredNetwork NetworkType is automatic
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_0500
 * @tc.name  : testV1SetPreferredNetwork003
 * @tc.desc  : test SetPreferredNetwork NetworkType is GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_0600
 * @tc.name  : testV1SetPreferredNetwork004
 * @tc.desc  : test SetPreferredNetwork NetworkType is GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_0700
 * @tc.name  : testV1SetPreferredNetwork005
 * @tc.desc  : test SetPreferredNetwork NetworkType is WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_0800
 * @tc.name  : testV1SetPreferredNetwork006
 * @tc.desc  : test SetPreferredNetwork NetworkType is WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_0900
 * @tc.name  : testV1SetPreferredNetwork007
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1000
 * @tc.name  : testV1SetPreferredNetwork008
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1100
 * @tc.name  : testV1SetPreferredNetwork009
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1200
 * @tc.name  : testV1SetPreferredNetwork010
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1300
 * @tc.name  : testV1SetPreferredNetwork011
 * @tc.desc  : test SetPreferredNetwork NetworkType is WCDMA and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1400
 * @tc.name  : testV1SetPreferredNetwork012
 * @tc.desc  : test SetPreferredNetwork NetworkType is WCDMA and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1500
 * @tc.name  : testV1SetPreferredNetwork013
 * @tc.desc  : test SetPreferredNetwork NetworkType is CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1600
 * @tc.name  : testV1SetPreferredNetwork014
 * @tc.desc  : test SetPreferredNetwork NetworkType is CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1700
 * @tc.name  : testV1SetPreferredNetwork015
 * @tc.desc  : test SetPreferredNetwork NetworkType is EV-DO
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_EVDO));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1800
 * @tc.name  : testV1SetPreferredNetwork016
 * @tc.desc  : test SetPreferredNetwork NetworkType is EV-DO
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_EVDO));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_1900
 * @tc.name  : testV1SetPreferredNetwork017
 * @tc.desc  : test SetPreferredNetwork NetworkType is EV-DO and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2000
 * @tc.name  : testV1SetPreferredNetwork018
 * @tc.desc  : test SetPreferredNetwork NetworkType is EV-DO and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2100
 * @tc.name  : testV1SetPreferredNetwork019
 * @tc.desc  : test SetPreferredNetwork NetworkType is WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2200
 * @tc.name  : testV1SetPreferredNetwork020
 * @tc.desc  : test SetPreferredNetwork NetworkType is WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2300
 * @tc.name  : testV1SetPreferredNetwork021
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2400
 * @tc.name  : testV1SetPreferredNetwork022
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2500
 * @tc.name  : testV1SetPreferredNetwork023
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2600
 * @tc.name  : testV1SetPreferredNetwork024
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2700
 * @tc.name  : testV1SetPreferredNetwork025
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork025, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2800
 * @tc.name  : testV1SetPreferredNetwork026
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork026, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_2900
 * @tc.name  : testV1SetPreferredNetwork027
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork027, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3000
 * @tc.name  : testV1SetPreferredNetwork028
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork028, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3100
 * @tc.name  : testV1SetPreferredNetwork029
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork029, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3200
 * @tc.name  : testV1SetPreferredNetwork030
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork030, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3300
 * @tc.name  : testV1SetPreferredNetwork031
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork031, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3400
 * @tc.name  : testV1SetPreferredNetwork032
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork032, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3500
 * @tc.name  : testV1SetPreferredNetwork033
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE and TD-SCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork033, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3600
 * @tc.name  : testV1SetPreferredNetwork034
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE and TD-SCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork034, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3700
 * @tc.name  : testV1SetPreferredNetwork035
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, TDSCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork035, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3800
 * @tc.name  : testV1SetPreferredNetwork036
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, TDSCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork036, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_3900
 * @tc.name  : testV1SetPreferredNetwork037
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, TD-SCDMA, and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork037, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4000
 * @tc.name  : testV1SetPreferredNetwork038
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, TD-SCDMA, and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork038, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4100
 * @tc.name  : testV1SetPreferredNetwork039
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, TDSCDMA, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork039, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4200
 * @tc.name  : testV1SetPreferredNetwork040
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, TDSCDMA, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork040, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4300
 * @tc.name  : testV1SetPreferredNetwork041
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork041, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4400
 * @tc.name  : testV1SetPreferredNetwork042
 * @tc.desc  : test SetPreferredNetwork NetworkType is TD-SCDMA, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork042, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4500
 * @tc.name  : testV1SetPreferredNetwork043
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, TD-SCDMA, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork043, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4600
 * @tc.name  : testV1SetPreferredNetwork044
 * @tc.desc  : test SetPreferredNetwork NetworkType is LTE, TD-SCDMA, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork044, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4700
 * @tc.name  : testV1SetPreferredNetwork045
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork045, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(SLOTID_1, GetSerialId(),
                                                      static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4800
 * @tc.name  : testV1SetPreferredNetwork046
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork046, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(SLOTID_2, GetSerialId(),
                                                      static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_4900
 * @tc.name  : testV1SetPreferredNetwork047
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR and LTE
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork047, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5000
 * @tc.name  : testV1SetPreferredNetwork048
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR and LTE
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork048, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5100
 * @tc.name  : testV1SetPreferredNetwork049
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork049, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5200
 * @tc.name  : testV1SetPreferredNetwork050
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork050, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5300
 * @tc.name  : testV1SetPreferredNetwork051
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork051, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5400
 * @tc.name  : testV1SetPreferredNetwork052
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork052, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5500
 * @tc.name  : testV1SetPreferredNetwork053
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork053, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5600
 * @tc.name  : testV1SetPreferredNetwork054
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork054, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5700
 * @tc.name  : testV1SetPreferredNetwork055
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork055, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5800
 * @tc.name  : testV1SetPreferredNetwork056
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork056, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_5900
 * @tc.name  : testV1SetPreferredNetwork057
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, and TD-SCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork057, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_6000
 * @tc.name  : testV1SetPreferredNetwork058
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, and TD-SCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork058, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_6100
 * @tc.name  : testV1SetPreferredNetwork059
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, TDSCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork059, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_6200
 * @tc.name  : testV1SetPreferredNetwork060
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, TDSCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork060, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_6300
 * @tc.name  : testV1SetPreferredNetwork061
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, TD-SCDMA, and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork061, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_6400
 * @tc.name  : testV1SetPreferredNetwork062
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, TD-SCDMA, and WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork062, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_6500
 * @tc.name  : testV1SetPreferredNetwork063
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, TD-SCDMA, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork063, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_6600
 * @tc.name  : testV1SetPreferredNetwork064
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, TD-SCDMA, WCDMA, and GSM
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork064, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_6700
 * @tc.name  : testV1SetPreferredNetwork065
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, TD-SCDMA, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork065, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetwork_6800
 * @tc.name  : testV1SetPreferredNetwork066
 * @tc.desc  : test SetPreferredNetwork NetworkType is NR, LTE, TD-SCDMA, WCDMA, GSM, EV-DO, and CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetwork066, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(),
        static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPreferredNetwork_0300
 * @tc.name  : testV1GetPreferredNetwork001
 * @tc.desc  : test GetPreferredNetwork Serial Id is invalid
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPreferredNetwork001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPreferredNetwork_0400
 * @tc.name  : testV1GetPreferredNetwork002
 * @tc.desc  : test GetPreferredNetwork Serial Id is invalid
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPreferredNetwork002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetworkResponse_0100
 * @tc.name  : testV1SetPreferredNetworkResponse001
 * @tc.desc  : test slot1 SetPreferredNetworkResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetworkResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setPreferredNetworkResponseFlag = false;
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setPreferredNetworkResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetworkResponse_0200
 * @tc.name  : testV1SetPreferredNetworkResponse002
 * @tc.desc  : test slot2 SetPreferredNetworkResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetworkResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setPreferredNetworkResponseFlag = false;
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setPreferredNetworkResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetworkResponse_0300
 * @tc.name  : testV1SetPreferredNetworkResponse003
 * @tc.desc  : test slot1 SetPreferredNetworkResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetworkResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setPreferredNetworkResponseFlag = false;
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, -1, static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setPreferredNetworkResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetPreferredNetworkResponse_0400
 * @tc.name  : testV1SetPreferredNetworkResponse004
 * @tc.desc  : test slot2 SetPreferredNetworkResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetPreferredNetworkResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setPreferredNetworkResponseFlag = false;
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, -1, static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setPreferredNetworkResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPreferredNetworkResponse_0100
 * @tc.name  : testV1GetPreferredNetworkResponse001
 * @tc.desc  : test slot1 GetPreferredNetworkResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPreferredNetworkResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getPreferredNetworkResponseFlag = false;
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPreferredNetworkResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPreferredNetworkResponse_0200
 * @tc.name  : testV1GetPreferredNetworkResponse002
 * @tc.desc  : test slot2 GetPreferredNetworkResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPreferredNetworkResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getPreferredNetworkResponseFlag = false;
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPreferredNetworkResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPreferredNetworkResponse_0300
 * @tc.name  : testV1GetPreferredNetworkResponse003
 * @tc.desc  : test slot1 GetPreferredNetworkResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPreferredNetworkResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getPreferredNetworkResponseFlag = false;
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPreferredNetworkResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPreferredNetworkResponse_0400
 * @tc.name  : testV1GetPreferredNetworkResponse004
 * @tc.desc  : test slot2 GetPreferredNetworkResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPreferredNetworkResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getPreferredNetworkResponseFlag = false;
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPreferredNetworkResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfigResponse_0100
 * @tc.name  : testV1GetPhysicalChannelConfigResponse001
 * @tc.desc  : test slot1 GetPhysicalChannelConfigResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPhysicalChannelConfigResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getPhysicalChannelConfigResponseFlag = false;
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPhysicalChannelConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfigResponse_0200
 * @tc.name  : testV1GetPhysicalChannelConfigResponse002
 * @tc.desc  : test slot2 GetPhysicalChannelConfigResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPhysicalChannelConfigResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getPhysicalChannelConfigResponseFlag = false;
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPhysicalChannelConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfigResponse_0300
 * @tc.name  : testV1GetPhysicalChannelConfigResponse003
 * @tc.desc  : test slot1 GetPhysicalChannelConfigResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPhysicalChannelConfigResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getPhysicalChannelConfigResponseFlag = false;
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPhysicalChannelConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfigResponse_0400
 * @tc.name  : testV1GetPhysicalChannelConfigResponse004
 * @tc.desc  : test solt2 GetPhysicalChannelConfigResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetPhysicalChannelConfigResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getPhysicalChannelConfigResponseFlag = false;
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPhysicalChannelConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SeparateConferenceResponse_0100
 * @tc.name  : testV1SeparateConferenceResponse001
 * @tc.desc  : test slot1 SeparateConferenceResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SeparateConferenceResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_separateConferenceResponseFlag = false;
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_separateConferenceResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SeparateConferenceResponse_0200
 * @tc.name  : testV1SeparateConferenceResponse002
 * @tc.desc  : test slot2 SeparateConferenceResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SeparateConferenceResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_separateConferenceResponseFlag = false;
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_2, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_separateConferenceResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SeparateConferenceResponse_0300
 * @tc.name  : testV1SeparateConferenceResponse003
 * @tc.desc  : test slot1 SeparateConferenceResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SeparateConferenceResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_separateConferenceResponseFlag = false;
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, -1, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_separateConferenceResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SeparateConferenceResponse_0400
 * @tc.name  : testV1SeparateConferenceResponse004
 * @tc.desc  : test slot2 SeparateConferenceResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SeparateConferenceResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_separateConferenceResponseFlag = false;
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_2, -1, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_separateConferenceResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1CallSupplementResponse_0100
 * @tc.name  : testV1CallSupplementResponse001
 * @tc.desc  : test slot1 CallSupplementResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1CallSupplementResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_callSupplementResponseFlag = false;
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_callSupplementResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1CallSupplementResponse_0200
 * @tc.name  : testV1CallSupplementResponse002
 * @tc.desc  : test slot2 CallSupplementResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1CallSupplementResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_callSupplementResponseFlag = false;
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_callSupplementResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1CallSupplementResponse_0300
 * @tc.name  : testV1CallSupplementResponse003
 * @tc.desc  : test slot1 CallSupplementResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1CallSupplementResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_callSupplementResponseFlag = false;
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_NE(true, g_callSupplementResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1CallSupplementResponse_0400
 * @tc.name  : testV1CallSupplementResponse004
 * @tc.desc  : test slot2 CallSupplementResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1CallSupplementResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_callSupplementResponseFlag = false;
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_2, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_NE(true, g_callSupplementResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCallWaitingResponse_0100
 * @tc.name  : testV1GetCallWaitingResponse001
 * @tc.desc  : test slot1 GetCallWaitingResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetCallWaitingResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCallWaitingResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallWaitingResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCallWaitingResponse_0200
 * @tc.name  : testV1GetCallWaitingResponse002
 * @tc.desc  : test slot2 GetCallWaitingResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetCallWaitingResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallWaitingResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallWaitingResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCallWaitingResponse_0300
 * @tc.name  : testV1GetCallWaitingResponse003
 * @tc.desc  : test slot1 GetCallWaitingResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetCallWaitingResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCallWaitingResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallWaitingResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCallWaitingResponse_0400
 * @tc.name  : testV1GetCallWaitingResponse004
 * @tc.desc  : test slot2 GetCallWaitingResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetCallWaitingResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallWaitingResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_NE(true, g_getCallWaitingResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCallWaitingResponse_0100
 * @tc.name  : testV1SetCallWaitingResponse001
 * @tc.desc  : test slot1 SetCallWaitingResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetCallWaitingResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setCallWaitingResponseFlag = false;
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallWaitingResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCallWaitingResponse_0200
 * @tc.name  : testV1SetCallWaitingResponse002
 * @tc.desc  : test slot2 SetCallWaitingResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetCallWaitingResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setCallWaitingResponseFlag = false;
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallWaitingResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCallWaitingResponse_0300
 * @tc.name  : testV1SetCallWaitingResponse003
 * @tc.desc  : test slot1 SetCallWaitingResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetCallWaitingResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setCallWaitingResponseFlag = false;
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, -1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallWaitingResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCallWaitingResponse_0400
 * @tc.name  : testV1SetCallWaitingResponse004
 * @tc.desc  : test slot2 SetCallWaitingResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetCallWaitingResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setCallWaitingResponseFlag = false;
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_2, -1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallWaitingResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetUssdResponse_0100
 * @tc.name  : testV1GetUssdResponse001
 * @tc.desc  : test slot1 GetUssdResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetUssdResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getUssdResponseFlag = false;
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getUssdResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetUssdResponse_0200
 * @tc.name  : testV1GetUssdResponse002
 * @tc.desc  : test slot2 GetUssdResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetUssdResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getUssdResponseFlag = false;
    int32_t ret = g_rilInterface->GetUssd(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getUssdResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetUssdResponse_0300
 * @tc.name  : testV1GetUssdResponse003
 * @tc.desc  : test slot1 GetUssdResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetUssdResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getUssdResponseFlag = false;
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getUssdResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetUssdResponse_0400
 * @tc.name  : testV1GetUssdResponse004
 * @tc.desc  : test slot2 GetUssdResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetUssdResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getUssdResponseFlag = false;
    int32_t ret = g_rilInterface->GetUssd(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getUssdResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetMuteResponse_0100
 * @tc.name  : testV1SetMuteResponse001
 * @tc.desc  : test slot1 SetMuteResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetMuteResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setMuteResponseFlag = false;
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setMuteResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetMuteResponse_0200
 * @tc.name  : testV1SetMuteResponse002
 * @tc.desc  : test slot2 SetMuteResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetMuteResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setMuteResponseFlag = false;
    int32_t ret = g_rilInterface->SetMute(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setMuteResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetMuteResponse_0300
 * @tc.name  : testV1SetMuteResponse003
 * @tc.desc  : test slot1 SetMuteResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetMuteResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setMuteResponseFlag = false;
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setMuteResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetMuteResponse_0400
 * @tc.name  : testV1SetMuteResponse004
 * @tc.desc  : test slot2 SetMuteResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetMuteResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setMuteResponseFlag = false;
    int32_t ret = g_rilInterface->SetMute(SLOTID_2, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setMuteResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMuteResponse_0100
 * @tc.name  : testV1GetMuteResponse001
 * @tc.desc  : test slot1 GetMuteResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetMuteResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getMuteResponseFlag = false;
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getMuteResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMuteResponse_0200
 * @tc.name  : testV1GetMuteResponse002
 * @tc.desc  : test slot2 GetMuteResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetMuteResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getMuteResponseFlag = false;
    int32_t ret = g_rilInterface->GetMute(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getMuteResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMuteResponse_0300
 * @tc.name  : testV1GetMuteResponse003
 * @tc.desc  : test slot1 GetMuteResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetMuteResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getMuteResponseFlag = false;
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getMuteResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMuteResponse_0400
 * @tc.name  : testV1GetMuteResponse004
 * @tc.desc  : test slot2 GetMuteResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetMuteResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getMuteResponseFlag = false;
    int32_t ret = g_rilInterface->GetMute(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getMuteResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermittedResponse_0100
 * @tc.name  : testV1SetDataPermittedResponse001
 * @tc.desc  : test slot1 SetDataPermittedResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDataPermittedResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setDataPermittedResponseFlag = false;
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setDataPermittedResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermittedResponse_0200
 * @tc.name  : testV1SetDataPermittedResponse002
 * @tc.desc  : test slot2 SetDataPermittedResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDataPermittedResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setDataPermittedResponseFlag = false;
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setDataPermittedResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermittedResponse_0300
 * @tc.name  : testV1SetDataPermittedResponse003
 * @tc.desc  : test slot1 SetDataPermittedResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDataPermittedResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setDataPermittedResponseFlag = false;
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setDataPermittedResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetDataPermittedResponse_0400
 * @tc.name  : testV1SetDataPermittedResponse004
 * @tc.desc  : test slot2 SetDataPermittedResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetDataPermittedResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setDataPermittedResponseFlag = false;
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setDataPermittedResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ShutDownResponse_0100
 * @tc.name  : testV1ShutDownResponse001
 * @tc.desc  : test slot1 ShutDownResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1ShutDownResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_shutDownResponseFlag = false;
    int32_t ret = g_rilInterface->ShutDown(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_shutDownResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ShutDownResponse_0200
 * @tc.name  : testV1ShutDownResponse002
 * @tc.desc  : test slot2 ShutDownResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1ShutDownResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_shutDownResponseFlag = false;
    int32_t ret = g_rilInterface->ShutDown(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_shutDownResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ShutDownResponse_0300
 * @tc.name  : testV1ShutDownResponse003
 * @tc.desc  : test slot1 ShutDownResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1ShutDownResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_shutDownResponseFlag = false;
    int32_t ret = g_rilInterface->ShutDown(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_shutDownResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ShutDownResponse_0400
 * @tc.name  : testV1ShutDownResponse004
 * @tc.desc  : test slot2 ShutDownResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1ShutDownResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_shutDownResponseFlag = false;
    int32_t ret = g_rilInterface->ShutDown(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_shutDownResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioStateResponse_0100
 * @tc.name  : testV1SetRadioStateResponse001
 * @tc.desc  : test slot1 SetRadioStateResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetRadioStateResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setRadioStateResponseFlag = false;
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setRadioStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioStateResponse_0200
 * @tc.name  : testV1SetRadioStateResponse002
 * @tc.desc  : test slot2 SetRadioStateResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetRadioStateResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setRadioStateResponseFlag = false;
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setRadioStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioStateResponse_0300
 * @tc.name  : testV1SetRadioStateResponse003
 * @tc.desc  : test slot1 SetRadioStateResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetRadioStateResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setRadioStateResponseFlag = false;
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, -1, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setRadioStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioStateResponse_0400
 * @tc.name  : testV1SetRadioStateResponse004
 * @tc.desc  : test slot2 SetRadioStateResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetRadioStateResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setRadioStateResponseFlag = false;
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, -1, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setRadioStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioStateResponse_0100
 * @tc.name  : testV1GetRadioStateResponse001
 * @tc.desc  : test slot1 GetRadioStateResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetRadioStateResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getRadioStateResponseFlag = false;
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getRadioStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioStateResponse_0200
 * @tc.name  : testV1GetRadioStateResponse002
 * @tc.desc  : test slot2 GetRadioStateResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetRadioStateResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getRadioStateResponseFlag = false;
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getRadioStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioStateResponse_0300
 * @tc.name  : testV1GetRadioStateResponse003
 * @tc.desc  : test slot1 GetRadioStateResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetRadioStateResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getRadioStateResponseFlag = false;
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getRadioStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioStateResponse_0400
 * @tc.name  : testV1GetRadioStateResponse004
 * @tc.desc  : test slot2 GetRadioStateResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetRadioStateResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getRadioStateResponseFlag = false;
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getRadioStateResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImeiResponse_0100
 * @tc.name  : testV1GetImeiResponse001
 * @tc.desc  : test slot1 GetImeiResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImeiResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getImeiResponseFlag = false;
    int32_t ret = g_rilInterface->GetImei(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImeiResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImeiResponse_0200
 * @tc.name  : testV1GetImeiResponse002
 * @tc.desc  : test slot2 GetImeiResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImeiResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getImeiResponseFlag = false;
    int32_t ret = g_rilInterface->GetImei(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImeiResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImeiResponse_0300
 * @tc.name  : testV1GetImeiResponse003
 * @tc.desc  : test slot1 GetImeiResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImeiResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getImeiResponseFlag = false;
    int32_t ret = g_rilInterface->GetImei(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImeiResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImeiResponse_0400
 * @tc.name  : testV1GetImeiResponse004
 * @tc.desc  : test slot2 GetImeiResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImeiResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getImeiResponseFlag = false;
    int32_t ret = g_rilInterface->GetImei(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImeiResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImeiSvResponse_0100
 * @tc.name  : testV1GetImeiSvResponse001
 * @tc.desc  : test slot1 GetImeiSvResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImeiSvResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getImeiSvResponseFlag = false;
    int32_t ret = g_rilInterface->GetImeiSv(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImeiSvResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImeiSvResponse_0200
 * @tc.name  : testV1GetImeiSvResponse002
 * @tc.desc  : test slot2 GetImeiSvResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImeiSvResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getImeiSvResponseFlag = false;
    int32_t ret = g_rilInterface->GetImeiSv(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImeiSvResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImeiSvResponse_0300
 * @tc.name  : testV1GetImeiSvResponse003
 * @tc.desc  : test slot1 GetImeiSvResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImeiSvResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getImeiSvResponseFlag = false;
    int32_t ret = g_rilInterface->GetImeiSv(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImeiSvResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImeiSvResponse_0200
 * @tc.name  : testV1GetImeiSvResponse002
 * @tc.desc  : test slot2 GetImeiSvResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImeiSvResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getImeiSvResponseFlag = false;
    int32_t ret = g_rilInterface->GetImeiSv(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImeiSvResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMeidResponse_0100
 * @tc.name  : testV1GetMeidResponse001
 * @tc.desc  : test slot1 GetMeidResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetMeidResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getMeidResponseFlag = false;
    int32_t ret = g_rilInterface->GetMeid(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getMeidResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMeidResponse_0200
 * @tc.name  : testV1GetMeidResponse002
 * @tc.desc  : test slot2 GetMeidResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetMeidResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getMeidResponseFlag = false;
    int32_t ret = g_rilInterface->GetMeid(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getMeidResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMeidResponse_0300
 * @tc.name  : testV1GetMeidResponse003
 * @tc.desc  : test slot1 GetMeidResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetMeidResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getMeidResponseFlag = false;
    int32_t ret = g_rilInterface->GetMeid(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getMeidResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMeidResponse_0400
 * @tc.name  : testV1GetMeidResponse004
 * @tc.desc  : test slot2 GetMeidResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetMeidResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getMeidResponseFlag = false;
    int32_t ret = g_rilInterface->GetMeid(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getMeidResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetBasebandVersionResponse_0100
 * @tc.name  : testV1GetBasebandVersionResponse001
 * @tc.desc  : test slot1 GetBasebandVersionResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetBasebandVersionResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getBasebandVersionResponseFlag = false;
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getBasebandVersionResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetBasebandVersionResponse_0200
 * @tc.name  : testV1GetBasebandVersionResponse002
 * @tc.desc  : test slot2 GetBasebandVersionResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetBasebandVersionResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getBasebandVersionResponseFlag = false;
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getBasebandVersionResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetBasebandVersionResponse_0300
 * @tc.name  : testV1GetBasebandVersionResponse003
 * @tc.desc  : test slot1 GetBasebandVersionResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetBasebandVersionResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getBasebandVersionResponseFlag = false;
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getBasebandVersionResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetBasebandVersionResponse_0400
 * @tc.name  : testV1GetBasebandVersionResponse004
 * @tc.desc  : test slot2 GetBasebandVersionResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetBasebandVersionResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getBasebandVersionResponseFlag = false;
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getBasebandVersionResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImsiResponse_0100
 * @tc.name  : testV1GetImsiResponse001
 * @tc.desc  : test slot1 GetImsiResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImsiResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getImsiResponseFlag = false;
    int32_t ret = g_rilInterface->GetImsi(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImsiResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImsiResponse_0200
 * @tc.name  : testV1GetImsiResponse002
 * @tc.desc  : test slot2 GetImsiResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImsiResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getImsiResponseFlag = false;
    int32_t ret = g_rilInterface->GetImsi(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImsiResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImsiResponse_0300
 * @tc.name  : testV1GetImsiResponse003
 * @tc.desc  : test slot1 GetImsiResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImsiResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getImsiResponseFlag = false;
    int32_t ret = g_rilInterface->GetImsi(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImsiResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImsiResponse_0400
 * @tc.name  : testV1GetImsiResponse004
 * @tc.desc  : test slot2 GetImsiResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetImsiResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getImsiResponseFlag = false;
    int32_t ret = g_rilInterface->GetImsi(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getImsiResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSimResponse_0100
 * @tc.name  : testV1SetActiveSimResponse001
 * @tc.desc  : test slot1 SetActiveSimResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetActiveSimResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setActiveSimResponseFlag = false;
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setActiveSimResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSimResponse_0200
 * @tc.name  : testV1SetActiveSimResponse002
 * @tc.desc  : test slot2 SetActiveSimResponse is triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetActiveSimResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setActiveSimResponseFlag = false;
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, GetSerialId(), 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setActiveSimResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSimResponse_0300
 * @tc.name  : testV1SetActiveSimResponse003
 * @tc.desc  : test slot1 SetActiveSimResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetActiveSimResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setActiveSimResponseFlag = false;
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, -1, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setActiveSimResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSimResponse_0400
 * @tc.name  : testV1SetActiveSimResponse004
 * @tc.desc  : test slot2 SetActiveSimResponse is not triggered
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetActiveSimResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setActiveSimResponseFlag = false;
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, -1, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setActiveSimResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_0100
 * @tc.name: testV1AddSimMessage001
 * @tc.desc: Call function V1 AddSimMessage, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    int32_t ret1 = 0;
    for (int i = 0; i < 100; i++) {
        SmsMessageIOInfo msgIoInfo;
        msgIoInfo.smscPdu = TEST_SMSC_PDU;
        msgIoInfo.pdu = TEST_STORAGE_PDU;
        msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
        msgIoInfo.index = 100;
        ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);

        ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret1);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_0200
 * @tc.name: testV1AddSimMessage002
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, pdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_0300
 * @tc.name: testV1AddSimMessage003
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, smscPdu is "!&#"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_0400
 * @tc.name: testV1AddSimMessage004
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, pdu is "!&#"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "!&#";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_0500
 * @tc.name: testV1AddSimMessage005
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, -1, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_0600
 * @tc.name: testV1AddSimMessage006
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, serialId is 0
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 0, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 0, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_0700
 * @tc.name: testV1AddSimMessage007
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serialId is 1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 1, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_0800
 * @tc.name: testV1AddSimMessage008
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serialId is 2
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 2, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 2, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_0900
 * @tc.name: testV1AddSimMessage009
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serialId is 3
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 3, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 3, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1000
 * @tc.name: testV1AddSimMessage010
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serialId is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, -2147483648, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, -2147483648, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1100
 * @tc.name: testV1AddSimMessage011
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 2147483647, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 2147483647, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1200
 * @tc.name: testV1AddSimMessage012
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, serial is -1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.serial = -1;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1300
 * @tc.name: testV1AddSimMessage013
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, serial is 0
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.serial = 0;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1400
 * @tc.name: testV1AddSimMessage014
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serial is 1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.serial = 1;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1500
 * @tc.name: testV1AddSimMessage015
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serial is 2
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.serial = 2;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1600
 * @tc.name: testV1AddSimMessage016
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serial is 3
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.serial = 3;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1700
 * @tc.name: testV1AddSimMessage017
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serial is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.serial = -2147483648;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1800
 * @tc.name: testV1AddSimMessage018
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, serial is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.serial = 2147483647;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_1900
 * @tc.name: testV1AddSimMessage019
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, smscPdu is "abc"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "abc";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2000
 * @tc.name: testV1AddSimMessage020
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, smscPdu is "+-*%"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "+-*%";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2100
 * @tc.name: testV1AddSimMessage021
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, pdu is "abc"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "abc";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2200
 * @tc.name: testV1AddSimMessage022
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, pdu is "+-*%"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "+-*%";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2300
 * @tc.name: testV1AddSimMessage023
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, state is -1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = -1;
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2400
 * @tc.name: testV1AddSimMessage024
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, state is 0
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNREAD);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2500
 * @tc.name: testV1AddSimMessage025
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, state is 1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage025, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_READ);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2600
 * @tc.name: testV1AddSimMessage026
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, state is 2
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage026, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2700
 * @tc.name: testV1AddSimMessage027
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, state is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage027, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = -2147483648;
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2800
 * @tc.name: testV1AddSimMessage028
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, state is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage028, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = 2147483647;
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_2900
 * @tc.name: testV1AddSimMessage029
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, index is -1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage029, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = -1;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_3000
 * @tc.name: testV1AddSimMessage030
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, index is 0
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage030, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_3100
 * @tc.name: testV1AddSimMessage031
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, index is 1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage031, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 1;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_3200
 * @tc.name: testV1AddSimMessage032
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, index is 2
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage032, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 2;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_3300
 * @tc.name: testV1AddSimMessage033
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, index is 3
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage033, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 3;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_3400
 * @tc.name: testV1AddSimMessage034
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, index is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage034, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = -2147483648;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_3500
 * @tc.name: testV1AddSimMessage035
 * @tc.desc: Call function V1 AddSimMessage, slotId is 1, index is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage035, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 2147483647;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddSimMessage_3600
 * @tc.name: testV1AddSimMessage036
 * @tc.desc: Call function V1 AddSimMessage, slotId is 0, smscPdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddSimMessage036, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 100;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelSimMessage_0100
 * @tc.name: testV1DelSimMessage001
 * @tc.desc: Call function V1 DelSimMessage, AddSimMessage.slotId is 0, DelSimMessage.serialId is 1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1DelSimMessage001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_2, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);

    int32_t ret2 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret2);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelSimMessage_0200
 * @tc.name: testV1DelSimMessage002
 * @tc.desc: Call function V1 DelSimMessage, AddSimMessage.serialId is 100, DelSimMessage.serialId is 10
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1DelSimMessage002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 10, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);

    int32_t ret2 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret2);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelSimMessage_0300
 * @tc.name: testV1DelSimMessage003
 * @tc.desc: Call function V1 DelSimMessage, AddSimMessage.index is 0, DelSimMessage.index is 10
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1DelSimMessage003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, 100, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);

    int32_t ret1 = g_rilInterface->DelSimMessage(SLOTID_1, 10, 10);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret1);

    int32_t ret2 = g_rilInterface->DelSimMessage(SLOTID_1, 100, msgIoInfo.index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret2);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNrOptionMode_0100
 * @tc.name: testV1SetNrOptionMode001
 * @tc.desc: V1SetNrOptionMode, slotId is SLOTID_1, serialId is normal, mode is NR_MODE_UNKNOWN
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNrOptionMode001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetNrOptionMode(SLOTID_1, GetSerialId(), static_cast<int32_t>(NrMode::NR_MODE_UNKNOWN));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNrOptionMode_0200
 * @tc.name: testV1SetNrOptionMode002
 * @tc.desc: V1SetNrOptionMode, slotId is SLOTID_1, serialId is normal, mode is NR_MODE_NSA_ONLY
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNrOptionMode002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetNrOptionMode(SLOTID_1, GetSerialId(), static_cast<int32_t>(NrMode::NR_MODE_NSA_ONLY));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNrOptionMode_0300
 * @tc.name: testV1SetNrOptionMode003
 * @tc.desc: V1SetNrOptionMode, slotId is SLOTID_1, serialId is normal, mode is NR_MODE_SA_ONLY
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNrOptionMode003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetNrOptionMode(SLOTID_1, GetSerialId(), static_cast<int32_t>(NrMode::NR_MODE_SA_ONLY));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNrOptionMode_0400
 * @tc.name: testV1SetNrOptionMode004
 * @tc.desc: V1SetNrOptionMode, slotId is SLOTID_2, serialId is normal, mode is NR_MODE_UNKNOWN
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNrOptionMode004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetNrOptionMode(SLOTID_2, GetSerialId(), static_cast<int32_t>(NrMode::NR_MODE_UNKNOWN));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNrOptionMode_0500
 * @tc.name: testV1SetNrOptionMode005
 * @tc.desc: V1SetNrOptionMode, slotId is SLOTID_2, serialId is normal, mode is NR_MODE_NSA_ONLY
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNrOptionMode005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetNrOptionMode(SLOTID_2, GetSerialId(), static_cast<int32_t>(NrMode::NR_MODE_NSA_ONLY));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNrOptionMode_0600
 * @tc.name: testV1SetNrOptionMode006
 * @tc.desc: V1SetNrOptionMode, slotId is SLOTID_2, serialId is normal, mode is NR_MODE_SA_ONLY
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNrOptionMode006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret =
        g_rilInterface->SetNrOptionMode(SLOTID_2, GetSerialId(), static_cast<int32_t>(NrMode::NR_MODE_SA_ONLY));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNrOptionMode_0700
 * @tc.name: testV1SetNrOptionMode007
 * @tc.desc: V1SetNrOptionMode, slotId is SLOTID_1, serialId is -1, mode is NR_MODE_SA_ONLY
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNrOptionMode007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetNrOptionMode(SLOTID_1, -1, static_cast<int32_t>(NrMode::NR_MODE_SA_ONLY));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNrOptionMode_0800
 * @tc.name: testV1SetNrOptionMode008
 * @tc.desc: V1SetNrOptionMode, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1SetNrOptionMode008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetNrOptionMode(SLOTID_1, GetSerialId(), static_cast<int32_t>(NrMode::NR_MODE_NSA_ONLY));
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNrOptionMode_0100
 * @tc.name: testV1GetNrOptionMode001
 * @tc.desc: V1GetNrOptionMode, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1GetNrOptionMode001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetNrOptionMode(SLOTID_2, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_0300
 * @tc.name: testV1UpdateSimMessage001
 * @tc.desc: UpdateSimMessage, smscPdu is TEST_CDMA_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_0400
 * @tc.name: testV1UpdateSimMessage002
 * @tc.desc: UpdateSimMessage, smscPdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_0500
 * @tc.name: testV1UpdateSimMessage003
 * @tc.desc: UpdateSimMessage, smscPdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "@";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_0600
 * @tc.name: testV1UpdateSimMessage004
 * @tc.desc: UpdateSimMessage, pdu is TEST_SEND_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_SEND_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_0700
 * @tc.name: testV1UpdateSimMessage005
 * @tc.desc: UpdateSimMessage, pdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_0800
 * @tc.name: testV1UpdateSimMessage006
 * @tc.desc: UpdateSimMessage, pdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "@";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_0900
 * @tc.name: testV1UpdateSimMessage007
 * @tc.desc: UpdateSimMessage, state is SIM_MESSAGE_STATUS_UNREAD
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNREAD);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1000
 * @tc.name: testV1UpdateSimMessage008
 * @tc.desc: UpdateSimMessage, state is SIM_MESSAGE_STATUS_READ
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_READ);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1100
 * @tc.name: testV1UpdateSimMessage009
 * @tc.desc: UpdateSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1200
 * @tc.name: testV1UpdateSimMessage010
 * @tc.desc: UpdateSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 255;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1300
 * @tc.name: testV1UpdateSimMessage011
 * @tc.desc: UpdateSimMessage, smscPdu is TEST_CDMA_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1400
 * @tc.name: testV1UpdateSimMessage012
 * @tc.desc: UpdateSimMessage, smscPdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1500
 * @tc.name: testV1UpdateSimMessage013
 * @tc.desc: UpdateSimMessage, smscPdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "@";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1600
 * @tc.name: testV1UpdateSimMessage014
 * @tc.desc: UpdateSimMessage, pdu is TEST_SEND_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_SEND_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1700
 * @tc.name: testV1UpdateSimMessage015
 * @tc.desc: UpdateSimMessage, pdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1800
 * @tc.name: testV1UpdateSimMessage016
 * @tc.desc: UpdateSimMessage, pdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "@";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_1900
 * @tc.name: testV1UpdateSimMessage017
 * @tc.desc: UpdateSimMessage, state is SIM_MESSAGE_STATUS_UNREAD
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNREAD);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_2000
 * @tc.name: testV1UpdateSimMessage018
 * @tc.desc: UpdateSimMessage, state is SIM_MESSAGE_STATUS_READ
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_READ);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_2100
 * @tc.name: testV1UpdateSimMessage019
 * @tc.desc: UpdateSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_2200
 * @tc.name: testV1UpdateSimMessage020
 * @tc.desc: UpdateSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 255;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_2300
 * @tc.name: testV1UpdateSimMessage021
 * @tc.desc: UpdateSimMessage, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage021, Function | MediumTest | Level1)
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
        ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_2400
 * @tc.name: testV1UpdateSimMessage022
 * @tc.desc: UpdateSimMessage, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateSimMessage_2500
 * @tc.name: testV1UpdateSimMessage023
 * @tc.desc: UpdateSimMessage, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1UpdateSimMessage023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_0300
 * @tc.name: testV1AddCdmaSimMessage001
 * @tc.desc: AddCdmaSimMessage, smscPdu is TEST_CDMA_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_0400
 * @tc.name: testV1AddCdmaSimMessage002
 * @tc.desc: AddCdmaSimMessage, smscPdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_0500
 * @tc.name: testV1AddCdmaSimMessage003
 * @tc.desc: AddCdmaSimMessage, smscPdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "@";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_0600
 * @tc.name: testV1AddCdmaSimMessage004
 * @tc.desc: AddCdmaSimMessage, pdu is TEST_SEND_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_SEND_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_0700
 * @tc.name: testV1AddCdmaSimMessage005
 * @tc.desc: AddCdmaSimMessage, pdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_0800
 * @tc.name: testV1AddCdmaSimMessage006
 * @tc.desc: AddCdmaSimMessage, pdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "@";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_0900
 * @tc.name: testV1AddCdmaSimMessage007
 * @tc.desc: AddCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNREAD
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNREAD);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1000
 * @tc.name: testV1AddCdmaSimMessage008
 * @tc.desc: AddCdmaSimMessage, state is SIM_MESSAGE_STATUS_READ
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_READ);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1100
 * @tc.name: testV1AddCdmaSimMessage009
 * @tc.desc: AddCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1200
 * @tc.name: testV1AddCdmaSimMessage010
 * @tc.desc: AddCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 255;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1300
 * @tc.name: testV1AddCdmaSimMessage011
 * @tc.desc: AddCdmaSimMessage, smscPdu is TEST_CDMA_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1400
 * @tc.name: testV1AddCdmaSimMessage012
 * @tc.desc: AddCdmaSimMessage, smscPdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1500
 * @tc.name: testV1AddCdmaSimMessage013
 * @tc.desc: AddCdmaSimMessage, smscPdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = "@";
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1600
 * @tc.name: testV1AddCdmaSimMessage014
 * @tc.desc: AddCdmaSimMessage, pdu is TEST_SEND_PDU
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_SEND_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1700
 * @tc.name: testV1AddCdmaSimMessage015
 * @tc.desc: AddCdmaSimMessage, pdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1800
 * @tc.name: testV1AddCdmaSimMessage016
 * @tc.desc: AddCdmaSimMessage, pdu is "@"
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = "@";
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_1900
 * @tc.name: testV1AddCdmaSimMessage017
 * @tc.desc: AddCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNREAD
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNREAD);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_2000
 * @tc.name: testV1AddCdmaSimMessage018
 * @tc.desc: AddCdmaSimMessage, state is SIM_MESSAGE_STATUS_READ
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_READ);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_2100
 * @tc.name: testV1AddCdmaSimMessage019
 * @tc.desc: AddCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_2200
 * @tc.name: testV1AddCdmaSimMessage020
 * @tc.desc: AddCdmaSimMessage, state is SIM_MESSAGE_STATUS_UNSENT
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_UNSENT);
    msgIoInfo.index = 255;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_2300
 * @tc.name: testV1AddCdmaSimMessage021
 * @tc.desc: AddCdmaSimMessage, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage021, Function | MediumTest | Level1)
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
        ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_2400
 * @tc.name: testV1AddCdmaSimMessage022
 * @tc.desc: AddCdmaSimMessage, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AddCdmaSimMessage_2500
 * @tc.name: testV1AddCdmaSimMessage023
 * @tc.desc: AddCdmaSimMessage, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1AddCdmaSimMessage023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessage_0300
 * @tc.name: testV1DelCdmaSimMessage001
 * @tc.desc: DelCdmaSimMessage, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1DelCdmaSimMessage001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_1, -1, index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DelCdmaSimMessage_0400
 * @tc.name: testV1DelCdmaSimMessage002
 * @tc.desc: DelCdmaSimMessage, index is 255
 */
HWTEST_F(HdfRilHdiTestAdditional4th, testV1DelCdmaSimMessage002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t index = 255;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_1, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}