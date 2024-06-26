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

#include "hdi_common_v1_3.h"
#include "camera.h"
#include "video_key_info.h"
constexpr uint32_t ITEM_CAPACITY = 100;
constexpr uint32_t DATA_CAPACITY = 2000;
constexpr uint32_t DATA_COUNT = 1;
constexpr uint32_t FPS_COUNT = 2;
constexpr uint32_t CHECK_AREA_COUNT = 4;
constexpr uint32_t STREAMINFO_WIDTH = 1920;
constexpr uint32_t STREAMINFO_HEIGHT = 1080;
constexpr uint32_t HIGH_RESOLUTION_PHOTO_WIDTH = 8192;
constexpr uint32_t HIGH_RESOLUTION_PHOTO_HEIGHT = 6144;
namespace OHOS::Camera {
Test::ResultCallback Test::resultCallback_ = 0;
OHOS::HDI::Camera::V1_0::FlashlightStatus Test::statusCallback =
                static_cast<OHOS::HDI::Camera::V1_0::FlashlightStatus>(0);
uint64_t Test::GetCurrentLocalTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp =
        std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    return static_cast<uint64_t>(tmp.count());
}

int32_t Test::DumpImageFile(int streamId, std::string suffix, const void* buffer, int32_t size)
{
    if (imageDataSaveSwitch == SWITCH_OFF) {
        return 0;
    }
    if (streamId < 0) {
        CAMERA_LOGE("ivalid stream id: %{public}d", streamId);
        return -1;
    }
    char mkdirCmd[PATH_MAX] = {0};
    char path[PATH_MAX] = {0};
    int ret = sprintf_s(mkdirCmd, sizeof(mkdirCmd) / sizeof(mkdirCmd[0]),
        "mkdir -p /data/stream-%d", streamId);
    if (ret < 0) {
        return -1;
    }
    system(mkdirCmd);
    ret = sprintf_s(path, sizeof(path) / sizeof(path[0]), "/data/stream-%d/%lld.%s",
        streamId, GetCurrentLocalTimeStamp(), suffix.c_str());
    if (ret < 0) {
        return -1;
    }

    int imgFd = open(path, O_RDWR | O_CREAT, 00766);
    if (imgFd == -1) {
        CAMERA_LOGE("open file failed, errno: %{public}s", strerror(errno));
        return -1;
    }

    ret = write(imgFd, buffer, size);
    if (ret == -1) {
        CAMERA_LOGE("write file failed, error: %{public}s", strerror(errno));
        close(imgFd);
        return -1;
    }
    close(imgFd);
    return 0;
}

void Test::Init()
{
    uint32_t mainVer;
    uint32_t minVer;
    int32_t ret;
    if (serviceV1_3 == nullptr) {
        serviceV1_3 = OHOS::HDI::Camera::V1_3::ICameraHost::Get("camera_service", false);
        EXPECT_NE(serviceV1_3, nullptr);
        CAMERA_LOGI("V1_2::ICameraHost get success");
        ret = serviceV1_3->GetVersion(mainVer, minVer);
        EXPECT_EQ(ret, 0);
        CAMERA_LOGI("V1_2::ICameraHost get version success, %{public}d, %{public}d", mainVer, minVer);
    }

    hostCallback = new TestCameraHostCallback();
    ret = serviceV1_3->SetCallback(hostCallback);
    EXPECT_EQ(ret, 0);
    serviceV1_3->GetCameraIds(cameraIds);
    if (cameraIds.size() == 0) {
        CAMERA_LOGE("camera device list empty");
        GTEST_SKIP() << "No Camera Available" << std::endl;
        return;
    }
}

void Test::Open(int cameraId)
{
    if (cameraDeviceV1_3 == nullptr) {
        if (cameraIds.size() == 0) {
            CAMERA_LOGE("camera device list empty");
            GTEST_SKIP() << "No Camera Available" << std::endl;
            return;
        }
        EXPECT_NE(serviceV1_3, nullptr);
        EXPECT_NE(cameraIds.size(), 0);
        GetCameraMetadata(cameraId);
        deviceCallback = new OHOS::Camera::Test::DemoCameraDeviceCallback();

        EXPECT_NE(serviceV1_3, nullptr);
        if (DEVICE_1 == cameraId) {
            rc = serviceV1_3->OpenCamera_V1_3(cameraIds[1], deviceCallback, cameraDeviceV1_3);
        } else {
            rc = serviceV1_3->OpenCamera_V1_3(cameraIds[0], deviceCallback, cameraDeviceV1_3);
        }
        EXPECT_EQ(rc, HDI::Camera::V1_0::NO_ERROR);
        EXPECT_NE(cameraDeviceV1_3, nullptr);
        CAMERA_LOGI("OpenCamera V1_2 success");
    }
}

