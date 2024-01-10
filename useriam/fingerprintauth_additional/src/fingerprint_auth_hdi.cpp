/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include <hdf_base.h>
#include "iam_hat_test.h"
#include "fingerprint_auth_hdi_test.h"

#define LOG_LABEL OHOS::UserIam::Common::LABEL_FINGERPRINT_AUTH_IMPL

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::FingerprintAuth;
using namespace OHOS::HDI::FingerprintAuth::V1_0;
using Property = OHOS::HDI::FingerprintAuth::V1_1::Property;
using SaCommandParamNone = OHOS::HDI::FingerprintAuth::V1_1::SaCommandParamNone;
using SaCommandParam = OHOS::HDI::FingerprintAuth::V1_1::SaCommandParam;
using SaCommand = OHOS::HDI::FingerprintAuth::V1_1::SaCommand;

static ExecutorImpl g_executorImpl;
static OHOS::Parcel parcel;
bool g_onResultFlag = false;
bool g_onTipFlag = false;
bool g_onSaCommandsFlag = false;

void UserIamFingerprintAuthTestAdditional::SetUpTestCase() {}

void UserIamFingerprintAuthTestAdditional::TearDownTestCase() {}

void UserIamFingerprintAuthTestAdditional::SetUp() {}

void UserIamFingerprintAuthTestAdditional::TearDown() {}

class DummyIExecutorCallback : public IExecutorCallback {
public:
    DummyIExecutorCallback(int32_t result, int32_t acquire) : result_(result), acquire_(acquire) {}

    int32_t OnResult(int32_t result, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "result is " << result << " extraInfo len is " << extraInfo.size() << endl;
        g_onResultFlag = true;
        return result_;
    }

    int32_t OnTip(int32_t acquire, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "result is " << acquire << " extraInfo len is " << extraInfo.size() << endl;
        g_onTipFlag = true;
        return acquire_;
    }

private:
    int32_t result_;
    int32_t acquire_;
};

class DummyISaCommandCallback : public ISaCommandCallback {
public:
    explicit DummyISaCommandCallback(int32_t result) : result_(result) {}

    int32_t OnSaCommands(const std::vector<SaCommand> &commands) override
    {
        g_onSaCommandsFlag = true;
        return result_;
    }

private:
    int32_t result_;
};

static void FillTestIExecutorCallback(Parcel &parcel, sptr<IExecutorCallback> &callbackObj)
{
    bool isNull = parcel.ReadBool();
    if (isNull) {
        callbackObj = nullptr;
    } else {
        callbackObj = new (std::nothrow) DummyIExecutorCallback(parcel.ReadInt32(), parcel.ReadInt32());
        if (callbackObj == nullptr) {
            cout << "callbackObj construct fail" << endl;
        }
    }
    cout << "success" << endl;
}

void FillTestGetPropertyTypeVector(Parcel &parcel, std::vector<GetPropertyType> &types)
{
    std::vector<uint32_t> propertyTypeUint32;
    FillTestUint32Vector(parcel, propertyTypeUint32);
    for (const auto &val : propertyTypeUint32) {
        types.push_back(static_cast<GetPropertyType>(val));
    }

    cout << "success" << endl;
}

void FillTestProperty(Parcel &parcel, Property &property)
{
    property.authSubType = parcel.ReadUint64();
    property.lockoutDuration = parcel.ReadInt32();
    property.remainAttempts = parcel.ReadInt32();
    FillTestString(parcel, property.enrollmentProgress);
    FillTestString(parcel, property.sensorInfo);

    cout << "success" << endl;
}

