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

class HdfRilHdiTestAdditional1st : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
/**
** HdfRilHdiTestAdditional1st implement
**/
void HdfRilHdiTestAdditional1st::SetUpTestCase()
{
    g_rilInterface = OHOS::HDI::Ril::V1_3::IRil::Get();
    if (g_rilInterface != nullptr) {
        g_rilInterface->SetCallback1_3(&g_callback);
        g_rilInterface->SendRilAck();
        g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
        g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    }
}

void HdfRilHdiTestAdditional1st::TearDownTestCase() {}
void HdfRilHdiTestAdditional1st::SetUp() {}
void HdfRilHdiTestAdditional1st::TearDown() {}
/**
**  Xts Test
**/
/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimStatus_0300
 * @tc.name  : testV1GetSimStatus001
 * @tc.desc  : GetSimStatus, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetSimStatus001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret;
    for (int i = 0; i < 10; i++) {
        ret = g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
        if (ret != 2) {
            WaitFor(WAIT_TIME_SECOND);
            EXPECT_EQ(SUCCESS, ret);
        } else {
            return;
        }
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimStatusResponse_0100
 * @tc.name  : testV1GetSimStatusResponse001
 * @tc.desc  : GetSimStatusResponse, slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetSimStatusResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getSimStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
    if (ret != 2) {
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
        EXPECT_EQ(true, g_getSimStatusResponseFlag);
    } else {
        return;
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimStatusResponse_0200
 * @tc.name  : testV1GetSimStatusResponse002
 * @tc.desc  : GetSimStatusResponse, slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetSimStatusResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getSimStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    if (ret != 2) {
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
        EXPECT_EQ(true, g_getSimStatusResponseFlag);
    } else {
        return;
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimCardStatusResponse_0100
 * @tc.name  : testV1GetSimCardStatusResponse001
 * @tc.desc  : GetSimCardStatusResponse, slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetSimCardStatusResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getSimCardStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
    if (ret != 2) {
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
        EXPECT_EQ(true, g_getSimCardStatusResponseFlag);
    } else {
        return;
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimCardStatusResponse_0200
 * @tc.name  : testV1GetSimCardStatusResponse002
 * @tc.desc  : GetSimCardStatusResponse, slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetSimCardStatusResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getSimCardStatusResponseFlag = false;
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    if (ret != 2) {
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
        EXPECT_EQ(true, g_getSimCardStatusResponseFlag);
    } else {
        return;
    }
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_0600
 * @tc.name   : testV1SetEmergencyCallList004
 * @tc.desc   : Call the function V1 SetEmergencyCallList with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, -11111, emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_0700
 * @tc.name   : testV1SetEmergencyCallList005
 * @tc.desc   : Call the function V1 SetEmergencyCallList with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, 0, emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_0800
 * @tc.name   : testV1SetEmergencyCallList006
 * @tc.desc   : Call the function V1 SetEmergencyCallList with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, 11111, emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_0900
 * @tc.name   : testV1SetEmergencyCallList007
 * @tc.desc   : Call the function V1 SetEmergencyCallList with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, 32768, emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1000
 * @tc.name   : testV1SetEmergencyCallList008
 * @tc.desc   : Call the function V1 SetEmergencyCallList with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList008, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, -32768, emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1100
 * @tc.name   : testV1SetEmergencyCallList009
 * @tc.desc   : Call the function V1 SetEmergencyCallList with index and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 0;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1200
 * @tc.name   : testV1SetEmergencyCallList010
 * @tc.desc   : Call the function V1 SetEmergencyCallList with index and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList010, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = -1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1300
 * @tc.name   : testV1SetEmergencyCallList011
 * @tc.desc   : Call the function V1 SetEmergencyCallList with total and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 0;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1400
 * @tc.name   : testV1SetEmergencyCallList012
 * @tc.desc   : Call the function V1 SetEmergencyCallList with total and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList012, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = -1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1500
 * @tc.name   : testV1SetEmergencyCallList013
 * @tc.desc   : Call the function V1 SetEmergencyCallList with eccNum and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "0";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1600
 * @tc.name   : testV1SetEmergencyCallList014
 * @tc.desc   : Call the function V1 SetEmergencyCallList with eccNum and a value of 110
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "110";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1700
 * @tc.name   : testV1SetEmergencyCallList015
 * @tc.desc   : Call the function V1 SetEmergencyCallList with eccNum and a value of -110
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList015, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "-110";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1800
 * @tc.name   : testV1SetEmergencyCallList016
 * @tc.desc   : Call the function V1 SetEmergencyCallList with mcc and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "0";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_1900
 * @tc.name   : testV1SetEmergencyCallList017
 * @tc.desc   : Call the function V1 SetEmergencyCallList with mcc and a value of 86
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "86";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_2000
 * @tc.name   : testV1SetEmergencyCallList018
 * @tc.desc   : Call the function V1 SetEmergencyCallList with mcc and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList018, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "-1";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_2100
 * @tc.name   : testV1SetEmergencyCallList019
 * @tc.desc   : Call the function V1 SetEmergencyCallList with eccType and a value of TYPE_POLICE
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_POLICE;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_2200
 * @tc.name   : testV1SetEmergencyCallList020
 * @tc.desc   : Call the function V1 SetEmergencyCallList with eccType and a value of TYPE_AMBULANCE
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_AMBULANCE;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_2300
 * @tc.name   : testV1SetEmergencyCallList021
 * @tc.desc   : Call the function V1 SetEmergencyCallList with eccType and a value of TYPE_FIRE
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_FIRE;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_2400
 * @tc.name   : testV1SetEmergencyCallList022
 * @tc.desc   : Call the function V1 SetEmergencyCallList with eccType and a value of TYPE_SEA
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_SEA;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_2500
 * @tc.name   : testV1SetEmergencyCallList023
 * @tc.desc   : Call the function V1 SetEmergencyCallList with eccType and a value of TYPE_MOUNTAIN
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_MOUNTAIN;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_2600
 * @tc.name   : testV1SetEmergencyCallList024
 * @tc.desc   : Call the function V1 SetEmergencyCallList with simpresent and a value of TYPE_NO_CARD
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_NO_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetEmergencyCallList_2700
 * @tc.name   : testV1SetEmergencyCallList025
 * @tc.desc   : Call the function V1 SetEmergencyCallList with abnormalService and a value of TYPE_ONLY_CS
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetEmergencyCallList025, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    EmergencyInfoList emergencyInfoList;
    EmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = EccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = SimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = AbnormalServiceType::TYPE_ONLY_CS;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CloseUnFinishedUssd_0600
 * @tc.name   : testV1CloseUnFinishedUssd004
 * @tc.desc   : Call the function V1 CloseUnFinishedUssd with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CloseUnFinishedUssd004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CloseUnFinishedUssd(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CloseUnFinishedUssd_0700
 * @tc.name   : testV1CloseUnFinishedUssd005
 * @tc.desc   : Call the function V1 CloseUnFinishedUssd with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CloseUnFinishedUssd005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CloseUnFinishedUssd(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CloseUnFinishedUssd_0800
 * @tc.name   : testV1CloseUnFinishedUssd006
 * @tc.desc   : Call the function V1 CloseUnFinishedUssd with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CloseUnFinishedUssd006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CloseUnFinishedUssd(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CloseUnFinishedUssd_0900
 * @tc.name   : testV1CloseUnFinishedUssd007
 * @tc.desc   : Call the function V1 CloseUnFinishedUssd with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CloseUnFinishedUssd007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CloseUnFinishedUssd(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CloseUnFinishedUssd_1000
 * @tc.name   : testV1CloseUnFinishedUssd008
 * @tc.desc   : Call the function V1 CloseUnFinishedUssd with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CloseUnFinishedUssd008, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CloseUnFinishedUssd(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_0600
 * @tc.name   : testV1SetBarringPassword004
 * @tc.desc   : Call the function V1 SetBarringPassword with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, -11111, setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_0700
 * @tc.name   : testV1SetBarringPassword005
 * @tc.desc   : Call the function V1 SetBarringPassword with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, 0, setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_0800
 * @tc.name   : testV1SetBarringPassword006
 * @tc.desc   : Call the function V1 SetBarringPassword with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, 11111, setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_0900
 * @tc.name   : testV1SetBarringPassword007
 * @tc.desc   : Call the function V1 SetBarringPassword with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, 32768, setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_1000
 * @tc.name   : testV1SetBarringPassword008
 * @tc.desc   : Call the function V1 SetBarringPassword with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, -32768, setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_1100
 * @tc.name   : testV1SetBarringPassword009
 * @tc.desc   : Call the function V1 SetBarringPassword with fac and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "0";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_1200
 * @tc.name   : testV1SetBarringPassword010
 * @tc.desc   : Call the function V1 SetBarringPassword with fac and a value of -AB
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "-AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_1300
 * @tc.name   : testV1SetBarringPassword011
 * @tc.desc   : Call the function V1 SetBarringPassword with oldPassword and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "0";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_1400
 * @tc.name   : testV1SetBarringPassword012
 * @tc.desc   : Call the function V1 SetBarringPassword with oldPassword and a value of 1qaz!QAZ
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1qaz!QAZ";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_1500
 * @tc.name   : testV1SetBarringPassword013
 * @tc.desc   : Call the function V1 SetBarringPassword with oldPassword and a value of -1234
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "-1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_1600
 * @tc.name   : testV1SetBarringPassword014
 * @tc.desc   : Call the function V1 SetBarringPassword with newPassword and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "0";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_1700
 * @tc.name   : testV1SetBarringPassword015
 * @tc.desc   : Call the function V1 SetBarringPassword with newPassword and a value of 1qaz!QAZ
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "1qaz!QAZ";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPassword_1800
 * @tc.name   : testV1SetBarringPassword016
 * @tc.desc   : Call the function V1 SetBarringPassword with newPassword and a value of -2345
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetBarringPassword016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "-2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetEmergencyCallList_0600
 * @tc.name   : testV1GetEmergencyCallList004
 * @tc.desc   : Call the function V1 GetEmergencyCallList with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetEmergencyCallList004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetEmergencyCallList_0700
 * @tc.name   : testV1GetEmergencyCallList005
 * @tc.desc   : Call the function V1 GetEmergencyCallList with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetEmergencyCallList005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetEmergencyCallList_0800
 * @tc.name   : testV1GetEmergencyCallList006
 * @tc.desc   : Call the function V1 GetEmergencyCallList with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetEmergencyCallList006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetEmergencyCallList_0900
 * @tc.name   : testV1GetEmergencyCallList007
 * @tc.desc   : Call the function V1 GetEmergencyCallList with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetEmergencyCallList007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetEmergencyCallList_1000
 * @tc.name   : testV1GetEmergencyCallList008
 * @tc.desc   : Call the function V1 GetEmergencyCallList with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetEmergencyCallList008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallList_0600
 * @tc.name   : testV1GetCallList004
 * @tc.desc   : Call the function V1 GetCallList with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallList004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallList_0700
 * @tc.name   : testV1GetCallList005
 * @tc.desc   : Call the function V1 GetCallList with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallList005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallList_0800
 * @tc.name   : testV1GetCallList006
 * @tc.desc   : Call the function V1 GetCallList with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallList006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallList_0900
 * @tc.name   : testV1GetCallList007
 * @tc.desc   : Call the function V1 GetCallList with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallList007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallList_1000
 * @tc.name   : testV1GetCallList008
 * @tc.desc   : Call the function V1 GetCallList with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallList008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Dial_0600
 * @tc.name   : testV1Dial004
 * @tc.desc   : Call the function V1 Dial with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Dial004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, -11111, dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Dial_0700
 * @tc.name   : testV1Dial005
 * @tc.desc   : Call the function V1 Dial with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Dial005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, 0, dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Dial_0800
 * @tc.name   : testV1Dial006
 * @tc.desc   : Call the function V1 Dial with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Dial006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, 11111, dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Dial_0900
 * @tc.name   : testV1Dial007
 * @tc.desc   : Call the function V1 Dial with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Dial007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, 32768, dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Dial_1000
 * @tc.name   : testV1Dial008
 * @tc.desc   : Call the function V1 Dial with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Dial008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, -32768, dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Dial_1100
 * @tc.name   : testV1Dial009
 * @tc.desc   : Call the function V1 Dial with dialInfo.clir and a value of 1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Dial009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 1;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Dial_1200
 * @tc.name   : testV1Dial010
 * @tc.desc   : Call the function V1 Dial with dialInfo.clir and a value of 2
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Dial010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 2;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Dial_1300
 * @tc.name   : testV1Dial011
 * @tc.desc   : Call the function V1 Dial with dialInfo.address and a value of -10086
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Dial011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "-10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Dial_1400
 * @tc.name   : testV1Dial012
 * @tc.desc   : Call the function V1 Dial with dialInfo.address and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Dial012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "0";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Reject_0600
 * @tc.name   : testV1Reject004
 * @tc.desc   : Call the function V1 Reject with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Reject004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Reject_0700
 * @tc.name   : testV1Reject005
 * @tc.desc   : Call the function V1 Reject with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Reject005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Reject_0800
 * @tc.name   : testV1Reject006
 * @tc.desc   : Call the function V1 Reject with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Reject006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Reject_0900
 * @tc.name   : testV1Reject007
 * @tc.desc   : Call the function V1 Reject with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Reject007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Reject_1000
 * @tc.name   : testV1Reject008
 * @tc.desc   : Call the function V1 Reject with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Reject008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Hangup_0600
 * @tc.name   : testV1Hangup004
 * @tc.desc   : Call the function V1 Hangup with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Hangup004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, -11111, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Hangup_0700
 * @tc.name   : testV1Hangup005
 * @tc.desc   : Call the function V1 Hangup with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Hangup005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, 0, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Hangup_0800
 * @tc.name   : testV1Hangup006
 * @tc.desc   : Call the function V1 Hangup with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Hangup006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, 11111, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Hangup_0900
 * @tc.name   : testV1Hangup007
 * @tc.desc   : Call the function V1 Hangup with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Hangup007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, 32768, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Hangup_1000
 * @tc.name   : testV1Hangup008
 * @tc.desc   : Call the function V1 Hangup with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Hangup008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, -32768, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Hangup_1100
 * @tc.name   : testV1Hangup009
 * @tc.desc   : Call the function V1 Hangup with callId and a value of 1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Hangup009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Hangup_1100
 * @tc.name   : testV1Hangup010
 * @tc.desc   : Call the function V1 Hangup with callId and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Hangup010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Hangup_1200
 * @tc.name   : testV1Hangup011
 * @tc.desc   : Call the function V1 Hangup with callId and a value of 255
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Hangup011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, GetSerialId(), 255);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Hangup_1400
 * @tc.name   : testV1Hangup012
 * @tc.desc   : Call the function V1 Hangup with callId and a value of -255
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Hangup012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, GetSerialId(), -255);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Answer_0600
 * @tc.name   : testV1Answer004
 * @tc.desc   : Call the function V1 Answer with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Answer004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Answer_0700
 * @tc.name   : testV1Answer005
 * @tc.desc   : Call the function V1 Answer with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Answer005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Answer_0800
 * @tc.name   : testV1Answer006
 * @tc.desc   : Call the function V1 Answer with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Answer006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Answer_0900
 * @tc.name   : testV1Answer007
 * @tc.desc   : Call the function V1 Answer with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Answer007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1Answer_1000
 * @tc.name   : testV1Answer008
 * @tc.desc   : Call the function V1 Answer with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1Answer008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1HoldCall_0600
 * @tc.name   : testV1HoldCall004
 * @tc.desc   : Call the function V1 HoldCall with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1HoldCall004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1HoldCall_0700
 * @tc.name   : testV1HoldCall005
 * @tc.desc   : Call the function V1 HoldCall with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1HoldCall005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1HoldCall_0800
 * @tc.name   : testV1HoldCall006
 * @tc.desc   : Call the function V1 HoldCall with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1HoldCall006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1HoldCall_0900
 * @tc.name   : testV1HoldCall007
 * @tc.desc   : Call the function V1 HoldCall with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1HoldCall007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1HoldCall_1000
 * @tc.name   : testV1HoldCall008
 * @tc.desc   : Call the function V1 HoldCall with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1HoldCall008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1UnHoldCall_0600
 * @tc.name   : testV1UnHoldCall004
 * @tc.desc   : Call the function V1 UnHoldCall with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1UnHoldCall004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1UnHoldCall_0700
 * @tc.name   : testV1UnHoldCall005
 * @tc.desc   : Call the function V1 UnHoldCall with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1UnHoldCall005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1UnHoldCall_0800
 * @tc.name   : testV1UnHoldCall006
 * @tc.desc   : Call the function V1 UnHoldCall with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1UnHoldCall006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1UnHoldCall_0900
 * @tc.name   : testV1UnHoldCall007
 * @tc.desc   : Call the function V1 UnHoldCall with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1UnHoldCall007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1UnHoldCall_1000
 * @tc.name   : testV1UnHoldCall008
 * @tc.desc   : Call the function V1 UnHoldCall with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1UnHoldCall008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SwitchCall_0600
 * @tc.name   : testV1SwitchCall004
 * @tc.desc   : Call the function V1 SwitchCall with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SwitchCall004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SwitchCall_0700
 * @tc.name   : testV1SwitchCall005
 * @tc.desc   : Call the function V1 SwitchCall with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SwitchCall005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SwitchCall_0800
 * @tc.name   : testV1SwitchCall006
 * @tc.desc   : Call the function V1 SwitchCall with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SwitchCall006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SwitchCall_0900
 * @tc.name   : testV1SwitchCall007
 * @tc.desc   : Call the function V1 SwitchCall with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SwitchCall007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SwitchCall_1000
 * @tc.name   : testV1SwitchCall008
 * @tc.desc   : Call the function V1 SwitchCall with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SwitchCall008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CombineConference_0600
 * @tc.name   : testV1CombineConference004
 * @tc.desc   : Call the function V1 CombineConference with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CombineConference004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, -11111, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CombineConference_0700
 * @tc.name   : testV1CombineConference005
 * @tc.desc   : Call the function V1 CombineConference with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CombineConference005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, 0, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CombineConference_0800
 * @tc.name   : testV1CombineConference006
 * @tc.desc   : Call the function V1 CombineConference with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CombineConference006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, 11111, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CombineConference_0900
 * @tc.name   : testV1CombineConference007
 * @tc.desc   : Call the function V1 CombineConference with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CombineConference007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, 32768, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CombineConference_1000
 * @tc.name   : testV1CombineConference008
 * @tc.desc   : Call the function V1 CombineConference with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CombineConference008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, -32768, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CombineConference_1100
 * @tc.name   : testV1CombineConference009
 * @tc.desc   : Call the function V1 CombineConference with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1CombineConference009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_0600
 * @tc.name   : testV1SeparateConference004
 * @tc.desc   : Call the function V1 SeparateConference with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, -11111, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_0600
 * @tc.name   : testV1SeparateConference005
 * @tc.desc   : Call the function V1 SeparateConference with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, 0, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_0800
 * @tc.name   : testV1SeparateConference006
 * @tc.desc   : Call the function V1 SeparateConference with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, 11111, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_0900
 * @tc.name   : testV1SeparateConference007
 * @tc.desc   : Call the function V1 SeparateConference with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, 32768, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_1000
 * @tc.name   : testV1SeparateConference008
 * @tc.desc   : Call the function V1 SeparateConference with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, -32768, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_1100
 * @tc.name   : testV1SeparateConference009
 * @tc.desc   : Call the function V1 SeparateConference with callId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, GetSerialId(), 0, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_1200
 * @tc.name   : testV1SeparateConference010
 * @tc.desc   : Call the function V1 SeparateConference with callId and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, GetSerialId(), -1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_1300
 * @tc.name   : testV1SeparateConference011
 * @tc.desc   : Call the function V1 SeparateConference with callId and a value of 255
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, GetSerialId(), 255, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_1400
 * @tc.name   : testV1SeparateConference012
 * @tc.desc   : Call the function V1 SeparateConference with callId and a value of -255
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, GetSerialId(), -255, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SeparateConference_1500
 * @tc.name   : testV1SeparateConference013
 * @tc.desc   : Call the function V1 SeparateConference with callType and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SeparateConference013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, GetSerialId(), 1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallWaiting_0600
 * @tc.name   : testV1GetCallWaiting004
 * @tc.desc   : Call the function V1 GetCallWaiting with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallWaiting004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallWaiting_0700
 * @tc.name   : testV1GetCallWaiting005
 * @tc.desc   : Call the function V1 GetCallWaiting with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallWaiting005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallWaiting_0800
 * @tc.name   : testV1GetCallWaiting006
 * @tc.desc   : Call the function V1 GetCallWaiting with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallWaiting006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallWaiting_0900
 * @tc.name   : testV1GetCallWaiting007
 * @tc.desc   : Call the function V1 GetCallWaiting with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallWaiting007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallWaiting_1000
 * @tc.name   : testV1GetCallWaiting008
 * @tc.desc   : Call the function V1 GetCallWaiting with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallWaiting008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallWaiting_0600
 * @tc.name   : testV1SetCallWaiting004
 * @tc.desc   : Call the function V1 SetCallWaiting with serialId and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallWaiting004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, -11111, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallWaiting_0700
 * @tc.name   : testV1SetCallWaiting005
 * @tc.desc   : Call the function V1 SetCallWaiting with serialId and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallWaiting005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, 0, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallWaiting_0800
 * @tc.name   : testV1SetCallWaiting006
 * @tc.desc   : Call the function V1 SetCallWaiting with serialId and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallWaiting006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, 11111, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallWaiting_0900
 * @tc.name   : testV1SetCallWaiting007
 * @tc.desc   : Call the function V1 SetCallWaiting with serialId and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallWaiting007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, 32768, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallWaiting_1000
 * @tc.name   : testV1SetCallWaiting008
 * @tc.desc   : Call the function V1 SetCallWaiting with serialId and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallWaiting008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, -32768, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallWaiting_1100
 * @tc.name   : testV1SetCallWaiting009
 * @tc.desc   : Call the function V1 SetCallWaiting with activate  and a value of 1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallWaiting009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallWaiting_1200
 * @tc.name   : testV1SetCallWaiting010
 * @tc.desc   : Call the function V1 SetCallWaiting with activate  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallWaiting010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallWaiting_1300
 * @tc.name   : testV1SetCallWaiting011
 * @tc.desc   : Call the function V1 SetCallWaiting with activate  and a value of 255
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallWaiting011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, GetSerialId(), 255);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallWaiting_1400
 * @tc.name   : testV1SetCallWaiting012
 * @tc.desc   : Call the function V1 SetCallWaiting with activate  and a value of -255
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallWaiting012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, GetSerialId(), -255);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_0600
 * @tc.name   : testV1GetCallTransferInfo004
 * @tc.desc   : Call the function V1 GetCallTransferInfo with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, -11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_0700
 * @tc.name   : testV1GetCallTransferInfo005
 * @tc.desc   : Call the function V1 GetCallTransferInfo with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_0800
 * @tc.name   : testV1GetCallTransferInfo006
 * @tc.desc   : Call the function V1 GetCallTransferInfo with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, 11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_0900
 * @tc.name   : testV1GetCallTransferInfo007
 * @tc.desc   : Call the function V1 GetCallTransferInfo with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, 32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_1000
 * @tc.name   : testV1GetCallTransferInfo008
 * @tc.desc   : Call the function V1 GetCallTransferInfo with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, -32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_1100
 * @tc.name   : testV1GetCallTransferInfo009
 * @tc.desc   : Call the function V1 GetCallTransferInfo with reason  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_1200
 * @tc.name   : testV1GetCallTransferInfo010
 * @tc.desc   : Call the function V1 GetCallTransferInfo with reason  and a value of 2
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, GetSerialId(), 2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_1300
 * @tc.name   : testV1GetCallTransferInfo011
 * @tc.desc   : Call the function V1 GetCallTransferInfo with reason  and a value of 3
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, GetSerialId(), 3);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_1400
 * @tc.name   : testV1GetCallTransferInfo012
 * @tc.desc   : Call the function V1 GetCallTransferInfo with reason  and a value of 4
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, GetSerialId(), 4);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallTransferInfo_1500
 * @tc.name   : testV1GetCallTransferInfo013
 * @tc.desc   : Call the function V1 GetCallTransferInfo with reason  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallTransferInfo013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_0600
 * @tc.name   : testV1SetCallTransferInfo004
 * @tc.desc   : Call the function V1 SetCallTransferInfo with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, -11111, callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_0700
 * @tc.name   : testV1SetCallTransferInfo005
 * @tc.desc   : Call the function V1 SetCallTransferInfo with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, 0, callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_0800
 * @tc.name   : testV1SetCallTransferInfo006
 * @tc.desc   : Call the function V1 SetCallTransferInfo with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, 11111, callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_0900
 * @tc.name   : testV1SetCallTransferInfo007
 * @tc.desc   : Call the function V1 SetCallTransferInfo with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, 32768, callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1000
 * @tc.name   : testV1SetCallTransferInfo008
 * @tc.desc   : Call the function V1 SetCallTransferInfo with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, -32768, callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1100
 * @tc.name   : testV1SetCallTransferInfo009
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.reason  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 0;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1200
 * @tc.name   : testV1SetCallTransferInfo010
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.reason  and a value of 2
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 2;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1300
 * @tc.name   : testV1SetCallTransferInfo011
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.reason  and a value of 3
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 3;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1400
 * @tc.name   : testV1SetCallTransferInfo012
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.reason  and a value of 4
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 4;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1500
 * @tc.name   : testV1SetCallTransferInfo013
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.reason  and a value of 5
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 5;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1600
 * @tc.name   : testV1SetCallTransferInfo014
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.reason  and a value of 6
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 6;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1700
 * @tc.name   : testV1SetCallTransferInfo015
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.reason  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = -1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1800
 * @tc.name   : testV1SetCallTransferInfo016
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.mode  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 0;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_1900
 * @tc.name   : testV1SetCallTransferInfo017
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.mode  and a value of 2
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 2;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_2000
 * @tc.name   : testV1SetCallTransferInfo018
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.mode  and a value of 3
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 3;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_2100
 * @tc.name   : testV1SetCallTransferInfo019
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.mode  and a value of 4
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 4;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_2200
 * @tc.name   : testV1SetCallTransferInfo020
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.mode  and a value of 5
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 5;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_2300
 * @tc.name   : testV1SetCallTransferInfo021
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.mode  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = -1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_2400
 * @tc.name   : testV1SetCallTransferInfo022
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.number  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "0";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_2500
 * @tc.name   : testV1SetCallTransferInfo023
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.number  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "-1";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfo_2600
 * @tc.name   : testV1SetCallTransferInfo024
 * @tc.desc   : Call the function V1 SetCallTransferInfo with callForwardSetInfo.number  and a value of 12306
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallTransferInfo024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "12306";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallRestriction_0600
 * @tc.name   : testV1GetCallRestriction004
 * @tc.desc   : Call the function V1 GetCallRestriction with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallRestriction004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_1, -11111, fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallRestriction_0700
 * @tc.name   : testV1GetCallRestriction005
 * @tc.desc   : Call the function V1 GetCallRestriction with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallRestriction005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_1, 0, fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallRestriction_0800
 * @tc.name   : testV1GetCallRestriction006
 * @tc.desc   : Call the function V1 GetCallRestriction with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallRestriction006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_1, 11111, fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallRestriction_0900
 * @tc.name   : testV1GetCallRestriction007
 * @tc.desc   : Call the function V1 GetCallRestriction with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallRestriction007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_1, 32768, fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallRestriction_1000
 * @tc.name   : testV1GetCallRestriction008
 * @tc.desc   : Call the function V1 GetCallRestriction with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallRestriction008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_1, -32768, fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_0600
 * @tc.name   : testV1SetCallRestriction004
 * @tc.desc   : Call the function V1 SetCallRestriction with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, -11111, callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_0700
 * @tc.name   : testV1SetCallRestriction005
 * @tc.desc   : Call the function V1 SetCallRestriction with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, 0, callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_0800
 * @tc.name   : testV1SetCallRestriction006
 * @tc.desc   : Call the function V1 SetCallRestriction with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, 11111, callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_0900
 * @tc.name   : testV1SetCallRestriction007
 * @tc.desc   : Call the function V1 SetCallRestriction with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, 32768, callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_1000
 * @tc.name   : testV1SetCallRestriction008
 * @tc.desc   : Call the function V1 SetCallRestriction with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, -32768, callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_1100
 * @tc.name   : testV1SetCallRestriction009
 * @tc.desc   : Call the function V1 SetCallRestriction with callRestrictionInfo.mode  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 0;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_1200
 * @tc.name   : testV1SetCallRestriction010
 * @tc.desc   : Call the function V1 SetCallRestriction with callRestrictionInfo.mode  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = -1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_1300
 * @tc.name   : testV1SetCallRestriction011
 * @tc.desc   : Call the function V1 SetCallRestriction with callRestrictionInfo.mode  and a value of 255
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 255;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_1400
 * @tc.name   : testV1SetCallRestriction012
 * @tc.desc   : Call the function V1 SetCallRestriction with callRestrictionInfo.password  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "0";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_1500
 * @tc.name   : testV1SetCallRestriction013
 * @tc.desc   : Call the function V1 SetCallRestriction with callRestrictionInfo.password  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 255;
    callRestrictionInfo.password = "-1";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_1600
 * @tc.name   : testV1SetCallRestriction014
 * @tc.desc   : Call the function V1 SetCallRestriction with callRestrictionInfo.password  and a value of abcd1234
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 255;
    callRestrictionInfo.password = "abcd1234";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestriction_1700
 * @tc.name   : testV1SetCallRestriction015
 * @tc.desc   : Verifying whether a value of 1qaz!QAZ for the password is feasible
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallRestriction015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 255;
    callRestrictionInfo.password = "1qaz!QAZ";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClip_0600
 * @tc.name   : testV1GetClip004
 * @tc.desc   : Call the function V1 GetClip with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClip004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClip_0700
 * @tc.name   : testV1GetClip005
 * @tc.desc   : Call the function V1 GetClip with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClip005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClip_0800
 * @tc.name   : testV1GetClip006
 * @tc.desc   : Call the function V1 GetClip with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClip006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClip_0900
 * @tc.name   : testV1GetClip007
 * @tc.desc   : Call the function V1 GetClip with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClip007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClip_1000
 * @tc.name   : testV1GetClip008
 * @tc.desc   : Call the function V1 GetClip with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClip008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClip_0600
 * @tc.name   : testV1SetClip004
 * @tc.desc   : Call the function V1 SetClip with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClip004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, -11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClip_0700
 * @tc.name   : testV1SetClip005
 * @tc.desc   : Call the function V1 SetClip with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClip005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClip_0800
 * @tc.name   : testV1SetClip006
 * @tc.desc   : Call the function V1 SetClip with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClip006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, 11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClip_0900
 * @tc.name   : testV1SetClip007
 * @tc.desc   : Call the function V1 SetClip with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClip007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, 32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClip_1000
 * @tc.name   : testV1SetClip008
 * @tc.desc   : Call the function V1 SetClip with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClip008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, -32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClip_1100
 * @tc.name   : testV1SetClip009
 * @tc.desc   : Call the function V1 SetClip with action  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClip009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClip_1200
 * @tc.name   : testV1SetClip010
 * @tc.desc   : Call the function V1 SetClip with action  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClip010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClir_0600
 * @tc.name   : testV1GetClir004
 * @tc.desc   : Call the function V1 GetClir with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClir004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClir_0700
 * @tc.name   : testV1GetClir005
 * @tc.desc   : Call the function V1 GetClir with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClir005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClir_0800
 * @tc.name   : testV1GetClir006
 * @tc.desc   : Call the function V1 GetClir with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClir006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClir_0900
 * @tc.name   : testV1GetClir007
 * @tc.desc   : Call the function V1 GetClir with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClir007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClir_1000
 * @tc.name   : testV1GetClir008
 * @tc.desc   : Call the function V1 GetClir with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetClir008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClir_0600
 * @tc.name   : testV1SetClir004
 * @tc.desc   : Call the function V1 SetClir with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClir004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, -11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClir_0700
 * @tc.name   : testV1SetClir005
 * @tc.desc   : Call the function V1 SetClir with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClir005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClir_0800
 * @tc.name   : testV1SetClir006
 * @tc.desc   : Call the function V1 SetClir with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClir006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, 11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClir_0900
 * @tc.name   : testV1SetClir007
 * @tc.desc   : Call the function V1 SetClir with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClir007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, 32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClir_1000
 * @tc.name   : testV1SetClir008
 * @tc.desc   : Call the function V1 SetClir with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClir008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, -32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClir_1100
 * @tc.name   : testV1SetClir009
 * @tc.desc   : Call the function V1 SetClir with action  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClir009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClir_1200
 * @tc.name   : testV1SetClir010
 * @tc.desc   : Call the function V1 SetClir with action  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetClir010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_0600
 * @tc.name   : testV1SetCallPreferenceMode004
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, -11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_0700
 * @tc.name   : testV1SetCallPreferenceMode005
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_0800
 * @tc.name   : testV1SetCallPreferenceMode006
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, 11111, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_0900
 * @tc.name   : testV1SetCallPreferenceMode007
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, 32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_1000
 * @tc.name   : testV1SetCallPreferenceMode008
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, -32768, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_1100
 * @tc.name   : testV1SetCallPreferenceMode009
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with mode  and a value of 2
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, GetSerialId(), 2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_1200
 * @tc.name   : testV1SetCallPreferenceMode010
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with mode  and a value of 3
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, GetSerialId(), 3);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_1300
 * @tc.name   : testV1SetCallPreferenceMode011
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with mode  and a value of 4
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, GetSerialId(), 4);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_1400
 * @tc.name   : testV1SetCallPreferenceMode012
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with mode  and a value of 5
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, GetSerialId(), 5);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceMode_1500
 * @tc.name   : testV1SetCallPreferenceMode013
 * @tc.desc   : Call the function V1 SetCallPreferenceMode with mode  and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCallPreferenceMode013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallPreferenceMode_0600
 * @tc.name   : testV1GetCallPreferenceMode004
 * @tc.desc   : Call the function V1 GetCallPreferenceMode with serialId  and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallPreferenceMode004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_1, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallPreferenceMode_0700
 * @tc.name   : testV1GetCallPreferenceMode005
 * @tc.desc   : Call the function V1 GetCallPreferenceMode with serialId  and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallPreferenceMode005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallPreferenceMode_0800
 * @tc.name   : testV1GetCallPreferenceMode006
 * @tc.desc   : Call the function V1 GetCallPreferenceMode with serialId  and a value of 11111
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallPreferenceMode006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_1, 11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallPreferenceMode_0900
 * @tc.name   : testV1GetCallPreferenceMode007
 * @tc.desc   : Call the function V1 GetCallPreferenceMode with serialId  and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallPreferenceMode007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_1, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallPreferenceMode_1000
 * @tc.name   : testV1GetCallPreferenceMode008
 * @tc.desc   : Call the function V1 GetCallPreferenceMode with serialId  and a value of -32768
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1GetCallPreferenceMode008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_1, -32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_0300
 * @tc.name  : testV1SetCBConfig001
 * @tc.desc  : Call function SetCBConfig with an gsmType of 0.
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCBConfig001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 0;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_0500
 * @tc.name  : testV1SetCBConfig002
 * @tc.desc  : Call function SetCBConfig with an gsmType of 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCBConfig002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 0;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = 0;
    for (int i = 0; i < 10; i++) {
        ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_0600
 * @tc.name  : testV1SetCBConfig003
 * @tc.desc  : Call function SetCBConfig with an entry of -1.
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCBConfig003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = -1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_0700
 * @tc.name  : testV1SetCBConfig004
 * @tc.desc  : Call function SetCBConfig with the second argument as 255.
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCBConfig004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, 255, info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_0800
 * @tc.name  : testV1SetCBConfig005
 * @tc.desc  : Call function SetCBConfig with the second argument as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCBConfig005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, 1, info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_0900
 * @tc.name  : testV1SetCBConfig006
 * @tc.desc  : Call function SetCBConfig with the second argument as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCBConfig006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, 1000000001, info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1000
 * @tc.name  : testV1SetCBConfig007
 * @tc.desc  : Call function SetCBConfig with the second argument as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCBConfig007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, -1, info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1100
 * @tc.name  : testV1SetCBConfig008
 * @tc.desc  : Call function SetCBConfig with the third argument as info.mode = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCBConfig008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CBConfigInfo info;
    info.mode = 255;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1200
 * @tc.name  : testV1SetCBConfig009
 * @tc.desc  : Call function SetCBConfig with the third argument as info.mode = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional1st, testV1SetCBConfig009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CBConfigInfo info;
    info.mode = 1;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}