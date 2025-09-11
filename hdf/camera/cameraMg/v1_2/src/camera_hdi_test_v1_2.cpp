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
#include "camera_hdi_test_v1_2.h"
#include <functional>

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

constexpr uint32_t ITEM_CAPACITY = 100;
constexpr uint32_t DATA_CAPACITY = 2000;
constexpr uint32_t DATA_COUNT = 1;
std::vector<std::string> pendingImagesId = {"-1", "-2"};
void CameraHdiTestV1_2::SetUpTestCase(void) {}
void CameraHdiTestV1_2::TearDownTestCase(void) {}
void CameraHdiTestV1_2::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init(); // assert inside
    cameraTest->Open(DEVICE_0); // assert inside
}

void CameraHdiTestV1_2::TearDown(void)
{
    cameraTest->Close();
}

bool CameraHdiTestV1_2::TakePhoteWithDefferredImage(int PhotoCount)
{
    auto meta = std::make_shared<CameraSetting>(100, 100);
    uint8_t value = OHOS::HDI::Camera::V1_2::STILL_IMAGE;
    meta->addEntry(OHOS_CONTROL_DEFERRED_IMAGE_DELIVERY, &value, sizeof(value));
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        // take photo
        cameraTest->imageDataSaveSwitch = SWITCH_ON;
        cameraTest->intents = {PREVIEW, STILL_CAPTURE};
        cameraTest->StartStream(cameraTest->intents);
        if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
            cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
            for (int i = 0; i < PhotoCount; i++) {
                cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
            }
            cameraTest->captureIds = {cameraTest->captureIdPreview};
            cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
            cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
        } else {
            printf("DefferredImage StartStream failed.\r\n");
            return false;
        }
    } else {
        printf("DefferredImage UpdateSettings failed.\r\n");
        return false;
    }
    return true;
}
   
bool CameraHdiTestV1_2::RemovePendingImages()
{
    std::vector<std::string> pendingImages;
    int ret = cameraTest->imageProcessSession_->GetPendingImages(pendingImages);
    if (ret == 0 && pendingImages.size() != 0) {
        for (auto imageId = pendingImages.begin(); imageId != pendingImages.end(); ++imageId) {
            ret = cameraTest->imageProcessSession_->RemoveImage(*imageId);
            if (ret != 0) {
                printf("RemoveImage failed.\r\n");
                return false;
            }
        }
    }
    return true;
}

