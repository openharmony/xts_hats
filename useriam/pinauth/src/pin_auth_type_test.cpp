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
#include "pin_auth_hdi_test.h"

#define LOG_LABEL OHOS::UserIam::Common::LABEL_FACE_AUTH_IMPL

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::PinAuth;
using namespace OHOS::HDI::PinAuth::V3_0;
using HdiAuthType = OHOS::HDI::PinAuth::V3_0::AuthType;
using HdiExecutorRole = OHOS::HDI::PinAuth::V3_0::ExecutorRole;

static OHOS::Parcel parcel;

void PinAuthTypeTest::SetUpTestCase()
{
}

void PinAuthTypeTest::TearDownTestCase()
{
}

void PinAuthTypeTest::SetUp()
{
}

void PinAuthTypeTest::TearDown()
{
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0201
 * @tc.name: Test AuthType
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(PinAuthTypeTest, Security_IAM_PinAuth_HDI_FUNC_0201, Function | MediumTest | Level1)
{
    cout << "start test AuthType" << endl;
    HdiAuthType pin = HdiAuthType::PIN;
    HdiAuthType face = HdiAuthType::FACE;
    HdiAuthType fingerprint = HdiAuthType::FINGERPRINT;
    EXPECT_EQ(pin, 1);
    EXPECT_EQ(face, 2);
    EXPECT_EQ(fingerprint, 4);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0202
 * @tc.name: Test ExecutorRole
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(PinAuthTypeTest, Security_IAM_PinAuth_HDI_FUNC_0202, Function | MediumTest | Level1)
{
    cout << "start test ExecutorRole" << endl;
    HdiExecutorRole collector = HdiExecutorRole::COLLECTOR;
    HdiExecutorRole verifier = HdiExecutorRole::VERIFIER;
    HdiExecutorRole all_in_one = HdiExecutorRole::ALL_IN_ONE;
    EXPECT_EQ(collector, 1);
    EXPECT_EQ(verifier, 2);
    EXPECT_EQ(all_in_one, 3);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0203
 * @tc.name: Test ExecutorSecureLevel
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(PinAuthTypeTest, Security_IAM_PinAuth_HDI_FUNC_0203, Function | MediumTest | Level1)
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
 * @tc.number: Security_IAM_PinAuth_HDI_NEW_FUNC_0201
 * @tc.name: Test GetPropertyType
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(PinAuthTypeTest, Security_IAM_PinAuth_HDI_NEW_FUNC_0201, Function | MediumTest | Level1)
{
    cout << "start test GetPropertyType" << endl;
    GetPropertyType auth_sub_type = OHOS::HDI::PinAuth::V3_0::AUTH_SUB_TYPE;
    GetPropertyType lockout_duration = OHOS::HDI::PinAuth::V3_0::LOCKOUT_DURATION;
    GetPropertyType remain_attempts = OHOS::HDI::PinAuth::V3_0::REMAIN_ATTEMPTS;
    GetPropertyType next_fail_lockout_duration = OHOS::HDI::PinAuth::V3_0::NEXT_FAIL_LOCKOUT_DURATION;
    GetPropertyType credential_length = OHOS::HDI::PinAuth::V3_0::CREDENTIAL_LENGTH ;
    EXPECT_EQ(auth_sub_type, 1);
    EXPECT_EQ(lockout_duration, 2);
    EXPECT_EQ(remain_attempts, 3);
    EXPECT_EQ(next_fail_lockout_duration, 6);
    EXPECT_EQ(credential_length, 7);
}