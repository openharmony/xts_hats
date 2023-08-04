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
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "camera_hdi_uttest_v1_1.h"

using namespace OHOS;
using namespace std;
using namespace testing::ext;
using namespace OHOS::Camera;

void CameraHdiTestV1_1::SetUpTestCase(void) {}
void CameraHdiTestV1_1::TearDownTestCase(void) {}
void CameraHdiTestV1_1::SetUp(void)
{
    cameraTest = std::make_shared<OHOS::Camera::Test>();
}

void CameraHdiTestV1_1::TearDown(void)
{
    cameraTest->Close();
}

/**
 * @tc.name: Prelaunch
 * @tc.desc: Prelaunch cameraId:device/0
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_DriverSystem_CameraHdiMg_0620, TestSize.Level1)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/0";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: Prelaunch
 * @tc.desc: Prelaunch cameraId:{}
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_DriverSystem_CameraHdiMg_0630, TestSize.Level1)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = {};
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.name: Prelaunch
 * @tc.desc: Prelaunch cameraId:device/10
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_DriverSystem_CameraHdiMg_0640, TestSize.Level1)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "device/10";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.name: Prelaunch
 * @tc.desc: Prelaunch cameraId:ABC
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_DriverSystem_CameraHdiMg_0650, TestSize.Level1)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    cameraTest->prelaunchConfig = std::make_shared<OHOS::HDI::Camera::V1_1::PrelaunchConfig>();
    cameraTest->prelaunchConfig->cameraId = "ABC123";
    cameraTest->prelaunchConfig->streamInfos_V1_1 = {};
    cameraTest->prelaunchConfig->setting = {};

    cameraTest->rc = cameraTest->serviceV1_1->Prelaunch(*cameraTest->prelaunchConfig);
    EXPECT_EQ(true, cameraTest->rc == INVALID_ARGUMENT);
}

/**
 * @tc.name: SUB_DriverSystem_CameraHdiMg_0660
 * @tc.desc: OHOS_ABILITY_PRELAUNCH_AVAILABLE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_DriverSystem_CameraHdiMg_0660, TestSize.Level1)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    EXPECT_NE(cameraTest->ability, nullptr);
    common_metadata_header_t* data = cameraTest->ability->get();
    EXPECT_NE(data, nullptr);
    camera_metadata_item_t entry;
    int ret = FindCameraMetadataItem(data, OHOS_ABILITY_PRELAUNCH_AVAILABLE, &entry);
    EXPECT_EQ(ret, CAM_META_SUCCESS);
    std::cout << "OHOS_ABILITY_PRELAUNCH_AVAILABLE value is " << static_cast<int>(entry.data.u8[0]) << std::endl;
}

/**
 * @tc.name: GetDefaultSettings
 * @tc.desc: GetDefaultSettings
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_DriverSystem_CameraHdiMg_0670, TestSize.Level1)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetDefaultSettings(cameraTest->abilityVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
}

/**
 * @tc.name: GetDefaultSettings
 * @tc.desc: GetDefaultSettings
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(CameraHdiTestV1_1, SUB_DriverSystem_CameraHdiMg_0680, TestSize.Level1)
{
    cameraTest->Init();
    if (cameraTest->serviceV1_1 == nullptr) {
        return;
    }
    cameraTest->Open();
    if (cameraTest->cameraDeviceV1_1 == nullptr) {
        return;
    }
    std::shared_ptr<CameraAbility> ability;
    std::shared_ptr<CameraAbility> abilitys;
    EXPECT_EQ(true, cameraTest->cameraDevice != nullptr);
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetDefaultSettings(cameraTest->abilityVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    MetadataUtils::ConvertVecToMetadata(cameraTest->abilityVec, ability);
    common_metadata_header_t* data = ability->get();
    std::string metastring = FormatCameraMetadataToString(data);
    if (!metastring.empty()) {
        std::cout << "DefaultSettings = " << metastring << std::endl;
    }

    std::shared_ptr<CameraMetadata> modeSetting = std::make_shared<CameraMetadata>(2, 128);
    int64_t expoTime = 0;
    modeSetting->addEntry(OHOS_SENSOR_EXPOSURE_TIME, &expoTime, 1);
    std::vector<uint8_t> metaVec;
    MetadataUtils::ConvertMetadataToVec(modeSetting, metaVec);
    cameraTest->rc = cameraTest->cameraDevice->UpdateSettings(metaVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);

    cameraTest->abilityVec.clear();
    cameraTest->rc = cameraTest->cameraDeviceV1_1->GetDefaultSettings(cameraTest->abilityVec);
    EXPECT_EQ(cameraTest->rc, HDI::Camera::V1_0::NO_ERROR);
    MetadataUtils::ConvertVecToMetadata(cameraTest->abilityVec, abilitys);
    common_metadata_header_t* datas = abilitys->get();
    std::string metastrings = FormatCameraMetadataToString(datas);
    if (!metastrings.empty()) {
        std::cout << "DefaultSettings1 = " << metastrings << std::endl;
    }
}