void Test::OpenSecureCamera(int cameraId)
{
    if (cameraDeviceV1_3 == nullptr) {
        serviceV1_3->GetCameraIds(cameraIds);
        if (cameraIds.size() == 0) {
            CAMERA_LOGE("camera device list empty");
            GTEST_SKIP() << "No Camera Available" << std::endl;
            return;
        }
        EXPECT_NE(serviceV1_3, nullptr);
        EXPECT_NE(cameraIds.size(), 0);
        GetCameraMetadata(cameraId);
        deviceCallback = new OHOS::Camera::Test::DemoCameraDeviceCallback();

        EXPECT_NE(serviceV1_3, nullptr);
        if (DEVICE_1 == cameraId) {
            rc = serviceV1_3->OpenSecureCamera(cameraIds[1], deviceCallback, cameraDeviceV1_3);
        } else {
            rc = serviceV1_3->OpenSecureCamera(cameraIds[0], deviceCallback, cameraDeviceV1_3);
        }
        EXPECT_EQ(rc, HDI::Camera::V1_0::NO_ERROR);
        EXPECT_NE(cameraDeviceV1_3, nullptr);
        CAMERA_LOGI("OpenSecureCamera success");
    }
}


void Test::GetCameraMetadata(int cameraId)
{
    if (DEVICE_1 == cameraId) {
        rc = serviceV1_3->GetCameraAbility(cameraIds[1], abilityVec);
    } else {
        rc = serviceV1_3->GetCameraAbility(cameraIds[0], abilityVec);
    }
    if (rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGE("GetCameraAbility failed, rc = %{public}d", rc);
    }
    MetadataUtils::ConvertVecToMetadata(abilityVec, ability);
    EXPECT_NE(ability, nullptr);
}

void Test::PrintAllTagDataU8(std::shared_ptr<CameraMetadata> ability, uint32_t tag)
{
    common_metadata_header_t* data = ability->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, tag, &entry);
    EXPECT_EQ(ret, 0);
    EXPECT_NE(entry.count, 0);
    std::cout << "----tag = " << tag << "count = " << entry.count << std::endl;
    for (int i = 0; i < entry.count; i++) {
        int v = entry.data.u8[i];
        std::cout << "tag[" << tag << "][" << i << "] = " << v << std::endl;
    }
    std::cout << "--------------------------------" << std::endl;
}

void Test::Close()
{
    if (cameraDeviceV1_3 != nullptr) {
        cameraDeviceV1_3->Close();
        cameraDeviceV1_3 = nullptr;
    }
}

void Test::DefaultPreview(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    infos->v1_0.streamId_ = streamIdPreview;
    infos->v1_0.width_ = previewWidth;
    infos->v1_0.height_ = previewHeight;
    infos->v1_0.format_ = previewFormat;
    infos->v1_0.dataspace_ = UT_DATA_SIZE;
    infos->v1_0.intent_ = StreamIntent::PREVIEW;
    infos->v1_0.tunneledMode_ = UT_TUNNEL_MODE;
}

void Test::DefaultCapture(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    infos->v1_0.streamId_ = streamIdCapture;
    infos->v1_0.width_ = captureWidth;
    infos->v1_0.height_ = captureHeight;
    infos->v1_0.format_ = snapshotFormat;
    infos->v1_0.dataspace_ = UT_DATA_SIZE;
    infos->v1_0.intent_ = StreamIntent::STILL_CAPTURE;
    infos->v1_0.tunneledMode_ = UT_TUNNEL_MODE;
}

void Test::CreateAndCommitStreamsForHighFrameRate(std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
    // Get stream operator
    cameraTest->streamOperatorCallbackV1_3 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_3();
    cameraTest->rc = cameraTest->cameraDeviceV1_3->GetStreamOperator_V1_3(cameraTest->streamOperatorCallbackV1_3,
        cameraTest->streamOperator_V1_3);
    EXPECT_NE(cameraTest->streamOperator_V1_3, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // Preview streamInfo
    cameraTest->streamInfoPre = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoPre);
    cameraTest->streamInfoPre->v1_0.width_ = STREAMINFO_WIDTH;
    cameraTest->streamInfoPre->v1_0.height_ = STREAMINFO_HEIGHT;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoPre);
    // Video streamInfo
    cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
    cameraTest->streamInfoVideo->v1_0.width_ = STREAMINFO_WIDTH;
    cameraTest->streamInfoVideo->v1_0.height_ = STREAMINFO_HEIGHT;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
    // Create and commitStream
    cameraTest->rc = cameraTest->streamOperator_V1_3->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_3->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_3::HIGH_FRAME_RATE),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
}

