/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include <gtest/gtest.h>
#include <osal_mem.h>
#include "codec_callback_if.h"
#include "codec_callback_type_stub.h"
#include "codec_component_manager.h"
using namespace std;
using namespace testing::ext;
namespace {
class CodecHdiManagerTest : public testing::Test {
public:
    static void SetUpTestCase()
    {}
    static void TearDownTestCase()
    {}
    void SetUp()
    {
        manager_ = GetCodecComponentManager();
    }
    void TearDown()
    {
        CodecComponentManagerRelease();
        manager_ = nullptr;
    }

public:
    struct CodecComponentManager *manager_ = nullptr;
};
/**
* @tc.name  HdfCodecHdiGetComponentNumTest
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0020
* @tc.desc   Obtain the total number of codec HDI2.0 capability sets
*/
HWTEST_F(CodecHdiManagerTest, SUB_DriverSystem_CodecHdi_V2_0020, Function | MediumTest | Level3)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto count = manager_->GetComponentNum();
    EXPECT_TRUE(count >= 0);
}
/**
* @tc.name  HdfCodecHdiGetCapabilityListTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0030
* @tc.desc   Obtains the capability data of the codec capability and prints the data to the Hilog
*/
HWTEST_F(CodecHdiManagerTest, SUB_DriverSystem_CodecHdi_V2_0030, Function | MediumTest | Level3)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto count = manager_->GetComponentNum();
    ASSERT_TRUE(count > 0);
    CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * count);
    ASSERT_TRUE(capList != nullptr);
    auto err = manager_->GetComponentCapabilityList(capList, count);
    EXPECT_EQ(err, HDF_SUCCESS);
    OsalMemFree(capList);
    capList = nullptr;
}
/**
* @tc.name  HdfCodecHdiCreateComponentTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0040
* @tc.desc   Open the OpenMax component based on a name that does not exist
*/
HWTEST_F(CodecHdiManagerTest, SUB_DriverSystem_CodecHdi_V2_0040, Function | MediumTest | Level3)
{
    struct CodecCallbackType *callback = CodecCallbackTypeStubGetInstance();
    ASSERT_TRUE(callback != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = manager_->CreateComponent(&component, &componentId, nullptr, (int64_t)this, callback);
    EXPECT_NE(ret, HDF_SUCCESS);
    EXPECT_EQ(component, nullptr);
    CodecCallbackTypeStubRelease(callback);
    callback = nullptr;
}
/**
* @tc.name  HdfCodecHdiCreateComponentTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0050
* @tc.desc   When a component is opened, the name of the component is transferred to a null pointer
*/
HWTEST_F(CodecHdiManagerTest, SUB_DriverSystem_CodecHdi_V2_0050, Function | MediumTest | Level3)
{
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("");
    auto count = manager_->GetComponentNum();
    ASSERT_TRUE(count > 0);
    CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * count);
    ASSERT_TRUE(capList != nullptr);
    auto err = manager_->GetComponentCapabilityList(capList, count);
    ASSERT_TRUE(err == HDF_SUCCESS);
    compName = capList[0].compName;
    OsalMemFree(capList);
    capList = nullptr;

    ASSERT_FALSE(compName.empty());
    struct CodecCallbackType *callback = CodecCallbackTypeStubGetInstance();
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    ASSERT_TRUE(callback != nullptr);
    auto ret = manager_->CreateComponent(&component, &componentId, compName.data(), (int64_t)this, callback);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ret = manager_->DestroyComponent(componentId);
    CodecCallbackTypeStubRelease(callback);
    callback = nullptr;
}
/**
* @tc.name  HdfCodecHdiDestoryComponentTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0740
* @tc.desc Releasing Components
*/
HWTEST_F(CodecHdiManagerTest, SUB_DriverSystem_CodecHdi_V2_0740, Function | MediumTest | Level3)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto ret = manager_->DestroyComponent(0);
    EXPECT_EQ(ret, HDF_SUCCESS);
}
}  // namespace