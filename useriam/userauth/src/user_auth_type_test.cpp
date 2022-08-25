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
#include "user_auth_hdi.h"

#define LOG_LABEL OHOS::UserIam::Common::LABEL_FACE_AUTH_IMPL

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::UserAuth::V1_0;

static OHOS::Parcel parcel;

void UserAuthTypeTest::SetUpTestCase()
{
}

void UserAuthTypeTest::TearDownTestCase()
{
}

void UserAuthTypeTest::SetUp()
{
}

void UserAuthTypeTest::TearDown()
{
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0201
 * @tc.name: Test AuthType
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserAuthTypeTest, Security_IAM_UserAuth_HDI_FUNC_0201, Function | MediumTest | Level1)
{
    cout << "start test AuthType" << endl;
    AuthType all = ALL;
    AuthType pin = PIN;
    AuthType face = FACE;
    AuthType fingerprint = FINGERPRINT;
    EXPECT_EQ(all, 0);
    EXPECT_EQ(pin, 1);
    EXPECT_EQ(face, 2);
    EXPECT_EQ(fingerprint, 4);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0202
 * @tc.name: Test ExecutorRole
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserAuthTypeTest, Security_IAM_UserAuth_HDI_FUNC_0202, Function | MediumTest | Level1)
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
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0203
 * @tc.name: Test ExecutorSecureLevel
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserAuthTypeTest, Security_IAM_UserAuth_HDI_FUNC_0203, Function | MediumTest | Level1)
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
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0204
 * @tc.name: Test PinSubType
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserAuthTypeTest, Security_IAM_UserAuth_HDI_FUNC_0204, Function | MediumTest | Level1)
{
    cout << "start test PinSubType" << endl;
    PinSubType pin_six = PIN_SIX;
    PinSubType pin_number = PIN_NUMBER;
    PinSubType pin_mix = PIN_MIX;
    EXPECT_EQ(pin_six, 10000);
    EXPECT_EQ(pin_number, 10001);
    EXPECT_EQ(pin_mix, 10002);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0205
 * @tc.name: Test ScheduleMode
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserAuthTypeTest, Security_IAM_UserAuth_HDI_FUNC_0205, Function | MediumTest | Level1)
{
    cout << "start test ScheduleMode" << endl;
    ScheduleMode enroll = ENROLL;
    ScheduleMode auth = AUTH;
    ScheduleMode identify = IDENTIFY;
    EXPECT_EQ(enroll, 0);
    EXPECT_EQ(auth, 1);
    EXPECT_EQ(identify, 2);
}