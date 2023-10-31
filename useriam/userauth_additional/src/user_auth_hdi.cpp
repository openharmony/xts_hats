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

#include "iam_hat_test.h"
#include "user_auth_hdi_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::UserAuth;
using namespace OHOS::HDI::UserAuth::V1_2;

static const uint32_t MAX_FUZZ_STRUCT_LEN = 20;
static UserAuthInterfaceService g_service;
int32_t Expectedvalue = 0;
static OHOS::Parcel parcel;

void UserIamUserAuthTestAdditional::SetUpTestCase() {}

void UserIamUserAuthTestAdditional::TearDownTestCase() {}

void UserIamUserAuthTestAdditional::SetUp() {}

void UserIamUserAuthTestAdditional::TearDown() {}

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

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0200
 * @tc.name  : testAddExecutor001
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor001, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.authType = ALL;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0300
 * @tc.name  : testAddExecutor002
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor002, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.authType = PIN;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0400
 * @tc.name  : testAddExecutor003
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor003, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.authType = FACE;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0500
 * @tc.name  : testAddExecutor004
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor004, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.authType = FINGERPRINT;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0600
 * @tc.name  : testAddExecutor005
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor005, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.executorRole = COLLECTOR;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0700
 * @tc.name  : testAddExecutor006
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor006, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.executorRole = VERIFIER;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0800
 * @tc.name  : testAddExecutor007
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor007, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.executorRole = ALL_IN_ONE;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0900
 * @tc.name  : testAddExecutor008
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor008, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.executorSensorHint = 0;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1000
 * @tc.name  : testAddExecutor009
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor009, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.executorSensorHint = -1;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1100
 * @tc.name  : testAddExecutor010
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor010, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.executorMatcher = -1;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1200
 * @tc.name  : testAddExecutor011
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor011, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.esl = ESL0;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1300
 * @tc.name  : testAddExecutor012
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor012, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.esl = ESL1;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1400
 * @tc.name  : testAddExecutor013
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor013, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.esl = ESL2;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1500
 * @tc.name  : testAddExecutor014
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor014, Function | MediumTest | Level2)
{
    cout << "start AddExecutor" << endl;
    int32_t ret = g_service.Init();
    ExecutorRegisterInfo info;
    FillExecutorRegisterInfo(parcel, info);
    uint64_t index = parcel.ReadUint64();
    std::vector<uint8_t> publicKey;
    FillTestUint8Vector(parcel, publicKey);
    std::vector<uint64_t> templateIds;
    FillTestUint64Vector(parcel, templateIds);
    cout << "authType" << info.authType << endl;
    info.esl = ESL3;
    ret = g_service.AddExecutor(info, index, publicKey, templateIds);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1600
 * @tc.name  : testDeleteExecutor001
 * @tc.desc  : An incorrect actuator index was passed
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteExecutor001, Function | MediumTest | Level2)
{
    cout << "start DeleteExecutor" << endl;
    int32_t ret = g_service.Init();
    uint64_t index = -1;
    ret = g_service.DeleteExecutor(index);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1700
 * @tc.name  : testOpenSession001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testOpenSession001, Function | MediumTest | Level1)
{
    cout << "start OpenSession" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    std::vector<uint8_t> challenge;
    FillTestUint8Vector(parcel, challenge);
    ret = g_service.OpenSession(userId, challenge);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1800
 * @tc.name  : testCloseSession001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCloseSession001, Function | MediumTest | Level1)
{
    cout << "start CloseSession" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    ret = g_service.CloseSession(userId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1900
 * @tc.name  : testBeginEnrollment001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginEnrollment001, Function | MediumTest | Level2)
{
    cout << "start BeginEnrollment" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    EnrollParam param;
    FillEnrollParam(parcel, param);
    ScheduleInfo info;
    FillScheduleInfo(parcel, info);
    ret = g_service.BeginEnrollment(userId, authToken, param, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2000
 * @tc.name  : testCancelAuthentication001
 * @tc.desc  : Pass the wrong contextId context index to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelAuthentication001, Function | MediumTest | Level2)
{
    cout << "start CancelAuthentication" << endl;
    int32_t ret = g_service.Init();
    uint64_t contextId = -1;
    ret = g_service.CancelAuthentication(contextId);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2100
 * @tc.name  : testCancelAuthentication002
 * @tc.desc  : Pass the wrong contextId context index to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelAuthentication002, Function | MediumTest | Level2)
{
    cout << "start CancelAuthentication" << endl;
    int32_t ret = g_service.Init();
    uint64_t contextId = 0;
    ret = g_service.CancelAuthentication(contextId);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2200
 * @tc.name  : testBeginIdentification001
 * @tc.desc  : Pass the wrong contextId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification001, Function | MediumTest | Level2)
{
    cout << "start BeginIdentification" << endl;
    int32_t ret = g_service.Init();
    uint64_t contextId = -1;
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    std::vector<uint8_t> challenge;
    FillTestUint8Vector(parcel, challenge);
    uint32_t executorId = parcel.ReadUint32();
    ScheduleInfo scheduleInfo;
    FillScheduleInfo(parcel, scheduleInfo);
    ret = g_service.BeginIdentification(contextId, authType, challenge, executorId, scheduleInfo);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2300
 * @tc.name  : testUpdateEnrollmentResult001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateEnrollmentResult001, Function | MediumTest | Level2)
{
    cout << "start UpdateEnrollmentResult" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    std::vector<uint8_t> scheduleResult;
    FillTestUint8Vector(parcel, scheduleResult);
    EnrollResultInfo info = {};
    FillCredentialInfo(parcel, info.oldInfo);
    info.credentialId = parcel.ReadUint64();
    FillTestUint8Vector(parcel, info.rootSecret);
    ret = g_service.UpdateEnrollmentResult(userId, scheduleResult, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2400
 * @tc.name  : testUpdateEnrollmentResult002
 * @tc.desc  : Modify the error registration result and determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateEnrollmentResult002, Function | MediumTest | Level2)
{
    cout << "start UpdateEnrollmentResult" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> scheduleResult(1);
    FillTestUint8Vector(parcel, scheduleResult);
    scheduleResult[0] = -1;
    EnrollResultInfo info = {};
    FillCredentialInfo(parcel, info.oldInfo);
    info.credentialId = parcel.ReadUint64();
    FillTestUint8Vector(parcel, info.rootSecret);
    ret = g_service.UpdateEnrollmentResult(userId, scheduleResult, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2500
 * @tc.name  : testUpdateEnrollmentResult003
 * @tc.desc  : Modify the error registration result and determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateEnrollmentResult003, Function | MediumTest | Level2)
{
    cout << "start UpdateEnrollmentResult" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> scheduleResult(1);
    FillTestUint8Vector(parcel, scheduleResult);
    scheduleResult[0] = 0;
    EnrollResultInfo info = {};
    FillCredentialInfo(parcel, info.oldInfo);
    info.credentialId = parcel.ReadUint64();
    FillTestUint8Vector(parcel, info.rootSecret);
    ret = g_service.UpdateEnrollmentResult(userId, scheduleResult, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2600
 * @tc.name  : testCancelEnrollment001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelEnrollment001, Function | MediumTest | Level1)
{
    cout << "start CancelEnrollment" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    ret = g_service.CancelEnrollment(userId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2700
 * @tc.name  : testCancelEnrollment002
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelEnrollment002, Function | MediumTest | Level1)
{
    cout << "start CancelEnrollment" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32() + 1;
    ret = g_service.CancelEnrollment(userId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2800
 * @tc.name  : testDeleteCredential001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteCredential001, Function | MediumTest | Level2)
{
    cout << "start DeleteCredential" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    uint64_t credentialId = parcel.ReadUint64();
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    CredentialInfo info;
    FillCredentialInfo(parcel, info);
    ret = g_service.DeleteCredential(userId, credentialId, authToken, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2900
 * @tc.name  : testDeleteCredential002
 * @tc.desc  : Pass the wrong authentication token to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteCredential002, Function | MediumTest | Level2)
{
    cout << "start DeleteCredential" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    uint64_t credentialId = -1;
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    CredentialInfo info;
    FillCredentialInfo(parcel, info);
    ret = g_service.DeleteCredential(userId, credentialId, authToken, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3000
 * @tc.name  : testDeleteCredential003
 * @tc.desc  : Pass the wrong authentication token to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteCredential003, Function | MediumTest | Level2)
{
    cout << "start DeleteCredential" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    uint64_t credentialId = parcel.ReadUint64() + 1;
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    CredentialInfo info;
    FillCredentialInfo(parcel, info);
    ret = g_service.DeleteCredential(userId, credentialId, authToken, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3100
 * @tc.name  : testDeleteCredential004
 * @tc.desc  : Pass the wrong authentication token to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteCredential004, Function | MediumTest | Level2)
{
    cout << "start DeleteCredential" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    uint64_t credentialId = parcel.ReadUint64() - 1;
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    CredentialInfo info;
    FillCredentialInfo(parcel, info);
    ret = g_service.DeleteCredential(userId, credentialId, authToken, info);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3200
 * @tc.name  : testGetCredential001
 * @tc.desc  : Pass the wrong authentication token to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential001, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3300
 * @tc.name  : testGetCredential002
 * @tc.desc  : Pass the authType authentication type to all types and determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential002, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    authType = ALL;
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3400
 * @tc.name  : testGetCredential003
 * @tc.desc  : Pass the authType authentication type to the PIN type to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential003, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    authType = PIN;
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3500
 * @tc.name  : testGetCredential004
 * @tc.desc  : Pass the authType authentication type to the PIN type to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential004, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    authType = FACE;
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3600
 * @tc.name  : testGetCredential005
 * @tc.desc  : Pass the authType authentication type to the PIN type to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential005, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    authType = FINGERPRINT;
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3700
 * @tc.name  : testGetUserInfo001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetUserInfo001, Function | MediumTest | Level2)
{
    cout << "start GetUserInfo" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    uint64_t secureUid = parcel.ReadUint64();
    PinSubType pinSubType = static_cast<PinSubType>(parcel.ReadUint32());
    std::vector<EnrolledInfo> infos;
    FillEnrolledInfoVector(parcel, infos);
    ret = g_service.GetUserInfo(userId, secureUid, pinSubType, infos);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3800
 * @tc.name  : testDeleteUser001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteUser001, Function | MediumTest | Level2)
{
    cout << "start DeleteUser" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    std::vector<CredentialInfo> deletedInfos;
    FillCredentialInfoVector(parcel, deletedInfos);
    ret = g_service.DeleteUser(userId, authToken, deletedInfos);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3900
 * @tc.name  : testDeleteUser002
 * @tc.desc  : The authentication token passed in the wrong user password determines the validity of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteUser002, Function | MediumTest | Level2)
{
    cout << "start DeleteUser" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> authToken(1);
    FillTestUint8Vector(parcel, authToken);
    authToken[0] = -1;
    std::vector<CredentialInfo> deletedInfos;
    FillCredentialInfoVector(parcel, deletedInfos);
    ret = g_service.DeleteUser(userId, authToken, deletedInfos);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4000
 * @tc.name  : testDeleteUser003
 * @tc.desc  : The authentication token passed in the wrong user password determines the validity of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteUser003, Function | MediumTest | Level2)
{
    cout << "start DeleteUser" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> authToken(1);
    FillTestUint8Vector(parcel, authToken);
    authToken[0] = 0;
    std::vector<CredentialInfo> deletedInfos;
    FillCredentialInfoVector(parcel, deletedInfos);
    ret = g_service.DeleteUser(userId, authToken, deletedInfos);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4100
 * @tc.name  : testEnforceDeleteUser001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testEnforceDeleteUser001, Function | MediumTest | Level2)
{
    cout << "start EnforceDeleteUser" << endl;
    int32_t ret = g_service.Init();
    int32_t userId = -1;
    std::vector<CredentialInfo> deletedInfos;
    FillCredentialInfoVector(parcel, deletedInfos);
    ret = g_service.EnforceDeleteUser(userId, deletedInfos);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}
