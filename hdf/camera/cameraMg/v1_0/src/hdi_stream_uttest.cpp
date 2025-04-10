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

void HdiStreamUtTest::SetUpTestCase(void) {}
void HdiStreamUtTest::TearDownTestCase(void) {}
void HdiStreamUtTest::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
}

void HdiStreamUtTest::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.name: IsStreamSupported
 * @tc.desc: IsStreamSupported, normal cameraId
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_3300, TestSize.Level0)
{
    cameraTest->Open();
    EXPECT_EQ(false, cameraTest->cameraDevice == nullptr);
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
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
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
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
    cameraTest->rc = cameraTest->streamOperator->IsStreamsSupported(OperationMode::NORMAL, modeSettingVec,
        streams, pType);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
#if defined DISPLAY_COMMUNITY
    EXPECT_EQ(true, pType ==HDI::Camera::V1_0::DYNAMIC_SUPPORTED);
#else
    EXPECT_NE(true, pType ==HDI::Camera::V1_0::DYNAMIC_SUPPORTED);
#endif
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_3400, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
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
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
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
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->streamId = -1, return error
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_3500, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
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
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, INVALID_ARGUMENT);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->streamId = 2147483391, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_3600, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 2147483391;
    cameraTest->streamInfo->width_ = 640;
    cameraTest->streamInfo->height_ = 480;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->width = -1, return error
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_3700, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = -1;
    cameraTest->streamInfo->height_ = 640;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, INVALID_ARGUMENT);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->width = 2147483647, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_3800, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 2147483647;
    cameraTest->streamInfo->height_ = 640;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->height = -1, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_3900, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = -1;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, INVALID_ARGUMENT);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->height = 2147483647, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4000, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 2147483647;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->format = 2147483647, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4100, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = 2147483647;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->dataspace = 2147483647, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4200, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 2147483647;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->StreamIntent = PREVIEW, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4300, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->StreamIntent = VIDEO, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4400, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = VIDEO;
    cameraTest->streamInfo->tunneledMode_ = 5;
    cameraTest->streamInfo->encodeType_ = ENCODE_TYPE_H265;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->StreamIntent = STILL_CAPTURE, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4500, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = STILL_CAPTURE;
    cameraTest->streamInfo->tunneledMode_ = 5;
    cameraTest->streamInfo->encodeType_ = ENCODE_TYPE_NULL;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->StreamIntent = POST_VIEW, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4600, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = POST_VIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->StreamIntent = StreamIntent::ANALYZE, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4700, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = StreamIntent::ANALYZE;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: CreateStreams
 * @tc.desc: CreateStreams, StreamInfo->StreamIntent = PREVIEW, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4800, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamInfo->streamId_});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: startStream, stopStream
 * @tc.desc: startStream, stopStream
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_4900, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->captureIds = {};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: GetStreamAttributes
 * @tc.desc: GetStreamAttributes, success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_5000, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    std::vector<StreamAttribute> attributes;
    cameraTest->rc = cameraTest->streamOperator->GetStreamAttributes(attributes);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: capture
 * @tc.desc: capture, input normal
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_5100, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: capture
 * @tc.desc: preview, capture->captureInfo->streamId = -1, return error
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_5200, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    int captureId = 2001;
    cameraTest->captureInfo = std::make_shared<CaptureInfo>();
    cameraTest->captureInfo->streamIds_ = {-1};
    cameraTest->captureInfo->captureSetting_ = cameraTest->abilityVec;
    cameraTest->captureInfo->enableShutterCallback_ = true;
    cameraTest->rc = cameraTest->streamOperator->Capture(captureId, *cameraTest->captureInfo, true);
    EXPECT_EQ(INVALID_ARGUMENT, cameraTest->rc);
    sleep(1);
    cameraTest->streamOperator->CancelCapture(captureId);
    cameraTest->captureIds = {};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}


