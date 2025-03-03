/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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
#include "camera_tag_test_v1_3.h"
#include <functional>
using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;
constexpr uint32_t ITEM_CAPACITY = 100;
constexpr uint32_t DATA_CAPACITY = 2000;
constexpr uint32_t DATA_COUNT = 1;
constexpr uint32_t FPS_COUNT = 2;
constexpr uint32_t EXPOSURE_COUNT = 4;
constexpr uint32_t CHECK_AREA_COUNT = 4;
constexpr uint32_t STREAMINFO_WIDTH = 1920;
constexpr uint32_t STREAMINFO_HEIGHT = 1080;
constexpr uint32_t HIGH_RESOLUTION_PHOTO_WIDTH = 8192;
constexpr uint32_t HIGH_RESOLUTION_PHOTO_HEIGHT = 6144;
void CameraTagTestV1_3::SetUpTestCase(void) {}
void CameraTagTestV1_3::TearDownTestCase(void) {}
void CameraTagTestV1_3::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init();
    cameraTest->Open(DEVICE_0);
}

void CameraTagTestV1_3::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.name: SUB_Driver_Camera_SlowMotion_0100
 * @tc.desc: Get and Print all data in OHOS_ABILITY_CAMERA_MODES
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_SlowMotion_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    (void)ITEM_CAPACITY;
    (void)DATA_CAPACITY;
    (void)DATA_COUNT;
    (void)FPS_COUNT;
    (void)EXPOSURE_COUNT;
    (void)CHECK_AREA_COUNT;
    (void)STREAMINFO_WIDTH;
    (void)STREAMINFO_HEIGHT;
    (void)HIGH_RESOLUTION_PHOTO_WIDTH;
    (void)HIGH_RESOLUTION_PHOTO_HEIGHT;
    cameraTest->PrintAllTagDataU8(cameraTest->ability, OHOS_ABILITY_CAMERA_MODES);
}

