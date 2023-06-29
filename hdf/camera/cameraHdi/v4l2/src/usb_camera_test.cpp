/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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
#include "usb_camera_test.h"
using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;
bool usbCameraExit_ = false;

void USBCameraTest::SetUpTestCase(void)
{}
void USBCameraTest::TearDownTestCase(void)
{}
void USBCameraTest::SetUp(void)
{
    if (display_ == nullptr)
    display_ = std::make_shared<TestDisplay>();
    display_->UsbInit();
}
void USBCameraTest::TearDown(void)
{
    display_->Close();
}

/**
  * @tc.name: USB Camera
  * @tc.desc:  Add USB Camera, getCameraID success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0010, TestSize.Level1)
{
    uint32_t rc = 0;
    std::cout << "==========[test log] Add USB Camera, getCameraID success."<< std::endl;
    std::vector<std::string> cameraIds;
    std::cout << "==========[test log] 1. get current system cameraID."<< std::endl;
    display_->cameraHost->GetCameraIds(cameraIds);
    std::cout << "==========[test log] First cameraId.size = " << cameraIds.size() << std::endl;
    std::cout << "==========[test log] OnCameraStatus interface has been mobilized" << std::endl;
    for (const auto &cameraId : cameraIds) {
        std::cout << "==========[test log] cameraId = " << cameraId << std::endl;
    }
    std::cout << "==========[test log] 2. please add the usb camera, wait for 10s..."<< std::endl;
    sleep(5); // judging add or delete the usb camera, wait for 10s.
    std::cout << "==========[test log] r u ready? wait for 10s..."<< std::endl;
    sleep(5); // judging r u ready, wait for 10s.
    std::cout << "==========[test log] 3. check the cameraID again... wait for 10s..."<< std::endl;
    sleep(5); // checking the cameraID again, wait for 10s.
    if (cameraIds.size() == 1) {
        cameraIds.clear();
    }
    rc = display_->cameraHost->GetCameraIds(cameraIds);
    EXPECT_EQ(rc, HDI::Camera::V1_0::NO_ERROR);
    for (const auto &cameraId : cameraIds) {
        std::cout << "cameraId = " << cameraId << std::endl;
    }
    if (cameraIds.size() > 1) {
        usbCameraExit_ = true;
    }
    std::cout << "==========[test log] Second cameraId.size = " << cameraIds.size() << std::endl;
}

/**
  * @tc.name: USB Camera
  * @tc.desc:  Delect USB Camera, getCameraID success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0020, TestSize.Level1)
{
    uint32_t rc = 0;
    std::cout << "==========[test log] Delect USB Camera, getCameraID success."<< std::endl;
    std::vector<std::string> cameraIds;
    std::cout << "==========[test log] 1. get current system cameraID."<< std::endl;
    display_->cameraHost->GetCameraIds(cameraIds);
    std::cout << "==========[test log] First cameraId.size = " << cameraIds.size() << std::endl;
    std::cout << "==========[test log] OnCameraStatus interface has been mobilized" << std::endl;
    for (const auto &cameraId : cameraIds) {
        std::cout << "==========[test log] cameraId = " << cameraId << std::endl;
    }
    std::cout << "==========[test log] 2. please delect the usb camera, wait for 10s... "<< std::endl;
    sleep(10); // judging add or delete the usb camera, wait for 10s.
    std::cout << "==========[test log] r u ready? wait for 10s..."<< std::endl;
    sleep(10); // judging r u ready, wait for 10s.
    std::cout << "==========[test log] 3. check the cameraID again... wait for 10s..."<< std::endl;
    sleep(10); // checking the cameraID again, wait for 10s.
    if (cameraIds.size() == 1) {
        cameraIds.clear();
    }
    rc = display_->cameraHost->GetCameraIds(cameraIds);
    EXPECT_EQ(rc, HDI::Camera::V1_0::NO_ERROR);
    for (const auto &cameraId : cameraIds) {
        std::cout << "cameraId = " << cameraId << std::endl;
    }
    if (cameraIds.size() > 1) {
        usbCameraExit_ = true;
    }
    std::cout << "==========[test log] Second cameraId.size = " << cameraIds.size() << std::endl;
}

/**
  * @tc.name: USB Camera
  * @tc.desc:  Loop Add/Delect USB Camera, getCameraID success.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0030, TestSize.Level1)
{
    uint32_t rc = 0;
    std::cout << "==========[test log] Insert USB Camera, getCameraID success."<< std::endl;
    std::vector<std::string> cameraIds;
    std::cout << "==========[test log] 1. get current system cameraID."<< std::endl;
    display_->cameraHost->GetCameraIds(cameraIds);
    std::cout << "==========[test log] First cameraId.size = " << cameraIds.size() << std::endl;
    for (int i=0 ;i<=20 ;i++) {
        std::cout << "==========[test log] 2. please add or delete the usb camera, wait for 10s..."<< std::endl;
        sleep(10); // judging add or delete the usb camera, wait for 10s.
        std::cout << "==========[test log] r u ready? wait for 10s..."<< std::endl;
        sleep(10); // judging r u ready, wait for 10s.
        std::cout << "==========[test log] 3. check the cameraID again... wait for 10s..."<< std::endl;
        sleep(10); // checking the cameraID again, wait for 10s.
        std::cout << "==========[test log] Second cameraId.size = " << cameraIds.size() << std::endl;
        cameraIds.clear();
        display_->cameraHost->GetCameraIds(cameraIds);
        std::cout << "==========[test log] Second cameraId.size = " << cameraIds.size() << std::endl;
    }
	std::cout << "==========[test log] 2. please add the usb camera, wait for 10s..."<< std::endl;
    sleep(5); // judging add or delete the usb camera, wait for 10s.
    std::cout << "==========[test log] r u ready? wait for 10s..."<< std::endl;
    sleep(5); // judging r u ready, wait for 10s.
    std::cout << "==========[test log] 3. check the cameraID again... wait for 10s..."<< std::endl;
    sleep(5); // checking the cameraID again, wait for 10s.
    if (cameraIds.size() == 1) {
        cameraIds.clear();
    }
    rc = display_->cameraHost->GetCameraIds(cameraIds);
    EXPECT_EQ(rc, HDI::Camera::V1_0::NO_ERROR);
    for (const auto &cameraId : cameraIds) {
        std::cout << "cameraId = " << cameraId << std::endl;
    }
    if (cameraIds.size() > 1) {
        usbCameraExit_ = true;
    }
}


/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_ZOOM_RATIO_RANGE
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0040, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_ZOOM_RATIO_RANGE, &entry);
    if (ret == CAM_META_SUCCESS) {
        std::cout << "OHOS_ABILITY_ZOOM_RATIO_RANGE: count is " << (int)entry.count << std::endl;
        std::cout << "Zoom ratio range: [" << entry.data.f[0];
        std::cout << "," << entry.data.f[1] << "]" << std::endl;
    } else if (ret == CAM_META_ITEM_NOT_FOUND) {
        std::cout << "OHOS_ABILITY_ZOOM_RATIO_RANGE is not support" << std::endl;
    }
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_CAMERA_CONNECTION_TYPE
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0050, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_CONNECTION_TYPE, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "OHOS_ABILITY_CAMERA_CONNECTION_TYPE value is "
        << static_cast<int>(entry.data.u8[0]) << std::endl;
    EXPECT_TRUE(entry.data.u8[0] == OHOS_CAMERA_CONNECTION_TYPE_USB_PLUGIN);
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_CAMERA_POSITION
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0060, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_POSITION, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "OHOS_ABILITY_CAMERA_POSITION value is " << static_cast<int>(entry.data.u8[0]) << std::endl;
    EXPECT_TRUE(entry.data.u8[0] == OHOS_CAMERA_POSITION_OTHER);
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_FLASH_AVAILABLE
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0070, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_FLASH_AVAILABLE, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "OHOS_ABILITY_FLASH_AVAILABLE value is " << static_cast<int>(entry.data.u8[0]) << std::endl;
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_VIDEO_STABILIZATION_MODES
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0080, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_VIDEO_STABILIZATION_MODES, &entry);
    if (ret == CAM_META_SUCCESS) {
        for (int i = 0; i < entry.count; i++) {
            std::cout << "OHOS_ABILITY_VIDEO_STABILIZATION_MODES value is "
                    << static_cast<int>(entry.data.u8[i]) << std::endl;
        }
    } else if (ret == CAM_META_ITEM_NOT_FOUND) {
        std::cout << "OHOS_ABILITY_VIDEO_STABILIZATION_MODES is not support" << std::endl;
    }
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_FLASH_MODES
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0090, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_FLASH_MODES, &entry);
    if (ret == CAM_META_SUCCESS) {
        std::cout << "supported flash mode list:";
        for (int i = 0; i < entry.count; i++) {
            std::cout << " " << static_cast<int>(entry.data.u8[i]);
        }
        std::cout << std::endl;
    } else if (ret == CAM_META_ITEM_NOT_FOUND) {
        std::cout << "OHOS_ABILITY_FLASH_MODES is not support" << std::endl;
    }
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_FOCUS_MODES
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0100, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_FOCUS_MODES, &entry);
    if (ret == CAM_META_SUCCESS) {
        std::cout << "supported focus mode list:";
        for (int i = 0; i < entry.count; i++) {
            std::cout << " " << static_cast<int>(entry.data.u8[i]);
        }
        std::cout << std::endl;
    } else if (ret == CAM_META_ITEM_NOT_FOUND) {
        std::cout << "OHOS_ABILITY_FOCUS_MODES is not support" << std::endl;
    }
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_EXPOSURE_MODES
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0110, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_EXPOSURE_MODES, &entry);
    if (ret == CAM_META_SUCCESS) {
        std::cout << "supported exposure mode list:";
        for (int i = 0; i < entry.count; i++) {
            std::cout << " " << static_cast<int>(entry.data.u8[i]);
        }
        std::cout << std::endl;
    } else if (ret == CAM_META_ITEM_NOT_FOUND) {
        std::cout << "OHOS_ABILITY_EXPOSURE_MODES is not support" << std::endl;
    }
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_CONTROL_CAPTURE_MIRROR_SUPPORTED
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0120, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_CONTROL_CAPTURE_MIRROR_SUPPORTED, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "capture mirror supported is :";
    for (int i = 0; i < entry.count; i++) {
        std::cout << " " << static_cast<int>(entry.data.u8[i]);
    }
    std::cout << std::endl;
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_MUTE_MODES
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0130, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_MUTE_MODES, &entry);
    if (ret == CAM_META_SUCCESS) {
        std::cout << "supported mute mode is:";
        for (int i = 0; i < entry.count; i++) {
            std::cout << " " << static_cast<int>(entry.data.u8[i]);
        }
        std::cout << std::endl;
    } else if (ret == CAM_META_ITEM_NOT_FOUND) {
        std::cout << "OHOS_ABILITY_MUTE_MODES is not support" << std::endl;
    }
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_FPS_RANGES
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0140, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_FPS_RANGES, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "supported fps ranges list: [";
    std::cout << static_cast<int>(entry.data.i32[0]) << "," << static_cast<int>(entry.data.i32[1]) << "]";
    std::cout << std::endl;
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_CAMERA_TYPE
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0150, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_TYPE, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "OHOS_ABILITY_CAMERA_TYPE value is " << static_cast<int>(entry.data.u8[0]) << std::endl;
    EXPECT_TRUE(entry.data.u8[0] == OHOS_CAMERA_TYPE_UNSPECIFIED);
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_ABILITY_STREAM_AVAILABLE_BASIC_CONFIGURATIONS
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0160, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_STREAM_AVAILABLE_BASIC_CONFIGURATIONS, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "OHOS_ABILITY_STREAM_AVAILABLE_BASIC_CONFIGURATIONS value is "
        << static_cast<int>(entry.data.u8[0]) << std::endl;
    EXPECT_TRUE(entry.data.u8[0] == OHOS_CAMERA_FORMAT_RGBA_8888);
}

/**
  * @tc.name: USB Camera
  * @tc.desc: UpdateSettings, fps=10,default width = 640, height =480.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0170, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    // Get the device manager
    display_->OpenUsbCamera();
    // Create and get streamOperator information
    display_->AchieveStreamOperator();
    // Create data stream
    display_->StartStreamUpdate(640,480);
    // updateSettings
    const uint32_t ITEM_CAPACITY = 100;
    const uint32_t DATA_CAPACITY = 2000;
    const int32_t FPS_VALUE = 10;// 10:fps
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(
        ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<int32_t> fpsRange;
    fpsRange.push_back(FPS_VALUE);
    fpsRange.push_back(FPS_VALUE);
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, fpsRange.data(), fpsRange.size());
    const int32_t DEVICE_STREAM_ID = 0;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &DEVICE_STREAM_ID, 1);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    display_->rc = (CamRetCode)display_->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);

    // capture
    display_->StartCapture(display_->STREAM_ID_PREVIEW, display_->CAPTURE_ID_PREVIEW, false, true);
    // release stream
    display_->captureIds = {display_->CAPTURE_ID_PREVIEW};
    display_->streamIds = {display_->STREAM_ID_PREVIEW};
    display_->StopStream(display_->captureIds, display_->streamIds);
}

/**
  * @tc.name: USB Camera
  * @tc.desc: UpdateSettings, fps=30,default width = 640, height =480.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0180, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    // Get the device manager
    display_->OpenUsbCamera();
    // Create and get streamOperator information
    display_->AchieveStreamOperator();
    // Create data stream
    display_->StartStreamUpdate(640,480);
    // updateSettings
    const uint32_t ITEM_CAPACITY = 100;
    const uint32_t DATA_CAPACITY = 2000;
    const int32_t FPS_VALUE = 30;// 30:fps
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(
        ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<int32_t> fpsRange;
    fpsRange.push_back(FPS_VALUE);
    fpsRange.push_back(FPS_VALUE);
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, fpsRange.data(), fpsRange.size());
    const int32_t DEVICE_STREAM_ID = 0;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &DEVICE_STREAM_ID, 1);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    display_->rc = (CamRetCode)display_->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);

    // capture
    display_->StartCapture(display_->STREAM_ID_PREVIEW, display_->CAPTURE_ID_PREVIEW, false, true);
    // release stream
    display_->captureIds = {display_->CAPTURE_ID_PREVIEW};
    display_->streamIds = {display_->STREAM_ID_PREVIEW};
    display_->StopStream(display_->captureIds, display_->streamIds);
}
/**
  * @tc.name: USB Camera
  * @tc.desc: Preview stream, width = 800, height =600, fps=10.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0190, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    // Get the device manager
    display_->OpenUsbCamera();
    // Create and get streamOperator information
    display_->AchieveStreamOperator();
    // Create data stream
    display_->StartStreamUpdate(800,600);
    // updateSettings
    const uint32_t ITEM_CAPACITY = 100;
    const uint32_t DATA_CAPACITY = 2000;
    const int32_t FPS_VALUE = 10;// 10:fps
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(
        ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<int32_t> fpsRange;
    fpsRange.push_back(FPS_VALUE);
    fpsRange.push_back(FPS_VALUE);
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, fpsRange.data(), fpsRange.size());
    const int32_t DEVICE_STREAM_ID = 0;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &DEVICE_STREAM_ID, 1);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    display_->rc = (CamRetCode)display_->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);

    // capture
    display_->StartCapture(display_->STREAM_ID_PREVIEW, display_->CAPTURE_ID_PREVIEW, false, true);
    // release stream
    display_->captureIds = {display_->CAPTURE_ID_PREVIEW};
    display_->streamIds = {display_->STREAM_ID_PREVIEW};
    display_->StopStream(display_->captureIds, display_->streamIds);
}

    /**
    * @tc.name: USB Camera
    * @tc.desc: Preview stream, width = 352, height =288, fps=30.
    * @tc.level: Level0
    * @tc.size: MediumTest
    * @tc.type: Function
    */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0200, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    // Get the device manager
    display_->OpenUsbCamera();
    // Create and get streamOperator information
    display_->AchieveStreamOperator();
    // Create data stream
    display_->StartStreamUpdate(352,288);
     // updateSettings
    const uint32_t ITEM_CAPACITY = 100;
    const uint32_t DATA_CAPACITY = 2000;
    const int32_t FPS_VALUE = 30;// 30:fps
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(
        ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<int32_t> fpsRange;
    fpsRange.push_back(FPS_VALUE);
    fpsRange.push_back(FPS_VALUE);
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, fpsRange.data(), fpsRange.size());
    const int32_t DEVICE_STREAM_ID = 0;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &DEVICE_STREAM_ID, 1);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    display_->rc = (CamRetCode)display_->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);

    // capture
    display_->StartCapture(display_->STREAM_ID_PREVIEW, display_->CAPTURE_ID_PREVIEW, false, true);
    // release stream
    display_->captureIds = {display_->CAPTURE_ID_PREVIEW};
    display_->streamIds = {display_->STREAM_ID_PREVIEW};
    display_->StopStream(display_->captureIds, display_->streamIds);
}