void Test::CreateAndCommitStreamsForSlowMotion(std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
    // Get stream operator
    cameraTest->streamOperatorCallbackV1_3 = new OHOS::Camera::Test::TestStreamOperatorCallbackV1_3();
    cameraTest->rc = cameraTest->cameraDeviceV1_3->GetStreamOperator_V1_3(cameraTest->streamOperatorCallbackV1_3,
        cameraTest->streamOperator_V1_3);
    EXPECT_NE(cameraTest->streamOperator_V1_3, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    // Preview streamInfo
    cameraTest->streamInfoPre = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoPre);
    cameraTest->streamInfoPre->v1_0.width_ = STREAMINFO_WIDTH;
    cameraTest->streamInfoPre->v1_0.height_ = STREAMINFO_HEIGHT;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoPre);
    // Video streamInfo
    cameraTest->streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosVideo(cameraTest->streamInfoVideo);
    cameraTest->streamInfoVideo->v1_0.width_ = STREAMINFO_WIDTH;
    cameraTest->streamInfoVideo->v1_0.height_ = STREAMINFO_HEIGHT;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoVideo);
    // Create and commitStream
    cameraTest->rc = cameraTest->streamOperator_V1_3->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_3->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_2::SLOW_MOTION),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
}

void Test::UpdateSettingsForSlowMotionMode(std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
    // Update settings
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    int32_t valueInvalid[2] = {240, 240};
    float motionCheckArea[4] = {1, 1, 1, 1};
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, &valueInvalid, FPS_COUNT);
    meta->addEntry(OHOS_CONTROL_MOTION_DETECTION_CHECK_AREA, &motionCheckArea, CHECK_AREA_COUNT);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    int32_t slowMotionControl = OHOS_CAMERA_MOTION_DETECTION_ENABLE;
    meta->addEntry(OHOS_CONTROL_MOTION_DETECTION, &slowMotionControl, DATA_COUNT);
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    cameraTest->rc = (CamRetCode)cameraTest->cameraDeviceV1_3->UpdateSettings(setting);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    
    // Start capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdVideo, cameraTest->captureIdVideo, false, true);
    cameraTest->captureIds = {cameraTest->captureIdPreview, cameraTest->captureIdVideo};
    cameraTest->streamIds = {cameraTest->streamIdPreview, cameraTest->streamIdVideo};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

void Test::SuperSlowMotionStatusCallback(std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
    if (cameraTest->deviceCallback->resultMeta == nullptr) {
        CAMERA_LOGI("callback not triggered");
        return;
    }
    common_metadata_header_t* data = cameraTest->deviceCallback->resultMeta->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_STATUS_SLOW_MOTION_DETECTION, &entry);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        uint8_t value = entry.data.u8[0];
        // Detect the state of super slow motion
        if (value == OHOS_CONTROL_SLOW_MOTION_STATUS_DISABLE) {
            CAMERA_LOGI("slow motion status is disabled");
        } else if (value == OHOS_CONTROL_SLOW_MOTION_STATUS_READY) {
            CAMERA_LOGI("slow motion status is ready");
        } else if (value == OHOS_CONTROL_SLOW_MOTION_STATUS_START) {
            CAMERA_LOGI("slow motion status is started");
        } else if (value == OHOS_CONTROL_SLOW_MOTION_STATUS_RECORDING) {
            CAMERA_LOGI("slow motion status is recording");
        } else if (value == OHOS_CONTROL_SLOW_MOTION_STATUS_FINISH) {
            CAMERA_LOGI("slow motion status is finished");
        }
    }
}

void Test::startStreamForHighResolutionPhoto(std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
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
    cameraTest->streamInfoCapture->v1_0.width_ = HIGH_RESOLUTION_PHOTO_WIDTH;
    cameraTest->streamInfoCapture->v1_0.height_ = HIGH_RESOLUTION_PHOTO_HEIGHT;
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    // Create and commit streams
    cameraTest->rc = cameraTest->streamOperator_V1_3->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_3->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(OHOS::HDI::Camera::V1_3::HIGH_RESOLUTION_PHOTO),
        cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
}

void Test::CaptureByColorSpacesWithUpdateStreams(std::vector<int32_t> captureColorSpaces,
    std::shared_ptr<OHOS::Camera::Test> cameraTest)
{
    printf("Enter CaptureByColorSpacesWithUpdateStreams function!\n");
    if (!captureColorSpaces.empty()) {
        // Clear infos
        cameraTest->streamInfosV1_1.clear();
        // Preview streamInfo
        cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
        cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
        cameraTest->streamInfoV1_1->v1_0.dataspace_ = captureColorSpaces[0];
        cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
        // Capture streamInfo
        cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
        cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
        cameraTest->streamInfoCapture->v1_0.dataspace_ = captureColorSpaces[0];
        cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

        // StreamInfosV1_1 should not be empty
        cameraTest->rc = cameraTest->streamOperator_V1_3->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
        EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
        cameraTest->rc = cameraTest->streamOperator_V1_3->CommitStreams_V1_1(
            OHOS::HDI::Camera::V1_1::OperationMode_V1_1::NORMAL, cameraTest->abilityVec);
        // Start without colorspace setting
        cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
        cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);

        for (int32_t colorSpaces : captureColorSpaces) {
            printf("capture colorSpaces value %d\n", colorSpaces);
            // CancelCapture
            cameraTest->streamOperator_V1_3->CancelCapture(cameraTest->captureIdPreview);
            // Clear infos
            cameraTest->streamInfosV1_1.clear();
            // Preview streamInfo
            cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
            cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
            cameraTest->streamInfoV1_1->v1_0.dataspace_ = colorSpaces;
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
            // Capture streamInfo
            cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
            cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
            cameraTest->streamInfoCapture->v1_0.dataspace_ = colorSpaces;
            cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);
            // UpdateStreams
            cameraTest->rc = cameraTest->streamOperator_V1_3->UpdateStreams(cameraTest->streamInfosV1_1);
            // Capture
            EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
            cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
            cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
            sleep(UT_SECOND_TIMES);
        }

        // StopStream
        cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
    }
}

