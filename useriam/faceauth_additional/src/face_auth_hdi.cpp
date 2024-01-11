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
#include "iconsumer_surface.h"
#include "face_auth_hdi.h"

#define LOG_LABEL OHOS::UserIam::Common::LABEL_FACE_AUTH_IMPL

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::FaceAuth;
using namespace OHOS::HDI::FaceAuth::V1_0;
using Property = OHOS::HDI::FaceAuth::V1_1::Property;
using SaCommandParamNone = OHOS::HDI::FaceAuth::V1_1::SaCommandParamNone;
using SaCommandParam = OHOS::HDI::FaceAuth::V1_1::SaCommandParam;
using SaCommand = OHOS::HDI::FaceAuth::V1_1::SaCommand;

static ExecutorImpl g_executorImpl;
static OHOS::Parcel parcel;
bool g_onResultFlag = false;
bool g_onTipFlag = false;
bool g_onSaCommandsFlag = false;

void UserIamFaceAuthTestAdditional::SetUpTestCase() {}

void UserIamFaceAuthTestAdditional::TearDownTestCase() {}

void UserIamFaceAuthTestAdditional::SetUp() {}

void UserIamFaceAuthTestAdditional::TearDown() {}

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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetExecutorInfo_0200
 * @tc.name  testFaceAuthTestGetExecutorInfo001
 * @tc.desc  test GetExecutorInfo not empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetExecutorInfo001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetExecutorInfo001" << endl;
    ExecutorInfo executorInfo;
    int32_t ret = g_executorImpl.GetExecutorInfo(executorInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetExecutorInfo_0300
 * @tc.name  testFaceAuthTestGetExecutorInfo002
 * @tc.desc  test GetExecutorInfo 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetExecutorInfo002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetExecutorInfo002" << endl;
    ExecutorInfo executorInfo;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.GetExecutorInfo(executorInfo);
        cout << "ret " << i << " is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetTemplateInfo_0200
 * @tc.name  testFaceAuthTestGetTemplateInfo001
 * @tc.desc  test GetTemplateInfo templateId 0
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetTemplateInfo001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetTemplateInfo001" << endl;
    uint64_t templateId = 0;
    TemplateInfo templateInfo;
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetTemplateInfo_0300
 * @tc.name  testFaceAuthTestGetTemplateInfo002
 * @tc.desc  test GetTemplateInfo templateId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetTemplateInfo002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetTemplateInfo002" << endl;
    uint64_t templateId = 0x7FFFFFFFFFFFFFFF;
    TemplateInfo templateInfo;
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetTemplateInfo_0400
 * @tc.name  testFaceAuthTestGetTemplateInfo003
 * @tc.desc  test GetTemplateInfo templateId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetTemplateInfo003, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetTemplateInfo003" << endl;
    uint64_t templateId = 0xFFFFFFFFFFFFFFFF;
    TemplateInfo templateInfo;
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetTemplateInfo_0500
 * @tc.name  testFaceAuthTestGetTemplateInfo004
 * @tc.desc  test GetTemplateInfo templateId 0x10
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetTemplateInfo004, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetTemplateInfo004" << endl;
    uint64_t templateId = 0x10;
    TemplateInfo templateInfo;
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetTemplateInfo_0600
 * @tc.name  testFaceAuthTestGetTemplateInfo005
 * @tc.desc  test GetTemplateInfo 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetTemplateInfo005, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetTemplateInfo005" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_OnRegisterFinish_0200
 * @tc.name  testFaceAuthTestOnRegisterFinish001
 * @tc.desc  test OnRegisterFinish templateIdList empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestOnRegisterFinish001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestOnRegisterFinish001" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_OnRegisterFinish_0300
 * @tc.name  testFaceAuthTestOnRegisterFinish002
 * @tc.desc  test OnRegisterFinish frameworkPublicKey empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestOnRegisterFinish002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestOnRegisterFinish002" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_OnRegisterFinish_0400
 * @tc.name  testFaceAuthTestOnRegisterFinish003
 * @tc.desc  test OnRegisterFinish extraInfo empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestOnRegisterFinish003, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestOnRegisterFinish003" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_OnRegisterFinish_0500
 * @tc.name  testFaceAuthTestOnRegisterFinish004
 * @tc.desc  test OnRegisterFinish frameworkPublicKey and extraInfo empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestOnRegisterFinish004, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestOnRegisterFinish004" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> frameworkPublicKey;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_OnRegisterFinish_0600
 * @tc.name  testFaceAuthTestOnRegisterFinish005
 * @tc.desc  test OnRegisterFinish templateIdList and extraInfo empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestOnRegisterFinish005, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestOnRegisterFinish005" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> frameworkPublicKey;
    FillTestUint8Vector(parcel, frameworkPublicKey);
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_OnRegisterFinish_0700
 * @tc.name  testFaceAuthTestOnRegisterFinish006
 * @tc.desc  test OnRegisterFinish templateIdList and frameworkPublicKey empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestOnRegisterFinish006, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestOnRegisterFinish006" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> frameworkPublicKey;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_OnRegisterFinish_0800
 * @tc.name  testFaceAuthTestOnRegisterFinish007
 * @tc.desc  test OnRegisterFinish templateIdList and frameworkPublicKey and extraInfo empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestOnRegisterFinish007, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestOnRegisterFinish007" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> frameworkPublicKey;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_OnRegisterFinish_0900
 * @tc.name  testFaceAuthTestOnRegisterFinish008
 * @tc.desc  test OnRegisterFinish 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestOnRegisterFinish008, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestOnRegisterFinish008" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Enroll_0200
 * @tc.name  testFaceAuthTestEnroll001
 * @tc.desc test Enroll scheduleId 0
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestEnroll001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestEnroll001" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Enroll_0300
 * @tc.name  testFaceAuthTestEnroll002
 * @tc.desc  test Enroll scheduleId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestEnroll002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestEnroll002" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Enroll_0400
 * @tc.name  testFaceAuthTestEnroll003
 * @tc.desc  test Enroll scheduleId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestEnroll003, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestEnroll003" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Enroll_0500
 * @tc.name  testFaceAuthTestEnroll004
 * @tc.desc  test Enroll extraInfo empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestEnroll004, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestEnroll004" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Enroll_0600
 * @tc.name  testFaceAuthTestEnroll005
 * @tc.desc  test Enroll callbackObj nullptr
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestEnroll005, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestEnroll005" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Enroll_0700
 * @tc.name  testFaceAuthTestEnroll006
 * @tc.desc  test Enroll extraInfo empty and callbackObj nullptr
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestEnroll006, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestEnroll006" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Enroll_0800
 * @tc.name  testFaceAuthTestEnroll007
 * @tc.desc  test Enroll 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestEnroll007, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestEnroll007" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_0200
 * @tc.name  testFaceAuthTestAuthenticate001
 * @tc.desc  test Authenticate scheduleId 0
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestAuthenticate001" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_0300
 * @tc.name  testFaceAuthTestAuthenticate002
 * @tc.desc  test Authenticate scheduleId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestAuthenticate002" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_0400
 * @tc.name  testFaceAuthTestAuthenticate003
 * @tc.desc  test Authenticate scheduleId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate003, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestAuthenticate003" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_0500
 * @tc.name  testFaceAuthTestAuthenticate004
 * @tc.descc  test Authenticate templateIdList empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate004, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestAuthenticate004" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_0600
 * @tc.name  testFaceAuthTestAuthenticate005
 * @tc.desc  test Authenticate extraInfo empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate005, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestAuthenticate005" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_0700
 * @tc.name  testFaceAuthTestAuthenticate006
 * @tc.desc  test Authenticate callbackObj nullptr
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate006, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestAuthenticate006" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_0800
 * @tc.name  testFaceAuthTestAuthenticate007
 * @tc.desc  test Authenticate extraInfo empty and callbackObj nullptr
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate007, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestAuthenticate007" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_0900
 * @tc.name  testFaceAuthTestAuthenticate008
 * @tc.desc  test Authenticate templateIdList empty and callbackObj nullptr
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate008, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestAuthenticate008" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_1000
 * @tc.name  testFaceAuthTestAuthenticate009
 * @tc.desc  test Authenticate templateIdList and extraInfo empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate009, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestAuthenticate009" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_1100
 * @tc.name  testFaceAuthTestAuthenticate010
 * @tc.desc  test Authenticate templateIdList and extraInfo empty and callbackObj nullptr
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate010, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestAuthenticate010" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint64_t> templateIdList;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Authenticate_1200
 * @tc.name  testFaceAuthTestAuthenticate011
 * @tc.desc  test Authenticate 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestAuthenticate011, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestAuthenticate011" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Identify_0200
 * @tc.name  testFaceAuthTestIdentify001
 * @tc.desc  test Identify scheduleId 0
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestIdentify001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestIdentify001" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Identify_0300
 * @tc.name  testFaceAuthTestIdentify002
 * @tc.desc  test Identify scheduleId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestIdentify002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestIdentify002" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Identify_0400
 * @tc.name  testFaceAuthTestIdentify003
 * @tc.desc  test Identify scheduleId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestIdentify003, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestIdentify003" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Identify_0500
 * @tc.name  testFaceAuthTestIdentify004
 * @tc.desc  test Identify extraInfo empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestIdentify004, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestIdentify004" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Identify_0600
 * @tc.name  testFaceAuthTestIdentify005
 * @tc.desc  test Identify callbackObj null
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestIdentify005, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestIdentify005" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Identify_0700
 * @tc.name  testFaceAuthTestIdentify006
 * @tc.desc  test Identify extraInfo empty and callbackObj null
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestIdentify006, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestIdentify006" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Identify_0800
 * @tc.name  testFaceAuthTestIdentify007
 * @tc.desc  test Identify 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestIdentify007, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestIdentify007" << endl;
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
 * @tc.number: SUB_Security_Iam_FaceAuth_HDI_Delete_0200
 * @tc.name: testFaceAuthTestDelete001
 * @tc.desc  test Delete templateIdList empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestDelete001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestDelete001" << endl;
    std::vector<uint64_t> templateIdList;
    int32_t ret = g_executorImpl.Delete(templateIdList);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_Iam_FaceAuth_HDI_Delete_0300
 * @tc.name: testFaceAuthTestDelete002
 * @tc.desc  test Delete 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestDelete002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestDelete002" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Cancel_0200
 * @tc.name  testFaceAuthTestCancel001
 * @tc.desc  test Cancel scheduleId 0
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestCancel001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestCancel001" << endl;
    uint64_t scheduleId = 0;
    int32_t ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Cancel_0300
 * @tc.name  testFaceAuthTestCancel002
 * @tc.desc  test Cancel scheduleId 0x7FFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestCancel002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestCancel002" << endl;
    uint64_t scheduleId = 0x7FFFFFFFFFFFFFFF;
    int32_t ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Cancel_0400
 * @tc.name  testFaceAuthTestCancel003
 * @tc.desc  test Cancel scheduleId 0xFFFFFFFFFFFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestCancel003, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestCancel003" << endl;
    uint64_t scheduleId = 0xFFFFFFFFFFFFFFFF;
    int32_t ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_Cancel_0500
 * @tc.name  testFaceAuthTestCancel004
 * @tc.desc  test Cancel 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestCancel004, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestCancel004" << endl;
    uint64_t scheduleId = 0;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.Cancel(scheduleId);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SendCommand_0200
 * @tc.name  testFaceAuthTestSendCommand001
 * @tc.desc  test SendCommand commandId enum
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSendCommand001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSendCommand001" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SendCommand_0500
 * @tc.name  testFaceAuthTestSendCommand004
 * @tc.desc  test SendCommand commandId 0
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSendCommand004, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSendCommand004" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SendCommand_0600
 * @tc.name  testFaceAuthTestSendCommand005
 * @tc.desc  test SendCommand commandId 0x7FFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSendCommand005, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSendCommand005" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SendCommand_0700
 * @tc.name  testFaceAuthTestSendCommand006
 * @tc.desc  test SendCommand commandId -0x01
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSendCommand006, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSendCommand006" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SendCommand_0800
 * @tc.name  testFaceAuthTestSendCommand007
 * @tc.desc  test SendCommand commandId -0x7FFFFFFF
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSendCommand007, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSendCommand007" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SendCommand_0900
 * @tc.name  testFaceAuthTestSendCommand008
 * @tc.desc  test SendCommand extraInfo empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSendCommand008, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSendCommand008" << endl;
    int32_t commandId = LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SendCommand_1000
 * @tc.name  testFaceAuthTestSendCommand009
 * @tc.desc  test SendCommand callbackObj null
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSendCommand009, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestSendCommand009" << endl;
    int32_t commandId = LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SendCommand_1100
 * @tc.name  testFaceAuthTestSendCommand010
 * @tc.desc  test SendCommand extraInfo empty and callbackObj null
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSendCommand010, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestSendCommand010" << endl;
    int32_t commandId = LOCK_TEMPLATE;
    std::vector<uint8_t> extraInfo;
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SendCommand_1200
 * @tc.name  testFaceAuthTestSendCommand011
 * @tc.desc  test SendCommand 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSendCommand011, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSendCommand011" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SetBufferProducer_0200
 * @tc.name  testFaceAuthTestSetBufferProducer001
 * @tc.desc  test SetBufferProducer producerSequenceable null
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSetBufferProducer001, Function | MediumTest | Level2)
{
    cout << "start test testFaceAuthTestSetBufferProducer001" << endl;
    int32_t ret = g_executorImpl.SetBufferProducer(nullptr);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SetBufferProducer_0300
 * @tc.name  testFaceAuthTestSetBufferProducer002
 * @tc.desc  test SetBufferProducer 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSetBufferProducer002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSetBufferProducer002" << endl;
    sptr<IBufferProducer> bufferProducer = nullptr;
    if (parcel.ReadBool()) {
        auto surface = IConsumerSurface::Create();
        if (surface == nullptr) {
            cout << "CreateSurfaceAsConsumer fail" << endl;
        } else {
            bufferProducer = surface->GetProducer();
        }
    }
    sptr<BufferProducerSequenceable> producerSequenceable =
        new (std::nothrow) BufferProducerSequenceable(bufferProducer);
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_executorImpl.SetBufferProducer(producerSequenceable);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetProperty_0200
 * @tc.name  testFaceAuthTestGetProperty001
 * @tc.desc  test GetProperty ptype enum
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetProperty001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetProperty001" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<GetPropertyType> propertyTypes;
    propertyTypes.push_back(OHOS::HDI::FaceAuth::V1_1::AUTH_SUB_TYPE);
    propertyTypes.push_back(OHOS::HDI::FaceAuth::V1_1::LOCKOUT_DURATION);
    propertyTypes.push_back(OHOS::HDI::FaceAuth::V1_1::REMAIN_ATTEMPTS);
    propertyTypes.push_back(OHOS::HDI::FaceAuth::V1_1::ENROLL_PROGRESS);
    propertyTypes.push_back(OHOS::HDI::FaceAuth::V1_1::SENSOR_INFO);
    Property property;
    FillTestProperty(parcel, property);
    int32_t ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetProperty_0700
 * @tc.name  testFaceAuthTestGetProperty006
 * @tc.desc  test GetProperty templateIdList empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetProperty006, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetProperty006" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetProperty_0800
 * @tc.name  testFaceAuthTestGetProperty007
 * @tc.desc  test GetProperty property empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetProperty007, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetProperty007" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetProperty_0900
 * @tc.name  testFaceAuthTestGetProperty008
 * @tc.desc  test GetProperty templateIdList and property empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetProperty008, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetProperty008" << endl;
    std::vector<uint64_t> templateIdList;
    std::vector<GetPropertyType> propertyTypes;
    FillTestGetPropertyTypeVector(parcel, propertyTypes);
    Property property;
    int32_t ret = g_executorImpl.GetProperty(templateIdList, propertyTypes, property);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetProperty_1000
 * @tc.name  testFaceAuthTestGetProperty009
 * @tc.desc  test GetProperty 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetProperty009, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestGetProperty009" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SetCachedTemplates_0200
 * @tc.name  testFaceAuthTestSetCachedTemplates001
 * @tc.desc  test SetCachedTemplates templateIdList empty
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSetCachedTemplates001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSetCachedTemplates001" << endl;
    std::vector<uint64_t> templateIdList;

    int32_t ret = g_executorImpl.SetCachedTemplates(templateIdList);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_SetCachedTemplates_0300
 * @tc.name  testFaceAuthTestSetCachedTemplates002
 * @tc.desc  test SetCachedTemplates 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestSetCachedTemplates002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestSetCachedTemplates002" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_RegisterSaCommandCallback_0200
 * @tc.name  testFaceAuthTestRegisterSaCommandCallback001
 * @tc.desc  test RegisterSaCommandCallback callbackObj null
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestRegisterSaCommandCallback001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestRegisterSaCommandCallback001" << endl;

    int32_t ret = g_executorImpl.RegisterSaCommandCallback(nullptr);

    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_RegisterSaCommandCallback_0300
 * @tc.name  testFaceAuthTestRegisterSaCommandCallback002
 * @tc.desc  test RegisterSaCommandCallback 1000
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestRegisterSaCommandCallback002, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestRegisterSaCommandCallback002" << endl;
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
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetExecutorList_0200
 * @tc.name  testFaceAuthTestGetExecutorList001
 * @tc.desc  test GetExecutorList 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetExecutorList001, Function | MediumTest | Level1)
{
    cout << "start test GetExecutorList" << endl;
    FaceAuthInterfaceService faceauth_Interface;
    std::vector<sptr<V1_0::IExecutor>> executorList;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = faceauth_Interface.GetExecutorList(executorList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_GetExecutorListV1_1_0200
 * @tc.name  testFaceAuthTestGetExecutorListV1_1001
 * @tc.desc  test GetExecutorListV1_1 1000 times
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestGetExecutorListV1_1001, Function | MediumTest | Level1)
{
    cout << "start test GetExecutorListV1_1" << endl;
    FaceAuthInterfaceService faceauth_Interface;
    std::vector<sptr<V1_1::IExecutor>> executorList;
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = faceauth_Interface.GetExecutorListV1_1(executorList);
        cout << "ret is " << ret << endl;
        EXPECT_EQ(ret, 0);
    }
}
/**
 * @tc.number  SUB_Security_Iam_FaceAuth_HDI_OnResult_0100
 * @tc.name  testFaceAuthTestOnResult001
 * @tc.desc  test OnResult Callback
 */
HWTEST_F(UserIamFaceAuthTestAdditional, testFaceAuthTestOnResult001, Function | MediumTest | Level1)
{
    cout << "start test testFaceAuthTestOnResult001" << endl;
    uint8_t commandId = parcel.ReadUint8();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(g_onResultFlag, true);
}