/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include <OMX_Component.h>
#include <OMX_Core.h>
#include <OMX_Video.h>
#include <OMX_VideoExt.h>
#include <ashmem.h>
#include <gtest/gtest.h>
#include <hdf_log.h>
#include <osal_mem.h>
#include <securec.h>
#include <servmgr_hdi.h>

#include "codec_component_manager.h"
#include "codec_component_type.h"
#include "hdf_io_service_if.h"
#include "codec_omx_ext.h"

#define HDF_LOG_TAG codec_hdi_test

using namespace std;
using namespace testing::ext;

namespace {
struct CodecComponentManager *g_manager = nullptr;
struct CodecComponentType *g_component = nullptr;
struct CodecCallbackType *g_callback = nullptr;
static inline std::string g_compName = "";
uint32_t g_componentId = 0;
union OMX_VERSIONTYPE g_version;

template <typename T>
void InitParam(T &param)
{
    memset_s(&param, sizeof(param), 0x0, sizeof(param));
    param.nSize = sizeof(param);
    param.nVersion = g_version;
}
    
class CodecHdiAdapterTestAdditional : public testing::Test {
public:
    static void SetUpTestCase()
    {
        g_manager = GetCodecComponentManager();
        if (g_manager == nullptr) {
            std::cout<<"GetCodecComponentManager ret nullptr"<<std::endl;
            return;
        }
        auto count = g_manager->GetComponentNum();
        if (count > 0) {
            CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * count);
            ASSERT_TRUE(capList != nullptr);
            auto err = g_manager->GetComponentCapabilityList(capList, count);
            ASSERT_TRUE(err == HDF_SUCCESS);
            g_compName = capList[0].compName;
            OsalMemFree(capList);
            capList = nullptr;
        }
    }
    static void TearDownTestCase()
    {
        CodecComponentManagerRelease();
        g_manager = nullptr;
    }
    void SetUp()
    {
        if (g_manager == nullptr) {
            return;
        }
        g_callback = CodecCallbackTypeGet(nullptr);
        if (g_callback == nullptr) {
            return;
        }
        if (g_compName.empty()) {
            return;
        }

        auto ret = g_manager->CreateComponent(&g_component, &g_componentId, g_compName.data(),
                                             reinterpret_cast<int64_t>(this), g_callback);
        if (ret != HDF_SUCCESS) {
            return;
        }
        struct CompVerInfo verInfo;
        ret = g_component->GetComponentVersion(g_component, &verInfo);
        if (ret != HDF_SUCCESS) {
            return;
        }
        g_version = verInfo.compVersion;
    }
    void TearDown()
    {
        if (g_manager != nullptr && g_component != nullptr) {
            g_manager->DestroyComponent(g_componentId);
        }
        if (g_callback != nullptr) {
            CodecCallbackTypeRelease(g_callback);
            g_callback = nullptr;
        }
    }
};

