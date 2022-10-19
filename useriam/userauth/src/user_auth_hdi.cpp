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

#include "user_auth_hdi.h"
#include "iam_hat_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::UserAuth::V1_0;

static const uint32_t MAX_FUZZ_STRUCT_LEN = 20;
static UserAuthInterfaceService g_service;
int32_t Expectedvalue = NULL;
static OHOS::Parcel parcel;

void UserIamUserAuthTest::SetUpTestCase()
{
}

void UserIamUserAuthTest::TearDownTestCase()
{
}

void UserIamUserAuthTest::SetUp()
{
}

void UserIamUserAuthTest::TearDown()
{
}

static void FillAuthTypeVector(Parcel &parcel, vector<AuthType> &vector)
{
    uint32_t len = parcel.ReadInt32() % MAX_FUZZ_STRUCT_LEN;
    vector.resize(len);
    for (uint32_t i = 0; i < len; i++) {
        vector[i] = static_cast<AuthType>(parcel.ReadInt32());
    }
}

static void FillEnrollParam(Parcel &parcel, EnrollParam &enrollParam)
{
    enrollParam.authType = static_cast<AuthType>(parcel.ReadInt32());
    enrollParam.executorSensorHint = parcel.ReadUint32();
}

static void FillExecutorRegisterInfo(Parcel &parcel, ExecutorRegisterInfo &executorRegisterInfo)
{
    executorRegisterInfo.authType = static_cast<AuthType>(parcel.ReadInt32());
    executorRegisterInfo.executorRole = static_cast<ExecutorRole>(parcel.ReadInt32());
    executorRegisterInfo.executorSensorHint = parcel.ReadUint32();
    executorRegisterInfo.executorMatcher = parcel.ReadUint32();
    executorRegisterInfo.esl = static_cast<ExecutorSecureLevel>(parcel.ReadInt32());
    FillTestUint8Vector(parcel, executorRegisterInfo.publicKey);
}

static void FillExecutorInfo(Parcel &parcel, ExecutorInfo &executorInfo)
{
    executorInfo.executorIndex = parcel.ReadUint64();
    FillExecutorRegisterInfo(parcel, executorInfo.info);
}

static void FillExecutorInfoVector(Parcel &parcel, vector<ExecutorInfo> &vector)
{
    uint32_t len = parcel.ReadInt32() % MAX_FUZZ_STRUCT_LEN;
    vector.resize(len);
    for (uint32_t i = 0; i < len; i++) {
        FillExecutorInfo(parcel, vector[i]);
    }
}

static void FillScheduleInfo(Parcel &parcel, ScheduleInfo &scheduleInfo)
{
    scheduleInfo.scheduleId = parcel.ReadUint64();
    FillTestUint64Vector(parcel, scheduleInfo.templateIds);
    scheduleInfo.authType = static_cast<AuthType>(parcel.ReadInt32());
    scheduleInfo.executorMatcher = parcel.ReadUint32();
    scheduleInfo.scheduleMode = static_cast<ScheduleMode>(parcel.ReadInt32());
    FillExecutorInfoVector(parcel, scheduleInfo.executors);
}

static void FillCredentialInfo(Parcel &parcel, CredentialInfo &credentialInfo)
{
    credentialInfo.credentialId = parcel.ReadUint64();
    credentialInfo.executorIndex = parcel.ReadUint64();
    credentialInfo.templateId = parcel.ReadUint64();
    credentialInfo.authType = static_cast<AuthType>(parcel.ReadInt32());
    credentialInfo.executorMatcher = parcel.ReadUint32();
    credentialInfo.executorSensorHint = parcel.ReadUint32();
}

static void FillEnrolledInfo(Parcel &parcel, EnrolledInfo &enrolledInfo)
{
    enrolledInfo.enrolledId = parcel.ReadUint64();
    enrolledInfo.authType = static_cast<AuthType>(parcel.ReadInt32());
}

static void FillIdentifyResultInfo(Parcel &parcel, IdentifyResultInfo &identifyResultInfo)
{
    identifyResultInfo.result = parcel.ReadInt32();
    identifyResultInfo.userId = parcel.ReadInt32();
    FillTestUint8Vector(parcel, identifyResultInfo.token);
}

static void FillExecutorSendMsg(Parcel &parcel, ExecutorSendMsg &executorSendMsg)
{
    executorSendMsg.executorIndex = parcel.ReadUint32();
    FillTestUint8Vector(parcel, executorSendMsg.msg);
}

static void FillAuthSolution(Parcel &parcel, AuthSolution &authSolution)
{
    authSolution.userId = parcel.ReadInt32();
    authSolution.authTrustLevel = parcel.ReadUint32();
    authSolution.authType = static_cast<AuthType>(parcel.ReadInt32());
    authSolution.executorSensorHint = parcel.ReadUint32();
    FillTestUint8Vector(parcel, authSolution.challenge);
}

