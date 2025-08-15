/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include "v4_0/codec_callback_service.h"
#include "v4_0/icodec_callback.h"
#include "v4_0/icodec_component_manager.h"
using namespace std;
using namespace testing::ext;
using OHOS::sptr;
using namespace OHOS::HDI::Codec::V4_0;
constexpr int64_t APP_DATA = 3;
constexpr uint32_t INVALID_COMPONENT_ID = -1;
namespace {
class CodecHdiManagerTest : public testing::Test {
public:
    static void SetUpTestCase()
    {}
    static void TearDownTestCase()
    {}
    void SetUp()
    {
        manager_ = ICodecComponentManager::Get();
        callback_ = new CodecCallbackService();
    }
    void TearDown()
    {
        manager_ = nullptr;
        callback_ = nullptr;
    }

public:
    sptr<ICodecComponentManager> manager_;
    sptr<ICodecCallback> callback_;
};

/**
* @tc.number : SUB_Driver_Codec_idlomx_0100
* @tc.name   : HdfCodecHdiGetComponentNumTest_001
* @tc.desc   : Verify the GetComponentNum function when the input parameter is valid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiManagerTest, HdfCodecHdiGetComponentNumTest_001, TestSize.Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    int32_t count = 0;
    auto ret = manager_->GetComponentNum(count);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(count >= 0);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_0200
* @tc.name   : HdfCodecHdiGetCapabilityListTest_001
* @tc.desc   : Verify the GetComponentCapabilityList function when the input parameter is valid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiManagerTest, HdfCodecHdiGetCapabilityListTest_001, TestSize.Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    int32_t count = 0;
    auto ret = manager_->GetComponentNum(count);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(count > 0);

    std::vector<CodecCompCapability> capList;;
    ret = manager_->GetComponentCapabilityList(capList, count);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_0300
* @tc.name   : HdfCodecHdiCreateComponentTest_001
* @tc.desc   : Verify the CreateComponent function when the input parameter is invalid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiManagerTest, HdfCodecHdiCreateComponentTest_001, TestSize.Level1)
{
    ASSERT_TRUE(callback_ != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    sptr<ICodecComponent> component;
    uint32_t componentId = 0;
    int32_t ret = manager_->CreateComponent(component, componentId, "", APP_DATA, callback_);
    ASSERT_NE(ret, HDF_SUCCESS);
    ASSERT_EQ(component, nullptr);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_0400
* @tc.name   : HdfCodecHdiCreateComponentTest_002
* @tc.desc   : Verify the CreateComponent function when the input parameter is invalid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiManagerTest, HdfCodecHdiCreateComponentTest_002, TestSize.Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    sptr<ICodecComponent> component;
    uint32_t componentId = 0;
    std::string compName("");

    int32_t count = 0;
    auto ret = manager_->GetComponentNum(count);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(count > 0);

    std::vector<CodecCompCapability> capList;;
    ret = manager_->GetComponentCapabilityList(capList, count);
    ASSERT_EQ(ret, HDF_SUCCESS);

    compName = capList[0].compName;
    ret = manager_->CreateComponent(component, componentId, compName, APP_DATA, nullptr);
    ASSERT_NE(ret, HDF_SUCCESS);
    ASSERT_EQ(component, nullptr);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_0500
* @tc.name   : HdfCodecHdiCreateComponentTest_003
* @tc.desc   : Verify the CreateComponent function when the input parameter is valid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiManagerTest, HdfCodecHdiCreateComponentTest_003, TestSize.Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("");
    int32_t count = 0;
    auto ret = manager_->GetComponentNum(count);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(count > 0);

    std::vector<CodecCompCapability> capList;;
    ret = manager_->GetComponentCapabilityList(capList, count);
    ASSERT_EQ(ret, HDF_SUCCESS);

    compName = capList[0].compName;
    ASSERT_FALSE(compName.empty());
    sptr<ICodecComponent> component;
    uint32_t componentId = 0;
    ret = manager_->CreateComponent(component, componentId, compName, APP_DATA, callback_);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (componentId != 0) {
        ret = manager_->DestroyComponent(componentId);
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_6000
* @tc.name   : HdfCodecHdiCreateComponentTest_004
* @tc.desc   : Verify the CreateComponent function when the input parameter is valid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiManagerTest, HdfCodecHdiCreateComponentTest_004, TestSize.Level2)
{
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("abc/%@123  ，abc");
    sptr<ICodecComponent> component;
    uint32_t componentId = 0;
    auto ret = manager_->CreateComponent(component, componentId, compName, APP_DATA, callback_);
    ASSERT_NE(ret, HDF_SUCCESS);
    if (componentId != 0) {
        ret = manager_->DestroyComponent(componentId);
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_0600
* @tc.name   : HdfCodecHdiDestroyComponentTest_001
* @tc.desc   : Verify the DestroyComponent function when the input parameter is invalid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiManagerTest, HdfCodecHdiDestroyComponentTest_001, TestSize.Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto ret = manager_->DestroyComponent(INVALID_COMPONENT_ID);
    ASSERT_EQ(ret, HDF_ERR_INVALID_PARAM);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_6100
* @tc.name   : HdfCodecHdiDestroyComponentTest_002
* @tc.desc   : Verify the DestroyComponent function when the input parameter is invalid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiManagerTest, HdfCodecHdiDestroyComponentTest_002, TestSize.Level2)
{
    ASSERT_TRUE(manager_ != nullptr);
    uint32_t componentId = 0;
    auto ret = manager_->DestroyComponent(componentId);
    ASSERT_EQ(ret, HDF_ERR_INVALID_PARAM);
}
}  // namespace
