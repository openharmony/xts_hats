/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file expected in compliance with the License.
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
#include "hdi_device_uttest.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

void HdiDeviceUtTestAdditional::SetUpTestCase(void) {}
void HdiDeviceUtTestAdditional::TearDownTestCase(void) {}
void HdiDeviceUtTestAdditional::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
}

void HdiDeviceUtTestAdditional::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.number: SUB_Driver_Camera_GetStreamOperator_0100
 * @tc.name: testGetStreamOperator001
 * @tc.desc: GetStreamOperator, stability test
*/
HWTEST_F(HdiDeviceUtTestAdditional, testGetStreamOperator001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
            cameraTest->streamOperator);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number: SUB_Driver_Camera_SetResultMode_0100
 * @tc.name: testSetResultMode001
 * @tc.desc: SetResultMode stability test
*/
HWTEST_F(HdiDeviceUtTestAdditional, testSetResultMode001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
    std::vector<MetaType> enableTypes;
    cameraTest->rc = cameraTest->cameraDevice->GetEnabledResults(enableTypes);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    for (auto &type : enableTypes) {
        CAMERA_LOGE("check hdi_device: type = %{public}d", type);
    }
    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->cameraDevice->SetResultMode(PER_FRAME);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number: SUB_Driver_Camera_GetEnabledResults_0100
 * @tc.name: testGetEnabledResults001
 * @tc.desc: GetEnabledResults return NO_ERROR
*/
HWTEST_F(HdiDeviceUtTestAdditional, testGetEnabledResults001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
    std::vector<MetaType> enableTypes;
    cameraTest->rc = cameraTest->cameraDevice->GetEnabledResults(enableTypes);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number: SUB_Driver_Camera_GetEnabledResults_0200
 * @tc.name: testGetEnabledResults002
 * @tc.desc: GetEnabledResults stability test
*/
HWTEST_F(HdiDeviceUtTestAdditional, testGetEnabledResults002, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
    std::vector<MetaType> enableTypes;
    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->cameraDevice->GetEnabledResults(enableTypes);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number: SUB_Driver_Camera_EnableResult_0100
 * @tc.name: testEnableResult001
 * @tc.desc: EnableResult, stability test
*/
HWTEST_F(HdiDeviceUtTestAdditional, testEnableResult001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    std::vector<int32_t> resultsList;
    resultsList.push_back(OHOS_CAMERA_STREAM_ID);
    resultsList.push_back(OHOS_CONTROL_AE_EXPOSURE_COMPENSATION);
    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->cameraDevice->EnableResult(resultsList);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number: SUB_Driver_Camera_DisableResult_0100
 * @tc.name: testDisableResult001
 * @tc.desc: DisableResult, stability test
*/
HWTEST_F(HdiDeviceUtTestAdditional, testDisableResult001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    std::vector<OHOS::Camera::MetaType> resultsOriginal;
    for (int i = 0; i < 100; i++) {
        cameraTest->cameraDevice->DisableResult(resultsOriginal);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

