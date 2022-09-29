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
uint32_t g_componentId = 0;
struct CodecCallbackType *g_callback = nullptr;
int32_t g_count = 0;

constexpr const char *ENCODER_AVC = "rk.video_encoder.avc";
constexpr const char *DECODER_AVC = "rk.video_decoder.avc";

constexpr int32_t INT_TO_STR_LEN = 32;
constexpr int32_t ARRAY_TO_STR_LEN = 1000;

union OMX_VERSIONTYPE g_version;
constexpr int32_t BUFFER_SIZE = (640 * 480 * 3);
constexpr int32_t ROLE_LEN = 240;
constexpr int32_t FRAMERATE = (30 << 16);
constexpr uint32_t BUFFER_ID_ERROR = 65000;
constexpr int64_t APP_DATA = 64;
constexpr int64_t APP_DATA_MAX = 9223372036854775807;
constexpr int64_t APP_DATA_MIN = -9223372036854775807;
enum class PortIndex { PORT_INDEX_INPUT = 0, PORT_INDEX_OUTPUT = 1, PORT_INDEX_ERROR_INPUT = 2 };

template <typename T>
void InitParam(T &param)
{
    memset_s(&param, sizeof(param), 0x0, sizeof(param));
    param.nSize = sizeof(param);
    param.nVersion = g_version;
}

struct BufferInfo {
    std::shared_ptr<OmxCodecBuffer> omxBuffer;
    std::shared_ptr<OHOS::Ashmem> sharedMem;
    BufferInfo()
    {
        omxBuffer = nullptr;
        sharedMem = nullptr;
    }
    ~BufferInfo()
    {
        omxBuffer = nullptr;
        if (sharedMem != nullptr) {
            sharedMem->UnmapAshmem();
            sharedMem->CloseAshmem();
            sharedMem = nullptr;
        }
    }
};
std::map<int32_t, std::shared_ptr<BufferInfo>> inputBuffers;
std::map<int32_t, std::shared_ptr<BufferInfo>> outputBuffers;
static void InitCodecBufferWithAshMem(enum PortIndex portIndex, int bufferSize, shared_ptr<OmxCodecBuffer> omxBuffer,
                                      shared_ptr<OHOS::Ashmem> sharedMem)
{
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = g_version;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    omxBuffer->bufferLen = sizeof(int);
    omxBuffer->buffer = (uint8_t *)(uintptr_t)sharedMem->GetAshmemFd();
    omxBuffer->allocLen = bufferSize;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;
    if (portIndex == PortIndex::PORT_INDEX_INPUT) {
        omxBuffer->type = READ_ONLY_TYPE;
        sharedMem->MapReadAndWriteAshmem();
    } else {
        omxBuffer->type = READ_WRITE_TYPE;
        sharedMem->MapReadOnlyAshmem();
    }
}

static bool UseBufferOnPort(enum PortIndex portIndex, int bufferCount, int bufferSize)
{
    for (int i = 0; i < bufferCount; i++) {
        std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
        if (omxBuffer == nullptr) {
            HDF_LOGE("%{public}s omxBuffer is null", __func__);
            return false;
        }

        int fd = OHOS::AshmemCreate(0, bufferSize);
        shared_ptr<OHOS::Ashmem> sharedMem = make_shared<OHOS::Ashmem>(fd, bufferSize);
        if (sharedMem == nullptr) {
            HDF_LOGE("%{public}s sharedMem is null", __func__);
            if (fd >= 0) {
                close(fd);
                fd = -1;
            }
            return false;
        }
        InitCodecBufferWithAshMem(portIndex, bufferSize, omxBuffer, sharedMem);
        auto err = g_component->UseBuffer(g_component, (uint32_t)portIndex, omxBuffer.get());
        if (err != HDF_SUCCESS) {
            HDF_LOGE("%{public}s failed to UseBuffer with  portIndex[%{public}d]", __func__, portIndex);
            sharedMem->UnmapAshmem();
            sharedMem->CloseAshmem();
            sharedMem = nullptr;
            omxBuffer = nullptr;
            return false;
        }
        omxBuffer->bufferLen = 0;
        HDF_LOGI("UseBuffer returned bufferID [%{public}d]", omxBuffer->bufferId);
        std::shared_ptr<BufferInfo> bufferInfo = std::make_shared<BufferInfo>();
        bufferInfo->omxBuffer = omxBuffer;
        bufferInfo->sharedMem = sharedMem;
        if (portIndex == PortIndex::PORT_INDEX_INPUT) {
            inputBuffers.emplace(std::make_pair(omxBuffer->bufferId, bufferInfo));
        } else {
            outputBuffers.emplace(std::make_pair(omxBuffer->bufferId, bufferInfo));
        }
    }
    return true;
}

