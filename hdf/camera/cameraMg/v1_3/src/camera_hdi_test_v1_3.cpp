/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include "camera_hdi_test_v1_3.h"
#include <functional>

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;
constexpr uint32_t ITEM_CAPACITY = 100;
constexpr uint32_t DATA_CAPACITY = 2000;
constexpr uint32_t DATA_COUNT = 1;
constexpr uint32_t FPS_COUNT = 2;
constexpr uint32_t EXPOSURE_COUNT = 4;
int64_t OHOS::Camera::Test::StreamConsumer::g_timestamp[2] = {0};
void CameraHdiTestV1_3::SetUpTestCase(void) {}
void CameraHdiTestV1_3::TearDownTestCase(void) {}
void CameraHdiTestV1_3::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init(); // assert inside
    cameraTest->Open(DEVICE_0); // assert inside
}

void CameraHdiTestV1_3::TearDown(void)
{
    cameraTest->Close();
}

bool g_IsTagValueExistsU8(std::shared_ptr<CameraMetadata> ability, uint32_t tag, uint8_t value)
{
    common_metadata_header_t* data = ability->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, tag, &entry);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(entry.count, 0);
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == value) {
            return true;
        }
    }
    return false;
}

/**
 * @tc.name:SUB_Driver_Camera_Moon_0200
 * @tc.desc: Update moon ability setting
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_Moon_0200, TestSize.Level1)
{
    int32_t rc;
    // step 2: set callback object
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    rc = cameraTest->serviceV1_3->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(rc, 0);
    // Start OHOS_ABILITY_MOON_CAPTURE_BOOST ability query
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_MOON_CAPTURE_BOOST, &entry);

    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
        constexpr float zoomRatio = 15;
        uint8_t stabControl = OHOS_CAMERA_MOON_CAPTURE_BOOST_ENABLE;
        meta->addEntry(OHOS_CONTROL_ZOOM_RATIO, &zoomRatio, DATA_COUNT);
        meta->addEntry(OHOS_CONTROL_MOON_CAPTURE_BOOST, &stabControl, DATA_COUNT);
        // ability meta data serialization for updating
        std::vector<uint8_t> setting;
        MetadataUtils::ConvertMetadataToVec(meta, setting);
        cameraTest->rc = (CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
        EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
        CAMERA_LOGD("MoonCaptureBoost mode is set enabled.");

        cameraTest->intents = {PREVIEW, STILL_CAPTURE, VIDEO};
        cameraTest->StartStream(cameraTest->intents);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
        cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
        cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, true);
        sleep(1);
        cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdCapture};
        cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
        cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
        sleep(UT_SECOND_TIMES);
        common_metadata_header_t* data = cameraTest->deviceCallback->resultMeta->get();
        EXPECT_NE(data, nullptr);
        camera_metadata_item_t entry;
        int ret = FindCameraMetadataItem(data, OHOS_STATUS_MOON_CAPTURE_DETECTION, &entry);
        if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
            uint8_t value = entry.data.u8[0];
            // 查询到状态， 检测状态到 月亮模式可开启
            if (OHOS_CAMERA_MOON_CAPTURE_BOOST_ENABLE == value) {
                printf("Moon mode is set enabled.");
            } else {
                printf("Moon mode is not enabled.");
            }
        } else {
            printf("Moon is not exits.");
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_SlowMotion_0300
 * @tc.desc: CommitStreams_V1_1 for HIGH_FRAME_RATE, preview and video, fps is 120
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_SlowMotion_0300, TestSize.Level1)
{
    if (!g_IsTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_CAMERA_MODES, OHOS::HDI::Camera::V1_3::HIGH_FRAME_RATE)) {
        cout << "skip this test, because HIGH_FRAME_RATE not in OHOS_ABILITY_CAMERA_MODES" << endl;
        return;
    }
    cameraTest->CreateAndCommitStreamsForHighFrameRate(cameraTest);

    // Update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    int32_t highFrameRate[2] = {120, 120};
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, &highFrameRate, FPS_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // Start capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);

    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
    sleep(DATA_COUNT);
    cameraTest->streamInfosV1_1.clear();
}

/**
 * @tc.name: SUB_Driver_Camera_SlowMotion_0400
 * @tc.desc: CommitStreams_V1_1 for HIGH_FRAME_RATE, preview and video, fps is 240
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_SlowMotion_0400, TestSize.Level1)
{
    if (!g_IsTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_CAMERA_MODES, OHOS::HDI::Camera::V1_3::HIGH_FRAME_RATE)) {
        cout << "skip this test, because HIGH_FRAME_RATE not in OHOS_ABILITY_CAMERA_MODES" << endl;
        return;
    }
    cameraTest->CreateAndCommitStreamsForHighFrameRate(cameraTest);

    // Update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    int32_t highFrameRate[2] = {240, 240};
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, &highFrameRate, FPS_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // Start capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);

    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
    sleep(DATA_COUNT);
    cameraTest->streamInfosV1_1.clear();
}


/**
 * @tc.name: SUB_Driver_Camera_SuperSlowMotion_0400
 * @tc.desc: CommitStreams_V1_1 for SLOW_MOTION, preview and video, fps is 960
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_SuperSlowMotion_0400, TestSize.Level1)
{
    if (!g_IsTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_CAMERA_MODES, OHOS::HDI::Camera::V1_2::SLOW_MOTION)) {
        cout << "skip this test, because SLOW_MOTION not in OHOS_ABILITY_CAMERA_MODES" << endl;
        return;
    }
    cameraTest->CreateAndCommitStreamsForSlowMotion(cameraTest);

    // Update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    int32_t slowMotionValue[2] = {240, 240};
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, &slowMotionValue, FPS_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // Start capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);

    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
    sleep(DATA_COUNT);
    cameraTest->streamInfosV1_1.clear();
}

/**
 * @tc.name: SUB_Driver_Camera_SuperSlowMotion_0200
 * @tc.desc: OHOS_ABILITY_MOTION_DETECTION_SUPPORT, OHOS_CAMERA_MOTION_DETECTION_SUPPORTED
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_SuperSlowMotion_0200, TestSize.Level1)
{
    if (!g_IsTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_CAMERA_MODES, OHOS::HDI::Camera::V1_2::SLOW_MOTION)) {
        cout << "skip this test, because SLOW_MOTION not in OHOS_ABILITY_CAMERA_MODES" << endl;
        return;
    }
    // Set callback object
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    cameraTest->rc = cameraTest->serviceV1_3->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_MOTION_DETECTION_SUPPORT, &entry);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        EXPECT_TRUE(entry.data.u8 != nullptr);
        if (entry.data.u8[0] == OHOS_CAMERA_MOTION_DETECTION_SUPPORTED) {
            cameraTest->CreateAndCommitStreamsForSlowMotion(cameraTest);
            cameraTest->UpdateSettingsForSlowMotionMode(cameraTest);
            cameraTest->SuperSlowMotionStatusCallback(cameraTest);
        }
    }
}

/**
 * @tc.name:SUB_Driver_Camera_PerOptimization_0100
 * @tc.desc:CAPTURE_DURATION
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_PerOptimization_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAPTURE_DURATION_SUPPORTED, &entry);
    if (cameraTest->rc != 0 || entry.data.u8[0] != 1) {
        cout << "skip this test, because OHOS_ABILITY_CAPTURE_DURATION_SUPPORTED not supported now" << endl;
        return;
    }

    // Start stream
    cameraTest->intents = {PREVIEW, STILL_CAPTURE};
    cameraTest->StartStream(cameraTest->intents);

    // Fill capture setting
    std::shared_ptr<CameraSetting> modeSetting = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t muteMode = static_cast<uint8_t>(OHOS_CAMERA_MUTE_MODE_OFF);
    modeSetting->addEntry(OHOS_CONTROL_MUTE_MODE, &muteMode, DATA_COUNT);
    uint8_t deferredImage = OHOS::HDI::Camera::V1_2::STILL_IMAGE;
    modeSetting->addEntry(OHOS_CONTROL_DEFERRED_IMAGE_DELIVERY, &deferredImage, DATA_COUNT);
    std::vector<uint8_t> controlVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, controlVec);
    cameraTest->abilityVec = controlVec;

    // Start preview and capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, true, true);
    // OncaptureReady trigger
    for (uint8_t i = 0; i < 2; i++) {
        cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    }

    // Release stream
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);

    sleep(UT_SECOND_TIMES);
    common_metadata_header_t* callbackData = cameraTest->deviceCallback->resultMeta->get();
    EXPECT_NE(callbackData, nullptr);
    camera_metadata_item_t callbackEntry;
    cameraTest->rc = FindCameraMetadataItem(callbackData, OHOS_CAMERA_CUSTOM_SNAPSHOT_DURATION, &callbackEntry);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    printf("currentSnapshotDuration = %d\n", callbackEntry.data.ui32[0]);
}

/**
 * @tc.name: SUB_Driver_Camera_Moving_0200
 * @tc.desc: OHOS_CONTROL_MOVING_PHOTO OHOS_CAMERA_MOVING_PHOTO_OFF
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_Moving_0200, TestSize.Level1)
{
    CAMERA_LOGI("test Camera_Device_Hdi_V1_3_013 start.");
    cameraTest->streamOperatorCallbackV1_3 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_3();
    cameraTest->rc = cameraTest->cameraDeviceV1_3->GetStreamOperator_V1_3(
        cameraTest->streamOperatorCallbackV1_3, cameraTest->streamOperator_V1_3);
    EXPECT_NE(cameraTest->streamOperator_V1_3, nullptr);

    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    std::shared_ptr<CameraSetting> modeSetting = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t movingPhoto = static_cast<uint8_t>(OHOS_CAMERA_MOVING_PHOTO_OFF);
    modeSetting->addEntry(OHOS_CONTROL_MOVING_PHOTO, &movingPhoto, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, metaVec);
    cameraTest->cameraDeviceV1_3->UpdateSettings(metaVec);

    cameraTest->rc = cameraTest->streamOperator_V1_3->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_3->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_Moving_0300
 * @tc.desc: OHOS_CONTROL_MOVING_PHOTO OHOS_CAMERA_MOVING_PHOTO_ON
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_Moving_0300, TestSize.Level1)
{
    CAMERA_LOGI("test Moving Photo Switch On");
    cameraTest->streamOperatorCallbackV1_3 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_3();
    cameraTest->rc = cameraTest->cameraDeviceV1_3->GetStreamOperator_V1_3(
        cameraTest->streamOperatorCallbackV1_3, cameraTest->streamOperator_V1_3);
    EXPECT_NE(cameraTest->streamOperator_V1_3, nullptr);

    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    std::shared_ptr<CameraSetting> modeSetting = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t movingPhoto = static_cast<uint8_t>(OHOS_CAMERA_MOVING_PHOTO_ON);
    modeSetting->addEntry(OHOS_CONTROL_MOVING_PHOTO, &movingPhoto, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, metaVec);
    cameraTest->cameraDeviceV1_3->UpdateSettings(metaVec);

    cameraTest->rc = cameraTest->streamOperator_V1_3->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_3->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_HighSolution_0200
 * @tc.desc: CommitStreams_V1_1 for HIGH_RESOLUTION_PHOTO, preview and capture
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_HighResolution_0200, TestSize.Level1)
{
    if (!g_IsTagValueExistsU8(cameraTest->ability,
        OHOS_ABILITY_CAMERA_MODES, OHOS::HDI::Camera::V1_3::HIGH_RESOLUTION_PHOTO)) {
        cout << "skip this test, because HIGH_RESOLUTION_PHOTO not in OHOS_ABILITY_CAMERA_MODES" << endl;
        return;
    }
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAPTURE_EXPECT_TIME, &entry);
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.ui32, nullptr);
        EXPECT_EQ(entry.count > 1, true);
        CAMERA_LOGI("mode is %{public}u and captureExpectTime is %{public}u", entry.data.ui32[0], entry.data.ui32[1]);
    }
    cameraTest->startStreamForHighResolutionPhoto(cameraTest);

    // Update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    int32_t aeExposureCompensation = EXPOSURE_COUNT;
    meta->addEntry(OHOS_CONTROL_AE_EXPOSURE_COMPENSATION, &aeExposureCompensation, DATA_COUNT);
    uint8_t focusMode = OHOS_CAMERA_FOCUS_MODE_AUTO;
    meta->addEntry(OHOS_CONTROL_FOCUS_MODE, &focusMode, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_SMPhoto_0200
 * @tc.desc: CommitStreams_V1_1 for CAPTURE_MACRO, preview and capture
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_SMPhoto_0200, TestSize.Level1)
{
    if (!g_IsTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_CAMERA_MODES, OHOS::HDI::Camera::V1_3::CAPTURE_MACRO)) {
        cout << "skip this test, because CAPTURE_MACRO not in OHOS_ABILITY_CAMERA_MODES" << endl;
        return;
    }
    // Get stream operator
    cameraTest->streamOperatorCallbackV1_3 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_3();
    cameraTest->rc = cameraTest->cameraDeviceV1_3->GetStreamOperator_V1_3(cameraTest->streamOperatorCallbackV1_3,
        cameraTest->streamOperator_V1_3);
    EXPECT_NE(cameraTest->streamOperator_V1_3, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // Preview streamInfo
    cameraTest->streamInfoPre = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoPre);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoPre);
    // Capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);
    cameraTest->rc = cameraTest->streamOperator_V1_3->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_3->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_3::CAPTURE_MACRO),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // Update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    float lensFocusDistance = DATA_COUNT;
    meta->addEntry(OHOS_CONTROL_LENS_FOCUS_DISTANCE, &lensFocusDistance, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);

    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_SMVideo_0200
 * @tc.desc: CommitStreams_V1_1 for VIDEO_MACRO, preview and capture
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_SMVideo_0200, TestSize.Level1)
{
    if (!g_IsTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_CAMERA_MODES, OHOS::HDI::Camera::V1_3::VIDEO_MACRO)) {
        cout << "skip this test, because VIDEO_MACRO not in OHOS_ABILITY_CAMERA_MODES" << endl;
        return;
    }
    // Get stream operator
    cameraTest->streamOperatorCallbackV1_3 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_3();
    cameraTest->rc = cameraTest->cameraDeviceV1_3->GetStreamOperator_V1_3(cameraTest->streamOperatorCallbackV1_3,
        cameraTest->streamOperator_V1_3);
    EXPECT_NE(cameraTest->streamOperator_V1_3, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // Preview streamInfo
    cameraTest->streamInfoPre = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoPre);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoPre);
    // Capture streamInfo
    cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
    // Create and commit streams
    cameraTest->rc = cameraTest->streamOperator_V1_3->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_3->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_3::VIDEO_MACRO),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // Update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    float lensFocusDistance = DATA_COUNT;
    meta->addEntry(OHOS_CONTROL_LENS_FOCUS_DISTANCE, &lensFocusDistance, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);

    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_ZoomBand_0100
 * @tc.desc:OHOS_ABILITY_EQUIVALENT_FOCUS
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_ZoomBand_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_EQUIVALENT_FOCUS, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_EQUIVALENT_FOCUS can not be find");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.i32, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        CAMERA_LOGI("print tag<OHOS_ABILITY_EQUIVALENT_FOCUS> value start.");
        constexpr size_t step = 10; // print step
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.i32[i] << " ";
            if ((i != 0) && (i % step == 0 || i == entry.count - 1)) {
                CAMERA_LOGI("%{public}s\n", ss.str().c_str());
                ss.clear();
                ss.str("");
            }
        }
        CAMERA_LOGI("print tag<OHOS_ABILITY_EQUIVALENT_FOCUS> value end.");
    }
}