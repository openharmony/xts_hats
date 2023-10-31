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
#include "hdi_host_uttest.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

void HdiHostUtTestAdditional::SetUpTestCase(void) {}
void HdiHostUtTestAdditional::TearDownTestCase(void) {}
void HdiHostUtTestAdditional::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
}

void HdiHostUtTestAdditional::TearDown(void) { cameraTest->Close(); }

/**
 * @tc.number: SUB_Driver_Camera_GetCameraIds_0100
 * @tc.name: testGetCameraIds001
 * @tc.desc: GetCameraIds stability test
 */
HWTEST_F(HdiHostUtTestAdditional, testGetCameraIds001, Function | MediumTest | Level1)
{
    EXPECT_EQ(true, cameraTest->cameraDevice == nullptr);
    if (cameraTest->cameraDevice == nullptr) {
        for (int i = 0; i < 100; i++) {
            cameraTest->rc = cameraTest->service->GetCameraIds(cameraTest->cameraIds);
            EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
            EXPECT_LT(0, cameraTest->cameraIds.size());
            CAMERA_LOGE("check hdi_host: cameraIds.size() = %{public}zu", cameraTest->cameraIds.size());
        }
    }
}

/**
 * @tc.number: SUB_Driver_Camera_SetFlashLight_0100
 * @tc.name: testSetFlashLight001
 * @tc.desc: SetFlashlight, cameraId is not found, status is false
 */
HWTEST_F(HdiHostUtTestAdditional, testSetFlashLight001, Function | MediumTest | Level2)
{
    EXPECT_EQ(true, cameraTest->cameraDevice == nullptr);
    if (cameraTest->cameraDevice == nullptr) {
        std::string testCameraId = "abc";
        cameraTest->status = false;
        cameraTest->rc = cameraTest->service->SetFlashlight(testCameraId, cameraTest->status);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number: SUB_Driver_Camera_SetFlashLight_0200
 * @tc.name: testSetFlashLight002
 * @tc.desc: SetFlashlight, cameraId is not found, status is false
 */
HWTEST_F(HdiHostUtTestAdditional, testSetFlashLight002, Function | MediumTest | Level2)
{
    EXPECT_EQ(true, cameraTest->cameraDevice == nullptr);
    if (cameraTest->cameraDevice == nullptr) {
        std::string testCameraId = "";
        cameraTest->status = false;
        cameraTest->rc = cameraTest->service->SetFlashlight(testCameraId, cameraTest->status);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number: SUB_Driver_Camera_SetFlashLight_0300
 * @tc.name: testSetFlashLight003
 * @tc.desc: SetFlashlight stability test
 */
HWTEST_F(HdiHostUtTestAdditional, testSetFlashLight003, Function | MediumTest | Level1)
{
    EXPECT_EQ(true, cameraTest->cameraDevice == nullptr);
    if (cameraTest->cameraDevice == nullptr) {
        for (int i = 0; i < 100; i++) {
            cameraTest->service->GetCameraIds(cameraTest->cameraIds);
            cameraTest->status = true;
            cameraTest->rc = cameraTest->service->SetFlashlight(cameraTest->cameraIds.front(), cameraTest->status);
            EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);
        }
    }
}