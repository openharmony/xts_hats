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
 * See the License for  the specific language governing permissions and
 * limitations under the License.
 */
#include "camera_hdi_uttest_v1_1.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;
int64_t OHOS::Camera::Test::StreamConsumer::g_timestamp[2] = {0};
void CameraHdiTestV1_1Additional::SetUpTestCase(void) {}
void CameraHdiTestV1_1Additional::TearDownTestCase(void) {}
void CameraHdiTestV1_1Additional::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
}
void CameraHdiTestV1_1Additional::TearDown(void) { cameraTest->Close(); }

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0100
 * @tc.name   : testPrelaunch001
 * @tc.desc   : Prelaunch cameraId:device/6
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch001, TestSize.Level1)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/6";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0500
 * @tc.name   : testPrelaunch005
 * @tc.desc   : Prelaunch cameraId:{}
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch005, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0600
 * @tc.name   : testPrelaunch006
 * @tc.desc   : Prelaunch cameraId:{}
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch006, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0700
 * @tc.name   : testPrelaunch007
 * @tc.desc   : Prelaunch cameraId:{}
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch007, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0800
 * @tc.name   : testPrelaunch008
 * @tc.desc   : Prelaunch cameraId:{}
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch008, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_0900
 * @tc.name   : testPrelaunch009
 * @tc.desc   : Prelaunch cameraId:device/10
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch009, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1000
 * @tc.name   : testPrelaunch010
 * @tc.desc   : Prelaunch cameraId:device/10
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch010, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1100
 * @tc.name   : testPrelaunch011
 * @tc.desc   : Prelaunch cameraId:device/10
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch011, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1200
 * @tc.name   : testPrelaunch012
 * @tc.desc   : Prelaunch cameraId:device/10
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch012, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1300
 * @tc.name   : testPrelaunch013
 * @tc.desc   : Prelaunch cameraId:ABC123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch013, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1400
 * @tc.name   : testPrelaunch014
 * @tc.desc   : Prelaunch cameraId:ABC123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch014, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1500
 * @tc.name   : testPrelaunch015
 * @tc.desc   : Prelaunch cameraId:ABC123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch015, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1600
 * @tc.name   : testPrelaunch016
 * @tc.desc   : Prelaunch cameraId:ABC123
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch016, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1700
 * @tc.name   : testPrelaunch017
 * @tc.desc   : Prelaunch cameraId:1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch017, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1800
 * @tc.name   : testPrelaunch018
 * @tc.desc   : Prelaunch cameraId:1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch018, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_1900
 * @tc.name   : testPrelaunch019
 * @tc.desc   : Prelaunch cameraId:1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch019, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_2000
 * @tc.name   : testPrelaunch020
 * @tc.desc   : Prelaunch cameraId:1
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch020, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "1";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(255);

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
}

/**
 * @tc.number : SUB_Driver_Camera_Getdefaultsettings_0100
 * @tc.name   : testGetDefaultSettings001
 * @tc.desc   : GetDefaultSettings
 */
HWTEST_F(CameraHdiTestV1_1Additional, testGetDefaultSettings001, TestSize.Level1)
{
    cameraTest->Open();
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
        cameraTest->rc = cameraTest->cameraDeviceV1_1->GetDefaultSettings(cameraTest->abilityVec);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5800
 * @tc.name   : testPrelaunch058
 * @tc.desc   : Prelaunch cameraId:deviceA
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch058, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "deviceA";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(0);

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    }
}

/**
 * @tc.number : SUB_Driver_Camera_Prelaunch_5900
 * @tc.name   : testPrelaunch059
 * @tc.desc   : Prelaunch cameraId:deviceA
 */
HWTEST_F(CameraHdiTestV1_1Additional, testPrelaunch059, TestSize.Level2)
{
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    std::vector<uint8_t> settings;
    cameraTest->prelaunchConfig->cameraId = "deviceA";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting.push_back(1);

    for (int i = 0; i < 100; i++) {
        cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
        EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::INVALID_ARGUMENT);
    }
}