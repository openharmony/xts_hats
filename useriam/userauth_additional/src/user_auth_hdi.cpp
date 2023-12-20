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

struct HdiBeginEnrollmentV1_1List {
    int32_t userId[4] = {12345, 1234, 12345, 12345};
    int32_t authType[4] = {1, 0, 2, 4};
    uint32_t executorSensorHint[4] = {0, 65535, 1, 0};
};
struct HdiBeginAuthenticationV1_1List {
    uint32_t authType[4] = {0, 1, 2, 4};
    uint32_t userId[4] = {356581, 1};
};
struct HdiBeginIdentificationV1_1List {
    uint32_t addExecutor[2] = {0, 1};
    uint32_t authType[4] = {0, 1, 2, 4};
};

void UserIamUserAuthTestAdditional::SetUpTestCase() {}

void UserIamUserAuthTestAdditional::TearDownTestCase() {}

void UserIamUserAuthTestAdditional::SetUp() { EXPECT_EQ(g_service.Init(), 0); }

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
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor001, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::ALL;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0300
 * @tc.name  : testAddExecutor002
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor002, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::PIN;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0400
 * @tc.name  : testAddExecutor003
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor003, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0500
 * @tc.name  : testAddExecutor004
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor004, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FINGERPRINT;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0600
 * @tc.name  : testAddExecutor005
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor005, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FINGERPRINT;
    info.publicKey.resize(32);
    info.executorRole = ExecutorRole::COLLECTOR;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0700
 * @tc.name  : testAddExecutor006
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor006, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FINGERPRINT;
    info.publicKey.resize(32);
    info.executorRole = ExecutorRole::VERIFIER;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0800
 * @tc.name  : testAddExecutor007
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor007, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FINGERPRINT;
    info.publicKey.resize(32);
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_0900
 * @tc.name  : testAddExecutor008
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor008, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FINGERPRINT;
    info.publicKey.resize(32);
    info.executorSensorHint = 65535;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1000
 * @tc.name  : testAddExecutor009
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor009, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FINGERPRINT;
    info.publicKey.resize(32);
    info.executorSensorHint = -1;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1100
 * @tc.name  : testAddExecutor010
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor010, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.publicKey.resize(32);
    info.executorMatcher = -1;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1200
 * @tc.name  : testAddExecutor011
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor011, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.publicKey.resize(32);
    info.esl = ESL0;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1300
 * @tc.name  : testAddExecutor012
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor012, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.publicKey.resize(32);
    info.esl = ESL1;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1400
 * @tc.name  : testAddExecutor013
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor013, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.publicKey.resize(32);
    info.esl = ESL2;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1500
 * @tc.name  : testAddExecutor014
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor014, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.publicKey.resize(32);
    info.esl = ESL3;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1600
 * @tc.name  : testAddExecutor015
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor015, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.publicKey.resize(32);
    info.executorMatcher = 0;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_1700
 * @tc.name  : testAddExecutor016
 * @tc.desc  : Modify the info actuator registration information to check whether the function can be executed correctly
 */
