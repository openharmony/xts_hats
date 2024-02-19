/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
#include "hdi_stream_uttest.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

void HdiStreamUtTestAdditional::SetUpTestCase(void) {}
void HdiStreamUtTestAdditional::TearDownTestCase(void) {}
void HdiStreamUtTestAdditional::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
}

void HdiStreamUtTestAdditional::TearDown(void) { cameraTest->Close(); }

/**
 * @tc.number : SUB_Driver_Camera_CancelCapture_0100
 * @tc.name   : testCancelCapture001
 * @tc.desc   : CancelCapture captureId = 0, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCancelCapture001, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, true, true);
    cameraTest->rc = cameraTest->streamOperator->CancelCapture(0);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number : SUB_Driver_Camera_CancelCapture_0200
 * @tc.name   : testCancelCapture002
 * @tc.desc   : CancelCapture captureId = 2147483647, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCancelCapture002, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, true, true);
    cameraTest->rc = cameraTest->streamOperator->CancelCapture(2147483647);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number : SUB_Driver_Camera_CancelCapture_0300
 * @tc.name   : testCancelCapture003
 * @tc.desc   : Do not create stream, call the CancelCapture function directly, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCancelCapture003, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    int captureId = 10;
    cameraTest->rc = cameraTest->streamOperator->CancelCapture(captureId);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    cameraTest->captureIds = {};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number : SUB_Driver_Camera_CancelCapture_0400
 * @tc.name   : testCancelCapture004
 * @tc.desc   : CancelCapture stability test, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCancelCapture004, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    for (int i = 0; i < 10; i++) {
        cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, true, true);
        cameraTest->rc = cameraTest->streamOperator->CancelCapture(cameraTest->captureIdPreview);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
}

/**
 * @tc.number : SUB_Driver_Camera_ReleaseStreams_0100
 * @tc.name   : testReleaseStreams001
 * @tc.desc   : Do not create stream, call the ReleaseStreams function directly, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testReleaseStreams001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({100});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_ReleaseStreams_0200
 * @tc.name   : testReleaseStreams002
 * @tc.desc   : ReleaseStreams stability test, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testReleaseStreams002, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    for (int i = 0; i < 10; i++) {
        cameraTest->StartStream(cameraTest->intents);
        cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_0100
 * @tc.name   : testIsStreamsSupported001
 * @tc.desc   : IsStreamsSupported, StreamInfo->streamId = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported001, Function | MediumTest | Level2)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = -1;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_0200
 * @tc.name   : testIsStreamsSupported002
 * @tc.desc   : IsStreamsSupported, StreamInfo->streamId = 2147483647, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported002, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 2147483647;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_0300
 * @tc.name   : testIsStreamsSupported003
 * @tc.desc   : IsStreamsSupported, StreamInfo->format = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported003, Function | MediumTest | Level2)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = -1;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_0400
 * @tc.name   : testIsStreamsSupported004
 * @tc.desc   : IsStreamsSupported, StreamInfo->format = 2147483647, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported004, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = 2147483647;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_0500
 * @tc.name   : testIsStreamsSupported005
 * @tc.desc   : IsStreamsSupported, StreamInfo->width = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported005, Function | MediumTest | Level2)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = -1;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_0600
 * @tc.name   : testIsStreamsSupported006
 * @tc.desc   : IsStreamsSupported, StreamInfo->width = 2147483647, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported006, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 2147483647;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_0700
 * @tc.name   : testIsStreamsSupported007
 * @tc.desc   : IsStreamsSupported, StreamInfo->height = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported007, Function | MediumTest | Level2)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = -1;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_0800
 * @tc.name   : testIsStreamsSupported008
 * @tc.desc   : IsStreamsSupported, StreamInfo->height = 2147483647, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported008, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 2147483647;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_0900
 * @tc.name   : testIsStreamsSupported009
 * @tc.desc   : IsStreamsSupported, Add AWB in modeSetting, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported009, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(100, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_AUTO;
    modeSetting->addEntry(OHOS_CONTROL_AWB_MODE, &awbMode, 1);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 2147483647;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1000
 * @tc.name   : testIsStreamsSupported010
 * @tc.desc   : IsStreamsSupported, StreamInfo->dataspace = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported010, Function | MediumTest | Level2)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = -1;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1100
 * @tc.name   : testIsStreamsSupported011
 * @tc.desc   : IsStreamsSupported, StreamInfo->dataspace = 2147483647, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported011, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 2147483647;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1200
 * @tc.name   : testIsStreamsSupported012
 * @tc.desc   : IsStreamsSupported, StreamInfo->intent = VIDEO, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported012, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = VIDEO;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1300
 * @tc.name   : testIsStreamsSupported013
 * @tc.desc   : IsStreamsSupported, stability test, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported013, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    for (int i = 0; i < 1000; i++) {
        cameraTest->rc =
            cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1400
 * @tc.name   : testIsStreamsSupported014
 * @tc.desc   : IsStreamsSupported, StreamInfo->tunneledMode = 0, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported014, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 0;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1500
 * @tc.name   : testIsStreamsSupported015
 * @tc.desc   : IsStreamsSupported, StreamInfo->intent = STILL_CAPTURE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported015, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = STILL_CAPTURE;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1600
 * @tc.name   : testIsStreamsSupported016
 * @tc.desc   : IsStreamsSupported, StreamInfo->intent = POST_VIEW, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported016, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = POST_VIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1700
 * @tc.name   : testIsStreamsSupported017
 * @tc.desc   : IsStreamsSupported, StreamInfo->intent = ANALYZE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported017, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = ANALYZE;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1800
 * @tc.name   : testIsStreamsSupported018
 * @tc.desc   : IsStreamsSupported, StreamInfo->intent = CUSTOM, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported018, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = CUSTOM;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_1900
 * @tc.name   : testIsStreamsSupported019
 * @tc.desc   : IsStreamsSupported, StreamInfo->streamId,format,height,width,dataspace = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported019, Function | MediumTest | Level2)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = -1;
    cameraTest->streamInfo->format_ = -1;
    cameraTest->streamInfo->height_ = -1;
    cameraTest->streamInfo->width_ = -1;
    cameraTest->streamInfo->dataspace_ = -1;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_2000
 * @tc.name   : testIsStreamsSupported020
 * @tc.desc   : IsStreamsSupported, StreamInfo->streamId,format,height,width,dataspace = 2147483647, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported020, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 2147483647;
    cameraTest->streamInfo->format_ = 2147483647;
    cameraTest->streamInfo->height_ = 2147483647;
    cameraTest->streamInfo->width_ = 2147483647;
    cameraTest->streamInfo->dataspace_ = 2147483647;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_2100
 * @tc.name   : testIsStreamsSupported021
 * @tc.desc   : IsStreamsSupported, StreamInfo->streamId = -1,format,height,width,dataspace = 2147483647, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported021, Function | MediumTest | Level2)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = -1;
    cameraTest->streamInfo->format_ = 2147483647;
    cameraTest->streamInfo->height_ = 2147483647;
    cameraTest->streamInfo->width_ = 2147483647;
    cameraTest->streamInfo->dataspace_ = 2147483647;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_2200
 * @tc.name   : testIsStreamsSupported022
 * @tc.desc   : IsStreamsSupported, StreamInfo->streamId = 2147483647,format,height,width,dataspace = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported022, Function | MediumTest | Level2)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    int64_t colorGains[4] = {0};
    modeSetting->addEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &colorGains, 4);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 2147483647;
    cameraTest->streamInfo->format_ = -1;
    cameraTest->streamInfo->height_ = -1;
    cameraTest->streamInfo->width_ = -1;
    cameraTest->streamInfo->dataspace_ = -1;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_2300
 * @tc.name   : testIsStreamsSupported023
 * @tc.desc   : IsStreamsSupported, OHOS_CONTROL_AE_EXPOSURE_COMPENSATION, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported023, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int32_t expo = 0xa0;
    modeSetting->addEntry(OHOS_CONTROL_AE_EXPOSURE_COMPENSATION, &expo, 1);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_2400
 * @tc.name   : testIsStreamsSupported024
 * @tc.desc   : IsStreamsSupported, OHOS_CAMERA_STREAM_ID, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported024, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    const int32_t deviceStreamId = 0;
    modeSetting->addEntry(OHOS_CAMERA_STREAM_ID, &deviceStreamId, 1);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_IsStreamsSupported_2500
 * @tc.name   : testIsStreamsSupported025
 * @tc.desc   : IsStreamsSupported, OHOS_CONTROL_AE_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testIsStreamsSupported025, Function | MediumTest | Level1)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t aeMode = OHOS_CAMERA_AE_MODE_ON;
    modeSetting->addEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->dataspace_ = 8;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;
    StreamSupportType pType;
    std::vector<StreamInfo> streams;
    streams.push_back(*cameraTest->streamInfo);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);
    cameraTest->rc =
        cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec, streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CreateStreams_0200
 * @tc.name   : testCreateStreams002
 * @tc.desc   : CreateStreams, StreamInfo->format = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCreateStreams002, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = -1;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_CreateStreams_0300
 * @tc.name   : testCreateStreams003
 * @tc.desc   : CreateStreams, StreamInfo->dataspace = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCreateStreams003, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = -1;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_CreateStreams_0400
 * @tc.name   : testCreateStreams004
 * @tc.desc   : CreateStreams, StreamInfo->tunneledMode = 0, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCreateStreams004, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 0;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_CreateStreams_0500
 * @tc.name   : testCreateStreams005
 * @tc.desc   : CreateStreams, StreamInfo->streamId,format,height,width,dataspace = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCreateStreams005, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = -1;
    cameraTest->streamInfo->width_ = -1;
    cameraTest->streamInfo->height_ = -1;
    cameraTest->streamInfo->format_ = -1;
    cameraTest->streamInfo->dataspace_ = -1;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_CreateStreams_0600
 * @tc.name   : testCreateStreams006
 * @tc.desc   : CreateStreams, StreamInfo->streamId,format,height,width,dataspace = 2147483647, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCreateStreams006, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 2147483647;
    cameraTest->streamInfo->width_ = 2147483647;
    cameraTest->streamInfo->height_ = 2147483647;
    cameraTest->streamInfo->format_ = 2147483647;
    cameraTest->streamInfo->dataspace_ = 2147483647;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    std::vector<int> streamIds;
    streamIds.push_back(cameraTest->streamInfo->streamId_);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams(streamIds);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CreateStreams_0700
 * @tc.name   : testCreateStreams007
 * @tc.desc   : CreateStreams, StreamInfo->streamId = -1,format,height,width,dataspace = 2147483647, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCreateStreams007, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = -1;
    cameraTest->streamInfo->width_ = 2147483647;
    cameraTest->streamInfo->height_ = 2147483647;
    cameraTest->streamInfo->format_ = 2147483647;
    cameraTest->streamInfo->dataspace_ = 2147483647;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_CreateStreams_0800
 * @tc.name   : testCreateStreams008
 * @tc.desc   : CreateStreams, StreamInfo->streamId = 2147483647,format,height,width,dataspace = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCreateStreams008, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 2147483647;
    cameraTest->streamInfo->width_ = -1;
    cameraTest->streamInfo->height_ = -1;
    cameraTest->streamInfo->format_ = -1;
    cameraTest->streamInfo->dataspace_ = -1;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_CreateStreams_0900
 * @tc.name   : testCreateStreams009
 * @tc.desc   : CreateStreams, stability test, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCreateStreams009, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

        std::vector<int> streamIds;
        streamIds.push_back(cameraTest->streamInfo->streamId_);
        cameraTest->rc = cameraTest->streamOperator->ReleaseStreams(streamIds);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_AttachBufferQueue_0100
 * @tc.name   : testAttachBufferQueue001
 * @tc.desc   : AttachBufferQueue, StreamInfo->streamId_ = 2147483647, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testAttachBufferQueue001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101; // PREVIEW streamId
    cameraTest->streamInfo->width_ = 720;    // Pixel Width
    cameraTest->streamInfo->height_ = 480;   // Pixel height
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);

    cameraTest->streamInfoSnapshot = std::make_shared<StreamInfo>();
    cameraTest->streamInfoSnapshot->streamId_ = 2147483647; // STILL_CAPTURE streamId
    cameraTest->streamInfoSnapshot->width_ = 720;           // Pixel Width
    cameraTest->streamInfoSnapshot->height_ = 480;          // Pixel height
    cameraTest->streamInfoSnapshot->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfoSnapshot->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfoSnapshot->intent_ = STILL_CAPTURE;
    cameraTest->streamInfoSnapshot->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> snapshotConsumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfoSnapshot->bufferQueue_ =
        snapshotConsumer->CreateProducerSeq([this](void *addr, uint32_t size) {
            cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
        });

    cameraTest->streamInfoSnapshot->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfoSnapshot->intent_] = snapshotConsumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfoSnapshot);

    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> preview_consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    OHOS::sptr<OHOS::IBufferProducer> producerTemp =
        preview_consumer->CreateProducer([this](void *addr, uint32_t size) {
            cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
        });

    OHOS::sptr<BufferProducerSequenceable> bufferQueue = new BufferProducerSequenceable(producerTemp);
    cameraTest->rc =
        cameraTest->streamOperator->AttachBufferQueue(cameraTest->streamInfoSnapshot->streamId_, bufferQueue);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->DetachBufferQueue(cameraTest->streamInfoSnapshot->streamId_);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    std::vector<int> streamIds = {cameraTest->streamInfo->streamId_, cameraTest->streamInfoSnapshot->streamId_};
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams(streamIds);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_DetachBufferQueue_0100
 * @tc.name   : testDetachBufferQueue001
 * @tc.desc   : DetachBufferQueue, params = 101, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testDetachBufferQueue001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101; // PREVIEW streamId
    cameraTest->streamInfo->width_ = 720;    // Pixel Width
    cameraTest->streamInfo->height_ = 480;   // Pixel height
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);

    cameraTest->streamInfoSnapshot = std::make_shared<StreamInfo>();
    cameraTest->streamInfoSnapshot->streamId_ = 102; // STILL_CAPTURE streamId
    cameraTest->streamInfoSnapshot->width_ = 720;    // Pixel Width
    cameraTest->streamInfoSnapshot->height_ = 480;   // Pixel height
    cameraTest->streamInfoSnapshot->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfoSnapshot->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfoSnapshot->intent_ = STILL_CAPTURE;
    cameraTest->streamInfoSnapshot->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> snapshotConsumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfoSnapshot->bufferQueue_ =
        snapshotConsumer->CreateProducerSeq([this](void *addr, uint32_t size) {
            cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
        });

    cameraTest->streamInfoSnapshot->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfoSnapshot->intent_] = snapshotConsumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfoSnapshot);

    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> preview_consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    OHOS::sptr<OHOS::IBufferProducer> producerTemp =
        preview_consumer->CreateProducer([this](void *addr, uint32_t size) {
            cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
        });

    OHOS::sptr<BufferProducerSequenceable> bufferQueue = new BufferProducerSequenceable(producerTemp);
    cameraTest->rc =
        cameraTest->streamOperator->AttachBufferQueue(cameraTest->streamInfoSnapshot->streamId_, bufferQueue);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->DetachBufferQueue(cameraTest->streamInfo->streamId_);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    std::vector<int> streamIds = {cameraTest->streamInfo->streamId_, cameraTest->streamInfoSnapshot->streamId_};
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams(streamIds);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_ChangeToOfflineStream_0100
 * @tc.name   : testChangeToOfflineStream001
 * @tc.desc   : ChangeToOfflineStream, StreamInfo->streamId_ = 2147483647, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testChangeToOfflineStream001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 720;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });
    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);

    cameraTest->streamInfoSnapshot = std::make_shared<StreamInfo>();
    cameraTest->streamInfoSnapshot->streamId_ = 2147483647;
    cameraTest->streamInfoSnapshot->width_ = 720;
    cameraTest->streamInfoSnapshot->height_ = 480;
    cameraTest->streamInfoSnapshot->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfoSnapshot->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfoSnapshot->intent_ = STILL_CAPTURE;
    cameraTest->streamInfoSnapshot->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> snapshotConsumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfoSnapshot->bufferQueue_ =
        snapshotConsumer->CreateProducerSeq([this](void *addr, uint32_t size) {
            cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
        });
    cameraTest->streamInfoSnapshot->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfoSnapshot->intent_] = snapshotConsumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfoSnapshot);

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    cameraTest->rc =
        (CamRetCode)cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    int captureId = 2001;
    cameraTest->captureInfo = std::make_shared<CaptureInfo>();
    cameraTest->captureInfo->streamIds_ = {2147483647};
    cameraTest->captureInfo->captureSetting_ = cameraTest->abilityVec;
    cameraTest->captureInfo->enableShutterCallback_ = true;
    bool isStreaming = true;
    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->Capture(captureId, *cameraTest->captureInfo, isStreaming);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    OHOS::sptr<IStreamOperatorCallback> streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    OHOS::sptr<IOfflineStreamOperator> offlineStreamOperator = nullptr;

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->ChangeToOfflineStream(
        {cameraTest->streamInfoSnapshot->streamId_}, streamOperatorCallback, offlineStreamOperator);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    sleep(UT_SECOND_TIMES);

    cameraTest->rc = (CamRetCode)offlineStreamOperator->CancelCapture(captureId);

    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    std::vector<int> streamIds = {2147483647};
    cameraTest->rc = (CamRetCode)offlineStreamOperator->ReleaseStreams(streamIds);
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = (CamRetCode)offlineStreamOperator->Release();
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_Capture_0100
 * @tc.name   : testCapture001
 * @tc.desc   : Capture, streamIds_ = -1, captureId = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCapture001, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    int captureId = -1;
    cameraTest->captureInfo = std::make_shared<CaptureInfo>();
    cameraTest->captureInfo->streamIds_ = {-1};
    cameraTest->captureInfo->captureSetting_ = cameraTest->abilityVec;
    cameraTest->captureInfo->enableShutterCallback_ = true;
    cameraTest->rc = cameraTest->streamOperator->Capture(captureId, *cameraTest->captureInfo, true);
    EXPECT_EQ(HDI::Camera::V1_0::INVALID_ARGUMENT, cameraTest->rc);
    sleep(1);
    cameraTest->streamOperator->CancelCapture(captureId);
    cameraTest->captureIds = {};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.number : SUB_Driver_Camera_CancelCapture_0500
 * @tc.name   : testCancelCapture005
 * @tc.desc   : CancelCapture, captureId = 2147483647, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testCancelCapture005, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 720;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });
    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);

    cameraTest->streamInfoSnapshot = std::make_shared<StreamInfo>();
    cameraTest->streamInfoSnapshot->streamId_ = 102;
    cameraTest->streamInfoSnapshot->width_ = 720;
    cameraTest->streamInfoSnapshot->height_ = 480;
    cameraTest->streamInfoSnapshot->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfoSnapshot->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfoSnapshot->intent_ = STILL_CAPTURE;
    cameraTest->streamInfoSnapshot->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> snapshotConsumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfoSnapshot->bufferQueue_ =
        snapshotConsumer->CreateProducerSeq([this](void *addr, uint32_t size) {
            cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
        });
    cameraTest->streamInfoSnapshot->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfoSnapshot->intent_] = snapshotConsumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfoSnapshot);

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    cameraTest->rc =
        (CamRetCode)cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    int captureId = 2147483647;
    cameraTest->captureInfo = std::make_shared<CaptureInfo>();
    cameraTest->captureInfo->streamIds_ = {102};
    cameraTest->captureInfo->captureSetting_ = cameraTest->abilityVec;
    cameraTest->captureInfo->enableShutterCallback_ = true;
    bool isStreaming = true;
    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->Capture(captureId, *cameraTest->captureInfo, isStreaming);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    OHOS::sptr<IStreamOperatorCallback> streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    OHOS::sptr<IOfflineStreamOperator> offlineStreamOperator = nullptr;

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->ChangeToOfflineStream(
        {cameraTest->streamInfoSnapshot->streamId_}, streamOperatorCallback, offlineStreamOperator);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    sleep(UT_SECOND_TIMES);

    cameraTest->rc = (CamRetCode)offlineStreamOperator->CancelCapture(captureId);

    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    std::vector<int> streamIds = {102};
    cameraTest->rc = (CamRetCode)offlineStreamOperator->ReleaseStreams(streamIds);
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = (CamRetCode)offlineStreamOperator->Release();
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_ReleaseStreams_0300
 * @tc.name   : testReleaseStreams003
 * @tc.desc   : ReleaseStreams, params = 101, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testReleaseStreams003, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 720;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });
    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);

    cameraTest->streamInfoSnapshot = std::make_shared<StreamInfo>();
    cameraTest->streamInfoSnapshot->streamId_ = 102;
    cameraTest->streamInfoSnapshot->width_ = 720;
    cameraTest->streamInfoSnapshot->height_ = 480;
    cameraTest->streamInfoSnapshot->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfoSnapshot->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfoSnapshot->intent_ = STILL_CAPTURE;
    cameraTest->streamInfoSnapshot->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> snapshotConsumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfoSnapshot->bufferQueue_ =
        snapshotConsumer->CreateProducerSeq([this](void *addr, uint32_t size) {
            cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
        });
    cameraTest->streamInfoSnapshot->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfoSnapshot->intent_] = snapshotConsumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfoSnapshot);

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    cameraTest->rc =
        (CamRetCode)cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    int captureId = 2001;
    cameraTest->captureInfo = std::make_shared<CaptureInfo>();
    cameraTest->captureInfo->streamIds_ = {102};
    cameraTest->captureInfo->captureSetting_ = cameraTest->abilityVec;
    cameraTest->captureInfo->enableShutterCallback_ = true;
    bool isStreaming = true;
    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->Capture(captureId, *cameraTest->captureInfo, isStreaming);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
    OHOS::sptr<IStreamOperatorCallback> streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    OHOS::sptr<IOfflineStreamOperator> offlineStreamOperator = nullptr;

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->ChangeToOfflineStream(
        {cameraTest->streamInfoSnapshot->streamId_}, streamOperatorCallback, offlineStreamOperator);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    sleep(UT_SECOND_TIMES);

    cameraTest->rc = (CamRetCode)offlineStreamOperator->CancelCapture(captureId);

    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    std::vector<int> streamIds = {102};
    cameraTest->rc = (CamRetCode)offlineStreamOperator->ReleaseStreams(streamIds);
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = (CamRetCode)offlineStreamOperator->Release();
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->ReleaseStreams({101});
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
}

/**
 * @tc.number : SUB_Driver_Camera_Release_0100
 * @tc.name   : testRelease001
 * @tc.desc   : Release, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testRelease001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 720;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });
    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);

    cameraTest->streamInfoSnapshot = std::make_shared<StreamInfo>();
    cameraTest->streamInfoSnapshot->streamId_ = 102;
    cameraTest->streamInfoSnapshot->width_ = 720;
    cameraTest->streamInfoSnapshot->height_ = 480;
    cameraTest->streamInfoSnapshot->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfoSnapshot->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfoSnapshot->intent_ = STILL_CAPTURE;
    cameraTest->streamInfoSnapshot->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> snapshotConsumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfoSnapshot->bufferQueue_ =
        snapshotConsumer->CreateProducerSeq([this](void *addr, uint32_t size) {
            cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
        });
    cameraTest->streamInfoSnapshot->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfoSnapshot->intent_] = snapshotConsumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfoSnapshot);

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    cameraTest->rc =
        (CamRetCode)cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    OHOS::sptr<IStreamOperatorCallback> streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    OHOS::sptr<IOfflineStreamOperator> offlineStreamOperator = nullptr;

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->ChangeToOfflineStream(
        {cameraTest->streamInfoSnapshot->streamId_}, streamOperatorCallback, offlineStreamOperator);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    std::vector<int> streamIds = {102};
    cameraTest->rc = (CamRetCode)offlineStreamOperator->ReleaseStreams(streamIds);
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = (CamRetCode)offlineStreamOperator->Release();
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_AttachBufferQueue_0200
 * @tc.name   : testAttachBufferQueue002
 * @tc.desc   : AttachBufferQueue, StreamInfo->streamId_ = -1, return error
 */