void Test::DefaultSketch(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    infos->v1_0.streamId_ = streamIdSketch;
    infos->v1_0.width_ = sketchWidth;
    infos->v1_0.height_ = sketchHeight;
    infos->v1_0.format_ = previewFormat;
    infos->v1_0.dataspace_ = UT_DATA_SIZE;
    infos->v1_0.intent_ = StreamIntent::PREVIEW;
    infos->v1_0.tunneledMode_ = UT_TUNNEL_MODE;
}

void Test::DefaultInfosSketch(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    DefaultSketch(infos);
    std::shared_ptr<StreamConsumer> consumer_pre = std::make_shared<StreamConsumer>();
    infos->v1_0.bufferQueue_ = consumer_pre->CreateProducerSeq([this](void* addr, uint32_t size) {
        DumpImageFile(streamIdSketch, "yuv", addr, size);
    });
    infos->v1_0.bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    consumerMap_[StreamIntent::PREVIEW] = consumer_pre;
}

void Test::DefaultInfosPreviewV1_2(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    DefaultPreview(infos);
    std::shared_ptr<StreamConsumer> consumer_pre = std::make_shared<StreamConsumer>();
    infos->v1_0.bufferQueue_ = consumer_pre->CreateProducerSeq([this](void* addr, uint32_t size) {
        DumpImageFile(streamIdPreview, "yuv", addr, size);
    });
    infos->v1_0.bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
}

void Test::DefaultInfosPreview(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    DefaultPreview(infos);
    std::shared_ptr<StreamConsumer> consumer_pre = std::make_shared<StreamConsumer>();
    infos->v1_0.bufferQueue_ = consumer_pre->CreateProducerSeq([this](void* addr, uint32_t size) {
        DumpImageFile(streamIdPreview, "yuv", addr, size);
    });
    infos->v1_0.bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    consumerMap_[StreamIntent::PREVIEW] = consumer_pre;
}

void Test::DefaultInfosCapture(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    DefaultCapture(infos);
    std::shared_ptr<StreamConsumer> consumer_capture = std::make_shared<StreamConsumer>();
    infos->v1_0.bufferQueue_ = consumer_capture->CreateProducerSeq([this](void* addr, uint32_t size) {
        DumpImageFile(streamIdCapture, "jpeg", addr, size);
    });
    infos->v1_0.bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    consumerMap_[StreamIntent::STILL_CAPTURE] = consumer_capture;
}

void Test::DefaultInfosProfessionalCapture(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    DefaultCapture(infos);
    std::shared_ptr<StreamConsumer> consumer_capture = std::make_shared<StreamConsumer>();
    infos->v1_0.bufferQueue_ = consumer_capture->CreateProducerSeq([this](void* addr, uint32_t size) {
        DumpImageFile(streamIdCapture, "yuv", addr, size);
    });
    infos->v1_0.bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    consumerMap_[StreamIntent::STILL_CAPTURE] = consumer_capture;
}

void Test::DefaultInfosVideo(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    infos->v1_0.streamId_ = streamIdVideo;
    infos->v1_0.width_ = videoWidth;
    infos->v1_0.height_ = videoHeight;
    infos->v1_0.format_ = videoFormat;
    infos->v1_0.dataspace_ = UT_DATA_SIZE;
    infos->v1_0.intent_ = StreamIntent::VIDEO;
    infos->v1_0.encodeType_ = ENCODE_TYPE_H265;
    infos->v1_0.tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<StreamConsumer> consumer_video = std::make_shared<StreamConsumer>();
    infos->v1_0.bufferQueue_ = consumer_video->CreateProducerSeq([this](void* addr, uint32_t size) {
        DumpImageFile(streamIdVideo, "yuv", addr, size);
    });
    infos->v1_0.bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    consumerMap_[StreamIntent::VIDEO] = consumer_video;
}

