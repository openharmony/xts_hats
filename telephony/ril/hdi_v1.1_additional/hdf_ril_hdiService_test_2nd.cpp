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

class HdfRilHdiTestAdditional2nd : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
/**
** HdfRilHdiTestAdditional2nd implement
**/
void HdfRilHdiTestAdditional2nd::SetUpTestCase()
{
    g_rilInterface = OHOS::HDI::Ril::V1_2::IRil::Get();
    if (g_rilInterface != nullptr) {
        g_rilInterface->SetCallback1_2(&g_callback);
        g_rilInterface->SendRilAck();
        g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
        g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    }
}

void HdfRilHdiTestAdditional2nd::TearDownTestCase() {}
void HdfRilHdiTestAdditional2nd::SetUp() {}
void HdfRilHdiTestAdditional2nd::TearDown() {}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1300
 * @tc.name  : testV1SetCBConfig010
 * @tc.desc  : Call function SetCBConfig with the third argument as info.mode = 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CBConfigInfo info;
    info.mode = 1000000001;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1400
 * @tc.name  : testV1SetCBConfig011
 * @tc.desc  : Call function SetCBConfig with the third argument as info.mode = -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig011, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CBConfigInfo info;
    info.mode = -1;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1500
 * @tc.name  : testV1SetCBConfig012
 * @tc.desc  : Call function SetCBConfig with the third argument as info.mids = TEST_STORAGE_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_STORAGE_PDU;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1600
 * @tc.name  : testV1SetCBConfig013
 * @tc.desc  : Call function SetCBConfig with the third argument as info.mids = TEST_SEND_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_SEND_PDU;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1700
 * @tc.name  : testV1SetCBConfig014
 * @tc.desc  : Call function SetCBConfig with the third argument as info.mids = TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_SMSC_PDU;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1800
 * @tc.name  : testV1SetCBConfig015
 * @tc.desc  : Call function SetCBConfig with the third argument as info.mids = TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_CDMA_PDU;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_1900
 * @tc.name  : testV1SetCBConfig016
 * @tc.desc  : Call function SetCBConfig with the third argument as info.dcss = TEST_ID_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_ID_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_2000
 * @tc.name  : testV1SetCBConfig017
 * @tc.desc  : Call function SetCBConfig with the third argument as info.dcss = TEST_STORAGE_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_2100
 * @tc.name  : testV1SetCBConfig018
 * @tc.desc  : Call function SetCBConfig with the third argument as info.dcss = TEST_SEND_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_2200
 * @tc.name  : testV1SetCBConfig019
 * @tc.desc  : Call function SetCBConfig with the third argument as info.dcss = TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_SMSC_PDU;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_2300
 * @tc.name  : testV1SetCBConfig020
 * @tc.desc  : Call function SetCBConfig with the third argument as info.dcss = TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCBConfig_2400
 * @tc.name  : testV1SetCBConfig021
 * @tc.desc  : Call function SetCBConfig 100 times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCBConfig021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t gsmType = 1;
    int32_t ret = 0;
    CBConfigInfo info;
    info.mode = gsmType;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfig_0300
 * @tc.name  : testV1GetCBConfig001
 * @tc.desc  : Call function GetCBConfig with an entry of 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCBConfig001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_2, 1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfig_0400
 * @tc.name  : testV1GetCBConfig002
 * @tc.desc  : Call function GetCBConfig with an entry of -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCBConfig002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfig_0500
 * @tc.name  : testV1GetCBConfig003
 * @tc.desc  : Call function GetCBConfig with an entry of 1 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCBConfig003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetCBConfig(SLOTID_2, 1);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfig_0700
 * @tc.name  : testV1GetCBConfig005
 * @tc.desc  : Call function GetCBConfig 100 times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCBConfig005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetCBConfig(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfig_0800
 * @tc.name  : testV1GetCBConfig006
 * @tc.desc  : Call function GetCBConfig with the second argument as 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCBConfig006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_1, 255);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfig_0900
 * @tc.name  : testV1GetCBConfig007
 * @tc.desc  : Call function GetCBConfig with the second argument as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCBConfig007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfig_1000
 * @tc.name  : testV1GetCBConfig008
 * @tc.desc  : Call function GetCBConfig with the second argument as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCBConfig008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCBConfig_1100
 * @tc.name  : testV1GetCBConfig009
 * @tc.desc  : Call function GetCBConfig with the second argument as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCBConfig009, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_0300
 * @tc.name  : testV1SetCdmaCBConfig001
 * @tc.desc  : Call function SetCdmaCBConfig with an entry of -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig001, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_2, -1, broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_0500
 * @tc.name  : testV1SetCdmaCBConfig003
 * @tc.desc  : Call function SetCdmaCBConfig with an entry of 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_2, 0, broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_0600
 * @tc.name  : testV1SetCdmaCBConfig004
 * @tc.desc  : Call function SetCdmaCBConfig with an entry of 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    CdmaCBConfigInfoList broadcastInfoList = {};
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetCdmaCBConfig(SLOTID_2, 0, broadcastInfoList);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_0700
 * @tc.name  : testV1SetCdmaCBConfig005
 * @tc.desc  : Call function SetCdmaCBConfig with the second argument as 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, 255, broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_0800
 * @tc.name  : testV1SetCdmaCBConfig006
 * @tc.desc  : Call function SetCdmaCBConfig with the second argument as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, 1, broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_0900
 * @tc.name  : testV1SetCdmaCBConfig007
 * @tc.desc  : Call function SetCdmaCBConfig with the second argument as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, 1000000001, broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1000
 * @tc.name  : testV1SetCdmaCBConfig008
 * @tc.desc  : Call function SetCdmaCBConfig with the second argument as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig008, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, -1, broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1100
 * @tc.name  : testV1SetCdmaCBConfig009
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.serial as SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.serial = SLOTID_1;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1200
 * @tc.name  : testV1SetCdmaCBConfig010
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.serial as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.serial = 1000000001;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1300
 * @tc.name  : testV1SetCdmaCBConfig011
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.serial as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig011, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.serial = -1;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1400
 * @tc.name  : testV1SetCdmaCBConfig012
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.serial as 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.serial = 255;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1500
 * @tc.name  : testV1SetCdmaCBConfig013
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.serial as 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.serial = 1;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1600
 * @tc.name  : testV1SetCdmaCBConfig014
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.size as SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.size = SLOTID_1;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1700
 * @tc.name  : testV1SetCdmaCBConfig015
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.size as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.size = 1000000001;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1800
 * @tc.name  : testV1SetCdmaCBConfig016
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.size as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig016, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.size = -1;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_1900
 * @tc.name  : testV1SetCdmaCBConfig017
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.size as 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.size = 255;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetCdmaCBConfig_2000
 * @tc.name  : testV1SetCdmaCBConfig018
 * @tc.desc  : Call function SetCdmaCBConfig with the broadcastInfoList.size as 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetCdmaCBConfig018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    CdmaCBConfigInfoList broadcastInfoList = {};
    broadcastInfoList.size = 1;
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_0300
 * @tc.name  : testV1GetCdmaCBConfig001
 * @tc.desc  : Call function GetCdmaCBConfig with the SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_0400
 * @tc.name  : testV1GetCdmaCBConfig002
 * @tc.desc  : Call function GetCdmaCBConfig with the 1 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetCdmaCBConfig(SLOTID_2, GetSerialId());
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_0500
 * @tc.name  : testV1GetCdmaCBConfig003
 * @tc.desc  : Call function GetCdmaCBConfig with the -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_0600
 * @tc.name  : testV1GetCdmaCBConfig004
 * @tc.desc  : Call function GetCdmaCBConfig with the 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_0700
 * @tc.name  : testV1GetCdmaCBConfig005
 * @tc.desc  : Call function GetCdmaCBConfig with the 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_0800
 * @tc.name  : testV1GetCdmaCBConfig006
 * @tc.desc  : Call function GetCdmaCBConfig with the 1000000001 100Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;

    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetCdmaCBConfig(SLOTID_2, 1000000001);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_0900
 * @tc.name  : testV1GetCdmaCBConfig007
 * @tc.desc  : Call function GetCdmaCBConfig with the SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, SLOTID_1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_1000
 * @tc.name  : testV1GetCdmaCBConfig008
 * @tc.desc  : Call function GetCdmaCBConfig with the 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_1100
 * @tc.name  : testV1GetCdmaCBConfig009
 * @tc.desc  : Call function GetCdmaCBConfig with the 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, 255);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_1200
 * @tc.name  : testV1GetCdmaCBConfig010
 * @tc.desc  : Call function GetCdmaCBConfig with the 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_1300
 * @tc.name  : testV1GetCdmaCBConfig011
 * @tc.desc  : Call function GetCdmaCBConfig with the -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCdmaCBConfig_1400
 * @tc.name  : testV1GetCdmaCBConfig012
 * @tc.desc  : Call function GetCdmaCBConfig with the 255 100Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCdmaCBConfig012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;

    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, 255);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_0400
 * @tc.name  : testV1SendSmsMoreMode001
 * @tc.desc  : Call function SendSmsMoreMode with the SLOTID_1 100Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    GsmSmsMessageInfo msg;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_0500
 * @tc.name  : testV1SendSmsMoreMode002
 * @tc.desc  : Call function SendSmsMoreMode with the 100Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    GsmSmsMessageInfo msg;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SendSmsMoreMode(SLOTID_2, GetSerialId(), msg);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_0700
 * @tc.name  : testV1SendSmsMoreMode003
 * @tc.desc  : Call function SendSmsMoreMode with the -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    GsmSmsMessageInfo msg;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_2, -1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_0800
 * @tc.name  : testV1SendSmsMoreMode004
 * @tc.desc  : Call function SendSmsMoreMode with the 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    GsmSmsMessageInfo msg;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_2, 0, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_0900
 * @tc.name  : testV1SendSmsMoreMode005
 * @tc.desc  : Call function SendSmsMoreMode with the second SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, SLOTID_1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1000
 * @tc.name  : testV1SendSmsMoreMode006
 * @tc.desc  : Call function SendSmsMoreMode with the second SLOTID_1 100Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    GsmSmsMessageInfo msg;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, SLOTID_1, msg);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1100
 * @tc.name  : testV1SendSmsMoreMode007
 * @tc.desc  : Call function SendSmsMoreMode with the second 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, 255, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1200
 * @tc.name  : testV1SendSmsMoreMode008
 * @tc.desc  : Call function SendSmsMoreMode with the second 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, 1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1300
 * @tc.name  : testV1SendSmsMoreMode009
 * @tc.desc  : Call function SendSmsMoreMode with the second -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, -1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1400
 * @tc.name  : testV1SendSmsMoreMode010
 * @tc.desc  : Call function SendSmsMoreMode with the second 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, 1000000001, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1500
 * @tc.name  : testV1SendSmsMoreMode011
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.serial = SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = SLOTID_1;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1600
 * @tc.name  : testV1SendSmsMoreMode012
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.serial = SLOTID_1 100Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    GsmSmsMessageInfo msg;
    msg.serial = SLOTID_1;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1700
 * @tc.name  : testV1SendSmsMoreMode013
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.serial = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = 255;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1800
 * @tc.name  : testV1SendSmsMoreMode014
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.serial = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = 1;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_1900
 * @tc.name  : testV1SendSmsMoreMode015
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.serial = -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = -1;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2000
 * @tc.name  : testV1SendSmsMoreMode016
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.serial = 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = 1000000001;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2100
 * @tc.name  : testV1SendSmsMoreMode017
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.state = SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = SLOTID_1;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2200
 * @tc.name  : testV1SendSmsMoreMode018
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.state = SLOTID_1 100Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    GsmSmsMessageInfo msg;
    msg.state = SLOTID_1;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2300
 * @tc.name  : testV1SendSmsMoreMode019
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.state = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = 255;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2400
 * @tc.name  : testV1SendSmsMoreMode020
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.state = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = 1;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2500
 * @tc.name  : testV1SendSmsMoreMode021
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.state = -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = -1;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2600
 * @tc.name  : testV1SendSmsMoreMode022
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.state = 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = 1000000001;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2700
 * @tc.name  : testV1SendSmsMoreMode023
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.smscPdu = TEST_STORAGE_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2800
 * @tc.name  : testV1SendSmsMoreMode024
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.smscPdu = TEST_STORAGE_PDU 100Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_STORAGE_PDU;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_2900
 * @tc.name  : testV1SendSmsMoreMode025
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.smscPdu = TEST_SEND_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode025, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3000
 * @tc.name  : testV1SendSmsMoreMode026
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.smscPdu = TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode026, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3100
 * @tc.name  : testV1SendSmsMoreMode027
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.smscPdu = TEST_STORAGE_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode027, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3200
 * @tc.name  : testV1SendSmsMoreMode028
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.smscPdu = TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode028, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3300
 * @tc.name  : testV1SendSmsMoreMode029
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.smscPdu = TEST_ID_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode029, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_ID_LIST;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3400
 * @tc.name  : testV1SendSmsMoreMode030
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.smscPdu = TEST_DCS_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode030, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3500
 * @tc.name  : testV1SendSmsMoreMode031
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.pdu = TEST_STORAGE_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode031, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.pdu = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3600
 * @tc.name  : testV1SendSmsMoreMode032
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.pdu = TEST_STORAGE_PDU 100Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode032, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    GsmSmsMessageInfo msg;
    msg.pdu = TEST_STORAGE_PDU;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3700
 * @tc.name  : testV1SendSmsMoreMode033
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.pdu = TEST_SEND_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode033, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3800
 * @tc.name  : testV1SendSmsMoreMode034
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.pdu = TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode034, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.pdu = TEST_SMSC_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_3900
 * @tc.name  : testV1SendSmsMoreMode035
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.pdu = TEST_STORAGE_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode035, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.pdu = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_4000
 * @tc.name  : testV1SendSmsMoreMode036
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.pdu = TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode036, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.pdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_4100
 * @tc.name  : testV1SendSmsMoreMode037
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.pdu = TEST_ID_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode037, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.pdu = TEST_ID_LIST;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsMoreMode_4200
 * @tc.name  : testV1SendSmsMoreMode038
 * @tc.desc  : Call function SendSmsMoreMode with the third argument as msg.pdu = TEST_DCS_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsMoreMode038, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.pdu = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendRilAck_0200
 * @tc.name  : testV1SendRilAck001
 * @tc.desc  : Call function SendRilAck 100 Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendRilAck001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SendRilAck();
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_0300
 * @tc.name  : testV1GetRrcConnectionState001
 * @tc.desc  : Call function GetRrcConnectionState 100 Times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetRrcConnectionState(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_0400
 * @tc.name  : testV1GetRrcConnectionState002
 * @tc.desc  : Call function GetRrcConnectionState with -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_0500
 * @tc.name  : testV1GetRrcConnectionState003
 * @tc.desc  : Call function GetRrcConnectionState with 1 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetRrcConnectionState(SLOTID_2, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_0600
 * @tc.name  : testV1GetRrcConnectionState004
 * @tc.desc  : Call function GetRrcConnectionState with 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_0700
 * @tc.name  : testV1GetRrcConnectionState005
 * @tc.desc  : Call function GetRrcConnectionState with 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_2, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_0800
 * @tc.name  : testV1GetRrcConnectionState006
 * @tc.desc  : Call function GetRrcConnectionState with Second SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_1, SLOTID_1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_0900
 * @tc.name  : testV1GetRrcConnectionState007
 * @tc.desc  : Call function GetRrcConnectionState with Second 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_1, 255);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_1000
 * @tc.name  : testV1GetRrcConnectionState008
 * @tc.desc  : Call function GetRrcConnectionState with Second 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_1100
 * @tc.name  : testV1GetRrcConnectionState009
 * @tc.desc  : Call function GetRrcConnectionState with Second -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRrcConnectionState_1200
 * @tc.name  : testV1GetRrcConnectionState010
 * @tc.desc  : Call function GetRrcConnectionState with Second 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRrcConnectionState010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRrcConnectionState(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_0300
 * @tc.name  : testV1SendSmsAck001
 * @tc.desc  : Call function SendSmsAck with 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ModeData data;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_1, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_0500
 * @tc.name  : testV1SendSmsAck003
 * @tc.desc  : Call function SendSmsAck with -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ModeData data;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_1, -1, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_0600
 * @tc.name  : testV1SendSmsAck004
 * @tc.desc  : Call function SendSmsAck with 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ModeData data;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_1, 0, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_0700
 * @tc.name  : testV1SendSmsAck005
 * @tc.desc  : Call function SendSmsAck with Second 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_0800
 * @tc.name  : testV1SendSmsAck006
 * @tc.desc  : Call function SendSmsAck with Second 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 1, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_0900
 * @tc.name  : testV1SendSmsAck007
 * @tc.desc  : Call function SendSmsAck with Second -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, -1, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1000
 * @tc.name  : testV1SendSmsAck008
 * @tc.desc  : Call function SendSmsAck with Second 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 1000000001, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1100
 * @tc.name  : testV1SendSmsAck009
 * @tc.desc  : Call function SendSmsAck with data.serial as 255.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.serial = 255;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1200
 * @tc.name  : testV1SendSmsAck010
 * @tc.desc  : Call function SendSmsAck with data.serial as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.serial = 1;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1300
 * @tc.name  : testV1SendSmsAck011
 * @tc.desc  : Call function SendSmsAck with data.serial as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.serial = -1;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1400
 * @tc.name  : testV1SendSmsAck012
 * @tc.desc  : Call function SendSmsAck with data.serial as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.serial = 1000000001;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1500
 * @tc.name  : testV1SendSmsAck013
 * @tc.desc  : Call function SendSmsAck with data.pdu as TEST_STORAGE_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.pdu = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1600
 * @tc.name  : testV1SendSmsAck014
 * @tc.desc  : Call function SendSmsAck with data.pdu as TEST_SEND_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1700
 * @tc.name  : testV1SendSmsAck015
 * @tc.desc  : Call function SendSmsAck with data.pdu as TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.pdu = TEST_SMSC_PDU;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1800
 * @tc.name  : testV1SendSmsAck016
 * @tc.desc  : Call function SendSmsAck with data.pdu as TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.pdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_1900
 * @tc.name  : testV1SendSmsAck017
 * @tc.desc  : Call function SendSmsAck with data.pdu as TEST_ID_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.pdu = TEST_ID_LIST;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_2000
 * @tc.name  : testV1SendSmsAck018
 * @tc.desc  : Call function SendSmsAck with data.pdu as TEST_DCS_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.pdu = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_2100
 * @tc.name  : testV1SendSmsAck019
 * @tc.desc  : Call function SendSmsAck with data.result as 1;
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.result = 1;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_2200
 * @tc.name  : testV1SendSmsAck020
 * @tc.desc  : Call function SendSmsAck with data.result as 0;
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.result = 0;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_2300
 * @tc.name  : testV1SendSmsAck021
 * @tc.desc  : Call function SendSmsAck with data.mode as 255;
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.mode = 255;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_2400
 * @tc.name  : testV1SendSmsAck022
 * @tc.desc  : Call function SendSmsAck with data.mode as 1;
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.mode = 1;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_2500
 * @tc.name  : testV1SendSmsAck023
 * @tc.desc  : Call function SendSmsAck with data.mode as -1;
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.mode = -1;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendSmsAck_2600
 * @tc.name  : testV1SendSmsAck024
 * @tc.desc  : Call function SendSmsAck with data.mode as 1000000001;
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendSmsAck024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ModeData data;
    data.mode = 1000000001;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, 255, data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_0300
 * @tc.name: testV1SetRadioState001
 * @tc.desc: SetRadioState, slotId is 0, serialId is GetSerialId(), fun is 0, rst is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 0, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_0400
 * @tc.name: testV1SetRadioState002
 * @tc.desc: SetRadioState, slotId is 0, serialId is GetSerialId(), fun is 4, rst is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 4, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_0500
 * @tc.name: testV1SetRadioState003
 * @tc.desc: SetRadioState, slotId is 0, serialId is GetSerialId(), fun is 0, rst is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_0600
 * @tc.name: testV1SetRadioState004
 * @tc.desc: SetRadioState, slotId is 0, serialId is GetSerialId(), fun is 1, rst is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_0700
 * @tc.name: testV1SetRadioState005
 * @tc.desc: SetRadioState, slotId is 0, fun is 4, rst is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 4, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_0800
 * @tc.name: testV1SetRadioState006
 * @tc.desc: SetRadioState, slotId is 0, serialId is GetSerialId(), fun is 0, rst is 2
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 0, 2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_0900
 * @tc.name: testV1SetRadioState007
 * @tc.desc: SetRadioState, slotId is 0, serialId is GetSerialId(), fun is 3, rst is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 3, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1000
 * @tc.name: testV1SetRadioState008
 * @tc.desc: SetRadioState, slotId is 0, serialId is -1, fun is 1, rst is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, -1, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1100
 * @tc.name: testV1SetRadioState009
 * @tc.desc: SetRadioState, slotId is 0, serialId is 2147483647, fun is 4, rst is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, 2147483647, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1200
 * @tc.name: testV1SetRadioState010
 * @tc.desc: SetRadioState, slotId is 1, serialId is GetSerialId(), fun is 0, rst is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, GetSerialId(), 0, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1300
 * @tc.name: testV1SetRadioState011
 * @tc.desc: SetRadioState, slotId is 1, serialId is GetSerialId(), fun is 4, rst is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, GetSerialId(), 4, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1400
 * @tc.name: testV1SetRadioState012
 * @tc.desc: SetRadioState, slotId is 1, serialId is GetSerialId(), fun is 0, rst is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, GetSerialId(), 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1500
 * @tc.name: testV1SetRadioState013
 * @tc.desc: SetRadioState, slotId is 1, serialId is GetSerialId(), fun is 1, rst is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, GetSerialId(), 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1600
 * @tc.name: testV1SetRadioState014
 * @tc.desc: SetRadioState, slotId is 1, fun is 4, rst is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, GetSerialId(), 4, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1700
 * @tc.name: testV1SetRadioState015
 * @tc.desc: SetRadioState, slotId is 0, serialId is GetSerialId(), fun is 0, rst is 2
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, GetSerialId(), 0, 2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1800
 * @tc.name: testV1SetRadioState016
 * @tc.desc: SetRadioState, slotId is 1, serialId is GetSerialId(), fun is 3, rst is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, GetSerialId(), 3, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_1900
 * @tc.name: testV1SetRadioState017
 * @tc.desc: SetRadioState, slotId is 1, serialId is -1, fun is 1, rst is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, -1, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_2000
 * @tc.name: testV1SetRadioState018
 * @tc.desc: SetRadioState, slotId is 1, serialId is 2147483647, fun is 4, rst is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_2, 2147483647, 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioState_2100
 * @tc.name: testV1SetRadioState019
 * @tc.desc: SetRadioState, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SetRadioState019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 1, 0);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioState_0300
 * @tc.name: testV1GetRadioState001
 * @tc.desc: GetRadioState, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRadioState001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioState_0400
 * @tc.name: testV1GetRadioState002
 * @tc.desc: GetRadioState, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRadioState002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioState_0500
 * @tc.name: testV1GetRadioState003
 * @tc.desc: GetRadioState, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRadioState003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_1, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioState_0600
 * @tc.name: testV1GetRadioState004
 * @tc.desc: GetRadioState, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRadioState004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_2, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}