static void FillScheduleInfoVector(Parcel &parcel, vector<ScheduleInfo> &vector)
{
    uint32_t len = parcel.ReadInt32() % MAX_FUZZ_STRUCT_LEN;
    vector.resize(len);
    for (uint32_t i = 0; i < len; i++) {
        FillScheduleInfo(parcel, vector[i]);
    }
}

static void FillCredentialInfoVector(Parcel &parcel, vector<CredentialInfo> &vector)
{
    uint32_t len = parcel.ReadInt32() % MAX_FUZZ_STRUCT_LEN;
    vector.resize(len);
    for (uint32_t i = 0; i < len; i++) {
        FillCredentialInfo(parcel, vector[i]);
    }
}

static void FillEnrolledInfoVector(Parcel &parcel, vector<EnrolledInfo> &vector)
{
    uint32_t len = parcel.ReadInt32() % MAX_FUZZ_STRUCT_LEN;
    vector.resize(len);
    for (uint32_t i = 0; i < len; i++) {
        FillEnrolledInfo(parcel, vector[i]);
    }
}

static void FillExecutorSendMsgVector(Parcel &parcel, vector<ExecutorSendMsg> &vector)
{
    uint32_t len = parcel.ReadInt32() % MAX_FUZZ_STRUCT_LEN;
    vector.resize(len);
    for (uint32_t i = 0; i < len; i++) {
        FillExecutorSendMsg(parcel, vector[i]);
    }
}