/**
* @tc.number  SUB_Driver_Codec_Adapter_6400
* @tc.name  testAdapter001
* @tc.desc  The first input parameter is empty. GetComponentVersion is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter001, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    struct CompVerInfo verInfo;
    auto ret = g_component->GetComponentVersion(nullptr, &verInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_6500
* @tc.name  testAdapter002
* @tc.desc  The second input parameter is empty. GetComponentVersion is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    auto ret = g_component->GetComponentVersion(g_component, nullptr);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_6600
* @tc.name  testAdapter003
* @tc.desc  The second and third parameters are OMX_CommandStateSet and OMX_StateInvalid. SendCommand is successful.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter003, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateInvalid, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_6700
* @tc.name  testAdapter004
* @tc.desc  The second and third parameters are OMX_CommandStateSet and OMX_StateLoaded. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateLoaded, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_6800
* @tc.name  testAdapter005
* @tc.desc  The second and third parameters are OMX_CommandStateSet and OMX_StateWaitForResources.
*           SendCommand is successful.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter005, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateWaitForResources, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_6900
* @tc.name  testAdapter006
* @tc.desc  The second and third parameters are OMX_CommandStateSet and OMX_StateKhronosExtensions.
*           SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter006, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateKhronosExtensions, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7000
* @tc.name  testAdapter007
* @tc.desc  The second and third parameters are OMX_CommandStateSet and OMX_StateVendorStartUnused.
*           SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter007, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateVendorStartUnused, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7100
* @tc.name  testAdapter008
* @tc.desc  The second and third parameters are OMX_CommandStateSet and -1. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter008, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, -1, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7200
* @tc.name  testAdapter009
* @tc.desc  The second and third parameters are OMX_CommandStateSet and OMX_StateMax. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter009, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateMax, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7300
* @tc.name  testAdapter010
* @tc.desc  The second and third parameters are OMX_CommandPortEnable and OMX_StateInvalid. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter010, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortEnable, OMX_StateInvalid, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7400
* @tc.name  testAdapter011
* @tc.desc  The second and third parameters are OMX_CommandPortEnable and OMX_StateLoaded. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter011, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortEnable, OMX_StateLoaded, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7500
* @tc.name  testAdapter012
* @tc.desc  The second and third parameters are OMX_CommandPortEnable and OMX_StateIdle. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter012, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortEnable, OMX_StateIdle, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7600
* @tc.name  testAdapter013
* @tc.desc  The second and third parameters are OMX_CommandFlush and OMX_StateExecuting. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter013, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandFlush, OMX_StateExecuting, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7700
* @tc.name  testAdapter014
* @tc.desc  The second and third parameters are OMX_CommandFlush and OMX_StateWaitForResources. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter014, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandFlush, OMX_StateWaitForResources, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7800
* @tc.name  testAdapter015
* @tc.desc  The second and third parameters are OMX_CommandFlush and OMX_StateKhronosExtensions. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter015, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandFlush, OMX_StateKhronosExtensions, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_7900
* @tc.name  testAdapter016
* @tc.desc  The second and third parameters are OMX_CommandFlush and OMX_StateVendorStartUnused. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter016, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandFlush, OMX_StateVendorStartUnused, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8000
* @tc.name  testAdapter017
* @tc.desc  The second and third parameters are OMX_CommandFlush and -1. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter017, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandFlush, -1, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8100
* @tc.name  testAdapter018
* @tc.desc  The second and third parameters are OMX_CommandFlush and OMX_StateMax. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter018, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandFlush, OMX_StateMax, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8200
* @tc.name  testAdapter019
* @tc.desc  The second and third parameters are OMX_CommandPortDisable and OMX_StateInvalid. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter019, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortDisable, OMX_StateInvalid, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8300
* @tc.name  testAdapter020
* @tc.desc  The second and third parameters are OMX_CommandPortDisable and OMX_StateLoaded. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter020, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortDisable, OMX_StateLoaded, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8400
* @tc.name  testAdapter021
* @tc.desc  The second and third parameters are OMX_CommandPortDisable and OMX_StateIdle. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter021, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortDisable, OMX_StateIdle, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8500
* @tc.name  testAdapter022
* @tc.desc  The second and third parameters are OMX_CommandPortDisable and OMX_StateExecuting. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter022, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortDisable, OMX_StateExecuting, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8600
* @tc.name  testAdapter023
* @tc.desc  The second and third parameters are OMX_CommandPortDisable and OMX_StateWaitForResources.
*           SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter023, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortDisable, OMX_StateWaitForResources, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8700
* @tc.name  testAdapter024
* @tc.desc  The second and third parameters are OMX_CommandPortDisable and OMX_StateKhronosExtensions.
*           SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter024, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortDisable, OMX_StateKhronosExtensions, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8800
* @tc.name  testAdapter025
* @tc.desc  The second and third parameters are OMX_CommandPortDisable and OMX_StateVendorStartUnused.
*           SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter025, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortDisable, OMX_StateVendorStartUnused, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_8900
* @tc.name  testAdapter026
* @tc.desc  The second and third parameters are OMX_CommandPortDisable and -1. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter026, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortDisable, -1, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9000
* @tc.name  testAdapter027
* @tc.desc  The second and third parameters are OMX_CommandPortDisable and OMX_StateMax. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter027, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandPortDisable, OMX_StateMax, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9100
* @tc.name  testAdapter028
* @tc.desc  The second and third parameters are OMX_CommandMax and OMX_StateInvalid. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter028, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, OMX_StateInvalid, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9200
* @tc.name  testAdapter029
* @tc.desc  The second and third parameters are OMX_CommandMax and OMX_StateLoaded. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter029, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, OMX_StateLoaded, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9300
* @tc.name  testAdapter030
* @tc.desc  The second and third parameters are OMX_CommandMax and OMX_StateIdle. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter030, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, OMX_StateIdle, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9400
* @tc.name  testAdapter031
* @tc.desc  The second and third parameters are OMX_CommandMax and OMX_StateExecuting. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter031, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, OMX_StateExecuting, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9500
* @tc.name  testAdapter032
* @tc.desc  The second and third parameters are OMX_CommandMax and OMX_StateWaitForResources. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter032, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, OMX_StateWaitForResources, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9600
* @tc.name  testAdapter033
* @tc.desc  The second and third parameters are OMX_CommandMax and OMX_StateKhronosExtensions. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter033, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, OMX_StateKhronosExtensions, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9700
* @tc.name  testAdapter034
* @tc.desc  The second and third parameters are OMX_CommandMax and OMX_StateVendorStartUnused. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter034, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, OMX_StateVendorStartUnused, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9800
* @tc.name  testAdapter035
* @tc.desc  The second and third parameters are OMX_CommandMax and -1. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter035, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, -1, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number  SUB_Driver_Codec_Adapter_9900
* @tc.name  testAdapter036
* @tc.desc  The second and third parameters are OMX_CommandMax and OMX_StateMax. SendCommand is error.
*/
HWTEST_F(CodecHdiAdapterTestAdditional, testAdapter036, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, OMX_StateMax, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}

}  // namespace