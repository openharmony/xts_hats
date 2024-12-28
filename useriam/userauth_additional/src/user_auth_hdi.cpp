/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
using namespace OHOS::HDI::UserAuth::V3_0;

static const uint32_t MAX_FUZZ_STRUCT_LEN = 20;
static UserAuthInterfaceService g_service;
int32_t Expectedvalue = 0;
static OHOS::Parcel parcel;

struct HdiBeginEnrollmentList {
    int32_t userId[4] = {12345, 1234, 12345, 12345};
    int32_t authType[4] = {1, 0, 2, 4};
    uint32_t executorSensorHint[4] = {0, 65535, 1, 0};
};
struct HdiBeginAuthenticationList {
    uint32_t authType[4] = {0, 1, 2, 4};
    uint32_t userId[4] = {356581, 1};
};
struct HdiBeginIdentificationList {
    uint32_t addExecutor[2] = {0, 1};
    uint32_t authType[4] = {0, 1, 2, 4};
};

void UserIamUserAuthTestAdditional::SetUpTestCase() {}

void UserIamUserAuthTestAdditional::TearDownTestCase() {}

void UserIamUserAuthTestAdditional::SetUp()
{
    const std::string deviceUdid = std::string(64, '0');
    EXPECT_EQ(g_service.Init(deviceUdid), 0);
}

void UserIamUserAuthTestAdditional::TearDown() {}

static void FillEnrollParam(Parcel &parcel, EnrollParam &enrollParam)
{
    enrollParam.authType = static_cast<AuthType>(parcel.ReadInt32());
    enrollParam.executorSensorHint = parcel.ReadUint32();
    enrollParam.userId = parcel.ReadInt32();
}

static void FillExecutorIndexInfo(Parcel &parcel, uint64_t &executorIndex)
{
    executorIndex = parcel.ReadUint64();
}

static void FillExecutorIndexVector(Parcel &parcel, vector<uint64_t > &vector)
{
    uint32_t len = parcel.ReadInt32() % MAX_FUZZ_STRUCT_LEN;
    vector.resize(len);
    for (uint32_t i = 0; i < len; i++) {
        FillExecutorIndexInfo(parcel, vector[i]);
    }
}

