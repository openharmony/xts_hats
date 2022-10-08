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

#include <hdf_base.h>
#include "iam_hat_test.h"
#include "fingerprint_auth_hdi.h"

#define LOG_LABEL OHOS::UserIam::Common::LABEL_FINGERPRINT_AUTH_IMPL

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::FingerprintAuth::V1_0;

static ExecutorImpl g_executorImpl;
static OHOS::Parcel parcel;

void UserIamFingerprintAuthTest::SetUpTestCase()
{
}

void UserIamFingerprintAuthTest::TearDownTestCase()
{
}

void UserIamFingerprintAuthTest::SetUp()
{
}

void UserIamFingerprintAuthTest::TearDown()
{
}

class DummyIExecutorCallback : public IExecutorCallback {
public:
    DummyIExecutorCallback(int32_t result, int32_t acquire) : result_(result), acquire_(acquire)
    {
    }

    int32_t OnResult(int32_t result, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "result is " << result << " extraInfo len is " << extraInfo.size() << endl;
        return result_;
    }

    int32_t OnTip(int32_t acquire, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "result is " << acquire << " extraInfo len is " << extraInfo.size() << endl;
        return acquire_;
    }

private:
    int32_t result_;
    int32_t acquire_;
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
    cout << "success" << endl;
}

static void FillTestTemplateInfo(Parcel &parcel, TemplateInfo &templateInfo)
{
    templateInfo.executorType = parcel.ReadUint32();
    templateInfo.lockoutDuration = parcel.ReadInt32();
    templateInfo.remainAttempts = parcel.ReadInt32();
    FillTestUint8Vector(parcel, templateInfo.extraInfo);
    cout << "success" << endl;
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
    cout << "success" << endl;
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0101
 * @tc.name: Test SendCommand
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0101, Function | MediumTest | Level1)
{
    cout << "start test SendCommand" << endl;
    uint8_t commandId = parcel.ReadUint8();
    cout << "commandId is " << commandId << endl;
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.SendCommand(commandId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0102
 * @tc.name: Test Cancel
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0102, Function | MediumTest | Level1)
{
    cout << "start test Cancel" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    int32_t ret = g_executorImpl.Cancel(scheduleId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0103
 * @tc.name: Test Delete
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0103, Function | MediumTest | Level1)
{
    cout << "start test Delete" << endl;
    std::vector<uint64_t> templateIdList;
    FillTestUint64Vector(parcel, templateIdList);
    int32_t ret = g_executorImpl.Delete(templateIdList);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0104
 * @tc.name: Test GetExecutorInfo
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0104, Function | MediumTest | Level1)
{
    cout << "start test GetExecutorInfo" << endl;
    ExecutorInfo executorInfo;
    FillTestExecutorInfo(parcel, executorInfo);
    int32_t ret = g_executorImpl.GetExecutorInfo(executorInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0105
 * @tc.name: Test GetTemplateInfo
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0105, Function | MediumTest | Level1)
{
    cout << "start test GetTemplateInfo" << endl;
    uint64_t templateId = parcel.ReadUint64();
    TemplateInfo templateInfo;
    FillTestTemplateInfo(parcel, templateInfo);
    int32_t ret = g_executorImpl.GetTemplateInfo(templateId, templateInfo);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0106
 * @tc.name: Test OnRegisterFinish
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0106, Function | MediumTest | Level1)
{
    cout << "start test OnRegisterFinish" << endl;
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
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0107
 * @tc.name: Test Enroll
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0107, Function | MediumTest | Level1)
{
    cout << "start test Enroll" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Enroll(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0108
 * @tc.name: Test Authenticate
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0108, Function | MediumTest | Level1)
{
    cout << "start test Authenticate" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
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
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0109
 * @tc.name: Test Identify
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0109, Function | MediumTest | Level1)
{
    cout << "start test Identify" << endl;
    uint64_t scheduleId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Identify(scheduleId, extraInfo, callbackObj);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_Fingerprint_HDI_FUNC_0110
 * @tc.name: Test GetExecutorList
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamFingerprintAuthTest, Security_IAM_Fingerprint_HDI_FUNC_0110, Function | MediumTest | Level1)
{
    cout << "start test GetExecutorList" << endl;
    FingerprintAuthInterfaceService fingerprint_Interface;
    std::vector<sptr<IExecutor>> executorList;
    int32_t ret = fingerprint_Interface.GetExecutorList(executorList);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}