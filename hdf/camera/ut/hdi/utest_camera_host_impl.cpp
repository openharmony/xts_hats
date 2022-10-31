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

#include "utest_camera_host_impl.h"

using namespace OHOS;
using namespace testing::ext;
using namespace OHOS::Camera;
using namespace OHOS::HDI::Camera::V1_0;

/**
  * @tc.name: InitCameraHost
  * @tc.desc: InitCameraHost, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraHostImplTest, SUB_DriverSystem_CameraHdi_5070, TestSize.Level0)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
}

/**
  * @tc.name: GetCameraIds
  * @tc.desc: GetCameraIds, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraHostImplTest, SUB_DriverSystem_CameraHdi_5080, TestSize.Level0)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
    ret = GetCameraIds();
    ASSERT_EQ(true, ret);
}

/**
  * @tc.name: GetCameraAbility
  * @tc.desc: GetCameraAbility, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraHostImplTest, SUB_DriverSystem_CameraHdi_5090, TestSize.Level0)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
    ret = GetCameraIds();
    ASSERT_EQ(true, ret);
    std::vector<uint8_t> ability;
    std::string cameraId = cameraIds_.front();
    CamRetCode rc = (CamRetCode)cameraHost_->GetCameraAbility(cameraId, ability);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
}

/**
  * @tc.name: SetFlashlight
  * @tc.desc: SetFlashlight, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
#ifndef CAMERA_BUILT_ON_OHOS_LITE
HWTEST_F(CameraHostImplTest, SUB_DriverSystem_CameraHdi_5100, TestSize.Level0)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
    ret = GetCameraIds();
    ASSERT_EQ(true, ret);
    std::string cameraId = cameraIds_.front();
    bool isEnable = true;
    CamRetCode rc = (CamRetCode)cameraHost_->SetFlashlight(cameraId, isEnable);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
    sleep(1);
    isEnable = false;
    rc = (CamRetCode)cameraHost_->SetFlashlight(cameraId, isEnable);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
}
#endif

/**
  * @tc.name: OpenCamera
  * @tc.desc: OpenCamera, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraHostImplTest, SUB_DriverSystem_CameraHdi_5110, TestSize.Level0)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
    ret = GetCameraIds();
    ASSERT_EQ(true, ret);

    std::string cameraId = cameraIds_.front();
#ifdef CAMERA_BUILT_ON_OHOS_LITE
    std::shared_ptr<CameraDeviceCallback> deviceCallback = std::make_shared<CameraDeviceCallback>();
#else
    sptr<ICameraDeviceCallback> deviceCallback = new DemoCameraDeviceCallback();
#endif
    CamRetCode rc = (CamRetCode)cameraHost_->OpenCamera(cameraId, deviceCallback, cameraDevice_);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
    if (cameraDevice_ != nullptr) {
        cameraDevice_->Close();
        cameraDevice_ = nullptr;
    }
}

/**
  * @tc.name: OpenCamera
  * @tc.desc: OpenCamera_CallbackIsNull, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraHostImplTest, SUB_DriverSystem_CameraHdi_5120, TestSize.Level0)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
    ret = GetCameraIds();
    ASSERT_EQ(true, ret);

    std::string cameraId = cameraIds_.front();
#ifdef CAMERA_BUILT_ON_OHOS_LITE
    std::shared_ptr<CameraDeviceCallback> deviceCallback = nullptr;
#else
    OHOS::sptr<ICameraDeviceCallback> deviceCallback = nullptr;
#endif
    CamRetCode rc = (CamRetCode)cameraHost_->OpenCamera(cameraId, deviceCallback, cameraDevice_);
    ASSERT_EQ(INVALID_ARGUMENT, rc);
    if (cameraDevice_ != nullptr) {
        cameraDevice_->Close();
        cameraDevice_ = nullptr;
    }
}

/**
  * @tc.name: OpenCamera
  * @tc.desc: OpenCamera_CameraIdIsEmpty, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraHostImplTest, SUB_DriverSystem_CameraHdi_5130, TestSize.Level0)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
    std::string cameraId;
#ifdef CAMERA_BUILT_ON_OHOS_LITE
    std::shared_ptr<CameraDeviceCallback> deviceCallback = std::make_shared<CameraDeviceCallback>();
#else
    sptr<ICameraDeviceCallback> deviceCallback = new DemoCameraDeviceCallback();
#endif
    CamRetCode rc = (CamRetCode)cameraHost_->OpenCamera(cameraId, deviceCallback, cameraDevice_);
    ASSERT_EQ(INVALID_ARGUMENT, rc);
    if (cameraDevice_ != nullptr) {
        cameraDevice_->Close();
        cameraDevice_ = nullptr;
    }
}

/**
  * @tc.name: OpenCamera
  * @tc.desc: OpenCamera_CameraIdIsNotFound, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraHostImplTest, SUB_DriverSystem_CameraHdi_5140, TestSize.Level0)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
    std::string cameraId = "qwerty";
#ifdef CAMERA_BUILT_ON_OHOS_LITE
    std::shared_ptr<CameraDeviceCallback> deviceCallback = std::make_shared<CameraDeviceCallback>();
#else
    OHOS::sptr<ICameraDeviceCallback> deviceCallback = new DemoCameraDeviceCallback();
#endif
    CamRetCode rc = (CamRetCode)cameraHost_->OpenCamera(cameraId, deviceCallback, cameraDevice_);
    ASSERT_EQ(INVALID_ARGUMENT, rc);
    if (cameraDevice_ != nullptr) {
        cameraDevice_->Close();
        cameraDevice_ = nullptr;
    }
}

/**
  * @tc.name: SetCallback
  * @tc.desc: SetCallback, success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(CameraHostImplTest, SUB_DriverSystem_CameraHdi_5150, TestSize.Level0)
{
    bool ret = InitCameraHost();
    ASSERT_EQ(true, ret);
#ifdef CAMERA_BUILT_ON_OHOS_LITE
    std::shared_ptr<CameraHostCallback> callback = std::make_shared<CameraHostCallback>();
#else
    OHOS::sptr<ICameraHostCallback> callback = new DemoCameraHostCallback();
#endif
    CamRetCode rc = (CamRetCode)cameraHost_->SetCallback(callback);
    ASSERT_EQ(HDI::Camera::V1_0::NO_ERROR, rc);
}