void Test::DefaultInfosAnalyze(
    std::shared_ptr<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1> &infos)
{
    infos->v1_0.streamId_ = streamIdAnalyze;
    infos->v1_0.width_ = analyzeWidth;
    infos->v1_0.height_ = analyzeHeight;
    infos->v1_0.format_ = analyzeFormat;
    infos->v1_0.dataspace_ = UT_DATA_SIZE;
    infos->v1_0.intent_ = StreamIntent::ANALYZE;
    infos->v1_0.tunneledMode_ = UT_TUNNEL_MODE;

    std::shared_ptr<StreamConsumer> consumer_analyze = std::make_shared<StreamConsumer>();
    infos->v1_0.bufferQueue_ = consumer_analyze->CreateProducerSeq([this](void* addr, uint32_t size) {
        common_metadata_header_t *data = static_cast<common_metadata_header_t *>(addr);
        camera_metadata_item_t entry = {};

        int ret = FindCameraMetadataItem(data, OHOS_STATISTICS_FACE_IDS, &entry);
        if (ret == 0) {
            for (size_t i = 0; i < entry.count; i++) {
                int id = entry.data.i32[i];
                CAMERA_LOGI("Face ids : %{public}d", id);
            }
        }

        ret = FindCameraMetadataItem(data, OHOS_STATISTICS_FACE_RECTANGLES, &entry);
        if (ret == 0) {
            for (size_t i = 0; i < entry.count; i++) {
                int id = entry.data.i32[i];
                CAMERA_LOGI("Face rectangles : %{public}d", id);
            }
        }
    });
    infos->v1_0.bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    consumerMap_[StreamIntent::ANALYZE] = consumer_analyze;
}

void Test::StartProfessionalStream(std::vector<StreamIntent> intents, uint8_t professionalMode)
{
    streamOperatorCallbackV1_3 =
        OHOS::sptr<OHOS::HDI::Camera::V1_3::IStreamOperatorCallback> (new TestStreamOperatorCallbackV1_3);
    uint32_t mainVersion = 1;
    uint32_t minVersion = 0;
    rc = cameraDeviceV1_3->GetStreamOperator_V1_3(streamOperatorCallbackV1_3, streamOperator_V1_3);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        rc = streamOperator_V1_3->GetVersion(mainVersion, minVersion);
        if (rc != HDI::Camera::V1_0::NO_ERROR) {
            CAMERA_LOGE("streamOperator_V1_3 get version failed, rc = %{public}d", rc);
        } else {
            CAMERA_LOGI("streamOperator_V1_3 get version success, %{public}u, %{public}u",
                mainVersion, minVersion);
        }
        CAMERA_LOGI("GetStreamOperator success");
    } else {
        CAMERA_LOGE("GetStreamOperator fail, rc = %{public}d", rc);
    }
    streamInfoPre = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    streamInfoAnalyze = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();

    for (auto& streamType : intents) {
        if (streamType == StreamIntent::PREVIEW) {
            DefaultInfosPreview(streamInfoPre);
            streamInfos.push_back(*streamInfoPre);
        } else if (streamType == StreamIntent::VIDEO) {
            DefaultInfosVideo(streamInfoVideo);
            streamInfos.push_back(*streamInfoVideo);
        } else if (streamType == StreamIntent::ANALYZE) {
            DefaultInfosAnalyze(streamInfoAnalyze);
            streamInfos.push_back(*streamInfoAnalyze);
        } else {
            DefaultInfosProfessionalCapture(streamInfoCapture);
            streamInfos.push_back(*streamInfoCapture);
        }
    }

    rc = streamOperator_V1_3->CreateStreams_V1_1(streamInfos);
    EXPECT_EQ(false, rc != HDI::Camera::V1_0::NO_ERROR);
    rc = streamOperator_V1_3->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(professionalMode),
        abilityVec);
    EXPECT_EQ(false, rc != HDI::Camera::V1_0::NO_ERROR);
    sleep(1);
    std::vector<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>().swap(streamInfos);
}

void Test::StartStream(std::vector<StreamIntent> intents, OHOS::HDI::Camera::V1_3::OperationMode mode)
{
    streamOperatorCallbackV1_3 =
        OHOS::sptr<OHOS::HDI::Camera::V1_3::IStreamOperatorCallback> (new TestStreamOperatorCallbackV1_3);
    uint32_t mainVersion = 1;
    uint32_t minVersion = 0;
    rc = cameraDeviceV1_3->GetStreamOperator_V1_3(streamOperatorCallbackV1_3, streamOperator_V1_3);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        rc = streamOperator_V1_3->GetVersion(mainVersion, minVersion);
        if (rc != HDI::Camera::V1_0::NO_ERROR) {
            CAMERA_LOGE("streamOperator_V1_3 get version failed, rc = %{public}d", rc);
        } else {
            CAMERA_LOGI("streamOperator_V1_3 get version success, %{public}u, %{public}u",
                mainVersion, minVersion);
        }
        CAMERA_LOGI("GetStreamOperator success");
    } else {
        CAMERA_LOGE("GetStreamOperator fail, rc = %{public}d", rc);
    }
    streamInfoPre = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    streamInfoVideo = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    streamInfoAnalyze = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();

    for (auto& intent : intents) {
        if (intent == StreamIntent::PREVIEW) {
            DefaultInfosPreview(streamInfoPre);
            streamInfos.push_back(*streamInfoPre);
        } else if (intent == StreamIntent::VIDEO) {
            DefaultInfosVideo(streamInfoVideo);
            streamInfos.push_back(*streamInfoVideo);
        } else if (intent == StreamIntent::ANALYZE) {
            DefaultInfosAnalyze(streamInfoAnalyze);
            streamInfos.push_back(*streamInfoAnalyze);
        } else {
            DefaultInfosCapture(streamInfoCapture);
            streamInfos.push_back(*streamInfoCapture);
        }
    }

    rc = streamOperator_V1_3->CreateStreams_V1_1(streamInfos);
    EXPECT_EQ(false, rc != HDI::Camera::V1_0::NO_ERROR);
    rc = streamOperator_V1_3->CommitStreams_V1_1(
        static_cast<OHOS::HDI::Camera::V1_1::OperationMode_V1_1>(mode), abilityVec);
    EXPECT_EQ(false, rc != HDI::Camera::V1_0::NO_ERROR);
    sleep(1);
    std::vector<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>().swap(streamInfos);
}