static void FillScheduleInfo(Parcel &parcel, ScheduleInfo &scheduleInfo)
{
    scheduleInfo.scheduleId = parcel.ReadUint64();
    FillTestUint64Vector(parcel, scheduleInfo.templateIds);
    scheduleInfo.authType = static_cast<AuthType>(parcel.ReadInt32());
    scheduleInfo.executorMatcher = parcel.ReadUint32();
    scheduleInfo.scheduleMode = static_cast<ScheduleMode>(parcel.ReadInt32());
    FillExecutorIndexVector(parcel, scheduleInfo.executorIndexes);
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
 * @tc.desc  : First input parameter being the ExecutorRegisterInfo structure ->authType value being
 * ALL/PIN/FACE/FINGERPRINT
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor001, Function | MediumTest | Level1)
{
    uint32_t i = 0;
    uint32_t authType[4] = {0, 1, 2, 4};
    ExecutorRegisterInfo info = {};
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    for (i = 0; i < 4; i++) {
        info.authType = static_cast<AuthType>(authType[i]);
        info.publicKey.resize(32);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1800
 * @tc.name  : testDeleteExecutor001
 * @tc.desc  : Delete call without adding an authentication executor
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteExecutor001, Function | MediumTest | Level2)
{
    cout << "start DeleteExecutor" << endl;
    uint64_t index = -1;
    auto ret = g_service.DeleteExecutor(index);
    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue, true);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1900
 * @tc.name  : testDeleteExecutor002
 * @tc.desc  : First call the AddExecutor function to add an authentication executor,
 *              and then call the DeleteExecutor function to delete it
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteExecutor002, Function | MediumTest | Level1)
{
    cout << "start DeleteExecutor" << endl;
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::ALL;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    auto ret = g_service.DeleteExecutor(index);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2000
 * @tc.name  : testDeleteExecutor003
 * @tc.desc  : Call the AddExecutor function to add an authentication executor,
 *              and then call the DeleteExecutor function to pass in different parameters for deletion
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteExecutor003, Function | MediumTest | Level2)
{
    cout << "start DeleteExecutor" << endl;
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::ALL;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_NE(index, 0);
    index = 0;
    auto ret = g_service.DeleteExecutor(index);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2200
 * @tc.name  : testOpenSession002
 * @tc.desc  : Call the OpenSession function with the parameter userId = -1/0/1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testOpenSession002, Function | MediumTest | Level1)
{
    cout << "start OpenSession" << endl;
    uint32_t i = 0;
    int32_t userId[3] = {-1, 0, 1};
    std::vector<uint8_t> challenge;
    for (i = 0; i < 3; i++) {
        FillTestUint8Vector(parcel, challenge);
        EXPECT_EQ(g_service.OpenSession(userId[i], challenge), 0);
        EXPECT_EQ(g_service.CloseSession(userId[i]), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2500
 * @tc.name  : testCloseSession002
 * @tc.desc  : Close unopened authentication credential management sessions
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCloseSession002, Function | MediumTest | Level2)
{
    cout << "start CloseSession" << endl;
    uint32_t i = 0;
    uint32_t ret = 0;
    int32_t userId[2] = {-1, 1000};
    for (i = 0; i < 2; i++) {
        EXPECT_NE(g_service.CloseSession(userId[i]), 0);
    }
    cout << "ret is " << ret << endl;
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2600
 * @tc.name  : testCloseSession003
 * @tc.desc  : Close unopened authentication credential management sessions
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCloseSession003, Function | MediumTest | Level2)
{
    cout << "start CloseSession" << endl;
    int32_t userId = 1000;
    int32_t i = 0;
    while (i < 50) {
        EXPECT_NE(g_service.CloseSession(userId), 0);
        i++;
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2700
 * @tc.name  : testBeginEnrollment001
 * @tc.desc  : Directly call the BeginEnrollment function without preprocessing
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginEnrollment001, Function | MediumTest | Level2)
{
    cout << "start BeginEnrollment" << endl;
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    EnrollParam param;
    FillEnrollParam(parcel, param);
    ScheduleInfo info;
    FillScheduleInfo(parcel, info);
    auto ret = g_service.BeginEnrollment(authToken, param, info);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2800
 * @tc.name  : testBeginEnrollment002
 * @tc.desc  : Call the OpenSession function and AddExecutor function, then call the BeginEnrollment function
 * and test different authTypes
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginEnrollment002, Function | MediumTest | Level1)
{
    uint32_t i = 0;
    uint64_t index = 0;
    std::vector<uint8_t> challenge;
    std::vector<uint8_t> authToken;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    ScheduleInfo scheduleInfo = {};
    ExecutorRegisterInfo info = {};
    EnrollParam param = {};
    param.userId = 12345;
    uint32_t authType[4] = {0, 1, 2, 4};

    for (i = 0; i < 4; i++) {
        EXPECT_EQ(g_service.OpenSession(param.userId, challenge), 0);
        info.executorRole = ExecutorRole::ALL_IN_ONE;
        info.esl = ExecutorSecureLevel::ESL0;
        info.publicKey.resize(32);
        info.authType = static_cast<AuthType>(authType[i]);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
        param.authType = static_cast<AuthType>(authType[i]);

        if (i == 1) {
            EXPECT_EQ(g_service.BeginEnrollment(authToken, param, scheduleInfo), 0);

            EXPECT_EQ(g_service.CancelEnrollment(param.userId), 0);
        } else {
            EXPECT_NE(g_service.BeginEnrollment(authToken, param, scheduleInfo), 0);
        }
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
        EXPECT_EQ(g_service.CloseSession(param.userId), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3200
 * @tc.name  : testCancelEnrollment001
 * @tc.desc  : Call the CancelEnrollment function with userId=-123 as the input parameter
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelEnrollment001, Function | MediumTest | Level1)
{
    cout << "start CancelEnrollment" << endl;
    int32_t userId = -123;
    auto ret = g_service.CancelEnrollment(userId);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3300
 * @tc.name  : testCancelEnrollment002
 * @tc.desc  : The BeginEnrollment function is invoked to register the authentication credentials,
 *              and then the CancelEnrollment function is invoked to cancel the registration
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelEnrollment002, Function | MediumTest | Level1)
{
    uint32_t i = 0;
    int32_t userId[3] = {-12345, 0, 1};
    std::vector<uint8_t> challenge;
    ExecutorRegisterInfo info = {};
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    std::vector<uint8_t> authToken;
    EnrollParam param = {};
    ScheduleInfo scheduleInfo = {};

    for (i = 0; i < 3; i++) {
        EXPECT_EQ(g_service.OpenSession(userId[i], challenge), 0);

        info.authType = AuthType::PIN;
        info.executorRole = ExecutorRole::ALL_IN_ONE;
        info.esl = ExecutorSecureLevel::ESL0;
        info.publicKey.resize(32);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

        param.authType = AuthType::PIN;
        param.userId = userId[i];
        EXPECT_EQ(g_service.BeginEnrollment(authToken, param, scheduleInfo), 0);

        EXPECT_EQ(g_service.CancelEnrollment(userId[i]), 0);
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
        EXPECT_EQ(g_service.CloseSession(userId[i]), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3700
 * @tc.name  : testCancelEnrollment006
 * @tc.desc  : Call the BeginEnrollment function to register the authentication credentials,
 *              then call the CancelEnrollment function to cancel, and then continue to cancel 50 times
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelEnrollment006, Function | MediumTest | Level1)
{
    int32_t userId = 1;
    int32_t i = 0;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = AuthType::PIN;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    std::vector<uint8_t> authToken;
    EnrollParam param = {};
    param.userId = userId;
    param.authType = AuthType::PIN;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginEnrollment(authToken, param, scheduleInfo), 0);

    EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
    while (i < 50) {
        EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
        i++;
    }
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3800
 * @tc.name  : testGetCredential001
 * @tc.desc  : Call the GetCredential function, with the first input parameter being userId=-1、0、1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential001, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    uint32_t i = 0;
    uint32_t userId[3] = {-1, 0, 1};
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    std::vector<CredentialInfo> infos;

    for (i = 0; i < 3; i++) {
        FillCredentialInfoVector(parcel, infos);
        EXPECT_EQ(g_service.GetCredential(userId[i], authType, infos), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3900
 * @tc.name  : testGetCredential002
 * @tc.desc  : Pass the authType authentication type to all types and determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential002, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    uint32_t i = 0;
    int32_t userId = parcel.ReadInt32();
    uint32_t authType[4] = {0, 1, 2, 4};
    std::vector<CredentialInfo> infos;

    for (i = 0; i < 4; i++) {
        FillCredentialInfoVector(parcel, infos);
        EXPECT_EQ(g_service.GetCredential(userId, static_cast<AuthType>(authType[i]), infos), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4500
 * @tc.name  : testGetUserInfo001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetUserInfo001, Function | MediumTest | Level2)
{
    cout << "start GetUserInfo" << endl;
    uint32_t i = 0;
    int userId[2] = {6789, -6789};
    uint64_t secureUid = parcel.ReadUint64();
    int32_t pinSubType = parcel.ReadInt32();
    std::vector<EnrolledInfo> infos;

    for (i = 0; i < 2; i++) {
        FillEnrolledInfoVector(parcel, infos);
        EXPECT_NE(g_service.GetUserInfo(userId[i], secureUid, pinSubType, infos), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4700
 * @tc.name  : testDeleteUser001
 * @tc.desc  : Call the DeleteUser function, with the first input parameter
 *              being userId=-1 and the second parameter being empty
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteUser001, Function | MediumTest | Level2)
{
    cout << "start DeleteUser" << endl;

    int32_t userId = -1;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = AuthType::PIN;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    std::vector<uint8_t> authToken;
    std::vector<CredentialInfo> deletedInfos;
    std::vector<uint8_t> rootSecret;
    auto ret = g_service.DeleteUser(userId, authToken, deletedInfos, rootSecret);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4800
 * @tc.name  : testDeleteUser002
 * @tc.desc  : Call the DeleteUser function, with the second input parameter being authToken=-1、0
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteUser002, Function | MediumTest | Level2)
{
    cout << "start DeleteUser" << endl;
    uint32_t i = 0;
    uint32_t num[2] = {-1, 0};
    std::vector<CredentialInfo> deletedInfos;
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> authToken(1);
    FillTestUint8Vector(parcel, authToken);
    std::vector<uint8_t> rootSecret(1);
    FillTestUint8Vector(parcel, rootSecret);
    for (i = 0; i < 2; i++) {
        authToken[0] = num[i];
        EXPECT_NE(g_service.DeleteUser(userId, authToken, deletedInfos, rootSecret), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5000
 * @tc.name  : testEnforceDeleteUser001
 * @tc.desc  : Directly call the EnforceDeleteUser function, with the first input parameter being userId=-1,65535
 */
HWTEST_F(UserIamUserAuthTestAdditional, testEnforceDeleteUser001, Function | MediumTest | Level2)
{
    cout << "start EnforceDeleteUser" << endl;
    uint32_t i = 0;
    int32_t userId[2] = {-1, 65535};
    std::vector<CredentialInfo> deletedInfos;

    for (i = 0; i < 2; i++) {
        EXPECT_NE(g_service.EnforceDeleteUser(userId[i], deletedInfos), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5200
 * @tc.name  : testBeginAuthentication001
 * @tc.desc  : When the registration results are not updated and the registration is completed,
 *              The first entry is contextId = 1/0/-1, the second entry is the AuthSolution structure ->userId = 365861,
 *              authTrustLevel = 10000,authType = PIN,executorSensorHint = 1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication001, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    uint64_t contextId[3] = {1, 0, -1};
    AuthType authType = AuthType::PIN;
    std::vector<uint8_t> challenge;
    ExecutorRegisterInfo info = {};
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    ScheduleInfo scheduleInfo = {};
    std::vector<uint8_t> authToken;
    EnrollParam enrollParam = {};
    enrollParam.userId = 365861;
    AuthParam authParam = {};
    std::vector<ScheduleInfo> scheduleInfos;

    for (i = 0; i < 3; i++) {
        EXPECT_EQ(g_service.OpenSession(enrollParam.userId, challenge), 0);

        info.authType = authType;
        info.executorRole = ExecutorRole::ALL_IN_ONE;
        info.esl = ExecutorSecureLevel::ESL0;
        info.publicKey.resize(32);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

        enrollParam.authType = authType;
        EXPECT_EQ(g_service.BeginEnrollment(authToken, enrollParam, scheduleInfo), 0);

        authParam.baseParam.userId = enrollParam.userId;
        authParam.baseParam.authTrustLevel = 10000;
        authParam.authType = authType;
        authParam.baseParam.executorSensorHint = 1;
        authParam.baseParam.challenge = challenge;
        EXPECT_NE(g_service.BeginAuthentication(contextId[i], authParam, scheduleInfos), 0);

        EXPECT_EQ(g_service.CancelEnrollment(enrollParam.userId), 0);
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
        EXPECT_EQ(g_service.CloseSession(enrollParam.userId), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5500
 * @tc.name  : testBeginAuthentication004
 * @tc.desc  : The first input parameter is contextId=1, the second input parameter is the
 *              AuthSolution structure->userId=0, authTrustLevel=-1、0、1000, authType=PIN, executorSensorHint=1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication004, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    int32_t userId = 0;
    uint64_t index = 0;
    uint64_t contextId = 1;
    uint32_t authTrustLevel[3] = {-1, 0, 1000};
    AuthParam authParam = {};
    ExecutorRegisterInfo info = {};
    AuthType authType = AuthType::PIN;
    std::vector<uint8_t> challenge;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    std::vector<ScheduleInfo> scheduleInfos;

    for (i = 0; i < 3; i++) {
        EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

        info.authType = authType;
        info.executorRole = ExecutorRole::ALL_IN_ONE;
        info.esl = ExecutorSecureLevel::ESL0;
        info.publicKey.resize(32);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

        authParam.baseParam.userId = userId;
        authParam.baseParam.authTrustLevel = authTrustLevel[i];
        authParam.authType = authType;
        authParam.baseParam.executorSensorHint = 1;
        authParam.baseParam.challenge = challenge;
        EXPECT_NE(g_service.BeginAuthentication(contextId, authParam, scheduleInfos), 0);

        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
        EXPECT_EQ(g_service.CloseSession(userId), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6200
 * @tc.name  : testUpdateAuthenticationResult001
 * @tc.desc  : The first input parameter is contextId=-1、0、1234567, and the second input parameter
 *              is scheduleResult is empty
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateAuthenticationResult001, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    uint64_t contextId[3] = {-1, 0, 1234567};
    std::vector<uint8_t> scheduleResult;
    AuthResultInfo authResultInfo = {};
    EnrolledState enrollState = {};

    for (i = 0; i < 3; i++) {
        EXPECT_NE(g_service.UpdateAuthenticationResult(contextId[i], scheduleResult, authResultInfo, enrollState), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6500
 * @tc.name  : testUpdateAuthenticationResult004
 * @tc.desc  : The first input is contextId = 1, the second input is scheduleResult The size is 100,
 *              and all inputs are initialized to 1 and 0
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateAuthenticationResult004, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    uint64_t contextId = 1;
    uint64_t num[2] = {1, 0};
    std::vector<uint8_t> scheduleResult(100);
    AuthResultInfo authResultInfo = {};
    EnrolledState enrollState = {};

    for (i = 0; i < 2; i++) {
        scheduleResult.insert(scheduleResult.begin(), 100, num[i]);
        EXPECT_NE(g_service.UpdateAuthenticationResult(contextId, scheduleResult, authResultInfo, enrollState), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6700
 * @tc.name  : testCancelAuthentication001
 * @tc.desc  : Verify the CancelAuthentication function with the first input being contextId = 0、-1、1000
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelAuthentication001, Function | MediumTest | Level2)
{
    cout << "start CancelAuthentication" << endl;
    uint32_t i = 0;
    uint64_t contextId[3] = {0, -1, 1000};

    for (i = 0; i < 3; i++) {
        EXPECT_NE(g_service.CancelAuthentication(contextId[i]), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7000
 * @tc.name  : testBeginIdentification001
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification001, Function | MediumTest | Level1)
{
    uint32_t i = 0;
    uint64_t index = 0;
    uint64_t contextId = 123456;
    uint32_t authType[3] = {0, 2, 4};
    ExecutorRegisterInfo info = {};
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    const std::string deviceUdid = std::string(64, '0');

    for (i = 0; i < 3; i++) {
        info.authType = static_cast<AuthType>(authType[i]);
        info.executorRole = ExecutorRole::ALL_IN_ONE;
        info.esl = ExecutorSecureLevel::ESL0;
        info.publicKey.resize(32);

        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
        EXPECT_EQ(g_service.BeginIdentification(contextId, static_cast<AuthType>(authType[i]), challenge,
                                                executorSensorHint, scheduleInfo),
                  0);
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
        EXPECT_EQ(g_service.Init(deviceUdid), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8000
 * @tc.name  : testBeginIdentification011
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification011, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    ExecutorRegisterInfo info = {};
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t contextId = 123456;
    AuthType authType = AuthType::FACE;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint[2] = {-1234, 1234};
    ScheduleInfo scheduleInfo = {};

    for (i = 0; i < 2; i++) {
        info.authType = AuthType::FACE;
        info.executorRole = ExecutorRole::ALL_IN_ONE;
        info.esl = ExecutorSecureLevel::ESL0;
        info.publicKey.resize(32);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
        EXPECT_NE(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint[i], scheduleInfo),
                  0);
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8200
 * @tc.name  : testBeginIdentification013
 * @tc.desc  : The AddExecutor function is called to add the authentication actuator,
 *              and then the BeginIdentification function is called 50 times to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification013, Function | MediumTest | Level2)
{
    int i = 0;
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 123456;
    AuthType authType = AuthType::FACE;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);
    while (i < 50) {
        EXPECT_NE(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);
        cout << "i = " << i << endl;
        i++;
    }
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8300
 * @tc.name  : testUpdateIdentificationResult001
 * @tc.desc  : Call the BeginIdentification function to start recognition,
 *              and then call the UpdateIdenticationResult function to update the recognition result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateIdentificationResult001, Function | MediumTest | Level2)
{
    int size[2] = {0, 32};
    uint32_t i = 0;
    ExecutorRegisterInfo info = {};
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t contextId = 123456;
    std::vector<uint8_t> scheduleResult;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    AuthType authType = AuthType::FACE;
    IdentifyResultInfo identityResultInfo = {};
    const std::string deviceUdid = std::string(64, '0');

    for (i = 0; i < 2; i++) {
        info.authType = AuthType::FACE;
        info.executorRole = ExecutorRole::ALL_IN_ONE;
        info.esl = ExecutorSecureLevel::ESL0;
        info.publicKey.resize(32);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
        EXPECT_EQ(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);

        scheduleResult.resize(size[i]);
        EXPECT_NE(g_service.UpdateIdentificationResult(contextId, scheduleResult, identityResultInfo), 0);
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
        EXPECT_EQ(g_service.Init(deviceUdid), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8800
 * @tc.name  : testCancelIdentification003
 * @tc.desc  : Cancel by calling the CancelIdentification function directly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelIdentification003, Function | MediumTest | Level2)
{
    uint64_t contextId = 1000;
    EXPECT_NE(g_service.CancelIdentification(contextId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8900
 * @tc.name  : testCancelIdentification004
 * @tc.desc  : Cancel by calling the CancelIdentification function directly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelIdentification004, Function | MediumTest | Level2)
{
    uint64_t contextId = 123456;
    AuthType authType = AuthType::FACE;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_NE(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);
    EXPECT_NE(g_service.CancelIdentification(contextId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9000
 * @tc.name  : testGetAvailableStatus001
 * @tc.desc  : Directly call the GetAvailableStatus  function, with the first input parameter
 *              being userId=1000 and the second input parameter being all authType
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetAvailableStatus001, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    int32_t userId = 1000;
    uint32_t authType[] = {0, 1, 2, 4};
    uint32_t authTrustLevel = 0;
    int32_t checkResult = 0;
    for (i = 0; i < 4; i++) {
        g_service.GetAvailableStatus(userId, static_cast<AuthType>(authType[i]), authTrustLevel,
            checkResult);
        EXPECT_NE(checkResult, 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9400
 * @tc.name  : testGetAvailableStatus005
 * @tc.desc  : Call the BeginEnrollment function without calling the UpdateEnrollmentResult function to update and
 *              enroll the result, and then call the GetAvailableStatus  function,
 *              with the first entry being userId = 12345. The second entry is authType = PIN
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetAvailableStatus005, Function | MediumTest | Level2)
{
    int32_t userId = 12345;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = AuthType::PIN;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    std::vector<uint8_t> authToken;
    EnrollParam param = {};
    param.userId = userId;
    param.authType = AuthType::PIN;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginEnrollment(authToken, param, scheduleInfo), 0);

    AuthType authType = AuthType::PIN;
    uint32_t authTrustLevel = 0;
    int32_t checkResult = 0;
    g_service.GetAvailableStatus(userId, authType, authTrustLevel, checkResult);
    EXPECT_NE(checkResult, 0);
    EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9500
 * @tc.name  : testBeginEnrollmentV1_1_001
 * @tc.desc  : Call the HDI-encapsulated BeginEnrollmentV1_1 function, and use a loop to assign the parameters
 *              in the HdiBeginEnrollmentV1_1List struct to the BeginEnrollmentV1_1 function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginEnrollmentV1_1_001, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    HdiBeginEnrollmentList g_hdiBeginEnrollmentV1_1List;
    uint32_t userId = g_hdiBeginEnrollmentV1_1List.userId[i];
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    std::vector<uint8_t> authToken;
    EnrollParam param = {};
    ScheduleInfo scheduleInfo = {};

    for (i = 0; i < 4; i++) {
        info.authType = static_cast<AuthType>(g_hdiBeginEnrollmentV1_1List.authType[i]);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

        param.authType = static_cast<AuthType>(g_hdiBeginEnrollmentV1_1List.authType[i]);
        param.executorSensorHint = g_hdiBeginEnrollmentV1_1List.executorSensorHint[i];
        param.userId = g_hdiBeginEnrollmentV1_1List.userId[i];
        if (i == 0) {
            EXPECT_EQ(g_service.BeginEnrollment(authToken, param, scheduleInfo), 0);
        } else {
            EXPECT_NE(g_service.BeginEnrollment(authToken, param, scheduleInfo), 0);
        }

        EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    }
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0250
 * @tc.name  : testBeginAuthenticationV1_1_001
 * @tc.desc  : Call the HDI-encapsulated BeginAuthenticationV1_1 function, and use a loop to assign the parameters
 *              in the HdiBeginAuthenticationV1_1List structure to the BeginAuthenticationV1_1 function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthenticationV1_1_001, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    uint32_t j;
    HdiBeginAuthenticationList g_hdiBeginAuthenticationV1_1List;
    uint32_t userId = g_hdiBeginAuthenticationV1_1List.userId[i];
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;

    uint64_t contextId = 1;
    AuthParam authParam = {};
    authParam.baseParam.authTrustLevel = 0;
    authParam.baseParam.executorSensorHint = 0;
    authParam.baseParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;

    for (i = 0; i < 4; i++) {
        info.authType = static_cast<AuthType>(g_hdiBeginAuthenticationV1_1List.authType[i]);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
        for (j = 4; j > 0; j--) {
            authParam.baseParam.userId = g_hdiBeginAuthenticationV1_1List.userId[0];
            authParam.authType = static_cast<AuthType>(g_hdiBeginAuthenticationV1_1List.authType[j - 1]);
            EXPECT_NE(g_service.BeginAuthentication(contextId, authParam, scheduleInfos), 0);
        }
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    }

    for (i = 1; i < 2; i++) {
        info.authType = static_cast<AuthType>(g_hdiBeginAuthenticationV1_1List.authType[i]);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

        authParam.baseParam.userId = g_hdiBeginAuthenticationV1_1List.userId[i];
        authParam.authType = static_cast<AuthType>(g_hdiBeginAuthenticationV1_1List.authType[i]);
        EXPECT_NE(g_service.BeginAuthentication(contextId, authParam, scheduleInfos), 0);
    }
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0550
 * @tc.name  : testBeginIdentificationV1_1_001
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentificationV1_1 function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentificationV1_1_001, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    uint32_t j = 0;
    HdiBeginIdentificationList g_hdiBeginIdentificationV1_1List;

    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    uint64_t contextId = 123456;
    AuthType authType = AuthType::FACE;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            authType = static_cast<AuthType>(g_hdiBeginIdentificationV1_1List.authType[j]);
            if (i == 1 && authType == AuthType::FACE) {
                EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
                EXPECT_EQ(
                    g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo),
                    0);
            } else {
                EXPECT_NE(
                    g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo),
                    0);
            }
        }
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0890
 * @tc.name  : testGetAllUserInfo001
 * @tc.desc  : Call the GetAllUserInfo function to get the information
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetAllUserInfo001, Function | MediumTest | Level1)
{
    std::vector<UserInfo> userInfos;
    EXPECT_EQ(g_service.GetAllUserInfo(userInfos), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0910
 * @tc.name  : testInit001
 * @tc.desc  : Call the Init function for initialization
 */
HWTEST_F(UserIamUserAuthTestAdditional, testInit001, Function | MediumTest | Level1)
{
    cout << "start Init" << endl;
    const std::string deviceUdid = std::string(64, '0');
    EXPECT_EQ(g_service.Init(deviceUdid), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0920
 * @tc.name  : testBeginIdentification014
 * @tc.desc  : Verify that the function BeginIdentification returns a failure when the first entry structure of the
 *              function AddExecutor is ExecutorRegisterInfo->executorRole = COLLECTOR/VERIFIER/ALL_IN_ONE
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification014, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint32_t executorRole[3] = {1, 2, 3};
    ExecutorRegisterInfo info = {};
    uint64_t contextId = 123456;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};

    for (i = 0; i < 3; i++) {
        info.authType = AuthType::FACE;
        info.executorRole = static_cast<ExecutorRole>(executorRole[i]);
        info.esl = ExecutorSecureLevel::ESL0;
        info.publicKey.resize(32);
        if (executorRole[i] == ExecutorRole::ALL_IN_ONE) {
            EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
            EXPECT_EQ(
                g_service.BeginIdentification(contextId, AuthType::FACE, challenge, executorSensorHint, scheduleInfo),
                0);
            EXPECT_EQ(g_service.DeleteExecutor(index), 0);
        } else {
            EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
            EXPECT_NE(
                g_service.BeginIdentification(contextId, AuthType::FACE, challenge, executorSensorHint, scheduleInfo),
                0);
            EXPECT_EQ(g_service.DeleteExecutor(index), 0);
        }
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0940
 * @tc.name  : testBeginIdentification016
 * @tc.desc  : Verify that the function BeginIdentification returns a success when the first entry structure of the
 *              function AddExecutor is ExecutorRegisterInfo->esl = ESL0\ESL1\ESL2\ESL3
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification016, Function | MediumTest | Level1)
{
    uint32_t i = 0;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint32_t esl[4] = {0, 1, 2, 3};
    ExecutorRegisterInfo info = {};
    uint64_t contextId = 123456;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    const std::string deviceUdid = std::string(64, '0');

    for (i = 0; i < 4; i++) {
        info.authType = AuthType::FACE;
        info.executorRole = ExecutorRole::ALL_IN_ONE;
        info.esl = static_cast<ExecutorSecureLevel>(esl[i]);
        info.publicKey.resize(32);

        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
        EXPECT_EQ(g_service.BeginIdentification(contextId, AuthType::FACE, challenge, executorSensorHint, scheduleInfo),
                  0);
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
        EXPECT_EQ(g_service.Init(deviceUdid), 0);
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1110
 * @tc.name  : testGetValidSolution001
 * @tc.desc  : Call the GetValidSolution function to get the the authentication information
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetValidSolution001, Function | MediumTest | Level2)
{
    int32_t userId = parcel.ReadInt32();
    std::vector<int32_t> authTypes = {AuthType::ALL, AuthType::PIN, AuthType::FACE, AuthType::FINGERPRINT};
    uint32_t authTrustLevel = 0;
    std::vector<int32_t> validTypes;
    EXPECT_NE(g_service.GetValidSolution(userId, authTypes, authTrustLevel, validTypes), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1120
 * @tc.name  : testDeleteCredential001
 * @tc.desc  : Call the DeleteCredential function to deletes credential information
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteCredential001, Function | MediumTest | Level2)
{
    int32_t userId = parcel.ReadInt32();
    uint64_t credentialId = parcel.ReadUint64();
    std::vector<uint8_t> authToken;
    CredentialInfo info;
    EXPECT_NE(g_service.DeleteCredential(userId, credentialId, authToken, info), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1130
 * @tc.name  : testUpdateEnrollmentResult001
 * @tc.desc  : Call the UpdateEnrollmentResult function directly to update and enroll the result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateEnrollmentResult001, Function | MediumTest | Level2)
{
    int32_t userId = 12345;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    std::vector<uint8_t> scheduleResult(1);
    scheduleResult[0] = 1;
    EnrollResultInfo enrolledResultInfo = {};
    EXPECT_NE(g_service.UpdateEnrollmentResult(userId, scheduleResult, enrolledResultInfo), 0);

    EXPECT_EQ(g_service.CloseSession(userId), 0);
}