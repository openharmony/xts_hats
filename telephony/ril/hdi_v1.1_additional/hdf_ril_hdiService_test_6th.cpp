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
#include "v1_2/iril.h"
#include "gtest/gtest.h"
#include "hdf_ril_callback_common.h"

using namespace OHOS::HDI::Ril::V1_2;
using namespace testing::ext;

class HdfRilHdiTestAdditional6th : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
/**
** HdfRilHdiTestAdditional6th implement
**/
void HdfRilHdiTestAdditional6th::SetUpTestCase()
{
    g_rilInterface = OHOS::HDI::Ril::V1_2::IRil::Get();
    if (g_rilInterface != nullptr) {
        g_rilInterface->SetCallback1_2(&g_callback);
        g_rilInterface->SendRilAck();
        g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
        g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    }
}

void HdfRilHdiTestAdditional6th::TearDownTestCase() {}
void HdfRilHdiTestAdditional6th::SetUp() {}
void HdfRilHdiTestAdditional6th::TearDown() {}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfoResponse_0400
 * @tc.name  : testV1GetOperatorInfoResponse004
 * @tc.desc  : Call function GetOperatorInfoResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetOperatorInfoResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getOperatorInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getOperatorInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSearchInformationResponse_0100
 * @tc.name  : testV1GetNetworkSearchInformationResponse001
 * @tc.desc  : Call function GetNetworkSearchInformationResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNetworkSearchInformationResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getNetworkSearchInformationResponseFlag = false;
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getNetworkSearchInformationResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSearchInformationResponse_0200
 * @tc.name  : testV1GetNetworkSearchInformationResponse002
 * @tc.desc  : Call function GetNetworkSearchInformationResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNetworkSearchInformationResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getNetworkSearchInformationResponseFlag = false;
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getNetworkSearchInformationResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSearchInformationResponse_0300
 * @tc.name  : testV1GetNetworkSearchInformationResponse003
 * @tc.desc  : Call function GetNetworkSearchInformationResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNetworkSearchInformationResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getNetworkSearchInformationResponseFlag = false;
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getNetworkSearchInformationResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSearchInformationResponse_0400
 * @tc.name  : testV1GetNetworkSearchInformationResponse004
 * @tc.desc  : Call function GetNetworkSearchInformationResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNetworkSearchInformationResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getNetworkSearchInformationResponseFlag = false;
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getNetworkSearchInformationResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSelectionModeResponse_0100
 * @tc.name  : testV1GetNetworkSelectionModeResponse001
 * @tc.desc  : Call function GetNetworkSelectionModeResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNetworkSelectionModeResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getNetworkSelectionModeResponseFlag = false;
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getNetworkSelectionModeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSelectionModeResponse_0200
 * @tc.name  : testV1GetNetworkSelectionModeResponse002
 * @tc.desc  : Call function GetNetworkSelectionModeResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNetworkSelectionModeResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getNetworkSelectionModeResponseFlag = false;
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getNetworkSelectionModeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSelectionModeResponse_0300
 * @tc.name  : testV1GetNetworkSelectionModeResponse003
 * @tc.desc  : Call function GetNetworkSelectionModeResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNetworkSelectionModeResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getNetworkSelectionModeResponseFlag = false;
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getNetworkSelectionModeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNetworkSelectionModeResponse_0400
 * @tc.name  : testV1GetNetworkSelectionModeResponse004
 * @tc.desc  : Call function GetNetworkSelectionModeResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNetworkSelectionModeResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getNetworkSelectionModeResponseFlag = false;
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getNetworkSelectionModeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNeighboringCellInfoListResponse_0100
 * @tc.name  : testV1GetNeighboringCellInfoListResponse001
 * @tc.desc  : Call function GetNeighboringCellInfoListResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNeighboringCellInfoListResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getNeighboringCellInfoListResponseFlag = false;
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getNeighboringCellInfoListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNeighboringCellInfoListResponse_0200
 * @tc.name  : testV1GetNeighboringCellInfoListResponse002
 * @tc.desc  : Call function GetNeighboringCellInfoListResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNeighboringCellInfoListResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getNeighboringCellInfoListResponseFlag = false;
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getNeighboringCellInfoListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNeighboringCellInfoListResponse_0300
 * @tc.name  : testV1GetNeighboringCellInfoListResponse003
 * @tc.desc  : Call function GetNeighboringCellInfoListResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNeighboringCellInfoListResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getNeighboringCellInfoListResponseFlag = false;
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getNeighboringCellInfoListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetNeighboringCellInfoListResponse_0400
 * @tc.name  : testV1GetNeighboringCellInfoListResponse004
 * @tc.desc  : Call function GetNeighboringCellInfoListResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetNeighboringCellInfoListResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getNeighboringCellInfoListResponseFlag = false;
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getNeighboringCellInfoListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfoResponse_0100
 * @tc.name  : testV1GetCurrentCellInfoResponse001
 * @tc.desc  : Call function GetCurrentCellInfoResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCurrentCellInfoResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCurrentCellInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCurrentCellInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfoResponse_0200
 * @tc.name  : testV1GetCurrentCellInfoResponse002
 * @tc.desc  : Call function GetCurrentCellInfoResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCurrentCellInfoResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCurrentCellInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCurrentCellInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfoResponse_0300
 * @tc.name  : testV1GetCurrentCellInfoResponse003
 * @tc.desc  : Call function GetCurrentCellInfoResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCurrentCellInfoResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCurrentCellInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getCurrentCellInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfoResponse_0400
 * @tc.name  : testV1GetCurrentCellInfoResponse004
 * @tc.desc  : Call function GetCurrentCellInfoResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCurrentCellInfoResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCurrentCellInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getCurrentCellInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfoResponse_1_1_0100
 * @tc.name  : testV1GetCurrentCellInfoResponse_1_1001
 * @tc.desc  : Call function GetCurrentCellInfoResponse_1_1 slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCurrentCellInfoResponse_1_1001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCurrentCellInfoResponse11Flag = false;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCurrentCellInfoResponse11Flag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfoResponse_1_1_0200
 * @tc.name  : testV1GetCurrentCellInfoResponse_1_1002
 * @tc.desc  : Call function GetCurrentCellInfoResponse_1_1 slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCurrentCellInfoResponse_1_1002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCurrentCellInfoResponse11Flag = false;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCurrentCellInfoResponse11Flag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfoResponse_1_1_0300
 * @tc.name  : testV1GetCurrentCellInfoResponse_1_1003
 * @tc.desc  : Call function GetCurrentCellInfoResponse_1_1 slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCurrentCellInfoResponse_1_1003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCurrentCellInfoResponse11Flag = false;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getCurrentCellInfoResponse11Flag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCurrentCellInfoResponse_1_1_0400
 * @tc.name  : testV1GetCurrentCellInfoResponse_1_1004
 * @tc.desc  : Call function GetCurrentCellInfoResponse_1_1 slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCurrentCellInfoResponse_1_1004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCurrentCellInfoResponse11Flag = false;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getCurrentCellInfoResponse11Flag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionModeResponse_0100
 * @tc.name  : testV1SetNetworkSelectionModeResponse001
 * @tc.desc  : Call function SetNetworkSelectionModeResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetNetworkSelectionModeResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setNetworkSelectionModeResponseFlag = false;
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setNetworkSelectionModeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionModeResponse_0200
 * @tc.name  : testV1SetNetworkSelectionModeResponse002
 * @tc.desc  : Call function SetNetworkSelectionModeResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetNetworkSelectionModeResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setNetworkSelectionModeResponseFlag = false;
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setNetworkSelectionModeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionModeResponse_0300
 * @tc.name  : testV1SetNetworkSelectionModeResponse003
 * @tc.desc  : Call function SetNetworkSelectionModeResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetNetworkSelectionModeResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setNetworkSelectionModeResponseFlag = false;
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, -1, setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_setNetworkSelectionModeResponseFlag);
}
/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetNetworkSelectionModeResponse_0400
 * @tc.name  : testV1SetNetworkSelectionModeResponse004
 * @tc.desc  : Call function SetNetworkSelectionModeResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetNetworkSelectionModeResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setNetworkSelectionModeResponseFlag = false;
    SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, -1, setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_setNetworkSelectionModeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1HangupResponse_0100
 * @tc.name  : testV1HangupResponse001
 * @tc.desc  : Call function HangupResponse slotId is SLOTID_1 callId is 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1HangupResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_hangupResponseFlag = false;
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_hangupResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1HangupResponse_0200
 * @tc.name  : testV1HangupResponse002
 * @tc.desc  : Call function HangupResponse slotId is SLOTID_2 callId is 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1HangupResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_hangupResponseFlag = false;
    int32_t ret = g_rilInterface->Hangup(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_hangupResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1HangupResponse_0300
 * @tc.name  : testV1HangupResponse003
 * @tc.desc  : Call function HangupResponse slotId is SLOTID_1 callId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1HangupResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_hangupResponseFlag = false;
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_hangupResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1HangupResponse_0400
 * @tc.name  : testV1HangupResponse004
 * @tc.desc  : Call function HangupResponse slotId is SLOTID_2 callId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1HangupResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_hangupResponseFlag = false;
    int32_t ret = g_rilInterface->Hangup(SLOTID_2, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_hangupResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1RejectResponse_0100
 * @tc.name  : testV1RejectResponse001
 * @tc.desc  : Call function RejectResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1RejectResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_rejectResponseFlag = false;
    int32_t ret = g_rilInterface->Reject(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_rejectResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1RejectResponse_0200
 * @tc.name  : testV1RejectResponse002
 * @tc.desc  : Call function RejectResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1RejectResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_rejectResponseFlag = false;
    int32_t ret = g_rilInterface->Reject(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_rejectResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1RejectResponse_0300
 * @tc.name  : testV1RejectResponse003
 * @tc.desc  : Call function RejectResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1RejectResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_rejectResponseFlag = false;
    int32_t ret = g_rilInterface->Reject(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_rejectResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1RejectResponse_0400
 * @tc.name  : testV1RejectResponse004
 * @tc.desc  : Call function RejectResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1RejectResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_rejectResponseFlag = false;
    int32_t ret = g_rilInterface->Reject(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_rejectResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AnswerResponse_0100
 * @tc.name  : testV1AnswerResponse001
 * @tc.desc  : Call function AnswerResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1AnswerResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_answerResponseFlag = false;
    int32_t ret = g_rilInterface->Answer(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_answerResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AnswerResponse_0200
 * @tc.name  : testV1AnswerResponse002
 * @tc.desc  : Call function AnswerResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1AnswerResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_answerResponseFlag = false;
    int32_t ret = g_rilInterface->Answer(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_answerResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AnswerResponse_0300
 * @tc.name  : testV1AnswerResponse003
 * @tc.desc  : Call function AnswerResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1AnswerResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_answerResponseFlag = false;
    int32_t ret = g_rilInterface->Answer(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_answerResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1AnswerResponse_0400
 * @tc.name  : testV1AnswerResponse004
 * @tc.desc  : Call function AnswerResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1AnswerResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_answerResponseFlag = false;
    int32_t ret = g_rilInterface->Answer(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_answerResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1HoldCallResponse_0100
 * @tc.name  : testV1HoldCallResponse001
 * @tc.desc  : Call function HoldCallResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1HoldCallResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_holdCallResponseFlag = false;
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_holdCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1HoldCallResponse_0200
 * @tc.name  : testV1HoldCallResponse002
 * @tc.desc  : Call function HoldCallResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1HoldCallResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_holdCallResponseFlag = false;
    int32_t ret = g_rilInterface->HoldCall(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_holdCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1HoldCallResponse_0300
 * @tc.name  : testV1HoldCallResponse003
 * @tc.desc  : Call function HoldCallResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1HoldCallResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_holdCallResponseFlag = false;
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_holdCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1HoldCallResponse_0400
 * @tc.name  : testV1HoldCallResponse004
 * @tc.desc  : Call function HoldCallResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1HoldCallResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_holdCallResponseFlag = false;
    int32_t ret = g_rilInterface->HoldCall(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_holdCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnHoldCallResponse_0100
 * @tc.name  : testV1UnHoldCallResponse001
 * @tc.desc  : Call function UnHoldCallResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UnHoldCallResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_unHoldCallResponseFlag = false;
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_unHoldCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnHoldCallResponse_0200
 * @tc.name  : testV1UnHoldCallResponse002
 * @tc.desc  : Call function UnHoldCallResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UnHoldCallResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_unHoldCallResponseFlag = false;
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_unHoldCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnHoldCallResponse_0300
 * @tc.name  : testV1UnHoldCallResponse003
 * @tc.desc  : Call function UnHoldCallResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UnHoldCallResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_unHoldCallResponseFlag = false;
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_unHoldCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnHoldCallResponse_0400
 * @tc.name  : testV1UnHoldCallResponse004
 * @tc.desc  : Call function UnHoldCallResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UnHoldCallResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_unHoldCallResponseFlag = false;
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_unHoldCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SwitchCallResponse_0100
 * @tc.name  : testV1SwitchCallResponse001
 * @tc.desc  : Call function SwitchCallResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SwitchCallResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_switchCallResponseFlag = false;
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_switchCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SwitchCallResponse_0200
 * @tc.name  : testV1SwitchCallResponse002
 * @tc.desc  : Call function SwitchCallResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SwitchCallResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_switchCallResponseFlag = false;
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_switchCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SwitchCallResponse_0300
 * @tc.name  : testV1SwitchCallResponse003
 * @tc.desc  : Call function SwitchCallResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SwitchCallResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_switchCallResponseFlag = false;
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_switchCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SwitchCallResponse_0400
 * @tc.name  : testV1SwitchCallResponse004
 * @tc.desc  : Call function SwitchCallResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SwitchCallResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_switchCallResponseFlag = false;
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_switchCallResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetClipResponse_0100
 * @tc.name  : testV1GetClipResponse001
 * @tc.desc  : Call function GetClipResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetClipResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getClipResponseFlag = false;
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getClipResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetClipResponse_0200
 * @tc.name  : testV1GetClipResponse002
 * @tc.desc  : Call function GetClipResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetClipResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getClipResponseFlag = false;
    int32_t ret = g_rilInterface->GetClip(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getClipResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetClipResponse_0300
 * @tc.name  : testV1GetClipResponse003
 * @tc.desc  : Call function GetClipResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetClipResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getClipResponseFlag = false;
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getClipResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetClipResponse_0400
 * @tc.name  : testV1GetClipResponse004
 * @tc.desc  : Call function GetClipResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetClipResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getClipResponseFlag = false;
    int32_t ret = g_rilInterface->GetClip(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getClipResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetClipResponse_0100
 * @tc.name  : testV1SetClipResponse001
 * @tc.desc  : Call function SetClipResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetClipResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setClipResponseFlag = false;
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setClipResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetClipResponse_0200
 * @tc.name  : testV1SetClipResponse002
 * @tc.desc  : Call function SetClipResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetClipResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setClipResponseFlag = false;
    int32_t ret = g_rilInterface->SetClip(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setClipResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetClipResponse_0300
 * @tc.name  : testV1SetClipResponse003
 * @tc.desc  : Call function SetClipResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetClipResponse003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setClipResponseFlag = false;
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_setClipResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetClipResponse_0400
 * @tc.name  : testV1SetClipResponse004
 * @tc.desc  : Call function SetClipResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetClipResponse004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setClipResponseFlag = false;
    int32_t ret = g_rilInterface->SetClip(SLOTID_2, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_setClipResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponseResponse_0100
 * @tc.name  : testV1SimStkSendTerminalResponseResponse001
 * @tc.desc  : Call function SimStkSendTerminalResponseResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendTerminalResponseResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simStkSendTerminalResponseResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, GetSerialId(), TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendTerminalResponseResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponseResponse_0200
 * @tc.name  : testV1SimStkSendTerminalResponseResponse002
 * @tc.desc  : Call function SimStkSendTerminalResponseResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendTerminalResponseResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simStkSendTerminalResponseResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_2, GetSerialId(), TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendTerminalResponseResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponseResponse_0300
 * @tc.name  : testV1SimStkSendTerminalResponseResponse003
 * @tc.desc  : Call function SimStkSendTerminalResponseResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendTerminalResponseResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simStkSendTerminalResponseResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, -1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendTerminalResponseResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponseResponse_0400
 * @tc.name  : testV1SimStkSendTerminalResponseResponse004
 * @tc.desc  : Call function SimStkSendTerminalResponseResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendTerminalResponseResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simStkSendTerminalResponseResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_2, -1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendTerminalResponseResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReadyResponse_0100
 * @tc.name  : testV1SimStkIsReadyResponse001
 * @tc.desc  : Call function SimStkIsReadyResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkIsReadyResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simStkIsReadyResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkIsReadyResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReadyResponse_0200
 * @tc.name  : testV1SimStkIsReadyResponse002
 * @tc.desc  : Call function SimStkIsReadyResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkIsReadyResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simStkIsReadyResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkIsReadyResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReadyResponse_0300
 * @tc.name  : testV1SimStkIsReadyResponse003
 * @tc.desc  : Call function SimStkIsReadyResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkIsReadyResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simStkIsReadyResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkIsReadyResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReadyResponse_0400
 * @tc.name  : testV1SimStkIsReadyResponse004
 * @tc.desc  : Call function SimStkIsReadyResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkIsReadyResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simStkIsReadyResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkIsReadyResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocolResponse_0100
 * @tc.name  : testV1SetRadioProtocolResponse001
 * @tc.desc  : Call function SetRadioProtocolResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetRadioProtocolResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setRadioProtocolResponseFlag = false;
    RadioProtocol protocol;
    protocol.slotId = SLOTID_1;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_1, GetSerialId(), protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setRadioProtocolResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocolResponse_0200
 * @tc.name  : testV1SetRadioProtocolResponse002
 * @tc.desc  : Call function SetRadioProtocolResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetRadioProtocolResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setRadioProtocolResponseFlag = false;
    RadioProtocol protocol;
    protocol.slotId = SLOTID_2;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_2, GetSerialId(), protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setRadioProtocolResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocolResponse_0300
 * @tc.name  : testV1SetRadioProtocolResponse003
 * @tc.desc  : Call function SetRadioProtocolResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetRadioProtocolResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setRadioProtocolResponseFlag = false;
    RadioProtocol protocol;
    protocol.slotId = SLOTID_1;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_1, -1, protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_setRadioProtocolResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocolResponse_0400
 * @tc.name  : testV1SetRadioProtocolResponse004
 * @tc.desc  : Call function SetRadioProtocolResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetRadioProtocolResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setRadioProtocolResponseFlag = false;
    RadioProtocol protocol;
    protocol.slotId = SLOTID_2;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_2, -1, protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_setRadioProtocolResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannelResponse_0100
 * @tc.name  : testV1SimOpenLogicalChannelResponse001
 * @tc.desc  : Call function SimOpenLogicalChannelResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimOpenLogicalChannelResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simOpenLogicalChannelResponseFlag = false;
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_1, GetSerialId(), appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simOpenLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannelResponse_0200
 * @tc.name  : testV1SimOpenLogicalChannelResponse002
 * @tc.desc  : Call function SimOpenLogicalChannelResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimOpenLogicalChannelResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simOpenLogicalChannelResponseFlag = false;
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_2, GetSerialId(), appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simOpenLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannelResponse_0300
 * @tc.name  : testV1SimOpenLogicalChannelResponse003
 * @tc.desc  : Call function SimOpenLogicalChannelResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimOpenLogicalChannelResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simOpenLogicalChannelResponseFlag = false;
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_1, -1, appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simOpenLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannelResponse_0400
 * @tc.name  : testV1SimOpenLogicalChannelResponse004
 * @tc.desc  : Call function SimOpenLogicalChannelResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimOpenLogicalChannelResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simOpenLogicalChannelResponseFlag = false;
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_2, -1, appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simOpenLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannelResponse_0100
 * @tc.name  : testV1SimCloseLogicalChannelResponse001
 * @tc.desc  : Call function SimCloseLogicalChannelResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimCloseLogicalChannelResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simCloseLogicalChannelResponseFlag = false;
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_1, GetSerialId(), g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simCloseLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannelResponse_0200
 * @tc.name  : testV1SimCloseLogicalChannelResponse002
 * @tc.desc  : Call function SimCloseLogicalChannelResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimCloseLogicalChannelResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simCloseLogicalChannelResponseFlag = false;
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_2, GetSerialId(), g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simCloseLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannelResponse_0300
 * @tc.name  : testV1SimCloseLogicalChannelResponse003
 * @tc.desc  : Call function SimCloseLogicalChannelResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimCloseLogicalChannelResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simCloseLogicalChannelResponseFlag = false;
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_1, -1, g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simCloseLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannelResponse_0400
 * @tc.name  : testV1SimCloseLogicalChannelResponse004
 * @tc.desc  : Call function SimCloseLogicalChannelResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimCloseLogicalChannelResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simCloseLogicalChannelResponseFlag = false;
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_2, -1, g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simCloseLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannelResponse_0100
 * @tc.name  : testV1SimTransmitApduLogicalChannelResponse001
 * @tc.desc  : Call function SimTransmitApduLogicalChannelResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimTransmitApduLogicalChannelResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simTransmitApduLogicalChannelResponseFlag = false;
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simTransmitApduLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannelResponse_0200
 * @tc.name  : testV1SimTransmitApduLogicalChannelResponse002
 * @tc.desc  : Call function SimTransmitApduLogicalChannelResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimTransmitApduLogicalChannelResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simTransmitApduLogicalChannelResponseFlag = false;
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_2, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simTransmitApduLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannelResponse_0300
 * @tc.name  : testV1SimTransmitApduLogicalChannelResponse003
 * @tc.desc  : Call function SimTransmitApduLogicalChannelResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimTransmitApduLogicalChannelResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simTransmitApduLogicalChannelResponseFlag = false;
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, -1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simTransmitApduLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannelResponse_0400
 * @tc.name  : testV1SimTransmitApduLogicalChannelResponse004
 * @tc.desc  : Call function SimTransmitApduLogicalChannelResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimTransmitApduLogicalChannelResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simTransmitApduLogicalChannelResponseFlag = false;
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_2, -1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simTransmitApduLogicalChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannelResponse_0100
 * @tc.name  : testV1SimTransmitApduBasicChannelResponse001
 * @tc.desc  : Call function SimTransmitApduBasicChannelResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimTransmitApduBasicChannelResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simTransmitApduBasicChannelResponseFlag = false;
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simTransmitApduBasicChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannelResponse_0200
 * @tc.name  : testV1SimTransmitApduBasicChannelResponse002
 * @tc.desc  : Call function SimTransmitApduBasicChannelResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimTransmitApduBasicChannelResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simTransmitApduBasicChannelResponseFlag = false;
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_2, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simTransmitApduBasicChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannelResponse_0300
 * @tc.name  : testV1SimTransmitApduBasicChannelResponse003
 * @tc.desc  : Call function SimTransmitApduBasicChannelResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimTransmitApduBasicChannelResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simTransmitApduBasicChannelResponseFlag = false;
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, -1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simTransmitApduBasicChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannelResponse_0400
 * @tc.name  : testV1SimTransmitApduBasicChannelResponse004
 * @tc.desc  : Call function SimTransmitApduBasicChannelResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimTransmitApduBasicChannelResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simTransmitApduBasicChannelResponseFlag = false;
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_2, -1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(true, g_simTransmitApduBasicChannelResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLockResponse_0100
 * @tc.name  : testV1UnlockSimLockResponse001
 * @tc.desc  : Call function UnlockSimLockResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UnlockSimLockResponse001, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_unlockSimLockResponseFlag = false;
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, GetSerialId(), 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_unlockSimLockResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLockResponse_0200
 * @tc.name  : testV1UnlockSimLockResponse002
 * @tc.desc  : Call function UnlockSimLockResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UnlockSimLockResponse002, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_unlockSimLockResponseFlag = false;
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, GetSerialId(), 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_unlockSimLockResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLockResponse_0300
 * @tc.name  : testV1UnlockSimLockResponse003
 * @tc.desc  : Call function UnlockSimLockResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UnlockSimLockResponse003, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_unlockSimLockResponseFlag = false;
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, -1, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_unlockSimLockResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLockResponse_0400
 * @tc.name  : testV1UnlockSimLockResponse004
 * @tc.desc  : Call function UnlockSimLockResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UnlockSimLockResponse004, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_unlockSimLockResponseFlag = false;
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, -1, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_unlockSimLockResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthenticationResponse_0100
 * @tc.name  : testV1SimAuthenticationResponse001
 * @tc.desc  : Call function SimAuthenticationResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimAuthenticationResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simAuthenticationResponseFlag = false;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simAuthenticationResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthenticationResponse_0200
 * @tc.name  : testV1SimAuthenticationResponse002
 * @tc.desc  : Call function SimAuthenticationResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimAuthenticationResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simAuthenticationResponseFlag = false;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_2, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simAuthenticationResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthenticationResponse_0300
 * @tc.name  : testV1SimAuthenticationResponse003
 * @tc.desc  : Call function SimAuthenticationResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimAuthenticationResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simAuthenticationResponseFlag = false;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_1, -1, simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simAuthenticationResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthenticationResponse_0400
 * @tc.name  : testV1SimAuthenticationResponse004
 * @tc.desc  : Call function SimAuthenticationResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimAuthenticationResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simAuthenticationResponseFlag = false;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_2, -1, simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simAuthenticationResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelopeResponse_0100
 * @tc.name  : testV1SimStkSendEnvelopeResponse001
 * @tc.desc  : Call function SimStkSendEnvelopeResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendEnvelopeResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simStkSendEnvelopeResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, GetSerialId(), TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendEnvelopeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelopeResponse_0200
 * @tc.name  : testV1SimStkSendEnvelopeResponse002
 * @tc.desc  : Call function SimStkSendEnvelopeResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendEnvelopeResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simStkSendEnvelopeResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_2, GetSerialId(), TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendEnvelopeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelopeResponse_0300
 * @tc.name  : testV1SimStkSendEnvelopeResponse003
 * @tc.desc  : Call function SimStkSendEnvelopeResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendEnvelopeResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simStkSendEnvelopeResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, -1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendEnvelopeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelopeResponse_0400
 * @tc.name  : testV1SimStkSendEnvelopeResponse004
 * @tc.desc  : Call function SimStkSendEnvelopeResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendEnvelopeResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simStkSendEnvelopeResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_2, -1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendEnvelopeResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResultResponse_0100
 * @tc.name  : testV1SimStkSendCallSetupRequestResultResponse001
 * @tc.desc  : Call function SimStkSendCallSetupRequestResultResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendCallSetupRequestResultResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simStkSendCallSetupRequestResultResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_1, GetSerialId(), true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendCallSetupRequestResultResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResultResponse_0200
 * @tc.name  : testV1SimStkSendCallSetupRequestResultResponse002
 * @tc.desc  : Call function SimStkSendCallSetupRequestResultResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendCallSetupRequestResultResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simStkSendCallSetupRequestResultResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_2, GetSerialId(), true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendCallSetupRequestResultResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResultResponse_0300
 * @tc.name  : testV1SimStkSendCallSetupRequestResultResponse003
 * @tc.desc  : Call function SimStkSendCallSetupRequestResultResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendCallSetupRequestResultResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_simStkSendCallSetupRequestResultResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_1, -1, true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendCallSetupRequestResultResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResultResponse_0400
 * @tc.name  : testV1SimStkSendCallSetupRequestResultResponse004
 * @tc.desc  : Call function SimStkSendCallSetupRequestResultResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SimStkSendCallSetupRequestResultResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_simStkSendCallSetupRequestResultResponseFlag = false;
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_2, -1, true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_simStkSendCallSetupRequestResultResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioProtocolResponse_0100
 * @tc.name  : testV1GetRadioProtocolResponse001
 * @tc.desc  : Call function GetRadioProtocolResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetRadioProtocolResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getRadioProtocolResponseFlag = false;
    int32_t ret = g_rilInterface->GetRadioProtocol(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getRadioProtocolResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioProtocolResponse_0200
 * @tc.name  : testV1GetRadioProtocolResponse002
 * @tc.desc  : Call function GetRadioProtocolResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetRadioProtocolResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getRadioProtocolResponseFlag = false;
    int32_t ret = g_rilInterface->GetRadioProtocol(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getRadioProtocolResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioProtocolResponse_0300
 * @tc.name  : testV1GetRadioProtocolResponse003
 * @tc.desc  : Call function GetRadioProtocolResponse slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetRadioProtocolResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getRadioProtocolResponseFlag = false;
    int32_t ret = g_rilInterface->GetRadioProtocol(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getRadioProtocolResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioProtocolResponse_0400
 * @tc.name  : testV1GetRadioProtocolResponse004
 * @tc.desc  : Call function GetRadioProtocolResponse slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetRadioProtocolResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getRadioProtocolResponseFlag = false;
    int32_t ret = g_rilInterface->GetRadioProtocol(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getRadioProtocolResponseFlag);
}


/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetUssd_0600
 * @tc.name   : testV1SetUssd004
 * @tc.desc   : Call the function V1 SetUssd with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetUssd004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, -11111, "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetUssd_0700
 * @tc.name   : testV1SetUssd005
 * @tc.desc   : Call the function V1 SetUssd with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetUssd005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, 0, "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetUssd_0800
 * @tc.name   : testV1SetUssd006
 * @tc.desc   : Call the function V1 SetUssd with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetUssd006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, 11111, "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetUssd_0900
 * @tc.name   : testV1SetUssd007
 * @tc.desc   : Call the function V1 SetUssd with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetUssd007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, 32768, "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetUssd_1000
 * @tc.name   : testV1SetUssd008
 * @tc.desc   : Call the function V1 SetUssd with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetUssd008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, -32768, "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetUssd_1100
 * @tc.name   : testV1SetUssd009
 * @tc.desc   : Call the function V1 SetUssd with str  and a value of 1qaz!QAZ
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetUssd009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, GetSerialId(), "1qaz!QAZ");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetUssd_1200
 * @tc.name   : testV1SetUssd010
 * @tc.desc   : Call the function V1 SetUssd with str  and a value of 000000000000000
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetUssd010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, GetSerialId(), "000000000000000");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetUssd_0600
 * @tc.name   : testV1GetUssd004
 * @tc.desc   : Call the function V1 GetUssd with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetUssd004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetUssd_0700
 * @tc.name   : testV1GetUssd005
 * @tc.desc   : Call the function V1 GetUssd with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetUssd005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetUssd_0800
 * @tc.name   : testV1GetUssd006
 * @tc.desc   : Call the function V1 GetUssd with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetUssd006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetUssd_0900
 * @tc.name   : testV1GetUssd007
 * @tc.desc   : Call the function V1 GetUssd with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetUssd007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetUssd_1000
 * @tc.name   : testV1GetUssd008
 * @tc.desc   : Call the function V1 GetUssd with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetUssd008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetMute_0600
 * @tc.name   : testV1SetMute004
 * @tc.desc   : Call the function V1 SetMute with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetMute004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, -11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetMute_0700
 * @tc.name   : testV1SetMute005
 * @tc.desc   : Call the function V1 SetMute with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetMute005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetMute_0800
 * @tc.name   : testV1SetMute006
 * @tc.desc   : Call the function V1 SetMute with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetMute006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, 11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetMute_0900
 * @tc.name   : testV1SetMute007
 * @tc.desc   : Call the function V1 SetMute with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetMute007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, 32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetMute_1000
 * @tc.name   : testV1SetMute008
 * @tc.desc   : Call the function V1 SetMute with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetMute008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, -32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetMute_1100
 * @tc.name   : testV1SetMute009
 * @tc.desc   : Call the function V1 SetMute with mute  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetMute009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetMute_1200
 * @tc.name   : testV1SetMute010
 * @tc.desc   : Call the function V1 SetMute with mute  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetMute010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetMute_0600
 * @tc.name   : testV1GetMute004
 * @tc.desc   : Call the function V1 GetMute with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetMute004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetMute_0700
 * @tc.name   : testV1GetMute005
 * @tc.desc   : Call the function V1 GetMute with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetMute005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetMute_0800
 * @tc.name   : testV1GetMute006
 * @tc.desc   : Call the function V1 GetMute with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetMute006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetMute_0900
 * @tc.name   : testV1GetMute007
 * @tc.desc   : Call the function V1 GetMute with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetMute007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetMute_1000
 * @tc.name   : testV1GetMute008
 * @tc.desc   : Call the function V1 GetMute with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetMute008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallFailReason_0600
 * @tc.name   : testV1GetCallFailReason004
 * @tc.desc   : Call the function V1 GetCallFailReason with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCallFailReason004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallFailReason_0700
 * @tc.name   : testV1GetCallFailReason005
 * @tc.desc   : Call the function V1 GetCallFailReason with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCallFailReason005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallFailReason_0800
 * @tc.name   : testV1GetCallFailReason006
 * @tc.desc   : Call the function V1 GetCallFailReason with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCallFailReason006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallFailReason_0900
 * @tc.name   : testV1GetCallFailReason007
 * @tc.desc   : Call the function V1 GetCallFailReason with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCallFailReason007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallFailReason_1000
 * @tc.name   : testV1GetCallFailReason008
 * @tc.desc   : Call the function V1 GetCallFailReason with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCallFailReason008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_0600
 * @tc.name   : testV1CallSupplement004
 * @tc.desc   : Call the function V1 CallSupplement with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, -11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_0700
 * @tc.name   : testV1CallSupplement005
 * @tc.desc   : Call the function V1 CallSupplement with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_0800
 * @tc.name   : testV1CallSupplement006
 * @tc.desc   : Call the function V1 CallSupplement with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, 11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_0900
 * @tc.name   : testV1CallSupplement007
 * @tc.desc   : Call the function V1 CallSupplement with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, 32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_1000
 * @tc.name   : testV1CallSupplement008
 * @tc.desc   : Call the function V1 CallSupplement with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, -32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_1100
 * @tc.name   : testV1CallSupplement009
 * @tc.desc   : Call the function V1 CallSupplement with type  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_1200
 * @tc.name   : testV1CallSupplement010
 * @tc.desc   : Call the function V1 CallSupplement with type  and a value of 2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, GetSerialId(), 2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_1300
 * @tc.name   : testV1CallSupplement011
 * @tc.desc   : Call the function V1 CallSupplement with type  and a value of 3
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, GetSerialId(), 3);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_1400
 * @tc.name   : testV1CallSupplement012
 * @tc.desc   : Call the function V1 CallSupplement with type  and a value of 4
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, GetSerialId(), 4);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CallSupplement_1500
 * @tc.name   : testV1CallSupplement013
 * @tc.desc   : Call the function V1 CallSupplement with type  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1CallSupplement013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_0600
 * @tc.name   : testV1SendDtmf004
 * @tc.desc   : Call the function V1 SendDtmf with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, -11111, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_0700
 * @tc.name   : testV1SendDtmf005
 * @tc.desc   : Call the function V1 SendDtmf with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, 0, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_0800
 * @tc.name   : testV1SendDtmf006
 * @tc.desc   : Call the function V1 SendDtmf with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, 11111, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_0900
 * @tc.name   : testV1SendDtmf007
 * @tc.desc   : Call the function V1 SendDtmf with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, 32768, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1000
 * @tc.name   : testV1SendDtmf008
 * @tc.desc   : Call the function V1 SendDtmf with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, -32768, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1100
 * @tc.name   : testV1SendDtmf009
 * @tc.desc   : Call the function V1 SendDtmf with callId  and a value of 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 1;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1200
 * @tc.name   : testV1SendDtmf010
 * @tc.desc   : Call the function V1 SendDtmf with callId  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = -1;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1300
 * @tc.name   : testV1SendDtmf011
 * @tc.desc   : Call the function V1 SendDtmf with dtmfKey and a value of #### and with stringLength and a value of 4
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#####";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 4;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1400
 * @tc.name   : testV1SendDtmf012
 * @tc.desc   : Call the function V1 SendDtmf with dtmfKey  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "0";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1500
 * @tc.name   : testV1SendDtmf013
 * @tc.desc   : Call the function V1 SendDtmf with dtmfKey  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "-1";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1600
 * @tc.name   : testV1SendDtmf014
 * @tc.desc   : Call the function V1 SendDtmf with onLength  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 0;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1700
 * @tc.name   : testV1SendDtmf015
 * @tc.desc   : Call the function V1 SendDtmf with onLength  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = -1;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1800
 * @tc.name   : testV1SendDtmf016
 * @tc.desc   : Call the function V1 SendDtmf with offLength  and a value of 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 1;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_1900
 * @tc.name   : testV1SendDtmf017
 * @tc.desc   : Call the function V1 SendDtmf with offLength  and a value of 300
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 300;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_2000
 * @tc.name   : testV1SendDtmf018
 * @tc.desc   : Call the function V1 SendDtmf with offLength  and a value of 400
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 400;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_2100
 * @tc.name   : testV1SendDtmf019
 * @tc.desc   : Call the function V1 SendDtmf with offLength  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = -1;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_2200
 * @tc.name   : testV1SendDtmf020
 * @tc.desc   : Call the function V1 SendDtmf with dtmfKey and a value of #### and with stringLength and a value of 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "####";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmf_2300
 * @tc.name   : testV1SendDtmf021
 * @tc.desc   : Call the function V1 SendDtmf with dtmfKey and a value of # and with stringLength and a value of 4
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SendDtmf021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 4;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_0600
 * @tc.name   : testV1StartDtmf004
 * @tc.desc   : Call the function V1 StartDtmf with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, -11111, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_0700
 * @tc.name   : testV1StartDtmf005
 * @tc.desc   : Call the function V1 StartDtmf with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, 0, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_0800
 * @tc.name   : testV1StartDtmf006
 * @tc.desc   : Call the function V1 StartDtmf with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, 11111, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_0900
 * @tc.name   : testV1StartDtmf007
 * @tc.desc   : Call the function V1 StartDtmf with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, 32768, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_1000
 * @tc.name   : testV1StartDtmf008
 * @tc.desc   : Call the function V1 StartDtmf with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, -32768, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_1100
 * @tc.name   : testV1StartDtmf009
 * @tc.desc   : Call the function V1 StartDtmf with callId and a value of 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 1;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_1200
 * @tc.name   : testV1StartDtmf010
 * @tc.desc   : Call the function V1 StartDtmf with callId and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = -1;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_1300
 * @tc.name   : testV1StartDtmf011
 * @tc.desc   : Call the function V1 StartDtmf with dtmfKey and a value of #####
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#####";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_1400
 * @tc.name   : testV1StartDtmf012
 * @tc.desc   : Call the function V1 StartDtmf with dtmfKey and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "0";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmf_1500
 * @tc.name   : testV1StartDtmf013
 * @tc.desc   : Call the function V1 StartDtmf with dtmfKey and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StartDtmf013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "-1";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_0600
 * @tc.name   : testV1StopDtmf004
 * @tc.desc   : Call the function V1 StopDtmf with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, -11111, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_0700
 * @tc.name   : testV1StopDtmf005
 * @tc.desc   : Call the function V1 StopDtmf with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, 0, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_0800
 * @tc.name   : testV1StopDtmf006
 * @tc.desc   : Call the function V1 StopDtmf with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, 11111, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_0900
 * @tc.name   : testV1StopDtmf007
 * @tc.desc   : Call the function V1 StopDtmf with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, 32768, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_1000
 * @tc.name   : testV1StopDtmf008
 * @tc.desc   : Call the function V1 StopDtmf with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, -32768, dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_1100
 * @tc.name   : testV1StopDtmf009
 * @tc.desc   : Call the function V1 StopDtmf with callId and a value of 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 1;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_1200
 * @tc.name   : testV1StopDtmf010
 * @tc.desc   : Call the function V1 StopDtmf with callId and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = -1;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_1300
 * @tc.name   : testV1StopDtmf011
 * @tc.desc   : Call the function V1 StopDtmf with dtmfKey and a value of ####
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "####";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_1400
 * @tc.name   : testV1StopDtmf012
 * @tc.desc   : Call the function V1 StopDtmf with dtmfKey and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "0";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmf_1500
 * @tc.name   : testV1StopDtmf013
 * @tc.desc   : Call the function V1 StopDtmf with dtmfKey and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1StopDtmf013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "-1";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_0300
 * @tc.name: testV1SetSimLock001
 * @tc.desc: SetSimLock, classx is 1, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock001, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 1;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_0400
 * @tc.name: testV1SetSimLock002
 * @tc.desc: SetSimLock, classx is 2, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock002, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 2;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_0500
 * @tc.name: testV1SetSimLock003
 * @tc.desc: SetSimLock, classx is 4, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock003, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 4;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_0600
 * @tc.name: testV1SetSimLock004
 * @tc.desc: SetSimLock, classx is 8, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock004, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 8;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_0700
 * @tc.name: testV1SetSimLock005
 * @tc.desc: SetSimLock, classx is 16, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock005, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 16;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_0800
 * @tc.name: testV1SetSimLock006
 * @tc.desc: SetSimLock, classx is 32, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock006, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 32;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_0900
 * @tc.name: testV1SetSimLock007
 * @tc.desc: SetSimLock, classx is 64, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock007, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 64;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1000
 * @tc.name: testV1SetSimLock008
 * @tc.desc: SetSimLock, classx is 128, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock008, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 128;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1100
 * @tc.name: testV1SetSimLock009
 * @tc.desc: SetSimLock, classx is 255, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock009, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1200
 * @tc.name: testV1SetSimLock010
 * @tc.desc: SetSimLock, classx is 255, fac is OI, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock010, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1300
 * @tc.name: testV1SetSimLock011
 * @tc.desc: SetSimLock, classx is 255, fac is OX, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock011, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "OX";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1400
 * @tc.name: testV1SetSimLock012
 * @tc.desc: SetSimLock, classx is 255, fac is AI, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock012, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AI";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1500
 * @tc.name: testV1SetSimLock013
 * @tc.desc: SetSimLock, classx is 255, fac is IR, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock013, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1600
 * @tc.name: testV1SetSimLock014
 * @tc.desc: SetSimLock, classx is 255, fac is AB, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock014, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AB";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1700
 * @tc.name: testV1SetSimLock015
 * @tc.desc: SetSimLock, classx is 255, fac is AG, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock015, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AG";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1800
 * @tc.name: testV1SetSimLock016
 * @tc.desc: SetSimLock, classx is 255, fac is AC, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock016, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AC";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_1900
 * @tc.name: testV1SetSimLock017
 * @tc.desc: SetSimLock, classx is 255, fac is FD, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock017, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "FD";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2000
 * @tc.name: testV1SetSimLock018
 * @tc.desc: SetSimLock, classx is 255, fac is PN, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock018, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "PN";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2100
 * @tc.name: testV1SetSimLock019
 * @tc.desc: SetSimLock, classx is 255, fac is PU, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock019, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "PU";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2200
 * @tc.name: testV1SetSimLock020
 * @tc.desc: SetSimLock, classx is 255, fac is PP, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock020, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "PP";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2300
 * @tc.name: testV1SetSimLock021
 * @tc.desc: SetSimLock, classx is 255, fac is OI, mode is 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock021, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 1;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2400
 * @tc.name: testV1SetSimLock022
 * @tc.desc: SetSimLock, classx is 255, fac is OI, mode is 2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock022, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 2;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2500
 * @tc.name: testV1SetSimLock023
 * @tc.desc: SetSimLock, classx is 255, fac is IR, mode is 2, status is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock023, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 2;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";
    simLockInfo.status = 0;

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2600
 * @tc.name: testV1SetSimLock024
 * @tc.desc: SetSimLock, classx is 255, fac is IR, mode is 2, status is 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock024, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 2;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2700
 * @tc.name: testV1SetSimLock025
 * @tc.desc: SetSimLock, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock025, Function | MediumTest | Level2)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 2;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, -1, simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2800
 * @tc.name: testV1SetSimLock026
 * @tc.desc: SetSimLock, classx is 1, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock026, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 1;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_2900
 * @tc.name: testV1SetSimLock027
 * @tc.desc: SetSimLock, classx is 2, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock027, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 2;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3000
 * @tc.name: testV1SetSimLock028
 * @tc.desc: SetSimLock, classx is 4, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock028, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 4;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3100
 * @tc.name: testV1SetSimLock029
 * @tc.desc: SetSimLock, classx is 8, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock029, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 8;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3200
 * @tc.name: testV1SetSimLock030
 * @tc.desc: SetSimLock, classx is 16, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock030, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 16;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3300
 * @tc.name: testV1SetSimLock031
 * @tc.desc: SetSimLock, classx is 32, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock031, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 32;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3400
 * @tc.name: testV1SetSimLock032
 * @tc.desc: SetSimLock, classx is 64, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock032, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 64;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3500
 * @tc.name: testV1SetSimLock033
 * @tc.desc: SetSimLock, classx is 128, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock033, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 128;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3600
 * @tc.name: testV1SetSimLock034
 * @tc.desc: SetSimLock, classx is 255, fac is AO, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock034, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3700
 * @tc.name: testV1SetSimLock035
 * @tc.desc: SetSimLock, classx is 255, fac is OI, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock035, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3800
 * @tc.name: testV1SetSimLock036
 * @tc.desc: SetSimLock, classx is 255, fac is OX, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock036, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "OX";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_3900
 * @tc.name: testV1SetSimLock037
 * @tc.desc: SetSimLock, classx is 255, fac is AI, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock037, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AI";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4000
 * @tc.name: testV1SetSimLock038
 * @tc.desc: SetSimLock, classx is 255, fac is IR, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock038, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4100
 * @tc.name: testV1SetSimLock039
 * @tc.desc: SetSimLock, classx is 255, fac is AB, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock039, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AB";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4200
 * @tc.name: testV1SetSimLock040
 * @tc.desc: SetSimLock, classx is 255, fac is AG, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock040, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AG";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4300
 * @tc.name: testV1SetSimLock041
 * @tc.desc: SetSimLock, classx is 255, fac is AC, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock041, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AC";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4400
 * @tc.name: testV1SetSimLock042
 * @tc.desc: SetSimLock, classx is 255, fac is FD, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock042, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "FD";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4500
 * @tc.name: testV1SetSimLock043
 * @tc.desc: SetSimLock, classx is 255, fac is PN, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock043, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "PN";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4600
 * @tc.name: testV1SetSimLock044
 * @tc.desc: SetSimLock, classx is 255, fac is PU, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock044, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "PU";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4700
 * @tc.name: testV1SetSimLock045
 * @tc.desc: SetSimLock, classx is 255, fac is PP, mode is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock045, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "PP";
    simLockInfo.mode = 0;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4800
 * @tc.name: testV1SetSimLock046
 * @tc.desc: SetSimLock, classx is 255, fac is OI, mode is 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock046, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 1;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_4900
 * @tc.name: testV1SetSimLock047
 * @tc.desc: SetSimLock, classx is 255, fac is OI, mode is 2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock047, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 2;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_5000
 * @tc.name: testV1SetSimLock048
 * @tc.desc: SetSimLock, classx is 255, fac is IR, mode is 2, status is 0
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock048, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 2;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";
    simLockInfo.status = 0;

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_5100
 * @tc.name: testV1SetSimLock049
 * @tc.desc: SetSimLock, classx is 255, fac is IR, mode is 2, status is 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock049, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 2;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLock_5200
 * @tc.name: testV1SetSimLock050
 * @tc.desc: SetSimLock, SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSimLock050, Function | MediumTest | Level2)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimLockInfo simLockInfo;
    simLockInfo.fac = "IR";
    simLockInfo.mode = 2;
    simLockInfo.classx = 255;
    simLockInfo.passwd = "1234";
    simLockInfo.status = 1;

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, -1, simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SignalStrengthUpdated_0100
 * @tc.name  : testV1SignalStrengthUpdated001
 * @tc.desc  : Call function GetSignalStrength with serialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SignalStrengthUpdated001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_signalStrengthUpdatedFlag = false;
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_signalStrengthUpdatedFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1NetworkPhyChnlCfgUpdated_0100
 * @tc.name  : testV1NetworkPhyChnlCfgUpdated001
 * @tc.desc  : Call function GetPhysicalChannelConfig with serialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1NetworkPhyChnlCfgUpdated001, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_networkPhyChnlCfgUpdatedFlag = false;
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(false, g_networkPhyChnlCfgUpdatedFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1NetworkCurrentCellUpdated_0100
 * @tc.name: testV1NetworkCurrentCellUpdated001
 * @tc.desc: GetCurrentCellInfo, SerialId is 1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1NetworkCurrentCellUpdated001, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_networkCurrentCellUpdatedFlag = false;
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(false, g_networkCurrentCellUpdatedFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessageResponse_0100
 * @tc.name  : testV1UpdateCdmaSimMessageResponse001
 * @tc.desc  : Call function UpdateCdmaSimMessageResponse with slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UpdateCdmaSimMessageResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_updateCdmaSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_updateCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessageResponse_0200
 * @tc.name  : testV1UpdateCdmaSimMessageResponse002
 * @tc.desc  : Call function UpdateCdmaSimMessageResponse with slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UpdateCdmaSimMessageResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_updateCdmaSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_updateCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessageResponse_0300
 * @tc.name  : testV1UpdateCdmaSimMessageResponse003
 * @tc.desc  : Call function UpdateCdmaSimMessageResponse with slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UpdateCdmaSimMessageResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_updateCdmaSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_updateCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UpdateCdmaSimMessageResponse_0400
 * @tc.name  : testV1UpdateCdmaSimMessageResponse004
 * @tc.desc  : Call function UpdateCdmaSimMessageResponse with slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1UpdateCdmaSimMessageResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_updateCdmaSimMessageResponseFlag = false;
    SmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_CDMA_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, -1, msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_updateCdmaSimMessageResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddrResponse_0100
 * @tc.name  : testV1SetSmscAddrResponse001
 * @tc.desc  : Call function SetSmscAddrResponse with slotId is SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSmscAddrResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setSmscAddrResponseFlag = false;
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_1, GetSerialId(), address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setSmscAddrResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddrResponse_0200
 * @tc.name  : testV1SetSmscAddrResponse002
 * @tc.desc  : Call function SetSmscAddrResponse with slotId is SLOTID_2.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSmscAddrResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setSmscAddrResponseFlag = false;
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_2, GetSerialId(), address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setSmscAddrResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddrResponse_0300
 * @tc.name  : testV1SetSmscAddrResponse003
 * @tc.desc  : Call function SetSmscAddrResponse with slotId is SLOTID_1 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSmscAddrResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setSmscAddrResponseFlag = false;
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_1, -1, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setSmscAddrResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddrResponse_0400
 * @tc.name  : testV1SetSmscAddrResponse004
 * @tc.desc  : Call function SetSmscAddrResponse with slotId is SLOTID_2 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetSmscAddrResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setSmscAddrResponseFlag = false;
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_2, -1, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setSmscAddrResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddrResponse_0100
 * @tc.name  : testV1GetSmscAddrResponse001
 * @tc.desc  : Call function GetSmscAddrResponse with slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddrResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getSmscAddrResponseFlag = false;
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSmscAddrResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddrResponse_0200
 * @tc.name  : testV1GetSmscAddrResponse002
 * @tc.desc  : Call function GetSmscAddrResponse with slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddrResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getSmscAddrResponseFlag = false;
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSmscAddrResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddrResponse_0300
 * @tc.name  : testV1GetSmscAddrResponse003
 * @tc.desc  : Call function GetSmscAddrResponse with slotId is SLOTID_1 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddrResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getSmscAddrResponseFlag = false;
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSmscAddrResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddrResponse_0400
 * @tc.name  : testV1GetSmscAddrResponse004
 * @tc.desc  : Call function GetSmscAddrResponse with slotId is SLOTID_2 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddrResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getSmscAddrResponseFlag = false;
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_EQ(false, g_getSmscAddrResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfigResponse_0100
 * @tc.name  : testV1SetCBConfigResponse001
 * @tc.desc  : Call function SetCBConfigResponse with slotId is SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetCBConfigResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setCBConfigResponseFlag = false;
    CBConfigInfo info;
    info.mode = 255;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCBConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfigResponse_0200
 * @tc.name  : testV1SetCBConfigResponse002
 * @tc.desc  : Call function SetCBConfigResponse with slotId is SLOTID_2.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetCBConfigResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setCBConfigResponseFlag = false;
    CBConfigInfo info;
    info.mode = 255;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_2, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCBConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfigResponse_0300
 * @tc.name  : testV1SetCBConfigResponse003
 * @tc.desc  : Call function SetCBConfigResponse with slotId is SLOTID_1 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetCBConfigResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setCBConfigResponseFlag = false;
    CBConfigInfo info;
    info.mode = 255;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, -1, info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCBConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfigResponse_0400
 * @tc.name  : testV1SetCBConfigResponse004
 * @tc.desc  : Call function SetCBConfigResponse with slotId is SLOTID_2 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1SetCBConfigResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setCBConfigResponseFlag = false;
    CBConfigInfo info;
    info.mode = 255;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_2, -1, info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCBConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfigResponse_0100
 * @tc.name  : testV1GetCBConfigResponse001
 * @tc.desc  : Call function GetCBConfigResponse with slotId is SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCBConfigResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCBConfigResponseFlag = false;
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCBConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfigResponse_0200
 * @tc.name  : testV1GetCBConfigResponse002
 * @tc.desc  : Call function GetCBConfigResponse with slotId is SLOTID_2.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCBConfigResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCBConfigResponseFlag = false;
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCBConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfigResponse_0300
 * @tc.name  : testV1GetCBConfigResponse003
 * @tc.desc  : Call function GetCBConfigResponse with slotId is SLOTID_1 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCBConfigResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCBConfigResponseFlag = false;
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCBConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfigResponse_0400
 * @tc.name  : testV1GetCBConfigResponse004
 * @tc.desc  : Call function GetCBConfigResponse with slotId is SLOTID_2 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetCBConfigResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCBConfigResponseFlag = false;
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCBConfigResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_0300
 * @tc.name  : testV1GetSmscAddr001
 * @tc.desc  : Call function GetSmscAddr with SerialId is 1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_0400
 * @tc.name  : testV1GetSmscAddr002
 * @tc.desc  : Call function GetSmscAddr with SerialId is 0.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_0500
 * @tc.name  : testV1GetSmscAddr003
 * @tc.desc  : Call function GetSmscAddr with SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_0600
 * @tc.name  : testV1GetSmscAddr004
 * @tc.desc  : Call function GetSmscAddr with SerialId is 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_0700
 * @tc.name  : testV1GetSmscAddr005
 * @tc.desc  : Call function GetSmscAddr with SerialId is 1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_2, 1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_0800
 * @tc.name  : testV1GetSmscAddr006
 * @tc.desc  : Call function GetSmscAddr with SerialId is 0.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_0900
 * @tc.name  : testV1GetSmscAddr007
 * @tc.desc  : Call function GetSmscAddr with SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_1000
 * @tc.name  : testV1GetSmscAddr008
 * @tc.desc  : Call function GetSmscAddr with SerialId is 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_1100
 * @tc.name  : testV1GetSmscAddr009
 * @tc.desc  : Call function GetSmscAddr with SLOTID_1 SerialId is 1 3times.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 3; i++) {
        ret = g_rilInterface->GetSmscAddr(SLOTID_1, 1);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSmscAddr_1200
 * @tc.name  : testV1GetSmscAddr010
 * @tc.desc  : Call function GetSmscAddr with SLOTID_2 SerialId is 1 3times..
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetSmscAddr010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 3; i++) {
        ret = g_rilInterface->GetSmscAddr(SLOTID_2, 1);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfo_0300
 * @tc.name  : testV1GetLinkBandwidthInfo001
 * @tc.desc  : Call function GetLinkBandwidthInfo with slotId is SLOTID_1 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetLinkBandwidthInfo001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_1, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfo_0400
 * @tc.name  : testV1GetLinkBandwidthInfo002
 * @tc.desc  : Call function GetLinkBandwidthInfo with slotId is SLOTID_1 SerialId is 0.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetLinkBandwidthInfo002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_1, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfo_0500
 * @tc.name  : testV1GetLinkBandwidthInfo003
 * @tc.desc  : Call function GetLinkBandwidthInfo with slotId is SLOTID_1 SerialId is 1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetLinkBandwidthInfo003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_1, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfo_0600
 * @tc.name  : testV1GetLinkBandwidthInfo004
 * @tc.desc  : Call function GetLinkBandwidthInfo with slotId is SLOTID_1 SerialId is 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetLinkBandwidthInfo004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_1, 1000000001, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfo_0700
 * @tc.name  : testV1GetLinkBandwidthInfo005
 * @tc.desc  : Call function GetLinkBandwidthInfo with slotId is SLOTID_2 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetLinkBandwidthInfo005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_2, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfo_0800
 * @tc.name  : testV1GetLinkBandwidthInfo006
 * @tc.desc  : Call function GetLinkBandwidthInfo with slotId is SLOTID_2 SerialId is 0.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetLinkBandwidthInfo006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_2, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfo_0900
 * @tc.name  : testV1GetLinkBandwidthInfo007
 * @tc.desc  : Call function GetLinkBandwidthInfo with slotId is SLOTID_2 SerialId is 1.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetLinkBandwidthInfo007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_2, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfo_1000
 * @tc.name  : testV1GetLinkBandwidthInfo008
 * @tc.desc  : Call function GetLinkBandwidthInfo with slotId is SLOTID_2 SerialId is 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional6th, testV1GetLinkBandwidthInfo008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_2, 1000000001, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}