/**
 * @tc.name:SUB_Driver_Camera_DefferredImage_0200
 * @tc.desc:ProcessPendingImages
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_DefferredImage_0200, TestSize.Level1)
{
    int ret = 0;
    bool isImageProcessServiceExist = true;
    bool isExit = cameraTest->IsTagValueExistsU8(cameraTest->ability,\
        OHOS_ABILITY_DEFERRED_IMAGE_DELIVERY,\
        OHOS::HDI::Camera::V1_2::STILL_IMAGE);
    if (!isExit) {
        GTEST_SKIP() << "skip this test, because OHOS_ABILITY_DEFERRED_IMAGE_DELIVERY not found" << std::endl;
        return;
    }
    // get DefferredImageTestInit
    ret = cameraTest->DefferredImageTestInit();
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        CAMERA_LOGE("DefferredImageTestInit Fail!!!");
        printf("DefferredImageTestInit Fail!!!\r\n");
        isImageProcessServiceExist = false;
    }
    // 如果存在未处理的图片，则删除未处理的图片
    bool pImage = RemovePendingImages();
    if (!pImage) {
        GTEST_SKIP() << "RemovePendingImages failed." << std::endl;
    }
    // take photo using deferred image delivery
    bool flag = TakePhoteWithDefferredImage(1);
    if (!flag) {
        GTEST_SKIP() << "TakePhoteWithDefferredImage failed." << std::endl;
    }
    // image deferred delivery process
    ASSERT_EQ(isImageProcessServiceExist, true);
    int taskCount = 0;
    ret = cameraTest->imageProcessSession_->GetCoucurrency(OHOS::HDI::Camera::V1_2::HIGH_PREFORMANCE, taskCount);
    EXPECT_EQ(ret, 0);
    std::vector<std::string> pendingImages;
    ret = cameraTest->imageProcessSession_->GetPendingImages(pendingImages);
    EXPECT_EQ(ret, 0);
    EXPECT_GE(taskCount, 1);
}

void CameraHdiTestV1_2::ProcessPendingImages(int ret)
{
    // image deferred delivery process
    int taskCount = 0;
    std::vector<std::string> pendingImages;
    ret = cameraTest->imageProcessSession_->GetCoucurrency(OHOS::HDI::Camera::V1_2::BALANCED, taskCount);
    EXPECT_EQ(ret, 0);
    ret = cameraTest->imageProcessSession_->GetPendingImages(pendingImages);
    EXPECT_EQ(ret, 0);
    ret = cameraTest->imageProcessSession_->SetExecutionMode(OHOS::HDI::Camera::V1_2::BALANCED);
    EXPECT_EQ(ret, 0);
    ret = cameraTest->imageProcessSession_->SetExecutionMode(OHOS::HDI::Camera::V1_2::LOW_POWER);
    EXPECT_EQ(ret, 0);
    ret = cameraTest->imageProcessSession_->SetExecutionMode(OHOS::HDI::Camera::V1_2::HIGH_PREFORMANCE);
    EXPECT_EQ(ret, 0);
    // process the first image
    ret = cameraTest->imageProcessSession_->ProcessImage(pendingImagesId[0]);
    EXPECT_EQ(ret, 0);
    // process the second image
    ret = cameraTest->imageProcessSession_->ProcessImage(pendingImagesId[1]);
    EXPECT_EQ(ret, 0);
    // process the third image, and test the Interrupt, Reset, RemoveImage Interfaces
    ret = cameraTest->imageProcessSession_->Interrupt();
    EXPECT_EQ(ret, 0);
    ret = cameraTest->imageProcessSession_->Reset();
    EXPECT_EQ(ret, 0);
    ret = cameraTest->imageProcessSession_->RemoveImage(pendingImagesId[0]);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name:SUB_Driver_Camera_DefferredImage_0300
 * @tc.desc:ProcessPendingImages
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_DefferredImage_0300, TestSize.Level1)
{
    int ret = 0;
    bool isImageProcessServiceExist = true;
    bool isExit = cameraTest->IsTagValueExistsU8(cameraTest->ability,\
        OHOS_ABILITY_DEFERRED_IMAGE_DELIVERY, OHOS::HDI::Camera::V1_2::STILL_IMAGE);
    if (!isExit) {
        GTEST_SKIP() << "skip this test, because OHOS_ABILITY_DEFERRED_IMAGE_DELIVERY not found" << std::endl;
        return;
    }
    // get DefferredImageTestInit
    ret = cameraTest->DefferredImageTestInit();
    EXPECT_EQ(ret, 0);
    if (ret != 0) {
        CAMERA_LOGE("DefferredImageTestInit Fail");
        printf("DefferredImageTestInit Fail\r\n");
        isImageProcessServiceExist = false;
    }
    // 如果存在未处理的图片，则删除未处理的图片
    bool pImage = RemovePendingImages();
    if (!pImage) {
        GTEST_SKIP() << "RemovePendingImages failed." << std::endl;
    }
    // take three photo using deferred image delivery, three times
    bool flag = TakePhoteWithDefferredImage(3);
    if (!flag) {
        GTEST_SKIP() << "TakePhoteThreeWithDefferredImage failed." << std::endl;
    }
    ASSERT_EQ(isImageProcessServiceExist, true);
    // 进行二段式处理拍照图片
    ProcessPendingImages(ret);
}

/**
 * @tc.name: SUB_Driver_Camera_ScanMode_0100
 * @tc.desc: CommitStreams_V1_1 for Scan code, preview and video
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_ScanMode_0100, TestSize.Level1)
{
    // Get Stream Operator
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_1(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // video streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // is streams supported V1_1
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    StreamSupportType pType;
    cameraTest->rc = cameraTest->streamOperator_V1_1->IsStreamsSupported_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::SCAN_CODE),
        modeSettingVec, cameraTest->streamInfosV1_1, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    // create and commitstreams
    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::SCAN_CODE),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);

    // start capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    sleep(UT_SECOND_TIMES);

    // stop stream
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_XMAGE_0200
 * @tc.desc: UpdateSettings:CAMERA_CUSTOM_COLOR_BRIGHT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_XMAGE_0200, TestSize.Level1)
{
    // Start Xmage control setting and verify
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t xmageMode = CAMERA_CUSTOM_COLOR_BRIGHT;
    meta->addEntry(OHOS_CONTROL_SUPPORTED_COLOR_MODES, &xmageMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->cameraDevice->UpdateSettings(metaVec);
    cameraTest->intents = {PREVIEW, STILL_CAPTURE};
    cameraTest->StartStream(cameraTest->intents);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    sleep(1);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_XMAGE_0300
 * @tc.desc: UpdateSettings:CAMERA_CUSTOM_COLOR_NORMAL
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_XMAGE_0300, TestSize.Level1)
{
    // Start Xmage control setting and verify
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(100, 200);
    uint8_t xmageMode = 100;
    meta->addEntry(OHOS_CONTROL_SUPPORTED_COLOR_MODES, &xmageMode, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    cameraTest->cameraDevice->UpdateSettings(metaVec);
    cameraTest->intents = {PREVIEW, STILL_CAPTURE};
    cameraTest->StartStream(cameraTest->intents);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

#define TODEFINESTRING(x) #x

static std::string TranslateXMageAbilityToString(camera_xmage_color_type mode)
{
    std::string res;
    switch (mode) {
        case CAMERA_CUSTOM_COLOR_NORMAL:
        {
            res = TODEFINESTRING(CAMERA_CUSTOM_COLOR_NORMAL);
            break;
        }
        case CAMERA_CUSTOM_COLOR_BRIGHT:
        {
            res = TODEFINESTRING(CAMERA_CUSTOM_COLOR_BRIGHT);
            break;
        }
        case CAMERA_CUSTOM_COLOR_SOFT:
        {
            res = TODEFINESTRING(CAMERA_CUSTOM_COLOR_SOFT);
            break;
        }
        default:
            break;
    }
    return res;
}

/**
 * @tc.name: SUB_Driver_Camera_XMAGE_0400
 * @tc.desc: OHOS_CONTROL_SUPPORTED_COLOR_MODES,all value
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_XMAGE_0400, TestSize.Level1)
{
    // Start Xmage control setting and verify
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_SUPPORTED_COLOR_MODES, &entry);
    std::vector<uint8_t> xmageAbilities;
    // 查询支持的Xmage所有模式
    if (ret == 0) {
        EXPECT_TRUE(entry.data.u8 != nullptr);
        EXPECT_NE(entry.count, 0);

        for (uint32_t i = 0; i < entry.count; ++i) {
            // 打印并保存当前相机所支持的xmage能力
            CAMERA_LOGI("Current camera xmage ability %{public}s supported!",
                TranslateXMageAbilityToString(static_cast<camera_xmage_color_type>(entry.data.u8[i])).c_str());

            xmageAbilities.push_back(entry.data.u8[i]);
        }
    } else {
        GTEST_SKIP() << "XMage not supported" << std::endl;
    }
    CAMERA_LOGI("%{public}lu xmage abilities supported",
                          static_cast<unsigned long>(xmageAbilities.size()));
    for (uint32_t i = 0; i < xmageAbilities.size(); ++i) {
        std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(100, 200);
        // UpdateSettings mode
        uint8_t xmageMode = xmageAbilities[i];
        meta->addEntry(OHOS_CONTROL_SUPPORTED_COLOR_MODES, &xmageMode, 1);
        std::vector<uint8_t> metaVec;
        MetadataUtils::ConvertMetadataToVec(meta, metaVec);
        cameraTest->cameraDevice->UpdateSettings(metaVec);
        CAMERA_LOGI("Now current camera xmage ability is %{public}s !",
                TranslateXMageAbilityToString(static_cast<camera_xmage_color_type>(xmageMode)).c_str());
        cameraTest->intents = {PREVIEW, STILL_CAPTURE, VIDEO};
        cameraTest->StartStream(cameraTest->intents);
        cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
        cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, true);
        cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
        cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdCapture,
                                                                        cameraTest->captureIdVideo};
        cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture, cameraTest->streamIdVideo};
        cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
    }
}

void CameraHdiTestV1_2::TakePhotoWithTags(std::shared_ptr<OHOS::Camera::CameraSetting> metaDate)
{
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(metaDate, metaVec);
    cameraTest->intents = {PREVIEW, STILL_CAPTURE};
    cameraTest->StartStream(cameraTest->intents);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    sleep(1);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_Stabilization_0200
 * @tc.desc:OHOS_CAMERA_VIDEO_STABILIZATION_OFF
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_Stabilization_0200, TestSize.Level1)
{
    //start stream
    cameraTest->intents = {PREVIEW, VIDEO};
    cameraTest->StartStream(cameraTest->intents);
    //updateSettings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t videoStabiliMode = OHOS_CAMERA_VIDEO_STABILIZATION_OFF;
    meta->addEntry(OHOS_CONTROL_VIDEO_STABILIZATION_MODE, &videoStabiliMode, DATA_COUNT);
    const int32_t deviceStreamId = cameraTest->streamIdPreview;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &deviceStreamId, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    //get preview capture and video
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    //release stream
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name:SUB_Driver_Camera_Stabilization_0300
 * @tc.desc:OHOS_CAMERA_VIDEO_STABILIZATION_AUTO
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_Stabilization_0300, TestSize.Level1)
{
    //start stream
    cameraTest->intents = {PREVIEW, VIDEO};
    cameraTest->StartStream(cameraTest->intents);

    //updateSettings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t videoStabiliMode = OHOS_CAMERA_VIDEO_STABILIZATION_AUTO;
    meta->addEntry(OHOS_CONTROL_VIDEO_STABILIZATION_MODE, &videoStabiliMode, DATA_COUNT);
    const int32_t deviceStreamId = cameraTest->streamIdPreview;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &deviceStreamId, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    //get preview capture and video
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);

    //release stream
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

void CaptureByColorSpaces(std::vector<int32_t> captureColorSpaces, std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
    if (!captureColorSpaces.empty()) {
        for (int32_t colorSpaces : captureColorSpaces) {
            printf("capture colorSpaces value %d\n", colorSpaces);
            // preview streamInfo
            cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
            cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
            if (colorSpaces == OHOS_CAMERA_BT2020_HLG_FULL) {
                cameraTest->streamInfoV1_1->v1_0.format_ = OHOS::GRAPHIC_PIXEL_FMT_YCBCR_P010; // 10bit
            }
            cameraTest->streamInfoV1_1->v1_0.dataspace_ = colorSpaces;
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
            // capture streamInfo
            cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
            cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
            cameraTest->streamInfoCapture->v1_0.dataspace_ = colorSpaces;
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);
            cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
            cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(
                OperationMode::NORMAL, cameraTest->abilityVec);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
            sleep(UT_SECOND_TIMES);
            cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
            cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
            cameraTest->captureIds = {cameraTest->captureIdPreview};
            cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
            cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
            cameraTest->streamInfosV1_1.clear();
        }
    }
}

void VideoByColorSpaces(std::vector<int32_t> videoColorSpaces, std::vector<int32_t> previewColorSpaces,
    std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
    cameraTest->imageDataSaveSwitch = SWITCH_ON;

    if (videoColorSpaces.empty() || previewColorSpaces.empty()) {
        printf("ColorSpaces empty is null\n");
        return;
    }
    for (int32_t colorSpaces : previewColorSpaces) {
        printf("preview colorSpaces value %d\n", colorSpaces);
        
        // preview streamInfo
        cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
        cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
        cameraTest->streamInfoV1_1->v1_0.dataspace_ = colorSpaces;
        if (colorSpaces == CM_BT2020_HLG_LIMIT) {
            cameraTest->streamInfoV1_1->v1_0.format_ = OHOS::GRAPHIC_PIXEL_FMT_YCBCR_P010; // 10bit
        }

        // video streamInfo
        for (int32_t colorSpaces_ : videoColorSpaces) {
            printf("video colorSpaces_ value %d\n", colorSpaces_);
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

            cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
            cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
            cameraTest->streamInfoVideo->v1_0.format_ = cameraTest->streamInfoV1_1->v1_0.format_; // equal to preview
            cameraTest->streamInfoVideo->v1_0.dataspace_ = colorSpaces_;
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
            cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
            cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(
                static_cast<OHOS::HDI::Camera::V1_0::OperationMode>(OHOS::HDI::Camera::V1_1::OperationMode_V1_1::VIDEO),
                cameraTest->abilityVec);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
                    
            //updateSettings
            std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
            uint8_t videoStabiliMode = OHOS_CAMERA_VIDEO_STABILIZATION_AUTO;
            meta->addEntry(OHOS_CONTROL_VIDEO_STABILIZATION_MODE, &videoStabiliMode, DATA_COUNT);
            const int32_t deviceStreamId = cameraTest->streamIdPreview;
            meta->addEntry(OHOS_CAMERA_STREAM_ID, &deviceStreamId, DATA_COUNT);
            std::vector<uint8_t> setting;
            MetadataUtils::ConvertMetadataToVec(meta, setting);
            cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
            sleep(UT_SECOND_TIMES);

            cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
            cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
            cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
            cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
            cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
            cameraTest->streamInfosV1_1.clear();
            sleep(1);
        }
    }
    cameraTest->imageDataSaveSwitch = SWITCH_OFF;
}

void SuperStubByColorSpaces(std::vector<int32_t> superStubColorSpaces, std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
    if (!superStubColorSpaces.empty()) {
        for (int32_t colorSpaces : superStubColorSpaces) {
            printf("superStubColorSpaces colorSpaces value %d\n", colorSpaces);
            // preview streamInfo
            cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
            cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
            cameraTest->streamInfoV1_1->v1_0.dataspace_ = colorSpaces;
            if (colorSpaces == CM_BT2020_HLG_LIMIT) {
                cameraTest->streamInfoV1_1->v1_0.format_ = OHOS::GRAPHIC_PIXEL_FMT_YCBCR_P010; // 10bit
            }
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
            // video streamInfo
            cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
            cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
            cameraTest->streamInfoVideo->v1_0.dataspace_ = colorSpaces;
            cameraTest->streamInfoVideo->v1_0.format_ = cameraTest->streamInfoV1_1->v1_0.format_; // equal to preview
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
            cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
            cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams_V1_1(
                static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::SUPER_STAB),
                cameraTest->abilityVec);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
            sleep(UT_SECOND_TIMES);
            cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
            cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
            cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
            cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
            cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
            cameraTest->streamInfosV1_1.clear();
        }
    }
}

/**
 * @tc.name:SUB_Driver_Camera_Colorspace_0200
 * @tc.desc: Update macro ability setting and check the callback
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_Colorspace_0200, TestSize.Level1)
{
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_1(
        cameraTest->streamOperatorCallback, cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_AVAILABLE_COLOR_SPACES, &entry);
    printf("OHOS_ABILITY_AVAILABLE_COLOR_SPACES value count %d\n", entry.count);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.i32 != nullptr && entry.count > 0) {
        std::vector<int32_t> captureColorSpaces;
        std::vector<int32_t> videoColorSpaces;
        std::vector<int32_t> superStubColorSpaces;
        std::vector<int32_t> previewColorSpaces;
        int32_t operatorMode = -2;
        for (size_t i = 0; i < entry.count - 1; i++) {
            if (operatorMode == -2 && entry.data.i32[i] == HDI::Camera::V1_2::OperationMode_V1_2::CAPTURE) {
                operatorMode = HDI::Camera::V1_2::OperationMode_V1_2::CAPTURE;
            } else if (operatorMode == -2 && entry.data.i32[i] == HDI::Camera::V1_2::OperationMode_V1_2::VIDEO) {
                operatorMode = HDI::Camera::V1_2::OperationMode_V1_2::VIDEO;
            } else if (operatorMode == -2 && entry.data.i32[i] == HDI::Camera::V1_2::OperationMode_V1_2::SUPER_STAB) {
                operatorMode = HDI::Camera::V1_2::OperationMode_V1_2::SUPER_STAB;
            } else if (entry.data.i32[i] == -1 && operatorMode != -2 && entry.data.i32[i + 1] == -1) {
                operatorMode = -2;
            } else if (entry.data.i32[i] == -1 && operatorMode != -2 && entry.data.i32[i + 1] != -1) {
                operatorMode = -1;
            } else if (operatorMode == HDI::Camera::V1_2::OperationMode_V1_2::CAPTURE) {
                captureColorSpaces.push_back(entry.data.i32[i]);
            } else if (operatorMode == HDI::Camera::V1_2::OperationMode_V1_2::VIDEO) {
                if (std::find(cameraTest->previewColorSpaces_.begin(), cameraTest->previewColorSpaces_.end(),
                    entry.data.i32[i]) != cameraTest->previewColorSpaces_.end()) {
                    previewColorSpaces.push_back(entry.data.i32[i]);
                } else {
                    videoColorSpaces.push_back(entry.data.i32[i]);
                }
            } else if (operatorMode == HDI::Camera::V1_2::OperationMode_V1_2::SUPER_STAB) {
                superStubColorSpaces.push_back(entry.data.i32[i]);
            } else if (operatorMode == -2 && entry.data.i32[i] > 0) {
                operatorMode = -1;
            }
        }
        CaptureByColorSpaces(captureColorSpaces, cameraTest);
        VideoByColorSpaces(videoColorSpaces, previewColorSpaces, cameraTest);
        SuperStubByColorSpaces(superStubColorSpaces, cameraTest);
    } else {
        GTEST_SKIP() << "OHOS_ABILITY_AVAILABLE_COLOR_SPACES NOT FOUND" << std::endl;
    }
}

/**
 * @tc.name: SUB_Driver_Camera_APERTURE_0500
 * @tc.desc: OHOS_CONTROL_CAMERA_PHYSICAL_APERTURE_RANGE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_APERTURE_0500, TestSize.Level1)
{
    // Get Stream Operator
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_1(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // capture streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // create and commitstreams
    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::PORTRAIT),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    //update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    float physicalApertureValue = 2;
    meta->addEntry(OHOS_CONTROL_CAMERA_PHYSICAL_APERTURE_VALUE, &physicalApertureValue, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // start capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, true);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdCapture};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_APERTURE_0600
 * @tc.desc: OHOS_CONTROL_CAMERA_PHYSICAL_APERTURE_RANGE, all value
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_APERTURE_0600, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_PHYSICAL_APERTURE_RANGE, &entry);
    if (cameraTest->rc != 0) {
        GTEST_SKIP() << "skip test, because OHOS_ABILITY_CAMERA_PHYSICAL_APERTURE_RANGE not supported now" << std::endl;
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR && entry.data.f != nullptr && entry.count > 0) {
        float entryValues[] = { entry.data.f[3], entry.data.f[7], entry.data.f[8], entry.data.f[9], entry.data.f[10],
            entry.data.f[14], entry.data.f[18] };
        for (size_t i = 0; i < sizeof(entryValues) / sizeof(float); i++) {
            // Get Stream Operator
            cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
            cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_1(cameraTest->streamOperatorCallback,
                cameraTest->streamOperator_V1_1);
            EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

            // preview streamInfo
            cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
            cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

            // capture streamInfo
            cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
            cameraTest->DefaultInfosCapture(cameraTest->streamInfoV1_1);
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

            // create and commitstreams
            cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
            cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams_V1_1(
                static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::PORTRAIT),
                cameraTest->abilityVec);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

            //update settings
            std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
            float physicalApertureValue = entryValues[i];
            meta->addEntry(OHOS_CONTROL_CAMERA_PHYSICAL_APERTURE_VALUE, &physicalApertureValue, DATA_COUNT);
            std::vector<uint8_t> setting;
            MetadataUtils::ConvertMetadataToVec(meta, setting);
            cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
            EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

            // start capture
            cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
            cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, true);

            cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdCapture};
            cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture};
            cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
            sleep(1);
            cameraTest->streamInfosV1_1.clear();
        }
    }
}

/**
 * @tc.name:SUB_Driver_Camera_Macro_0200
 * @tc.desc: Update macro ability setting and check the callback
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_Macro_0200, TestSize.Level1)
{
    int32_t rc;
    // step 2: set callback object
    cameraTest->hostCallbackV1_2 = new OHOS::Camera::Test::TestCameraHostCallbackV1_2();
    rc = cameraTest->serviceV1_2->SetCallback_V1_2(cameraTest->hostCallbackV1_2);
    EXPECT_EQ(rc, 0);
    // Start OHOS_ABILITY_CAMERA_MACRO_SUPPORTED ability query
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MACRO_SUPPORTED, &entry);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        cameraTest->intents = {PREVIEW, STILL_CAPTURE, VIDEO};
        cameraTest->StartStream(cameraTest->intents);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
        cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
        cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, true);
        cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
        sleep(1);
        cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdCapture,
            cameraTest->captureIdVideo};
        cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdCapture,
            cameraTest->streamIdVideo};
        cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
        sleep(UT_SECOND_TIMES);
        common_metadata_header_t* data = cameraTest->deviceCallback->resultMeta->get();
        EXPECT_NE(data, nullptr);
        camera_metadata_item_t entry;
        int ret = FindCameraMetadataItem(data, OHOS_CAMERA_MACRO_STATUS, &entry);
        if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
            uint8_t value = entry.data.u8[0];
            // 查询到状态， 检测状态到 微距模式可开启
            if (OHOS_CAMERA_MACRO_ENABLE == value) {
                printf("Macro mode is set enabled.");
            } else {
                printf("Macro mode is not enabled.");
            }
        } else {
            GTEST_SKIP() << "OHOS_CAMERA_MACRO_STATUS NOT FOUND" << std::endl;
        }
    } else {
        GTEST_SKIP() << "OHOS_ABILITY_CAMERA_MACRO_SUPPORTED NOT FOUND" << std::endl;
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Macro_0300
 * @tc.desc: macro mode CAPTURE_MACRO
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_Macro_0300, TestSize.Level1)
{
    CAMERA_LOGI("test SUB_Driver_Camera_Macro_0300 start ...");
    // Get Stream Operator
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_1(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // capture streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    float zoomRatio = 15;
    uint8_t macroControl = OHOS_CAMERA_MACRO_ENABLE;
    meta->addEntry(OHOS_CONTROL_ZOOM_RATIO, &zoomRatio, DATA_COUNT);
    meta->addEntry(OHOS_CONTROL_CAMERA_MACRO, &macroControl, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // is streams supported V1_1
    StreamSupportType pType;
    cameraTest->rc = cameraTest->streamOperator_V1_1->IsStreamsSupported_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::CAPTURE_MACRO),
        setting, cameraTest->streamInfosV1_1, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::CAPTURE_MACRO),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_Macro_0400
 * @tc.desc: macro mode VIDEO_MACRO
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_Macro_0400, TestSize.Level1)
{
    CAMERA_LOGI("test SUB_Driver_Camera_Macro_0400 start ...");
    // Get Stream Operator
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_1(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // video streamInfo
    cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    float zoomRatio = 15;
    uint8_t macroControl = OHOS_CAMERA_MACRO_ENABLE;
    meta->addEntry(OHOS_CONTROL_ZOOM_RATIO, &zoomRatio, DATA_COUNT);
    meta->addEntry(OHOS_CONTROL_CAMERA_MACRO, &macroControl, DATA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // is streams supported V1_1
    StreamSupportType tType;
    cameraTest->rc = cameraTest->streamOperator_V1_1->IsStreamsSupported_V1_1(
    static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::VIDEO_MACRO),
    setting, cameraTest->streamInfosV1_1, tType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams_V1_1(
    static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::VIDEO_MACRO),
    cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: NotifyDeviceStateChangeInfo
 * @tc.desc: notifyType fallingState deviceState fallingState
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_NotifyDevice_0100, TestSize.Level1)
{
    int32_t notifyType = 1;
    int32_t deviceState = 1008;
    cameraTest->rc = cameraTest->serviceV1_2->NotifyDeviceStateChangeInfo(notifyType, deviceState);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: NotifyDeviceStateChangeInfo
 * @tc.desc: notifyType foldState deviceState unknown
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_NotifyDevice_0200, TestSize.Level1)
{
    int32_t notifyType = 2;
    int32_t deviceState = 0;
    cameraTest->rc = cameraTest->serviceV1_2->NotifyDeviceStateChangeInfo(notifyType, deviceState);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: NotifyDeviceStateChangeInfo
 * @tc.desc: notifyType foldState deviceState expand
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_NotifyDevice_0300, TestSize.Level1)
{
    int32_t notifyType = 2;
    int32_t deviceState = 1;
    cameraTest->rc = cameraTest->serviceV1_2->NotifyDeviceStateChangeInfo(notifyType, deviceState);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: NotifyDeviceStateChangeInfo
 * @tc.desc: notifyType foldState deviceState folded
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_NotifyDevice_0400, TestSize.Level1)
{
    int32_t notifyType = 2;
    int32_t deviceState = 2;
    cameraTest->rc = cameraTest->serviceV1_2->NotifyDeviceStateChangeInfo(notifyType, deviceState);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: NotifyDeviceStateChangeInfo
 * @tc.desc: notifyType foldState deviceState halfFolded
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_NotifyDevice_0500, TestSize.Level1)
{
    int32_t notifyType = 2;
    int32_t deviceState = 3;
    cameraTest->rc = cameraTest->serviceV1_2->NotifyDeviceStateChangeInfo(notifyType, deviceState);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.name: NotifyDeviceStateChangeInfo
 * @tc.desc: notifyType foldState deviceState error
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_NotifyDevice_0600, TestSize.Level1)
{
    int32_t notifyType = 2;
    int32_t deviceState = 10;
    cameraTest->rc = cameraTest->serviceV1_2->NotifyDeviceStateChangeInfo(notifyType, deviceState);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.name: CommitStreams_V1_1_SUPER_STAB
 * @tc.desc: CommitStreams_V1_1 for super stabilization mode, preview and video
 * @tc.size: MediumTest
 * @tc.type: Function
 */