HWTEST_F(UserIamUserAuthTestAdditional, testAddExecutor016, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.publicKey.resize(32);
    info.executorMatcher = 65535;
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;

    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
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
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2100
 * @tc.name  : testOpenSession001
 * @tc.desc  : Call the OpenSession function with the parameter userId = -1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testOpenSession001, Function | MediumTest | Level1)
{
    cout << "start OpenSession" << endl;
    int32_t userId = -1;
    std::vector<uint8_t> challenge;
    FillTestUint8Vector(parcel, challenge);
    auto ret = g_service.OpenSession(userId, challenge);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2200
 * @tc.name  : testOpenSession002
 * @tc.desc  : Call the OpenSession function with the parameter userId = 0
 */
HWTEST_F(UserIamUserAuthTestAdditional, testOpenSession002, Function | MediumTest | Level1)
{
    cout << "start OpenSession" << endl;
    int32_t userId = 0;
    std::vector<uint8_t> challenge;
    FillTestUint8Vector(parcel, challenge);
    auto ret = g_service.OpenSession(userId, challenge);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2300
 * @tc.name  : testOpenSession003
 * @tc.desc  : Call the OpenSession function with the parameter userId = 1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testOpenSession003, Function | MediumTest | Level1)
{
    cout << "start OpenSession" << endl;
    int32_t userId = 1;
    std::vector<uint8_t> challenge;
    FillTestUint8Vector(parcel, challenge);
    auto ret = g_service.OpenSession(userId, challenge);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2400
 * @tc.name  : testCloseSession001
 * @tc.desc  : Call the CloseSession function with the parameter userId = -1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCloseSession001, Function | MediumTest | Level2)
{
    cout << "start CloseSession" << endl;
    int32_t userId = -1;
    auto ret = g_service.CloseSession(userId);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2500
 * @tc.name  : testCloseSession002
 * @tc.desc  : Close unopened authentication credential management sessions
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCloseSession002, Function | MediumTest | Level2)
{
    cout << "start CloseSession" << endl;
    int32_t userId = 1000;
    auto ret = g_service.CloseSession(userId);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
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
    int32_t userId = -1;
    std::vector<uint8_t> authToken;
    FillTestUint8Vector(parcel, authToken);
    EnrollParam param;
    FillEnrollParam(parcel, param);
    ScheduleInfo info;
    FillScheduleInfo(parcel, info);
    auto ret = g_service.BeginEnrollment(userId, authToken, param, info);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2800
 * @tc.name  : testBeginEnrollment002
 * @tc.desc  : Call the OpenSession function and AddExecutor function, then call the BeginEnrollment function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginEnrollment002, Function | MediumTest | Level1)
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
    param.authType = AuthType::PIN;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

    EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_2900
 * @tc.name  : testBeginEnrollment003
 * @tc.desc  : Call the OpenSession function and AddExecutor function, then call the BeginEnrollment function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginEnrollment003, Function | MediumTest | Level2)
{
    int32_t userId = 1234;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = AuthType::ALL;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    std::vector<uint8_t> authToken;
    EnrollParam param = {};
    param.authType = AuthType::ALL;
    ScheduleInfo scheduleInfo = {};
    EXPECT_NE(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3000
 * @tc.name  : testBeginEnrollment004
 * @tc.desc  : Call the OpenSession function and AddExecutor function, then call the BeginEnrollment function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginEnrollment004, Function | MediumTest | Level2)
{
    int32_t userId = 12345;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    std::vector<uint8_t> authToken;
    EnrollParam param = {};
    param.authType = AuthType::FACE;
    ScheduleInfo scheduleInfo = {};
    EXPECT_NE(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3100
 * @tc.name  : testBeginEnrollment005
 * @tc.desc  : Call the OpenSession function and AddExecutor function, then call the BeginEnrollment function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginEnrollment005, Function | MediumTest | Level2)
{
    int32_t userId = 12345;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FINGERPRINT;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    std::vector<uint8_t> authToken;
    EnrollParam param = {};
    param.authType = AuthType::FINGERPRINT;
    ScheduleInfo scheduleInfo = {};
    EXPECT_NE(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
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
    int32_t userId = -12345;
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
    param.authType = AuthType::PIN;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

    EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3400
 * @tc.name  : testCancelEnrollment003
 * @tc.desc  : The BeginEnrollment function is invoked to register the authentication credentials,
 *              and then the CancelEnrollment function is invoked to cancel the registration
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelEnrollment003, Function | MediumTest | Level1)
{
    int32_t userId = -12345;
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
    param.authType = AuthType::PIN;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

    userId = -4567;
    EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3500
 * @tc.name  : testCancelEnrollment004
 * @tc.desc  : The BeginEnrollment function is invoked to register the authentication credentials,
 *              and then the CancelEnrollment function is invoked to cancel the registration
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelEnrollment004, Function | MediumTest | Level1)
{
    int32_t userId = 0;
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
    param.authType = AuthType::PIN;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

    EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3600
 * @tc.name  : testCancelEnrollment005
 * @tc.desc  : The BeginEnrollment function is invoked to register the authentication credentials,
 *              and then the CancelEnrollment function is invoked to cancel the registration
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelEnrollment005, Function | MediumTest | Level1)
{
    int32_t userId = 1;
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
    param.authType = AuthType::PIN;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

    EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
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
    param.authType = AuthType::PIN;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

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
 * @tc.desc  : Call the GetCredential function, with the first input parameter being userId=-1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential001, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t userId = -1;
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    auto ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_3900
 * @tc.name  : testGetCredential002
 * @tc.desc  : Pass the authType authentication type to ALL types and determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential002, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    authType = AuthType::ALL;
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    auto ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4000
 * @tc.name  : testGetCredential003
 * @tc.desc  : Pass the authType authentication type to the PIN type to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential003, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    authType = AuthType::PIN;
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    auto ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4100
 * @tc.name  : testGetCredential004
 * @tc.desc  : Pass the authType authentication type to the FACE type to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential004, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    authType = AuthType::FACE;
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    auto ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4200
 * @tc.name  : testGetCredential005
 * @tc.desc  : Pass the authType authentication type to the FINGERPRINT type to determine the function result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential005, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t userId = parcel.ReadInt32();
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    authType = AuthType::FINGERPRINT;
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    auto ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4300
 * @tc.name  : testGetCredential006
 * @tc.desc  : Call the GetCredential function, with the first input parameter being userId=0
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential006, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t userId = 0;
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    auto ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4400
 * @tc.name  : testGetCredential007
 * @tc.desc  : Call the GetCredential function, with the first input parameter being userId=-1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetCredential007, Function | MediumTest | Level1)
{
    cout << "start GetCredential" << endl;
    int32_t userId = 1;
    AuthType authType = static_cast<AuthType>(parcel.ReadInt32());
    std::vector<CredentialInfo> infos;
    FillCredentialInfoVector(parcel, infos);
    auto ret = g_service.GetCredential(userId, authType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_EQ(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4500
 * @tc.name  : testGetUserInfo001
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetUserInfo001, Function | MediumTest | Level2)
{
    cout << "start GetUserInfo" << endl;
    int32_t userId = 6789;
    uint64_t secureUid = parcel.ReadUint64();
    PinSubType pinSubType = static_cast<PinSubType>(parcel.ReadUint32());
    std::vector<EnrolledInfo> infos;
    FillEnrolledInfoVector(parcel, infos);
    auto ret = g_service.GetUserInfo(userId, secureUid, pinSubType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4600
 * @tc.name  : testGetUserInfo002
 * @tc.desc  : Pass the wrong userId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetUserInfo002, Function | MediumTest | Level2)
{
    cout << "start GetUserInfo" << endl;
    int32_t userId = -6789;
    uint64_t secureUid = 0;
    PinSubType pinSubType = PinSubType::PIN_SIX;
    std::vector<EnrolledInfo> infos;
    auto ret = g_service.GetUserInfo(userId, secureUid, pinSubType, infos);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
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
    auto ret = g_service.DeleteUser(userId, authToken, deletedInfos);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4800
 * @tc.name  : testDeleteUser002
 * @tc.desc  : Call the DeleteUser function, with the second input parameter being authToken=-1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteUser002, Function | MediumTest | Level2)
{
    cout << "start DeleteUser" << endl;
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> authToken(1);
    FillTestUint8Vector(parcel, authToken);
    authToken[0] = -1;
    std::vector<CredentialInfo> deletedInfos;
    auto ret = g_service.DeleteUser(userId, authToken, deletedInfos);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_4900
 * @tc.name  : testDeleteUser003
 * @tc.desc  : Call the DeleteUser function, with the second input parameter being authToken=0
 */
HWTEST_F(UserIamUserAuthTestAdditional, testDeleteUser003, Function | MediumTest | Level2)
{
    cout << "start DeleteUser" << endl;
    int32_t userId = parcel.ReadInt32();
    std::vector<uint8_t> authToken(1);
    FillTestUint8Vector(parcel, authToken);
    authToken[0] = 0;
    std::vector<CredentialInfo> deletedInfos;
    FillCredentialInfoVector(parcel, deletedInfos);
    auto ret = g_service.DeleteUser(userId, authToken, deletedInfos);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5000
 * @tc.name  : testEnforceDeleteUser001
 * @tc.desc  : Directly call the EnforceDeleteUser function, with the first input parameter being userId=-1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testEnforceDeleteUser001, Function | MediumTest | Level2)
{
    cout << "start EnforceDeleteUser" << endl;
    int32_t userId = -1;
    std::vector<CredentialInfo> deletedInfos;
    auto ret = g_service.EnforceDeleteUser(userId, deletedInfos);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5100
 * @tc.name  : testEnforceDeleteUser002
 * @tc.desc  : Directly call the EnforceDeleteUser function, with the first input parameter being userId=65535
 */
HWTEST_F(UserIamUserAuthTestAdditional, testEnforceDeleteUser002, Function | MediumTest | Level2)
{
    cout << "start EnforceDeleteUser" << endl;
    int32_t userId = 65535;
    std::vector<CredentialInfo> deletedInfos;
    auto ret = g_service.EnforceDeleteUser(userId, deletedInfos);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5200
 * @tc.name  : testBeginAuthentication001
 * @tc.desc  : When the registration results are not updated and the registration is completed,
 *              The first entry is contextId = 1, the second entry is the AuthSolution structure ->userId = 365861,
 *              authTrustLevel = 10000,authType = PIN,executorSensorHint = 1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication001, Function | MediumTest | Level2)
{
    int32_t userId = 365861;
    AuthType authType = AuthType::PIN;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    ScheduleInfo scheduleInfo = {};
    std::vector<uint8_t> authToken;
    EnrollParam enrollParam = {};
    enrollParam.authType = authType;
    EXPECT_EQ(g_service.BeginEnrollment(userId, authToken, enrollParam, scheduleInfo), 0);

    uint64_t contextId = 1;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = 10000;
    authParam.authType = authType;
    authParam.executorSensorHint = 1;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5300
 * @tc.name  : testBeginAuthentication002
 * @tc.desc  : The first input parameter is contextId=0, the second input parameter is the
 *              AuthSolution structure->userId=365861, authTrustLevel=10000, authType=PIN, executorSensorHint=1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication002, Function | MediumTest | Level2)
{
    int32_t userId = 365861;
    AuthType authType = AuthType::PIN;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 0;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = 10000;
    authParam.authType = authType;
    authParam.executorSensorHint = 1;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5400
 * @tc.name  : testBeginAuthentication003
 * @tc.desc  : The first input parameter is contextId=-1, the second input parameter is the
 *              AuthSolution structure->userId=365861, authTrustLevel=10000, authType=PIN, executorSensorHint=1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication003, Function | MediumTest | Level2)
{
    int32_t userId = 365861;
    AuthType authType = AuthType::PIN;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = -1;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = 10000;
    authParam.authType = authType;
    authParam.executorSensorHint = 1;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5500
 * @tc.name  : testBeginAuthentication004
 * @tc.desc  : The first input parameter is contextId=1, the second input parameter is the
 *              AuthSolution structure->userId=-1, authTrustLevel=10000, authType=PIN, executorSensorHint=1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication004, Function | MediumTest | Level2)
{
    int32_t userId = -1;
    AuthType authType = AuthType::PIN;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 1;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = 10000;
    authParam.authType = authType;
    authParam.executorSensorHint = 1;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5600
 * @tc.name  : testBeginAuthentication005
 * @tc.desc  : The first input parameter is contextId=1, the second input parameter is the
 *              AuthSolution structure->userId=0, authTrustLevel=10000, authType=PIN, executorSensorHint=1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication005, Function | MediumTest | Level2)
{
    int32_t userId = 0;
    AuthType authType = AuthType::PIN;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 1;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = 10000;
    authParam.authType = authType;
    authParam.executorSensorHint = 1;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5700
 * @tc.name  : testBeginAuthentication006
 * @tc.desc  : The first input parameter is contextId=1, the second input parameter is the
 *              AuthSolution structure->userId=1, authTrustLevel=-1, authType=PIN, executorSensorHint=1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication006, Function | MediumTest | Level2)
{
    int32_t userId = 1;
    AuthType authType = AuthType::PIN;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 1;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = -1;
    authParam.authType = authType;
    authParam.executorSensorHint = 1;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5800
 * @tc.name  : testBeginAuthentication007
 * @tc.desc  : The first input parameter is contextId=1, the second input parameter is the
 *              AuthSolution structure->userId=1, authTrustLevel=0, authType=PIN, executorSensorHint=1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication007, Function | MediumTest | Level2)
{
    int32_t userId = 1;
    AuthType authType = AuthType::PIN;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 1;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = 0;
    authParam.authType = authType;
    authParam.executorSensorHint = 1;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_5900
 * @tc.name  : testBeginAuthentication008
 * @tc.desc  : The first input parameter is contextId=1, the second input parameter is the
 *              AuthSolution structure->userId=1, authTrustLevel=0, authType=FACE, executorSensorHint=1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication008, Function | MediumTest | Level2)
{
    int32_t userId = 1;
    AuthType authType = AuthType::FACE;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 1;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = 0;
    authParam.authType = authType;
    authParam.executorSensorHint = 1;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6000
 * @tc.name  : testBeginAuthentication009
 * @tc.desc  : The first input parameter is contextId=1, the second input parameter is the
 *              AuthSolution structure->userId=1, authTrustLevel=0, authType=FINGERPRINT, executorSensorHint=1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication009, Function | MediumTest | Level2)
{
    int32_t userId = 1;
    AuthType authType = AuthType::FINGERPRINT;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 1;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = 0;
    authParam.authType = authType;
    authParam.executorSensorHint = 1;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6100
 * @tc.name  : testBeginAuthentication010
 * @tc.desc  : The first input parameter is contextId=1, the second input parameter is the
 *              AuthSolution structure->userId=1, authTrustLevel=0, authType=ALL, executorSensorHint=0
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthentication010, Function | MediumTest | Level2)
{
    int32_t userId = 1;
    AuthType authType = AuthType::ALL;
    std::vector<uint8_t> challenge;
    EXPECT_EQ(g_service.OpenSession(userId, challenge), 0);

    ExecutorRegisterInfo info = {};
    info.authType = authType;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);

    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    uint64_t index = 0;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 1;
    AuthSolution authParam = {};
    authParam.userId = userId;
    authParam.authTrustLevel = 0;
    authParam.authType = authType;
    authParam.executorSensorHint = 0;
    authParam.challenge = challenge;
    std::vector<ScheduleInfo> scheduleInfos;
    auto ret = g_service.BeginAuthentication(contextId, authParam, scheduleInfos);
    EXPECT_NE(ret, 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6200
 * @tc.name  : testUpdateAuthenticationResult001
 * @tc.desc  : The first input parameter is contextId=-1, and the second input parameter is scheduleResult is empty
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateAuthenticationResult001, Function | MediumTest | Level2)
{
    uint64_t contextId = -1;
    std::vector<uint8_t> scheduleResult;
    AuthResultInfo authResultInfo = {};
    EXPECT_NE(g_service.UpdateAuthenticationResult(contextId, scheduleResult, authResultInfo), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6300
 * @tc.name  : testUpdateAuthenticationResult002
 * @tc.desc  : The first input parameter is contextId=0, and the second input parameter is scheduleResult is empty
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateAuthenticationResult002, Function | MediumTest | Level2)
{
    uint64_t contextId = 0;
    std::vector<uint8_t> scheduleResult;
    AuthResultInfo authResultInfo = {};
    EXPECT_NE(g_service.UpdateAuthenticationResult(contextId, scheduleResult, authResultInfo), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6400
 * @tc.name  : testUpdateAuthenticationResult003
 * @tc.desc  : The first input parameter is contextId=1234567,
 *              and the second input parameter is scheduleResult is empty
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateAuthenticationResult003, Function | MediumTest | Level2)
{
    uint64_t contextId = 1234567;
    std::vector<uint8_t> scheduleResult;
    AuthResultInfo authResultInfo = {};
    EXPECT_NE(g_service.UpdateAuthenticationResult(contextId, scheduleResult, authResultInfo), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6500
 * @tc.name  : testUpdateAuthenticationResult004
 * @tc.desc  : The first input is contextId = 1, the second input is scheduleResult The size is 100,
 *              and all inputs are initialized to 1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateAuthenticationResult004, Function | MediumTest | Level2)
{
    uint64_t contextId = 1;
    std::vector<uint8_t> scheduleResult(100, 1);
    AuthResultInfo authResultInfo = {};
    EXPECT_NE(g_service.UpdateAuthenticationResult(contextId, scheduleResult, authResultInfo), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6600
 * @tc.name  : testUpdateAuthenticationResult005
 * @tc.desc  : The first input is contextId = 1, the second input is scheduleResult The size is 100,
 *              and all inputs are initialized to 0
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateAuthenticationResult005, Function | MediumTest | Level2)
{
    uint64_t contextId = 1;
    std::vector<uint8_t> scheduleResult(100, 0);
    AuthResultInfo authResultInfo = {};
    EXPECT_NE(g_service.UpdateAuthenticationResult(contextId, scheduleResult, authResultInfo), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6700
 * @tc.name  : testCancelAuthentication001
 * @tc.desc  : Verify the CancelAuthentication function with the first input being contextId = 0
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelAuthentication001, Function | MediumTest | Level2)
{
    cout << "start CancelAuthentication" << endl;
    uint64_t contextId = 0;
    auto ret = g_service.CancelAuthentication(contextId);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6800
 * @tc.name  : testCancelAuthentication002
 * @tc.desc  : Verify the CancelAuthentication function with the first input being contextId = -1
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelAuthentication002, Function | MediumTest | Level2)
{
    cout << "start CancelAuthentication" << endl;
    uint64_t contextId = -1;
    auto ret = g_service.CancelAuthentication(contextId);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_6900
 * @tc.name  : testCancelAuthentication003
 * @tc.desc  : Verify the CancelAuthentication function with the first input being contextId = 1000
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelAuthentication003, Function | MediumTest | Level2)
{
    cout << "start CancelAuthentication" << endl;
    uint64_t contextId = 1000;
    auto ret = g_service.CancelAuthentication(contextId);
    cout << "ret is " << ret << endl;
    EXPECT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7000
 * @tc.name  : testBeginIdentification001
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification001, Function | MediumTest | Level1)
{
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

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7100
 * @tc.name  : testBeginIdentification002
 * @tc.desc  : Pass the wrong contextId to the function to determine the result of the function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification002, Function | MediumTest | Level2)
{
    uint64_t contextId = 623159;
    AuthType authType = AuthType::FACE;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_NE(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7200
 * @tc.name  : testBeginIdentification003
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification003, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 0;
    AuthType authType = AuthType::FACE;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7300
 * @tc.name  : testBeginIdentification004
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification004, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FACE;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = -1;
    AuthType authType = AuthType::FACE;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7400
 * @tc.name  : testBeginIdentification005
 * @tc.desc  : Verification When the AddExecutor function fails to add the authentication actuator,
 *              the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification005, Function | MediumTest | Level2)
{
    ExecutorRegisterInfo info = {};
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_NE(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 123456;
    AuthType authType = AuthType::FACE;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_NE(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7600
 * @tc.name  : testBeginIdentification007
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification007, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::FINGERPRINT;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 123456;
    AuthType authType = AuthType::FINGERPRINT;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7700
 * @tc.name  : testBeginIdentification008
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification008, Function | MediumTest | Level1)
{
    ExecutorRegisterInfo info = {};
    info.authType = AuthType::ALL;
    info.executorRole = ExecutorRole::ALL_IN_ONE;
    info.esl = ExecutorSecureLevel::ESL0;
    info.publicKey.resize(32);
    uint64_t index = 0;
    std::vector<uint8_t> publicKey;
    std::vector<uint64_t> templateIds;
    EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

    uint64_t contextId = 123456;
    AuthType authType = AuthType::ALL;
    std::vector<uint8_t> challenge;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7800
 * @tc.name  : testBeginIdentification009
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification009, Function | MediumTest | Level1)
{
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
    std::vector<uint8_t> challenge(1);
    challenge[0] = 0;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_7900
 * @tc.name  : testBeginIdentification010
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification010, Function | MediumTest | Level1)
{
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
    std::vector<uint8_t> challenge(1);
    challenge[0] = 1;
    uint32_t executorSensorHint = 0;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8000
 * @tc.name  : testBeginIdentification011
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification011, Function | MediumTest | Level2)
{
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
    uint32_t executorSensorHint = -1234;
    ScheduleInfo scheduleInfo = {};
    EXPECT_NE(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8100
 * @tc.name  : testBeginIdentification012
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentification function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentification012, Function | MediumTest | Level2)
{
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
    uint32_t executorSensorHint = 1234;
    ScheduleInfo scheduleInfo = {};
    EXPECT_NE(g_service.BeginIdentification(contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
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

    std::vector<uint8_t> scheduleResult;
    IdentifyResultInfo identityResultInfo = {};
    EXPECT_NE(g_service.UpdateIdentificationResult(contextId, scheduleResult, identityResultInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8400
 * @tc.name  : testUpdateIdentificationResult002
 * @tc.desc  : Call the BeginIdentification function to start recognition,
 *              and then call the UpdateIdenticationResult function to update the recognition result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateIdentificationResult002, Function | MediumTest | Level2)
{
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

    std::vector<uint8_t> scheduleResult;
    scheduleResult.resize(32);
    IdentifyResultInfo identityResultInfo = {};
    EXPECT_NE(g_service.UpdateIdentificationResult(contextId, scheduleResult, identityResultInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8500
 * @tc.name  : testUpdateIdentificationResult003
 * @tc.desc  : Call the BeginIdentification function to start recognition,
 *              and then call the UpdateIdenticationResult function to update the recognition result
 */
HWTEST_F(UserIamUserAuthTestAdditional, testUpdateIdentificationResult003, Function | MediumTest | Level2)
{
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

    std::vector<uint8_t> scheduleResult;
    IdentifyResultInfo identityResultInfo = {};
    contextId = 1;
    EXPECT_NE(g_service.UpdateIdentificationResult(contextId, scheduleResult, identityResultInfo), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8600
 * @tc.name  : testCancelIdentification001
 * @tc.desc  : BeginIdentification is called to start generating the identification,
 *              and then CancelIdentification function is called to cancel
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelIdentification001, Function | MediumTest | Level1)
{
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
    EXPECT_EQ(g_service.CancelIdentification(contextId), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_8700
 * @tc.name  : testCancelIdentification002
 * @tc.desc  : call BeginIdentification to start generating identification,
 *              and then call CancelIdentification function to pass in different parameters to cancel
 */
HWTEST_F(UserIamUserAuthTestAdditional, testCancelIdentification002, Function | MediumTest | Level2)
{
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
    contextId = 123;
    EXPECT_NE(g_service.CancelIdentification(contextId), 0);

    EXPECT_EQ(g_service.DeleteExecutor(index), 0);
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
 * @tc.name  : testGetAuthTrustLevel001
 * @tc.desc  : Directly call the GetAuthTrustLevel function, with the first input parameter
 *              being userId=1000 and the second input parameter being authType=ALL
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetAuthTrustLevel001, Function | MediumTest | Level2)
{
    int32_t userId = 1000;
    AuthType authType = AuthType::ALL;
    uint32_t authTrustLevel = 0;
    EXPECT_NE(g_service.GetAuthTrustLevel(userId, authType, authTrustLevel), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9100
 * @tc.name  : testGetAuthTrustLevel002
 * @tc.desc  : Directly call the GetAuthTrustLevel function, with the first input parameter
 *              being userId=1000 and the second input parameter being authType=PIN
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetAuthTrustLevel002, Function | MediumTest | Level2)
{
    int32_t userId = 1000;
    AuthType authType = AuthType::PIN;
    uint32_t authTrustLevel = 0;
    EXPECT_NE(g_service.GetAuthTrustLevel(userId, authType, authTrustLevel), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9200
 * @tc.name  : testGetAuthTrustLevel003
 * @tc.desc  : Directly call the GetAuthTrustLevel function, with the first input parameter
 *              being userId=1000 and the second input parameter being authType=FACE
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetAuthTrustLevel003, Function | MediumTest | Level2)
{
    int32_t userId = 1000;
    AuthType authType = AuthType::FACE;
    uint32_t authTrustLevel = 0;
    EXPECT_NE(g_service.GetAuthTrustLevel(userId, authType, authTrustLevel), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9300
 * @tc.name  : testGetAuthTrustLevel004
 * @tc.desc  : Directly call the GetAuthTrustLevel function, with the first input parameter
 *              being userId=1000 and the second input parameter being authType=FINGERPRINT
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetAuthTrustLevel004, Function | MediumTest | Level2)
{
    int32_t userId = 1000;
    AuthType authType = AuthType::FINGERPRINT;
    uint32_t authTrustLevel = 0;
    EXPECT_NE(g_service.GetAuthTrustLevel(userId, authType, authTrustLevel), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9400
 * @tc.name  : testGetAuthTrustLevel005
 * @tc.desc  : Call the BeginEnrollment function without calling the UpdateEnrollmentResult function to update and
 *              enroll the result, and then call the GetAuthTrustLevel function,
 *              with the first entry being userId = 12345. The second entry is authType = PIN
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetAuthTrustLevel005, Function | MediumTest | Level2)
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
    param.authType = AuthType::PIN;
    ScheduleInfo scheduleInfo = {};
    EXPECT_EQ(g_service.BeginEnrollment(userId, authToken, param, scheduleInfo), 0);

    AuthType authType = AuthType::PIN;
    uint32_t authTrustLevel = 0;
    EXPECT_NE(g_service.GetAuthTrustLevel(userId, authType, authTrustLevel), 0);
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
    HdiBeginEnrollmentV1_1List g_hdiBeginEnrollmentV1_1List;
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
    ScheduleInfoV1_1 scheduleInfo = {};

    for (i = 0; i < 4; i++) {
        info.authType = static_cast<AuthType>(g_hdiBeginEnrollmentV1_1List.authType[i]);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

        param.authType = static_cast<AuthType>(g_hdiBeginEnrollmentV1_1List.authType[i]);
        param.executorSensorHint = g_hdiBeginEnrollmentV1_1List.executorSensorHint[i];
        userId = g_hdiBeginEnrollmentV1_1List.userId[i];
        if (i == 0) {
            EXPECT_EQ(g_service.BeginEnrollmentV1_1(userId, authToken, param, scheduleInfo), 0);
        } else {
            EXPECT_NE(g_service.BeginEnrollmentV1_1(userId, authToken, param, scheduleInfo), 0);
        }

        EXPECT_EQ(g_service.CancelEnrollment(userId), 0);
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    }
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9600
 * @tc.name  : testBeginAuthenticationV1_1_001
 * @tc.desc  : Call the HDI-encapsulated BeginAuthenticationV1_1 function, and use a loop to assign the parameters
 *              in the HdiBeginAuthenticationV1_1List structure to the BeginAuthenticationV1_1 function
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginAuthenticationV1_1_001, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    uint32_t j;
    HdiBeginAuthenticationV1_1List g_hdiBeginAuthenticationV1_1List;
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
    AuthSolution authParam = {};
    authParam.authTrustLevel = 0;
    authParam.executorSensorHint = 0;
    authParam.challenge = challenge;
    std::vector<ScheduleInfoV1_1> scheduleInfos;

    for (i = 0; i < 4; i++) {
        info.authType = static_cast<AuthType>(g_hdiBeginAuthenticationV1_1List.authType[i]);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
        for (j = 4; j > 0; j--) {
            authParam.userId = g_hdiBeginAuthenticationV1_1List.userId[0];
            authParam.authType = static_cast<AuthType>(g_hdiBeginAuthenticationV1_1List.authType[j - 1]);
            EXPECT_NE(g_service.BeginAuthenticationV1_1(contextId, authParam, scheduleInfos), 0);
        }
        EXPECT_EQ(g_service.DeleteExecutor(index), 0);
    }

    for (i = 1; i < 2; i++) {
        info.authType = static_cast<AuthType>(g_hdiBeginAuthenticationV1_1List.authType[i]);
        EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);

        authParam.userId = g_hdiBeginAuthenticationV1_1List.userId[i];
        authParam.authType = static_cast<AuthType>(g_hdiBeginAuthenticationV1_1List.authType[i]);
        EXPECT_NE(g_service.BeginAuthenticationV1_1(contextId, authParam, scheduleInfos), 0);
    }
    EXPECT_EQ(g_service.CloseSession(userId), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9700
 * @tc.name  : testBeginIdentificationV1_1_001
 * @tc.desc  : The AddExecutor function is called first to add the authentication actuator,
 *              and then the BeginIdentificationV1_1 function is called to start the identification
 */
HWTEST_F(UserIamUserAuthTestAdditional, testBeginIdentificationV1_1_001, Function | MediumTest | Level2)
{
    uint32_t i = 0;
    uint32_t j = 0;
    HdiBeginIdentificationV1_1List g_hdiBeginIdentificationV1_1List;

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
    ScheduleInfoV1_1 scheduleInfo = {};

    for (; i < 2; i++) {
        for (; j < 4; j++) {
            authType = static_cast<AuthType>(g_hdiBeginIdentificationV1_1List.authType[j]);
            if (i == 1 && authType == AuthType::FACE) {
                EXPECT_EQ(g_service.AddExecutor(info, index, publicKey, templateIds), 0);
                EXPECT_EQ(g_service.BeginIdentificationV1_1
                    (contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);
            } else {
                EXPECT_NE(g_service.BeginIdentificationV1_1
                    (contextId, authType, challenge, executorSensorHint, scheduleInfo), 0);
            }
        }
    }
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9800
 * @tc.name  : testGetAllUserInfo001
 * @tc.desc  : Call the GetAllUserInfo function to get the information
 */
HWTEST_F(UserIamUserAuthTestAdditional, testGetAllUserInfo001, Function | MediumTest | Level1)
{
    std::vector<UserInfo> userInfos;
    EXPECT_EQ(g_service.GetAllUserInfo(userInfos), 0);
}
/**
 * @tc.number: SUB_Security_IAM_UserAuth_HDI_FUNC_9900
 * @tc.name  : testInit001
 * @tc.desc  : Call the Init function for initialization
 */
HWTEST_F(UserIamUserAuthTestAdditional, testInit001, Function | MediumTest | Level1)
{
    cout << "start Init" << endl;
    EXPECT_EQ(g_service.Init(), 0);
}