/**
 * @tc.name: capture
 * @tc.desc: preview, capture->captureInfo->streamId = 65535, return success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_5300, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 65535;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = cameraTest->previewFormat;
    cameraTest->streamInfo->dataspace_ = 8;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = 5;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ =  consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(8);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    int captureId = 2001;
    cameraTest->captureInfo = std::make_shared<CaptureInfo>();
    cameraTest->captureInfo->streamIds_ = {65535};
    cameraTest->captureInfo->captureSetting_ = cameraTest->abilityVec;
    cameraTest->captureInfo->enableShutterCallback_ =true;
    bool isStreaming = true;
    cameraTest->rc = cameraTest->streamOperator->Capture(captureId, *cameraTest->captureInfo, isStreaming);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    sleep(1);
    cameraTest->streamOperator->CancelCapture(captureId);

    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams(cameraTest->captureInfo->streamIds_);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->cameraDevice->Close();
}

/**
 * @tc.name: capture
 * @tc.desc: preview, capture->captureId = -1, return error
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_5400, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);

    cameraTest->captureInfo = std::make_shared<CaptureInfo>();
    cameraTest->captureInfo->streamIds_ = {cameraTest->streamIdPreview};
    cameraTest->captureInfo->captureSetting_ = cameraTest->abilityVec;
    cameraTest->captureInfo->enableShutterCallback_ = false;
    bool isStreaming = true;
    int captureId = -1;
    cameraTest->rc = cameraTest->streamOperator->Capture(captureId, *cameraTest->captureInfo, isStreaming);
    EXPECT_EQ(INVALID_ARGUMENT, cameraTest->rc);
    sleep(1);
    cameraTest->rc = cameraTest->streamOperator->CancelCapture(cameraTest->captureIdPreview);
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: capture
 * @tc.desc: preview, capture->captureInfo->enableShutterCallback = true, return success
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_5500, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, true, true);

    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: CancelCapture
 * @tc.desc: CancelCapture captureId = -1
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_5600, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, true, true);
    cameraTest->rc = cameraTest->streamOperator->CancelCapture(-1);
    EXPECT_EQ(INVALID_ARGUMENT, cameraTest->rc);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: AttachBufferQueue & DetachBufferQueue
 * @tc.desc: AttachBufferQueue & DetachBufferQueue
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_5800, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 101; // PREVIEW streamId
    cameraTest->streamInfo->width_ = 720; // Pixel Width
    cameraTest->streamInfo->height_ = 480; // Pixel height
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);

    cameraTest->streamInfoSnapshot = std::make_shared<StreamInfo>();
    cameraTest->streamInfoSnapshot->streamId_ = 102; // STILL_CAPTURE streamId
    cameraTest->streamInfoSnapshot->width_ = 720; // Pixel Width
    cameraTest->streamInfoSnapshot->height_ = 480; // Pixel height
    cameraTest->streamInfoSnapshot->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfoSnapshot->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfoSnapshot->intent_ = STILL_CAPTURE;
    cameraTest->streamInfoSnapshot->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> snapshotConsumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfoSnapshot->bufferQueue_ = snapshotConsumer->CreateProducerSeq([this](void* addr,
        uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    cameraTest->streamInfoSnapshot->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfoSnapshot->intent_] = snapshotConsumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfoSnapshot);

    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> preview_consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    OHOS::sptr<OHOS::IBufferProducer> producerTemp = preview_consumer->CreateProducer([this](void* addr,
        uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    OHOS::sptr<BufferProducerSequenceable> bufferQueue = new BufferProducerSequenceable(producerTemp);
    cameraTest->rc = cameraTest->streamOperator->AttachBufferQueue(cameraTest->streamInfoSnapshot->streamId_,
        bufferQueue);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = cameraTest->streamOperator->DetachBufferQueue(cameraTest->streamInfoSnapshot->streamId_);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    std::vector<int> streamIds = {cameraTest->streamInfo->streamId_, cameraTest->streamInfoSnapshot->streamId_};
    cameraTest->rc = cameraTest->streamOperator->ReleaseStreams(streamIds);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: offlineStreamOperator
 * @tc.desc: offlineStreamOperator, input normal
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_Driver_Camera_Merge_6100, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = cameraTest->streamIdPreview;
    cameraTest->streamInfo->width_ = 1920;
    cameraTest->streamInfo->height_ = 1080;
    cameraTest->streamInfo->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfo->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfo->intent_ = PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });
    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfo->intent_] = consumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);

    cameraTest->streamInfoSnapshot = std::make_shared<StreamInfo>();
    cameraTest->streamInfoSnapshot->streamId_ = 102;
    cameraTest->streamInfoSnapshot->width_ = 1920;
    cameraTest->streamInfoSnapshot->height_ = 1080;
    cameraTest->streamInfoSnapshot->format_ = PIXEL_FMT_YCRCB_420_SP;
    cameraTest->streamInfoSnapshot->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfoSnapshot->intent_ = STILL_CAPTURE;
    cameraTest->streamInfoSnapshot->tunneledMode_ = UT_TUNNEL_MODE;
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> snapshotConsumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfoSnapshot->bufferQueue_ = snapshotConsumer->CreateProducerSeq([this](void* addr,
        uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });
    cameraTest->streamInfoSnapshot->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->consumerMap_[cameraTest->streamInfoSnapshot->intent_] = snapshotConsumer;
    cameraTest->streamInfos.push_back(*cameraTest->streamInfoSnapshot);

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
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

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->ChangeToOfflineStream({cameraTest->streamInfoSnapshot->streamId_},
        streamOperatorCallback, offlineStreamOperator);
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);

    sleep(UT_SECOND_TIMES);

    cameraTest->rc = (CamRetCode)offlineStreamOperator->CancelCapture(captureId);

    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    std::vector<int> streamIds = {102};
    cameraTest->rc = (CamRetCode)offlineStreamOperator->ReleaseStreams(streamIds);
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = (CamRetCode)offlineStreamOperator->Release();
    EXPECT_EQ(true, cameraTest->rc == HDI::Camera::V1_0::NO_ERROR);

    cameraTest->rc = (CamRetCode)cameraTest->streamOperator->ReleaseStreams({1201});
    EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, cameraTest->rc);
    sleep(UT_SECOND_TIMES);
}

#ifdef DISPLAY_COMMUNITY
/**
  * @tc.name: CAMERA_BUSY
  * @tc.desc: CAMERA_BUSY
  * @tc.level: Level1
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_001, TestSize.Level1)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    cameraTest->StartCapture(cameraTest->streamIdPreview, cameraTest->captureIdPreview, false, true);
    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> preview_consumer =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    OHOS::sptr<OHOS::IBufferProducer> producerTemp = preview_consumer->CreateProducer([this](void* addr,
        uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });

    OHOS::sptr<BufferProducerSequenceable> bufferProduder = new BufferProducerSequenceable(producerTemp);
    cameraTest->rc = cameraTest->streamOperator->AttachBufferQueue(cameraTest->streamIdPreview, bufferProduder);
    EXPECT_EQ(true, cameraTest->rc == CAMERA_BUSY);
    cameraTest->captureIds = {cameraTest->captureIdPreview};
    cameraTest->streamIds = {cameraTest->streamIdPreview};
    cameraTest->StopStream(cameraTest->captureIds, cameraTest->streamIds);
}

/**
 * @tc.name: DEVICE_ERROR
 * @tc.desc: DEVICE_ERROR
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_002, TestSize.Level0)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = cameraTest->streamIdPreview;
    cameraTest->streamInfo->width_ = cameraTest->previewWidth;
    cameraTest->streamInfo->height_ = cameraTest->previewHeight;
    cameraTest->streamInfo->format_ = cameraTest->previewFormat;
    cameraTest->streamInfo->dataspace_ = UT_DATA_SIZE;
    cameraTest->streamInfo->intent_ = StreamIntent::PREVIEW;
    cameraTest->streamInfo->tunneledMode_ = UT_TUNNEL_MODE;

    std::shared_ptr<OHOS::Camera::Test::StreamConsumer> consumer_pre =
        std::make_shared<OHOS::Camera::Test::StreamConsumer>();
    cameraTest->streamInfo->bufferQueue_ = consumer_pre->CreateProducerSeq([this](void* addr, uint32_t size) {
        cameraTest->DumpImageFile(cameraTest->streamIdPreview, "yuv", addr, size);
    });
    cameraTest->streamInfo->bufferQueue_->producer_->SetQueueSize(UT_DATA_SIZE);
    cameraTest->streamInfos.push_back(*cameraTest->streamInfo);
    cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->captureInfo = std::make_shared<CaptureInfo>();
    cameraTest->captureInfo->streamIds_ = {cameraTest->streamIdPreview};
    cameraTest->captureInfo->captureSetting_ = cameraTest->abilityVec;
    cameraTest->captureInfo->enableShutterCallback_ = true;
    cameraTest->rc = cameraTest->streamOperator->Capture(cameraTest->streamIdPreview, *cameraTest->captureInfo, true);
    EXPECT_EQ(true, cameraTest->rc == DEVICE_ERROR);
}

/**
 * @tc.name: INSUFFICIENT_RESOURCES
 * @tc.desc: INSUFFICIENT_RESOURCES
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_003, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::INSUFFICIENT_RESOURCES == -2);
}

/**
 * @tc.name: CAMERA_CLOSED
 * @tc.desc: CAMERA_CLOSED
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_004, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::CAMERA_CLOSED == -5);
}

/**
 * @tc.name: NO_PERMISSION
 * @tc.desc: NO_PERMISSION
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_005, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::NO_PERMISSION == -7);
}

/**
 * @tc.name: FLASHLIGHT_UNAVAILABLE
 * @tc.desc: FLASHLIGHT_UNAVAILABLE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_006, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::FLASHLIGHT_UNAVAILABLE == 2);
}

/**
 * @tc.name: UNKNOWN_ERROR
 * @tc.desc: UNKNOWN_ERROR
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_007, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::UNKNOWN_ERROR == 0);
}

/**
 * @tc.name: BUFFER_LOST
 * @tc.desc: BUFFER_LOST
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_008, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::BUFFER_LOST == 1);
}

/**
 * @tc.name: FATAL_ERROR
 * @tc.desc: FATAL_ERROR
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_009, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::FATAL_ERROR == 0);
}

/**
 * @tc.name: REQUEST_TIMEOUT
 * @tc.desc: REQUEST_TIMEOUT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_010, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::REQUEST_TIMEOUT == 1);
}

/**
 * @tc.name: DRIVER_ERROR
 * @tc.desc: DRIVER_ERROR
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_011, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::DRIVER_ERROR == 2);
}

/**
 * @tc.name: DEVICE_PREEMPT
 * @tc.desc: DEVICE_PREEMPT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_012, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::DEVICE_PREEMPT == 3);
}

/**
 * @tc.name: DEVICE_DISCONNECT
 * @tc.desc: DEVICE_DISCONNECT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_013, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::DEVICE_DISCONNECT == 4);
}

/**
 * @tc.name: DCAMERA_ERROR_BEGIN
 * @tc.desc: DCAMERA_ERROR_BEGIN
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_014, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::DCAMERA_ERROR_BEGIN == 1024);
}

/**
 * @tc.name: DCAMERA_ERROR_DEVICE_IN_USE
 * @tc.desc: DCAMERA_ERROR_DEVICE_IN_USE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_015, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::DCAMERA_ERROR_DEVICE_IN_USE == 1025);
    std::cout << "DCAMERA_ERROR_DEVICE_IN_USE value is " << HDI::Camera::V1_0::DCAMERA_ERROR_DEVICE_IN_USE << std::endl;
}

/**
 * @tc.name: DCAMERA_ERROR_NO_PERMISSION
 * @tc.desc: DCAMERA_ERROR_NO_PERMISSION
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(HdiStreamUtTest, SUB_DriverSystem_CameraHdiMg_016, TestSize.Level0)
{
    EXPECT_TRUE(HDI::Camera::V1_0::DCAMERA_ERROR_NO_PERMISSION == 1026);
    std::cout << "DCAMERA_ERROR_NO_PERMISSION value is " << HDI::Camera::V1_0::DCAMERA_ERROR_NO_PERMISSION << std::endl;
}
#endif