/**
 * @tc.name:SUB_Driver_Camera_Moon_0100
 * @tc.desc:Whether moon ability support
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_Moon_0100, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_MOON_CAPTURE_BOOST, &entry);
    printf("OHOS_ABILITY_MOON_CAPTURE_BOOST value count %d\n", entry.count);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << static_cast<int>(entry.data.u8[i]) << " ";
            if (i == entry.count - 1) {
                printf("OHOS_ABILITY_MOON_CAPTURE_BOOST: %s\n", ss.str().c_str());
                ss.clear();
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Moving_0100
 * @tc.desc: OHOS_ABILITY_MOVING_PHOTO
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_Moving_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;

    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_MOVING_PHOTO, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        printf("OHOS_ABILITY_MOVING_PHOTO is not support");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR && entry.data.i32 != nullptr && entry.count > 0) {
        CAMERA_LOGI("print tag<OHOS_ABILITY_MOVING_PHOTO> i32 value start.");
        printf("OHOS_ABILITY_MOVING_PHOTO i32 value count %d\n", entry.count);
        constexpr size_t step = 10;
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.i32[i] << " ";
            if ((i != 0) && (i % step == 0 || i == entry.count - 1)) {
                CAMERA_LOGI("%{public}s\n", ss.str().c_str());
                printf("OHOS_ABILITY_MOVING_PHOTO %s\n", ss.str().c_str());
                ss.clear();
                ss.str("");
            }
        }
        CAMERA_LOGI("print tag<OHOS_ABILITY_MOVING_PHOTO> i32 value end.");
    }
}

/**
 * @tc.name:SUB_Driver_Camera_HighResolution_0100
 * @tc.desc:Determine whether the HIGH_RESOLUTION_PHOTO mode is supported
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_HighResolution_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CAMERA_MODES is can not be found");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.u8, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        for (size_t i = 0; i < entry.count; i++) {
            uint8_t value = entry.data.u8[i];
            if (value == OHOS::HDI::Camera::V1_3::HIGH_RESOLUTION_PHOTO) {
                printf("HIGH_RESOLUTION_PHOTO mode is supported");
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_SlowMotion_0200
 * @tc.desc: Determine whether the HIGH_FRAME_RATE mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_SlowMotion_0200, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CAMERA_MODES is can not be found");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.u8, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        for (size_t i = 0; i < entry.count; i++) {
            uint8_t value = entry.data.u8[i];
            if (value == OHOS::HDI::Camera::V1_3::HIGH_FRAME_RATE) {
                printf("HIGH_FRAME_RATE mode is supported");
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_SuperSlowMotion_0100
 * @tc.desc: Determine whether the SLOW_MOTION mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_SuperSlowMotion_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CAMERA_MODES is can not be found");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.u8, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        for (size_t i = 0; i < entry.count; i++) {
            uint8_t value = entry.data.u8[i];
            if (value == OHOS::HDI::Camera::V1_3::SLOW_MOTION) {
                printf("SLOW_MOTION mode is supported");
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_SMPhoto_0100
 * @tc.desc: Determine whether the CAPTURE_MACRO mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_SMPhoto_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CAMERA_MODES is can not be found");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.u8, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        for (size_t i = 0; i < entry.count; i++) {
            uint8_t value = entry.data.u8[i];
            if (value == OHOS::HDI::Camera::V1_3::CAPTURE_MACRO) {
                printf("CAPTURE_MACRO mode is supported");
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_SMVideo_0100
 * @tc.desc: Determine whether the VIDEO_MACRO mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_SMVideo_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CAMERA_MODES is can not be found");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.u8, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        for (size_t i = 0; i < entry.count; i++) {
            uint8_t value = entry.data.u8[i];
            if (value == OHOS::HDI::Camera::V1_3::VIDEO_MACRO) {
                printf("VIDEO_MACRO mode is supported");
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_SecureStream_0100
 * @tc.desc: Determine whether the SECURE mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_SecureStream_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CAMERA_MODES is can not be found");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.u8, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        for (size_t i = 0; i < entry.count; i++) {
            uint8_t value = entry.data.u8[i];
            if (value == OHOS::HDI::Camera::V1_3::SECURE) {
                printf("SECURE mode is supported");
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalPhoto_0100
 * @tc.desc: Determine whether the PROFESSIONAL_PHOTO mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalPhoto_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CAMERA_MODES is can not be found");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.u8, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        for (size_t i = 0; i < entry.count; i++) {
            uint8_t value = entry.data.u8[i];
            if (value == OHOS::HDI::Camera::V1_3::PROFESSIONAL_PHOTO) {
                printf("PROFESSIONAL_PHOTO mode is supported");
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_0200
 * @tc.desc: Determine whether the OHOS_ABILITY_FLASH_MODES mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_0200, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_FLASH_MODES, &entry);
    printf("OHOS_ABILITY_FLASH_MODES value count %d\n", entry.count);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << static_cast<int>(entry.data.u8[i]) << " ";
            if (i == entry.count - 1) {
                printf("OHOS_ABILITY_FLASH_MODES: %s\n", ss.str().c_str());
                ss.clear();
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_0300
 * @tc.desc: Determine whether the OHOS_ABILITY_SCENE_ZOOM_CAP mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_0300, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_SCENE_ZOOM_CAP, &entry);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.i32 != nullptr && entry.count > 0) {
        CAMERA_LOGI("print tag<OHOS_ABILITY_SCENE_ZOOM_CAP> i32 value start.");
        printf("OHOS_ABILITY_SCENE_ZOOM_CAP i32 value count %d\n", entry.count);
        constexpr size_t step = 4; // print step
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.i32[i] << " ";
            if ((i != 0) && (i % step == 0 || i == entry.count - 1)) {
                CAMERA_LOGI("%{public}s\n", ss.str().c_str());
                printf("OHOS_ABILITY_SCENE_ZOOM_CAP %s\n", ss.str().c_str());
                ss.clear();
                ss.str("");
            }
        }
        CAMERA_LOGI("print tag<OHOS_ABILITY_SCENE_ZOOM_CAP> f value end.");
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_0400
 * @tc.desc: Determine whether the OHOS_ABILITY_METER_MODES mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_0400, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_METER_MODES, &entry);
    printf("OHOS_ABILITY_METER_MODES value count %d\n", entry.count);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << static_cast<int>(entry.data.u8[i]) << " ";
            if (i == entry.count - 1) {
                printf("OHOS_ABILITY_METER_MODES: %s\n", ss.str().c_str());
                ss.clear();
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_0500
 * @tc.desc: Determine whether the OHOS_ABILITY_FOCUS_MODES mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_0500, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_FOCUS_MODES, &entry);
    printf("OHOS_ABILITY_FOCUS_MODES value count %d\n", entry.count);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << static_cast<int>(entry.data.u8[i]) << " ";
            if (i == entry.count - 1) {
                printf("OHOS_ABILITY_FOCUS_MODES: %s\n", ss.str().c_str());
                ss.clear();
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_0600
 * @tc.desc: Determine whether the OHOS_ABILITY_SENSOR_EXPOSURE_TIME_RANGE mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_0600, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_SENSOR_EXPOSURE_TIME_RANGE, &entry);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.f != nullptr && entry.count > 0) {
        CAMERA_LOGI("print tag<OHOS_ABILITY_SENSOR_EXPOSURE_TIME_RANGE> f value start.");
        printf("OHOS_ABILITY_SENSOR_EXPOSURE_TIME_RANGE f value count %d\n", entry.count);
        constexpr size_t step = 4; // print step
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.r[i].numerator << "/" << entry.data.r[i].denominator <<" ";
            if ((i != 0) && (i % step == 0 || i == entry.count - 1)) {
                CAMERA_LOGI("%{public}s\n", ss.str().c_str());
                printf("OHOS_ABILITY_SENSOR_EXPOSURE_TIME_RANGE %s\n", ss.str().c_str());
                ss.clear();
                ss.str("");
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_0700
 * @tc.desc: Determine whether the OHOS_ABILITY_AE_COMPENSATION_RANGE mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_0700, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_AE_COMPENSATION_RANGE, &entry);
    printf("OHOS_ABILITY_AE_COMPENSATION_RANGE value count %d\n", entry.count);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.i32 != nullptr && entry.count > 0) {
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.i32[i] << " ";
            if (i == entry.count - 1) {
                printf("OHOS_ABILITY_AE_COMPENSATION_RANGE: %s\n", ss.str().c_str());
                ss.clear();
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_0800
 * @tc.desc: Determine whether the OHOS_ABILITY_SENSOR_EXPOSURE_TIME_RANGE mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_0800, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_AE_COMPENSATION_STEP, &entry);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.r != nullptr && entry.count > 0) {
        CAMERA_LOGI("print tag<OHOS_ABILITY_AE_COMPENSATION_STEP> f value start.");
        printf("OHOS_ABILITY_AE_COMPENSATION_STEP f value count %d\n", entry.count);
        if (entry.count == 1) {
            CAMERA_LOGI("%{public}d/%{public}d\n", entry.data.r[0].numerator, entry.data.r[0].denominator);
            printf("OHOS_ABILITY_AE_COMPENSATION_STEP %d/%d\n", entry.data.r[0].numerator, entry.data.r[0].denominator);
            return;
        }
        constexpr size_t step = 4; // print step
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.r[i].numerator << "/" << entry.data.r[i].denominator <<" ";
            if ((i != 0) && (i % step == 0 || i == entry.count - 1)) {
                CAMERA_LOGI("%{public}s\n", ss.str().c_str());
                printf("OHOS_ABILITY_AE_COMPENSATION_STEP %s\n", ss.str().c_str());
                ss.clear();
                ss.str("");
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_0900
 * @tc.desc: Determine whether the OHOS_ABILITY_LENS_INFO_MINIMUM_FOCUS_DISTANCE mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_0900, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_LENS_INFO_MINIMUM_FOCUS_DISTANCE, &entry);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.f != nullptr && entry.count > 0) {
        std::cout << "LENS_INFO_MINIMUM_FOCUS_DISTANCE:" << static_cast<int>(entry.data.f[0]) << std::endl;
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_1000
 * @tc.desc: Determine whether the OHOS_ABILITY_SENSOR_WB_VALUES mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_1000, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_SENSOR_WB_VALUES, &entry);
    printf("OHOS_ABILITY_SENSOR_WB_VALUES value count %d\n", entry.count);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.i32 != nullptr && entry.count > 0) {
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.i32[i] << " ";
            if (i == entry.count - 1) {
                printf("OHOS_ABILITY_SENSOR_WB_VALUES: %s\n", ss.str().c_str());
                ss.clear();
            }
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_ProfessionalVideo_0100
 * @tc.desc: Determine whether the PROFESSIONAL_VIDEO mode is supported
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ProfessionalVideo_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CAMERA_MODES is can not be found");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.u8, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        for (size_t i = 0; i < entry.count; i++) {
            uint8_t value = entry.data.u8[i];
            if (value == OHOS::HDI::Camera::V1_3::PROFESSIONAL_VIDEO) {
                printf("PROFESSIONAL_VIDEO mode is supported");
            }
        }
    }
}

/**
 * @tc.name:SUB_Driver_Camera_ZoomBand_0100
 * @tc.desc:OHOS_ABILITY_EQUIVALENT_FOCUS
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_ZoomBand_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_EQUIVALENT_FOCUS, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_EQUIVALENT_FOCUS can not be find");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.i32, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        CAMERA_LOGI("print tag<OHOS_ABILITY_EQUIVALENT_FOCUS> value start.");
        constexpr size_t step = 10; // print step
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.i32[i] << " ";
            if ((i != 0) && (i % step == 0 || i == entry.count - 1)) {
                CAMERA_LOGI("%{public}s\n", ss.str().c_str());
                ss.clear();
                ss.str("");
            }
        }
        CAMERA_LOGI("print tag<OHOS_ABILITY_EQUIVALENT_FOCUS> value end.");
    }
}
/**
 * @tc.name: SUB_Driver_Camera_HighQuality_0100
 * @tc.desc: Whether HIGH_QUALITY_SUPPORT ability support
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_HighQuality_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_HIGH_QUALITY_SUPPORT, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        printf("OHOS_ABILITY_HIGH_QUALITY_SUPPORT is not support");
        return;
    }
    cameraTest->PrintAllTagDataU8(cameraTest->ability, OHOS_ABILITY_HIGH_QUALITY_SUPPORT);
}

/**
 * @tc.name: SUB_Driver_Camera_Tripod_0100
 * @tc.desc: OHOS_ABILITY_TRIPOD_DETECTION
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_Tripod_0100, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_TRIPOD_DETECTION, &entry);
    printf("OHOS_ABILITY_TRIPOD_DETECTION value count %d\n", entry.count);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << static_cast<int>(entry.data.u8[i]) << " ";
            if (i == entry.count - 1) {
                printf("OHOS_ABILITY_TRIPOD_DETECTION: %s\n", ss.str().c_str());
                ss.clear();
            }
        }
    }
}

/**
 * @tc.name:SUB_Driver_Camera_conflict_0100
 * @tc.desc:OHOS_ABILITY_AVAILABLE_PROFILE_LEVEL
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_conflict_0100, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_AVAILABLE_PROFILE_LEVEL, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_AVAILABLE_PROFILE_LEVEL is not support");
        return;
    }
    CAMERA_LOGI("print tag<OHOS_ABILITY_AVAILABLE_PROFILE_LEVEL> value start.");
    constexpr size_t step = 20; // print step
    std::stringstream ss;
    if (entry.data.i32 != nullptr && entry.count > 0) {
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.i32[i] << " ";
            if ((i != 0) && (i % step == 0 || i == entry.count - 1)) {
                printf("OHOS_ABILITY_AVAILABLE_PROFILE_LEVEL: %s\n", ss.str().c_str());
                ss.clear();
                ss.str("");
            }
        }
    } else {
        printf("get tag<OHOS_ABILITY_AVAILABLE_PROFILE_LEVEL> failed.\n");
        CAMERA_LOGE("get tag<OHOS_ABILITY_AVAILABLE_PROFILE_LEVEL> failed.");
    }
    CAMERA_LOGI("print tag<OHOS_ABILITY_AVAILABLE_PROFILE_LEVEL> value end.");
}

/**
 * @tc.name:SUB_Driver_Camera_conflict_0200
 * @tc.desc:OHOS_ABILITY_AVAILABLE_CONFIGURATIONS
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_conflict_0200, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_AVAILABLE_CONFIGURATIONS, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_AVAILABLE_CONFIGURATIONS is not support");
        return;
    }
    CAMERA_LOGI("print tag<OHOS_ABILITY_AVAILABLE_CONFIGURATIONS> value start.");
    constexpr size_t step = 20; // print step
    std::stringstream ss;
    if (entry.data.i32 != nullptr && entry.count > 0) {
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.i32[i] << " ";
            if ((i != 0) && (i % step == 0 || i == entry.count - 1)) {
                printf("OHOS_ABILITY_AVAILABLE_CONFIGURATIONS: %s\n", ss.str().c_str());
                ss.clear();
                ss.str("");
            }
        }
    } else {
        printf("get tag<OHOS_ABILITY_AVAILABLE_CONFIGURATIONS> failed.\n");
        CAMERA_LOGE("get tag<OHOS_ABILITY_AVAILABLE_CONFIGURATIONS> failed.");
    }
    CAMERA_LOGI("print tag<OHOS_ABILITY_AVAILABLE_CONFIGURATIONS> value end.");
}

/**
 * @tc.name:SUB_Driver_Camera_conflict_0300
 * @tc.desc:OHOS_ABILITY_CONFLICT_CONFIGURATIONS
 * @tc.size:MediumTest
 * @tc.type:Function
*/
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_conflict_0300, TestSize.Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CONFLICT_CONFIGURATIONS, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CONFLICT_CONFIGURATIONS is not support");
        return;
    }
    CAMERA_LOGI("print tag<OHOS_ABILITY_CONFLICT_CONFIGURATIONS> value start.");
    constexpr size_t step = 20; // print step
    std::stringstream ss;
    if (entry.data.i32 != nullptr && entry.count > 0) {
        for (size_t i = 0; i < entry.count; i++) {
            ss << entry.data.i32[i] << " ";
            if ((i != 0) && (i % step == 0 || i == entry.count - 1)) {
                printf("OHOS_ABILITY_CONFLICT_CONFIGURATIONS: %s\n", ss.str().c_str());
                ss.clear();
                ss.str("");
            }
        }
    } else {
        printf("get tag<OHOS_ABILITY_CONFLICT_CONFIGURATIONS> failed.\n");
        CAMERA_LOGE("get tag<OHOS_ABILITY_CONFLICT_CONFIGURATIONS> failed.");
    }
    CAMERA_LOGI("print tag<OHOS_ABILITY_CONFLICT_CONFIGURATIONS> value end.");
}