void Test::StartCapture(int streamId, int captureId, bool shutterCallback, bool isStreaming)
{
    captureInfo = std::make_shared<CaptureInfo>();
    captureInfo->streamIds_ = {streamId};
    captureInfo->captureSetting_ = abilityVec;
    captureInfo->enableShutterCallback_ = shutterCallback;
    EXPECT_EQ(true, streamOperator_V1_3 != nullptr);
    rc = (CamRetCode)streamOperator_V1_3->Capture(captureId, *captureInfo, isStreaming);
    EXPECT_EQ(true, rc == HDI::Camera::V1_0::NO_ERROR);
    if (rc == HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("check Capture: Capture success, %{public}d", captureId);
    } else {
        std::cout << "rc = " << rc << std::endl;
        CAMERA_LOGE("check Capture: Capture fail, rc = %{public}d", rc);
    }
    sleep(UT_SLEEP_TIME);
}

void Test::StopStream(std::vector<int>& captureIds, std::vector<int>& streamIds)
{
    if (captureIds.size() > 0) {
        for (auto &captureId : captureIds) {
            EXPECT_EQ(true, streamOperator_V1_3 != nullptr);
            rc = streamOperator_V1_3->CancelCapture(captureId);
            EXPECT_EQ(true, rc == HDI::Camera::V1_0::NO_ERROR);
            if (rc == HDI::Camera::V1_0::NO_ERROR) {
                CAMERA_LOGI("check Capture: CancelCapture success, %{public}d", captureId);
            } else {
                CAMERA_LOGE("check Capture: CancelCapture fail, rc = %{public}d, captureId = %{public}d",
                    rc, captureId);
            }
        }
    }
    if (streamIds.size() > 0) {
        EXPECT_EQ(true, streamOperator_V1_3 != nullptr);
        rc = streamOperator_V1_3->ReleaseStreams(streamIds);
        EXPECT_EQ(true, rc == HDI::Camera::V1_0::NO_ERROR);
        if (rc == HDI::Camera::V1_0::NO_ERROR) {
            CAMERA_LOGI("check Capture: ReleaseStream success");
        } else {
            CAMERA_LOGE("check Capture: ReleaseStreams fail, rc = %{public}d", rc);
        }
    }
}

void Test::StreamConsumer::CalculateFps(int64_t timestamp, int32_t streamId)
{
    if (isFirstCalculateFps_) {
        if ((timestamp - intervalTimestamp_) >= interval_) {
            int64_t timeInterval = timestamp - intervalTimestamp_;
            if (timeInterval != 0) {
                float fps = (int64_t)(100000000000 * timestampCount_ / timeInterval) / 100.0;
                CAMERA_LOGI("Calculate FPS success, streamId: %{public}d, Fps:%{public}f", streamId, fps);
                interval_ = ONESECOND_OF_MICROSECOND_UNIT;
            } else {
                CAMERA_LOGE("Calculate FPS error timeInerval is 0");
            }
        }
    } else {
        intervalTimestamp_ = timestamp;
        isFirstCalculateFps_ = true;
    }
    if ((timestamp - intervalTimestamp_) >= ONESECOND_OF_MICROSECOND_UNIT * UT_SECOND_TIMES) {
        intervalTimestamp_ = timestamp;
        timestampCount_ = 0;
        interval_ = ONESECOND_OF_MICROSECOND_UNIT;
    }
    timestampCount_++;
}

void Test::StreamConsumer::GetTimeStamp(int64_t *g_timestamp, uint32_t lenght, int64_t timestamp, int32_t gotSize)
{
    if (gotSize != UT_PREVIEW_SIZE) {
        if (g_timestamp[0] == 0) {
            g_timestamp[0] = timestamp;
        } else {
            g_timestamp[1] = timestamp;
        }
    }
}

