/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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
#include "front_camera_tag_test_v1_2.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;
constexpr uint32_t ITEM_CAPACITY = 100;
constexpr uint32_t DATA_CAPACITY = 2000;
void FrontCameraTagTestV1_2::SetUpTestCase(void) {}
void FrontCameraTagTestV1_2::TearDownTestCase(void) {}
void FrontCameraTagTestV1_2::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
    cameraTest->Init(); // assert inside
    cameraTest->Open(DEVICE_1); // assert inside
}

void FrontCameraTagTestV1_2::TearDown(void)
{
    cameraTest->Close();
}

bool g_isFrontTagValueExistsU8(std::shared_ptr<CameraMetadata> ability, uint32_t tag, uint8_t value)
{
    common_metadata_header_t* data = ability->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, tag, &entry);
    if (ret == 0 && entry.count != 0) {
        for (int i = 0; i < entry.count; i++) {
            if (entry.data.u8[i] == value) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @tc.name: SUB_Driver_Camera_Modes_0050
 * @tc.desc: OHOS_ABILITY_SCENE_BEAUTY_TYPES, OHOS_CONTROL_BEAUTY_TYPE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(FrontCameraTagTestV1_2, SUB_Driver_Camera_Modes_0050, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_SCENE_BEAUTY_TYPES, &entry);
    if (ret != 0) {
        GTEST_SKIP() << "OHOS_ABILITY_SCENE_BEAUTY_TYPES not found" << std::endl;
    }
    CAMERA_LOGI("OHOS_ABILITY_SCENE_BEAUTY_TYPES found");
    printf("OHOS_ABILITY_SCENE_BEAUTY_TYPES value count is %d\n", entry.count);
    if (entry.count == 0) {
        GTEST_SKIP() << "OHOS_ABILITY_SCENE_BEAUTY_TYPES value count is 0" << std::endl;
    } else {
        for (size_t i = 0; i < entry.count; i++) {
            std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
            printf("OHOS_ABILITY_SCENE_BEAUTY_VALUES : %d\n", entry.data.u8[i]);
            uint8_t value = entry.data.u8[i];
            meta->addEntry(OHOS_CONTROL_BEAUTY_TYPE, &value, 1);
            std::vector<uint8_t> metaVec;
            MetadataUtils::ConvertMetadataToVec(meta, metaVec);
            cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
            EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
            CAMERA_LOGI("addEntry for OHOS_CONTROL_BEAUTY_TYPE success!");
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Modes_0070
 * @tc.desc: OHOS_ABILITY_BEAUTY_FACE_SLENDER_VALUES, OHOS_CONTROL_BEAUTY_FACE_SLENDER_VALUE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(FrontCameraTagTestV1_2, SUB_Driver_Camera_Modes_0070, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    camera_metadata_item_t entry;
    bool beautyFaceSlenderFlag = g_isFrontTagValueExistsU8(cameraTest->ability, OHOS_ABILITY_SCENE_BEAUTY_TYPES,
        OHOS_CAMERA_BEAUTY_TYPE_FACE_SLENDER);
    if (!beautyFaceSlenderFlag) {
        GTEST_SKIP() << "OHOS_CAMERA_BEAUTY_TYPE_FACE_SLENDER not found" << std::endl;
    }

    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_BEAUTY_FACE_SLENDER_VALUES, &entry);
    if (ret != 0) {
        GTEST_SKIP() << "OHOS_ABILITY_BEAUTY_FACE_SLENDER_VALUES not found" << std::endl;
    }
    printf("OHOS_ABILITY_BEAUTY_FACE_SLENDER_VALUES value count is %d\n", entry.count);
    if (entry.count == 0) {
        GTEST_SKIP() << "OHOS_ABILITY_BEAUTY_FACE_SLENDER_VALUES value count is 0" << std::endl;
    } else {
        for (size_t i = 0; i < entry.count; i++) {
            std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
            printf("OHOS_ABILITY_BEAUTY_FACE_SLENDER_VALUES : %d\n", entry.data.u8[i]);
            uint8_t value = entry.data.u8[i];
            meta->addEntry(OHOS_CONTROL_BEAUTY_FACE_SLENDER_VALUE, &value, 1);
            std::vector<uint8_t> metaVec;
            MetadataUtils::ConvertMetadataToVec(meta, metaVec);
            cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
            EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
            CAMERA_LOGI("addEntry for OHOS_CONTROL_BEAUTY_FACE_SLENDER_VALUE success!");
            cameraTest->TakePhotoWithTags(meta);
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Modes_0080
 * @tc.desc: OHOS_ABILITY_BEAUTY_SKIN_TONE_VALUES, OHOS_CONTROL_BEAUTY_SKIN_TONE_VALUE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(FrontCameraTagTestV1_2, SUB_Driver_Camera_Modes_0080, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    camera_metadata_item_t entry;
    bool beautyFaceSlenderFlag = g_isFrontTagValueExistsU8(cameraTest->ability,
        OHOS_ABILITY_SCENE_BEAUTY_TYPES, OHOS_CAMERA_BEAUTY_TYPE_SKIN_TONE);
    if (!beautyFaceSlenderFlag) {
        GTEST_SKIP() << "OHOS_CAMERA_BEAUTY_TYPE_SKIN_TONE not found" << std::endl;
    }
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_BEAUTY_SKIN_TONE_VALUES, &entry);
    if (ret != 0) {
        GTEST_SKIP() << "OHOS_ABILITY_BEAUTY_SKIN_TONE_VALUES not found" << std::endl;
    }
    printf("OHOS_ABILITY_BEAUTY_SKIN_TONE_VALUES value count is %d\n", entry.count);
    if (entry.count == 0) {
        GTEST_SKIP() << "OHOS_ABILITY_BEAUTY_SKIN_TONE_VALUES value count is 0" << std::endl;
    } else {
        for (size_t i = 0; i < entry.count; i++) {
            std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
            printf("OHOS_ABILITY_BEAUTY_SKIN_TONE_VALUES : %d\n", entry.data.i32[i]);
            int32_t value = entry.data.i32[i];
            meta->addEntry(OHOS_CONTROL_BEAUTY_SKIN_TONE_VALUE, &value, 1);
            std::vector<uint8_t> metaVec;
            MetadataUtils::ConvertMetadataToVec(meta, metaVec);
            cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
            EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
            CAMERA_LOGI("addEntry for OHOS_CONTROL_BEAUTY_SKIN_TONE_VALUE success!");
            cameraTest->TakePhotoWithTags(meta);
        }
    }
}

/**
 * @tc.name: SUB_Driver_Camera_Modes_0090
 * @tc.desc: OHOS_ABILITY_BEAUTY_SKIN_SMOOTH_VALUES, OHOS_CONTROL_BEAUTY_SKIN_SMOOTH_VALUE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(FrontCameraTagTestV1_2, SUB_Driver_Camera_Modes_0090, TestSize.Level1)
{
    common_metadata_header_t* data = cameraTest->ability->get();
    camera_metadata_item_t entry;
    bool beautyFaceSlenderFlag = g_isFrontTagValueExistsU8(cameraTest->ability,
        OHOS_ABILITY_SCENE_BEAUTY_TYPES, OHOS_CAMERA_BEAUTY_TYPE_SKIN_SMOOTH);
    if (!beautyFaceSlenderFlag) {
        GTEST_SKIP() << "OHOS_CAMERA_BEAUTY_TYPE_SKIN_SMOOTH value count is 0" << std::endl;
    }
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_BEAUTY_SKIN_SMOOTH_VALUES, &entry);
    if (ret != 0) {
        GTEST_SKIP() << "OHOS_ABILITY_BEAUTY_SKIN_SMOOTH_VALUES value not found" << std::endl;
    }
    printf("OHOS_ABILITY_BEAUTY_SKIN_SMOOTH_VALUES value count is %d\n", entry.count);
    if (entry.count == 0) {
        GTEST_SKIP() << "OHOS_ABILITY_BEAUTY_SKIN_SMOOTH_VALUES value count is 0" << std::endl;
        return;
    } else {
        for (size_t i = 0; i < entry.count; i++) {
            std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
            printf("OHOS_ABILITY_BEAUTY_SKIN_SMOOTH_VALUES : %d\n", entry.data.u8[i]);
            uint8_t value = entry.data.u8[i];
            meta->addEntry(OHOS_CONTROL_BEAUTY_SKIN_SMOOTH_VALUE, &value, 1);
            std::vector<uint8_t> metaVec;
            MetadataUtils::ConvertMetadataToVec(meta, metaVec);
            cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
            EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
            CAMERA_LOGI("addEntry for OHOS_CONTROL_BEAUTY_SKIN_SMOOTH_VALUE success!");
            cameraTest->TakePhotoWithTags(meta);
        }
    }
}