/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioState_0700
 * @tc.name: testV1GetRadioState005
 * @tc.desc: GetRadioState, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetRadioState005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 1000; i++) {
        ret = g_rilInterface->GetRadioState(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImei_0300
 * @tc.name: testV1GetImei001
 * @tc.desc: GetImei, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImei001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImei_0400
 * @tc.name: testV1GetImei002
 * @tc.desc: GetImei, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImei002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImei_0500
 * @tc.name: testV1GetImei003
 * @tc.desc: GetImei, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImei003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_1, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImei_0600
 * @tc.name: testV1GetImei004
 * @tc.desc: GetImei, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImei004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_2, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImei_0700
 * @tc.name: testV1GetImei005
 * @tc.desc: GetImei, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImei005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetImei(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMeid_0300
 * @tc.name: testV1GetMeid001
 * @tc.desc: GetMeid, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetMeid001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_1, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMeid_0400
 * @tc.name: testV1GetMeid002
 * @tc.desc: GetMeid, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetMeid002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_2, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMeid_0500
 * @tc.name: testV1GetMeid003
 * @tc.desc: GetMeid, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetMeid003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMeid_0600
 * @tc.name: testV1GetMeid004
 * @tc.desc: GetMeid, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetMeid004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetMeid_0700
 * @tc.name: testV1GetMeid005
 * @tc.desc: GetMeid, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetMeid005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetMeid(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetVoiceRadioTechnology_0300
 * @tc.name: testV1GetVoiceRadioTechnology001
 * @tc.desc: GetVoiceRadioTechnology, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetVoiceRadioTechnology001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetVoiceRadioTechnology_0400
 * @tc.name: testV1GetVoiceRadioTechnology002
 * @tc.desc: GetVoiceRadioTechnology, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetVoiceRadioTechnology002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetVoiceRadioTechnology_0500
 * @tc.name: testV1GetVoiceRadioTechnology003
 * @tc.desc: GetVoiceRadioTechnology, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetVoiceRadioTechnology003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_1, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetVoiceRadioTechnology_0600
 * @tc.name: testV1GetVoiceRadioTechnology004
 * @tc.desc: GetVoiceRadioTechnology, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetVoiceRadioTechnology004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_2, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetVoiceRadioTechnology_0700
 * @tc.name: testV1GetVoiceRadioTechnology005
 * @tc.desc: GetVoiceRadioTechnology, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetVoiceRadioTechnology005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetBasebandVersion_0300
 * @tc.name: testV1GetBasebandVersion001
 * @tc.desc: GetBasebandVersion, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetBasebandVersion001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetBasebandVersion_0400
 * @tc.name: testV1GetBasebandVersion002
 * @tc.desc: GetBasebandVersion, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetBasebandVersion002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetBasebandVersion_0500
 * @tc.name: testV1GetBasebandVersion003
 * @tc.desc: GetBasebandVersion, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetBasebandVersion003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_1, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetBasebandVersion_0600
 * @tc.name: testV1GetBasebandVersion004
 * @tc.desc: GetBasebandVersion, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetBasebandVersion004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_2, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetBasebandVersion_0700
 * @tc.name: testV1GetBasebandVersion005
 * @tc.desc: GetBasebandVersion, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetBasebandVersion005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetBasebandVersion(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ShutDown_0300
 * @tc.name: testV1ShutDown001
 * @tc.desc: ShutDown, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1ShutDown001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ShutDown_0400
 * @tc.name: testV1ShutDown002
 * @tc.desc: ShutDown, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1ShutDown002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ShutDown_0500
 * @tc.name: testV1ShutDown003
 * @tc.desc: ShutDown, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1ShutDown003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_1, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ShutDown_0600
 * @tc.name: testV1ShutDown004
 * @tc.desc: ShutDown, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1ShutDown004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_2, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ShutDown_0700
 * @tc.name: testV1ShutDown005
 * @tc.desc: ShutDown, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1ShutDown005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->ShutDown(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_0300
 * @tc.name: testV1GetSimIO001
 * @tc.desc: GetSimIO, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, 2147483647, msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_0400
 * @tc.name: testV1GetSimIO002
 * @tc.desc: GetSimIO, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, 2147483647, msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_0500
 * @tc.name: testV1GetSimIO003
 * @tc.desc: GetSimIO, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, -1, msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_0600
 * @tc.name: testV1GetSimIO004
 * @tc.desc: GetSimIO, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, -1, msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_0700
 * @tc.name: testV1GetSimIO005
 * @tc.desc: GetSimIO, slotId is 0, msg.p1 = 15, msg.p2 = 15
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 15;
    msg.p2 = 15;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_0800
 * @tc.name: testV1GetSimIO006
 * @tc.desc: GetSimIO, slotId is 0, msg.data = "1qaz"
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "1qaz";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_0900
 * @tc.name: testV1GetSimIO007
 * @tc.desc: GetSimIO, slotId is 0, msg.pin2 = "1qaz"
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 15;
    msg.p2 = 15;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "1qaz";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_1000
 * @tc.name: testV1GetSimIO008
 * @tc.desc: GetSimIO, slotId is 1, msg.p1 = 15, msg.p2 = 15
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 15;
    msg.p2 = 15;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_1100
 * @tc.name: testV1GetSimIO009
 * @tc.desc: GetSimIO, slotId is 1, msg.data = "1qaz"
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "1qaz";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_1200
 * @tc.name: testV1GetSimIO010
 * @tc.desc: GetSimIO, slotId is 1, msg.pin2 = "1qaz"
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 15;
    msg.p2 = 15;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "1qaz";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_1300
 * @tc.name: testV1GetSimIO011
 * @tc.desc: GetSimIO, slotId is 0, msg.serial = -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.serial = -1;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_1400
 * @tc.name: testV1GetSimIO012
 * @tc.desc: GetSimIO, slotId is 1, msg.serial = -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.serial = -1;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_1500
 * @tc.name: testV1GetSimIO013
 * @tc.desc: GetSimIO, slotId is 0, msg.serial = 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.serial = 1;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_1600
 * @tc.name: testV1GetSimIO014
 * @tc.desc: GetSimIO, slotId is 1, msg.serial = 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.serial = 1;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_1700
 * @tc.name: testV1GetSimIO015
 * @tc.desc: GetSimIO, slotId is 0, msg.serial = 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.serial = 2147483647;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIO_1800
 * @tc.name: testV1GetSimIO016
 * @tc.desc: GetSimIO, slotId is 1, msg.serial = 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSimIO016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.serial = 2147483647;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImsi_0300
 * @tc.name: testV1GetImsi001
 * @tc.desc: GetImsi, slotId is 0, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImsi001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImsi(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImsi_0400
 * @tc.name: testV1GetImsi002
 * @tc.desc: GetImsi, slotId is 1, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImsi002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImsi(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImsi_0500
 * @tc.name: testV1GetImsi003
 * @tc.desc: GetImsi, slotId is 0, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImsi003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImsi(SLOTID_1, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImsi_0600
 * @tc.name: testV1GetImsi004
 * @tc.desc: GetImsi, slotId is 1, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImsi004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetImsi(SLOTID_2, 2147483647);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetImsi_0700
 * @tc.name: testV1GetImsi005
 * @tc.desc: GetImsi, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetImsi005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetImsi(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfig_0300
 * @tc.name  : testV1GetPhysicalChannelConfig001
 * @tc.desc  : Call function GetPhysicalChannelConfig with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPhysicalChannelConfig001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfig_0400
 * @tc.name  : testV1GetPhysicalChannelConfig002
 * @tc.desc  : Call function GetPhysicalChannelConfig with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPhysicalChannelConfig002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfig_0500
 * @tc.name  : testV1GetPhysicalChannelConfig003
 * @tc.desc  : Call function GetPhysicalChannelConfig with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPhysicalChannelConfig003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfig_0600
 * @tc.name  : testV1GetPhysicalChannelConfig004
 * @tc.desc  : Call function GetPhysicalChannelConfig with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPhysicalChannelConfig004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfig_0700
 * @tc.name  : testV1GetPhysicalChannelConfig005
 * @tc.desc  : Call function GetPhysicalChannelConfig with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPhysicalChannelConfig005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfig_0800
 * @tc.name  : testV1GetPhysicalChannelConfig006
 * @tc.desc  : Call function GetPhysicalChannelConfig with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPhysicalChannelConfig006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfig_0900
 * @tc.name  : testV1GetPhysicalChannelConfig007
 * @tc.desc  : Call function GetPhysicalChannelConfig with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPhysicalChannelConfig007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPhysicalChannelConfig_1000
 * @tc.name  : testV1GetPhysicalChannelConfig008
 * @tc.desc  : Call function GetPhysicalChannelConfig with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPhysicalChannelConfig008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_2, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfo_0300
 * @tc.name  : testV1GetOperatorInfo001
 * @tc.desc  : Call function GetOperatorInfo with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetOperatorInfo001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfo_0400
 * @tc.name  : testV1GetOperatorInfo002
 * @tc.desc  : Call function GetOperatorInfo with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetOperatorInfo002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfo_0500
 * @tc.name  : testV1GetOperatorInfo003
 * @tc.desc  : Call function GetOperatorInfo with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetOperatorInfo003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfo_0600
 * @tc.name  : testV1GetOperatorInfo004
 * @tc.desc  : Call function GetOperatorInfo with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetOperatorInfo004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetOperatorInfo(SLOTID_1, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfo_0700
 * @tc.name  : testV1GetOperatorInfo005
 * @tc.desc  : Call function GetOperatorInfo with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetOperatorInfo005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfo_0800
 * @tc.name  : testV1GetOperatorInfo006
 * @tc.desc  : Call function GetOperatorInfo with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetOperatorInfo006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfo_0900
 * @tc.name  : testV1GetOperatorInfo007
 * @tc.desc  : Call function GetOperatorInfo with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetOperatorInfo007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetOperatorInfo_1000
 * @tc.name  : testV1GetOperatorInfo008
 * @tc.desc  : Call function GetOperatorInfo with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetOperatorInfo008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetOperatorInfo(SLOTID_2, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatus_0300
 * @tc.name  : testV1GetPsRegStatus001
 * @tc.desc  : Call function GetPsRegStatus with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPsRegStatus001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatus_0400
 * @tc.name  : testV1GetPsRegStatus002
 * @tc.desc  : Call function GetPsRegStatus with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPsRegStatus002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatus_0500
 * @tc.name  : testV1GetPsRegStatus003
 * @tc.desc  : Call function GetPsRegStatus with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPsRegStatus003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatus_0600
 * @tc.name  : testV1GetPsRegStatus004
 * @tc.desc  : Call function GetPsRegStatus with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPsRegStatus004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetPsRegStatus(SLOTID_1, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatus_0700
 * @tc.name  : testV1GetPsRegStatus005
 * @tc.desc  : Call function GetPsRegStatus with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPsRegStatus005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatus_0800
 * @tc.name  : testV1GetPsRegStatus006
 * @tc.desc  : Call function GetPsRegStatus with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPsRegStatus006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatus_0900
 * @tc.name  : testV1GetPsRegStatus007
 * @tc.desc  : Call function GetPsRegStatus with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPsRegStatus007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPsRegStatus_1000
 * @tc.name  : testV1GetPsRegStatus008
 * @tc.desc  : Call function GetPsRegStatus with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetPsRegStatus008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetPsRegStatus(SLOTID_2, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatus_0300
 * @tc.name  : testV1GetCsRegStatus001
 * @tc.desc  : Call function GetCsRegStatus with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCsRegStatus001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatus_0400
 * @tc.name  : testV1GetCsRegStatus002
 * @tc.desc  : Call function GetCsRegStatus with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCsRegStatus002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatus_0500
 * @tc.name  : testV1GetCsRegStatus003
 * @tc.desc  : Call function GetCsRegStatus with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCsRegStatus003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatus_0600
 * @tc.name  : testV1GetCsRegStatus004
 * @tc.desc  : Call function GetCsRegStatus with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCsRegStatus004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetCsRegStatus(SLOTID_1, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatus_0700
 * @tc.name  : testV1GetCsRegStatus005
 * @tc.desc  : Call function GetCsRegStatus with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCsRegStatus005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatus_0800
 * @tc.name  : testV1GetCsRegStatus006
 * @tc.desc  : Call function GetCsRegStatus with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCsRegStatus006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatus_0900
 * @tc.name  : testV1GetCsRegStatus007
 * @tc.desc  : Call function GetCsRegStatus with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCsRegStatus007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetCsRegStatus_1000
 * @tc.name  : testV1GetCsRegStatus008
 * @tc.desc  : Call function GetCsRegStatus with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetCsRegStatus008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetCsRegStatus(SLOTID_2, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrength_0300
 * @tc.name  : testV1GetSignalStrength001
 * @tc.desc  : Call function GetSignalStrength with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSignalStrength001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrength_0400
 * @tc.name  : testV1GetSignalStrength002
 * @tc.desc  : Call function GetSignalStrength with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSignalStrength002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrength_0500
 * @tc.name  : testV1GetSignalStrength003
 * @tc.desc  : Call function GetSignalStrength with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSignalStrength003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrength_0600
 * @tc.name  : testV1GetSignalStrength004
 * @tc.desc  : Call function GetSignalStrength with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSignalStrength004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetSignalStrength(SLOTID_1, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrength_0700
 * @tc.name  : testV1GetSignalStrength005
 * @tc.desc  : Call function GetSignalStrength with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSignalStrength005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrength_0800
 * @tc.name  : testV1GetSignalStrength006
 * @tc.desc  : Call function GetSignalStrength with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSignalStrength006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrength_0900
 * @tc.name  : testV1GetSignalStrength007
 * @tc.desc  : Call function GetSignalStrength with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSignalStrength007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSignalStrength_1000
 * @tc.name  : testV1GetSignalStrength008
 * @tc.desc  : Call function GetSignalStrength with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1GetSignalStrength008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetSignalStrength(SLOTID_2, 1000000001);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_0300
 * @tc.name  : testV1UnlockSimLock001
 * @tc.desc  : Call function UnlockSimLock with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock001, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, 1000000001, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_0400
 * @tc.name  : testV1UnlockSimLock002
 * @tc.desc  : Call function UnlockSimLock with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock002, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, 0, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_0500
 * @tc.name  : testV1UnlockSimLock003
 * @tc.desc  : Call function UnlockSimLock with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock003, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, -1, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_0600
 * @tc.name  : testV1UnlockSimLock004
 * @tc.desc  : Call function UnlockSimLock with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock004, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 10; i++) {
        ret = g_rilInterface->UnlockSimLock(SLOTID_1, 1000000001, 0, TEST_STORAGE_PDU);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_0700
 * @tc.name  : testV1UnlockSimLock005
 * @tc.desc  : Call function UnlockSimLock with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock005, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, 1000000001, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_0800
 * @tc.name  : testV1UnlockSimLock006
 * @tc.desc  : Call function UnlockSimLock with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock006, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, 0, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_0900
 * @tc.name  : testV1UnlockSimLock007
 * @tc.desc  : Call function UnlockSimLock with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock007, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, -1, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_1000
 * @tc.name  : testV1UnlockSimLock008
 * @tc.desc  : Call function UnlockSimLock with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock008, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 10; i++) {
        ret = g_rilInterface->UnlockSimLock(SLOTID_2, 1000000001, 0, TEST_STORAGE_PDU);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_1100
 * @tc.name  : testV1UnlockSimLock009
 * @tc.desc  : Call function UnlockSimLock with key as TEST_SEND_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock009, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, GetSerialId(), 0, TEST_SEND_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_1200
 * @tc.name  : testV1UnlockSimLock010
 * @tc.desc  : Call function UnlockSimLock with key as TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock010, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, GetSerialId(), 0, TEST_SMSC_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_1300
 * @tc.name  : testV1UnlockSimLock011
 * @tc.desc  : Call function UnlockSimLock with key as TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock011, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, GetSerialId(), 0, TEST_CDMA_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_1400
 * @tc.name  : testV1UnlockSimLock012
 * @tc.desc  : Call function UnlockSimLock with key as TEST_SMSC_ADDR.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock012, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, GetSerialId(), 0, TEST_SMSC_ADDR);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_1500
 * @tc.name  : testV1UnlockSimLock013
 * @tc.desc  : Call function UnlockSimLock with key as TEST_ID_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock013, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, GetSerialId(), 0, TEST_ID_LIST);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_1600
 * @tc.name  : testV1UnlockSimLock014
 * @tc.desc  : Call function UnlockSimLock with key as TEST_DCS_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock014, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, GetSerialId(), 0, TEST_DCS_LIST);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_1700
 * @tc.name  : testV1UnlockSimLock015
 * @tc.desc  : Call function UnlockSimLock with lockType as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock015, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, GetSerialId(), 1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1UnlockSimLock_1800
 * @tc.name  : testV1UnlockSimLock016
 * @tc.desc  : Call function UnlockSimLock with lockType as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1UnlockSimLock016, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, GetSerialId(), -1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_0300
 * @tc.name  : testV1SimAuthentication001
 * @tc.desc  : Call function SimAuthentication with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_1, 1000000001, simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_0400
 * @tc.name  : testV1SimAuthentication002
 * @tc.desc  : Call function SimAuthentication with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_1, 0, simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_0500
 * @tc.name  : testV1SimAuthentication003
 * @tc.desc  : Call function SimAuthentication with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_1, -1, simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_0600
 * @tc.name  : testV1SimAuthentication004
 * @tc.desc  : Call function SimAuthentication with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimAuthentication(SLOTID_1, 1000000001, simAuthInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_0700
 * @tc.name  : testV1SimAuthentication005
 * @tc.desc  : Call function SimAuthentication with serialId as 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_2, 1000000001, simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_0800
 * @tc.name  : testV1SimAuthentication006
 * @tc.desc  : Call function SimAuthentication with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_2, 0, simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_0900
 * @tc.name  : testV1SimAuthentication007
 * @tc.desc  : Call function SimAuthentication with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_2, -1, simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1000
 * @tc.name  : testV1SimAuthentication008
 * @tc.desc  : Call function SimAuthentication with serialId as 1000000001 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimAuthentication(SLOTID_2, 1000000001, simAuthInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1100
 * @tc.name  : testV1SimAuthentication009
 * @tc.desc  : Call function SimAuthentication with aid as TEST_SEND_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_SEND_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1200
 * @tc.name  : testV1SimAuthentication010
 * @tc.desc  : Call function SimAuthentication with aid as TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_SMSC_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1300
 * @tc.name  : testV1SimAuthentication011
 * @tc.desc  : Call function SimAuthentication with aid as TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_CDMA_PDU;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1400
 * @tc.name  : testV1SimAuthentication012
 * @tc.desc  : Call function SimAuthentication with aid as TEST_SMSC_ADDR.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_SMSC_ADDR;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1500
 * @tc.name  : testV1SimAuthentication013
 * @tc.desc  : Call function SimAuthentication with aid as TEST_ID_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_ID_LIST;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1600
 * @tc.name  : testV1SimAuthentication014
 * @tc.desc  : Call function SimAuthentication with aid as TEST_DCS_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_DCS_LIST;
    simAuthInfo.authData = TEST_STORAGE_PDU;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1700
 * @tc.name  : testV1SimAuthentication015
 * @tc.desc  : Call function SimAuthentication with authData as TEST_SEND_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_SEND_PDU;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1800
 * @tc.name  : testV1SimAuthentication016
 * @tc.desc  : Call function SimAuthentication with authData as TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_SMSC_PDU;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_1900
 * @tc.name  : testV1SimAuthentication017
 * @tc.desc  : Call function SimAuthentication with authData as TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_CDMA_PDU;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_2000
 * @tc.name  : testV1SimAuthentication018
 * @tc.desc  : Call function SimAuthentication with authData as TEST_SMSC_ADDR.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_SMSC_ADDR;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_2100
 * @tc.name  : testV1SimAuthentication019
 * @tc.desc  : Call function SimAuthentication with authData as TEST_ID_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_ID_LIST;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimAuthentication_2200
 * @tc.name  : testV1SimAuthentication020
 * @tc.desc  : Call function SimAuthentication with authData as TEST_DCS_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SimAuthentication020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    SimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = TEST_STORAGE_PDU;
    simAuthInfo.authData = TEST_DCS_LIST;
    ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_0100
 * @tc.name: testV1SendGsmSms001
 * @tc.desc: Call function V1 SendGsmSms, stability test
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        GsmSmsMessageInfo msg;
        msg.smscPdu = TEST_SMSC_PDU;
        msg.pdu = TEST_SEND_PDU;
        ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}
/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_0200
 * @tc.name: testV1SendGsmSms002
 * @tc.desc: Call function V1 SendGsmSms, msg.smscPdu is "+-*%"
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = "+-*%";
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_0300
 * @tc.name: testV1SendGsmSms003
 * @tc.desc: Call function V1 SendGsmSms, msg.pdu is "+-*% "
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = "+-*%";
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}
/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_0400
 * @tc.name: testV1SendGsmSms004
 * @tc.desc: Call function V1 SendGsmSms, msg.smscPdu is "!&#"
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = "!&#";
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_0500
 * @tc.name: testV1SendGsmSms005
 * @tc.desc: Call function V1 SendGsmSms, msg.pdu is "!&#"
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = "!&#";
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_0600
 * @tc.name: testV1SendGsmSms006
 * @tc.desc: Call function V1 SendGsmSms, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, -1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_0700
 * @tc.name: testV1SendGsmSms007
 * @tc.desc: Call function V1 SendGsmSms, serialId is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, 0, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_0800
 * @tc.name: testV1SendGsmSms008
 * @tc.desc: Call function V1 SendGsmSms, serialId is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, 1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_0900
 * @tc.name: testV1SendGsmSms009
 * @tc.desc: Call function V1 SendGsmSms, serialId is 2
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, 2, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1000
 * @tc.name: testV1SendGsmSms010
 * @tc.desc: Call function V1 SendGsmSms, serialId is 3
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, 3, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1100
 * @tc.name: testV1SendGsmSms011
 * @tc.desc: Call function V1 SendGsmSms, serialId is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, -2147483648, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1200
 * @tc.name: testV1SendGsmSms012
 * @tc.desc: Call function V1 SendGsmSms, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, 2147483647, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1300
 * @tc.name: testV1SendGsmSms013
 * @tc.desc: Call function V1 SendGsmSms, msg.serial is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = -1;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1400
 * @tc.name: testV1SendGsmSms014
 * @tc.desc: Call function V1 SendGsmSms, msg.serial is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = 0;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1500
 * @tc.name: testV1SendGsmSms015
 * @tc.desc: Call function V1 SendGsmSms, msg.serial is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = 1;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1600
 * @tc.name: testV1SendGsmSms016
 * @tc.desc: Call function V1 SendGsmSms, msg.serial is 2
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = 2;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1700
 * @tc.name: testV1SendGsmSms017
 * @tc.desc: Call function V1 SendGsmSms, msg.serial is 3
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = 3;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1800
 * @tc.name: testV1SendGsmSms018
 * @tc.desc: Call function V1 SendGsmSms, msg.serial is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = -2147483648;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_1900
 * @tc.name: testV1SendGsmSms019
 * @tc.desc: Call function V1 SendGsmSms, msg.serial is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.serial = 2147483647;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2000
 * @tc.name: testV1SendGsmSms020
 * @tc.desc: Call function V1 SendGsmSms, msg.state is -1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = -1;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2100
 * @tc.name: testV1SendGsmSms021
 * @tc.desc: Call function V1 SendGsmSms, msg.state is 0
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = 0;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2200
 * @tc.name: testV1SendGsmSms022
 * @tc.desc: Call function V1 SendGsmSms, msg.state is 1
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = 1;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2300
 * @tc.name: testV1SendGsmSms023
 * @tc.desc: Call function V1 SendGsmSms, msg.state is 2
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = 2;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2400
 * @tc.name: testV1SendGsmSms024
 * @tc.desc: Call function V1 SendGsmSms, msg.state is 3
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = 3;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2500
 * @tc.name: testV1SendGsmSms025
 * @tc.desc: Call function V1 SendGsmSms, msg.state is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms025, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = -2147483648;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2600
 * @tc.name: testV1SendGsmSms026
 * @tc.desc: Call function V1 SendGsmSms, msg.state is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms026, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.state = 2147483647;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2700
 * @tc.name: testV1SendGsmSms027
 * @tc.desc: Call function V1 SendGsmSms, msg.smscPdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms027, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = "";
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2800
 * @tc.name: testV1SendGsmSms028
 * @tc.desc: Call function V1 SendGsmSms, msg.pdu is ""
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms028, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = "";
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendGsmSms_2900
 * @tc.name: testV1SendGsmSms029
 * @tc.desc: Call function V1 SendGsmSms, msg.smscPdu is "abc"
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendGsmSms029, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    GsmSmsMessageInfo msg;
    msg.smscPdu = "abc";
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_0100
 * @tc.name: testV1SendCdmaSms001
 * @tc.desc: Call function V1 SendCdmaSms,
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendCdmaSms001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        SendCdmaSmsMessageInfo msg;
        msg.smscPdu = TEST_CDMA_PDU;
        ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_0200
 * @tc.name: testV1SendCdmaSms002
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.state is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendCdmaSms002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.state = 2147483647;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_0300
 * @tc.name: testV1SendCdmaSms003
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.smscPdu is null
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendCdmaSms003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = "";
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_0400
 * @tc.name: testV1SendCdmaSms004
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.smscPdu is "!&#"
 */
HWTEST_F(HdfRilHdiTestAdditional2nd, testV1SendCdmaSms004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = "!&#";
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}