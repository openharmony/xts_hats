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
#include "camera_hdi_uttest_v1_1.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;
int64_t OHOS::Camera::Test::StreamConsumer::g_timestamp[2] = {0};
void CameraHdiTestV1_1::SetUpTestCase(void) {}
void CameraHdiTestV1_1::TearDownTestCase(void) {}
void CameraHdiTestV1_1::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
}

void CameraHdiTestV1_1::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.name: Prelaunch
 * @tc.desc: Prelaunch cameraId:device/0
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_Prelaunch_0100, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open();
}

/**
 * @tc.name: Prelaunch
 * @tc.desc: Prelaunch cameraId:{}
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_Prelaunch_0200, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    cameraTest->Open();
}

/**
 * @tc.name: Prelaunch
 * @tc.desc: Prelaunch cameraId:device/10
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_Prelaunch_0300, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    cameraTest->Open();
}

/**
 * @tc.name: Prelaunch
 * @tc.desc: Prelaunch cameraId:ABC
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_Prelaunch_0400, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    cameraTest->Open();
}

/**
 * @tc.name: SUB_DriverSystem_CameraHdiMg_0660
 * @tc.desc: OHOS_ABILITY_PRELAUNCH_AVAILABLE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_Prelaunch_0500, TestSize.Level1)
{
    cameraTest->Open();
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_PRELAUNCH_AVAILABLE, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "OHOS_ABILITY_PRELAUNCH_AVAILABLE value is " << static_cast<int>(entry.data.u8[0]) << std::endl;
}

/**
 * @tc.name: GetDefaultSettings
 * @tc.desc: GetDefaultSettings
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_GetDefaultSettings_0100, TestSize.Level1)
{
    cameraTest->Open();
    EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetDefaultSettings(cameraTest->abilityVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: GetDefaultSettings
 * @tc.desc: GetDefaultSettings
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_GetDefaultSettings_0200, TestSize.Level1)
{
    cameraTest->Open();
    std::shared_ptr<CameraAbility> ability;
    std::shared_ptr<CameraAbility> abilitys;
    EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetDefaultSettings(cameraTest->abilityVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    MetadataUtils::ConvertVecToMetadata(cameraTest->abilityVec, ability);
    common_metadata_header_t* data = ability->get();
    std::string metastring = FormatCameraMetadataToString(data);
    if (!metastring.empty()) {
        std::cout << "DefaultSettings = " << metastring << std::endl;
    }

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->abilityVec.clear();
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetDefaultSettings(cameraTest->abilityVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    MetadataUtils::ConvertVecToMetadata(cameraTest->abilityVec, abilitys);
    common_metadata_header_t* datas = abilitys->get();
    std::string metastrings = FormatCameraMetadataToString(datas);
    if (!metastrings.empty()) {
        std::cout << "DefaultSettings1 = " << metastrings << std::endl;
    }
}
/**
 * @tc.name: GetStreamOperator_V1_1,defer_stream
 * @tc.desc: GetStreamOperator_V1_1,defer_stream
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_DelayStream_0100, TestSize.Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetStreamOperator_V1_1(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator_V1_1);

    // Create stream
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // Attach bufferqueue
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    OHOS::sptr<BufferProducerSequenceable> bufferQueue =
        consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
            cameraTest->DumpImageFile(111, "yuv", addr, size);
    });
    EXPECT_NE(bufferQueue, nullptr);
    EXPECT_NE(bufferQueue->producer_, nullptr);
    bufferQueue->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->rc = cameraTest->streamOperator_V1_1->AttachBufferQueue(
        cameraTest->streamInfoV1_1->v1_0.streamId_, bufferQueue);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    // Capture
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    sleep(UT_SECOND_TIMES);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);

    // Release
    cameraTest->rc = cameraTest->streamOperator_V1_1->DetachBufferQueue(cameraTest->streamInfoV1_1->v1_0.streamId_);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::vector<int> streamIds = {cameraTest->streamInfoV1_1->v1_0.streamId_};
    cameraTest->rc = cameraTest->streamOperator_V1_1->ReleaseStreams(streamIds);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: Quick Thumbnail
 * @tc.desc: OHOS_ABILITY_STREAM_QUICK_THUMBNAIL_AVAILABLE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_QuickThumbnail_0100, TestSize.Level1)
{
    cameraTest->Open();
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data,OHOS_ABILITY_STREAM_QUICK_THUMBNAIL_AVAILABLE, &entry);
    if (ret != CAM_META_SUCCESS) {
        GTEST_SKIP() << "This function is not supported " << std::endl;
        return;
    }
    std::cout << "OHOS_ABILITY_STREAM_QUICK_THUMBNAIL_AVAILABLE value is"
        << static_cast<int>(entry.data.u8[0]) << std::endl;
}
/**
 * @tc.name: Quick Thumbnail
 * @tc.desc: timestamp query,EXTENDED_STREAM_INFO_QUICK_THUMBNAIL set 0
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_QuickThumbnail_0200, TestSize.Level1)
{
    int64_t timeStampCapture = 0;
    int64_t timeStampThumbnail = 0;
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetStreamOperator_V1_1(
        cameraTest->streamOperatorCallback, cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // capture extended streamInfo
    OHOS::HDI::Camera::V1_1::ExtendedStreamInfo extendedStreamInfo;
    extendedStreamInfo.type = OHOS::HDI::Camera::V1_1::EXTENDED_STREAM_INFO_QUICK_THUMBNAIL;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer2 =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    extendedStreamInfo.bufferQueue = consumer2->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(105, "yuv", addr, size);
    });
    EXPECT_NE(extendedStreamInfo.bufferQueue, nullptr);
    EXPECT_NE(extendedStreamInfo.bufferQueue->producer_, nullptr);
    extendedStreamInfo.bufferQueue->producer_->SetQueueSize(UT_DATA_SIZE);
    // quikThumbnial do not need these param
    extendedStreamInfo.width = 0;
    extendedStreamInfo.height = 0;
    extendedStreamInfo.format = 0;
    extendedStreamInfo.dataspace = 0;

    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->streamInfoCapture->extendedStreamInfos = {extendedStreamInfo};
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    timeStampThumbnail = OHOS::Camera::Test::StreamConsumer::g_timestamp[0];
    timeStampCapture = OHOS::Camera::Test::StreamConsumer::g_timestamp[1];
    EXPECT_EQ(true, timeStampThumbnail == timeStampCapture);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: Quick Thumbnail
 * @tc.desc: XTENDED_STREAM_INFO_QUICK_THUMBNAIL set 0
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_QuickThumbnail_0300, TestSize.Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetStreamOperator_V1_1(
        cameraTest->streamOperatorCallback, cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // capture extended streamInfo
    OHOS::HDI::Camera::V1_1::ExtendedStreamInfo extendedStreamInfo;
    extendedStreamInfo.type = OHOS::HDI::Camera::V1_1::EXTENDED_STREAM_INFO_QUICK_THUMBNAIL;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer2 =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    extendedStreamInfo.bufferQueue = consumer2->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(105, "yuv", addr, size);
    });
    EXPECT_NE(extendedStreamInfo.bufferQueue, nullptr);
    EXPECT_NE(extendedStreamInfo.bufferQueue->producer_, nullptr);
    extendedStreamInfo.bufferQueue->producer_->SetQueueSize(UT_DATA_SIZE);
    // quikThumbnial do not need these param
    extendedStreamInfo.width = 0;
    extendedStreamInfo.height = 0;
    extendedStreamInfo.format = 0;
    extendedStreamInfo.dataspace = 0;

    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->streamInfoCapture->extendedStreamInfos = {extendedStreamInfo};
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}



/**
 * @tc.name: Quick Thumbnail  
 * @tc.desc: Quick Thumbnail BufferSize Set different,Size = 40
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_QuickThumbnail_0400, TestSize.Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetStreamOperator_V1_1(
        cameraTest->streamOperatorCallback, cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // capture extended streamInfo
    OHOS::HDI::Camera::V1_1::ExtendedStreamInfo extendedStreamInfo;
    extendedStreamInfo.type = OHOS::HDI::Camera::V1_1::EXTENDED_STREAM_INFO_QUICK_THUMBNAIL;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer2 =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    extendedStreamInfo.bufferQueue = consumer2->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(105, "yuv", addr, 40);
    });
    EXPECT_NE(extendedStreamInfo.bufferQueue, nullptr);
    EXPECT_NE(extendedStreamInfo.bufferQueue->producer_, nullptr);
    extendedStreamInfo.bufferQueue->producer_->SetQueueSize(UT_DATA_SIZE);
    // quikThumbnial do not need these param
    extendedStreamInfo.width = 0;
    extendedStreamInfo.height = 0;
    extendedStreamInfo.format = 0;
    extendedStreamInfo.dataspace = 0;

    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->streamInfoCapture->extendedStreamInfos = {extendedStreamInfo};
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: Quick Thumbnail  
 * @tc.desc: Quick Thumbnail StreamIntent = PREVIEW
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_QuickThumbnail_0500, TestSize.Level3)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetStreamOperator_V1_1(
        cameraTest->streamOperatorCallback, cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // capture extended streamInfo
    OHOS::HDI::Camera::V1_1::ExtendedStreamInfo extendedStreamInfo;
    extendedStreamInfo.type = OHOS::HDI::Camera::V1_1::EXTENDED_STREAM_INFO_QUICK_THUMBNAIL;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer2 =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    extendedStreamInfo.bufferQueue = consumer2->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(105, "yuv", addr, 40);
    });
    EXPECT_NE(extendedStreamInfo.bufferQueue, nullptr);
    EXPECT_NE(extendedStreamInfo.bufferQueue->producer_, nullptr);
    extendedStreamInfo.bufferQueue->producer_->SetQueueSize(UT_DATA_SIZE);
    // quikThumbnial do not need these param
    extendedStreamInfo.width = 0;
    extendedStreamInfo.height = 0;
    extendedStreamInfo.format = 0;
    extendedStreamInfo.dataspace = 0;

    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->streamInfoCapture->extendedStreamInfos = {extendedStreamInfo};
    cameraTest->DefaultCapture(cameraTest->streamInfoCapture);
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer_capture = std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfoCapture->v1_0.bufferQueue_ = consumer_capture->CreateProducerSeq([this](void* addr, uint32_t size) {
    cameraTest->DumpImageFile(cameraTest->streamIdCapture, "jpeg", addr, size);
    });
    cameraTest->streamInfoCapture->v1_0.bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[StreamIntent::PREVIEW] = consumer_capture;

    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: Quick Thumbnail  
 * @tc.desc: Quick Thumbnail EXTENDED_STREAM_INFO_QUICK_THUMBNAIL = 1
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_Driver_Camera_QuickThumbnail_0600, TestSize.Level3)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetStreamOperator_V1_1(
        cameraTest->streamOperatorCallback, cameraTest->streamOperator_V1_1);
    EXPECT_NE(cameraTest->streamOperator_V1_1, nullptr);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    // preview streamInfo
    cameraTest->streamInfoV1_1 = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->DefaultInfosPreview(cameraTest->streamInfoV1_1);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoV1_1);

    // capture extended streamInfo
    OHOS::HDI::Camera::V1_1::ExtendedStreamInfo extendedStreamInfo;
    extendedStreamInfo.type = static_cast<OHOS::HDI::Camera::V1_1::ExtendedStreamInfoType>(1);
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer2 =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    extendedStreamInfo.bufferQueue = consumer2->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(105, "yuv", addr, 40);
    });
    EXPECT_NE(extendedStreamInfo.bufferQueue, nullptr);
    EXPECT_NE(extendedStreamInfo.bufferQueue->producer_, nullptr);
    extendedStreamInfo.bufferQueue->producer_->SetQueueSize(UT_DATA_SIZE);
    // quikThumbnial do not need these param
    extendedStreamInfo.width = 0;
    extendedStreamInfo.height = 0;
    extendedStreamInfo.format = 0;
    extendedStreamInfo.dataspace = 0;

    // capture streamInfo
    cameraTest->streamInfoCapture = std::make_shared<OHOS::HDI::Camera::V1_1::StreamInfo_V1_1>();
    cameraTest->streamInfoCapture->extendedStreamInfos = {extendedStreamInfo};
    cameraTest->DefaultInfosCapture(cameraTest->streamInfoCapture);
    cameraTest->streamInfosV1_1.push_back(*cameraTest->streamInfoCapture);

    cameraTest->rc = cameraTest->streamOperator_V1_1->CreateStreams_V1_1(cameraTest->streamInfosV1_1);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    cameraTest->rc = cameraTest->streamOperator_V1_1->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->StartCapture(cameraTest->streamIdCapture, cameraTest->captureIdCapture, false, false);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}