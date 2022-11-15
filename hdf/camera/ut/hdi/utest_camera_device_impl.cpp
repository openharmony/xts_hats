/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
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

#include "utest_camera_device_impl.h"

using namespace OHOS::HDI::Camera::V1_0;

void CameraDeviceImplTest::SetUpTestCase(void)
{
}

void CameraDeviceImplTest::TearDownTestCase(void)
{
    std::cout << "Camera::CameraDeviceImpl TearDownTestCase" << std::endl;
}

void CameraDeviceImplTest::SetUp(void)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
    ret = GetCameraIds();
    ASSERT_EQ(true, ret);
    ret = GetCameraDevice();
    ASSERT_EQ(true, ret);
}

void CameraDeviceImplTest::TearDown(void)
{
}

/**
  * @tc.name: GetStreamOperator
  * @tc.desc: GetStreamOperator, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5000, TestSize.Level0)
{
    bool ret = GetStreamOperator();
    ASSERT_EQ(true, ret);
}

/**
  * @tc.name: updateSettings
  * @tc.desc: UpdateSettings, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5010, TestSize.Level0)
{
    std::vector<std::string> cameraIds;
    CamRetCode ret = (CamRetCode)cameraHost_->GetCameraIds(cameraIds);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
    std::vector<uint8_t> ability;
    std::string cameraId = cameraIds.front();
    ret = (CamRetCode)cameraHost_->GetCameraAbility(cameraId, ability);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
    ret = (CamRetCode)cameraDevice_->UpdateSettings(ability);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, ret);
    std::cout << "UTestUpdateSettings ret = " << ret << std::endl;
}

/**
  * @tc.name: SetResultMode
  * @tc.desc: SetResultMode, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5020, TestSize.Level0)
{
    ResultCallbackMode mode = PER_FRAME;
    CamRetCode rc = (CamRetCode)cameraDevice_->SetResultMode(mode);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
}

/**
  * @tc.name: SetResultMode
  * @tc.desc: SetResultMode, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5021, TestSize.Level0)
{
    ResultCallbackMode mode = ON_CHANGED;
    CamRetCode rc = (CamRetCode)cameraDevice_->SetResultMode(mode);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
}


/**
  * @tc.name: SetResultMode_02
  * @tc.desc: SetResultMode, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5022, TestSize.Level0)
{
    ResultCallbackMode mode = PER_FRAME;
    CamRetCode rc = (CamRetCode)cameraDevice_->SetResultMode(mode);
    ASSERT_NE(HDI::Camera::V1_0::CAMERA_BUSY, rc);
}

/**
  * @tc.name: SetResultMode_03
  * @tc.desc: SetResultMode, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5023, TestSize.Level0)
{
    ResultCallbackMode mode = PER_FRAME;
    CamRetCode rc = (CamRetCode)cameraDevice_->SetResultMode(mode);
    ASSERT_NE(HDI::Camera::V1_0::INSUFFICIENT_RESOURCES, rc);
}

/**
  * @tc.name: SetResultMode_04
  * @tc.desc: SetResultMode, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5024, TestSize.Level0)
{
    ResultCallbackMode mode = PER_FRAME;
    CamRetCode rc = (CamRetCode)cameraDevice_->SetResultMode(mode);
    ASSERT_NE(HDI::Camera::V1_0::METHOD_NOT_SUPPORTED, rc);
}

/**
  * @tc.name: SetResultMode_05
  * @tc.desc: SetResultMode, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5025, TestSize.Level0)
{
    ResultCallbackMode mode = PER_FRAME;
    CamRetCode rc = (CamRetCode)cameraDevice_->SetResultMode(mode);
    ASSERT_NE(HDI::Camera::V1_0::CAMERA_CLOSED, rc);
}

/**
  * @tc.name: SetResultMode_06
  * @tc.desc: SetResultMode, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5026, TestSize.Level0)
{
    ResultCallbackMode mode = PER_FRAME;
    CamRetCode rc = (CamRetCode)cameraDevice_->SetResultMode(mode);
    ASSERT_NE(HDI::Camera::V1_0::DEVICE_ERROR, rc);
}

/**
  * @tc.name: SetResultMode_07
  * @tc.desc: SetResultMode, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5027, TestSize.Level0)
{
    ResultCallbackMode mode = PER_FRAME;
    CamRetCode rc = (CamRetCode)cameraDevice_->SetResultMode(mode);
    ASSERT_NE(HDI::Camera::V1_0::NO_PERMISSION, rc);
}

/**
  * @tc.name: GetEnabledResults
  * @tc.desc: GetEnabledResults, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5030, TestSize.Level0)
{
    std::vector<int32_t> results;
    CamRetCode rc = (CamRetCode)cameraDevice_->GetEnabledResults(results);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
    for (const auto &type : results) {
        std::cout << "MetaType = " << type << std::endl;
    }
}

/**
  * @tc.name: EnableResult
  * @tc.desc: EnableResult, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5040, TestSize.Level0)
{
    std::vector<int32_t> results;
    results.push_back(OHOS_SENSOR_EXPOSURE_TIME);
    results.push_back(OHOS_SENSOR_COLOR_CORRECTION_GAINS);
    CamRetCode rc = (CamRetCode)cameraDevice_->EnableResult(results);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
}

/**
  * @tc.name: DisableResult
  * @tc.desc: DisableResult, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5050, TestSize.Level0)
{
    std::vector<int32_t> results;
    results.push_back(OHOS_SENSOR_EXPOSURE_TIME);
    results.push_back(OHOS_SENSOR_COLOR_CORRECTION_GAINS);
    CamRetCode rc = (CamRetCode)cameraDevice_->EnableResult(results);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
    std::vector<int32_t> disable_tag;
    rc = (CamRetCode)cameraDevice_->GetEnabledResults(disable_tag);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
    rc = (CamRetCode)cameraDevice_->DisableResult(disable_tag);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
}

/**
  * @tc.name: Close
  * @tc.desc: Close, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraDeviceImplTest, SUB_DriverSystem_CameraHdi_5060, TestSize.Level0)
{
    if (cameraDevice_ != nullptr) {
        cameraDevice_->Close();
        cameraDevice_ = nullptr;
    }
}