void FillTestISaCommandCallback(Parcel &parcel, sptr<ISaCommandCallback> &callbackObj)
{
    bool isNull = parcel.ReadBool();
    if (isNull) {
        callbackObj = nullptr;
    } else {
        callbackObj = new (std::nothrow) DummyISaCommandCallback(parcel.ReadInt32());
        if (callbackObj == nullptr) {
            cout << "callbackObj construct fail" << endl;
        }
    }
    cout << "success" << endl;
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetExecutorInfo_0200
 * @tc.name  testFingerprintAuthTestGetExecutorInfo001
 * @tc.desc  test GetExecutorInfo executorInfo not empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetExecutorInfo001,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetExecutorInfo001" << endl;
    ExecutorInfo executorInfo;
    int32_t ret = g_executorImpl.GetExecutorInfo(executorInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetExecutorInfo_0300
 * @tc.name  testFingerprintAuthTestGetExecutorInfo002
 * @tc.desc  test GetExecutorInfo 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetExecutorInfo002,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetExecutorInfo002" << endl;
    ExecutorInfo executorInfo;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.GetExecutorInfo(executorInfo);
        cout << "ret " << i << " is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetTemplateInfo_0200
 * @tc.name  testFingerprintAuthTestGetTemplateInfo001
 * @tc.desc  test GetTemplateInfo templateId 0
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetTemplateInfo001,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetTemplateInfo001" << endl;
    uint64_t templateId = 0;
    TemplateInfo templateInfo;
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetTemplateInfo_0300
 * @tc.name  testFingerprintAuthTestGetTemplateInfo002
 * @tc.desc  test GetTemplateInfo templateId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetTemplateInfo002,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetTemplateInfo002" << endl;
    uint64_t templateId = 0x7FFFFFFFFFFFFFFF;
    TemplateInfo templateInfo;
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetTemplateInfo_0400
 * @tc.name  testFingerprintAuthTestGetTemplateInfo003
 * @tc.desc  test GetTemplateInfo templateId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetTemplateInfo003,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetTemplateInfo003" << endl;
    uint64_t templateId = 0xFFFFFFFFFFFFFFFF;
    TemplateInfo templateInfo;
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetTemplateInfo_0500
 * @tc.name  testFingerprintAuthTestGetTemplateInfo004
 * @tc.desc  test GetTemplateInfo templateId 0x10
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetTemplateInfo004,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetTemplateInfo004" << endl;
    uint64_t templateId = 0x10;
    TemplateInfo templateInfo;
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetTemplateInfo_0600
 * @tc.name  testFingerprintAuthTestGetTemplateInfo005
 * @tc.desc  test GetTemplateInfo 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetTemplateInfo005,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetTemplateInfo005" << endl;
    uint64_t templateId = 0x01;
    TemplateInfo templateInfo;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
        cout << "ret " << i << " is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0200
 * @tc.name  testFingerprintAuthTestOnRegisterFinish001
 * @tc.desc  test OnRegisterFinish templateIdList empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish001,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish001" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> frameworkPublicKey;
    FillTestUint8Vector(parcel, frameworkPublicKey);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0300
 * @tc.name  testFingerprintAuthTestOnRegisterFinish002
 * @tc.desc  test OnRegisterFinish frameworkPublicKey empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish002,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish002" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> frameworkPublicKey;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0400
 * @tc.name  testFingerprintAuthTestOnRegisterFinish003
 * @tc.desc  test OnRegisterFinish extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish003,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish003" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> frameworkPublicKey;
    FillTestUint8Vector(parcel, frameworkPublicKey);
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0500
 * @tc.name  testFingerprintAuthTestOnRegisterFinish004
 * @tc.desc  test OnRegisterFinish frameworkPublicKey and extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish004,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish004" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> frameworkPublicKey;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0600
 * @tc.name  testFingerprintAuthTestOnRegisterFinish005
 * @tc.desc  test OnRegisterFinish templateIdList and extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish005,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish005" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> frameworkPublicKey;
    FillTestUint8Vector(parcel, frameworkPublicKey);
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0700
 * @tc.name  testFingerprintAuthTestOnRegisterFinish006
 * @tc.desc  test OnRegisterFinish templateIdList and frameworkPublicKey empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish006,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish006" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> frameworkPublicKey;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0800
 * @tc.name  testFingerprintAuthTestOnRegisterFinish007
 * @tc.desc  test OnRegisterFinish templateIdList and frameworkPublicKey and extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish007,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish007" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> frameworkPublicKey;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnRegisterFinish_0900
 * @tc.name  testFingerprintAuthTestOnRegisterFinish008
 * @tc.desc  test OnRegisterFinish 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnRegisterFinish008,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnRegisterFinish008" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> frameworkPublicKey;
    FillTestUint8Vector(parcel, frameworkPublicKey);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
        cout << "ret " << i << " is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0200
 * @tc.name  testFingerprintAuthTestEnroll001
 * @tc.desc  test Enroll scheduleId 0
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestEnroll001" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0300
 * @tc.name  testFingerprintAuthTestEnroll002
 * @tc.desc  test Enroll scheduleId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll002, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestEnroll002" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0400
 * @tc.name  testFingerprintAuthTestEnroll003
 * @tc.desc  test Enroll scheduleId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll003, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestEnroll003" << endl;
    uint64_t scheduleId = 0xFFFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0500
 * @tc.name  testFingerprintAuthTestEnroll004
 * @tc.desc  test Enroll extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll004, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestEnroll004" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0600
 * @tc.name  testFingerprintAuthTestEnroll005
 * @tc.desc  test Enroll callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll005, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestEnroll005" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0700
 * @tc.name  testFingerprintAuthTestEnroll006
 * @tc.desc  test Enroll extraInfo empty and callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll006, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestEnroll006" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Enroll_0800
 * @tc.name  testFingerprintAuthTestEnroll007
 * @tc.desc  test Enroll 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestEnroll007, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestEnroll007" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
        cout << "ret " << i << " is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0200
 * @tc.name  testFingerprintAuthTestAuthenticate001
 * @tc.desc  test Authenticate scheduleId 0
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate001" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0300
 * @tc.name  testFingerprintAuthTestAuthenticate002
 * @tc.desc  test Authenticate scheduleId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate002, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate002" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0400
 * @tc.name  testFingerprintAuthTestAuthenticate003
 * @tc.desc  test Authenticate scheduleId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate003, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate003" << endl;
    uint64_t scheduleId = 0xFFFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0500
 * @tc.name  testFingerprintAuthTestAuthenticate004
 * @tc.desc  test Authenticate templateIdList empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate004, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate004" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0600
 * @tc.name  testFingerprintAuthTestAuthenticate005
 * @tc.desc  test Authenticate extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate005, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate005" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0700
 * @tc.name  testFingerprintAuthTestAuthenticate006
 * @tc.desc  test Authenticate callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate006, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestAuthenticate006" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0800
 * @tc.name  testFingerprintAuthTestAuthenticate007
 * @tc.desc  test Authenticate extraInfo empty and callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate007, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestAuthenticate007" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_0900
 * @tc.name  testFingerprintAuthTestAuthenticate008
 * @tc.desc  test Authenticate templateIdList empty and callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate008, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestAuthenticate008" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_1000
 * @tc.name  testFingerprintAuthTestAuthenticate009
 * @tc.desc  test Authenticate empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate009, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate009" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_1100
 * @tc.name  testFingerprintAuthTestAuthenticate010
 * @tc.desc  test Authenticate templateIdList and extraInfo empty and callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate010, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestAuthenticate010" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Authenticate_1200
 * @tc.name  testFingerprintAuthTestAuthenticate011
 * @tc.desc  test Authenticate 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticate011, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate011" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0200
 * @tc.name  testFingerprintAuthTestIdentify001
 * @tc.desc  test Identify scheduleId 0
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestIdentify001" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0300
 * @tc.name  testFingerprintAuthTestIdentify002
 * @tc.desc  test Identify scheduleId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify002, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestIdentify002" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0400
 * @tc.name  testFingerprintAuthTestIdentify003
 * @tc.desc  test Identify scheduleId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify003, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestIdentify003" << endl;
    uint64_t scheduleId = 0xFFFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0500
 * @tc.name  testFingerprintAuthTestIdentify004
 * @tc.desc  test Identify extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify004, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestIdentify004" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0600
 * @tc.name  testFingerprintAuthTestIdentify005
 * @tc.desc  test Identify callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify005, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestIdentify005" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0700
 * @tc.name  testFingerprintAuthTestIdentify006
 * @tc.desc  test Identify extraInfo empty and callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify006, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestIdentify006" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Identify_0800
 * @tc.name  testFingerprintAuthTestIdentify007
 * @tc.desc  test Identify 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestIdentify007, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestIdentify007" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Delete_0200
 * @tc.name  testFingerprintAuthTestDelete001
 * @tc.desc  test Delete templateIdList empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestDelete001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestDelete001" << endl;
    std::vector<uint64_t> templateIdList;
    int32_t ret = g_executorImpl.Delete(templateIdList);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Delete_0300
 * @tc.name  testFingerprintAuthTestDelete002
 * @tc.desc  test Delete 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestDelete002, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestDelete002" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Delete(templateIdList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Cancel_0200
 * @tc.name  testFingerprintAuthTestCancel001
 * @tc.desc  test Cancel scheduleId 0
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestCancel001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestCancel001" << endl;
    uint64_t scheduleId = 0;
    int32_t ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Cancel_0300
 * @tc.name  testFingerprintAuthTestCancel002
 * @tc.desc  test Cancel scheduleId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestCancel002, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestCancel002" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    int32_t ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Cancel_0400
 * @tc.name  testFingerprintAuthTestCancel003
 * @tc.desc  test Cancel scheduleId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestCancel003, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestCancel003" << endl;
    uint64_t scheduleId = 0xFFFFFFFFFFFFFFFF;
    int32_t ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_Cancel_0500
 * @tc.name  testFingerprintAuthTestCancel004
 * @tc.desc  test Cancel 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestCancel004, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestCancel004" << endl;
    uint64_t scheduleId = 0;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Cancel(scheduleId);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_0200
 * @tc.name  testFingerprintAuthTestSendCommand001
 * @tc.desc  test SendCommand commandId enum
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand001" << endl;
    int32_t commandId = LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
	commandId = UNLOCK_TEMPLATE;
	ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
	commandId = VENDOR_COMMAND_BEGIN;
	ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_0500
 * @tc.name  testFingerprintAuthTestSendCommand004
 * @tc.desc  test SendCommand commandId 0
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand004, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand004" << endl;
    int32_t commandId = 0;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_0600
 * @tc.name  testFingerprintAuthTestSendCommand005
 * @tc.desc  test SendCommand commandId 0x7FFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand005, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand005" << endl;
    int32_t commandId = 0x7FFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_0700
 * @tc.name  testFingerprintAuthTestSendCommand006
 * @tc.desc  test SendCommand commandId -0x01
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand006, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand006" << endl;
    int32_t commandId = -0x01;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_0800
 * @tc.name  testFingerprintAuthTestSendCommand007
 * @tc.desc  test SendCommand commandId -0x7FFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand007, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand007" << endl;
    int32_t commandId = -0x7FFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_0900
 * @tc.name  testFingerprintAuthTestSendCommand008
 * @tc.desc  test SendCommand extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand008, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand008" << endl;
    int32_t commandId = LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_1000
 * @tc.name  testFingerprintAuthTestSendCommand009
 * @tc.desc  test SendCommand callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand009, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestSendCommand009" << endl;
    int32_t commandId = LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_1100
 * @tc.name  testFingerprintAuthTestSendCommand010
 * @tc.desc  test SendCommand extraInfo empty  and callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand010, Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestSendCommand010" << endl;
    int32_t commandId = LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SendCommand_1200
 * @tc.name  testFingerprintAuthTestSendCommand011
 * @tc.desc  test SendCommand 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSendCommand011, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSendCommand011" << endl;
    int32_t commandId = LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_0100
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1001
 * @tc.desc  test AuthenticateV1_1 scheduleId 0
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1001,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1001" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_0200
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1002
 * @tc.desc  test AuthenticateV1_1 scheduleId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1002,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1002" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_0300
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1003
 * @tc.desc  test AuthenticateV1_1 scheduleId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1003,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1003" << endl;
    uint64_t scheduleId = 0xFFFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_0400
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1004
 * @tc.desc  test AuthenticateV1_1 endAfterFirstFail true
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1004,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1004" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_0500
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1005
 * @tc.desc  test AuthenticateV1_1 endAfterFirstFail false
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1005,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1004" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, false, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_0600
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1006
 * @tc.desc  test AuthenticateV1_1 templateIdList empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1006,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate006" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_0700
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1007
 * @tc.desc  test AuthenticateV1_1 extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1007,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticate007" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_0800
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1008
 * @tc.desc  test AuthenticateV1_1 callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1008,
         Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestAuthenticate008" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_0900
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1009
 * @tc.desc  test AuthenticateV1_1 extraInfo empty and callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1009,
         Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1009" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_1000
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1010
 * @tc.desc  test AuthenticateV1_1 templateIdList empty and callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1010,
         Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1010" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_1100
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1011
 * @tc.desc  test AuthenticateV1_1 templateIdList and extraInfo empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1011,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1011" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_1200
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1012
 * @tc.desc  test AuthenticateV1_1 templateIdList and extraInfo empty and callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1012,
         Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1012" << endl;
    uint64_t scheduleId = 0;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_AuthenticateV1_1_1300
 * @tc.name  testFingerprintAuthTestAuthenticateV1_1013
 * @tc.desc  test AuthenticateV1_1 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestAuthenticateV1_1013,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestAuthenticateV1_1013" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateIdList, true, extraInfo, callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetProperty_0200
 * @tc.name  testFingerprintAuthTestGetProperty001
 * @tc.desc  test GetProperty ptype enum
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetProperty001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetProperty001" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<GetPropertyType> propertyTypes;
    propertyTypes.push_back(OHOS::HDI::FingerprintAuth::V1_1::AUTH_SUB_TYPE);
    propertyTypes.push_back(OHOS::HDI::FingerprintAuth::V1_1::LOCKOUT_DURATION);
    propertyTypes.push_back(OHOS::HDI::FingerprintAuth::V1_1::REMAIN_ATTEMPTS);
    propertyTypes.push_back(OHOS::HDI::FingerprintAuth::V1_1::ENROLL_PROGRESS);
    propertyTypes.push_back(OHOS::HDI::FingerprintAuth::V1_1::SENSOR_INFO);
    Property property;
    FillTestProperty(parcel, property);
    int32_t ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetProperty_0700
 * @tc.name  testFingerprintAuthTestGetProperty006
 * @tc.desc  test GetProperty templateIdList empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetProperty006, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetProperty006" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<GetPropertyType> propertyTypes;
    FillTestGetPropertyTypeVector(parcel, propertyTypes);
    Property property;
    FillTestProperty(parcel, property);
    int32_t ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetProperty_0800
 * @tc.name  testFingerprintAuthTestGetProperty007
 * @tc.desc  test GetProperty property empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetProperty007, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetProperty007" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<GetPropertyType> propertyTypes;
    FillTestGetPropertyTypeVector(parcel, propertyTypes);
    Property property;
    int32_t ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetProperty_0900
 * @tc.name  testFingerprintAuthTestGetProperty008
 * @tc.desc  test GetProperty templateIdList and property empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetProperty008, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetProperty008" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<GetPropertyType> propertyTypes;
    FillTestGetPropertyTypeVector(parcel, propertyTypes);
    Property property;
    int32_t ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetProperty_1000
 * @tc.name  testFingerprintAuthTestGetProperty009
 * @tc.desc  test GetProperty 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetProperty009, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetProperty009" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<GetPropertyType> propertyTypes;
    FillTestGetPropertyTypeVector(parcel, propertyTypes);
    Property property;
    FillTestProperty(parcel, property);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SetCachedTemplates_0200
 * @tc.name  testFingerprintAuthTestSetCachedTemplates001
 * @tc.desc  test SetCachedTemplates templateIdList empty
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSetCachedTemplates001,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSetCachedTemplates001" << endl;
    std::vector<uint64_t> templateIdList;

    int32_t ret = g_executorImpl.SetCachedTemplates(templateIdList);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_SetCachedTemplates_0300
 * @tc.name  testFingerprintAuthTestSetCachedTemplates002
 * @tc.desc  test SetCachedTemplates 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestSetCachedTemplates002,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestSetCachedTemplates002" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);

    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.SetCachedTemplates(templateIdList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_RegisterSaCommandCallback_0200
 * @tc.name  testFingerprintAuthTestRegisterSaCommandCallback001
 * @tc.desc  test SetCachedTemplates callbackObj null
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestRegisterSaCommandCallback001,
         Function | MediumTest | Level2)
{
    cout << "start test testFingerprintAuthTestRegisterSaCommandCallback001" << endl;
    sptr<ISaCommandCallback> callbackObj = nullptr;
    FillTestISaCommandCallback(parcel, callbackObj);

    int32_t ret = g_executorImpl.RegisterSaCommandCallback(nullptr);

    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_RegisterSaCommandCallback_0300
 * @tc.name  testFingerprintAuthTestRegisterSaCommandCallback002
 * @tc.desc  test SetCachedTemplates 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestRegisterSaCommandCallback002,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestRegisterSaCommandCallback002" << endl;
    sptr<ISaCommandCallback> callbackObj = nullptr;
    FillTestISaCommandCallback(parcel, callbackObj);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.RegisterSaCommandCallback(callbackObj);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetExecutorList_0200
 * @tc.name  testFingerprintAuthTestGetExecutorList001
 * @tc.desc  test GetExecutorList 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetExecutorList001,
         Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestGetExecutorList001" << endl;
    FingerprintAuthInterfaceService fingerprint_Interface;
    std::vector<sptr<V1_0::IExecutor>> executorList;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = fingerprint_Interface.GetExecutorList(executorList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_GetExecutorListV1_1_0200
 * @tc.name  testFingerprintAuthTestGetExecutorListV1_1001
 * @tc.desc  test GetExecutorListV1_1 1000 times
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestGetExecutorListV1_1001,
         Function | MediumTest | Level1)
{
    cout << "start test GetExecutorListV1_1" << endl;
    FingerprintAuthInterfaceService fingerprint_Interface;
    std::vector<sptr<V1_1::IExecutor>> executorList;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = fingerprint_Interface.GetExecutorListV1_1(executorList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FingerprintAuth_HDI_OnResult_0100
 * @tc.name  testFingerprintAuthTestOnResult001
 * @tc.desc  test OnResult Callback
 */
HWTEST_F(UserIamFingerprintAuthTestAdditional, testFingerprintAuthTestOnResult001, Function | MediumTest | Level1)
{
    cout << "start test testFingerprintAuthTestOnResult001" << endl;
    uint8_t commandId = parcel.ReadUint8();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(g_onResultFlag, true);
}