OHOS::sptr<OHOS::IBufferProducer> Test::StreamConsumer::CreateProducer(std::function<void(void*, uint32_t)> callback)
{
    consumer_ = OHOS::IConsumerSurface::Create();
    if (consumer_ == nullptr) {
        return nullptr;
    }
    sptr<IBufferConsumerListener> listener = new TestBufferConsumerListener();
    consumer_->RegisterConsumerListener(listener);
    auto producer = consumer_->GetProducer();
    if (producer == nullptr) {
        return nullptr;
    }

    callback_ = callback;
    consumerThread_ = new std::thread([this, listener] {
        int32_t flushFence = 0;
        int64_t timestamp = 0;
        OHOS::Rect damage;
        TestBufferConsumerListener* checker = static_cast<TestBufferConsumerListener*>(listener.GetRefPtr());
        while (running_ == true) {
            OHOS::sptr<OHOS::SurfaceBuffer> buffer = nullptr;
            if (checker->checkBufferAvailable()) {
                consumer_->AcquireBuffer(buffer, flushFence, timestamp, damage);
                if (buffer != nullptr) {
                    void* addr = buffer->GetVirAddr();
                    uint32_t size = buffer->GetSize();
                    int32_t gotSize = 0;
                    int32_t isKey = 0;
                    int32_t streamId = 0;
                    int32_t captureId = 0;
                    buffer->GetExtraData()->ExtraGet(OHOS::Camera::dataSize, gotSize);
                    buffer->GetExtraData()->ExtraGet(OHOS::Camera::isKeyFrame, isKey);
                    buffer->GetExtraData()->ExtraGet(OHOS::Camera::timeStamp, timestamp);
                    buffer->GetExtraData()->ExtraGet(OHOS::Camera::streamId, streamId);
                    buffer->GetExtraData()->ExtraGet(OHOS::Camera::captureId, captureId);
                    GetTimeStamp(g_timestamp, sizeof(g_timestamp) / sizeof(g_timestamp[0]), timestamp, gotSize);
                    if (gotSize) {
                        CalculateFps(timestamp, streamId);
                        callback_(addr, gotSize);
                    } else {
                        callback_(addr, size);
                    }
                    consumer_->ReleaseBuffer(buffer, -1);
                    shotCount_--;
                    if (shotCount_ == 0) {
                        std::unique_lock<std::mutex> l(l_);
                        cv_.notify_one();
                    }
                }
            }
            if (running_ == false) {
                break;
            }
            usleep(1);
        }
    });

    return producer;
}

OHOS::sptr<BufferProducerSequenceable> Test::StreamConsumer::CreateProducerSeq(
    std::function<void(void*, uint32_t)> callback)
{
    OHOS::sptr<OHOS::IBufferProducer> producer = CreateProducer(callback);
    if (producer == nullptr) {
        return nullptr;
    }

    return new BufferProducerSequenceable(producer);
}