/**
 * @tc.name: SUB_Driver_Camera_Tripod_0100
 * @tc.desc: OHOS_ABILITY_STATISTICS_DETECT_TYPE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_Detect_0100, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_STATISTICS_DETECT_TYPE, &entry);
    printf("OHOS_ABILITY_STATISTICS_DETECT_TYPE value count %d\n", entry.count);
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        std::stringstream ss;
        for (size_t i = 0; i < entry.count; i++) {
            ss << static_cast<int>(entry.data.u8[i]) << " ";
            if (i == entry.count - 1) {
                printf("OHOS_ABILITY_STATISTICS_DETECT_TYPE: %s\n", ss.str().c_str());
                ss.clear();
            }
        }
    }
}

/**
 * @tc.number: SUB_Driver_Camera_LightPainting_0100
 * @tc.name: Querying the LIGHT_PAINTING Mode 
 * @tc.desc: Determine Whether the LIGHT_PAINTING mode is supported
*/
HWTEST_F(CameraTagTestV1_3, SUB_Driver_Camera_LightPainting_0100, Function | MediumTest | Level1)
{
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    cameraTest->rc = FindCameraMetadataItem(data, OHOS_ABILITY_CAMERA_MODES, &entry);
    if (cameraTest->rc != HDI::Camera::V1_0::NO_ERROR) {
        CAMERA_LOGI("OHOS_ABILITY_CAMERA_MODES is can not be found");
        return;
    }
    if (cameraTest->rc == HDI::Camera::V1_0::NO_ERROR) {
        EXPECT_NE(entry.data.u8, nullptr);
        EXPECT_EQ(entry.count > 0, true);
        for (size_t i = 0; i < entry.count; i++ ) {
            uint8_t value = entry.data.u8[i];
            if (value == OHOS::HDI::Camera::V1_3::LIGHT_PAINTING) {
                CAMERA_LOGI("LIGHT_PAINTING mode is supported");
            }
        }
    }
}