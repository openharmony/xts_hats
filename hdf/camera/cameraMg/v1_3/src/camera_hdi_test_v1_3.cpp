/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        for (int i = 0; i < entry.count; i++) {
            if (entry.data.u8[i] == value) {
                return true;
            }
        }
    } else {
        printf("Find CameraMetadata fail!\n");
        CAMERA_LOGE("Find CameraMetadata fail!");
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

        cameraTest->intents = {PREVIEW, STILL_CAPTURE};
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
        EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
        if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
            uint8_t value = entry.data.u8[0];
            // 查询到状态， 检测状态到 月亮模式可开启
            if (OHOS_CAMERA_MOON_CAPTURE_BOOST_ENABLE == value) {
                printf("Moon mode is set enabled.");
            } else {
                printf("Moon mode is not enabled.");
            }
        } else {
            printf("Moon is not exits .");
        }
    }
}

/**
 * @tc.name:SUB_Driver_Camera_Moon_0300
 * @tc.desc: Update moon ability setting
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_Moon_0300, TestSize.Level1)
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

        cameraTest->intents = {PREVIEW, STILL_CAPTURE};
        cameraTest->StartStream(cameraTest->intents);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
        cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
        cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, true);
        sleep(1);
        cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdCapture};
        cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
        cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
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
 * @tc.name:SUB_Driver_Camera_DynamicFps_0100
 * @tc.desc:Dynamic FPS configuration, range setting, streams fps constrain
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_DynamicFps_0100, TestSize.Level1)
{
    // PREVIEW and VIDEO stream
    cameraTest->intents = {PREVIEW, VIDEO};
    // This requirement only in VIDEO mode
    cameraTest->StartStream(cameraTest->intents, OHOS::HDI::Camera::V1_3::OperationMode::VIDEO);
    // Bind fps range with preview stream and video stream
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_CONTROL_FPS_RANGES, &entry);
    // The FPS only valid in current release, non backward compatibility
    int32_t fpsRanges[] = {1, 30};
    bool result;
    if (cameraTest->rc != CAM_META_SUCCESS) {
        std::cout << "Not found TAG[OHOS_CONTROL_FPS_RANGES], insert one" << std::endl;
        result = cameraTest->ability->addEntry(OHOS_CONTROL_FPS_RANGES,
            fpsRanges, sizeof(fpsRanges) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    } else {
        std::cout << "Found TAG[OHOS_CONTROL_FPS_RANGES], Update it" << std::endl;
        result = cameraTest->ability->updateEntry(OHOS_CONTROL_FPS_RANGES,
            fpsRanges, sizeof(fpsRanges) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    }
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(cameraTest->ability, cameraTest->abilityVec);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    // Release stream
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_DynamicFps_0200
 * @tc.desc:Dynamic FPS configuration, range setting, streams fps constrain
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_DynamicFps_0200, TestSize.Level1)
{
    // PREVIEW and VIDEO stream
    cameraTest->intents = {PREVIEW, VIDEO};
    // This requirement only in VIDEO mode
    cameraTest->StartStream(cameraTest->intents, OHOS::HDI::Camera::V1_3::OperationMode::VIDEO);
    // Bind fps range with preview stream and video stream
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_CONTROL_FPS_RANGES, &entry);
    // The FPS only valid in current release, non backward compatibility
    int32_t fpsRanges[] = {35, 60};
    bool result;
    if (cameraTest->rc != CAM_META_SUCCESS) {
        std::cout << "Not found TAG[OHOS_CONTROL_FPS_RANGES], insert one" << std::endl;
        result = cameraTest->ability->addEntry(OHOS_CONTROL_FPS_RANGES,
            fpsRanges, sizeof(fpsRanges) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    } else {
        std::cout << "Found TAG[OHOS_CONTROL_FPS_RANGES], Update it" << std::endl;
        result = cameraTest->ability->updateEntry(OHOS_CONTROL_FPS_RANGES,
            fpsRanges, sizeof(fpsRanges) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    }
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(cameraTest->ability, cameraTest->abilityVec);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    // Release stream
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
+ * @tc.name:SUB_Driver_Camera_DynamicFps_0300
+ * @tc.desc:Dynamic FPS configuration, fixed fps setting, streams fps constrain
+ * @tc.size:MediumTest
+ * @tc.type:Function
+*/
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_DynamicFps_0300, TestSize.Level1)
{
    // PREVIEW and VIDEO stream
    cameraTest->intents = {PREVIEW, VIDEO};
    // This requirement only in VIDEO mode
    cameraTest->StartStream(cameraTest->intents, OHOS::HDI::Camera::V1_3::OperationMode::VIDEO);
    // Bind fixed fps with preview stream and video stream, constraint
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_CONTROL_FPS_RANGES, &entry);
    // The FPS only valid in current release, non backward compatibility
    int32_t previewFixedFps[] = {30, 30};
    bool result;
    if (cameraTest->rc != CAM_META_SUCCESS) {
        std::cout << "Not found TAG[OHOS_CONTROL_FPS_RANGES], insert one" << std::endl;
        result = cameraTest->ability->addEntry(OHOS_CONTROL_FPS_RANGES,
            previewFixedFps, sizeof(previewFixedFps) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    } else {
        std::cout << "Found TAG[OHOS_CONTROL_FPS_RANGES], Update it" << std::endl;
        result = cameraTest->ability->updateEntry(OHOS_CONTROL_FPS_RANGES,
            previewFixedFps, sizeof(previewFixedFps) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    }
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(cameraTest->ability, cameraTest->abilityVec);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    // Update video fixed fps, constraint relationship
    int32_t videoFixedFps[] = {60, 60};
    std::cout << "Update fixed fps for video capture" << std::endl;
    result = cameraTest->ability->updateEntry(OHOS_CONTROL_FPS_RANGES,
        videoFixedFps, sizeof(videoFixedFps) / sizeof(int32_t));
    EXPECT_EQ(true, result);
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(cameraTest->ability, cameraTest->abilityVec);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    // Release stream
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
+ * @tc.name:SUB_Driver_Camera_DynamicFps_0400
+ * @tc.desc:Dynamic FPS configuration, fixed fps setting, streams fps constrain
+ * @tc.size:MediumTest
+ * @tc.type:Function
+*/
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_DynamicFps_0400, TestSize.Level1)
{
    // PREVIEW and VIDEO stream
    cameraTest->intents = {PREVIEW, VIDEO};
    // This requirement only in VIDEO mode
    cameraTest->StartStream(cameraTest->intents, OHOS::HDI::Camera::V1_3::OperationMode::VIDEO);
    // Bind fixed fps with preview stream and video stream, constraint
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_CONTROL_FPS_RANGES, &entry);
    // The FPS only valid in current release, non backward compatibility
    int32_t previewFixedFps[] = {45, 45};
    bool result;
    if (cameraTest->rc != CAM_META_SUCCESS) {
        std::cout << "Not found TAG[OHOS_CONTROL_FPS_RANGES], insert one" << std::endl;
        result = cameraTest->ability->addEntry(OHOS_CONTROL_FPS_RANGES,
            previewFixedFps, sizeof(previewFixedFps) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    } else {
        std::cout << "Found TAG[OHOS_CONTROL_FPS_RANGES], Update it" << std::endl;
        result = cameraTest->ability->updateEntry(OHOS_CONTROL_FPS_RANGES,
            previewFixedFps, sizeof(previewFixedFps) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    }
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(cameraTest->ability, cameraTest->abilityVec);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    // Update video fixed fps, constraint relationship
    int32_t videoFixedFps[] = {15, 15};
    std::cout << "Update fixed fps for video capture" << std::endl;
    result = cameraTest->ability->updateEntry(OHOS_CONTROL_FPS_RANGES,
        videoFixedFps, sizeof(videoFixedFps) / sizeof(int32_t));
    EXPECT_EQ(true, result);
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(cameraTest->ability, cameraTest->abilityVec);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    // Release stream
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};

    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_DynamicFps_0500
 * @tc.desc:Dynamic FPS configuration, range setting, streams fps constrain
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_DynamicFps_0500, TestSize.Level1)
{
    // PREVIEW and VIDEO stream
    cameraTest->intents = {PREVIEW, VIDEO};
    // This requirement only in VIDEO mode
    cameraTest->StartStream(cameraTest->intents, OHOS::HDI::Camera::V1_3::OperationMode::VIDEO);
    // Bind fps range with preview stream and video stream
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_CONTROL_FPS_RANGES, &entry);
    // The FPS only valid in current release, non backward compatibility
    int32_t fpsRanges[] = {30, 30};
    bool result;
    if (cameraTest->rc != CAM_META_SUCCESS) {
        std::cout << "Not found TAG[OHOS_CONTROL_FPS_RANGES], insert one" << std::endl;
        result = cameraTest->ability->addEntry(OHOS_CONTROL_FPS_RANGES,
            fpsRanges, sizeof(fpsRanges) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    } else {
        std::cout << "Found TAG[OHOS_CONTROL_FPS_RANGES], Update it" << std::endl;
        result = cameraTest->ability->updateEntry(OHOS_CONTROL_FPS_RANGES,
            fpsRanges, sizeof(fpsRanges) / sizeof(int32_t));
        EXPECT_EQ(true, result);
    }
    OHOS::Camera::MetadataUtils::ConvertMetadataToVec(cameraTest->ability, cameraTest->abilityVec);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    // Release stream
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}
/**
 * @tc.name: SUB_Driver_Camera_HighQuality_0200
 * @tc.desc: OHOS_CONTROL_HIGH_QUALITY_MODE OHOS_CONTROL_HIGH_QUALITY_MODE_OFF
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_HighQuality_0200, TestSize.Level1)
{
    CAMERA_LOGI("test OHOS_CONTROL_HIGH_QUALITY_MODE_OFF start.");
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
    uint8_t movingPhoto = static_cast<uint8_t>(OHOS_CONTROL_HIGH_QUALITY_MODE_OFF);
    modeSetting->addEntry(OHOS_CONTROL_HIGH_QUALITY_MODE, &movingPhoto, 1);
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
 * @tc.name: SUB_Driver_Camera_HighQuality_0300
 * @tc.desc: OHOS_CONTROL_HIGH_QUALITY_MODE OHOS_CONTROL_HIGH_QUALITY_MODE_ON
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_HighQuality_0300, TestSize.Level1)
{
    CAMERA_LOGI("test OHOS_CONTROL_HIGH_QUALITY_MODE_ON start.");
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
    uint8_t movingPhoto = static_cast<uint8_t>(OHOS_CONTROL_HIGH_QUALITY_MODE_ON);
    modeSetting->addEntry(OHOS_CONTROL_HIGH_QUALITY_MODE, &movingPhoto, 1);
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

void UpdateMetadata(std::shared_ptr<OHOS::Camera::Test> cameraTest, std::shared_ptr<CameraSetting> meta)
{
    // 修改Zoom大于15x
    float zoomRatio = 16.0f;
    meta->addEntry(OHOS_CONTROL_ZOOM_RATIO, &zoomRatio, DATA_COUNT);
    // 使能脚架检测
    uint8_t tripoDetection = 1;
    meta->addEntry(OHOS_CONTROL_TRIPOD_DETECTION, &tripoDetection, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
}

/**
 * @tc.name: SUB_Driver_Camera_Tripod_0200
 * @tc.desc: OHOS_ABILITY_TRIPOD_DETECTION
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_Tripod_0200, TestSize.Level1)
{
    CAMERA_LOGI("CameraHdiTestV1_3 SUB_Driver_Camera_Tripod_0200 start.");
    // 查询是否支持脚架检测
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_TRIPOD_DETECTION, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || entry.data.i32 == nullptr
        || entry.count <= 0 || entry.data.i32[0] != 1) {
        cout << "skip this test, because OHOS_ABILITY_TRIPOD_DETECTION not supported" << endl;
        return;
    }
    cameraTest->intents = {PREVIEW, STILL_CAPTURE};
    cameraTest->StartStream(cameraTest->intents, OHOS::HDI::Camera::V1_3::CAPTURE);
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    UpdateMetadata(cameraTest, meta);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    sleep(UT_SECOND_TIMES);
    if (cameraTest->deviceCallback->resultMeta == nullptr) {
        CAMERA_LOGI("Camera_Device_Hdi_V1_3_048 onresult not be invoked.");
        return;
    }
    common_metadata_header_t* resultData = cameraTest->deviceCallback->resultMeta->get();
    if (resultData == nullptr) {
        CAMERA_LOGI("Camera_Device_Hdi_V1_3_048 onresult be invoked but data was nullptr.");
        return;
    }
    camera_metadata_item_t statusEntry;
    cameraTest->rc = FindCameraMetadataItem(resultData, OHOS_STATUS_TRIPOD_DETECTION_STATUS, &statusEntry);
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR && statusEntry.data.u8 != nullptr && statusEntry.count > 0) {
        CAMERA_LOGI("OHOS_STATUS_TRIPOD_DETECTION_STATUS value:%{public}d", statusEntry.data.u8[0]);
        // 使能脚架检测算法
        meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
        uint8_t tripodStablitationAlgorithm = 1;
        meta->addEntry(OHOS_CONTROL_TRIPOD_STABLITATION, &tripodStablitationAlgorithm, DATA_COUNT);
        std::vector<uint8_t> pointData;
        MetadataUtils::ConvertMetadataToVec(meta, pointData);
        cameraTest->cameraDeviceV1_3->UpdateSettings(pointData);
    }
    camera_metadata_item_t pointEntry;
    cameraTest->rc = FindCameraMetadataItem(resultData, OHOS_STATUS_SKETCH_POINT, &pointEntry);
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR && pointEntry.data.f != nullptr && pointEntry.count > 0) {
        CAMERA_LOGI("OHOS_STATUS_SKETCH_POINT x:%{public}f y:%{public}f", pointEntry.data.f[0], pointEntry.data.f[1]);
    }
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdCapture, cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

void FindHumanDetectResult(common_metadata_header_t* streamData)
{
    camera_metadata_item_t faceEntry;
    int32_t rc = FindCameraMetadataItem(streamData, OHOS_STATISTICS_DETECT_HUMAN_FACE_INFOS, &faceEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (faceEntry.data.i32 != nullptr && faceEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult human face result");
        }
    }
    camera_metadata_item_t bodyEntry;
    rc = FindCameraMetadataItem(streamData, OHOS_STATISTICS_DETECT_HUMAN_BODY_INFOS, &bodyEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (bodyEntry.data.i32 != nullptr && bodyEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult human body result");
        }
    }
    camera_metadata_item_t baseFaceEntry;
    rc = FindCameraMetadataItem(streamData, OHOS_STATISTICS_DETECT_BASE_FACE_INFO, &baseFaceEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (baseFaceEntry.data.i32 != nullptr && baseFaceEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult base face result");
        }
    }
}

void FindOtherDetectResult(common_metadata_header_t* streamData)
{
    camera_metadata_item_t catFaceEntry;
    int32_t rc = FindCameraMetadataItem(streamData, OHOS_STATISTICS_DETECT_CAT_FACE_INFOS, &catFaceEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (catFaceEntry.data.i32 != nullptr && catFaceEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult cat face result");
        }
    }
    camera_metadata_item_t catBodyEntry;
    rc = FindCameraMetadataItem(streamData, OHOS_STATISTICS_DETECT_CAT_BODY_INFOS, &catBodyEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (catBodyEntry.data.i32 != nullptr && catBodyEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult cat body result");
        }
    }
    camera_metadata_item_t dogFaceEntry;
    rc = FindCameraMetadataItem(streamData, OHOS_STATISTICS_DETECT_DOG_FACE_INFOS, &dogFaceEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (dogFaceEntry.data.i32 != nullptr && dogFaceEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult dog face result");
        }
    }
    camera_metadata_item_t dogBodyEntry;
    rc = FindCameraMetadataItem(streamData, OHOS_STATISTICS_DETECT_DOG_BODY_INFOS, &dogBodyEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (dogBodyEntry.data.i32 != nullptr && dogBodyEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult dog body result");
        }
    }
    camera_metadata_item_t salientEntry;
    rc = FindCameraMetadataItem(streamData, OHOS_STATISTICS_DETECT_SALIENT_INFOS, &salientEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (salientEntry.data.i32 != nullptr && salientEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult salient result");
        }
    }
    camera_metadata_item_t barCodeEntry;
    rc = FindCameraMetadataItem(streamData, OHOS_STATISTICS_DETECT_BAR_CODE_INFOS, &barCodeEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (barCodeEntry.data.i32 != nullptr && barCodeEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult bar code result");
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Detect_0200
 * @tc.desc: OHOS_ABILITY_STATISTICS_DETECT_TYPE,EnableResult
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_Detect_0200, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_STATISTICS_DETECT_TYPE, &entry);
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        cameraTest->intents = {PREVIEW, StreamIntent::VIDEO};
        cameraTest->StartStream(cameraTest->intents, OHOS::HDI::Camera::V1_3::VIDEO);
        cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
        cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
        std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
        std::vector<uint8_t> detectTypes;
        for (int i = 0; i < entry.count; i++) {
            detectTypes.push_back(entry.data.u8[i]);
        }
        uint8_t* typesToEnable = detectTypes.data();
        meta->addEntry(OHOS_CONTROL_STATISTICS_DETECT_SETTING, typesToEnable, detectTypes.size());
        std::vector<uint8_t> setting;
        MetadataUtils::ConvertMetadataToVec(meta, setting);
        cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->streamOperator_V1_3->EnableResult(
                cameraTest->streamIdVideo, setting);
        ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
        sleep(UT_SECOND_TIMES);
        if (cameraTest->streamOperatorCallbackV1_3->streamResultMeta == nullptr) {
            CAMERA_LOGI("Camera_Device_Hdi_V1_3_053 onresult not be invoked.");
            return;
        }
        common_metadata_header_t* streamData = cameraTest->streamOperatorCallbackV1_3->streamResultMeta->get();
        if (data == nullptr) {
            CAMERA_LOGI("Camera_Device_Hdi_V1_3_053 onresult be invoked but data was nullptr.");
            return;
        }
        FindHumanDetectResult(streamData);
        FindOtherDetectResult(streamData);
        cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
        cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
        cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Detect_0300
 * @tc.desc: OHOS_ABILITY_STATISTICS_DETECT_TYPE,EnableResult
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_3, SUB_Driver_Camera_Detect_0300, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_STATISTICS_DETECT_TYPE, &entry);
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        cameraTest->intents = {PREVIEW, StreamIntent::VIDEO};
        cameraTest->StartStream(cameraTest->intents, OHOS::HDI::Camera::V1_3::VIDEO);
        cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
        cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
        std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
        std::vector<uint8_t> detectTypes;
        for (int i = 0; i < entry.count; i++) {
            detectTypes.push_back(entry.data.u8[i]);
        }
        uint8_t* typesToEnable = detectTypes.data();
        meta->addEntry(OHOS_CONTROL_STATISTICS_DETECT_SETTING, typesToEnable, detectTypes.size());
        std::vector<uint8_t> setting;
        MetadataUtils::ConvertMetadataToVec(meta, setting);
        cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->streamOperator_V1_3->DisableResult(
                cameraTest->streamIdVideo, setting);
        ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
        sleep(UT_SECOND_TIMES);
        if (cameraTest->streamOperatorCallbackV1_3->streamResultMeta == nullptr) {
            CAMERA_LOGI("Camera_Device_Hdi_V1_3_053 onresult not be invoked.");
            return;
        }
        common_metadata_header_t* streamData = cameraTest->streamOperatorCallbackV1_3->streamResultMeta->get();
        if (data == nullptr) {
            CAMERA_LOGI("Camera_Device_Hdi_V1_3_053 onresult be invoked but data was nullptr.");
            return;
        }
        FindHumanDetectResult(streamData);
        FindOtherDetectResult(streamData);
        cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
        cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
        cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
    }
}

/**
 * @tc.name:SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_FOCUS_RANGE_TYPE_0100
 * @tc.desc: test TAG OHOS_ABILITY_FOCUS_RANGE_TYPES and OHOS_CONTROL_FOCUS_RANGE_TYPE
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_FOCUS_RANGE_TYPE_0100, TestSize.Level1)
{
    cameraTest->Close();
    cameraTest->Open(DEVICE_1); // front camera
    ASSERT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_FOCUS_RANGE_TYPES, &entry);
    if (cameraTest->rc != CAM_META_SUCCESS || entry.count == 0) {
        CAMERA_LOGE("OHOS_ABILITY_FOCUS_RANGE_TYPES is not supported");
        return;
    }
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == OHOS_CAMERA_FOCUS_RANGE_NEAR) {
            CAMERA_LOGI("focus range type OHOS_CAMERA_FOCUS_RANGE_NEAR is supported");
        } else if (entry.data.u8[i] == OHOS_CAMERA_FOCUS_RANGE_AUTO) {
            CAMERA_LOGI("focus range type OHOS_CAMERA_FOCUS_RANGE_AUTO is supported");
        } else {
            CAMERA_LOGE("focus range type is not supported");
            return;
        }
    }
    camera_metadata_item_t item;
    bool status = false;
    uint8_t metaFocusRangeType = static_cast<uint8_t>(OHOS_CAMERA_FOCUS_RANGE_NEAR);
    int ret = FindCameraMetadataItem(data, OHOS_CONTROL_FOCUS_RANGE_TYPE, &item);
    if (ret == CAM_META_ITEM_NOT_FOUND) {
        status = cameraTest->ability->addEntry(OHOS_CONTROL_FOCUS_RANGE_TYPE, &metaFocusRangeType, DATA_COUNT);
        CAMERA_LOGI("add OHOS_CAMERA_FOCUS_RANGE_NEAR to ability");
    } else if (ret == CAM_META_SUCCESS) {
        status = cameraTest->ability->updateEntry(OHOS_CONTROL_FOCUS_RANGE_TYPE, &metaFocusRangeType, DATA_COUNT);
        CAMERA_LOGI("update OHOS_CAMERA_FOCUS_RANGE_NEAR to ability");
    }
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<uint8_t> setting;
    uint8_t nearType = 1;
    meta->addEntry(OHOS_CONTROL_FOCUS_RANGE_TYPE, &nearType, DATA_COUNT);
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    CAMERA_LOGI("first: update settings success");
    ret = FindCameraMetadataItem(data, OHOS_CONTROL_FOCUS_RANGE_TYPE, &item);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    EXPECT_TRUE(item.data.u8[0] == OHOS_CAMERA_FOCUS_RANGE_NEAR);
}

/**
 * @tc.name:SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_FOCUS_RANGE_TYPE_0200
 * @tc.desc: test TAG OHOS_ABILITY_FOCUS_RANGE_TYPES and OHOS_CONTROL_FOCUS_RANGE_TYPE
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_FOCUS_RANGE_TYPE_0200, TestSize.Level1)
{
    cameraTest->Close();
    cameraTest->Open(DEVICE_1); // front camera
    ASSERT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_FOCUS_RANGE_TYPES, &entry);
    if (cameraTest->rc != CAM_META_SUCCESS || entry.count == 0) {
        CAMERA_LOGE("OHOS_ABILITY_FOCUS_RANGE_TYPES is not supported");
        return;
    }
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == OHOS_CAMERA_FOCUS_RANGE_NEAR) {
            CAMERA_LOGI("focus range type OHOS_CAMERA_FOCUS_RANGE_NEAR is supported");
        } else if (entry.data.u8[i] == OHOS_CAMERA_FOCUS_RANGE_AUTO) {
            CAMERA_LOGI("focus range type OHOS_CAMERA_FOCUS_RANGE_AUTO is supported");
        } else {
            CAMERA_LOGE("focus range type is not supported");
            return;
        }
    }
    camera_metadata_item_t item;
    bool status = false;
    std::vector<uint8_t> setting;
    uint8_t metaFocusRangeType = static_cast<uint8_t>(OHOS_CAMERA_FOCUS_RANGE_AUTO);
    int ret = FindCameraMetadataItem(data, OHOS_CONTROL_FOCUS_RANGE_TYPE, &item);
    if (ret == CAM_META_ITEM_NOT_FOUND) {
        status = cameraTest->ability->addEntry(OHOS_CONTROL_FOCUS_RANGE_TYPE, &metaFocusRangeType, DATA_COUNT);
        CAMERA_LOGI("add OHOS_CAMERA_FOCUS_RANGE_AUTO to ability");
    } else if (ret == CAM_META_SUCCESS) {
        status = cameraTest->ability->updateEntry(OHOS_CONTROL_FOCUS_RANGE_TYPE, &metaFocusRangeType, DATA_COUNT);
        CAMERA_LOGI("update OHOS_CAMERA_FOCUS_RANGE_AUTO to ability");
    }
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t autoType = 0;
    meta->addEntry(OHOS_CONTROL_FOCUS_RANGE_TYPE, &autoType, DATA_COUNT);
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    CAMERA_LOGI("second: update settings success");
    ret = FindCameraMetadataItem(data, OHOS_CONTROL_FOCUS_RANGE_TYPE, &item);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    EXPECT_TRUE(item.data.u8[0] == OHOS_CAMERA_FOCUS_RANGE_AUTO);
}

/**
 * @tc.name:SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_FOCUS_DRIVEN_TYPE_0100
 * @tc.desc: test TAG OHOS_ABILITY_FOCUS_DRIVEN_TYPES and OHOS_CONTROL_FOCUS_DRIVEN_TYPE
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_FOCUS_DRIVEN_TYPE_0100, TestSize.Level1)
{
    ASSERT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_FOCUS_DRIVEN_TYPES, &entry);
    if (cameraTest->rc != CAM_META_SUCCESS || entry.count == 0) {
        CAMERA_LOGE("OHOS_ABILITY_FOCUS_DRIVEN_TYPES is not supported");
        return;
    }
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == OHOS_CAMERA_FOCUS_DRIVEN_FACE) {
            CAMERA_LOGI("focus driven type OHOS_CAMERA_FOCUS_DRIVEN_FACE is supported");
        } else if (entry.data.u8[i] == OHOS_CAMERA_FOCUS_DRIVEN_AUTO) {
            CAMERA_LOGI("focus driven type OHOS_CAMERA_FOCUS_DRIVEN_AUTO is supported");
        } else {
            CAMERA_LOGE("focus driven type is not supported");
            return;
        }
    }
    camera_metadata_item_t item;
    bool status = false;
    uint8_t metaFocusDrivenType = static_cast<uint8_t>(OHOS_CAMERA_FOCUS_DRIVEN_FACE);
    int ret = FindCameraMetadataItem(data, OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &item);
    if (ret == CAM_META_ITEM_NOT_FOUND) {
        status = cameraTest->ability->addEntry(OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &metaFocusDrivenType, DATA_COUNT);
        CAMERA_LOGI("add OHOS_CAMERA_FOCUS_DRIVEN_FACE to ability");
    } else if (ret == CAM_META_SUCCESS) {
        status = cameraTest->ability->updateEntry(OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &metaFocusDrivenType, DATA_COUNT);
        CAMERA_LOGI("update OHOS_CAMERA_FOCUS_DRIVEN_FACE to ability");
    }
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<uint8_t> setting;
    uint8_t faceType = 1;
    meta->addEntry(OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &faceType, DATA_COUNT);
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    CAMERA_LOGI("first: update settings success");
    ret = FindCameraMetadataItem(data, OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &item);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    EXPECT_TRUE(item.data.u8[0] == OHOS_CAMERA_FOCUS_DRIVEN_FACE);
}

/**
 * @tc.name:SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_FOCUS_DRIVEN_TYPE_0200
 * @tc.desc: test TAG OHOS_ABILITY_FOCUS_DRIVEN_TYPES and OHOS_CONTROL_FOCUS_DRIVEN_TYPE
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_FOCUS_DRIVEN_TYPE_0200, TestSize.Level1)
{
    ASSERT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_FOCUS_DRIVEN_TYPES, &entry);
    if (cameraTest->rc != CAM_META_SUCCESS || entry.count == 0) {
        CAMERA_LOGE("OHOS_ABILITY_FOCUS_DRIVEN_TYPES is not supported");
        return;
    }
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == OHOS_CAMERA_FOCUS_DRIVEN_FACE) {
            CAMERA_LOGI("focus driven type OHOS_CAMERA_FOCUS_DRIVEN_FACE is supported");
        } else if (entry.data.u8[i] == OHOS_CAMERA_FOCUS_DRIVEN_AUTO) {
            CAMERA_LOGI("focus driven type OHOS_CAMERA_FOCUS_DRIVEN_AUTO is supported");
        } else {
            CAMERA_LOGE("focus driven type is not supported");
            return;
        }
    }
    camera_metadata_item_t item;
    bool status = false;
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<uint8_t> setting;
    uint8_t metaFocusDrivenType = static_cast<uint8_t>(OHOS_CAMERA_FOCUS_DRIVEN_AUTO);
    int ret = FindCameraMetadataItem(data, OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &item);
    if (ret == CAM_META_ITEM_NOT_FOUND) {
        status = cameraTest->ability->addEntry(OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &metaFocusDrivenType, DATA_COUNT);
        CAMERA_LOGI("add OHOS_CAMERA_FOCUS_DRIVEN_AUTO to ability");
    } else if (ret == CAM_META_SUCCESS) {
        status = cameraTest->ability->updateEntry(OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &metaFocusDrivenType, DATA_COUNT);
        CAMERA_LOGI("update OHOS_CAMERA_FOCUS_DRIVEN_AUTO to ability");
    }
    meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t autoType = 0;
    meta->addEntry(OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &autoType, DATA_COUNT);
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    CAMERA_LOGI("second: update settings success");
    ret = FindCameraMetadataItem(data, OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &item);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    EXPECT_TRUE(item.data.u8[0] == OHOS_CAMERA_FOCUS_DRIVEN_AUTO);
}

bool CameraHdiTestV1_3::SetFocusDrivenType(common_metadata_header_t* data, camera_focus_driven_type_t type,
    std::shared_ptr<OHOS::Camera::CameraSetting>& meta, std::vector<uint8_t>& setting)
{
    bool result = false;
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_FOCUS_DRIVEN_TYPES, &entry);
    if (cameraTest->rc != CAM_META_SUCCESS || entry.count == 0) {
        CAMERA_LOGE("OHOS_ABILITY_FOCUS_DRIVEN_TYPES is not supported");
        return result;
    }
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == type) {
            result = true;
            break;
        }
    }
    if (result == false) {
        return result;
    }
    camera_metadata_item_t item;
    bool status = false;
    uint8_t metaFocusDrivenType = static_cast<uint8_t>(type);
    int ret = FindCameraMetadataItem(data, OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &item);
    if (ret == CAM_META_ITEM_NOT_FOUND) {
        status = cameraTest->ability->addEntry(OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &metaFocusDrivenType, DATA_COUNT);
    } else if (ret == CAM_META_SUCCESS) {
        status = cameraTest->ability->updateEntry(OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &metaFocusDrivenType, DATA_COUNT);
    }
    meta->addEntry(OHOS_CONTROL_FOCUS_DRIVEN_TYPE, &metaFocusDrivenType, DATA_COUNT);
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    cameraTest->rc == HDI::Camera::V1_0::NO_ERROR ? result = true : result = false;
    return result;
}

void FindFocusTrackingResult(common_metadata_header_t* streamData)
{
    camera_metadata_item_t modeEntry;
    int32_t rc = FindCameraMetadataItem(streamData, OHOS_CONTROL_FOCUS_TRACKING_MODE, &modeEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (modeEntry.data.u8 != nullptr && modeEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult human face result");
        }
    }
    camera_metadata_item_t rectEntry;
    rc = FindCameraMetadataItem(streamData, OHOS_ABILITY_FOCUS_TRACKING_REGION, &rectEntry);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        if (rectEntry.data.i32 != nullptr && rectEntry.count > 0) {
            CAMERA_LOGI("FindDetectResult human body result");
        }
    }
}

/**
 * @tc.name:SUB_MULTIMEDIA_CAMERA_OHOS_FOCUS_TRACKING_0100
 * @tc.desc: test TAG OHOS_ABILITY_FOCUS_TRACKING_REGION and OHOS_CONTROL_FOCUS_TRACKING_MODE
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_MULTIMEDIA_CAMERA_OHOS_FOCUS_TRACKING_0100, TestSize.Level1)
{
    ASSERT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<uint8_t> setting;
    bool result = SetFocusDrivenType(data, OHOS_CAMERA_FOCUS_DRIVEN_FACE, meta, setting);
    if (result == false) {
        CAMERA_LOGE("SetFocusDrivenType failed");
        return;
    }
    cameraTest->intents = {PREVIEW, StreamIntent::VIDEO};
    cameraTest->StartStream(cameraTest->intents, OHOS::HDI::Camera::V1_3::VIDEO);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->streamOperator_V1_3->EnableResult(
        cameraTest->streamIdVideo, setting);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        return;
    }
    sleep(UT_SECOND_TIMES);
    if (cameraTest->streamOperatorCallbackV1_3->streamResultMeta == nullptr) {
        CAMERA_LOGI("onresult not be invoked.");
        return;
    }
    common_metadata_header_t* streamData = cameraTest->streamOperatorCallbackV1_3->streamResultMeta->get();
    FindFocusTrackingResult(streamData);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_COLOR_RESERVATION_TYPE_0100
 * @tc.desc: test TAG OHOS_ABILITY_COLOR_RESERVATION_TYPES and OHOS_CONTROL_COLOR_RESERVATION_TYPE
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_COLOR_RESERVATION_TYPE_0100, TestSize.Level1)
{
    ASSERT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_COLOR_RESERVATION_TYPES, &entry);
    if (cameraTest->rc != CAM_META_SUCCESS || entry.count == 0) {
        CAMERA_LOGE("OHOS_ABILITY_COLOR_RESERVATION_TYPES is not supported");
        return;
    }
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == OHOS_CAMERA_COLOR_RESERVATION_PORTRAIT) {
            CAMERA_LOGI("color reservation type OHOS_CAMERA_COLOR_RESERVATION_PORTRAIT is supported");
        } else if (entry.data.u8[i] == OHOS_CAMERA_COLOR_RESERVATION_NONE) {
            CAMERA_LOGI("color reservation type OHOS_CAMERA_COLOR_RESERVATION_NONE is supported");
        } else {
            CAMERA_LOGE("color reservation type is not supported");
            return;
        }
    }
    camera_metadata_item_t item;
    bool status = false;
    uint8_t metacolorType = static_cast<uint8_t>(OHOS_CAMERA_COLOR_RESERVATION_PORTRAIT);
    int ret = FindCameraMetadataItem(data, OHOS_CONTROL_COLOR_RESERVATION_TYPE, &item);
    if (ret == CAM_META_ITEM_NOT_FOUND) {
        status = cameraTest->ability->addEntry(OHOS_CONTROL_COLOR_RESERVATION_TYPE, &metacolorType, DATA_COUNT);
        CAMERA_LOGI("add OHOS_CAMERA_COLOR_RESERVATION_PORTRAIT to ability");
    } else if (ret == CAM_META_SUCCESS) {
        status = cameraTest->ability->updateEntry(OHOS_CONTROL_COLOR_RESERVATION_TYPE, &metacolorType, DATA_COUNT);
        CAMERA_LOGI("update OHOS_CAMERA_COLOR_RESERVATION_PORTRAIT to ability");
    }
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<uint8_t> setting;
    uint8_t portraitType = 1;
    meta->addEntry(OHOS_CONTROL_COLOR_RESERVATION_TYPE, &portraitType, DATA_COUNT);
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    CAMERA_LOGI("first: update settings success");
    ret = FindCameraMetadataItem(data, OHOS_CONTROL_COLOR_RESERVATION_TYPE, &item);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    EXPECT_TRUE(item.data.u8[0] == OHOS_CAMERA_COLOR_RESERVATION_PORTRAIT);
}

/**
 * @tc.name:SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_COLOR_RESERVATION_TYPE_0200
 * @tc.desc: test TAG OHOS_ABILITY_COLOR_RESERVATION_TYPES and OHOS_CONTROL_COLOR_RESERVATION_TYPE
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_3, SUB_MULTIMEDIA_CAMERA_OHOS_CONTROL_COLOR_RESERVATION_TYPE_0200, TestSize.Level1)
{
    ASSERT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    ASSERT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_COLOR_RESERVATION_TYPES, &entry);
    if (cameraTest->rc != CAM_META_SUCCESS || entry.count == 0) {
        CAMERA_LOGE("OHOS_ABILITY_COLOR_RESERVATION_TYPES is not supported");
        return;
    }
    for (int i = 0; i < entry.count; i++) {
        if (entry.data.u8[i] == OHOS_CAMERA_COLOR_RESERVATION_PORTRAIT) {
            CAMERA_LOGI("color reservation type OHOS_CAMERA_COLOR_RESERVATION_PORTRAIT is supported");
        } else if (entry.data.u8[i] == OHOS_CAMERA_COLOR_RESERVATION_NONE) {
            CAMERA_LOGI("color reservation type OHOS_CAMERA_COLOR_RESERVATION_NONE is supported");
        } else {
            CAMERA_LOGE("color reservation type is not supported");
            return;
        }
    }
    camera_metadata_item_t item;
    bool status = false;
    uint8_t metacolorType = static_cast<uint8_t>(OHOS_CAMERA_COLOR_RESERVATION_NONE);
    int ret = FindCameraMetadataItem(data, OHOS_CONTROL_COLOR_RESERVATION_TYPE, &item);
    if (ret == CAM_META_ITEM_NOT_FOUND) {
        status = cameraTest->ability->addEntry(OHOS_CONTROL_COLOR_RESERVATION_TYPE, &metacolorType, DATA_COUNT);
        CAMERA_LOGI("add OHOS_CAMERA_COLOR_RESERVATION_NONE to ability");
    } else if (ret == CAM_META_SUCCESS) {
        status = cameraTest->ability->updateEntry(OHOS_CONTROL_COLOR_RESERVATION_TYPE, &metacolorType, DATA_COUNT);
        CAMERA_LOGI("update OHOS_CAMERA_COLOR_RESERVATION_NONE to ability");
    }
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<uint8_t> setting;
    uint8_t noneType = 0;
    meta->addEntry(OHOS_CONTROL_COLOR_RESERVATION_TYPE, &noneType, DATA_COUNT);
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (OHOS::HDI::Camera::V1_0::CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    CAMERA_LOGI("second: update settings success");
    ret = FindCameraMetadataItem(data, OHOS_CONTROL_COLOR_RESERVATION_TYPE, &item);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    EXPECT_TRUE(item.data.u8[0] == OHOS_CAMERA_COLOR_RESERVATION_NONE);
}
