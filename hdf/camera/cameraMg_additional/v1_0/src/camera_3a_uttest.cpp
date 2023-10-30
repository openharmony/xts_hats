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
#include "camera_3a_uttest.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

void Camera3AUtTestAdditional::SetUpTestCase(void) {}
void Camera3AUtTestAdditional::TearDownTestCase(void) {}
void Camera3AUtTestAdditional::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
    cameraTest->Open();
}

void Camera3AUtTestAdditional::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_0100
 * @tc.name: testUpdateSettings001
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_EXPOSURE_COMPENSATION
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings001, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    int32_t expo = 0xa0;
    meta->addEntry(OHOS_CONTROL_AE_EXPOSURE_COMPENSATION, &expo, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_0200
 * @tc.name: testUpdateSettings002
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FPS_RANGES
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings002, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    const int32_t FPS_VALUE = 10;
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    std::vector<int32_t> fpsRange;
    fpsRange.push_back(FPS_VALUE);
    fpsRange.push_back(FPS_VALUE);
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, fpsRange.data(), fpsRange.size());
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_0300
 * @tc.name: testUpdateSettings003
 * @tc.desc: UpdateSettings, OHOS_CAMERA_STREAM_ID
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings003, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    const int32_t deviceStreamId = 0;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &deviceStreamId, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_0400
 * @tc.name: testUpdateSettings004
 * @tc.desc: UpdateSettings, OHOS_SENSOR_COLOR_CORRECTION_GAINS
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings004, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    int64_t colorGains[4] = {0};
    meta->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_0500
 * @tc.name: testUpdateSettings005
 * @tc.desc: UpdateSettings, OHOS_SENSOR_COLOR_CORRECTION_GAINS
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings005, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    int64_t expoTime = 0;
    meta->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_0600
 * @tc.name: testUpdateSettings006
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_MODE
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings006, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeMode = OHOS_CAMERA_AE_MODE_ON;
    meta->addEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_0700
 * @tc.name: testUpdateSettings007
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AWB_MODE
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings007, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_AUTO;
    meta->addEntry(OHOS_CONTROL_AWB_MODE, &awbMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_0800
 * @tc.name: testUpdateSettings008
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_MODE
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings008, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeMode = OHOS_CAMERA_AE_MODE_OFF;
    meta->addEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_0900
 * @tc.name: testUpdateSettings009
 * @tc.desc: UpdateSettings, stability test
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings009, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    int32_t expo = 0xa0;
    meta->addEntry(OHOS_CONTROL_AE_EXPOSURE_COMPENSATION, &expo, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1000
 * @tc.name: testUpdateSettings010
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_MODE
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings010, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeMode = OHOS_CAMERA_AE_MODE_ON_AUTO_FLASH;
    meta->addEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1100
 * @tc.name: testUpdateSettings011
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_MODE
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings011, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeMode = OHOS_CAMERA_AE_MODE_ON_ALWAYS_FLASH;
    meta->addEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1200
 * @tc.name: testUpdateSettings012
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_MODE
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings012, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeMode = OHOS_CAMERA_AE_MODE_ON_AUTO_FLASH_REDEYE;
    meta->addEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1300
 * @tc.name: testUpdateSettings013
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_MODE
*/
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings013, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeMode = OHOS_CAMERA_AE_MODE_ON_EXTERNAL_FLASH;
    meta->addEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}