HWTEST_F(HdiStreamUtTestAdditional, testAttachBufferQueue002, Function | MediumTest | Level2)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc =
        cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback, cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = -1;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void *addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> preview_consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    OHOS::sptr<OHOS::IBufferProducer> producerTemp =
        preview_consumer->CreateProducer([this](void *addr, uint32_t size) {
            cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
        });
    OHOS::sptr<BufferProducerSequenceable> bufferQueue = new BufferProducerSequenceable(producerTemp);
    cameraTest->rc = cameraTest->streamOperator->AttachBufferQueue(cameraTest->streamInfo->streamId_, bufferQueue);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_0100
 * @tc.name   : testCommitStreams001
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AE_EXPOSURE_COMPENSATION, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int32_t expo = 0xa0;
    modeSetting->addEntry(OHOS_CONTROL_AE_EXPOSURE_COMPENSATION, &expo, 1);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_0200
 * @tc.name   : testCommitStreams002
 * @tc.desc   : CommitStreams, OHOS_ABILITY_CAMERA_POSITION, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams002, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t abilityCameraPosition = OHOS_CAMERA_POSITION_FRONT;
    modeSetting->addEntry(OHOS_ABILITY_CAMERA_POSITION, &abilityCameraPosition, 1);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_0300
 * @tc.name   : testCommitStreams003
 * @tc.desc   : CommitStreams, OHOS_ABILITY_CAMERA_TYPE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams003, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t abilityCameraType = OHOS_CAMERA_TYPE_WIDE_ANGLE;
    modeSetting->addEntry(OHOS_ABILITY_CAMERA_TYPE, &abilityCameraType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_0400
 * @tc.name   : testCommitStreams004
 * @tc.desc   : CommitStreams, OHOS_ABILITY_CAMERA_CONNECTION_TYPE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams004, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t abilityCameraConnType = OHOS_CAMERA_CONNECTION_TYPE_BUILTIN;
    modeSetting->addEntry(OHOS_ABILITY_CAMERA_CONNECTION_TYPE, &abilityCameraConnType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_0500
 * @tc.name   : testCommitStreams005
 * @tc.desc   : CommitStreams, OHOS_CONTROL_EXPOSUREMODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams005, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t exposureMode = OHOS_CAMERA_EXPOSURE_MODE_CONTINUOUS_AUTO;
    modeSetting->addEntry(OHOS_CONTROL_EXPOSUREMODE, &exposureMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_0600
 * @tc.name   : testCommitStreams006
 * @tc.desc   : CommitStreams, OHOS_CONTROL_FOCUS_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams006, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t focusMode = OHOS_CAMERA_FOCUS_MODE_AUTO;
    modeSetting->addEntry(OHOS_CONTROL_FOCUS_MODE, &focusMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_0700
 * @tc.name   : testCommitStreams007
 * @tc.desc   : CommitStreams, OHOS_CONTROL_FLASHMODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams007, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t flashMode = OHOS_CAMERA_FLASH_MODE_OPEN;
    modeSetting->addEntry(OHOS_CONTROL_FLASHMODE, &flashMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_0800
 * @tc.name   : testCommitStreams008
 * @tc.desc   : CommitStreams, OHOS_CONTROL_FLASH_STATE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams008, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t flashState = OHOS_CAMERA_FLASH_STATE_CHARGING;
    modeSetting->addEntry(OHOS_CONTROL_FLASH_STATE, &flashState, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_0900
 * @tc.name   : testCommitStreams009
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AE_ANTIBANDING_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams009, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t aeAntibandingMode = OHOS_CAMERA_AE_ANTIBANDING_MODE_50HZ;
    modeSetting->addEntry(OHOS_CONTROL_AE_ANTIBANDING_MODE, &aeAntibandingMode, 1);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1000
 * @tc.name   : testCommitStreams010
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AE_LOCK, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams010, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t aeLock = OHOS_CAMERA_AE_LOCK_ON;
    modeSetting->addEntry(OHOS_CONTROL_AE_LOCK, &aeLock, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1100
 * @tc.name   : testCommitStreams011
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AE_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams011, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t aeMode = OHOS_CAMERA_AE_MODE_ON;
    modeSetting->addEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1200
 * @tc.name   : testCommitStreams012
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AF_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams012, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t afMode = OHOS_CAMERA_AF_MODE_AUTO;
    modeSetting->addEntry(OHOS_CONTROL_AF_MODE, &afMode, 1);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1300
 * @tc.name   : testCommitStreams013
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AF_TRIGGER, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams013, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t afTrigger = OHOS_CAMERA_AF_TRIGGER_IDLE;
    modeSetting->addEntry(OHOS_CONTROL_AF_TRIGGER, &afTrigger, 1);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1400
 * @tc.name   : testCommitStreams014
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AF_STATE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams014, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t afState = OHOS_CAMERA_AF_STATE_PASSIVE_SCAN;
    modeSetting->addEntry(OHOS_CONTROL_AF_STATE, &afState, 1);
    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1500
 * @tc.name   : testCommitStreams015
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AWB_LOCK, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams015, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t awbLock = OHOS_CAMERA_AWB_LOCK_ON;
    modeSetting->addEntry(OHOS_CONTROL_AWB_LOCK, &awbLock, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1600
 * @tc.name   : testCommitStreams016
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AWB_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams016, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_AUTO;
    modeSetting->addEntry(OHOS_CONTROL_AWB_MODE, &awbMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1700
 * @tc.name   : testCommitStreams017
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AWB_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams017, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_INCANDESCENT;
    modeSetting->addEntry(OHOS_CONTROL_AWB_MODE, &awbMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1800
 * @tc.name   : testCommitStreams018
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AWB_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams018, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t awbMode = OHOS_CAMERA_AWB_MODE_FLUORESCENT;
    modeSetting->addEntry(OHOS_CONTROL_AWB_MODE, &awbMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_1900
 * @tc.name   : testCommitStreams019
 * @tc.desc   : CommitStreams, OHOS_STATISTICS_FACE_DETECT_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams019, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t faceDetectMode = OHOS_CAMERA_FACE_DETECT_MODE_SIMPLE;
    modeSetting->addEntry(OHOS_STATISTICS_FACE_DETECT_MODE, &faceDetectMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2000
 * @tc.name   : testCommitStreams020
 * @tc.desc   : CommitStreams, OHOS_STATISTICS_HISTOGRAM_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams020, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t histogramMode = OHOS_CAMERA_HISTOGRAM_MODE_ON;
    modeSetting->addEntry(OHOS_STATISTICS_HISTOGRAM_MODE, &histogramMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2100
 * @tc.name   : testCommitStreams021
 * @tc.desc   : CommitStreams, OHOS_STREAM_AVAILABLE_FORMATS, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams021, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t format = OHOS_CAMERA_FORMAT_RGBA_8888;
    modeSetting->addEntry(OHOS_STREAM_AVAILABLE_FORMATS, &format, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2200
 * @tc.name   : testCommitStreams022
 * @tc.desc   : CommitStreams, OHOS_ABILITY_STREAM_QUICK_THUMBNAIL_AVAILABLE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams022, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t quickThumbnailAvailable = OHOS_CAMERA_QUICK_THUMBNAIL_TRUE;
    modeSetting->addEntry(OHOS_ABILITY_STREAM_QUICK_THUMBNAIL_AVAILABLE, &quickThumbnailAvailable, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2300
 * @tc.name   : testCommitStreams023
 * @tc.desc   : CommitStreams, OHOS_CONTROL_FOCUS_STATE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams023, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t focusState = OHOS_CAMERA_FOCUS_STATE_FOCUSED;
    modeSetting->addEntry(OHOS_CONTROL_FOCUS_STATE, &focusState, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2400
 * @tc.name   : testCommitStreams024
 * @tc.desc   : CommitStreams, OHOS_CONTROL_METER_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams024, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t meterMode = OHOS_CAMERA_SPOT_METERING;
    modeSetting->addEntry(OHOS_CONTROL_METER_MODE, &meterMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2500
 * @tc.name   : testCommitStreams025
 * @tc.desc   : CommitStreams, OHOS_CONTROL_CAPTURE_MIRROR, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams025, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t mirror = OHOS_CAMERA_MIRROR_ON;
    modeSetting->addEntry(OHOS_CONTROL_CAPTURE_MIRROR, &mirror, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2600
 * @tc.name   : testCommitStreams026
 * @tc.desc   : CommitStreams, OHOS_JPEG_ORIENTATION, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams026, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t jpegOrientation = OHOS_CAMERA_JPEG_ROTATION_90;
    modeSetting->addEntry(OHOS_JPEG_ORIENTATION, &jpegOrientation, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2700
 * @tc.name   : testCommitStreams027
 * @tc.desc   : CommitStreams, OHOS_JPEG_QUALITY, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams027, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t jpegQuality = OHOS_CAMERA_JPEG_LEVEL_MIDDLE;
    modeSetting->addEntry(OHOS_JPEG_QUALITY, &jpegQuality, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2800
 * @tc.name   : testCommitStreams028
 * @tc.desc   : CommitStreams, OHOS_ABILITY_VIDEO_STABILIZATION_MODES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams028, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t videoStabilizationMode = OHOS_CAMERA_VIDEO_STABILIZATION_LOW;
    modeSetting->addEntry(OHOS_ABILITY_VIDEO_STABILIZATION_MODES, &videoStabilizationMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_2900
 * @tc.name   : testCommitStreams029
 * @tc.desc   : CommitStreams, OHOS_ABILITY_FLASH_AVAILABLE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams029, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t flashAvailable = OHOS_CAMERA_FLASH_TRUE;
    modeSetting->addEntry(OHOS_ABILITY_FLASH_AVAILABLE, &flashAvailable, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3000
 * @tc.name   : testCommitStreams030
 * @tc.desc   : CommitStreams, OHOS_ABILITY_MEMORY_TYPE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams030, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t memoryType = OHOS_CAMERA_MEMORY_OVERLAY;
    modeSetting->addEntry(OHOS_ABILITY_MEMORY_TYPE, &memoryType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3100
 * @tc.name   : testCommitStreams031
 * @tc.desc   : CommitStreams, OHOS_ABILITY_MUTE_MODES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams031, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t muteMode = OHOS_CAMERA_MUTE_MODE_SOLID_COLOR_BLACK;
    modeSetting->addEntry(OHOS_ABILITY_MUTE_MODES, &muteMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3200
 * @tc.name   : testCommitStreams032
 * @tc.desc   : CommitStreams, OHOS_ABILITY_SCENE_FILTER_TYPES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams032, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t filterType = OHOS_CAMERA_FILTER_TYPE_CLASSIC;
    modeSetting->addEntry(OHOS_ABILITY_SCENE_FILTER_TYPES, &filterType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3300
 * @tc.name   : testCommitStreams033
 * @tc.desc   : CommitStreams, OHOS_ABILITY_SCENE_FILTER_TYPES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams033, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t filterType = OHOS_CAMERA_FILTER_TYPE_DAWN;
    modeSetting->addEntry(OHOS_ABILITY_SCENE_FILTER_TYPES, &filterType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3400
 * @tc.name   : testCommitStreams034
 * @tc.desc   : CommitStreams, OHOS_ABILITY_SCENE_FILTER_TYPES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams034, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t filterType = OHOS_CAMERA_FILTER_TYPE_PURE;
    modeSetting->addEntry(OHOS_ABILITY_SCENE_FILTER_TYPES, &filterType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3500
 * @tc.name   : testCommitStreams035
 * @tc.desc   : CommitStreams, OHOS_ABILITY_SCENE_PORTRAIT_EFFECT_TYPES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams035, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t portraitEffectType = OHOS_CAMERA_PORTRAIT_CIRCLES;
    modeSetting->addEntry(OHOS_ABILITY_SCENE_PORTRAIT_EFFECT_TYPES, &portraitEffectType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3600
 * @tc.name   : testCommitStreams036
 * @tc.desc   : CommitStreams, OHOS_ABILITY_SCENE_BEAUTY_TYPES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams036, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t beautyType = OHOS_CAMERA_BEAUTY_TYPE_AUTO;
    modeSetting->addEntry(OHOS_ABILITY_SCENE_BEAUTY_TYPES, &beautyType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3700
 * @tc.name   : testCommitStreams037
 * @tc.desc   : CommitStreams, OHOS_ABILITY_SCENE_BEAUTY_TYPES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams037, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t beautyType = OHOS_CAMERA_BEAUTY_TYPE_SKIN_SMOOTH;
    modeSetting->addEntry(OHOS_ABILITY_SCENE_BEAUTY_TYPES, &beautyType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3800
 * @tc.name   : testCommitStreams038
 * @tc.desc   : CommitStreams, OHOS_ABILITY_SUPPORTED_COLOR_MODES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams038, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t xmageColorType = CAMERA_CUSTOM_COLOR_BRIGHT;
    modeSetting->addEntry(OHOS_ABILITY_SUPPORTED_COLOR_MODES, &xmageColorType, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_3900
 * @tc.name   : testCommitStreams039
 * @tc.desc   : CommitStreams, OHOS_ABILITY_FOCUS_MODES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams039, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t focusMode = OHOS_CAMERA_FOCUS_MODE_AUTO;
    modeSetting->addEntry(OHOS_ABILITY_FOCUS_MODES, &focusMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_4000
 * @tc.name   : testCommitStreams040
 * @tc.desc   : CommitStreams, OHOS_ABILITY_FOCUS_MODES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams040, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t focusMode = OHOS_CAMERA_FOCUS_MODE_LOCKED;
    modeSetting->addEntry(OHOS_ABILITY_FOCUS_MODES, &focusMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}
/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_4100
 * @tc.name   : testCommitStreams041
 * @tc.desc   : CommitStreams, OHOS_CONTROL_AF_AVAILABLE_MODES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams041, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t afMode = OHOS_CAMERA_AF_MODE_MACRO;
    modeSetting->addEntry(OHOS_CONTROL_AF_AVAILABLE_MODES, &afMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_4200
 * @tc.name   : testCommitStreams042
 * @tc.desc   : CommitStreams, OHOS_STATISTICS_FACE_DETECT_SWITCH, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams042, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t faceDetectMode = OHOS_CAMERA_FACE_DETECT_MODE_SIMPLE;
    modeSetting->addEntry(OHOS_STATISTICS_FACE_DETECT_SWITCH, &faceDetectMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_4300
 * @tc.name   : testCommitStreams043
 * @tc.desc   : CommitStreams, OHOS_ABILITY_STREAM_AVAILABLE_BASIC_CONFIGURATIONS, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams043, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t format = OHOS_CAMERA_FORMAT_YCBCR_420_888;
    modeSetting->addEntry(OHOS_ABILITY_STREAM_AVAILABLE_BASIC_CONFIGURATIONS, &format, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_4400
 * @tc.name   : testCommitStreams044
 * @tc.desc   : CommitStreams, OHOS_ABILITY_METER_MODES, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams044, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t meterMode = OHOS_CAMERA_OVERALL_METERING;
    modeSetting->addEntry(OHOS_ABILITY_METER_MODES, &meterMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_4500
 * @tc.name   : testCommitStreams045
 * @tc.desc   : CommitStreams, OHOS_CONTROL_CAPTURE_MIRROR_SUPPORTED, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams045, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t mirror = OHOS_CAMERA_MIRROR_ON;
    modeSetting->addEntry(OHOS_CONTROL_CAPTURE_MIRROR_SUPPORTED, &mirror, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_CommitStreams_4600
 * @tc.name   : testCommitStreams046
 * @tc.desc   : CommitStreams, OHOS_CONTROL_VIDEO_STABILIZATION_MODE, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testCommitStreams046, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    uint8_t videoStabilizationMode = OHOS_CAMERA_VIDEO_STABILIZATION_LOW;
    modeSetting->addEntry(OHOS_CONTROL_VIDEO_STABILIZATION_MODE, &videoStabilizationMode, 1);

    std::vector<uint8_t> modeSettingVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, modeSettingVec);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, modeSettingVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.number : SUB_Driver_Camera_GetStreamAttributes_0100
 * @tc.name   : testGetStreamAttributes001
 * @tc.desc   : GetStreamAttributes, stability test, return success
 */
HWTEST_F(HdiStreamUtTestAdditional, testGetStreamAttributes001, Function | MediumTest | Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    for (int i = 0; i < 100; i++) {
        cameraTest->StartStream(cameraTest->intents);
        std::vector<StreamAttribute> attributes;
        cameraTest->rc = cameraTest->streamOperator->GetStreamAttributes(attributes);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
        cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}