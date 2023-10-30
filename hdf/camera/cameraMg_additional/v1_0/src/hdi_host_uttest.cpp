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

void HdiHostUtTestAdditional::TearDown(void)
{
    cameraTest->Close();
}

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