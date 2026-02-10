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
#include "camera_flashlight_test_v1_2.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

void CameraFlashlightTestV1_2::SetUpTestCase(void) {}
void CameraFlashlightTestV1_2::TearDownTestCase(void) {}
void CameraFlashlightTestV1_2::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init(); // assert inside
}

void CameraFlashlightTestV1_2::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0010
 * @tc.desc:SetCallback_V1_2, Callback object = nullptr;
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0010, TestSize.Level1)
{
    int32_t ret;
    // step 1: get serviceV1_2
    cameraTest->serviceV1_2 = OHOS::HDI::Camera::V1_2::ICameraHost::Get("camera_service", false);
    EXPECT_NE(cameraTest->serviceV1_2, nullptr);
    CAMERA_LOGI("V1_2::ICameraHost get success");
    // step 2: set callback object which is nullptr
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_2::INVALID_ARGUMENT);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0020
 * @tc.desc:SetCallback_V1_2, Callback object exits;
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0020, TestSize.Level1)
{
    int32_t ret;
    // step 1: get serviceV1_2
    cameraTest->serviceV1_2 = OHOS::HDI::Camera::V1_2::ICameraHost::Get("camera_service", false);
    EXPECT_NE(cameraTest->serviceV1_2, nullptr);
    CAMERA_LOGI("V1_2::ICameraHost get success");
    // step 2: set callback object which is exits
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0030
 * @tc.desc:SetFlashlight_V1_2:statusV1_2 = 3.0f,INVALID_ARGUMENT
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0030, TestSize.Level1)
{
    int32_t ret;
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->statusV1_2 = 3.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0030
 * @tc.desc:SetFlashlight_V1_2:statusV1_2 = 0.5f,INVALID_ARGUMENT
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0040, TestSize.Level1)
{
    int32_t ret;
    cameraTest->Open(DEVICE_0);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_FLASHLIGHT_ADJUST_SUPPORTED, &entry);
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    res = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(res, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Close();
    sleep(UT_SLEEP_TIME);
    cameraTest->Init();
    if (ret == 0) {
        cameraTest->statusV1_2 = 0.5f;
        cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
        cameraTest->statusV1_2 = 0.0f;
        cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
        EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_OFF);
    } else {
        cameraTest->statusV1_2 = 0.5f;
        cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
        printf("Flashlight NOT SUPPORT");
    }
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0050
 * @tc.desc:statusCallback,turn on the flashlight,statusCallback
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0050, TestSize.Level1)
{
    int32_t ret;
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_OFF);
    cameraTest->statusV1_2 = 1.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_ON);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0060
 * @tc.desc:statusCallback,turn off the flashlight,statusCallback
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0060, TestSize.Level1)
{
    int32_t ret;
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_ON);
    cameraTest->statusV1_2 = 0.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_OFF);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0080
 * @tc.desc:statusCallback,turn on the flashlight,statusCallback
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0080, TestSize.Level1)
{
    int32_t ret;
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_OFF);
    cameraTest->statusV1_2 = 1.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    sleep(UT_SECOND_TIMES);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_ON);
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    sleep(UT_SECOND_TIMES);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_ON);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0070
 * @tc.desc:statusCallback,turn off the flashlight,statusCallback
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0070, TestSize.Level1)
{
    int32_t ret;
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_ON);
    cameraTest->statusV1_2 = 0.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    sleep(UT_SECOND_TIMES);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_OFF);
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    sleep(UT_SECOND_TIMES);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_OFF);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0090
 * @tc.desc:turn on the flashlight,opencamera,statusCallback
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0090, TestSize.Level1)
{
    int32_t ret;
    sleep(1);
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_OFF);
    cameraTest->statusV1_2 = 1.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    sleep(UT_SECOND_TIMES);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_ON);
    cameraTest->Open(DEVICE_0);
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    sleep(UT_SECOND_TIMES);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_UNAVAILABLE);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0100
 * @tc.desc:opencamera,turn on the flashlight,statusCallback
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0100, TestSize.Level1)
{
    int32_t ret;
    sleep(1);
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->statusV1_2 = 1.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_2::DEVICE_CONFLICT);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_UNAVAILABLE);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0110
 * @tc.desc:opencamera,turn off the flashlight,statusCallback
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0110, TestSize.Level1)
{
    int32_t ret;
    sleep(1);
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->statusV1_2 = 0.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_2::DEVICE_CONFLICT);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_UNAVAILABLE);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0120
 * @tc.desc:opencamera,turn on the flashlight,closecamera,turn on the flashlight,
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0120, TestSize.Level1)
{
    int32_t ret;
    sleep(1);
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->statusV1_2 = 1.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_2::DEVICE_CONFLICT);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_UNAVAILABLE);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
    cameraTest->Close();
    sleep(1);
    EXPECT_EQ(true, cameraTest->cameraDevice == nullptr);
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    sleep(UT_SECOND_TIMES);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_ON);
}

/**
 * @tc.name:SUB_Driver_Camera_Flashlight_0130
 * @tc.desc:opencamera,turn off the flashlight,closecamera,turn off the flashlight,
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraFlashlightTestV1_2, SUB_Driver_Camera_Flashlight_0130, TestSize.Level1)
{
    int32_t ret;
    sleep(1);
    // step 1: get serviceV1_2
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    ret = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(ret, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->statusV1_2 = 0.0f;
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_2::DEVICE_CONFLICT);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_UNAVAILABLE);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
    cameraTest->Close();
    sleep(1);
    EXPECT_EQ(true, cameraTest->cameraDevice == nullptr);
    cameraTest->rc = cameraTest->serviceV1_2->SetFlashlight_V1_2(cameraTest->statusV1_2);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    sleep(UT_SECOND_TIMES);
    EXPECT_EQ(OHOS::Camera::Test::statusCallback, HDI::Camera::V1_0::FLASHLIGHT_OFF);
}