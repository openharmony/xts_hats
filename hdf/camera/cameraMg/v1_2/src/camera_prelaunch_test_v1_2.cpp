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
#include "camera_prelaunch_test_v1_2.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

void CameraPrelaunchTestV1_2::SetUpTestCase(void) {}
void CameraPrelaunchTestV1_2::TearDownTestCase(void) {}
void CameraPrelaunchTestV1_2::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init(); // assert inside
}

void CameraPrelaunchTestV1_2::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.name: PreCameraSwitch
 * @tc.desc: PreCameraSwitch cameraId:device/0
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_0010, TestSize.Level1)
{
    std::string cameraId = "device/0";
    cameraTest->rc = cameraTest->serviceV1_2->PreCameraSwitch(cameraId);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: PreCameraSwitch
 * @tc.desc: PreCameraSwitch cameraId:device/1
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_0020, TestSize.Level1)
{
    std::string cameraId = "device/1";
    cameraTest->rc = cameraTest->serviceV1_2->PreCameraSwitch(cameraId);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: PreCameraSwitch
 * @tc.desc: PreCameraSwitch cameraId:null
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_0030, TestSize.Level1)
{
    std::string cameraId = "";
    cameraTest->rc = cameraTest->serviceV1_2->PreCameraSwitch(cameraId);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.name: PreCameraSwitch
 * @tc.desc: PreCameraSwitch cameraId:device/10
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_0040, TestSize.Level1)
{
    std::string cameraId = "device/10";
    cameraTest->rc = cameraTest->serviceV1_2->PreCameraSwitch(cameraId);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.name: PreCameraSwitch
 * @tc.desc: PreCameraSwitch cameraId:ABC123
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_0050, TestSize.Level1)
{
    std::string cameraId = "ABC123";
    cameraTest->rc = cameraTest->serviceV1_2->PreCameraSwitch(cameraId);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,NORMAL
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_0060, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::NORMAL);
    if (cameraTest->rc == -4) {
        GETST_SKIP()<< "Prelaunch is not supported" << std::endl;
    }
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,CAPTURE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_0070, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::CAPTURE);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,VIDEO
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_0080, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::VIDEO);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,PORTRAIT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_0090, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::PORTRAIT);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,NIGHT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1000, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::NIGHT);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,PROFESSIONAL
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1100, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::PROFESSIONAL);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,SLOW_MOTION
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1200, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::SLOW_MOTION);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,SCAN_CODE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1300, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::SCAN_CODE);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,CAPTURE_MACRO
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1400, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::CAPTURE_MACRO);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,VIDEO_MACRO
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1500, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::VIDEO_MACRO);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0,SUPER_STAB
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1600, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::SUPER_STAB);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,NORMAL
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1700, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::NORMAL);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,CAPTURE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1800, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::CAPTURE);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,VIDEO
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_1900, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::VIDEO);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,PORTRAIT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2000, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::PORTRAIT);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,NIGHT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2100, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::NIGHT);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,PROFESSIONAL
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2200, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::PROFESSIONAL);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,SLOW_MOTION
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2300, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::SLOW_MOTION);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,SCAN_CODE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2400, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::SCAN_CODE);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,CAPTURE_MACRO
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2500, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::CAPTURE_MACRO);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,VIDEO_MACRO
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2600, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::VIDEO_MACRO);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,SUPER_STAB
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2700, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::SUPER_STAB);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/10,NORMAL
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2800, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::NORMAL);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/1,NORMAL
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_2900, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    int operationMode = -1;
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, operationMode);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:ABC123,NORMAL
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_3000, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::NORMAL);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    cameraTest->Open(DEVICE_0);
}

/**
 * @tc.name: PrelaunchWithOpMode
 * @tc.desc: PrelaunchWithOpMode cameraId:device/0
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraPrelaunchTestV1_2, SUB_Driver_Camera_PreCameraSwitch_3100, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    constexpr int32_t DEFAULT_ITEMS = 1;
    constexpr int32_t DEFAULT_DATA_LENGTH = 1;
    auto defaultSettings = std::make_shared<OHOS::Camera::CameraMetadata>(DEFAULT_ITEMS, DEFAULT_DATA_LENGTH);
    float zoomRatio = 10.0f;
    defaultSettings->addEntry(OHOS_CONTROL_ZOOM_RATIO, &zoomRatio, 1);
    std::vector<uint8_t> settings;
    MetadataUtils::ConvertMetadataToVec(defaultSettings, settings);
    cameraTest->prelaunchConfig->setting = settings;
    cameraTest->rc = cameraTest->serviceV1_2->PrelaunchWithOpMode(
        *cameraTest->prelaunchConfig, OHOS::HDI::Camera::V1_2::NORMAL);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}