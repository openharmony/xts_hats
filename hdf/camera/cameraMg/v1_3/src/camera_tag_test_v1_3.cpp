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

void invalidParmTestU8(int tag, uint8_t value)
{
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    uint8_t valueTest = value;
    meta->addEntry(tag, &valueTest, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    int rc = HDI::Camera::V1_0::NO_ERROR;
    EXPECT_EQ(rc, HDI::Camera::V1_0::NO_ERROR);
}

void invalidParmTestI32(int tag, int32_t value)
{
    std::shared_ptr<CameraSetting> meta = std::make_shared<CameraSetting>(ITEM_CAPACITY, DATA_CAPACITY);
    int32_t valueTest = value;
    meta->addEntry(tag, &valueTest, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(meta, metaVec);
    int rc = HDI::Camera::V1_0::NO_ERROR;
    EXPECT_EQ(rc, HDI::Camera::V1_0::NO_ERROR);
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
    if (ret == HDI::Camera::V1_0::NO_ERROR && entry.data.u8 != nullptr && entry.count > 0) {
        EXPECT_TRUE(entry.data.u8 != nullptr);
        CAMERA_LOGI("OHOS_ABILITY_MOON_CAPTURE_BOOST: %{public}d", entry.data.u8[0]);
    } else {
        CAMERA_LOGI("MoonCaptureBoost not supported");
    }
}
