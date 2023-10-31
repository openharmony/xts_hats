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

void Camera3AUtTestAdditional::TearDown(void) { cameraTest->Close(); }

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
 * @tc.desc: UpdateSettings, OHOS_SENSOR_EXPOSURE_TIME
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

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1400
 * @tc.name: testUpdateSettings014
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings014, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afState = OHOS_CAMERA_AF_STATE_INACTIVE;
    meta->addEntry(OHOS_CONTROL_AF_STATE, &afState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1500
 * @tc.name: testUpdateSettings015
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings015, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afState = OHOS_CAMERA_AF_STATE_PASSIVE_SCAN;
    meta->addEntry(OHOS_CONTROL_AF_STATE, &afState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1600
 * @tc.name: testUpdateSettings016
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings016, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afState = OHOS_CAMERA_AF_STATE_PASSIVE_FOCUSED;
    meta->addEntry(OHOS_CONTROL_AF_STATE, &afState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1700
 * @tc.name: testUpdateSettings017
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings017, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afState = OHOS_CAMERA_AF_STATE_ACTIVE_SCAN;
    meta->addEntry(OHOS_CONTROL_AF_STATE, &afState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1800
 * @tc.name: testUpdateSettings018
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings018, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afState = OHOS_CAMERA_AF_STATE_FOCUSED_LOCKED;
    meta->addEntry(OHOS_CONTROL_AF_STATE, &afState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_1900
 * @tc.name: testUpdateSettings019
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings019, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afState = OHOS_CAMERA_AF_STATE_NOT_FOCUSED_LOCKED;
    meta->addEntry(OHOS_CONTROL_AF_STATE, &afState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2000
 * @tc.name: testUpdateSettings020
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings020, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afState = OHOS_CAMERA_AF_STATE_PASSIVE_UNFOCUSED;
    meta->addEntry(OHOS_CONTROL_AF_STATE, &afState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2100
 * @tc.name: testUpdateSettings021
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings021, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afMode = OHOS_CAMERA_AF_MODE_OFF;
    meta->addEntry(OHOS_CONTROL_AF_MODE, &afMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2200
 * @tc.name: testUpdateSettings022
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings022, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afMode = OHOS_CAMERA_AF_MODE_AUTO;
    meta->addEntry(OHOS_CONTROL_AF_MODE, &afMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2300
 * @tc.name: testUpdateSettings023
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings023, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afMode = OHOS_CAMERA_AF_MODE_MACRO;
    meta->addEntry(OHOS_CONTROL_AF_MODE, &afMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2400
 * @tc.name: testUpdateSettings024
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings024, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afMode = OHOS_CAMERA_AF_MODE_CONTINUOUS_VIDEO;
    meta->addEntry(OHOS_CONTROL_AF_MODE, &afMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2500
 * @tc.name: testUpdateSettings025
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings025, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afMode = OHOS_CAMERA_AF_MODE_CONTINUOUS_PICTURE;
    meta->addEntry(OHOS_CONTROL_AF_MODE, &afMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2600
 * @tc.name: testUpdateSettings026
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings026, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afMode = OHOS_CAMERA_AF_MODE_EDOF;
    meta->addEntry(OHOS_CONTROL_AF_MODE, &afMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2700
 * @tc.name: testUpdateSettings027
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_ANTIBANDING_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings027, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeAntibandingMode = OHOS_CAMERA_AE_ANTIBANDING_MODE_OFF;
    meta->addEntry(OHOS_CONTROL_AE_ANTIBANDING_MODE, &aeAntibandingMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2800
 * @tc.name: testUpdateSettings028
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_ANTIBANDING_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings028, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeAntibandingMode = OHOS_CAMERA_AE_ANTIBANDING_MODE_50HZ;
    meta->addEntry(OHOS_CONTROL_AE_ANTIBANDING_MODE, &aeAntibandingMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_2900
 * @tc.name: testUpdateSettings029
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_ANTIBANDING_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings029, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeAntibandingMode = OHOS_CAMERA_AE_ANTIBANDING_MODE_60HZ;
    meta->addEntry(OHOS_CONTROL_AE_ANTIBANDING_MODE, &aeAntibandingMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3000
 * @tc.name: testUpdateSettings030
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_ANTIBANDING_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings030, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeAntibandingMode = OHOS_CAMERA_AE_ANTIBANDING_MODE_AUTO;
    meta->addEntry(OHOS_CONTROL_AE_ANTIBANDING_MODE, &aeAntibandingMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3100
 * @tc.name: testUpdateSettings031
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_TRIGGER
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings031, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afTrigger = OHOS_CAMERA_AF_TRIGGER_IDLE;
    meta->addEntry(OHOS_CONTROL_AF_TRIGGER, &afTrigger, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3200
 * @tc.name: testUpdateSettings032
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_TRIGGER
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings032, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afTrigger = OHOS_CAMERA_AF_TRIGGER_START;
    meta->addEntry(OHOS_CONTROL_AF_TRIGGER, &afTrigger, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3300
 * @tc.name: testUpdateSettings033
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AF_TRIGGER
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings033, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t afTrigger = OHOS_CAMERA_AF_TRIGGER_CANCEL;
    meta->addEntry(OHOS_CONTROL_AF_TRIGGER, &afTrigger, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3400
 * @tc.name: testUpdateSettings034
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_POSITION
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings034, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraPosition = OHOS_CAMERA_POSITION_FRONT;
    meta->addEntry(OHOS_ABILITY_CAMERA_POSITION, &abilityCameraPosition, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3500
 * @tc.name: testUpdateSettings035
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_POSITION
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings035, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraPosition = OHOS_CAMERA_POSITION_BACK;
    meta->addEntry(OHOS_ABILITY_CAMERA_POSITION, &abilityCameraPosition, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3600
 * @tc.name: testUpdateSettings036
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_POSITION
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings036, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraPosition = OHOS_CAMERA_POSITION_OTHER;
    meta->addEntry(OHOS_ABILITY_CAMERA_POSITION, &abilityCameraPosition, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3700
 * @tc.name: testUpdateSettings037
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_TYPE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings037, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraType = OHOS_CAMERA_TYPE_WIDE_ANGLE;
    meta->addEntry(OHOS_ABILITY_CAMERA_TYPE, &abilityCameraType, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3800
 * @tc.name: testUpdateSettings038
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_TYPE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings038, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraType = OHOS_CAMERA_TYPE_ULTRA_WIDE;
    meta->addEntry(OHOS_ABILITY_CAMERA_TYPE, &abilityCameraType, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_3900
 * @tc.name: testUpdateSettings039
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_TYPE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings039, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraType = OHOS_CAMERA_TYPE_TELTPHOTO;
    meta->addEntry(OHOS_ABILITY_CAMERA_TYPE, &abilityCameraType, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4000
 * @tc.name: testUpdateSettings040
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_TYPE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings040, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraType = OHOS_CAMERA_TYPE_TRUE_DEAPTH;
    meta->addEntry(OHOS_ABILITY_CAMERA_TYPE, &abilityCameraType, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4100
 * @tc.name: testUpdateSettings041
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_TYPE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings041, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraType = OHOS_CAMERA_TYPE_LOGICAL;
    meta->addEntry(OHOS_ABILITY_CAMERA_TYPE, &abilityCameraType, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4200
 * @tc.name: testUpdateSettings042
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_TYPE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings042, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraType = OHOS_CAMERA_TYPE_UNSPECIFIED;
    meta->addEntry(OHOS_ABILITY_CAMERA_TYPE, &abilityCameraType, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4300
 * @tc.name: testUpdateSettings043
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_CONNECTION_TYPE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings043, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraConnType = OHOS_CAMERA_CONNECTION_TYPE_BUILTIN;
    meta->addEntry(OHOS_ABILITY_CAMERA_CONNECTION_TYPE, &abilityCameraConnType, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4400
 * @tc.name: testUpdateSettings044
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_CONNECTION_TYPE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings044, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraConnType = OHOS_CAMERA_CONNECTION_TYPE_USB_PLUGIN;
    meta->addEntry(OHOS_ABILITY_CAMERA_CONNECTION_TYPE, &abilityCameraConnType, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4500
 * @tc.name: testUpdateSettings045
 * @tc.desc: UpdateSettings, OHOS_ABILITY_CAMERA_CONNECTION_TYPE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings045, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t abilityCameraConnType = OHOS_CAMERA_CONNECTION_TYPE_REMOTE;
    meta->addEntry(OHOS_ABILITY_CAMERA_CONNECTION_TYPE, &abilityCameraConnType, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4600
 * @tc.name: testUpdateSettings046
 * @tc.desc: UpdateSettings, OHOS_CONTROL_EXPOSUREMODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings046, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t exposureMode = OHOS_CAMERA_EXPOSURE_MODE_MANUAL;
    meta->addEntry(OHOS_CONTROL_EXPOSUREMODE, &exposureMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4700
 * @tc.name: testUpdateSettings047
 * @tc.desc: UpdateSettings, OHOS_CONTROL_EXPOSUREMODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings047, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t exposureMode = OHOS_CAMERA_EXPOSURE_MODE_CONTINUOUS_AUTO;
    meta->addEntry(OHOS_CONTROL_EXPOSUREMODE, &exposureMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4800
 * @tc.name: testUpdateSettings048
 * @tc.desc: UpdateSettings, OHOS_CONTROL_EXPOSUREMODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings048, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t exposureMode = OHOS_CAMERA_EXPOSURE_MODE_LOCKED;
    meta->addEntry(OHOS_CONTROL_EXPOSUREMODE, &exposureMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_4900
 * @tc.name: testUpdateSettings049
 * @tc.desc: UpdateSettings, OHOS_CONTROL_EXPOSUREMODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings049, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t exposureMode = OHOS_CAMERA_EXPOSURE_MODE_AUTO;
    meta->addEntry(OHOS_CONTROL_EXPOSUREMODE, &exposureMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5000
 * @tc.name: testUpdateSettings050
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FLASH_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings050, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t flashState = OHOS_CAMERA_FLASH_STATE_UNAVAILABLE;
    meta->addEntry(OHOS_CONTROL_FLASH_STATE, &flashState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5100
 * @tc.name: testUpdateSettings051
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FLASH_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings051, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t flashState = OHOS_CAMERA_FLASH_STATE_CHARGING;
    meta->addEntry(OHOS_CONTROL_FLASH_STATE, &flashState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5200
 * @tc.name: testUpdateSettings052
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FLASH_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings052, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t flashState = OHOS_CAMERA_FLASH_STATE_READY;
    meta->addEntry(OHOS_CONTROL_FLASH_STATE, &flashState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5300
 * @tc.name: testUpdateSettings053
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FLASH_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings053, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t flashState = OHOS_CAMERA_FLASH_STATE_FLASHING;
    meta->addEntry(OHOS_CONTROL_FLASH_STATE, &flashState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5400
 * @tc.name: testUpdateSettings054
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FLASH_STATE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings054, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t flashState = OHOS_CAMERA_FLASH_STATE_UNKNOWN;
    meta->addEntry(OHOS_CONTROL_FLASH_STATE, &flashState, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5500
 * @tc.name: testUpdateSettings055
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FOCUS_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings055, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t focusMode = OHOS_CAMERA_FOCUS_MODE_MANUAL;
    meta->addEntry(OHOS_CONTROL_FOCUS_MODE, &focusMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5600
 * @tc.name: testUpdateSettings056
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FOCUS_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings056, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t focusMode = OHOS_CAMERA_FOCUS_MODE_CONTINUOUS_AUTO;
    meta->addEntry(OHOS_CONTROL_FOCUS_MODE, &focusMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5700
 * @tc.name: testUpdateSettings057
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FOCUS_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings057, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t focusMode = OHOS_CAMERA_FOCUS_MODE_AUTO;
    meta->addEntry(OHOS_CONTROL_FOCUS_MODE, &focusMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5800
 * @tc.name: testUpdateSettings058
 * @tc.desc: UpdateSettings, OHOS_CONTROL_FOCUS_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings058, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t focusMode = OHOS_CAMERA_FOCUS_MODE_LOCKED;
    meta->addEntry(OHOS_CONTROL_FOCUS_MODE, &focusMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_5900
 * @tc.name: testUpdateSettings059
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_LOCK
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings059, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeLock = OHOS_CAMERA_AE_LOCK_OFF;
    meta->addEntry(OHOS_CONTROL_AE_LOCK, &aeLock, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number: SUB_Driver_Camera_UpdateSettings_6000
 * @tc.name: testUpdateSettings060
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_LOCK
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings060, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t aeLock = OHOS_CAMERA_AE_LOCK_ON;
    meta->addEntry(OHOS_CONTROL_AE_LOCK, &aeLock, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}