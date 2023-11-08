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
    uint8_t aeMode = OHOS_CAMERA_AE_MODE_OFF;
    meta->addEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);
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
 * @tc.number: SUB_Driver_Camera_UpdateSettings_6300
 * @tc.name: testUpdateSettings063
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AWB_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings063, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_OFF;
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
 * @tc.number: SUB_Driver_Camera_UpdateSettings_6400
 * @tc.name: testUpdateSettings064
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AWB_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings064, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_INCANDESCENT;
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
 * @tc.number: SUB_Driver_Camera_UpdateSettings_6500
 * @tc.name: testUpdateSettings065
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AWB_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings065, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_FLUORESCENT;
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
 * @tc.number: SUB_Driver_Camera_UpdateSettings_6600
 * @tc.name: testUpdateSettings066
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AWB_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings066, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_WARM_FLUORESCENT;
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
 * @tc.number: SUB_Driver_Camera_UpdateSettings_6700
 * @tc.name: testUpdateSettings067
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AWB_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings067, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_DAYLIGHT;
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
 * @tc.number: SUB_Driver_Camera_UpdateSettings_6800
 * @tc.name: testUpdateSettings068
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AWB_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings068, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_CLOUDY_DAYLIGHT;
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
 * @tc.number: SUB_Driver_Camera_UpdateSettings_6900
 * @tc.name: testUpdateSettings069
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AWB_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings069, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_TWILIGHT;
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
 * @tc.number: SUB_Driver_Camera_UpdateSettings_7000
 * @tc.name: testUpdateSettings070
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AWB_MODE
 */
HWTEST_F(Camera3AUtTestAdditional, testUpdateSettings070, Function | MediumTest | Level1)
{
    cameraTest->Open();

    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<CameraMetadata> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_SHADE;
    meta->addEntry(OHOS_CONTROL_AWB_MODE, &awbMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}