/**
  * @tc.name: USB Camera
  * @tc.desc: Preview stream, width = 320, height =240, fps=30.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0210, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    // Get the device manager
    display_->OpenUsbCamera();
    // Create and get streamOperator information
    display_->AchieveStreamOperator();
    // Create data stream
    display_->StartStreamUpdate(320,240);
    // updateSettings
    const uint32_t ITEM_CAPACITY = 100;
    const uint32_t DATA_CAPACITY = 2000;
    const int32_t FPS_VALUE = 30;// 30:fps
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(
        ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<int32_t> fpsRange;
    fpsRange.push_back(FPS_VALUE);
    fpsRange.push_back(FPS_VALUE);
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, fpsRange.data(), fpsRange.size());
    const int32_t DEVICE_STREAM_ID = 0;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &DEVICE_STREAM_ID, 1);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    display_->rc = (CamRetCode)display_->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);

    // capture
    display_->StartCapture(display_->STREAM_ID_PREVIEW, display_->CAPTURE_ID_PREVIEW, false, true);
    // release stream
    display_->captureIds = {display_->CAPTURE_ID_PREVIEW};
    display_->streamIds = {display_->STREAM_ID_PREVIEW};
    display_->StopStream(display_->captureIds, display_->streamIds);
}
    /**
  * @tc.name: USB Camera
  * @tc.desc: Preview stream, width = 176, height =144, fps=30.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0220, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    // Get the device manager
    display_->OpenUsbCamera();
    // Create and get streamOperator information
    display_->AchieveStreamOperator();
    // Create data stream
    display_->StartStreamUpdate(176,144);
    // updateSettings
    const uint32_t ITEM_CAPACITY = 100;
    const uint32_t DATA_CAPACITY = 2000;
    const int32_t FPS_VALUE = 30;// 30:fps
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(
        ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<int32_t> fpsRange;
    fpsRange.push_back(FPS_VALUE);
    fpsRange.push_back(FPS_VALUE);
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, fpsRange.data(), fpsRange.size());
    const int32_t DEVICE_STREAM_ID = 0;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &DEVICE_STREAM_ID, 1);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    display_->rc = (CamRetCode)display_->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);
    
    // capture
    display_->StartCapture(display_->STREAM_ID_PREVIEW, display_->CAPTURE_ID_PREVIEW, false, true);
    // release stream
    display_->captureIds = {display_->CAPTURE_ID_PREVIEW};
    display_->streamIds = {display_->STREAM_ID_PREVIEW};
    display_->StopStream(display_->captureIds, display_->streamIds);
}
        /**
  * @tc.name: USB Camera
  * @tc.desc: Preview stream, width = 160, height =120, fps=30.
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0230, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    // Get the device manager
    display_->OpenUsbCamera();
    // Create and get streamOperator information
    display_->AchieveStreamOperator();
    // Create data stream
    display_->StartStreamUpdate(160,120);
    // updateSettings
    const uint32_t ITEM_CAPACITY = 100;
    const uint32_t DATA_CAPACITY = 2000;
    const int32_t FPS_VALUE = 30;// 30:fps
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(
        ITEM_CAPACITY, DATA_CAPACITY);
    std::vector<int32_t> fpsRange;
    fpsRange.push_back(FPS_VALUE);
    fpsRange.push_back(FPS_VALUE);
    meta->addEntry(OHOS_CONTROL_FPS_RANGES, fpsRange.data(), fpsRange.size());
    const int32_t DEVICE_STREAM_ID = 0;
    meta->addEntry(OHOS_CAMERA_STREAM_ID, &DEVICE_STREAM_ID, 1);
    std::vector<uint8_t> setting;
    MetadataUtils::ConvertMetadataToVec(meta, setting);
    display_->rc = (CamRetCode)display_->cameraDevice->UpdateSettings(setting);
    EXPECT_EQ(true, display_->rc == HDI::Camera::V1_0::NO_ERROR);

    // capture
    display_->StartCapture(display_->STREAM_ID_PREVIEW, display_->CAPTURE_ID_PREVIEW, false, true);
    // release stream
    display_->captureIds = {display_->CAPTURE_ID_PREVIEW};
    display_->streamIds = {display_->STREAM_ID_PREVIEW};
    display_->StopStream(display_->captureIds, display_->streamIds);
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_JPEG_ORIENTATION
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, camera_usb_0025, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_JPEG_ORIENTATION, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "OHOS_JPEG_ORIENTATION value is " << static_cast<int>(entry.data.i32[0]) << std::endl;
}

/**
  * @tc.name: USB Camera
  * @tc.desc: get value of OHOS_JPEG_QUALITY
  * @tc.level: Level0
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(USBCameraTest, SUB_DriverSystem_UsbCameraHdi_0260, TestSize.Level1)
{
    if (!usbCameraExit_) {
        GTEST_SKIP() << "No usb camera plugged in" << std::endl;
    }
    ability_ = display_->GetCameraAbility();
    EXPECT_NE(ability_, nullptr);
    common_metadata_header_t *data = ability_->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_JPEG_QUALITY, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "OHOS_JPEG_ORIENTATION value is " << static_cast<int>(entry.data.u8[0]) << std::endl;
}