class CodecHdiAdapterTest : public testing::Test {
public:
    static void SetUpTestCase()
    {}
    static void TearDownTestCase()
    {}
    void SetUp()
    {}
    void TearDown()
    {}
};

static char g_arrayStr[ARRAY_TO_STR_LEN];
static char *GetArrayStr(int32_t *array, int32_t arrayLen, int32_t endValue)
{
    int32_t len = 0;
    int32_t totalLen = 0;
    int32_t ret;
    char value[INT_TO_STR_LEN];
    ret = memset_s(g_arrayStr, sizeof(g_arrayStr), 0, sizeof(g_arrayStr));
    if (ret != EOK) {
        HDF_LOGE("%{public}s: memset_s g_arrayStr failed, error code: %{public}d", __func__, ret);
        return g_arrayStr;
    }
    for (int32_t i = 0; i < arrayLen; i++) {
        if (array[i] == endValue) {
            break;
        }
        ret = memset_s(value, sizeof(value), 0, sizeof(value));
        if (ret != EOK) {
            HDF_LOGE("%{public}s: memset_s value failed, error code: %{public}d", __func__, ret);
            return g_arrayStr;
        }
        ret = sprintf_s(value, sizeof(value), "0x0%X, ", array[i]);
        if (ret < 0) {
            HDF_LOGE("%{public}s: sprintf_s value failed, error code: %{public}d", __func__, ret);
            return g_arrayStr;
        }
        len = strnlen(value, ARRAY_TO_STR_LEN);
        ret = memcpy_s(g_arrayStr + totalLen, len, value, len);
        if (ret != EOK) {
            HDF_LOGE("%{public}s: memcpy_s g_arrayStr failed, error code: %{public}d", __func__, ret);
            return g_arrayStr;
        }
        totalLen += len;
    }
    return g_arrayStr;
}

