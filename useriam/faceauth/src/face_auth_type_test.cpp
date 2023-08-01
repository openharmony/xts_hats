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
#include "face_auth_hdi_test.h"

#define LOG_LABEL OHOS::UserIam::Common::LABEL_FACE_AUTH_IMPL

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::FaceAuth;
using namespace OHOS::HDI::FaceAuth::V1_0;

static OHOS::Parcel parcel;

void FaceAuthTypeTest::SetUpTestCase()
{
}

void FaceAuthTypeTest::TearDownTestCase()
{
}

void FaceAuthTypeTest::SetUp()
{
}

void FaceAuthTypeTest::TearDown()
{
}

/**
 * @tc.number: Security_IAM_Face_HDI_FUNC_0201
 * @tc.name: Test AuthType
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FaceAuthTypeTest, Security_IAM_Face_HDI_FUNC_0201, Function | MediumTest | Level1)
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
 * @tc.number: Security_IAM_Face_HDI_FUNC_0202
 * @tc.name: Test ExecutorRole
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FaceAuthTypeTest, Security_IAM_Face_HDI_FUNC_0202, Function | MediumTest | Level1)
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
 * @tc.number: Security_IAM_Face_HDI_FUNC_0203
 * @tc.name: Test ExecutorSecureLevel
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FaceAuthTypeTest, Security_IAM_Face_HDI_FUNC_0203, Function | MediumTest | Level1)
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
 * @tc.number: Security_IAM_Face_HDI_FUNC_0204
 * @tc.name: Test CommandId
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FaceAuthTypeTest, Security_IAM_Face_HDI_FUNC_0204, Function | MediumTest | Level1)
{
    cout << "start test CommandId" << endl;
    CommandId lock_template = LOCK_TEMPLATE;
    CommandId unlock_template = UNLOCK_TEMPLATE;
    CommandId vendor_command_begin = VENDOR_COMMAND_BEGIN;
    CommandId init_algorithm = OHOS::HDI::FaceAuth::V1_1::INIT_ALGORITHM;
    EXPECT_EQ(lock_template, 1);
    EXPECT_EQ(unlock_template, 2);
    EXPECT_EQ(vendor_command_begin, 10000);
    EXPECT_EQ(init_algorithm, 3);
}

/**
 * @tc.number: Security_IAM_Face_HDI_FUNC_0205
 * @tc.name: Test FaceTipsCode
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FaceAuthTypeTest, Security_IAM_Face_HDI_FUNC_0205, Function | MediumTest | Level1)
{
    cout << "start test FaceTipsCode" << endl;
    FaceTipsCode f_bright = FACE_AUTH_TIP_TOO_BRIGHT;
    FaceTipsCode f_dark = FACE_AUTH_TIP_TOO_DARK;
    FaceTipsCode f_close = FACE_AUTH_TIP_TOO_CLOSE;
    FaceTipsCode f_far = FACE_AUTH_TIP_TOO_FAR;
    FaceTipsCode f_high = FACE_AUTH_TIP_TOO_HIGH;
    FaceTipsCode f_low = FACE_AUTH_TIP_TOO_LOW;
    FaceTipsCode f_right = FACE_AUTH_TIP_TOO_RIGHT;
    FaceTipsCode f_left = FACE_AUTH_TIP_TOO_LEFT;
    FaceTipsCode f_motion = FACE_AUTH_TIP_TOO_MUCH_MOTION;
    FaceTipsCode f_gaze = FACE_AUTH_TIP_POOR_GAZE;
    FaceTipsCode f_detected = FACE_AUTH_TIP_NOT_DETECTED;
    FaceTipsCode f_begin = VENDOR_FACE_AUTH_TIP_BEGIN;
    EXPECT_EQ(f_bright, 1);
    EXPECT_EQ(f_dark, 2);
    EXPECT_EQ(f_close, 3);
    EXPECT_EQ(f_far, 4);
    EXPECT_EQ(f_high, 5);
    EXPECT_EQ(f_low, 6);
    EXPECT_EQ(f_right, 7);
    EXPECT_EQ(f_left, 8);
    EXPECT_EQ(f_motion, 9);
    EXPECT_EQ(f_gaze, 10);
    EXPECT_EQ(f_detected, 11);
    EXPECT_EQ(f_begin, 10000);
}

/**
 * @tc.number: Security_IAM_Face_HDI_NEW_FUNC_0101
 * @tc.name: Test GetPropertyType
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FaceAuthTypeTest, Security_IAM_Face_HDI_NEW_FUNC_0201, Function | MediumTest | Level1)
{
    cout << "start test GetPropertyType" << endl;
    GetPropertyType g_type = OHOS::HDI::FaceAuth::V1_1::AUTH_SUB_TYPE;
    GetPropertyType g_duration = OHOS::HDI::FaceAuth::V1_1::LOCKOUT_DURATION;
    GetPropertyType g_attempts = OHOS::HDI::FaceAuth::V1_1::REMAIN_ATTEMPTS;
    GetPropertyType g_progress = OHOS::HDI::FaceAuth::V1_1::ENROLL_PROGRESS;
    GetPropertyType g_info = OHOS::HDI::FaceAuth::V1_1::SENSOR_INFO;

    EXPECT_EQ(g_type, 1);
    EXPECT_EQ(g_duration, 2);
    EXPECT_EQ(g_attempts, 3);
    EXPECT_EQ(g_progress, 4);
    EXPECT_EQ(g_info, 5);
}

/**
 * @tc.number: Security_IAM_Face_HDI_NEW_FUNC_0202
 * @tc.name: Test SaCommandId
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(FaceAuthTypeTest, Security_IAM_Face_HDI_NEW_FUNC_0202, Function | MediumTest | Level1)
{
    cout << "start test SaCommandId" << endl;
    SaCommandId begin_screen_brightness_increase = OHOS::HDI::FaceAuth::V1_1::BEGIN_SCREEN_BRIGHTNESS_INCREASE;
    SaCommandId end_screen_brightness_increase = OHOS::HDI::FaceAuth::V1_1::END_SCREEN_BRIGHTNESS_INCREASE;
    
    EXPECT_EQ(begin_screen_brightness_increase, 1);
    EXPECT_EQ(end_screen_brightness_increase, 2);
}