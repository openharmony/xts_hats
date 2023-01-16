/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "pin_auth_hdi.h"
#include "iam_hat_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::PinAuth::V1_0;

static ExecutorImpl g_executorImpl(make_shared<OHOS::UserIam::PinAuth::PinAuth>());
static OHOS::Parcel parcel;
int32_t Expectedvalue = NULL;

void UserIamPinAuthTest::SetUpTestCase()
{
}

void UserIamPinAuthTest::TearDownTestCase()
{
}

void UserIamPinAuthTest::SetUp()
{
}

void UserIamPinAuthTest::TearDown()
{
}

class DummyIExecutorCallback : public IExecutorCallback {
public:
    DummyIExecutorCallback(int32_t onResultResult, int32_t onGetDataResult)
        : onResultResult_(onResultResult), onGetDataResult_(onGetDataResult)
    {
    }

    int32_t OnResult(int32_t result, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "result is " << result << " extraInfo len is " << extraInfo.size() << endl;
        return onResultResult_;
    }

    int32_t OnGetData(uint64_t scheduleId, const std::vector<uint8_t> &salt, uint64_t authSubType) override
    {
        cout << "scheduleId is " << scheduleId << endl;
        cout << " salt len is " << salt.size() << endl;
        cout << " authSubType is " << authSubType << endl;
        return onGetDataResult_;
    }

private:
    int32_t onResultResult_;
    int32_t onGetDataResult_;
};

static void FillTestExecutorInfo(Parcel &parcel, ExecutorInfo &executorInfo)
{
    executorInfo.sensorId = parcel.ReadUint16();
    executorInfo.executorType = parcel.ReadUint32();
    executorInfo.executorRole = static_cast<ExecutorRole>(parcel.ReadInt32());
    executorInfo.authType = static_cast<AuthType>(parcel.ReadInt32());
    executorInfo.esl = static_cast<ExecutorSecureLevel>(parcel.ReadInt32());
    FillTestUint8Vector(parcel, executorInfo.publicKey);
    FillTestUint8Vector(parcel, executorInfo.extraInfo);
}

static void FillTestTemplateInfo(Parcel &parcel, TemplateInfo &templateInfo)
{
    templateInfo.executorType = parcel.ReadUint32();
    templateInfo.lockoutDuration = parcel.ReadInt32();
    templateInfo.remainAttempts = parcel.ReadInt32();
    FillTestUint8Vector(parcel, templateInfo.extraInfo);
}

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
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0101
 * @tc.name: Test GetExecutorInfo
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTest, Security_IAM_PinAuth_HDI_FUNC_0101, Function | MediumTest | Level1)
{
    cout << "start GetExecutorInfo" << endl;
    ExecutorInfo executorInfo;
    FillTestExecutorInfo(parcel, executorInfo);
    int32_t ret = g_executorImpl.GetExecutorInfo(executorInfo);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0102
 * @tc.name: Test GetTemplateInfo
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTest, Security_IAM_PinAuth_HDI_FUNC_0102, Function | MediumTest | Level1)
{
    cout << "start GetTemplateInfo" << endl;
    uint64_t templateId = parcel.ReadUint64();
    TemplateInfo templateInfo;
    FillTestTemplateInfo(parcel, templateInfo);
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0103
 * @tc.name: Test OnRegisterFinish
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTest, Security_IAM_PinAuth_HDI_FUNC_0103, Function | MediumTest | Level1)
{
    cout << "start OnRegisterFinish" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    std::vector<uint8_t> frameworkPublicKey;
    FillTestUint8Vector(parcel, frameworkPublicKey);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    int32_t ret = g_executorImpl.OnRegisterFinish(templateIdList, frameworkPublicKey, extraInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0104
 * @tc.name: Test OnSetData
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTest, Security_IAM_PinAuth_HDI_FUNC_0104, Function | MediumTest | Level1)
{
    cout << "start OnSetData" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t authSubType = parcel.ReadUint64();
    std::vector<uint8_t> data;
    FillTestUint8Vector(parcel, data);
    int32_t ret = g_executorImpl.OnSetData(scheduleId, authSubType, data);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0105
 * @tc.name: Test Enroll
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTest, Security_IAM_PinAuth_HDI_FUNC_0105, Function | MediumTest | Level1)
{
    cout << "start Enroll" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0106
 * @tc.name: Test Authenticate
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTest, Security_IAM_PinAuth_HDI_FUNC_0106, Function | MediumTest | Level1)
{
    cout << "start Authenticate" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0107
 * @tc.name: Test Delete
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTest, Security_IAM_PinAuth_HDI_FUNC_0107, Function | MediumTest | Level1)
{
    cout << "start Delete" << endl;
    uint64_t templateId = parcel.ReadUint64();
    int32_t ret = g_executorImpl.Delete(templateId);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0108
 * @tc.name: Test Cancel
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTest, Security_IAM_PinAuth_HDI_FUNC_0108, Function | MediumTest | Level1)
{
    cout << "start Cancel" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    int32_t ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_FUNC_0109
 * @tc.name: Test SendCommand
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTest, Security_IAM_PinAuth_HDI_FUNC_0109, Function | MediumTest | Level1)
{
    cout << "start SendCommand" << endl;
    int32_t commandId = parcel.ReadInt32();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
