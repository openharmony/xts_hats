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
class HdfRilHdiTestAdditional7th : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
/**
** HdfRilHdiTestAdditional7th implement
**/
void HdfRilHdiTestAdditional7th::SetUpTestCase()
{
    g_rilInterface = OHOS::HDI::Ril::V1_3::IRil::Get();
    if (g_rilInterface != nullptr) {
        g_rilInterface->SetCallback1_3(&g_callback);
        g_rilInterface->SendRilAck();
        g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
        g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    }
}

void HdfRilHdiTestAdditional7th::TearDownTestCase() {}
void HdfRilHdiTestAdditional7th::SetUp() {}
void HdfRilHdiTestAdditional7th::TearDown() {}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfo_1100
 * @tc.name  : testV1GetLinkBandwidthInfo009
 * @tc.desc  : Call function GetLinkBandwidthInfo 100 times.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkBandwidthInfo009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 1;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_1, GetSerialId(), 1);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapability_0300
 * @tc.name  : testV1GetLinkCapability001
 * @tc.desc  : Call function GetLinkCapability with slotId is SLOTID_1 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapability001, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapability_0400
 * @tc.name  : testV1GetLinkCapability002
 * @tc.desc  : Call function GetLinkCapability with slotId is SLOTID_1 SerialId is 0.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapability002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapability_0500
 * @tc.name  : testV1GetLinkCapability003
 * @tc.desc  : Call function GetLinkCapability with slotId is SLOTID_1 SerialId is 1.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapability003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapability_0600
 * @tc.name  : testV1GetLinkCapability004
 * @tc.desc  : Call function GetLinkCapability with slotId is SLOTID_1 SerialId is 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapability004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_1, 1000000001);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapability_0700
 * @tc.name  : testV1GetLinkCapability005
 * @tc.desc  : Call function GetLinkCapability with slotId is SLOTID_2 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapability005, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapability_0800
 * @tc.name  : testV1GetLinkCapability006
 * @tc.desc  : Call function GetLinkCapability with slotId is SLOTID_2 SerialId is 0.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapability006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapability_0900
 * @tc.name  : testV1GetLinkCapability007
 * @tc.desc  : Call function GetLinkCapability with slotId is SLOTID_2 SerialId is 1.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapability007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_2, 1);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapability_1000
 * @tc.name  : testV1GetLinkCapability008
 * @tc.desc  : Call function GetLinkCapability with slotId is SLOTID_2 SerialId is 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapability008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_2, 1000000001);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapability_1100
 * @tc.name  : testV1GetLinkCapability009
 * @tc.desc  : Call function GetLinkCapability 100 times.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapability009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->GetLinkCapability(SLOTID_1, GetSerialId());
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_0300
 * @tc.name  : testV1SetSmscAddr001
 * @tc.desc  : Call function SetSmscAddr SerialId is 1.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_1, 1, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_0400
 * @tc.name  : testV1SetSmscAddr002
 * @tc.desc  : Call function SetSmscAddr SerialId is 0.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_1, 0, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_0500
 * @tc.name  : testV1SetSmscAddr003
 * @tc.desc  : Call function SetSmscAddr SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr003, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_1, -1, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_0600
 * @tc.name  : testV1SetSmscAddr004
 * @tc.desc  : Call function SetSmscAddr SerialId is 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_1, 1000000001, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_0700
 * @tc.name  : testV1SetSmscAddr005
 * @tc.desc  : Call function SetSmscAddr Slotid is SLOTID_2 SerialId is 1.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_2, 1, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_0800
 * @tc.name  : testV1SetSmscAddr006
 * @tc.desc  : Call function SetSmscAddr Slotid is SLOTID_2 SerialId is 0.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_2, 0, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_0900
 * @tc.name  : testV1SetSmscAddr007
 * @tc.desc  : Call function SetSmscAddr Slotid is SLOTID_2 SerialId is -1.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr007, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_2, -1, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_1000
 * @tc.name  : testV1SetSmscAddr008
 * @tc.desc  : Call function SetSmscAddr SerialId is 1000000001.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_2, 1000000001, address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_1100
 * @tc.name  : testV1SetSmscAddr009
 * @tc.desc  : Call function SetSmscAddr 1000 times Slotid is SLOTID_1.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = 0;
    for (int i = 0; i < 1000; i++) {
        ret = g_rilInterface->SetSmscAddr(SLOTID_1, 1000000001, address);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSmscAddr_1200
 * @tc.name  : testV1SetSmscAddr010
 * @tc.desc  : Call function SetSmscAddr 1000 times Slotid is SLOTID_2.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSmscAddr010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    ServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = 0;
    int32_t ret = 0;
    for (int i = 0; i < 1000; i++) {
        ret = g_rilInterface->SetSmscAddr(SLOTID_2, 1000000001, address);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfoResponse_0100
 * @tc.name   : testV1SetCallTransferInfoResponse001
 * @tc.desc   : Call the function V1 SetCallTransferInfoResponse with slotId and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetCallTransferInfoResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setCallTransferInfoResponseFlag = false;
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallTransferInfoResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallTransferInfoResponse_0200
 * @tc.name   : testV1SetCallTransferInfoResponse002
 * @tc.desc   : Call the function V1 SetCallTransferInfoResponse with slotId and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetCallTransferInfoResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setCallTransferInfoResponseFlag = false;
    CallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_2, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallTransferInfoResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallRestriction_0100
 * @tc.name   : testV1GetCallRestrictionResponse001
 * @tc.desc   : Call the function V1 GetCallRestrictionResponse with slotId  and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallRestrictionResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCallRestrictionResponseFlag = false;
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_1, GetSerialId(), fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallRestrictionResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallRestriction_0200
 * @tc.name   : testV1GetCallRestrictionResponse002
 * @tc.desc   : Call the function V1 GetCallRestrictionResponse with slotId  and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallRestrictionResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallRestrictionResponseFlag = false;
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_2, GetSerialId(), fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallRestrictionResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestrictionResponse_0100
 * @tc.name   : testV1SetCallRestrictionResponse001
 * @tc.desc   : Call the function V1 SetCallRestrictionResponse with slotId  and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetCallRestrictionResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setCallRestrictionResponseFlag = false;
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallRestrictionResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallRestrictionResponse_0200
 * @tc.name   : testV1SetCallRestrictionResponse002
 * @tc.desc   : Call the function V1 SetCallRestrictionResponse with slotId  and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetCallRestrictionResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setCallRestrictionResponseFlag = false;
    CallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_2, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallRestrictionResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClirResponse_0100
 * @tc.name   : testV1GetClirResponse001
 * @tc.desc   : Call the function V1 GetClirResponse with slotId  and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetClirResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getClirResponseFlag = false;
    int32_t ret = g_rilInterface->GetClir(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getClirResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetClirResponse_0200
 * @tc.name   : testV1GetClirResponse002
 * @tc.desc   : Call the function V1 GetClirResponse with slotId  and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetClirResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getClirResponseFlag = false;
    int32_t ret = g_rilInterface->GetClir(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getClirResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClir_0100
 * @tc.name   : testV1SetClirResponse001
 * @tc.desc   : Call the function V1 SetClirResponse with slotId  and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetClirResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setClirResponseFlag = false;
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setClirResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetClir_0200
 * @tc.name   : testV1SetClirResponse002
 * @tc.desc   : Call the function V1 SetClirResponse with slotId  and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetClirResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setClirResponseFlag = false;
    int32_t ret = g_rilInterface->SetClir(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setClirResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmfResponse_0100
 * @tc.name   : testV1StartDtmfResponse001
 * @tc.desc   : Call the function V1 StartDtmfResponse with slotId and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1StartDtmfResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_startDtmfResponseFlag = false;
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_startDtmfResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StartDtmfResponse_0200
 * @tc.name   : testV1StartDtmfResponse002
 * @tc.desc   : Call the function V1 StartDtmfResponse with slotId and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1StartDtmfResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_startDtmfResponseFlag = false;
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_2, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_startDtmfResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmfResponse_0100
 * @tc.name   : testV1SendDtmfResponse001
 * @tc.desc   : Call the function V1 SendDtmfResponse with slotId  and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SendDtmfResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_sendDtmfResponseFlag = false;
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendDtmfResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SendDtmfResponse_0200
 * @tc.name   : testV1SendDtmfResponse002
 * @tc.desc   : Call the function V1 SendDtmfResponse with slotId  and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SendDtmfResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_sendDtmfResponseFlag = false;
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_2, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_sendDtmfResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmfResponse_0100
 * @tc.name   : testV1StopDtmfResponse001
 * @tc.desc   : Call the function V1 StopDtmfResponse with slotId and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1StopDtmfResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_stopDtmfResponseFlag = false;
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_stopDtmfResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1StopDtmfResponse_0200
 * @tc.name   : testV1StopDtmfResponse002
 * @tc.desc   : Call the function V1 StopDtmfResponse with slotId and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1StopDtmfResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_stopDtmfResponseFlag = false;
    DtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_2, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_stopDtmfResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallPreferenceModeResponse_0100
 * @tc.name   : testV1GetCallPreferenceModeResponse001
 * @tc.desc   : Call the function V1 GetCallPreferenceModeResponse with slotId  and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallPreferenceModeResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCallPreferenceModeResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallPreferenceModeResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallPreferenceModeResponse_0200
 * @tc.name   : testV1GetCallPreferenceModeResponse002
 * @tc.desc   : Call the function V1 GetCallPreferenceModeResponse with slotId  and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallPreferenceModeResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallPreferenceModeResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallPreferenceModeResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceModeResponse_0100
 * @tc.name   : testV1SetCallPreferenceModeResponse001
 * @tc.desc   : Call the function V1 SetCallPreferenceModeResponse with slotId  and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetCallPreferenceModeResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setCallPreferenceModeResponseFlag = false;
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallPreferenceModeResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetCallPreferenceModeResponse_0200
 * @tc.name   : testV1SetCallPreferenceModeResponse002
 * @tc.desc   : Call the function V1 SetCallPreferenceModeResponse with slotId  and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetCallPreferenceModeResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setCallPreferenceModeResponseFlag = false;
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setCallPreferenceModeResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetUssdResponse_0100
 * @tc.name   : testV1SetUssdResponse001
 * @tc.desc   : Call the function V1 SetUssdResponse with slotId  and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetUssdResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setUssdResponseFlag = false;
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, GetSerialId(), "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setUssdResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetUssdResponse_0200
 * @tc.name   : testV1SetUssdResponse002
 * @tc.desc   : Call the function V1 SetUssdResponse with slotId  and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetUssdResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setUssdResponseFlag = false;
    int32_t ret = g_rilInterface->SetUssd(SLOTID_2, GetSerialId(), "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setUssdResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CloseUnFinishedUssdResponse_0100
 * @tc.name   : testV1CloseUnFinishedUssdResponse001
 * @tc.desc   : Call the function V1 CloseUnFinishedUssdResponse with slotId and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1CloseUnFinishedUssdResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_closeUnFinishedUssdResponseFlag = false;
    int32_t ret = g_rilInterface->CloseUnFinishedUssd(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_closeUnFinishedUssdResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1CloseUnFinishedUssdResponse_0200
 * @tc.name   : testV1CloseUnFinishedUssdResponse002
 * @tc.desc   : Call the function V1 CloseUnFinishedUssdResponse with slotId and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1CloseUnFinishedUssdResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_closeUnFinishedUssdResponseFlag = false;
    int32_t ret = g_rilInterface->CloseUnFinishedUssd(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_closeUnFinishedUssdResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallFailReasonResponse_0100
 * @tc.name   : testV1GetCallFailReasonResponse001
 * @tc.desc   : Call the function V1 GetCallFailReasonResponse with slotId  and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallFailReasonResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCallFailReasonResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallFailReasonResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1GetCallFailReasonResponse_0200
 * @tc.name   : testV1GetCallFailReasonResponse002
 * @tc.desc   : Call the function V1 GetCallFailReasonResponse with slotId  and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallFailReasonResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallFailReasonResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallFailReasonResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPasswordResponse_0100
 * @tc.name   : testV1SetBarringPasswordResponse001
 * @tc.desc   : Call the function V1 SetBarringPasswordResponse with slotId and a value of SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetBarringPasswordResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setBarringPasswordResponseFlag = false;
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setBarringPasswordResponseFlag);
}

/**
 * @tc.number : SUB_Telephony_DriverSystem_V1SetBarringPasswordResponse_0200
 * @tc.name   : testV1SetBarringPasswordResponse002
 * @tc.desc   : Call the function V1 SetBarringPasswordResponse with slotId and a value of SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetBarringPasswordResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setBarringPasswordResponseFlag = false;
    SetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_2, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setBarringPasswordResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContextResponse_0100
 * @tc.name  : testV1DeactivatePdpContextResponse001
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1DeactivatePdpContextResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_deactivatePdpContextResponseFlag = false;
    int32_t serialId = 0;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_deactivatePdpContextResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContextResponse_0100
 * @tc.name  : testV1DeactivatePdpContextResponse002
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1DeactivatePdpContextResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_deactivatePdpContextResponseFlag = false;
    int32_t serialId = 0;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_deactivatePdpContextResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextListResponse_0100
 * @tc.name  : testV1GetPdpContextListResponse001
 * @tc.desc  : Obtains the PDP context list. SLOTID_1 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetPdpContextListResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getPdpContextListResponseFlag = false;
    int32_t serialId = 0;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPdpContextListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextListResponse_0200
 * @tc.name  : testV1GetPdpContextListResponse002
 * @tc.desc  : Obtains the PDP context list. SLOTID_2 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetPdpContextListResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getPdpContextListResponseFlag = false;
    int32_t serialId = 0;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getPdpContextListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfoResponse_0100
 * @tc.name  : testV1SetInitApnInfoResponse001
 * @tc.desc  : Call function SetInitApnInfoResponse with slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetInitApnInfoResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setInitApnInfoResponseFlag = false;
    int32_t serialId = 0;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "user1";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "user1";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    int32_t ret = g_rilInterface->SetInitApnInfo(SLOTID_1, serialId, dataProfileInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setInitApnInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfoResponse_0200
 * @tc.name  : testV1SetInitApnInfoResponse002
 * @tc.desc  : Call function SetInitApnInfoResponse with slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetInitApnInfoResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setInitApnInfoResponseFlag = false;
    int32_t serialId = 0;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "user1";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "user1";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    int32_t ret = g_rilInterface->SetInitApnInfo(SLOTID_2, serialId, dataProfileInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setInitApnInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfoResponse_0100
 * @tc.name  : testV1GetLinkBandwidthInfoResponse001
 * @tc.desc  : Call function GetLinkBandwidthInfoResponse with slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkBandwidthInfoResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getLinkBandwidthInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_1, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getLinkBandwidthInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkBandwidthInfoResponse_0200
 * @tc.name  : testV1GetLinkBandwidthInfoResponse002
 * @tc.desc  : Call function GetLinkBandwidthInfoResponse with slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkBandwidthInfoResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getLinkBandwidthInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_2, 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getLinkBandwidthInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapabilityResponse_0100
 * @tc.name  : testV1GetLinkCapabilityResponse001
 * @tc.desc  : Call function GetLinkCapabilityResponse with slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapabilityResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getLinkCapabilityResponseFlag = false;
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getLinkCapabilityResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetLinkCapabilityResponse_0200
 * @tc.name  : testV1GetLinkCapabilityResponse002
 * @tc.desc  : Call function GetLinkCapabilityResponse with slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetLinkCapabilityResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getLinkCapabilityResponseFlag = false;
    int32_t ret = g_rilInterface->GetLinkCapability(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getLinkCapabilityResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetVoiceRadioTechnologyResponse_0100
 * @tc.name: testV1GetVoiceRadioTechnologyResponse001
 * @tc.desc: Call function GetVoiceRadioTechnologyResponse slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetVoiceRadioTechnologyResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getVoiceRadioTechnologyResponseFlag = false;
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getVoiceRadioTechnologyResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetVoiceRadioTechnologyResponse_0200
 * @tc.name: testV1GetVoiceRadioTechnologyResponse002
 * @tc.desc: Call function GetVoiceRadioTechnologyResponse slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetVoiceRadioTechnologyResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getVoiceRadioTechnologyResponseFlag = false;
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getVoiceRadioTechnologyResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIOResponse_0100
 * @tc.name: testV1GetSimIOResponse001
 * @tc.desc: GetSimIOResponse, slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetSimIOResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getSimIOResponseFlag = false;
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSimIOResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetSimIOResponse_0200
 * @tc.name: testV1GetSimIOResponse002
 * @tc.desc: GetSimIOResponse, slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetSimIOResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getSimIOResponseFlag = false;
    SimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSimIOResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLockResponse_0100
 * @tc.name: testV1SetSimLockResponse001
 * @tc.desc: SetSimLockResponse, slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSimLockResponse001, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setSimLockResponseFlag = false;
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 1;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setSimLockResponseFlag);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetSimLockResponse_0200
 * @tc.name: testV1SetSimLockResponse002
 * @tc.desc: SetSimLockResponse, slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetSimLockResponse002, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setSimLockResponseFlag = false;
    SimLockInfo simLockInfo;
    simLockInfo.fac = "AO";
    simLockInfo.mode = 0;
    simLockInfo.classx = 1;
    simLockInfo.passwd = "1234";

    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setSimLockResponseFlag);

#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPasswordResponse_0100
 * @tc.name  : testV1ChangeSimPasswordResponse001
 * @tc.desc  : ChangeSimPasswordResponse, slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ChangeSimPasswordResponse001, Function | MediumTest | Level2)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_changeSimPasswordResponseFlag = false;
    ISimPasswordInfo simPassword;
    simPassword.fac = "AO";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_changeSimPasswordResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ChangeSimPasswordResponse_0200
 * @tc.name  : testV1ChangeSimPasswordResponse002
 * @tc.desc  : ChangeSimPasswordResponse, slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ChangeSimPasswordResponse002, Function | MediumTest | Level2)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_changeSimPasswordResponseFlag = false;
    ISimPasswordInfo simPassword;
    simPassword.fac = "AO";
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_changeSimPasswordResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitch_0300
 * @tc.name  : testV1SetVonrSwitch001
 * @tc.desc  : Call function SetVonrSwitch with slotId is SLOTID_1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitch001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitch_0400
 * @tc.name  : testV1SetVonrSwitch002
 * @tc.desc  : Call function SetVonrSwitch with slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitch002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitch_0500
 * @tc.name  : testV1SetVonrSwitch003
 * @tc.desc  : Call stability SetVonrSwitch with slotId is SLOTID_1 for 100 times
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitch003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetVonrSwitch(SLOTID_1, GetSerialId(), 1);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitch_0600
 * @tc.name  : testV1SetVonrSwitch004
 * @tc.desc  : Call stability SetVonrSwitch with slotId is SLOTID_2 for 100 times
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitch004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetVonrSwitch(SLOTID_2, GetSerialId(), 1);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitch_0700
 * @tc.name  : testV1SetVonrSwitch005
 * @tc.desc  : Call function SetVonrSwitch with slotId is SLOTID_1 Serialid is -1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitch005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_1, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitch_0800
 * @tc.name  : testV1SetVonrSwitch006
 * @tc.desc  : Call function SetVonrSwitch with slotId is SLOTID_2 Serialid is -1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitch006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_2, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_0300
 * @tc.name  : testV1ActivatePdpContext001
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_INVALID
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext001, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_INVALID;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_0400
 * @tc.name  : testV1ActivatePdpContext002
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_UNKNOWN
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_UNKNOWN;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_0500
 * @tc.name  : testV1ActivatePdpContext003
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_GSM;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_0600
 * @tc.name  : testV1ActivatePdpContext004
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_1XRTT
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_1XRTT;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_0700
 * @tc.name  : testV1ActivatePdpContext005
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_WCDMA;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_0800
 * @tc.name  : testV1ActivatePdpContext006
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_HSPA
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_HSPA;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_0900
 * @tc.name  : testV1ActivatePdpContext007
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_HSPAP
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_HSPAP;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_1000
 * @tc.name  : testV1ActivatePdpContext008
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_TD_SCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_TD_SCDMA;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_1100
 * @tc.name  : testV1ActivatePdpContext009
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_EVDO
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_EVDO;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_1200
 * @tc.name  : testV1ActivatePdpContext010
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_EHRPD
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_EHRPD;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_1300
 * @tc.name  : testV1ActivatePdpContext011
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_LTE
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_LTE;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_1400
 * @tc.name  : testV1ActivatePdpContext012
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_LTE_CA
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_LTE_CA;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_1500
 * @tc.name  : testV1ActivatePdpContext013
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_IWLAN
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_IWLAN;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_1600
 * @tc.name  : testV1ActivatePdpContext014
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_NR
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_NR;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1ActivatePdpContext_1700
 * @tc.name  : testV1ActivatePdpContext015
 * @tc.desc  : Call function ActivatePdpContext with radioTechnology is RADIO_TECHNOLOGY_NR and cycle 100 times
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1ActivatePdpContext015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    DataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = RADIO_TECHNOLOGY_NR;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_0300
 * @tc.name  : testV1SetLinkBandwidthReportingRule001
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_UNKNOWN
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_UNKNOWN);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_0400
 * @tc.name  : testV1SetLinkBandwidthReportingRule002
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_GSM
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_GSM);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_0500
 * @tc.name  : testV1SetLinkBandwidthReportingRule003
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_CDMA
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_CDMA);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_0600
 * @tc.name  : testV1SetLinkBandwidthReportingRule004
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_WCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_WCDMA);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_0700
 * @tc.name  : testV1SetLinkBandwidthReportingRule005
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_TDSCDMA
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_TDSCDMA);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_0800
 * @tc.name  : testV1SetLinkBandwidthReportingRule006
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_LTE
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_LTE);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_0900
 * @tc.name  : testV1SetLinkBandwidthReportingRule007
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_NR
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_NR);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_1000
 * @tc.name  : testV1SetLinkBandwidthReportingRule008
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_NR
 *slotId is SLOTID_1 and cycle 100 times
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_NR);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_1100
 * @tc.name  : testV1SetLinkBandwidthReportingRule009
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_UNKNOWN slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_UNKNOWN);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_1200
 * @tc.name  : testV1SetLinkBandwidthReportingRule010
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_GSM slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_GSM);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_1300
 * @tc.name  : testV1SetLinkBandwidthReportingRule011
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_CDMA slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_CDMA);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_1400
 * @tc.name  : testV1SetLinkBandwidthReportingRule012
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_WCDMA slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_WCDMA);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_1500
 * @tc.name  : testV1SetLinkBandwidthReportingRule013
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_TDSCDMA slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_TDSCDMA);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_1600
 * @tc.name  : testV1SetLinkBandwidthReportingRule014
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_LTE slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_LTE);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetLinkBandwidthReportingRule_1700
 * @tc.name  : testV1SetLinkBandwidthReportingRule015
 * @tc.desc  : Call function SetLinkBandwidthReportingRule with rat is NETWORK_TYPE_NR slotId is SLOTID_2
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetLinkBandwidthReportingRule015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();

    const int32_t maxUplinkLinkBandwidih[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000};
    uint32_t uplinkKbpsSize = sizeof(maxUplinkLinkBandwidih) / sizeof(int32_t);
    const int32_t maxDownlinkLinkBandwidih[] = {100,   500,    1000,   5000,   10000,  20000,
                                                50000, 100000, 200000, 500000, 1000000};
    uint32_t downlinkKbpsSize = sizeof(maxDownlinkLinkBandwidih) / sizeof(int32_t);
    DataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_NR);
    dLinkBandwidth.delayMs = 3000;
    dLinkBandwidth.delayUplinkKbps = 50;
    dLinkBandwidth.delayDownlinkKbps = 50;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(maxUplinkLinkBandwidih[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(maxDownlinkLinkBandwidih[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitchResponse_0100
 * @tc.name  : testV1SetVonrSwitchResponse001
 * @tc.desc  : Call function SetVonrSwitchResponse with slotId is SLOTID_1 and status is 1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitchResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setVonrSwitchResponseFlag = false;
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setVonrSwitchResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitchResponse_0200
 * @tc.name  : testV1SetVonrSwitchResponse002
 * @tc.desc  : Call function SetVonrSwitchResponse with slotId is SLOTID_2 and status is 1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitchResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setVonrSwitchResponseFlag = false;
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setVonrSwitchResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitchResponse_0300
 * @tc.name  : testV1SetVonrSwitchResponse003
 * @tc.desc  : Call function SetVonrSwitchResponse with slotId is SLOTID_1 and status is 0
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitchResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_setVonrSwitchResponseFlag = false;
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setVonrSwitchResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetVonrSwitchResponse_0400
 * @tc.name  : testV1SetVonrSwitchResponse004
 * @tc.desc  : Call function SetVonrSwitchResponse with slotId is SLOTID_2 and status is 0
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetVonrSwitchResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setVonrSwitchResponseFlag = false;
    int32_t ret = g_rilInterface->SetVonrSwitch(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setVonrSwitchResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetSimLockStatusResponse_0100
 * @tc.name  : testV1GetSimLockStatusResponse001
 * @tc.desc  : Call function GetSimLockStatus with SLOTID_1 simLockStatus is 1 classx is 255 fac is "OI" mode is 0;
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetSimLockStatusResponse001, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getSimLockStatusResponseFlag = false;
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 0;
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSimLockStatusResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetSimLockStatusResponse_0200
 * @tc.name  : testV1GetSimLockStatusResponse002
 * @tc.desc  : Call function GetSimLockStatus with SLOTID_2 simLockStatus is 1 classx is 255 fac is "OI" mode is 1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetSimLockStatusResponse002, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getSimLockStatusResponseFlag = false;
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "OI";
    simLockInfo.mode = 1;
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSimLockStatusResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetSimLockStatusResponse_0300
 * @tc.name  : testV1GetSimLockStatusResponse003
 * @tc.desc  : Call function GetSimLockStatus with SLOTID_1 SerialId is -1 fac is "SC" mode is 2 classx is 255
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetSimLockStatusResponse003, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getSimLockStatusResponseFlag = false;
    SimLockInfo simLockInfo;
    simLockInfo.fac = "SC";
    simLockInfo.mode = 2;
    simLockInfo.classx = 255;
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, -1, simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(ture, g_getSimLockStatusResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetSimLockStatusResponse_0400
 * @tc.name  : testV1GetSimLockStatusResponse004
 * @tc.desc  : Call function GetSimLockStatus with SLOTID_2 SerialId is -1 classx is 255 fac is "AB" mode is 1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetSimLockStatusResponse004, Function | MediumTest | Level1)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getSimLockStatusResponseFlag = false;
    SimLockInfo simLockInfo;
    simLockInfo.classx = 255;
    simLockInfo.fac = "AB";
    simLockInfo.mode = 1;
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, -1, simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getSimLockStatusResponseFlag);
#endif
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetCallTransferInfoResponse_0100
 * @tc.name  : testV1GetCallTransferInfoResponse001
 * @tc.desc  : Call function GetCallTransferInfo with SLOTID_2 SerialId is -1 a value of 1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallTransferInfoResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallTransferInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_2, -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallTransferInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetCallTransferInfoResponse_0200
 * @tc.name  : testV1GetCallTransferInfoResponse002
 * @tc.desc  : Call function GetCallTransferInfo with SLOTID_2 and a value of -1
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallTransferInfoResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallTransferInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_2, GetSerialId(), -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallTransferInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetCallTransferInfoResponse_0300
 * @tc.name  : testV1GetCallTransferInfoResponse003
 * @tc.desc  : Call function GetCallTransferInfo with SLOTID_1 a value of 65535
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallTransferInfoResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_getCallTransferInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, GetSerialId(), 65535);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallTransferInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetCallTransferInfoResponse_0400
 * @tc.name  : testV1GetCallTransferInfoResponse004
 * @tc.desc  : Call function GetCallTransferInfo with SLOTID_2 a value of 4555
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallTransferInfoResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallTransferInfoResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_2, GetSerialId(), 4555);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallTransferInfoResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1CombineConferenceResponse_0100
 * @tc.name  : testV1CombineConferenceResponse001
 * @tc.desc  : Call function CombineConference with SLOTID_2 a value of -32768999
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1CombineConferenceResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_combineConferenceResponseFlag = false;
    int32_t ret = g_rilInterface->CombineConference(SLOTID_2, GetSerialId(), -32768999);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_combineConferenceResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1CombineConferenceResponse_0200
 * @tc.name  : testV1CombineConferenceResponse002
 * @tc.desc  : Call function CombineConference with SLOTID_2 a value of 32768999
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1CombineConferenceResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_combineConferenceResponseFlag = false;
    int32_t ret = g_rilInterface->CombineConference(SLOTID_2, GetSerialId(), 32768999);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_combineConferenceResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1CombineConferenceResponse_0300
 * @tc.name  : testV1CombineConferenceResponse003
 * @tc.desc  : Call function CombineConference with SLOTID_2  SerialId is 1111 a value of 32768999
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1CombineConferenceResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_combineConferenceResponseFlag = false;
    int32_t ret = g_rilInterface->CombineConference(SLOTID_2, 1111, 32768999);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_combineConferenceResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1CombineConferenceResponse_0400
 * @tc.name  : testV1CombineConferenceResponse004
 * @tc.desc  : Call function CombineConference with SLOTID_1 and a value of 32768999
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1CombineConferenceResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_combineConferenceResponseFlag = false;
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, GetSerialId(), 32768999);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_combineConferenceResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetCallListResponse_0300
 * @tc.name  : testV1GetCallListResponse003
 * @tc.desc  : Call function GetCallList with SLOTID_2 and a value of -11111
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallListResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallListResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallList(SLOTID_2, -11111);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetCallListResponse_0400
 * @tc.name  : testV1GetCallListResponse004
 * @tc.desc  : Call function GetCallList with SLOTID_2 and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetCallListResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getCallListResponseFlag = false;
    int32_t ret = g_rilInterface->GetCallList(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getCallListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetEmergencyCallListResponse_0300
 * @tc.name  : testV1GetEmergencyCallListResponse003
 * @tc.desc  : Call function GetEmergencyCallList with SLOTID_2 and a value of 0
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetEmergencyCallListResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getEmergencyCallListResponseFlag = false;
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getEmergencyCallListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1GetEmergencyCallListResponse_0400
 * @tc.name  : testV1GetEmergencyCallListResponse004
 * @tc.desc  : Call function GetEmergencyCallList with SLOTID_2 and a value of 32768
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1GetEmergencyCallListResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_getEmergencyCallListResponseFlag = false;
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_2, 32768);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_getEmergencyCallListResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1SetEmergencyCallListResponse_0300
 * @tc.name  : testV1SetEmergencyCallListResponse003
 * @tc.desc  : Call function GetEmergencyCallList with SerialID is 65535
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetEmergencyCallListResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setEmergencyCallListResponse = false;
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
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_2, 65535, emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setEmergencyCallListResponse);
}

/**
 * @tc.number: SUB_Telephony_Driversystem_V1SetEmergencyCallListResponse_0400
 * @tc.name  : testV1SetEmergencyCallListResponse004
 * @tc.desc  : Call function GetEmergencyCallList with SerialID is -65535
 */
HWTEST_F(HdfRilHdiTestAdditional7th, testV1SetEmergencyCallListResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_setEmergencyCallListResponse = false;
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
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_2, -65535, emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_setEmergencyCallListResponse);
}