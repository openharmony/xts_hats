/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include "open_camera_test.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

void OpenCameraTest::SetUpTestCase(void)
{}
void OpenCameraTest::TearDownTestCase(void)
{}
void OpenCameraTest::SetUp(void)
{
    std::cout << "==========[test log] OpenCameraTest::SetUp.display_0: "<<display_<< std::endl;
    if (display_ == nullptr) {
        std::cout << "==========[test log] OpenCameraTest::SetUp.display_1: "<<display_<< std::endl;
        display_ = std::make_shared<TestDisplay>();
        std::cout << "==========[test log] OpenCameraTest::SetUp.display_2: "<<display_<< std::endl;
        std::cout << "==========[test log] OpenCameraTest::SetUp.display_3: "<<display_<< std::endl;
        display_->Init();
        std::cout << "==========[test log] OpenCameraTest::SetUp.display_4: "<<display_<< std::endl;
    }
}
void OpenCameraTest::TearDown(void)
{
    display_->Close();
}

/**
  * @tc.name: OpenCamera
  * @tc.desc: OpenCamera, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(OpenCameraTest, SUB_DriverSystem_CameraHdi_0010, TestSize.Level0)
{
    std::cout << "==========[test log] OpenCamera, success."<< std::endl;
    std::vector<std::string> cameraIds;
    display_->cameraHost->GetCameraIds(cameraIds);
    for (auto &cameraId : cameraIds) {
        std::cout << "cameraId = " << cameraId << std::endl;
    }
    std::string cameraId = cameraIds.front();
    const OHOS::sptr<ICameraDeviceCallback> callback = new DemoCameraDeviceCallback();
    OHOS::sptr<ICameraDevice> cameraDevice;
    display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(cameraId, callback, cameraDevice);
    EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);
}

/**
  * @tc.name: OpenCamera cameraID input error
  * @tc.desc: OpenCamera, cameraID is not found.
  * @tc.level: Level2
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(OpenCameraTest, SUB_DriverSystem_CameraHdi_0670, TestSize.Level0)
{
    std::cout << "==========[test log] OpenCamera, cameraID is not found."<< std::endl;
    sptr<ICameraHost> cameraHost = display_->CameraHostImplGetInstance();
    std::string cameraId = "qwerty";
    OHOS::sptr<ICameraDeviceCallback> callback = new DemoCameraDeviceCallback();
    std::cout << "opencamera begin" << std::endl;
    display_->rc = (CamRetCode)cameraHost->OpenCamera(cameraId, callback, display_->cameraDevice);
    std::cout << "opencamera end" << std::endl;
    EXPECT_EQ(INVALID_ARGUMENT, display_->rc);
}

/**
  * @tc.name: OpenCamera cameraID input error
  * @tc.desc: OpenCamera, cameraID is illegal.
  * @tc.level: Level2
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(OpenCameraTest, SUB_DriverSystem_CameraHdi_0680, TestSize.Level0)
{
    std::cout << "==========[test log] OpenCamera, cameraID is illegal."<< std::endl;
    std::string cameraId = "1";
    OHOS::sptr<ICameraDeviceCallback> callback = new DemoCameraDeviceCallback();
    display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(cameraId, callback, display_->cameraDevice);
    EXPECT_EQ(INVALID_ARGUMENT, display_->rc);
}

/**
  * @tc.name: OpenCamera cameraID input error
  * @tc.desc: OpenCamera, cameraID is Empty.
  * @tc.level: Level2
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(OpenCameraTest, SUB_DriverSystem_CameraHdi_0690, TestSize.Level0)
{
    std::cout << "==========[test log] OpenCamera, cameraID is Empty."<< std::endl;
    std::string cameraId;
    OHOS::sptr<ICameraDeviceCallback> callback = new DemoCameraDeviceCallback();
    display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(cameraId, callback, display_->cameraDevice);
    EXPECT_EQ(INVALID_ARGUMENT, display_->rc);
}

/**
  * @tc.name: OpenCamera Callback input error
  * @tc.desc: OpenCamera, Callback is Null.
  * @tc.level: Level2
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(OpenCameraTest, SUB_DriverSystem_CameraHdi_0700, TestSize.Level0)
{
    std::cout << "==========[test log] OpenCamera, Callback is Null."<< std::endl;
    std::string cameraId = "CAMERA_FIRST";
    OHOS::sptr<ICameraDeviceCallback> callback = nullptr;
    display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(cameraId, callback, display_->cameraDevice);
    EXPECT_EQ(INVALID_ARGUMENT, display_->rc);
}

/**
  * @tc.name: OpenCamera cameraID & Callback input error
  * @tc.desc: OpenCamera, cameraID is not found, callback is null.
  * @tc.level: Level1
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(OpenCameraTest, SUB_DriverSystem_CameraHdi_0710, TestSize.Level0)
{
    std::cout << "==========[test log] OpenCamera, cameraID is not found, callback is null."<< std::endl;
    sptr<ICameraHost> cameraHost = display_->CameraHostImplGetInstance();
    std::string cameraId = "qwerty";
    OHOS::sptr<ICameraDeviceCallback> callback = nullptr;
    std::cout << "opencamera begin" << std::endl;
    display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(cameraId, callback, display_->cameraDevice);
    std::cout << "opencamera end" << std::endl;
    EXPECT_EQ(INVALID_ARGUMENT, display_-> rc);
}

/**
  * @tc.name: OpenCamera cameraID & Callback input error
  * @tc.desc: OpenCamera, cameraID is illegal, callback is null.
  * @tc.level: Level1
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(OpenCameraTest, camera_open_0031, TestSize.Level0)
{
    std::cout << "==========[test log] OpenCamera, cameraID is illegal, callback is null."<< std::endl;
    std::string cameraId = "1";
    OHOS::sptr<ICameraDeviceCallback> callback = nullptr;
    display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(cameraId, callback, display_->cameraDevice);
    EXPECT_EQ(INVALID_ARGUMENT, display_-> rc);
}

/**
  * @tc.name: OpenCamera cameraID & Callback input error
  * @tc.desc: OpenCamera, cameraID is Empty, callback is null.
  * @tc.level: Level1
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(OpenCameraTest, camera_open_0032, TestSize.Level0)
{
    std::cout << "==========[test log] OpenCamera, cameraID is Empty, callback is null."<< std::endl;
    std::string cameraId;
    OHOS::sptr<ICameraDeviceCallback> callback = nullptr;
    display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(cameraId, callback, display_->cameraDevice);
    EXPECT_EQ(INVALID_ARGUMENT, display_->rc);
}

/**
  * @tc.name: Open all Cameras
  * @tc.desc: Open every Cameras what the getCameraId get.
  * @tc.level: Level2
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(OpenCameraTest, SUB_DriverSystem_CameraHdi_0020, TestSize.Level0)
{
    std::cout << "==========[test log] Open all Cameras."<< std::endl;
    std::vector<std::string> cameraIds;
    display_->cameraHost->GetCameraIds(cameraIds);
    for (auto &cameraId : cameraIds) {
        std::cout << "cameraId = " << cameraId << std::endl;
        const OHOS::sptr<ICameraDeviceCallback> callback = new DemoCameraDeviceCallback();
        display_->rc = (CamRetCode)display_->cameraHost->OpenCamera(cameraId, callback, display_->cameraDevice);
        EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);
        if (display_->rc == HDI::Camera::V1_0::NO_ERROR) {
            std::cout << "==========[test log] OpenCamera success, cameraId = " << cameraId << std::endl;
            } else {
                std::cout << "==========[test log] OpenCamera fail, rc = ";
                std::cout << display_->rc << ", cameraId = " << cameraId << std::endl;
            }
        }
}