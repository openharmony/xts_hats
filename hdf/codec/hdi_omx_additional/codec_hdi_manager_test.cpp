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
#include "codec_component_manager.h"
using namespace std;
using namespace testing::ext;
namespace {
class CodecHdiManagerTestAdditional : public testing::Test {
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
* @tc.number  SUB_Driver_Codec_CodecHdi_0600
* @tc.name  testCodecGetComponentNum001
* @tc.desc   Determines whether the function gets the param correctly
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecGetComponentNum001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto count = -1;
    count = manager_->GetComponentNum();
    EXPECT_TRUE(count >= 0);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_0700
* @tc.name  testCodecGetComponentCapabilityList001
* @tc.desc   Determine whether the function can obtain the correct value if there are existing values in the structure
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecGetComponentCapabilityList001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto count = manager_->GetComponentNum();
    CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * count);
    ASSERT_TRUE(capList != nullptr);
    capList->role = MEDIA_ROLETYPE_INVALID;
    auto err = manager_->GetComponentCapabilityList(capList, count);
    EXPECT_EQ(err, HDF_SUCCESS);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_0800
* @tc.name  testCodecGetComponentCapabilityList002
* @tc.desc   Determine whether the function can obtain the correct value if there are existing values in the structure
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecGetComponentCapabilityList002, Function | MediumTest | Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto count = manager_->GetComponentNum();
    ASSERT_TRUE(count > 0);
    CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * count);
    ASSERT_TRUE(capList != nullptr);
    capList->role = MEDIA_ROLETYPE_IMAGE_JPEG;
    auto err = manager_->GetComponentCapabilityList(capList, count);
    EXPECT_EQ(err, HDF_SUCCESS);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_0900
* @tc.name  testCodecGetComponentCapabilityList003
* @tc.desc   Determine whether the function can obtain the correct value if there are existing values in the structure
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecGetComponentCapabilityList003, Function | MediumTest | Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto count = manager_->GetComponentNum();
    CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * count);
    ASSERT_TRUE(capList != nullptr);
    capList->type = INVALID_TYPE;
    auto err = manager_->GetComponentCapabilityList(capList, count);
    EXPECT_EQ(err, HDF_SUCCESS);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1000
* @tc.name  testCodecGetComponentCapabilityList004
* @tc.desc   Determine whether the function can obtain the correct value if there are existing values in the structure
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecGetComponentCapabilityList004, Function | MediumTest | Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto count = manager_->GetComponentNum();
    CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * count);
    ASSERT_TRUE(capList != nullptr);
    capList->maxInst = -1;
    auto err = manager_->GetComponentCapabilityList(capList, count);
    EXPECT_EQ(err, HDF_SUCCESS);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1100
* @tc.name  testCodecGetComponentCapabilityList005
* @tc.desc   Determine whether the function can obtain the correct value if there are existing values in the structure
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecGetComponentCapabilityList005, Function | MediumTest | Level1)
{
    ASSERT_TRUE(manager_ != nullptr);
    auto count = manager_->GetComponentNum();
    CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * count);
    ASSERT_TRUE(capList != nullptr);
    capList->capsMask = -1;
    auto err = manager_->GetComponentCapabilityList(capList, count);
    EXPECT_EQ(err, HDF_SUCCESS);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1200
* @tc.name  testCodecGetComponentCapabilityList006
* @tc.desc   When the number of codec components is negative, the validity of the function is judged
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecGetComponentCapabilityList006, Function | MediumTest | Level2)
{
    ASSERT_TRUE(manager_ != nullptr);
    int count = -1;
    CodecCompCapability *capList = (CodecCompCapability *)
        OsalMemAlloc(sizeof(CodecCompCapability) * (manager_->GetComponentNum()));
    ASSERT_TRUE(capList != nullptr);
    auto err = manager_->GetComponentCapabilityList(capList, count);
    EXPECT_NE(err, HDF_SUCCESS);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1300
* @tc.name  testCodecCreateComponent001
* @tc.desc   When the parameter passed by the function already has a value, determine whether it will have an effect
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecCreateComponent001, Function | MediumTest | Level1)
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
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = -1;
    ASSERT_TRUE(callback != nullptr);

    int32_t ret = manager_->CreateComponent(&component, &componentId, compName.data(), (int64_t)this, callback);
    EXPECT_EQ(ret, HDF_SUCCESS);
    CodecCallbackTypeRelease(callback);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1400
