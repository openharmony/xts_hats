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

#include "pin_auth_hdi_test.h"
#include "iam_hat_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::UserIam::Common;
using namespace OHOS::HDI::PinAuth;
using namespace OHOS::HDI::PinAuth::V1_0;
using namespace OHOS::HDI::PinAuth::V1_1;
using Property = OHOS::HDI::PinAuth::V1_1::Property;

static ExecutorImpl g_executorImpl(make_shared<OHOS::UserIam::PinAuth::PinAuth>());
static OHOS::Parcel parcel;
int32_t Expectedvalue1 = 0;

void UserIamPinAuthTwoTest::SetUpTestCase()
{
}

void UserIamPinAuthTwoTest::TearDownTestCase()
{
}

void UserIamPinAuthTwoTest::SetUp()
{
}

void UserIamPinAuthTwoTest::TearDown()
{
}

class DummyIExecutorCallback : public OHOS::HDI::PinAuth::IExecutorCallback {
public:
    DummyIExecutorCallback(int32_t onResultResult, int32_t onGetDataResult, int32_t onGetDataV1Result)
        : onResultResult_(onResultResult), onGetDataResult_(onGetDataResult), onGetDataV1Result_(onGetDataV1Result)
    {
    }

    int32_t OnResult(int32_t result, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "result is " << result << " extraInfo len is " << extraInfo.size() << endl;
        return onResultResult_;
    }

    int32_t OnGetData(uint64_t scheduleId, const std::vector<uint8_t> &algoParameter, uint64_t authSubType) override
    {
        cout << "scheduleId is " << scheduleId << endl;
        cout << " algoParameter len is " << algoParameter.size() << endl;
        cout << " authSubType is " << authSubType << endl;
        return onGetDataResult_;
    }

    int32_t OnGetDataV1_1(uint64_t scheduleId, const std::vector<uint8_t> &algoParameter, uint64_t authSubType,
        uint32_t algoVersion) override
    {
        cout << "scheduleId is " << scheduleId << endl;
        cout << " authSubType is " << authSubType << endl;
        return onGetDataV1Result_;
    }

private:
    int32_t onResultResult_;
    int32_t onGetDataResult_;
    int32_t onGetDataV1Result_;
};

static void FillTestIExecutorCallback(Parcel &parcel, sptr<OHOS::HDI::PinAuth::IExecutorCallback> &callbackObj)
{
    bool isNull = parcel.ReadBool();
    if (isNull) {
        callbackObj = nullptr;
    } else {
        callbackObj = new (std::nothrow) DummyIExecutorCallback(parcel.ReadInt32(),
            parcel.ReadInt32(), parcel.ReadInt32());
        if (callbackObj == nullptr) {
            cout << "callbackObj construct fail" << endl;
        }
    }
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_NEW_FUNC_0104
 * @tc.name: Test AuthenticateV1_1
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTwoTest, Security_IAM_PinAuth_HDI_NEW_FUNC_0104, Function | MediumTest | Level1)
{
    cout << "start AuthenticateV1_1" << endl;
    
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<OHOS::HDI::PinAuth::IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.AuthenticateV1_1(scheduleId, templateId, extraInfo, callbackObj);

    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue1, true);
}