int32_t Test::TestStreamOperatorCallback::OnCaptureStarted(int32_t captureId, const std::vector<int32_t> &streamId)
{
    for (auto it : streamId) {
        CAMERA_LOGE("captureId: %{public}d, streamId: %{public}d", captureId, it);
    }
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestStreamOperatorCallback::OnCaptureEnded(int32_t captureId, const std::vector<CaptureEndedInfo> &infos)
{
    for (auto it : infos) {
        CAMERA_LOGE("captureId: %{public}d, streamId: %{public}d, count: %{public}d", captureId, it.streamId_,
            it.frameCount_);
    }
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestStreamOperatorCallback::OnCaptureError(int32_t captureId, const std::vector<CaptureErrorInfo> &infos)
{
    for (auto it : infos) {
        CAMERA_LOGE("captureId: %{public}d, streamId: %{public}d, error: %{public}d", captureId, it.streamId_,
            it.error_);
    }
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestStreamOperatorCallback::OnFrameShutter(int32_t captureId,
    const std::vector<int32_t> &streamIds, uint64_t timestamp)
{
    (void)timestamp;
    for (auto it : streamIds) {
        CAMERA_LOGE("captureId: %{public}d, streamId: %{public}d", captureId, it);
    }
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestStreamOperatorCallbackV1_2::OnCaptureStarted(int32_t captureId, const std::vector<int32_t> &streamId)
{
    return instanceImpl.OnCaptureStarted(captureId, streamId);
}

int32_t Test::TestStreamOperatorCallbackV1_2::OnCaptureEnded(int32_t captureId,
    const std::vector<CaptureEndedInfo> &infos)
{
    return instanceImpl.OnCaptureEnded(captureId, infos);
}

int32_t Test::TestStreamOperatorCallbackV1_2::OnCaptureError(int32_t captureId,
    const std::vector<CaptureErrorInfo> &infos)
{
    return instanceImpl.OnCaptureError(captureId, infos);
}

int32_t Test::TestStreamOperatorCallbackV1_2::OnFrameShutter(int32_t captureId,
    const std::vector<int32_t> &streamIds, uint64_t timestamp)
{
    return instanceImpl.OnFrameShutter(captureId, streamIds, timestamp);
}

int32_t Test::TestStreamOperatorCallbackV1_2::OnCaptureStarted_V1_2(int32_t captureId,
    const std::vector<HDI::Camera::V1_2::CaptureStartedInfo> &infos)
{
    for (auto it : infos) {
        CAMERA_LOGI("captureId: %{public}d, streamId: %{public}d", captureId, it.streamId_);
    }
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestStreamOperatorCallbackV1_3::OnCaptureStarted(int32_t captureId, const std::vector<int32_t> &streamId)
{
    return instanceImpl.OnCaptureStarted(captureId, streamId);
}

int32_t Test::TestStreamOperatorCallbackV1_3::OnCaptureEnded(int32_t captureId,
    const std::vector<CaptureEndedInfo> &infos)
{
    return instanceImpl.OnCaptureEnded(captureId, infos);
}

int32_t Test::TestStreamOperatorCallbackV1_3::OnCaptureError(int32_t captureId,
    const std::vector<CaptureErrorInfo> &infos)
{
    return instanceImpl.OnCaptureError(captureId, infos);
}

int32_t Test::TestStreamOperatorCallbackV1_3::OnFrameShutter(int32_t captureId,
    const std::vector<int32_t> &streamIds, uint64_t timestamp)
{
    (void)timestamp;
    for (auto it : streamIds) {
        CAMERA_LOGI("OnFrameShutter captureId: %{public}d, streamId: %{public}d", captureId, it);
    }
    return instanceImpl.OnFrameShutter(captureId, streamIds, timestamp);
}

int32_t Test::TestStreamOperatorCallbackV1_3::OnCaptureStarted_V1_2(int32_t captureId,
    const std::vector<HDI::Camera::V1_2::CaptureStartedInfo> &infos)
{
    return instanceImpl.OnCaptureStarted_V1_2(captureId, infos);
}

int32_t Test::TestStreamOperatorCallbackV1_3::OnCaptureReady(int32_t captureId,
    const std::vector<int32_t> &streamIds, uint64_t timestamp)
{
    (void)timestamp;
    for (auto it : streamIds) {
        CAMERA_LOGI("OnCaptureReady captureId: %{public}d, streamId: %{public}d", captureId, it);
    }
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestStreamOperatorCallbackV1_3::OnFrameShutterEnd(int32_t captureId,
    const std::vector<int32_t> &streamIds, uint64_t timestamp)
{
    (void)timestamp;
    for (auto it : streamIds) {
        CAMERA_LOGI("OnFrameShutterEnd captureId: %{public}d, streamId: %{public}d", captureId, it);
    }
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::DemoCameraDeviceCallback::OnError(ErrorType type, int32_t errorMsg)
{
    CAMERA_LOGE("type: %{public}d, errorMsg: %{public}d", type, errorMsg);
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::DemoCameraDeviceCallback::OnResult(uint64_t timestamp, const std::vector<uint8_t> &result)
{
    MetadataUtils::ConvertVecToMetadata(result, resultMeta);
    if (Test::resultCallback_) {
        Test::resultCallback_(timestamp, resultMeta);
    }
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestCameraHostCallback::OnCameraStatus(const std::string& cameraId, CameraStatus status)
{
    CAMERA_LOGE("cameraId: %{public}s, status: %{public}d", cameraId.c_str(), status);
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestCameraHostCallback::OnFlashlightStatus(const std::string& cameraId, FlashlightStatus status)
{
    CAMERA_LOGE("cameraId: %{public}s, status: %{public}d", cameraId.c_str(), status);
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestCameraHostCallback::OnCameraEvent(const std::string& cameraId, CameraEvent event)
{
    CAMERA_LOGE("cameraId: %{public}s, status: %{public}d", cameraId.c_str(), event);
    return HDI::Camera::V1_0::NO_ERROR;
}

int32_t Test::TestCameraHostCallbackV1_2::OnCameraStatus(const std::string& cameraId, CameraStatus status)
{
    return instanceImpl.OnCameraStatus(cameraId, status);
}

int32_t Test::TestCameraHostCallbackV1_2::OnFlashlightStatus(const std::string& cameraId, FlashlightStatus status)
{
    return instanceImpl.OnFlashlightStatus(cameraId, status);
}

int32_t Test::TestCameraHostCallbackV1_2::OnCameraEvent(const std::string& cameraId, CameraEvent event)
{
    return instanceImpl.OnCameraEvent(cameraId, event);
}

int32_t Test::TestCameraHostCallbackV1_2::OnFlashlightStatus_V1_2(FlashlightStatus status)
{
    CAMERA_LOGE("status: %{public}d", status);
    Test::statusCallback = status;
    return HDI::Camera::V1_0::NO_ERROR;
}
} // OHOS::Camera