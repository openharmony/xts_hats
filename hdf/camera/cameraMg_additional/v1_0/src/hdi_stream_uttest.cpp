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