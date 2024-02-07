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
#include "camera_stream_test_v1_2.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

constexpr uint32_t ITEM_CAPACITY = 100;
constexpr uint32_t DATA_CAPACITY = 2000;
constexpr uint32_t DATA_COUNT = 1;
void CameraStreamTestV1_2::SetUpTestCase(void) {}
void CameraStreamTestV1_2::TearDownTestCase(void) {}
void CameraStreamTestV1_2::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init(); // assert inside
    cameraTest->OpenCameraV1_2(DEVICE_0); // assert inside
}

void CameraStreamTestV1_2::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.name:SUB_Driver_Camera_Colorspace_0300
 * @tc.desc: updateColorSpace by updateStreams
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraStreamTestV1_2, SUB_Driver_Camera_Colorspace_0300, TestSize.Level1)
{
    cameraTest->streamOperatorCallbackV1_2 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_2();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_2(cameraTest->streamOperatorCallbackV1_2,
        cameraTest->streamOperator_V1_2);
    EXPECT_NE(cameraTest->streamOperator_V1_2, nullptr);
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfoV1_1->v1_0.dataspace_ = OHOS_CAMERA_SRGB_FULL;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfoCapture->v1_0.dataspace_ = OHOS_CAMERA_SRGB_FULL;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CommitStreams(
        OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->streamOperator_V1_2->CancelCapture(cameraTest->captureIdPreview);
    cameraTest->streamInfoV1_1->v1_0.dataspace_ = OHOS_CAMERA_P3_FULL;
    cameraTest->streamInfoCapture->v1_0.dataspace_ = OHOS_CAMERA_P3_FULL;
    cameraTest->rc = cameraTest->streamOperator_V1_2->UpdateStreams(cameraTest->streamInfosV1_1);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_HDRvivid_0100
 * @tc.desc: updateColorSpace by updateStreams
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraStreamTestV1_2, SUB_Driver_Camera_HDRvivid_0100, TestSize.Level1)
{
    cameraTest->streamOperatorCallbackV1_2 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_2();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_2(cameraTest->streamOperatorCallbackV1_2,
        cameraTest->streamOperator_V1_2);
    EXPECT_NE(cameraTest->streamOperator_V1_2, nullptr);

    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // video streamInfo
    cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CommitStreams(
        OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    cameraTest->streamOperator_V1_2->CancelCapture(cameraTest->captureIdPreview);
    cameraTest->streamOperator_V1_2->CancelCapture(cameraTest->captureIdVideo);
    cameraTest->streamInfoV1_1->v1_0.dataspace_ = OHOS_CAMERA_BT2020_HLG_FULL;
    cameraTest->streamInfoV1_1->v1_0.format_ = OHOS_CAMERA_FORMAT_YCBCR_P010;
    cameraTest->streamInfoVideo->v1_0.dataspace_ = OHOS_CAMERA_BT2020_HLG_FULL;
    cameraTest->streamInfoVideo->v1_0.format_ = OHOS_CAMERA_FORMAT_YCBCR_P010;
    cameraTest->rc = cameraTest->streamOperator_V1_2->UpdateStreams(cameraTest->streamInfosV1_1);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_HDRvivid_0200
 * @tc.desc: updateColorSpace by updateStreams
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraStreamTestV1_2, SUB_Driver_Camera_HDRvivid_0200, TestSize.Level1)
{
    cameraTest->streamOperatorCallbackV1_2 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_2();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_2(cameraTest->streamOperatorCallbackV1_2,
        cameraTest->streamOperator_V1_2);
    EXPECT_NE(cameraTest->streamOperator_V1_2, nullptr);

    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // video streamInfo
    cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CommitStreams(
        OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    cameraTest->streamOperator_V1_2->CancelCapture(cameraTest->captureIdPreview);
    cameraTest->streamOperator_V1_2->CancelCapture(cameraTest->captureIdVideo);
    cameraTest->streamInfoV1_1->v1_0.dataspace_ = OHOS_CAMERA_BT709_FULL;
    cameraTest->streamInfoV1_1->v1_0.format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfoVideo->v1_0.dataspace_ = OHOS_CAMERA_BT709_FULL;
    cameraTest->streamInfoVideo->v1_0.format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->rc = cameraTest->streamOperator_V1_2->UpdateStreams(cameraTest->streamInfosV1_1);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_NightMode_0200
 * @tc.desc:auto exposure
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraStreamTestV1_2, SUB_Driver_Camera_NightMode_0200, TestSize.Level1)
{
    //Get Stream Operator
    cameraTest->streamOperatorCallbackV1_2 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_2();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_2(cameraTest->streamOperatorCallbackV1_2,
        cameraTest->streamOperator_V1_2);
    EXPECT_NE(cameraTest->streamOperator_V1_2, nullptr);
    //preview streamInfo
    cameraTest->streamInfoPre = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoPre);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoPre);
    //capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);
    //create and commit stream
    cameraTest->rc = cameraTest->streamOperator_V1_2->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::NIGHT),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    //start capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    //wait for auto exposuring end
    sleep(10);
    //stop stream
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_NightMode_0300
 * @tc.desc:manual exposure
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraStreamTestV1_2, SUB_Driver_Camera_NightMode_0300, TestSize.Level1)
{
    //update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    int32_t manualExposureTime = 8000;
    meta->addEntry(OHOS_CONTROL_MANUAL_EXPOSURE_TIME, &manualExposureTime, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    //Get Stream Operator
    cameraTest->streamOperatorCallbackV1_2 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_2();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_2(cameraTest->streamOperatorCallbackV1_2,
        cameraTest->streamOperator_V1_2);
    EXPECT_NE(cameraTest->streamOperator_V1_2, nullptr);
    //preview streamInfo
    cameraTest->streamInfoPre = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoPre);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoPre);
    //capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);
    //create and commit stream
    cameraTest->rc = cameraTest->streamOperator_V1_2->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::NIGHT),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    //start capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    //wait for manual exposuring end
    sleep(10);
    //stop stream
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_NightMode_0400
 * @tc.desc:manual exposure and then confirmCapture
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraStreamTestV1_2, SUB_Driver_Camera_NightMode_0400, TestSize.Level1)
{
    //update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    int32_t manualExposureTime = 8000;
    meta->addEntry(OHOS_CONTROL_MANUAL_EXPOSURE_TIME, &manualExposureTime, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    //Get Stream Operator
    cameraTest->streamOperatorCallbackV1_2 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_2();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_2(cameraTest->streamOperatorCallbackV1_2,
        cameraTest->streamOperator_V1_2);
    EXPECT_NE(cameraTest->streamOperator_V1_2, nullptr);
    //preview streamInfo
    cameraTest->streamInfoPre = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoPre);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoPre);
    //capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);
    //create and commit stream
    cameraTest->rc = cameraTest->streamOperator_V1_2->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::NIGHT),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    //start capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    //wait for manual exposuring end confirmCapture
    sleep(UT_SLEEP_TIME);
    EXPECT_NE(cameraTest->streamOperator_V1_2, nullptr);
    cameraTest->streamOperator_V1_2->ConfirmCapture(cameraTest->streamIdCapture);
    sleep(UT_SLEEP_TIME);
    //stop stream
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_SmoothZoom_0200
 * @tc.desc: SMOOTH ZOOM
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraStreamTestV1_2, SUB_Driver_Camera_SmoothZoom_0200, TestSize.Level1)
{
    cameraTest->streamOperatorCallbackV1_2 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_2();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_2(cameraTest->streamOperatorCallbackV1_2,
        cameraTest->streamOperator_V1_2);
    EXPECT_NE(cameraTest->streamOperator_V1_2, nullptr);
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfoV1_1->v1_0.dataspace_ = OHOS_CAMERA_SRGB_FULL;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfoCapture->v1_0.dataspace_ = OHOS_CAMERA_SRGB_FULL;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);
    // video streamInfo
    cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
    // start stream
    cameraTest->rc = cameraTest->streamOperator_V1_2->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CommitStreams(
        OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    // cover tag OHOS_ABILITY_CAMERA_ZOOM_PERFORMANCE
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_ZOOM_PERFORMANCE, &entry);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // cover OHOS_CONTROL_PREPARE_ZOOM and its values
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    // cover OHOS_CAMERA_ZOOMSMOOTH_PREPARE_ENABLE
    uint8_t pre_zoom_value = OHOS_CAMERA_ZOOMSMOOTH_PREPARE_ENABLE;
    meta->addEntry(OHOS_CONTROL_PREPARE_ZOOM, &pre_zoom_value, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // cover OHOS_CONTROL_SMOOTH_ZOOM_RATIOS,  values type: uint32_t array
    uint32_t values[] = { 10, 300, 20, 400, 30, 500 };
    meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    meta->addEntry(OHOS_CONTROL_SMOOTH_ZOOM_RATIOS, values, sizeof(values) / sizeof(uint32_t));
    setting.clear();
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // cover GetStatus(), OHOS_STATUS_CAMERA_CURRENT_FPS and  OHOS_STATUS_CAMERA_CURRENT_ZOOM_RATIO
    std::shared_ptr<CameraSetting> metaIn = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    // no sense
    uint32_t current_fps = 0;
    uint32_t current_zoom_ratio = 0;
    metaIn->addEntry(OHOS_STATUS_CAMERA_CURRENT_FPS, &current_fps, DATA_COUNT);
    metaIn->addEntry(OHOS_STATUS_CAMERA_CURRENT_ZOOM_RATIO, &current_zoom_ratio, DATA_COUNT);
    std::vector<uint8_t> settingIn, settingOut;
    MetadataUtils::ConvertMetadataToVec(metaIn, settingIn);
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStatus(settingIn, settingOut);
    if (HDI::Camera::V1_0::NO_ERROR == cameraTest->rc) {
        std::cout << "GetStatus execute success." << std::endl;
    }
    meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    // cover OHOS_CAMERA_ZOOMSMOOTH_PREPARE_DISABLE
    pre_zoom_value = OHOS_CAMERA_ZOOMSMOOTH_PREPARE_DISABLE ;
    meta->addEntry(OHOS_CONTROL_PREPARE_ZOOM, &pre_zoom_value, DATA_COUNT);
    setting.clear();
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdCapture, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_SmoothZoom_0300
 * @tc.desc: SMOOTH ZOOM
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraStreamTestV1_2, SUB_Driver_Camera_SmoothZoom_0300, TestSize.Level1)
{
    cameraTest->streamOperatorCallbackV1_2 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_2();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_2(cameraTest->streamOperatorCallbackV1_2,
        cameraTest->streamOperator_V1_2);
    EXPECT_NE(cameraTest->streamOperator_V1_2, nullptr);
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfoV1_1->v1_0.dataspace_ = OHOS_CAMERA_SRGB_FULL;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfoCapture->v1_0.dataspace_ = OHOS_CAMERA_SRGB_FULL;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);
    // video streamInfo
    cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
    // start stream
    cameraTest->rc = cameraTest->streamOperator_V1_2->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_2->CommitStreams(
        OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    std::shared_ptr<CameraSetting> metaIn = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    std::shared_ptr<CameraSetting> metaOut = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t aperture_range = 1;
    metaIn->addEntry(OHOS_ABILITY_CAMERA_VIRTUAL_APERTURE_RANGE, &aperture_range, DATA_COUNT);
    std::vector<uint8_t> settingIn, settingOut;
    MetadataUtils::ConvertMetadataToVec(metaIn, settingIn);
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStatus(settingIn, settingOut);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdCapture, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}