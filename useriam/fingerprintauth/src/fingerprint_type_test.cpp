/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <hdf_base.h>
#include "iam_hat_test.h"
#include "fingerprint_auth_hdi.h"

#define LOG_LABEL OHOS::UserIam::Common::LABEL_FINGERPRINT_AUTH_IMPL

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::FingerprintAuth::V1_0;

static OHOS::Parcel parcel;

void FingerPrintTypeTest::SetUpTestCase()
{
}

void FingerPrintTypeTest::TearDownTestCase()
{
}

void FingerPrintTypeTest::SetUp()
{
}

void FingerPrintTypeTest::TearDown()
{
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0201
 * @tc.name: Test AuthType
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FingerPrintTypeTest, Security_IAM_Fingerprint_HDI_FUNC_0201, Function | MediumTest | Level1)
{
    cout << "start test AuthType" << endl;
    AuthType pin = PIN;
    AuthType face = FACE;
    AuthType fingerprint = FINGERPRINT;
    EXPECT_EQ(pin, 1);
    EXPECT_EQ(face, 2);
    EXPECT_EQ(fingerprint, 4);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0202
 * @tc.name: Test ExecutorRole
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FingerPrintTypeTest, Security_IAM_Fingerprint_HDI_FUNC_0202, Function | MediumTest | Level1)
{
    cout << "start test ExecutorRole" << endl;
    ExecutorRole collector = COLLECTOR;
    ExecutorRole verifier = VERIFIER;
    ExecutorRole all_in_one = ALL_IN_ONE;
    EXPECT_EQ(collector, 1);
    EXPECT_EQ(verifier, 2);
    EXPECT_EQ(all_in_one, 3);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0203
 * @tc.name: Test ExecutorSecureLevel
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FingerPrintTypeTest, Security_IAM_Fingerprint_HDI_FUNC_0203, Function | MediumTest | Level1)
{
    cout << "start test ExecutorSecureLevel" << endl;
    ExecutorSecureLevel esl0 = ESL0;
    ExecutorSecureLevel esl1 = ESL1;
    ExecutorSecureLevel esl2 = ESL2;
    ExecutorSecureLevel esl3 = ESL3;
    EXPECT_EQ(esl0, 0);
    EXPECT_EQ(esl1, 1);
    EXPECT_EQ(esl2, 2);
    EXPECT_EQ(esl3, 3);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0204
 * @tc.name: Test CommandId
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FingerPrintTypeTest, Security_IAM_Fingerprint_HDI_FUNC_0204, Function | MediumTest | Level1)
{
    cout << "start test CommandId" << endl;
    CommandId lock_template = LOCK_TEMPLATE;
    CommandId unlock_template = UNLOCK_TEMPLATE;
    CommandId vendor_command_begin = VENDOR_COMMAND_BEGIN;
    EXPECT_EQ(lock_template, 1);
    EXPECT_EQ(unlock_template, 2);
    EXPECT_EQ(vendor_command_begin, 10000);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0205
 * @tc.name: Test FingerprintTipsCode
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FingerPrintTypeTest, Security_IAM_Fingerprint_HDI_FUNC_0205, Function | MediumTest | Level1)
{
    cout << "start test FingerprintTipsCode" << endl;
    FingerprintTipsCode f_good = FINGERPRINT_AUTH_TIP_GOOD;
    FingerprintTipsCode f_dirty = FINGERPRINT_AUTH_TIP_DIRTY;
    FingerprintTipsCode f_insufficient = FINGERPRINT_AUTH_TIP_INSUFFICIENT;
    FingerprintTipsCode f_partial = FINGERPRINT_AUTH_TIP_PARTIAL;
    FingerprintTipsCode f_fast = FINGERPRINT_AUTH_TIP_TOO_FAST;
    FingerprintTipsCode f_slow = FINGERPRINT_AUTH_TIP_TOO_SLOW;
    FingerprintTipsCode f_begin = VENDOR_FINGERPRINT_AUTH_TIP_BEGIN;
    EXPECT_EQ(f_good, 0);
    EXPECT_EQ(f_dirty, 1);
    EXPECT_EQ(f_insufficient, 2);
    EXPECT_EQ(f_partial, 3);
    EXPECT_EQ(f_fast, 4);
    EXPECT_EQ(f_slow, 5);
    EXPECT_EQ(f_begin, 10000);
}