static void UpdateSettingsForSuperStabMode(std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
    bool isTagExitst = cameraTest->IsTagValueExistsU8(cameraTest->ability,
        OHOS_ABILITY_VIDEO_STABILIZATION_MODES,
        OHOS_CAMERA_VIDEO_STABILIZATION_HIGH);
    if (!isTagExitst) {
        GTEST_SKIP() << "skip this test, because stabilization tag not found" << std::endl;
        return;
    }
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t stabControl = OHOS_CAMERA_VIDEO_STABILIZATION_HIGH;
    meta->addEntry(OHOS_CONTROL_VIDEO_STABILIZATION_MODE, &stabControl, DATA_COUNT);
    // ability meta data serialization for updating
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    CAMERA_LOGD("Macro mode is set enabled.");
}

HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_SteadyShot_0300, TestSize.Level1)
{
    if (!cameraTest->IsTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_VIDEO_STABILIZATION_MODES,
        OHOS_CAMERA_VIDEO_STABILIZATION_HIGH)) {
        GTEST_SKIP() << "OHOS_ABILITY_VIDEO_STABILIZATION_MODES NOT FOUND" << std::endl;
        return;
    }
    // Get Stream Operator
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_1(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // video streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // Capture streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // create and commitstreams
    UpdateSettingsForSuperStabMode(cameraTest);
    cameraTest->imageDataSaveSwitch = SWITCH_ON;
    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::NORMAL),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // start preview, video and capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    // wait to stop
    uint32_t waitTime = 0;
    auto envStr = getenv("UT_SUPER_STAB_KEEP_SECOND");
    if (envStr != nullptr) {
        waitTime = atoi(envStr);
    }
    waitTime = (waitTime > 0 && waitTime < UT_SECOND_TIMES_MAX) ? waitTime : UT_SECOND_TIMES;
    sleep(waitTime);
    // stop stream
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_Sketch_0300
 * @tc.desc: sketch
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_Sketch_0300, TestSize.Level1)
{
    CAMERA_LOGI("test Camera_Device_Hdi_V1_2_003 start");
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_1(
        cameraTest->streamOperatorCallback, cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreviewV1_2(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // sketch streamInfo
    cameraTest->streamInfoSketch = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    // sketch extended streamInfo
    OHOS::HDI::Camera::V1_1::ExtendedStreamInfo extendedStreamInfo {
        .type = static_cast<OHOS::HDI::Camera::V1_1::ExtendedStreamInfoType>(
            OHOS::HDI::Camera::V1_2::EXTENDED_STREAM_INFO_SKETCH),
	    .width = 0,
   	    .height = 0,
   	    .format = 0,
	    .dataspace = 0,
	    .bufferQueue = nullptr
    };
    cameraTest->streamInfoSketch->extendedStreamInfos = {extendedStreamInfo};
    cameraTest->DefaultInfosSketch(cameraTest->streamInfoSketch);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoSketch);

    std::shared_ptr<CameraSetting> modeSetting = std::make_shared<CameraSetting>(100, 200);
    float zoomRatio = 20;
    modeSetting->addEntry(OHOS_CONTROL_ZOOM_RATIO, &zoomRatio, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, metaVec);
    cameraTest->cameraDeviceV1_2->UpdateSettings(metaVec);

    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(
		    OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdSketch, cameraTest->captureIdSketch, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdSketch};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdSketch};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: SUB_Driver_Camera_Sketch_0400
 * @tc.desc: sketch
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_2, SUB_Driver_Camera_Sketch_0400, TestSize.Level1)
{
    CAMERA_LOGI("test Camera_Device_Hdi_V1_2_003 start");
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_2->GetStreamOperator_V1_1(
        cameraTest->streamOperatorCallback, cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreviewV1_2(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    // sketch streamInfo
    cameraTest->streamInfoSketch = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    // sketch extended streamInfo
    OHOS::HDI::Camera::V1_1::ExtendedStreamInfo extendedStreamInfo {
        .type = static_cast<OHOS::HDI::Camera::V1_1::ExtendedStreamInfoType>(
            OHOS::HDI::Camera::V1_2::EXTENDED_STREAM_INFO_SKETCH),
	    .width = 0,
   	    .height = 0,
   	    .format = 0,
	    .dataspace = 0,
	    .bufferQueue = nullptr
    };
    cameraTest->streamInfoSketch->extendedStreamInfos = {extendedStreamInfo};
    cameraTest->DefaultInfosSketch(cameraTest->streamInfoSketch);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoSketch);
    std::shared_ptr<CameraSetting> modeSetting = std::make_shared<CameraSetting>(100, 200);
    float zoomRatio = 3;
    modeSetting->addEntry(OHOS_CONTROL_ZOOM_RATIO, &zoomRatio, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, metaVec);
    cameraTest->cameraDeviceV1_2->UpdateSettings(metaVec);
    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(
		    OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdSketch, cameraTest->captureIdSketch, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdSketch};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdSketch};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}
