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
#include "open_camera_test.h"
#include "camera_preview_test.h"
#include <string>
#include <vector>

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

class wlanBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};

class CameraBenchmarkTest :  public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
    std::shared_ptr<TestDisplay> display_ = nullptr;
};

void CameraBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    std::cout << "==========[test log] CameraBenchmarkTest::SetUp.display_0: "<<display_<< std::endl;
    if (display_ == nullptr) {
        std::cout << "==========[test log] CameraBenchmarkTest::SetUp.display_1: "<<display_<< std::endl;
        display_ = std::make_shared<TestDisplay>();
        std::cout << "==========[test log] CameraBenchmarkTest::SetUp.display_2: "<<display_<< std::endl;
        std::cout << "==========[test log] CameraBenchmarkTest::SetUp.display_3: "<<display_<< std::endl;
        display_->Init();
        std::cout << "==========[test log] CameraBenchmarkTest::SetUp.display_4: "<<display_<< std::endl;
    }
}
void CameraBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    display_->Close();
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
   // std::cout << "==========[test log] OpenCamera, success."<< std::endl;
    std::vector<std::string> cameraIds;
    display_->cameraHost->GetCameraIds(cameraIds);
    for (auto &cameraId : cameraIds) {
     //   std::cout << "cameraId = " << cameraId << std::endl;
    }
    std::string cameraId = cameraIds.front();
    const OHOS::sptr<ICameraDeviceCallback> callback = new DemoCameraDeviceCallback();
    OHOS::sptr<ICameraDevice> cameraDevice;
    for (auto _ : st) {
    display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(cameraId, callback, cameraDevice);
    }
    //EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);
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
    std::vector<std::string> cameraIds;
    for (auto _ : st) {
    display_->cameraHost->GetCameraIds(cameraIds);
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
    //std::cout << "==========[test log] GetStreamOperator, input nullptr." << std::endl;
    // Get the configured cameraId
    display_->cameraHost->GetCameraIds(display_->cameraIds);
    std::cout << "cameraIds.front() = " << display_->cameraIds.front() << std::endl;
    // Open the camera device and get the device
    const OHOS::sptr<ICameraDeviceCallback> callback = new DemoCameraDeviceCallback();
    display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(display_->cameraIds.front(),
        callback, display_->cameraDevice);
   // std::cout << "OpenCamera's RetCode = " << display_->rc << std::endl;
    EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);
    // Create and get streamOperator information
    OHOS::sptr<IStreamOperatorCallback> streamOperatorCallback = new DemoStreamOperatorCallback();
    for (auto _ : st) {
    display_->rc = (CamRetCode)display_->cameraDevice->GetStreamOperator(streamOperatorCallback,
        display_->streamOperator);
    }
   // std::cout << "GetStreamOperator's RetCode = " << display_->rc << std::endl;
   // EXPECT_EQ(HDI::Camera::V1_0::NO_ERROR, display_->rc);
}
BENCHMARK_REGISTER_F(CameraBenchmarkTest, SUB_GetStreamOperator_benchmark_0010)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_MAIN();