static void PrintCapability(CodecCompCapability *cap, int32_t index)
{
    HDF_LOGI("---------------------- component capability %{public}d ---------------------", index + 1);
    HDF_LOGI("role:%{public}d", cap->role);
    HDF_LOGI("type:%{public}d", cap->type);
    HDF_LOGI("compName:%{public}s", cap->compName);
    HDF_LOGI("supportProfiles:%{public}s", GetArrayStr(cap->supportProfiles, PROFILE_NUM, INVALID_PROFILE));
    HDF_LOGI("maxInst:%{public}d", cap->maxInst);
    HDF_LOGI("isSoftwareCodec:%{public}d", cap->isSoftwareCodec);
    HDF_LOGI("processModeMask:0x0%{public}x", cap->processModeMask);
    HDF_LOGI("capsMask:0x0%{public}x", cap->capsMask);
    HDF_LOGI("bitRate.min:%{public}d", cap->bitRate.min);
    HDF_LOGI("bitRate.max:%{public}d", cap->bitRate.max);
    if (strstr(cap->compName, "video") != NULL) {
        HDF_LOGI("minSize.width:%{public}d", cap->port.video.minSize.width);
        HDF_LOGI("minSize.height:%{public}d", cap->port.video.minSize.height);
        HDF_LOGI("maxSize.width:%{public}d", cap->port.video.maxSize.width);
        HDF_LOGI("maxSize.height:%{public}d", cap->port.video.maxSize.height);
        HDF_LOGI("widthAlignment:%{public}d", cap->port.video.whAlignment.widthAlignment);
        HDF_LOGI("heightAlignment:%{public}d", cap->port.video.whAlignment.heightAlignment);
        HDF_LOGI("blockCount.min:%{public}d", cap->port.video.blockCount.min);
        HDF_LOGI("blockCount.max:%{public}d", cap->port.video.blockCount.max);
        HDF_LOGI("blocksPerSecond.min:%{public}d", cap->port.video.blocksPerSecond.min);
        HDF_LOGI("blocksPerSecond.max:%{public}d", cap->port.video.blocksPerSecond.max);
        HDF_LOGI("blockSize.width:%{public}d", cap->port.video.blockSize.width);
        HDF_LOGI("blockSize.height:%{public}d", cap->port.video.blockSize.height);
        HDF_LOGI("supportPixFmts:%{public}s", GetArrayStr(cap->port.video.supportPixFmts, PIX_FORMAT_NUM, 0));
    } else {
        HDF_LOGI(":%{public}s", GetArrayStr(cap->port.audio.sampleFormats, SAMPLE_FMT_NUM, AUDIO_SAMPLE_FMT_INVALID));
        HDF_LOGI(":%{public}s", GetArrayStr(cap->port.audio.sampleRate, SAMPLE_RATE_NUM, AUD_SAMPLE_RATE_INVALID));
        HDF_LOGI(":%{public}s", GetArrayStr(cap->port.audio.channelLayouts, CHANNEL_NUM, -1));
        HDF_LOGI(":%{public}s", GetArrayStr(cap->port.audio.channelCount, CHANNEL_NUM, -1));
    }
    HDF_LOGI("-------------------------------------------------------------------");
}
/**
* @tc.name  HdfCodecHdiGetCodecComponentManagerTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0010
* @tc.desc Invoke the GetCodecComponentManager interface to obtain component management objects
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0010, Function | MediumTest | Level3)
{
    g_manager = GetCodecComponentManager();
    ASSERT_TRUE(g_manager != nullptr);
    g_callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(g_callback != nullptr);
}
/**
* @tc.name  HdfCodecHdiGetComponentNumTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0020
* @tc.desc Invoke the GetComponentNumber interface to obtain the number of components
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0020, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    g_count = g_manager->GetComponentNum();
    ASSERT_GT(g_count, 0);
}
/**
* @tc.name  HdfCodecHdiGetComponentCapabilityListTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0030
* @tc.desc Invoke the GetComponentCapabilityList interface to obtain the component capability set
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0030, Function | MediumTest | Level3)
{
    ASSERT_GT(g_count, 0);
    ASSERT_TRUE(g_manager != nullptr);
    CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * g_count);
    ASSERT_TRUE(capList != nullptr);
    g_manager->GetComponentCapabilityList(capList, g_count);
    for (int32_t i = 0; i < g_count; i++) {
        PrintCapability(&capList[i], i);
    }
    OsalMemFree(capList);
    capList = nullptr;
}
/**
* @tc.name  HdfCodecHdiCreateComponentCompNameErrorTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0040
* @tc.desc If compName is a component name that does not exist, the CreateComponent
           interface is invoked to create a component
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0040, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    char name[] = "test";
    int32_t ret = g_manager->CreateComponent(&g_component, &g_componentId, (char *)name, APP_DATA, g_callback);
    ASSERT_NE(ret, HDF_SUCCESS);
    ASSERT_TRUE(g_component == nullptr);
}
/**
* @tc.name  HdfCodecHdiCreateComponentCompNameNullptrTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0050
* @tc.desc When compName is set to Nullptr, the CreateComponent interface is invoked to create a component
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0050, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    int32_t ret = g_manager->CreateComponent(&g_component, &g_componentId, nullptr, APP_DATA, g_callback);
    ASSERT_NE(ret, HDF_SUCCESS);
    ASSERT_TRUE(g_component == nullptr);
}
/**
* @tc.name  HdfCodecHdiCreateComponentAppdataMaxTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0060
* @tc.desc When Appdata is the maximum value in the value range, the CreateComponent
           interface is invoked to create a component
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0060, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(callback != nullptr);
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = g_manager->CreateComponent(&component, &componentId, const_cast<char *>(DECODER_AVC),
                                             APP_DATA_MAX, callback);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(component != nullptr);
    ret = g_manager->DestroyComponent(componentId);
    ASSERT_EQ(ret, HDF_SUCCESS);
    CodecCallbackTypeRelease(callback);
    callback = nullptr;
}
/**
* @tc.name HdfCodecHdiCreateComponentAppdataMinTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0070
* @tc.desc When Appdata is the minimum value in the value range, the CreateComponent
           interface is invoked to create a componentt
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0070, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    struct CodecCallbackType *callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(callback != nullptr);
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = g_manager->CreateComponent(&component, &componentId, const_cast<char *>(DECODER_AVC),
                                             APP_DATA_MIN, callback);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(component != nullptr);
    ret = g_manager->DestroyComponent(componentId);
    ASSERT_EQ(ret, HDF_SUCCESS);
    CodecCallbackTypeRelease(callback);
    callback = nullptr;
}
/**
* @tc.name HdfCodecHdiCreateComponentCallbackIsNullptrTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0080
* @tc.desc When callback is nullptr, the CreateComponent interface is invoked to create a component
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0080, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    struct CodecCallbackType *callback = nullptr;
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret = g_manager->CreateComponent(&component, &componentId, const_cast<char *>(DECODER_AVC),
                                             APP_DATA, callback);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiCreateComponentSuccessTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0090
* @tc.desc Invoke the CreateComponent interface to create a video encoding component
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0090, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    int32_t ret = g_manager->CreateComponent(&g_component, &g_componentId, const_cast<char *>(ENCODER_AVC), APP_DATA,
                                             g_callback);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(g_component != nullptr);
}
// Test GetComponentVersion Adapter not support
/**
* @tc.name HdfCodecHdiGetComponentVersionUnsupportedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0100
* @tc.desc Invoke the GetComponentVersion interface to obtain the component version
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    struct CompVerInfo verInfo;
    int32_t ret = g_component->GetComponentVersion(g_component, &verInfo);
    g_version = verInfo.compVersion;
    ASSERT_NE(ret, HDF_SUCCESS);
}
// Test GetParameter
/**
* @tc.name HdfCodecHdiGetParameterParamStructNullptrTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0110
* @tc.desc When paramStrut is set to nullptr, the GetParameter interface is invoked to obtain component parameters
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0110, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto ret = g_component->GetParameter(g_component, OMX_IndexParamVideoPortFormat, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiGetParameterSuccessTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0120
* @tc.desc The GetParameter interface is invoked to obtain component parameters
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0120, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    param.eCompressionFormat = OMX_VIDEO_CodingAVC;
    auto ret = g_component->GetParameter(g_component, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiGetParameterSuccessTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0130
* @tc.desc Invoke the GetParameter interface to obtain component parameters when paramIndex does not match
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0130, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = g_component->GetParameter(g_component, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiGetParameterParamIndexUnusedTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0140
* @tc.desc When paramIndex is set to, invoke the GetParameter interface to obtain component parameters
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0140, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = g_component->GetParameter(g_component, OMX_IndexVideoStartUnused, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetParameterSuccessTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0150
* @tc.desc The SetParameter interface is invoked to set component parameters
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0150, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    int32_t ret = g_component->SetParameter(g_component, OMX_IndexParamVideoPortFormat,
                                            reinterpret_cast<int8_t *>(&param), sizeof(param));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetParameterParamStructNullptrTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0160
* @tc.desc When paramStrut is set to nullptr, the SetParameter interface is invoked to set component parameters
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0160, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SetParameter(g_component, OMX_IndexParamVideoPortFormat, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetParameterParamIndexNotMatchParamStructTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0170
* @tc.desc When paramIndex does not match paramStruct, the SetParameter
           interface is invoked to set component parameters
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0170, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    int32_t ret = g_component->SetParameter(g_component, OMX_IndexParamVideoPortFormat,
                                            reinterpret_cast<int8_t *>(&param), sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetParameterParamIndexUnusedTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0180
* @tc.desc When paramIndex is set to OMX_IndexVideoStartUnused, the SetParameter
           interface is invoked to set component parameters
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0180, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    int32_t ret = g_component->SetParameter(g_component, OMX_IndexVideoStartUnused, reinterpret_cast<int8_t *>(&param),
                                            sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetParameterCodecComponentTypeNullptrTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0190
* @tc.desc When CodecComponentType is set to nullptr, the SetParameter interface is invoked to set component parameters
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0190, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    int32_t ret = g_component->SetParameter(nullptr, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                            sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetParameterParamStructLenNotMatchTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0200
* @tc.desc Invoke the SetParameter interface to set component parameters when paramStructLen does not match
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    int32_t ret = g_component->SetParameter(g_component, OMX_IndexParamVideoPortFormat,
                                 reinterpret_cast<int8_t *>(&param), 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiGetConfigUnsupportedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0210
* @tc.desc Invoke the GetConfig interface to obtain component configurations
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0210, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    int32_t ret = g_component->GetConfig(g_component, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetConfigUnsupportedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0220
* @tc.desc Invoke the SetConfig interface to set component configurations
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0220, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    param.nEncodeBitrate = FRAMERATE;
    int32_t ret = g_component->SetConfig(g_component, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiGetExtensionIndexUnsupportedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0230
* @tc.desc Invoke the GetExtensionIndex interface to obtain the component extended index
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0230, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_INDEXTYPE indexType;
    int32_t ret =
        g_component->GetExtensionIndex(g_component, "OMX.Topaz.index.param.extended_video", (uint32_t *)&indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiGetStateSuccessTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0240
* @tc.desc When state is set to OMX_StateLoaded, the GetState interface is invoked to obtain the component status
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0240, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_STATETYPE state;
    int32_t ret = g_component->GetState(g_component, &state);
    ASSERT_EQ(state, OMX_StateLoaded);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiGetStateStateNullptrTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0250
* @tc.desc When state is set to nullptr, the GetState interface is invoked to obtain the component status
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0250, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->GetState(g_component, nullptr);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiTunnelRequestUnsupportedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0260
* @tc.desc Invoke the ComponentTunnelRequest interface to set component pipe communication
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0260, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OMX_TUNNELSETUPTYPE tunnelSetup;
    tunnelSetup.eSupplier = OMX_BufferSupplyInput;

    int32_t ret = g_component->ComponentTunnelRequest(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, tunneledComp,
                                                      tunneledPort, &tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiAllocateBufferInvalidInputBufferTypeTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0270
* @tc.desc When bufferType is set to InvalidInput and portIndex is set to PORT_INDEX_INPUT, the AllocateBuffer
           interface is invoked to set the external buffer
*/
struct OmxCodecBuffer allocBuffer;
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0270, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_INVALID;
    allocBuffer.fenceFd = -1;
    allocBuffer.version = g_version;
    allocBuffer.allocLen = BUFFER_SIZE;
    allocBuffer.buffer = 0;
    allocBuffer.bufferLen = 0;
    allocBuffer.pts = 0;
    allocBuffer.flag = 0;
    allocBuffer.type = READ_ONLY_TYPE;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiAllocateBufferInputBufferNotInitTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0280