* @tc.name  testCodecCreateComponent002
* @tc.desc   Open the OpenMax component based on a non-existent compName
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecCreateComponent002, Function | MediumTest | Level2)
{
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(callback != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("    ");
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = manager_->CreateComponent(&component, &componentId, compName.data(), (int64_t)this, callback);
    EXPECT_NE(ret, HDF_SUCCESS);
    EXPECT_EQ(component, nullptr);
    CodecCallbackTypeRelease(callback);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1500
* @tc.name  testCodecCreateComponent003
* @tc.desc   Open the OpenMax component based on a non-existent compName
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecCreateComponent003, Function | MediumTest | Level2)
{
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(callback != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("//\a");
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = manager_->CreateComponent(&component, &componentId, compName.data(), (int64_t)this, callback);
    EXPECT_NE(ret, HDF_SUCCESS);
    EXPECT_EQ(component, nullptr);
    CodecCallbackTypeRelease(callback);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1600
* @tc.name  testCodecCreateComponent004
* @tc.desc   Open the OpenMax component based on a non-existent compName
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecCreateComponent004, Function | MediumTest | Level2)
{
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(callback != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("a，,");
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = manager_->CreateComponent(&component, &componentId, compName.data(), (int64_t)this, callback);
    EXPECT_NE(ret, HDF_SUCCESS);
    EXPECT_EQ(component, nullptr);
    CodecCallbackTypeRelease(callback);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1700
* @tc.name  testCodecCreateComponent005
* @tc.desc   Open the OpenMax component based on a non-existent compName
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecCreateComponent005, Function | MediumTest | Level2)
{
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(callback != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("12344");
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = manager_->CreateComponent(&component, &componentId, compName.data(), (int64_t)this, callback);
    EXPECT_NE(ret, HDF_SUCCESS);
    EXPECT_EQ(component, nullptr);
    CodecCallbackTypeRelease(callback);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1800
* @tc.name  testCodecCreateComponent006
* @tc.desc   Open the OpenMax component based on a non-existent compName
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecCreateComponent006, Function | MediumTest | Level1)
{
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(callback != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("a@%");
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = manager_->CreateComponent(&component, &componentId, compName.data(), (int64_t)this, callback);
    EXPECT_NE(ret, HDF_SUCCESS);
    EXPECT_EQ(component, nullptr);
    CodecCallbackTypeRelease(callback);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_1900
* @tc.name  testCodecCreateComponent007
* @tc.desc   Open the OpenMax component based on a non-existent compName
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecCreateComponent007, Function | MediumTest | Level2)
{
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(callback != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("a~~~");
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = manager_->CreateComponent(&component, &componentId, compName.data(), (int64_t)this, callback);
    EXPECT_NE(ret, HDF_SUCCESS);
    EXPECT_EQ(component, nullptr);
    CodecCallbackTypeRelease(callback);
}

/**
* @tc.number  SUB_Driver_Codec_CodecHdi_2000
* @tc.name  testCodecCreateComponent008
* @tc.desc   Open the OpenMax component based on a non-existent compName
*/
HWTEST_F(CodecHdiManagerTestAdditional, testCodecCreateComponent008, Function | MediumTest | Level2)
{
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(callback != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("a～～～");
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = manager_->CreateComponent(&component, &componentId, compName.data(), (int64_t)this, callback);
    EXPECT_NE(ret, HDF_SUCCESS);
    EXPECT_EQ(component, nullptr);
    CodecCallbackTypeRelease(callback);
}
}  // namespace