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
class HdfRilHdiTestAdditional3rd : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
/**
** HdfRilHdiTestAdditional3rd implement
**/
void HdfRilHdiTestAdditional3rd::SetUpTestCase()
{
    g_rilInterface = OHOS::HDI::Ril::V1_2::IRil::Get();
    if (g_rilInterface != nullptr) {
        g_rilInterface->SetCallback1_2(&g_callback);
        g_rilInterface->SendRilAck();
        g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
        g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    }
}

void HdfRilHdiTestAdditional3rd::TearDownTestCase() {}
void HdfRilHdiTestAdditional3rd::SetUp() {}
void HdfRilHdiTestAdditional3rd::TearDown() {}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_0500
 * @tc.name: testV1SendCdmaSms005
 * @tc.desc: Call function V1 SendCdmaSms, serialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, -1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_0600
 * @tc.name: testV1SendCdmaSms006
 * @tc.desc: Call function V1 SendCdmaSms, serialId is 0
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, 0, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_0700
 * @tc.name: testV1SendCdmaSms007
 * @tc.desc: Call function V1 SendCdmaSms, serialId is 1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, 1, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_0800
 * @tc.name: testV1SendCdmaSms008
 * @tc.desc: Call function V1 SendCdmaSms, serialId is 2
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, 2, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_0900
 * @tc.name: testV1SendCdmaSms009
 * @tc.desc: Call function V1 SendCdmaSms, serialId is 3
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, 3, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1000
 * @tc.name: testV1SendCdmaSms010
 * @tc.desc: Call function V1 SendCdmaSms, serialId is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, -2147483648, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1100
 * @tc.name: testV1SendCdmaSms011
 * @tc.desc: Call function V1 SendCdmaSms, serialId is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, 2147483647, msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1200
 * @tc.name: testV1SendCdmaSms012
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.serial is -1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.serial = -1;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1300
 * @tc.name: testV1SendCdmaSms013
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.serial is 0
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.serial = 0;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1400
 * @tc.name: testV1SendCdmaSms014
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.serial is 1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.serial = 1;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1500
 * @tc.name: testV1SendCdmaSms015
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.serial is 2
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.serial = 2;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1600
 * @tc.name: testV1SendCdmaSms016
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.serial is 3
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.serial = 3;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1700
 * @tc.name: testV1SendCdmaSms017
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.serial is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.serial = -2147483648;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1800
 * @tc.name: testV1SendCdmaSms018
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.serial is 2147483647
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.serial = 2147483647;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_1900
 * @tc.name: testV1SendCdmaSms019
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.smscPdu is "+-*%"
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = "+-*%";
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_2000
 * @tc.name: testV1SendCdmaSms020
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.smscPdu is "123"
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.smscPdu = "123";
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_2100
 * @tc.name: testV1SendCdmaSms021
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.state is -1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.state = -1;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_2200
 * @tc.name: testV1SendCdmaSms022
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.state is 0
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.state = 0;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_2300
 * @tc.name: testV1SendCdmaSms023
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.state is 1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.state = 1;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_2400
 * @tc.name: testV1SendCdmaSms024
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.state is 2
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.state = 2;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_2500
 * @tc.name: testV1SendCdmaSms025
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.state is 3
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms025, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.state = 3;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendCdmaSms_2600
 * @tc.name: testV1SendCdmaSms026
 * @tc.desc: Call function V1 SendCdmaSms, slotId is 0, msg.state is -2147483648
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendCdmaSms026, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    SendCdmaSmsMessageInfo msg;
    msg.state = -2147483648;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_0300
 * @tc.name  : testV1SetActiveSim001
 * @tc.desc  : Call function SetActiveSim 100 times
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 1;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), 1, 1);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_0400
 * @tc.name  : testV1SetActiveSim002
 * @tc.desc  : Call function SetActiveSim with slotId is SLOTID_1 SerialId is 1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, 1, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_0500
 * @tc.name  : testV1SetActiveSim003
 * @tc.desc  : Call function SetActiveSim with slotId is SLOTID_1 SerialId is 0
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, 0, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_0600
 * @tc.name  : testV1SetActiveSim004
 * @tc.desc  : Call function SetActiveSim with slotId is SLOTID_1 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim004, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, -1, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_0700
 * @tc.name  : testV1SetActiveSim005
 * @tc.desc  : Call function SetActiveSim with slotId is SLOTID_2 SerialId is 1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, 1, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_0800
 * @tc.name  : testV1SetActiveSim006
 * @tc.desc  : Call function SetActiveSim with slotId is SLOTID_2 SerialId is 0
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, 0, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_0900
 * @tc.name  : testV1SetActiveSim007
 * @tc.desc  : Call function SetActiveSim with slotId is SLOTID_2 SerialId is 1000000001
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, 1000000001, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_1000
 * @tc.name  : testV1SetActiveSim008
 * @tc.desc  : Call function SetActiveSim with slotId is SLOTID_2 SerialId is -1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim008, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, -1, 1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_1100
 * @tc.name  : testV1SetActiveSim009
 * @tc.desc  : Call function SetActiveSim with slotId is SLOTID_1 the third argument as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_1200
 * @tc.name  : testV1SetActiveSim010
 * @tc.desc  : Call function SetActiveSim with slotId is SLOTID_2 the third argument as 0
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, GetSerialId(), 0, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_1300
 * @tc.name  : testV1SetActiveSim011
 * @tc.desc  : Call function SetActiveSim with the third argument as 1000000001
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), 1000000001, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_1400
 * @tc.name  : testV1SetActiveSim012
 * @tc.desc  : Call function SetActiveSim with the third argument as -1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim012, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), -1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_1500
 * @tc.name  : testV1SetActiveSim013
 * @tc.desc  : Call function SetActiveSim with the fourth argument as 0
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_1600
 * @tc.name  : testV1SetActiveSim014
 * @tc.desc  : Call function SetActiveSim with the fourth argument as -1
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim014, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), 1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetActiveSim_1700
 * @tc.name  : testV1SetActiveSim015
 * @tc.desc  : Call function SetActiveSim with the fourth argument as 2
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetActiveSim015, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), 1, 2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_0300
 * @tc.name  : testV1SimTransmitApduBasicChannel001
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel001, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, -1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_0400
 * @tc.name  : testV1SimTransmitApduBasicChannel002
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, 0, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_0500
 * @tc.name  : testV1SimTransmitApduBasicChannel003
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_0600
 * @tc.name  : testV1SimTransmitApduBasicChannel004
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serialId as 1 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, 1, reqInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_0700
 * @tc.name  : testV1SimTransmitApduBasicChannel005
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel005, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_2, -1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_0800
 * @tc.name  : testV1SimTransmitApduBasicChannel006
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_2, 0, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_0900
 * @tc.name  : testV1SimTransmitApduBasicChannel007
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_2, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_1000
 * @tc.name  : testV1SimTransmitApduBasicChannel008
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serialId as 1 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_2, 1, reqInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_1100
 * @tc.name  : testV1SimTransmitApduBasicChannel009
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serial as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel009, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = -1;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_1200
 * @tc.name  : testV1SimTransmitApduBasicChannel010
 * @tc.desc  : Call function SimTransmitApduBasicChannel with serial as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 1;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_1300
 * @tc.name  : testV1SimTransmitApduBasicChannel011
 * @tc.desc  : Call function SimTransmitApduBasicChannel with channelId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel011, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = -1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduBasicChannel_1400
 * @tc.name  : testV1SimTransmitApduBasicChannel012
 * @tc.desc  : Call function SimTransmitApduBasicChannel with channelId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduBasicChannel012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_0300
 * @tc.name  : testV1SimTransmitApduLogicalChannel001
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel001, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, -1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_0400
 * @tc.name  : testV1SimTransmitApduLogicalChannel002
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, 0, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_0500
 * @tc.name  : testV1SimTransmitApduLogicalChannel003
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_0600
 * @tc.name  : testV1SimTransmitApduLogicalChannel004
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serialId as 1 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, 1, reqInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_0700
 * @tc.name  : testV1SimTransmitApduLogicalChannel005
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel005, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_2, -1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_0800
 * @tc.name  : testV1SimTransmitApduLogicalChannel006
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_2, 0, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_0900
 * @tc.name  : testV1SimTransmitApduLogicalChannel007
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_2, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_1000
 * @tc.name  : testV1SimTransmitApduLogicalChannel008
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serialId as 1 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_2, 1, reqInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_1100
 * @tc.name  : testV1SimTransmitApduLogicalChannel009
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serial as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel009, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = -1;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_1200
 * @tc.name  : testV1SimTransmitApduLogicalChannel010
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with serial as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 1;
    reqInfo.channelId = 0;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_1300
 * @tc.name  : testV1SimTransmitApduLogicalChannel011
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with channelId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel011, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = -1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimTransmitApduLogicalChannel_1400
 * @tc.name  : testV1SimTransmitApduLogicalChannel012
 * @tc.desc  : Call function SimTransmitApduLogicalChannel with channelId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimTransmitApduLogicalChannel012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    ApduSimIORequestInfo reqInfo;
    reqInfo.serial = 0;
    reqInfo.channelId = 1;
    reqInfo.type = 0x80;
    reqInfo.instruction = 0xCA;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, 1, reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_0300
 * @tc.name  : testV1SimStkSendTerminalResponse001
 * @tc.desc  : Call function SimStkSendTerminalResponse with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, 1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_0400
 * @tc.name  : testV1SimStkSendTerminalResponse002
 * @tc.desc  : Call function SimStkSendTerminalResponse with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, -1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_0500
 * @tc.name  : testV1SimStkSendTerminalResponse003
 * @tc.desc  : Call function SimStkSendTerminalResponse with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_0600
 * @tc.name  : testV1SimStkSendTerminalResponse004
 * @tc.desc  : Call function SimStkSendTerminalResponse with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, 0, TEST_STORAGE_PDU);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_0700
 * @tc.name  : testV1SimStkSendTerminalResponse005
 * @tc.desc  : Call function SimStkSendTerminalResponse with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_2, 1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_0800
 * @tc.name  : testV1SimStkSendTerminalResponse006
 * @tc.desc  : Call function SimStkSendTerminalResponse with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_2, -1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_0900
 * @tc.name  : testV1SimStkSendTerminalResponse007
 * @tc.desc  : Call function SimStkSendTerminalResponse with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_2, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_1000
 * @tc.name  : testV1SimStkSendTerminalResponse008
 * @tc.desc  : Call function SimStkSendTerminalResponse with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_2, 0, TEST_STORAGE_PDU);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_1100
 * @tc.name  : testV1SimStkSendTerminalResponse009
 * @tc.desc  : Call function SimStkSendTerminalResponse with StrCmd as TEST_SEND_PDU .
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, 1, TEST_SEND_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_1200
 * @tc.name  : testV1SimStkSendTerminalResponse010
 * @tc.desc  : Call function SimStkSendTerminalResponse with StrCmd as TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, 1, TEST_SMSC_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_1300
 * @tc.name  : testV1SimStkSendTerminalResponse011
 * @tc.desc  : Call function SimStkSendTerminalResponse with StrCmd as TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, 1, TEST_CDMA_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_1400
 * @tc.name  : testV1SimStkSendTerminalResponse012
 * @tc.desc  : Call function SimStkSendTerminalResponse with StrCmd as TEST_SMSC_ADDR.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, 1, TEST_SMSC_ADDR);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_1500
 * @tc.name  : testV1SimStkSendTerminalResponse013
 * @tc.desc  : Call function SimStkSendTerminalResponse with StrCmd as TEST_ID_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, 1, TEST_ID_LIST);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendTerminalResponse_1600
 * @tc.name  : testV1SimStkSendTerminalResponse014
 * @tc.desc  : Call function SimStkSendTerminalResponse with StrCmd as TEST_DCS_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendTerminalResponse014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, 1, TEST_DCS_LIST);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_0300
 * @tc.name  : testV1SimStkSendEnvelope001
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, 1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_0400
 * @tc.name  : testV1SimStkSendEnvelope002
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, -1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_0500
 * @tc.name  : testV1SimStkSendEnvelope003
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_0600
 * @tc.name  : testV1SimStkSendEnvelope004
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, 0, TEST_STORAGE_PDU);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_0700
 * @tc.name  : testV1SimStkSendEnvelope005
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_2, 1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_0800
 * @tc.name  : testV1SimStkSendEnvelope006
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_2, -1, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_0900
 * @tc.name  : testV1SimStkSendEnvelope007
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_2, 0, TEST_STORAGE_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_1000
 * @tc.name  : testV1SimStkSendEnvelope008
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimStkSendEnvelope(SLOTID_2, 0, TEST_STORAGE_PDU);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_1100
 * @tc.name  : testV1SimStkSendEnvelope009
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as TEST_SEND_PDU .
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, 1, TEST_SEND_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_1200
 * @tc.name  : testV1SimStkSendEnvelope010
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as TEST_SMSC_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, 1, TEST_SMSC_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_1300
 * @tc.name  : testV1SimStkSendEnvelope011
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as TEST_CDMA_PDU.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, 1, TEST_CDMA_PDU);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_1400
 * @tc.name  : testV1SimStkSendEnvelope012
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as TEST_SMSC_ADDR.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, 1, TEST_SMSC_ADDR);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_1500
 * @tc.name  : testV1SimStkSendEnvelope013
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as TEST_ID_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, 1, TEST_ID_LIST);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendEnvelope_1600
 * @tc.name  : testV1SimStkSendEnvelope014
 * @tc.desc  : Call function SimStkSendEnvelope with SerialId as TEST_DCS_LIST.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendEnvelope014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, 1, TEST_DCS_LIST);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResult_0300
 * @tc.name  : testV1SimStkSendCallSetupRequestResult001
 * @tc.desc  : Call function SimStkSendCallSetupRequestResult with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendCallSetupRequestResult001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_1, 1, true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResult_0400
 * @tc.name  : testV1SimStkSendCallSetupRequestResult002
 * @tc.desc  : Call function SimStkSendCallSetupRequestResult with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendCallSetupRequestResult002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_1, -1, true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResult_0500
 * @tc.name  : testV1SimStkSendCallSetupRequestResult003
 * @tc.desc  : Call function SimStkSendCallSetupRequestResult with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendCallSetupRequestResult003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_1, 0, true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResult_0600
 * @tc.name  : testV1SimStkSendCallSetupRequestResult004
 * @tc.desc  : Call function SimStkSendCallSetupRequestResult with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendCallSetupRequestResult004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_1, 0, true);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResult_0700
 * @tc.name  : testV1SimStkSendCallSetupRequestResult005
 * @tc.desc  : Call function SimStkSendCallSetupRequestResult with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendCallSetupRequestResult005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_2, 1, true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResult_0800
 * @tc.name  : testV1SimStkSendCallSetupRequestResult006
 * @tc.desc  : Call function SimStkSendCallSetupRequestResult with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendCallSetupRequestResult006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_2, -1, true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResult_0900
 * @tc.name  : testV1SimStkSendCallSetupRequestResult007
 * @tc.desc  : Call function SimStkSendCallSetupRequestResult with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendCallSetupRequestResult007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_2, 0, true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkSendCallSetupRequestResult_1000
 * @tc.name  : testV1SimStkSendCallSetupRequestResult008
 * @tc.desc  : Call function SimStkSendCallSetupRequestResult with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkSendCallSetupRequestResult008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_2, 0, true);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReady_0300
 * @tc.name  : testV1SimStkIsReady001
 * @tc.desc  : Call function SimStkIsReady with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkIsReady001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReady_0400
 * @tc.name  : testV1SimStkIsReady002
 * @tc.desc  : Call function SimStkIsReady with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkIsReady002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_1, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReady_0500
 * @tc.name  : testV1SimStkIsReady003
 * @tc.desc  : Call function SimStkIsReady with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkIsReady003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReady_0600
 * @tc.name  : testV1SimStkIsReady004
 * @tc.desc  : Call function SimStkIsReady with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkIsReady004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimStkIsReady(SLOTID_1, 0);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReady_0700
 * @tc.name  : testV1SimStkIsReady005
 * @tc.desc  : Call function SimStkIsReady with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkIsReady005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_2, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReady_0800
 * @tc.name  : testV1SimStkIsReady006
 * @tc.desc  : Call function SimStkIsReady with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkIsReady006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_2, -1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReady_0900
 * @tc.name  : testV1SimStkIsReady007
 * @tc.desc  : Call function SimStkIsReady with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkIsReady007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_2, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimStkIsReady_1000
 * @tc.name  : testV1SimStkIsReady008
 * @tc.desc  : Call function SimStkIsReady with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimStkIsReady008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimStkIsReady(SLOTID_2, 0);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocol_0300
 * @tc.name  : testV1SetRadioProtocol001
 * @tc.desc  : Call function SetRadioProtocol with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetRadioProtocol001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    RadioProtocol protocol;
    protocol.slotId = SLOTID_1;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_1, 1, protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocol_0400
 * @tc.name  : testV1SetRadioProtocol002
 * @tc.desc  : Call function SetRadioProtocol with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetRadioProtocol002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    RadioProtocol protocol;
    protocol.slotId = SLOTID_1;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_1, -1, protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocol_0500
 * @tc.name  : testV1SetRadioProtocol003
 * @tc.desc  : Call function SetRadioProtocol with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetRadioProtocol003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    RadioProtocol protocol;
    protocol.slotId = SLOTID_1;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_1, 0, protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocol_0600
 * @tc.name  : testV1SetRadioProtocol004
 * @tc.desc  : Call function SetRadioProtocol with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetRadioProtocol004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    RadioProtocol protocol;
    protocol.slotId = SLOTID_1;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetRadioProtocol(SLOTID_1, 0, protocol);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocol_0700
 * @tc.name  : testV1SetRadioProtocol005
 * @tc.desc  : Call function SetRadioProtocol with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetRadioProtocol005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    RadioProtocol protocol;
    protocol.slotId = SLOTID_2;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_2, 1, protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocol_0800
 * @tc.name  : testV1SetRadioProtocol006
 * @tc.desc  : Call function SetRadioProtocol with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetRadioProtocol006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    RadioProtocol protocol;
    protocol.slotId = SLOTID_2;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_2, -1, protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocol_0900
 * @tc.name  : testV1SetRadioProtocol007
 * @tc.desc  : Call function SetRadioProtocol with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetRadioProtocol007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    RadioProtocol protocol;
    protocol.slotId = SLOTID_2;
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_2, 0, protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetRadioProtocol_1000
 * @tc.name  : testV1SetRadioProtocol008
 * @tc.desc  : Call function SetRadioProtocol with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetRadioProtocol008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    RadioProtocol protocol;
    protocol.slotId = SLOTID_2;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SetRadioProtocol(SLOTID_2, 0, protocol);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetRadioProtocol_0100
 * @tc.name  : testV1GetRadioProtocol001
 * @tc.desc  : Call function GetRadioProtocol with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetRadioProtocol001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetRadioProtocol(SLOTID_1, 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannel_0300
 * @tc.name  : testV1SimOpenLogicalChannel001
 * @tc.desc  : Call function SimOpenLogicalChannel with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimOpenLogicalChannel001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_1, 1, appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannel_0400
 * @tc.name  : testV1SimOpenLogicalChannel002
 * @tc.desc  : Call function SimOpenLogicalChannel with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimOpenLogicalChannel002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_1, -1, appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannel_0500
 * @tc.name  : testV1SimOpenLogicalChannel003
 * @tc.desc  : Call function SimOpenLogicalChannel with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimOpenLogicalChannel003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_1, 0, appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannel_0600
 * @tc.name  : testV1SimOpenLogicalChannel004
 * @tc.desc  : Call function SimOpenLogicalChannel with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimOpenLogicalChannel004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_1, 0, appID, p2);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannel_0700
 * @tc.name  : testV1SimOpenLogicalChannel005
 * @tc.desc  : Call function SimOpenLogicalChannel with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimOpenLogicalChannel005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_2, 1, appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannel_0800
 * @tc.name  : testV1SimOpenLogicalChannel006
 * @tc.desc  : Call function SimOpenLogicalChannel with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimOpenLogicalChannel006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_2, -1, appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannel_0900
 * @tc.name  : testV1SimOpenLogicalChannel007
 * @tc.desc  : Call function SimOpenLogicalChannel with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimOpenLogicalChannel007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_2, 0, appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimOpenLogicalChannel_1000
 * @tc.name  : testV1SimOpenLogicalChannel008
 * @tc.desc  : Call function SimOpenLogicalChannel with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimOpenLogicalChannel008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    std::string appID = "A00000015141434C00";
    int32_t p2 = 0;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_2, 0, appID, p2);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannel_0300
 * @tc.name  : testV1SimCloseLogicalChannel001
 * @tc.desc  : Call function SimCloseLogicalChannel with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimCloseLogicalChannel001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_1, 1, g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannel_0400
 * @tc.name  : testV1SimCloseLogicalChannel002
 * @tc.desc  : Call function SimCloseLogicalChannel with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimCloseLogicalChannel002, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_1, -1, g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannel_0500
 * @tc.name  : testV1SimCloseLogicalChannel003
 * @tc.desc  : Call function SimCloseLogicalChannel with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimCloseLogicalChannel003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_1, 0, g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannel_0600
 * @tc.name  : testV1SimCloseLogicalChannel004
 * @tc.desc  : Call function SimCloseLogicalChannel with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimCloseLogicalChannel004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_1, 0, g_currentChannelld);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannel_0700
 * @tc.name  : testV1SimCloseLogicalChannel005
 * @tc.desc  : Call function SimCloseLogicalChannel with SerialId as 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimCloseLogicalChannel005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_2, 1, g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannel_0800
 * @tc.name  : testV1SimCloseLogicalChannel006
 * @tc.desc  : Call function SimCloseLogicalChannel with SerialId as -1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimCloseLogicalChannel006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_2, -1, g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannel_0900
 * @tc.name  : testV1SimCloseLogicalChannel007
 * @tc.desc  : Call function SimCloseLogicalChannel with SerialId as 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimCloseLogicalChannel007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_2, 0, g_currentChannelld);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SimCloseLogicalChannel_1000
 * @tc.name  : testV1SimCloseLogicalChannel008
 * @tc.desc  : Call function SimCloseLogicalChannel with SerialId as 0 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SimCloseLogicalChannel008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_2, 0, g_currentChannelld);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DialResponse_0300
 * @tc.name  : testV1DialResponse001
 * @tc.desc  : Call function DialResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DialResponse001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_dialResponseFlag = false;
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = 0;
    ret = g_rilInterface->Dial(SLOTID_1, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_dialResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DialResponse_0400
 * @tc.name  : testV1DialResponse002
 * @tc.desc  : Call function DialResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DialResponse002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_dialResponseFlag = false;
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = 0;
    ret = g_rilInterface->Dial(SLOTID_2, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_dialResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DialResponse_0500
 * @tc.name  : testV1DialResponse003
 * @tc.desc  : Call function DialResponse 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DialResponse003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        g_dialResponseFlag = false;
        ret = g_rilInterface->Dial(SLOTID_1, GetSerialId(), dialInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
        EXPECT_EQ(true, g_dialResponseFlag);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DialResponse_0600
 * @tc.name  : testV1DialResponse004
 * @tc.desc  : Call function DialResponse 100times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DialResponse004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_dialResponseFlag = false;
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_rilInterface->Dial(SLOTID_2, GetSerialId(), dialInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
        EXPECT_EQ(true, g_dialResponseFlag);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DialResponse_0700
 * @tc.name  : testV1DialResponse005
 * @tc.desc  : Call function DialResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DialResponse005, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_dialResponseFlag = false;
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = 0;
    ret = g_rilInterface->Dial(SLOTID_1, -1, dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_NE(true, g_dialResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DialResponse_0800
 * @tc.name  : testV1DialResponse006
 * @tc.desc  : Call function DialResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DialResponse006, Function | MediumTest | Level2)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_dialResponseFlag = false;
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = 0;
    ret = g_rilInterface->Dial(SLOTID_2, -1, dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_NE(SUCCESS, ret);
    EXPECT_NE(true, g_dialResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DialResponse_0900
 * @tc.name  : testV1DialResponse007
 * @tc.desc  : Call function DialResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DialResponse007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    g_dialResponseFlag = false;
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 1;
    int32_t ret = 0;
    ret = g_rilInterface->Dial(SLOTID_1, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_dialResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DialResponse_1000
 * @tc.name  : testV1DialResponse008
 * @tc.desc  : Call function DialResponse.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DialResponse008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    g_dialResponseFlag = false;
    DialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 1;
    int32_t ret = 0;
    ret = g_rilInterface->Dial(SLOTID_2, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    EXPECT_EQ(true, g_dialResponseFlag);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_0300
 * @tc.name  : testV1DeactivatePdpContext001
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 0;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_0400
 * @tc.name  : testV1DeactivatePdpContext002
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_0500
 * @tc.name  : testV1DeactivatePdpContext003
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 serialId = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 100;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_0600
 * @tc.name  : testV1DeactivatePdpContext004
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1000000000;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_0700
 * @tc.name  : testV1DeactivatePdpContext005
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 255;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_0800
 * @tc.name  : testV1DeactivatePdpContext006
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 0;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_0900
 * @tc.name  : testV1DeactivatePdpContext007
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1000
 * @tc.name  : testV1DeactivatePdpContext008
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 serialId = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 100;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1100
 * @tc.name  : testV1DeactivatePdpContext009
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1000000000;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1200
 * @tc.name  : testV1DeactivatePdpContext010
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 255;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}
/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1300
 * @tc.name  : testV1DeactivatePdpContext011
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 uniInfo.gsmIndex = 1 uniInfo.arg1 = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 1;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1400
 * @tc.name  : testV1DeactivatePdpContext012
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 uniInfo.gsmIndex = 0 uniInfo.arg1 = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 0;
    uniInfo.arg1 = 0;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1500
 * @tc.name  : testV1DeactivatePdpContext013
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 uniInfo.gsmIndex = 255 uniInfo.arg1 = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 255;
    uniInfo.arg1 = 255;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1600
 * @tc.name  : testV1DeactivatePdpContext014
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 uniInfo.gsmIndex = 0 uniInfo.arg1 = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 0;
    uniInfo.arg1 = 255;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1700
 * @tc.name  : testV1DeactivatePdpContext015
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 uniInfo.gsmIndex = 255 uniInfo.arg1 = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 255;
    uniInfo.arg1 = 0;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1800
 * @tc.name  : testV1DeactivatePdpContext016
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 uniInfo.gsmIndex = 1 uniInfo.arg1 = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 1;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_1900
 * @tc.name  : testV1DeactivatePdpContext017
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 uniInfo.gsmIndex = 0 uniInfo.arg1 = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 0;
    uniInfo.arg1 = 0;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_2000
 * @tc.name  : testV1DeactivatePdpContext018
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 uniInfo.gsmIndex = 255 uniInfo.arg1 = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 255;
    uniInfo.arg1 = 255;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_2100
 * @tc.name  : testV1DeactivatePdpContext019
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 uniInfo.gsmIndex = 0 uniInfo.arg1 = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 0;
    uniInfo.arg1 = 255;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_2200
 * @tc.name  : testV1DeactivatePdpContext020
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 uniInfo.gsmIndex = 255 uniInfo.arg1 = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 255;
    uniInfo.arg1 = 0;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_2300
 * @tc.name  : testV1DeactivatePdpContext021
 * @tc.desc  : Deactivates the PDP context. SLOTID_1 uniInfo.gsmIndex = 1 uniInfo.arg1 = 2.Cycle 10 times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    int32_t ret;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    for (int i = 0; i < 10; i++) {
        ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1DeactivatePdpContext_2400
 * @tc.name  : testV1DeactivatePdpContext022
 * @tc.desc  : Deactivates the PDP context. SLOTID_2 uniInfo.gsmIndex = 1 uniInfo.arg1 = 2.Cycle 10 times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1DeactivatePdpContext022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    int32_t ret;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = 1;
    uniInfo.arg1 = 2;
    for (int i = 0; i < 10; i++) {
        ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_0300
 * @tc.name  : testV1GetPdpContextList001
 * @tc.desc  : Obtains the PDP context list. SLOTID_1 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 0;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_0400
 * @tc.name  : testV1GetPdpContextList002
 * @tc.desc  : Obtains the PDP context list. SLOTID_1 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_0500
 * @tc.name  : testV1GetPdpContextList003
 * @tc.desc  : Obtains the PDP context list. SLOTID_1 serialId = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 100;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_0600
 * @tc.name  : testV1GetPdpContextList004
 * @tc.desc  : Obtains the PDP context list. SLOTID_1 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1000000000;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_0700
 * @tc.name  : testV1GetPdpContextList005
 * @tc.desc  : Obtains the PDP context list. SLOTID_1 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 255;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_0800
 * @tc.name  : testV1GetPdpContextList006
 * @tc.desc  : Obtains the PDP context list. SLOTID_2 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 0;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_0900
 * @tc.name  : testV1GetPdpContextList007
 * @tc.desc  : Obtains the PDP context list. SLOTID_2 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_1000
 * @tc.name  : testV1GetPdpContextList008
 * @tc.desc  : Obtains the PDP context list. SLOTID_2 serialId = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 100;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_1100
 * @tc.name  : testV1GetPdpContextList009
 * @tc.desc  : Obtains the PDP context list. SLOTID_2 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1000000000;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_1200
 * @tc.name  : testV1GetPdpContextList010
 * @tc.desc  : Obtains the PDP context list. SLOTID_2 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 255;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_1300
 * @tc.name  : testV1GetPdpContextList011
 * @tc.desc  : Obtains the PDP context list. SLOTID_2 serialId = 1000000000.Cycle 10 times
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1000000000;
    int32_t ret;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    for (int i = 0; i < 10; i++) {
        ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1GetPdpContextList_1400
 * @tc.name  : testV1GetPdpContextList012
 * @tc.desc  : Obtains the PDP context list. SLOTID_2 serialId = 255.Cycle 10 times
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1GetPdpContextList012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret;
    int32_t serialId = 255;
    UniInfo uniInfo;
    uniInfo.serial = serialId;
    for (int i = 0; i < 10; i++) {
        ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
        WaitFor(WAIT_TIME_SECOND);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_0300
 * @tc.name  : testV1SetInitApnInfo001
 * @tc.desc  : slotId Card slot ID. SLOTID_1 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_0400
 * @tc.name  : testV1SetInitApnInfo002
 * @tc.desc  : slotId Card slot ID. SLOTID_1 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_0500
 * @tc.name  : testV1SetInitApnInfo003
 * @tc.desc  : slotId Card slot ID. SLOTID_1 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1000000000;
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_0600
 * @tc.name  : testV1SetInitApnInfo004
 * @tc.desc  : slotId Card slot ID. SLOTID_1 serialId = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 100;
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_0700
 * @tc.name  : testV1SetInitApnInfo005
 * @tc.desc  : slotId Card slot ID. SLOTID_1 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 255;
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_0800
 * @tc.name  : testV1SetInitApnInfo006
 * @tc.desc  : slotId Card slot ID. SLOTID_2 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_0900
 * @tc.name  : testV1SetInitApnInfo007
 * @tc.desc  : slotId Card slot ID. SLOTID_2 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_1000
 * @tc.name  : testV1SetInitApnInfo008
 * @tc.desc  : slotId Card slot ID. SLOTID_2 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1000000000;
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_1100
 * @tc.name  : testV1SetInitApnInfo009
 * @tc.desc  : slotId Card slot ID. SLOTID_2 serialId = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 100;
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_1200
 * @tc.name  : testV1SetInitApnInfo010
 * @tc.desc  : slotId Card slot ID. SLOTID_2 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 255;
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
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_1300
 * @tc.name  : testV1SetInitApnInfo011
 * @tc.desc  : slotId Card slot ID. SLOTID_2 serialId = 100.Cycle 20 times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 100;
    int32_t ret;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "user1";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "user1";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    for (int i = 0; i < 20; i++) {
        ret = g_rilInterface->SetInitApnInfo(SLOTID_2, serialId, dataProfileInfo);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SetInitApnInfo_1400
 * @tc.name  : testV1SetInitApnInfo012
 * @tc.desc  : slotId Card slot ID. SLOTID_2 serialId = 255.Cycle 20 times.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SetInitApnInfo012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 255;
    int32_t ret;
    DataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "user1";
    dataProfileInfo.authenticationType = 0;
    dataProfileInfo.userName = "user1";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    for (int i = 0; i < 20; i++) {
        ret = g_rilInterface->SetInitApnInfo(SLOTID_2, serialId, dataProfileInfo);
        WaitFor(WAIT_TIME_SECOND_LONG);
        EXPECT_EQ(SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_0300
 * @tc.name  : testV1SendDataSleepMode001
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 0;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_0400
 * @tc.name  : testV1SendDataSleepMode002
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_0500
 * @tc.name  : testV1SendDataSleepMode003
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 255;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_0600
 * @tc.name  : testV1SendDataSleepMode004
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 serialId = 10000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 10000;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_0700
 * @tc.name  : testV1SendDataSleepMode005
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1000000000;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_0800
 * @tc.name  : testV1SendDataSleepMode006
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 0;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_0900
 * @tc.name  : testV1SendDataSleepMode007
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1000
 * @tc.name  : testV1SendDataSleepMode008
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 255;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1100
 * @tc.name  : testV1SendDataSleepMode009
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 serialId = 10000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 10000;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1200
 * @tc.name  : testV1SendDataSleepMode010
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1000000000;
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1300
 * @tc.name  : testV1SendDataSleepMode011
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 dataSleepInfo.sleepEnable = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 0;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1400
 * @tc.name  : testV1SendDataSleepMode012
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 dataSleepInfo.sleepEnable = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 100;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1500
 * @tc.name  : testV1SendDataSleepMode013
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 dataSleepInfo.sleepEnable = 1000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1000;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1600
 * @tc.name  : testV1SendDataSleepMode014
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 dataSleepInfo.sleepEnable = 10000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 10000;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1700
 * @tc.name  : testV1SendDataSleepMode015
 * @tc.desc  : Sends the data service sleep mode. SLOTID_1 dataSleepInfo.sleepEnable = 32767.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 32767;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_1, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1800
 * @tc.name  : testV1SendDataSleepMode016
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 dataSleepInfo.sleepEnable = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 0;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_1900
 * @tc.name  : testV1SendDataSleepMode017
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 dataSleepInfo.sleepEnable = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 100;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_2000
 * @tc.name  : testV1SendDataSleepMode018
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 dataSleepInfo.sleepEnable = 1000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 1000;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_2100
 * @tc.name  : testV1SendDataSleepMode019
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 dataSleepInfo.sleepEnable = 10000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 10000;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataSleepMode_2200
 * @tc.name  : testV1SendDataSleepMode020
 * @tc.desc  : Sends the data service sleep mode. SLOTID_2 dataSleepInfo.sleepEnable = 32767.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataSleepMode020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataSleepInfo dataSleepInfo;
    dataSleepInfo.sleepEnable = 32767;
    int32_t ret = g_rilInterface->SendDataSleepMode(SLOTID_2, serialId, dataSleepInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_SLEEP_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_0300
 * @tc.name  : testV1SendDataPerformanceMode001
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode001, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 0;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_0400
 * @tc.name  : testV1SendDataPerformanceMode002
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode002, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_0500
 * @tc.name  : testV1SendDataPerformanceMode003
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode003, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 255;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_0600
 * @tc.name  : testV1SendDataPerformanceMode004
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 serialId = 10000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode004, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 10000;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_0700
 * @tc.name  : testV1SendDataPerformanceMode005
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode005, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = 1000000000;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_0800
 * @tc.name  : testV1SendDataPerformanceMode006
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 serialId = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode006, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 0;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_0900
 * @tc.name  : testV1SendDataPerformanceMode007
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 serialId = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode007, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1000
 * @tc.name  : testV1SendDataPerformanceMode008
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 serialId = 255.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode008, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 255;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1100
 * @tc.name  : testV1SendDataPerformanceMode009
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 serialId = 10000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode009, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 10000;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1200
 * @tc.name  : testV1SendDataPerformanceMode010
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 serialId = 1000000000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode010, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = 1000000000;
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1300
 * @tc.name  : testV1SendDataPerformanceMode011
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode011, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 0;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1400
 * @tc.name  : testV1SendDataPerformanceMode012
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 1.
 * dataPerformanceInfo.enforce = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode012, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 1;
    dataPerformanceInfo.enforce = 0;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1500
 * @tc.name  : testV1SendDataPerformanceMode013
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 1.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode013, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 1;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1600
 * @tc.name  : testV1SendDataPerformanceMode014
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode014, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 100;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1700
 * @tc.name  : testV1SendDataPerformanceMode015
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 1000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode015, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1000;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1800
 * @tc.name  : testV1SendDataPerformanceMode016
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 10000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode016, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 10000;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_1900
 * @tc.name  : testV1SendDataPerformanceMode017
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 32767.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode017, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 32767;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2000
 * @tc.name  : testV1SendDataPerformanceMode018
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 100.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode018, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 100;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2100
 * @tc.name  : testV1SendDataPerformanceMode019
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 1000.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode019, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 1000;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2200
 * @tc.name  : testV1SendDataPerformanceMode020
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 10000.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode020, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 10000;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2300
 * @tc.name  : testV1SendDataPerformanceMode021
 * @tc.desc  : Sends the data service performance mode. SLOTID_1 dataPerformanceInfo.performanceEnable = 32767.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode021, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 32767;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_1, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2400
 * @tc.name  : testV1SendDataPerformanceMode022
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode022, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 0;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2500
 * @tc.name  : testV1SendDataPerformanceMode023
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 1.
 * dataPerformanceInfo.enforce = 0.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode023, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 1;
    dataPerformanceInfo.enforce = 0;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2600
 * @tc.name  : testV1SendDataPerformanceMode024
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 1.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode024, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 1;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2700
 * @tc.name  : testV1SendDataPerformanceMode025
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 100.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode025, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 100;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2800
 * @tc.name  : testV1SendDataPerformanceMode026
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 1000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode026, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 1000;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_2900
 * @tc.name  : testV1SendDataPerformanceMode027
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 10000.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode027, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 10000;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_3000
 * @tc.name  : testV1SendDataPerformanceMode028
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 0.
 * dataPerformanceInfo.enforce = 32767.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode028, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 0;
    dataPerformanceInfo.enforce = 32767;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_3100
 * @tc.name  : testV1SendDataPerformanceMode029
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 100.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode029, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 100;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_3200
 * @tc.name  : testV1SendDataPerformanceMode030
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 1000.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode030, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 1000;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_3300
 * @tc.name  : testV1SendDataPerformanceMode031
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 10000.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode031, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 10000;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}

/**
 * @tc.number: SUB_Telephony_DriverSystem_V1SendDataPerformanceMode_3400
 * @tc.name  : testV1SendDataPerformanceMode032
 * @tc.desc  : Sends the data service performance mode. SLOTID_2 dataPerformanceInfo.performanceEnable = 32767.
 * dataPerformanceInfo.enforce = 1.
 */
HWTEST_F(HdfRilHdiTestAdditional3rd, testV1SendDataPerformanceMode032, Function | MediumTest | Level1)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    DataPerformanceInfo dataPerformanceInfo;
    dataPerformanceInfo.performanceEnable = 32767;
    dataPerformanceInfo.enforce = 1;
    int32_t ret = g_rilInterface->SendDataPerformanceMode(SLOTID_2, serialId, dataPerformanceInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_GT(ret, -1);
    GetBoolResult(HdiId::HREQ_DATA_SEND_DATA_PERFORMANCE_MODE);
}