static void FillAuthResultInfo(Parcel &parcel, AuthResultInfo &authResultInfo)
{
    authResultInfo.result = parcel.ReadUint32();
    authResultInfo.lockoutDuration = parcel.ReadInt32();
    authResultInfo.remainAttempts = parcel.ReadInt32();
    FillExecutorSendMsgVector(parcel, authResultInfo.msgs);
    FillTestUint8Vector(parcel, authResultInfo.token);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0101
 * @tc.name: Test Init
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0101, Function | MediumTest | Level1)
{
    cout << "start Init" << endl;
    int32_t ret = g_service.Init();
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0102
 * @tc.name: Test AddExecutor
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0102, Function | MediumTest | Level1)
{
    cout << "start AddExecutor" << endl;
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    int32_t ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0103
 * @tc.name: Test GetCredential
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0103, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    int32_t ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0104
 * @tc.name: Test GetUserInfo
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0104, Function | MediumTest | Level1)
{
    cout << "start GetUserInfo" << endl;
    int32_t userId = parcel.ReadInt32();
    uint64_t secureUid = parcel.ReadUint64();
    PinSubType pinSubType = static_cast<PinSubType>(parcel.ReadUint32());
    std::vector<EnrolledInfo> infos;
    FillEnrolledInfoVector(parcel, infos);
    int32_t ret = g_service.GetUserInfo(userId, secureUid, pinSubType, infos);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0105
 * @tc.name: Test DeleteUser
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0105, Function | MediumTest | Level1)
{
    cout << "start DeleteUser" << endl;
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    std::vector<CredentialInfo> deletedInfos;
    FillCredentialInfoVector(parcel, deletedInfos);
    int32_t ret = g_service.DeleteUser(userId, authToken, deletedInfos);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0106
 * @tc.name: Test BeginIdentification
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0106, Function | MediumTest | Level1)
{
    cout << "start BeginIdentification" << endl;
    uint64_t contextId = parcel.ReadUint64();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    std::vector<uint8_t> challenge;
    FillTestUint8Vector(parcel, challenge);
    uint32_t executorId = parcel.ReadUint32();
    ScheduleInfo scheduleInfo;
    FillScheduleInfo(parcel, scheduleInfo);
    int32_t ret = g_service.BeginIdentification(contextId, authType, challenge, executorId, scheduleInfo);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0107
 * @tc.name: Test DeleteExecutor
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0107, Function | MediumTest | Level1)
{
    cout << "start DeleteExecutor" << endl;
    uint64_t index = parcel.ReadUint64();
    int32_t ret = g_service.DeleteExecutor(index);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0108
 * @tc.name: Test OpenSession
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0108, Function | MediumTest | Level1)
{
    cout << "start OpenSession" << endl;
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> challenge;
    FillTestUint8Vector(parcel, challenge);
    int32_t ret = g_service.OpenSession(userId, challenge);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0109
 * @tc.name: Test CloseSession
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0109, Function | MediumTest | Level1)
{
    cout << "start CloseSession" << endl;
    int32_t userId = parcel.ReadInt32();
    int32_t ret = g_service.CloseSession(userId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0110
 * @tc.name: Test BeginEnrollment
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0110, Function | MediumTest | Level1)
{
    cout << "start BeginEnrollment" << endl;
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    EnrollParam param;
    FillEnrollParam(parcel, param);
    ScheduleInfo info;
    FillScheduleInfo(parcel, info);
    int32_t ret = g_service.BeginEnrollment(userId, authToken, param, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0111
 * @tc.name: Test UpdateEnrollmentResult
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0111, Function | MediumTest | Level1)
{
    cout << "start UpdateEnrollmentResult" << endl;
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> scheduleResult;
    FillTestUint8Vector(parcel, scheduleResult);
    EnrollResultInfo info = {};
    FillCredentialInfo(parcel, info.oldInfo);
    info.credentialId = parcel.ReadUint64();
    FillTestUint8Vector(parcel, info.rootSecret);
    int32_t ret = g_service.UpdateEnrollmentResult(userId, scheduleResult, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0112
 * @tc.name: Test CancelEnrollment
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0112, Function | MediumTest | Level1)
{
    cout << "start CancelEnrollment" << endl;
    int32_t userId = parcel.ReadInt32();
    int32_t ret = g_service.CancelEnrollment(userId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0113
 * @tc.name: Test DeleteCredential
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0113, Function | MediumTest | Level1)
{
    cout << "start DeleteCredential" << endl;
    int32_t userId = parcel.ReadInt32();
    uint64_t credentialId = parcel.ReadUint64();
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    CredentialInfo info;
    FillCredentialInfo(parcel, info);
    int32_t ret = g_service.DeleteCredential(userId, credentialId, authToken, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0114
 * @tc.name: Test EnforceDeleteUser
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0114, Function | MediumTest | Level1)
{
    cout << "start EnforceDeleteUser" << endl;
    int32_t userId = parcel.ReadInt32();
    std::vector<CredentialInfo> deletedInfos;
    FillCredentialInfoVector(parcel, deletedInfos);
    int32_t ret = g_service.EnforceDeleteUser(userId, deletedInfos);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0115
 * @tc.name: Test BeginAuthentication
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0115, Function | MediumTest | Level1)
{
    cout << "start BeginAuthentication" << endl;
    uint64_t contextId = parcel.ReadUint64();
    AuthSolution param;
    FillAuthSolution(parcel, param);
    std::vector<ScheduleInfo> scheduleInfos;
    FillScheduleInfoVector(parcel, scheduleInfos);
    int32_t ret = g_service.BeginAuthentication(contextId, param, scheduleInfos);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0116
 * @tc.name: Test UpdateAuthenticationResult
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0116, Function | MediumTest | Level1)
{
    cout << "start UpdateAuthenticationResult" << endl;
    uint64_t contextId = parcel.ReadUint64();
    std::vector<uint8_t> scheduleResult;
    FillTestUint8Vector(parcel, scheduleResult);
    AuthResultInfo info;
    FillAuthResultInfo(parcel, info);
    int32_t ret = g_service.UpdateAuthenticationResult(contextId, scheduleResult, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0117
 * @tc.name: Test CancelAuthentication
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0117, Function | MediumTest | Level1)
{
    cout << "start CancelAuthentication" << endl;
    uint64_t contextId = parcel.ReadUint64();
    int32_t ret = g_service.CancelAuthentication(contextId);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0118
 * @tc.name: Test UpdateIdentificationResult
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0118, Function | MediumTest | Level1)
{
    cout << "start UpdateIdentificationResult" << endl;
    uint64_t contextId = parcel.ReadUint64();
    std::vector<uint8_t> scheduleResult;
    FillTestUint8Vector(parcel, scheduleResult);
    IdentifyResultInfo info;
    FillIdentifyResultInfo(parcel, info);
    int32_t ret = g_service.UpdateIdentificationResult(contextId, scheduleResult, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0119
 * @tc.name: Test CancelIdentification
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0119, Function | MediumTest | Level1)
{
    cout << "start CancelIdentification" << endl;
    uint64_t contextId = parcel.ReadUint64();
    int32_t ret = g_service.CancelIdentification(contextId);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0120
 * @tc.name: Test GetAuthTrustLevel
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0120, Function | MediumTest | Level1)
{
    cout << "start GetAuthTrustLevel" << endl;
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    uint32_t authTrustLevel = parcel.ReadUint32();
    int32_t ret = g_service.GetAuthTrustLevel(userId, authType, authTrustLevel);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: Security_IAM_UserAuth_HDI_FUNC_0121
 * @tc.name: Test GetValidSolution
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamUserAuthTest, Security_IAM_UserAuth_HDI_FUNC_0121, Function | MediumTest | Level1)
{
    cout << "start GetValidSolution" << endl;
    int32_t userId = parcel.ReadInt32();
    std::vector<AuthType> authTypes;
    FillAuthTypeVector(parcel, authTypes);
    uint32_t authTrustLevel = parcel.ReadUint32();
    std::vector<AuthType> validTypes;
    FillAuthTypeVector(parcel, validTypes);
    int32_t ret = g_service.GetValidSolution(userId, authTypes, authTrustLevel, validTypes);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}