* @tc.desc When the buffer is not initialized and portIndex is set to PORT_INDEX_INPUT, the AllocateBuffer
           interface is invoked to set the external buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0280, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_VIRTUAL_ADDR;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiAllocateBufferInvalidOutputBufferTypeTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0290
* @tc.desc When bufferType is set to InvalidInput and portIndex is set to PORT_INDEX_OUTPUT, the AllocateBuffer
           interface is invoked to set the external buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0290, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_INVALID;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiAllocateBufferOutputBufferNotInitTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0300
* @tc.desc When the buffer is not initialized and portIndex is set to PORT_INDEX_OUTPUT, the AllocateBuffer
           interface is invoked to set the external buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0300, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_VIRTUAL_ADDR;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiAllocateBufferAvshareMemFdInputNotInitTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0310
* @tc.desc When the buffer is not initialized and portIndex is set to PORT_INDEX_OUTPUT, the AllocateBuffer
           interface is invoked to set the external buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0310, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiAllocateBufferAvshareMemFdOutputNotInitTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0320
* @tc.desc When the buffer is not initialized and portIndex is set to PORT_INDEX_OUTPUT, the AllocateBuffer
           interface is invoked to set the external buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0320, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiUseBufferInvalidInputBufferTypeTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0330
* @tc.desc When bufferType is set to InvalidInput and portIndex is set to PORT_INDEX_INPUT,
           the UseBuffer interface is invoked to set the internal buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0330, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = g_version;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_INVALID;
    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    auto err = g_component->UseBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, omxBuffer.get());
    ASSERT_NE(err, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiUseBufferInvalidOutputBufferTypeTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0340
* @tc.desc When bufferType is set to InvalidInput and portIndex is set to PORT_INDEX_OUTPUT,
           the UseBuffer interface is called to set the internal buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0340, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = g_version;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_INVALID;
    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    auto err = g_component->UseBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, omxBuffer.get());
    ASSERT_NE(err, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiUseBufferTestVirtualAddrInputTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0350
* @tc.desc When the buffer is not initialized and portIndex is set to PORT_INDEX_INPUT,
           the UseBuffer interface is invoked to set the internal buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0350, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = g_version;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_VIRTUAL_ADDR;
    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    auto err = g_component->UseBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, omxBuffer.get());
    ASSERT_NE(err, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiUseBufferTestVirtualAddrOutput_004
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0360
* @tc.desc When the buffer is not initialized and portIndex is set to PORT_INDEX_OUTPUT,
           call the UseBuffer interface to set the internal buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0360, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = g_version;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_VIRTUAL_ADDR;
    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    auto err = g_component->UseBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, omxBuffer.get());
    ASSERT_NE(err, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiUseBufferInputSuccessTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0370
* @tc.desc After the buffer is initialized and portIndex is set to PORT_INDEX_INPUT
           the UseBuffer interface is invoked to set the internal buffe
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0370, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_INPUT;
    auto err = g_component->GetParameter(g_component, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int bufferSize = param.nBufferSize;
    int bufferCount = param.nBufferCountActual;
    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_INPUT, bufferCount, bufferSize);
    ASSERT_TRUE(ret);
}
/**
* @tc.name HdfCodecHdiUseBufferOutputSuccessTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0380
* @tc.desc After the buffer is initialized and portIndex is set to PORT_INDEX_OUTPUT,
           call the UseBuffer interface to set the internal buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0380, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_OUTPUT;
    auto err = g_component->GetParameter(g_component, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int bufferSize = param.nBufferSize;
    int bufferCount = param.nBufferCountActual;
    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_OUTPUT, bufferCount, bufferSize);
    ASSERT_TRUE(ret);
}
/**
* @tc.name HdfCodecHdiUseBufferDynamicHandleOutputTest_007
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0390
* @tc.desc When bufferType is set to DynamicHandle and portIndex is set to PORT_INDEX_INPUT,
           the UseBuffer interface is invoked to set the internal buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0390, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = g_version;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_DYNAMIC_HANDLE;

    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    auto err = g_component->UseBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, omxBuffer.get());
    if (err != HDF_SUCCESS) {
        HDF_LOGE("%{public}s failed to UseBuffer with  input port", __func__);
        omxBuffer = nullptr;
    }
    ASSERT_NE(err, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiUseBufferAbnormalPortIndexTest_008
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0400
* @tc.desc After the buffer is initialized and portIndex is set to PORT_INDEX_ERROR_INPUT,
           call the UseBuffer interface to set the internal buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0400, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_INPUT;
    auto err = g_component->GetParameter(g_component, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int bufferSize = param.nBufferSize;
    int bufferCount = param.nBufferCountActual;
    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_ERROR_INPUT, bufferCount, bufferSize);
    ASSERT_FALSE(ret);
}
/**
* @tc.name HdfCodecHdiSetStateLoadedToIdleTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0410
* @tc.desc The status of the component that invokes the SendCommand
           interface changes from OMX_StateLoaded to OMX_StateIdle
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0410, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateIdle, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetStateIdleToExecutingTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0420
* @tc.desc The status of the component that invokes the SendCommand
           interface changes from OMX_StateIdle to OMX_StateExecuting
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0420, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateExecuting, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetStateExecutingToPauseTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0430
* @tc.desc The status of the component that invokes the SendCommand
           interface changes from OMX_StateExecuting to OMX_StatePause
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0430, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StatePause, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetStatePauseToIdleTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0440
* @tc.desc The status of the component that invokes the SendCommand
           interface changes from OMX_StatePause to OMX_StateIdle
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0440, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateIdle, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);

    OMX_STATETYPE state;
    ret = g_component->GetState(g_component, &state);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSendCommandCodecComponentTypeNullptrTest_005
* @tc.number SUB_DriverSystem_CodecHdi_adapter_0450
* @tc.desc When CodecComponentType is set to Nullptr, the SendCommand
           interface is invoked to send instructions
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0450, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(nullptr, OMX_CommandStateSet, OMX_StateIdle, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSendCommandAbnormalCommandTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0460
* @tc.desc When OMX_COMMANDTYPE is set to OMX_StateMax, the SendCommand
           interface is invoked to send instructions
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0460, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandMax, OMX_StatePause, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSendCommandAbnormalParamTest_007
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0470
* @tc.desc When the cmd command is set to OMX_StateMax, the SendCommand
           interface is invoked to send the command
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0470, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateMax, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiUseEglImageUnsupportedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0480
* @tc.desc Invoke the UseEglImage interface to use the EGL image
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0480, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer buffer;
    buffer.fenceFd = -1;
    buffer.version = g_version;
    buffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    buffer.allocLen = BUFFER_SIZE;
    buffer.buffer = 0;
    buffer.bufferLen = 0;
    buffer.pts = 0;
    buffer.flag = 0;
    buffer.type = READ_ONLY_TYPE;
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    int32_t ret = g_component->UseEglImage(g_component, &buffer, (uint32_t)PortIndex::PORT_INDEX_INPUT, eglImage.get(),
                                           BUFFER_SIZE);
    ASSERT_NE(ret, HDF_SUCCESS);
    eglImage = nullptr;
}
/**
* @tc.name HdfCodecHdiWaitStateIdleTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0490
* @tc.desc The status of the listening component changes to OMX_StateIdle
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0490, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    // wait for Idle status
    OMX_STATETYPE state = OMX_StateInvalid;
    do {
        usleep(100);
        auto ret = g_component->GetState(g_component, &state);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } while (state != OMX_StateIdle);
}
/**
* @tc.name HdfCodecHdiFillThisBufferSuccessTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0500
* @tc.desc Invoke the FillThisBuffer interface to execute the codec output to fill the buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0500, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = outputBuffers.begin();
    if (iter != outputBuffers.end()) {
        int32_t ret = g_component->FillThisBuffer(g_component, iter->second->omxBuffer.get());
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
}
/**
* @tc.name HdfCodecHdiFillThisBufferUseErrorBufferIdTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0510
* @tc.desc Invoke the FillThisBuffer interface when bufferID is set to BUFFER_ID_ERROR
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0510, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = outputBuffers.begin();
    if (iter != outputBuffers.end()) {
        auto omxBuffer = iter->second->omxBuffer;
        auto tempId = omxBuffer->bufferId;
        omxBuffer->bufferId = BUFFER_ID_ERROR;
        int32_t ret = g_component->FillThisBuffer(g_component, omxBuffer.get());
        ASSERT_NE(ret, HDF_SUCCESS);
        omxBuffer->bufferId = tempId;
    }
}
/**
* @tc.name HdfCodecHdiEmptyThisBufferSuccessTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0520
* @tc.desc The EmptyThisBuffer interface is invoked to execute the codec output to fill the buffer
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0520, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = inputBuffers.begin();
    if (iter != inputBuffers.end()) {
        int32_t ret = g_component->EmptyThisBuffer(g_component, iter->second->omxBuffer.get());
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
}
/**
* @tc.name HdfCodecHdiEmptyThisBufferUseErrorBufferIdTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0530
* @tc.desc Invoke the EmptyThisBuffer interface when bufferID is set to BUFFER_ID_ERROR
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0530, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = inputBuffers.begin();
    if (iter != inputBuffers.end()) {
        auto omxBuffer = iter->second->omxBuffer;
        auto tempId = omxBuffer->bufferId;
        omxBuffer->bufferId = BUFFER_ID_ERROR;
        int32_t ret = g_component->EmptyThisBuffer(g_component, iter->second->omxBuffer.get());
        ASSERT_NE(ret, HDF_SUCCESS);
        omxBuffer->bufferId = tempId;
    }
}
/**
* @tc.name HdfCodecHdiSetCallbackSuccessTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0540
* @tc.desc The SetCallback interface is invoked to set the callback
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0540, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    if (g_callback != nullptr) {
        CodecCallbackTypeRelease(g_callback);
    }
    g_callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    int32_t ret = g_component->SetCallbacks(g_component, g_callback, APP_DATA);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetCallbackAppDataLenMaxTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0550
* @tc.desc When AppData is set to the maximum value in the value range,
           the SetCallback interface is invoked to set the callback
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0550, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    if (g_callback != nullptr) {
        CodecCallbackTypeRelease(g_callback);
    }
    g_callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(g_callback != nullptr);    ASSERT_TRUE(g_callback != nullptr);
    int32_t ret = g_component->SetCallbacks(g_component, g_callback, APP_DATA_MAX);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiSetCallbackAppDataLenMinTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0560
* @tc.desc When AppData is set to the minimum value in the value range,
      the SetCallback interface is invoked to set the callback
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0560, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    if (g_callback != nullptr) {
        CodecCallbackTypeRelease(g_callback);
    }
    g_callback = CodecCallbackTypeGet(nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    int32_t ret = g_component->SetCallbacks(g_component, g_callback, APP_DATA_MIN);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiRoleEnumUnsupportedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0570
* @tc.desc Invoke the ComponentRoleEnum interface to obtain component tasks
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0570, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    uint8_t role[ROLE_LEN] = {0};
    int32_t ret = g_component->ComponentRoleEnum(g_component, role, ROLE_LEN, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiFreeBufferPortIndexOutputUnsupportedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0580
* @tc.desc Set portIndex to PORT_INDEX_OUTPUT and invoke the FreeBuffer interface to obtain component tasks
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0580, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = outputBuffers.begin();
    while (iter != outputBuffers.end()) {
        int32_t ret =
            g_component->FreeBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, iter->second->omxBuffer.get());
        ASSERT_NE(ret, HDF_SUCCESS);
        iter = outputBuffers.erase(iter);
    }
}
/**
* @tc.name HdfCodecHdiFreeBufferPortIndexInputUnsupportedTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0590
* @tc.desc Set portIndex to PORT_INDEX_INPUT and invoke the FreeBuffer interface to obtain component tasks
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0590, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = inputBuffers.begin();
    while (iter != inputBuffers.end()) {
        int32_t ret =
            g_component->FreeBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, iter->second->omxBuffer.get());
        ASSERT_NE(ret, HDF_SUCCESS);
        iter = inputBuffers.erase(iter);
    }
}
/**
* @tc.name HdfCodecHdiSetStateIdleToLoadedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0600
* @tc.desc The status of the component that invokes the SendCommand
           interface changes from OMX_StateIdle to OMX_StateLoaded
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0600, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateLoaded, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);

    OMX_STATETYPE state = OMX_StateInvalid;
    do {
        usleep(100);
        auto ret = g_component->GetState(g_component, &state);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } while (state != OMX_StateLoaded);
}
/**
* @tc.name HdfCodecHdiDeInitUnsupportedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0610
* @tc.desc Invoke the ComponentDeInit interface to deinitialize the component
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0610, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->ComponentDeInit(g_component);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiDestoryComponentSuccessTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0620
* @tc.desc Invoke the DestoryComponent interface to destroy components
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0620, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    ASSERT_TRUE(g_manager != nullptr);
    int ret = g_manager->DestroyComponent(g_componentId);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name HdfCodecHdiCallbackTypeReleaseAndManagerReleaseTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_adapter_0630
* @tc.desc Destroy callback events and component management objects
*/
HWTEST_F(CodecHdiAdapterTest, SUB_DriverSystem_CodecHdi_adapter_0630, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    CodecCallbackTypeRelease(g_callback);
    CodecComponentManagerRelease();
    g_callback = nullptr;
    g_manager = nullptr;
}
}  // namespace
