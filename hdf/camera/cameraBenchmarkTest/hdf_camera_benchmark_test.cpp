/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <benchmark/benchmark.h>
#include <string>
#include <vector>
#include "hdi_host_uttest.h"
//#include "common.h"
using namespace OHOS::HDI::Camera::V1_0;
using namespace OHOS;
using namespace std;
using namespace testing::ext;
//using namespace OHOS::Camera;

class CameraBenchmarkTest :  public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
    std::shared_ptr<OHOS::Camera::Test> cameraTest = nullptr;
};

void CameraBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
}
void CameraBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    cameraTest->Close();
}

/**
  * @tc.name: OpenCamera
  * @tc.desc: OpenCamera, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_OpenCamera_benchmark_0010)(
    benchmark::State &st)
{
    EXPECT_EQ(true, cameraTest->cameraDevice == nullptr);
    if (cameraTest->cameraDevice == nullptr) {
        cameraTest->service->GetCameraIds(cameraTest->cameraIds);
        cameraTest->deviceCallback = new OHOS::Camera::Test::DemoCameraDeviceCallback();
        for (auto _ : st) {
        cameraTest->rc = cameraTest->service->OpenCamera(cameraTest->cameraIds.front(), cameraTest->deviceCallback,
            cameraTest->cameraDevice);
        }
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_OpenCamera_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetCameraIds
  * @tc.desc: GetCameraIds, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_GetCameraIds_benchmark_0010)(
    benchmark::State &st)
{
    //std::cout << "==========[test log] GetCameraIds, success."<< std::endl;
    for (auto _ : st) {
        cameraTest->rc = cameraTest->service->GetCameraIds(cameraTest->cameraIds);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_GetCameraIds_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetStreamOperator
  * @tc.desc: GetStreamOperator, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_GetStreamOperator_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    for (auto _ : st) {
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_GetStreamOperator_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();
/**
  * @tc.name: GetCameraAbility
  * @tc.desc: GetCameraAbility, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_GetCameraAbility_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->rc = cameraTest->service->GetCameraIds(cameraTest->cameraIds);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    for (int i = 0; i < cameraTest->cameraIds.size(); i++) {
        for (auto _ : st) {
            cameraTest->rc = cameraTest->service->GetCameraAbility(cameraTest->cameraIds[i], cameraTest->abilityVec);
        }
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_GetCameraAbility_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetFlashlight
  * @tc.desc: SetFlashlight, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_SetFlashlight_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->service->GetCameraIds(cameraTest->cameraIds);
    cameraTest->status = true;
    for (auto _ : st) {
        cameraTest->rc = cameraTest->service->SetFlashlight(cameraTest->cameraIds.front(), cameraTest->status);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_SetFlashlight_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetResultMode
  * @tc.desc: SetResultMode, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_SetResultMode_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->Open();
    for (auto _ : st) {
        cameraTest->rc = cameraTest->cameraDevice->SetResultMode(PER_FRAME);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_SetResultMode_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: CreateStreams
  * @tc.desc: CreateStreams, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_CreateStreams_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 1001;
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
    for (auto _ : st) {
        cameraTest->rc = cameraTest->streamOperator->CreateStreams(cameraTest->streamInfos);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_CreateStreams_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: CommitStreams
  * @tc.desc: CommitStreams, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_CommitStreams_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->Open();
    cameraTest->streamOperatorCallback = new OHOS::Camera::Test::TestStreamOperatorCallback();
    cameraTest->rc = cameraTest->cameraDevice->GetStreamOperator(cameraTest->streamOperatorCallback,
        cameraTest->streamOperator);
    EXPECT_EQ(false, cameraTest->rc != HDI::Camera::V1_0::NO_ERROR || cameraTest->streamOperator == nullptr);

    cameraTest->streamInfo = std::make_shared<StreamInfo>();
    cameraTest->streamInfo->streamId_ = 2147483647;
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

    for (auto _ : st) {
        cameraTest->rc = cameraTest->streamOperator->CommitStreams(OperationMode::NORMAL, cameraTest->abilityVec);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_CommitStreams_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetStreamAttributes
  * @tc.desc: GetStreamAttributes, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_GetStreamAttributes_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    std::vector<StreamAttribute> attributes;
    for (auto _ : st) {
        cameraTest->rc = cameraTest->streamOperator->GetStreamAttributes(attributes);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_GetStreamAttributes_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();
/**
  * @tc.name: ReleaseStreams
  * @tc.desc: ReleaseStreams, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_ReleaseStreams_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    for (auto _ : st) {
        cameraTest->rc = cameraTest->streamOperator->ReleaseStreams({cameraTest->streamIdPreview});
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_ReleaseStreams_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: Capture
  * @tc.desc: Capture, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_Capture_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->Open();
    cameraTest->intents = {PREVIEW};
    cameraTest->StartStream(cameraTest->intents);
    int captureId = 2001;
    cameraTest->captureInfo = std::make_shared<CaptureInfo>();
    cameraTest->captureInfo->streamIds_ = {-1};
    cameraTest->captureInfo->captureSetting_ = cameraTest->abilityVec;
    cameraTest->captureInfo->enableShutterCallback_ = true;
    for (auto _ : st) {
        cameraTest->rc = cameraTest->streamOperator->Capture(captureId, *cameraTest->captureInfo, true);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_Capture_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: CancelCapture
  * @tc.desc: CancelCapture, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_CancelCapture_benchmark_0010)(
    benchmark::State &st)
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
    for (auto _ : st) {
        cameraTest->streamOperator->CancelCapture(captureId);
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_CancelCapture_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: Close
  * @tc.desc: Close, benchmark.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
BENCHMARK_F(CameraBenchmarkTest, SUB_Close_benchmark_0010)(
    benchmark::State &st)
{
    cameraTest->Open();
    for (auto _ : st) {
        cameraTest->cameraDevice->Close();
    }
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_Close_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_MAIN();
