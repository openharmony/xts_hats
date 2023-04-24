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

void HdiDeviceUtTest::SetUpTestCase(void) {}
void HdiDeviceUtTest::TearDownTestCase(void) {}
void HdiDeviceUtTest::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
}

void HdiDeviceUtTest::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.name: GetStreamOperator
 * @tc.desc: GetStreamOperator, normal callback input
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiDeviceUtTest, SUB_DriverSystem_CameraHdiMg_0290, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: GetStreamOperator
 * @tc.desc: GetStreamOperator, callback is nullptr
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiDeviceUtTest, SUB_DriverSystem_CameraHdiMg_0300, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = nullptr;
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.name: SetResultMode
 * @tc.desc: SetResultMode is PER_FRAME
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiDeviceUtTest, SUB_DriverSystem_CameraHdiMg_0310, TestSize.Level0)
{
    cameraTest->Open();
    EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
    std::vector<MetaType> enableTypes;
    cameraTest->rc = cameraTest->cameraDevice->GetEnabledResults(enableTypes);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    for (auto &type : enableTypes) {
        CAMERA_LOGE("check hdi_device: type = %{public}d", type);
    }
    cameraTest->rc = cameraTest->cameraDevice->SetResultMode(PER_FRAME);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: SetResultMode
 * @tc.desc: SetResultMode is ON_CHANGE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiDeviceUtTest, SUB_DriverSystem_CameraHdiMg_0320, TestSize.Level0)
{
    cameraTest->Open();
    EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
    std::vector<MetaType> enableTypes;
    cameraTest->rc = cameraTest->cameraDevice->GetEnabledResults(enableTypes);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    for (auto &type : enableTypes) {
        CAMERA_LOGE("check hdi_device: type = %{public}d", type);
    }
    cameraTest->rc = cameraTest->cameraDevice->SetResultMode(ON_CHANGED);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}