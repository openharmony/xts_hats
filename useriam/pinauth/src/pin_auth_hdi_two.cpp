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
using namespace OHOS::HDI::PinAuth::V2_0;
using HdiProperty = OHOS::HDI::PinAuth::V2_0::Property;

static AllInOneImpl g_executorImpl(make_shared<OHOS::UserIam::PinAuth::PinAuth>());
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

class DummyIExecutorCallback : public IExecutorCallback {
public:
    DummyIExecutorCallback(int32_t onResultResult, int32_t onGetDataResult, int32_t onGetDataV1Result,
        int32_t onTipResult, int32_t onMessageResult)
        : onResultResult_(onResultResult), onGetDataResult_(onGetDataResult), onTipResult_(onTipResult),
        onMessageResult_(onMessageResult)
    {
    }

    int32_t OnResult(int32_t result, const std::vector<uint8_t> &extraInfo) override
    {
        cout << "result is " << result << " extraInfo len is " << extraInfo.size() << endl;
        return onResultResult_;
    }

    int32_t OnGetData(const std::vector<uint8_t>& algoParameter, uint64_t authSubType, uint32_t algoVersion,
         const std::vector<uint8_t>& challenge) override
    {
        cout << "algoVersion is " << algoVersion << endl;
        cout << " algoParameter len is " << algoParameter.size() << endl;
        cout << " authSubType is " << authSubType << endl;
        return onGetDataResult_;
    }

    int32_t OnTip(int32_t tip, const std::vector<uint8_t>& extraInfo) override
    {
        return onTipResult_;
    }

    int32_t OnMessage(int32_t destRole, const std::vector<uint8_t>& msg) override
    {
        return onMessageResult_;
    }

private:
    int32_t onResultResult_;
    int32_t onGetDataResult_;
    int32_t onTipResult_;
    int32_t onMessageResult_;
};

static void FillTestIExecutorCallback(Parcel &parcel, sptr<IExecutorCallback> &callbackObj)
{
    bool isNull = parcel.ReadBool();
    if (isNull) {
        callbackObj = nullptr;
    } else {
        callbackObj = new (std::nothrow) DummyIExecutorCallback(parcel.ReadInt32(),
            parcel.ReadInt32(), parcel.ReadInt32(), parcel.ReadInt32(), parcel.ReadInt32());
        if (callbackObj == nullptr) {
            cout << "callbackObj construct fail" << endl;
        }
    }
}

/**
 * @tc.number: Security_IAM_PinAuth_HDI_NEW_FUNC_0104
 * @tc.name: Test Authenticate
 * @tc.size: MediumTest
 * @tc.type: Function
 * @tc.level: Level1
 */
HWTEST_F(UserIamPinAuthTwoTest, Security_IAM_PinAuth_HDI_NEW_FUNC_0104, Function | MediumTest | Level1)
{
    cout << "start Authenticate" << endl;
    
    uint64_t scheduleId = parcel.ReadUint64();
    uint64_t templateId = parcel.ReadUint64();
    std::vector<uint64_t> templateIdList;
    templateIdList.push_back(templateId);
    std::vector<uint8_t> extraInfo;
    FillTestUint8Vector(parcel, extraInfo);
    sptr<IExecutorCallback> callbackObj;
    FillTestIExecutorCallback(parcel, callbackObj);
    int32_t ret = g_executorImpl.Authenticate(scheduleId, templateIdList, extraInfo, callbackObj);

    cout << "ret is " << ret << endl;
    ASSERT_EQ(ret != Expectedvalue1, false);
}