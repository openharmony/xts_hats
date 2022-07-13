/*
 * Copyright (c) 2021 Shenzhen Kaihong DID Co., Ltd.
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
#include <buffer_handle.h>
#include <buffer_handle_utils.h>
#include <gtest/gtest.h>
#include <hdf_log.h>
#include <idisplay_gralloc.h>
#include <osal_mem.h>
#include <securec.h>
#include <servmgr_hdi.h>
#include "codec_callback_type_stub.h"
#include "codec_component_manager.h"
#include "codec_component_type.h"
#include "hdf_io_service_if.h"

#define HDF_LOG_TAG codec_hdi_test

using namespace std;
using namespace testing::ext;

namespace {
struct CodecComponentManager *g_manager = nullptr;
struct CodecComponentType *g_component = nullptr;
uint32_t g_componentId = 0;
struct CodecCallbackType *g_callback = nullptr;
int32_t g_count = 0;
CodecCompCapability g_codecAvcEncodeCap;

constexpr int32_t INT_TO_STR_LEN = 32;
constexpr int32_t ARRAY_TO_STR_LEN = 1000;
constexpr uint32_t WIDTH = 640;
constexpr uint32_t HEIGHT = 480;
OHOS::HDI::Display::V1_0::IDisplayGralloc *g_gralloc = nullptr;
#ifdef SUPPORT_OMX
union OMX_VERSIONTYPE g_version;
constexpr uint32_t MAX_ROLE_INDEX = 1000;
constexpr int32_t BUFFER_SIZE = 640 * 480 * 3;
constexpr int32_t ROLE_LEN = 240;
constexpr int32_t FRAMERATE = 30 << 16;
constexpr uint32_t BUFFER_ID_ERROR = 65000;
enum class PortIndex { PORT_INDEX_INPUT = 0, PORT_INDEX_OUTPUT = 1 };

template <typename T>
void InitParam(T &param)
{
    memset_s(&param, sizeof(param), 0x0, sizeof(param));
    param.nSize = sizeof(param);
    param.nVersion = g_version;
}

template <typename T>
void InitOhosParam(T &param)
{
    memset_s(&param, sizeof(param), 0x0, sizeof(param));
    param.size = sizeof(param);
    param.version = g_version;
}

struct BufferInfo {
    std::shared_ptr<OmxCodecBuffer> omxBuffer;
    std::shared_ptr<OHOS::Ashmem> sharedMem;
    BufferHandle *bufferHandle;
    BufferInfo()
    {
        omxBuffer = nullptr;
        sharedMem = nullptr;
        bufferHandle = nullptr;
    }
    ~BufferInfo()
    {
        omxBuffer = nullptr;
        if (sharedMem != nullptr) {
            sharedMem->UnmapAshmem();
            sharedMem->CloseAshmem();
            sharedMem = nullptr;
        }
        if (bufferHandle != nullptr && g_gralloc != nullptr) {
            g_gralloc->FreeMem(*bufferHandle);
            bufferHandle = nullptr;
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
#endif  // SUPPORT_OMX
class CodecHdiOmxTest : public testing::Test {
public:
    static void SetUpTestCase()
    {}
    static void TearDownTestCase()
    {}
    void SetUp()
    {
        width_ = WIDTH;
        height_ = HEIGHT;
        g_gralloc = OHOS::HDI::Display::V1_0::IDisplayGralloc::Get();
    }
    void TearDown()
    {}

public:
    uint32_t width_;
    uint32_t height_;
};

static char arrayStr[ARRAY_TO_STR_LEN];
static char *GetArrayStr(int32_t *array, int32_t arrayLen, int32_t endValue)
{
    int32_t len = 0;
    int32_t totalLen = 0;
    int32_t ret;
    char value[INT_TO_STR_LEN];
    ret = memset_s(arrayStr, sizeof(arrayStr), 0, sizeof(arrayStr));
    if (ret != EOK) {
        HDF_LOGE("%{public}s: memset_s arrayStr failed, error code: %{public}d", __func__, ret);
        return arrayStr;
    }
    for (int32_t i = 0; i < arrayLen; i++) {
        if (array[i] == endValue) {
            break;
        }
        ret = memset_s(value, sizeof(value), 0, sizeof(value));
        if (ret != EOK) {
            HDF_LOGE("%{public}s: memset_s value failed, error code: %{public}d", __func__, ret);
            return arrayStr;
        }
        ret = sprintf_s(value, sizeof(value) - 1, "0x0%X, ", array[i]);
        if (ret < 0) {
            HDF_LOGE("%{public}s: sprintf_s value failed, error code: %{public}d", __func__, ret);
            return arrayStr;
        }
        len = strlen(value);
        ret = memcpy_s(arrayStr + totalLen, len, value, len);
        if (ret != EOK) {
            HDF_LOGE("%{public}s: memcpy_s arrayStr failed, error code: %{public}d", __func__, ret);
            return arrayStr;
        }
        totalLen += len;
    }
    return arrayStr;
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
        HDF_LOGI("frameRate.min:%{public}d", cap->port.video.frameRate.min);
        HDF_LOGI("frameRate.max:%{public}d", cap->port.video.frameRate.max);
        HDF_LOGI("supportPixFmts:%{public}s", GetArrayStr(cap->port.video.supportPixFmts, PIX_FORMAT_NUM, 0));
        HDF_LOGI("bitRatemode:%{public}s", GetArrayStr((int32_t *)cap->port.video.bitRatemode, BIT_RATE_MODE_NUM, 0));
        HDF_LOGI("measuredFrameRate:%{public}s",
                 GetArrayStr(cap->port.video.measuredFrameRate, MEASURED_FRAME_RATE_NUM, 0));
    } else {
        HDF_LOGI(":%{public}s", GetArrayStr(cap->port.audio.sampleFormats, SAMPLE_FMT_NUM, AUDIO_SAMPLE_FMT_INVALID));
        HDF_LOGI(":%{public}s", GetArrayStr(cap->port.audio.sampleRate, SAMPLE_RATE_NUM, AUD_SAMPLE_RATE_INVALID));
        HDF_LOGI(":%{public}s", GetArrayStr(cap->port.audio.channelLayouts, CHANNEL_NUM, -1));
        HDF_LOGI(":%{public}s", GetArrayStr(cap->port.audio.channelCount, CHANNEL_NUM, -1));
    }
    HDF_LOGI("-------------------------------------------------------------------");
}
/**
* @tc.name  HdfCodecHdiInitTest
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0010
* @tc.desc   Obtain the CodecCompoentManager instance and initialize the Callback service
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0010, Function | MediumTest | Level3)
{
    g_manager = GetCodecComponentManager();
    ASSERT_TRUE(g_manager != nullptr);
    g_callback = CodecCallbackTypeStubGetInstance();
    ASSERT_TRUE(g_callback != nullptr);
}
/**
* @tc.name  HdfCodecHdiGetComponentNumTest
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0020
* @tc.desc   Obtain the total number of codec HDI2.0 capability sets
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0020, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    g_count = g_manager->GetComponentNum();
    ASSERT_TRUE(g_count > 0);
}
/**
* @tc.name  HdfCodecHdiGetCapabilityListTest
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0030
* @tc.desc   Obtains the capability data of the codec capability and prints the data to the Hilog
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0030, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_count > 0);
    ASSERT_TRUE(g_manager != nullptr);
    CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * g_count);
    ASSERT_TRUE(capList != nullptr);
    g_manager->GetComponentCapabilityList(capList, g_count);
    for (int32_t i = 0; i < g_count; i++) {
        PrintCapability(&capList[i], i);
        if (capList[i].type == VIDEO_ENCODER && capList[i].role == MEDIA_ROLETYPE_VIDEO_AVC) {
            g_codecAvcEncodeCap = capList[i];
        }
    }
    OsalMemFree(capList);
    capList = nullptr;
}
// Test CreateComponent
/**
* @tc.name  HdfCodecHdiCreateComponentTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0040
* @tc.desc   Open the OpenMax component based on a name that does not exist
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0040, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    char name[] = "test";
    int32_t ret = g_manager->CreateComponent(&g_component, &g_componentId, (char *)name, (int64_t)this, g_callback);
    ASSERT_NE(ret, HDF_SUCCESS);
    ASSERT_TRUE(g_component == nullptr);
}
/**
* @tc.name  HdfCodecHdiCreateComponentTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0050
* @tc.desc   When a component is opened, the name of the component is transferred to a null pointer
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0050, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    int32_t ret = g_manager->CreateComponent(&g_component, &g_componentId, nullptr, (int64_t)this, g_callback);
    ASSERT_NE(ret, HDF_SUCCESS);
    ASSERT_TRUE(g_component == nullptr);
}
/**
* @tc.name  HdfCodecHdiCreateComponentTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0060
* @tc.desc   open OMX.rk.video_encoder.avc 
*/
#ifdef SUPPORT_OMX
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0060, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    ASSERT_TRUE(g_codecAvcEncodeCap.role == MEDIA_ROLETYPE_VIDEO_AVC);
    int32_t ret = g_manager->CreateComponent(&g_component, &g_componentId, g_codecAvcEncodeCap.compName, (int64_t)this,
                                             g_callback);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(g_component != nullptr);
}
/**
* @tc.name  HdfCodecHdiCreateComponentTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0070
* @tc.desc   open OMX.rk.video_dncoder.avc 
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0070, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    struct CodecCallbackType *callback = CodecCallbackTypeStubGetInstance();
    ASSERT_TRUE(callback != nullptr);
    ASSERT_TRUE(g_codecAvcEncodeCap.role == MEDIA_ROLETYPE_VIDEO_AVC);
    struct CodecComponentType *component = nullptr;
    uint32_t componentId = 0;
    int32_t ret =
        g_manager->CreateComponent(&component, &componentId, g_codecAvcEncodeCap.compName, (int64_t)this, callback);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(component != nullptr);
    ret = g_manager->DestroyComponent(componentId);
    ASSERT_EQ(ret, HDF_SUCCESS);
    CodecComponentTypeRelease(component);
    component = nullptr;
    CodecCallbackTypeStubRelease(callback);
    callback = nullptr;
}
// Test GetComponentVersion
/**
* @tc.name  HdfCodecHdiGetVersionTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0080
* @tc.desc   Reads the version information of an open component
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0080, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    struct CompVerInfo verInfo;
    int32_t ret = g_component->GetComponentVersion(g_component, &verInfo);
    g_version = verInfo.compVersion;
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_STREQ(g_codecAvcEncodeCap.compName, verInfo.compName);
}
/**
* @tc.name  HdfCodecHdiGetVersionTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0090
* @tc.desc  The input parameter is empty. GetComponentVersion is error.
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0090, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->GetComponentVersion(g_component, nullptr);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0100
* @tc.desc  The input parameter structure pointer is null when the OMX_IndexParamVideoPortFormat parameter is read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecPixFormatParam pixFormat;
    InitOhosParam(pixFormat);
    pixFormat.portIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    auto ret = g_component->GetParameter(g_component, OMX_IndexParamCodecPixFormat,
                                         reinterpret_cast<int8_t *>(&pixFormat), sizeof(pixFormat));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0110
* @tc.desc  The OMX_IndexParamVideoPortFormat parameter of the input port of the component is normally read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0110, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecPixFormatParam pixFormat;
    InitOhosParam(pixFormat);
    pixFormat.portIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = g_component->GetParameter(g_component, OMX_IndexParamCodecPixFormat,
                                         reinterpret_cast<int8_t *>(&pixFormat), sizeof(pixFormat));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
// Test GetParameter
/**
* @tc.name  HdfCodecHdiGetParameterTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0120
* @tc.desc  The version information is not set for the input parameter. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0120, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto ret = g_component->GetParameter(g_component, OMX_IndexParamVideoPortFormat, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0130
* @tc.desc  The input parameter structure does not match the index. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0130, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiGetParameterTest_007
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0140
* @tc.desc  The input parameter index is not supported. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0140, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    memset_s(&param, sizeof(param), 0, sizeof(param));
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    param.eCompressionFormat = OMX_VIDEO_CodingAVC;
    auto ret = g_component->GetParameter(g_component, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_008
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0150
* @tc.desc  The input parameter prot is not supported. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0150, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiGetParameterTest_009
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0160
* @tc.desc  The input parameter MX_IndexVideoStartUnused. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0160, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = g_component->GetParameter(g_component, OMX_IndexVideoStartUnused, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}

// SetParameter with no version
/**
* @tc.name  HdfCodecHdiSetParameterTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0170
* @tc.desc  Setting the OMX_IndexParamVideoPortFormat Parameter of the Component Input Port
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0170, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiSetParameterTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0180
* @tc.desc  The input parameter version information is not set. As a result, the parameter setting fails
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0180, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    memset_s(&param, sizeof(param), 0, sizeof(param));
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    int32_t ret = g_component->SetParameter(g_component, OMX_IndexParamVideoPortFormat,
                                            reinterpret_cast<int8_t *>(&param), sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetParameterTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0190
* @tc.desc  The input parameter is  null 
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0190, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SetParameter(g_component, OMX_IndexParamVideoPortFormat, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetParameterTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0200
* @tc.desc  Parameter does not match index
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0200, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiSetParameterTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0210
* @tc.desc  The parameter index is not supported
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0210, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiSetParameterTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0220
* @tc.desc  
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0220, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecPixFormatParam pixFormat;
    InitOhosParam(pixFormat);
    pixFormat.portIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = g_component->GetParameter(g_component, OMX_IndexParamCodecPixFormat,
                                         reinterpret_cast<int8_t *>(&pixFormat), sizeof(pixFormat));
    ASSERT_EQ(ret, HDF_SUCCESS);
    pixFormat.codecColorFormat = PIXEL_FMT_RGB_555;
    ret = g_component->SetParameter(g_component, OMX_IndexParamCodecPixFormat, reinterpret_cast<int8_t *>(&pixFormat),
                                    sizeof(pixFormat));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
// Test GetConfig
/**
* @tc.name  HdfCodecHdiGetConfigTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0230
* @tc.desc  The configuration is read normally
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0230, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    int32_t ret = g_component->GetConfig(g_component, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetConfigTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0240
* @tc.desc  Reading Unsupported Port Parameters
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0240, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    int32_t ret = g_component->GetConfig(g_component, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetConfigTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0250
* @tc.desc  The input parameter is a null pointer
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0250, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->GetConfig(g_component, OMX_IndexConfigVideoBitrate, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetConfigTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0260
* @tc.desc  The input parameter index is not supported
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0260, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    int32_t ret = g_component->GetConfig(g_component, OMX_IndexVideoStartUnused, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
// Test SetConfig
/**
* @tc.name  HdfCodecHdiSetConfigTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0270
* @tc.desc  The configuration is successful
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0270, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    param.nEncodeBitrate = FRAMERATE;
    int32_t ret = g_component->SetConfig(g_component, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetConfigTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0280
* @tc.desc  Configure parameters. The port cannot be modified
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0280, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    param.nEncodeBitrate = FRAMERATE;
    int32_t ret = g_component->SetConfig(g_component, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetConfigTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0290
* @tc.desc  The input parameter is a null poin
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0290, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SetConfig(g_component, OMX_IndexConfigVideoBitrate, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetConfigTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0300
* @tc.desc  The input parameter index is not supported
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0300, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    int32_t ret = g_component->SetConfig(g_component, OMX_IndexVideoStartUnused, reinterpret_cast<int8_t *>(&param),
                                         sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
// Test GetExtensionIndex
/**
* @tc.name  HdfCodecHdiGetExtensionIndexTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0310
* @tc.desc  Obtaining the extended index normally
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0310, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_INDEXTYPE indexType;
    int32_t ret =
        g_component->GetExtensionIndex(g_component, "OMX.Topaz.index.param.extended_video", (uint32_t *)&indexType);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetExtensionIndexTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0320
* @tc.desc  The parameter name is null pointer
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0320, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_INDEXTYPE indexType;
    int32_t ret = g_component->GetExtensionIndex(g_component, nullptr, (uint32_t *)&indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetExtensionIndexTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0330
* @tc.desc  Unsupported parameter
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0330, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_INDEXTYPE indexType;
    int32_t ret =
        g_component->GetExtensionIndex(g_component, "OMX.Topaz.index.param.extended_test", (uint32_t *)&indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetExtensionIndexTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0340
* @tc.desc  Index is null pointer
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0340, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->GetExtensionIndex(g_component, "OMX.Topaz.index.param.extended_video", nullptr);
    ASSERT_NE(ret, HDF_SUCCESS);
}
// Test GetState
/**
* @tc.name  HdfCodecHdiGetStateTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0350
* @tc.desc  Reads the current status of the component
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0350, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_STATETYPE state;
    int32_t ret = g_component->GetState(g_component, &state);
    ASSERT_EQ(state, OMX_StateLoaded);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetStateTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0360
* @tc.desc  The input parameter is a null pointer
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0360, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->GetState(g_component, nullptr);
    ASSERT_NE(ret, HDF_SUCCESS);
}
// Test ComponentTunnelRequest
/**
* @tc.name  HdfCodecHdiTunnelRequestTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0370
* @tc.desc  The interface is not supported
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0370, Function | MediumTest | Level3)
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
// Test SendCommand
/**
* @tc.name  HdfCodecHdiLoadedToExecutingTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0380
* @tc.desc The status is changed to OMX_StateExecuting
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0380, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateExecuting, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiLoadedToIdleTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0390
* @tc.desc The status is changed to OMX_StateIdle
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0390, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateIdle, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);

    OMX_STATETYPE state;
    ret = g_component->GetState(g_component, &state);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0400
* @tc.desc The input buffer type is CODEC_BUFFER_TYPE_INVALID
*/
struct OmxCodecBuffer allocBuffer;
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0400, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiAllocateBufferTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0410
* @tc.desc The input buffer type is CODEC_BUFFER_TYPE_VIRTUAL_ADDR
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0410, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_VIRTUAL_ADDR;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0420
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_INVALID
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0420, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_INVALID;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0430
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_VIRTUAL_ADDR
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0430, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_VIRTUAL_ADDR;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
// Test UseBuffer
/**
* @tc.name  HdfCodecHdiUseBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0440
* @tc.desc The input buffer type is CODEC_BUFFER_TYPE_INVALID
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0440, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiUseBufferTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0450
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_INVALID
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0450, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiUseBufferTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0460
* @tc.desc The input buffer type is CODEC_BUFFER_TYPE_VIRTUAL_ADDR
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0460, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiUseBufferTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0470
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_VIRTUAL_ADDR
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0470, Function | MediumTest | Level3)
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
// Use buffer on input index
/**
* @tc.name  HdfCodecHdiUseBufferTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0480
* @tc.desc The intput buffer type is CODEC_BUFFER_TYPE_AVSHARE_MEM_FD
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0480, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_INPUT;
    auto err = g_component->GetParameter(g_component, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int bufferSize = param.nBufferSize;
    int bufferCount = param.nBufferCountActual;
    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_INPUT, bufferCount - 1, bufferSize);
    ASSERT_TRUE(ret);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0490
* @tc.desc The intput buffer type is CODEC_BUFFER_TYPE_HANDLE
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0490, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    AllocInfo alloc = {.width = this->width_,
                       .height = this->height_,
                       .usage = HBM_USE_CPU_READ | HBM_USE_CPU_WRITE | HBM_USE_MEM_DMA,
                       .format = PIXEL_FMT_YCBCR_420_SP};
    ASSERT_TRUE(g_gralloc != nullptr);
    BufferHandle *bufferHandle = nullptr;
    auto err = g_gralloc->AllocMem(alloc, bufferHandle);
    ASSERT_EQ(err, DISPLAY_SUCCESS);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    size_t handleSize =
        sizeof(BufferHandle) + (sizeof(int32_t) * (bufferHandle->reserveFds + bufferHandle->reserveInts));
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = g_version;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_HANDLE;

    omxBuffer->bufferLen = handleSize;
    omxBuffer->buffer = reinterpret_cast<uint8_t *>(bufferHandle);
    omxBuffer->allocLen = handleSize;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    err = g_component->UseBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, omxBuffer.get());
    if (err != HDF_SUCCESS) {
        HDF_LOGE("%{public}s failed to UseBuffer with  input port", __func__);
        omxBuffer = nullptr;
    }
    ASSERT_EQ(err, HDF_SUCCESS);
    omxBuffer->bufferLen = 0;

    std::shared_ptr<BufferInfo> bufferInfo = std::make_shared<BufferInfo>();
    ASSERT_TRUE(bufferInfo != nullptr);
    bufferInfo->omxBuffer = omxBuffer;
    bufferInfo->bufferHandle = bufferHandle;
    inputBuffers.emplace(std::make_pair(omxBuffer->bufferId, bufferInfo));
}
// Use Buffer on output index
/**
* @tc.name  HdfCodecHdiUseBufferTest_007
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0500
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_AVSHARE_MEM_FD
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0500, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_OUTPUT;
    auto err = g_component->GetParameter(g_component, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int bufferSize = param.nBufferSize;
    int bufferCount = param.nBufferCountActual;
    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_OUTPUT, bufferCount - 1, bufferSize);
    ASSERT_TRUE(ret);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_008
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0510
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_DYNAMIC_HANDLE
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0510, Function | MediumTest | Level3)
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
    ASSERT_EQ(err, HDF_SUCCESS);
    omxBuffer->bufferLen = 0;

    std::shared_ptr<BufferInfo> bufferInfo = std::make_shared<BufferInfo>();
    ASSERT_TRUE(bufferInfo != nullptr);
    bufferInfo->omxBuffer = omxBuffer;
    outputBuffers.emplace(std::make_pair(omxBuffer->bufferId, bufferInfo));
}
// Use buffer on input index error when OMX_ErrorInsufficientResources
/**
* @tc.name  HdfCodecHdiUseBufferTest_009
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0520
* @tc.desc The input buffer is full
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0520, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_INPUT;
    auto err = g_component->GetParameter(g_component, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int bufferSize = param.nBufferSize;
    int bufferCount = 1;
    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_INPUT, bufferCount, bufferSize);
    ASSERT_FALSE(ret);
}
// Use buffer on output index error when OMX_ErrorInsufficientResources
/**
* @tc.name  HdfCodecHdiUseBufferTest_010
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0530
* @tc.desc The output buffer is full
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0530, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_OUTPUT;
    auto err = g_component->GetParameter(g_component, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int bufferSize = param.nBufferSize;
    int bufferCount = 1;
    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_OUTPUT, bufferCount, bufferSize);
    ASSERT_FALSE(ret);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0540
* @tc.desc The input buffer is full
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0540, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0550
* @tc.desc The output buffer is full
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0550, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    int32_t ret = g_component->AllocateBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiUseEglImageTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0560
* @tc.desc The interface is invoked successfully. The OMX does not support this function
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0560, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiWaitStateTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0570
* @tc.desc Switching the Component Status to Idle
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0570, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiFillThisBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0580
* @tc.desc FillThisBuffer test
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0580, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateExecuting, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
    OMX_STATETYPE state = OMX_StateInvalid;
    do {
        ret = g_component->GetState(g_component, &state);
        ASSERT_EQ(ret, HDF_SUCCESS);
        usleep(100);
    } while (state != OMX_StateExecuting);

    // fill this buffer need OMX_StateExecuting
    auto iter = outputBuffers.begin();
    if (iter != outputBuffers.end()) {
        int32_t ret = g_component->FillThisBuffer(g_component, iter->second->omxBuffer.get());
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
}
/**
* @tc.name  HdfCodecHdiFillThisBufferTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0590
* @tc.desc The buffer ID is incorrect
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0590, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiEmptyThisBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0600
* @tc.desc EmptyThisBuffer test
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0600, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = inputBuffers.begin();
    if (iter != inputBuffers.end()) {
        int32_t ret = g_component->EmptyThisBuffer(g_component, iter->second->omxBuffer.get());
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
}
/**
* @tc.name  HdfCodecHdiEmptyThisBufferTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0610
* @tc.desc The buffer ID is incorrect
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0610, Function | MediumTest | Level3)
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
* @tc.name  HdfCodecHdiSetCallbackTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0620
* @tc.desc Setting Component Callbacks
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0620, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    if (g_callback != nullptr) {
        CodecCallbackTypeStubRelease(g_callback);
    }
    g_callback = CodecCallbackTypeStubGetInstance();
    ASSERT_TRUE(g_callback != nullptr);
    int32_t ret = g_component->SetCallbacks(g_component, g_callback, (int64_t)this);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetCallbackTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0630
* @tc.desc The callback pointer is null
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0630, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    uint8_t role[ROLE_LEN] = {0};
    int32_t ret = g_component->ComponentRoleEnum(g_component, role, ROLE_LEN, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiRoleEnumTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0640
* @tc.desc Obtaining Component Roles Based on Indexes
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0640, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->ComponentRoleEnum(g_component, nullptr, 0, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiRoleEnumTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0650
* @tc.desc The role is null
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0650, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    uint8_t role[ROLE_LEN] = {0};
    int32_t ret = g_component->ComponentRoleEnum(g_component, role, ROLE_LEN, MAX_ROLE_INDEX);
    ASSERT_NE(ret, HDF_SUCCESS);
}
// Executing to Idle
/**
* @tc.name  HdfCodecHdiRoleEnumTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0660
* @tc.desc The index is too large
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0660, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateIdle, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
// Release input buffer
/**
* @tc.name  HdfCodecHdiExecutingToIdleTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0670
* @tc.desc The component enters the OMX_Idle state
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0670, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = outputBuffers.begin();
    if (iter != outputBuffers.end()) {
        auto omxBuffer = iter->second->omxBuffer;
        auto tempId = omxBuffer->bufferId;
        omxBuffer->bufferId = BUFFER_ID_ERROR;
        int32_t ret = g_component->FreeBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, omxBuffer.get());
        ASSERT_NE(ret, HDF_SUCCESS);
        omxBuffer->bufferId = tempId;
    }
}
/**
* @tc.name  HdfCodecHdiFreeBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0680
* @tc.desc The buffer ID of the output port is incorrect
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0680, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = outputBuffers.begin();
    while (iter != outputBuffers.end()) {
        int32_t ret =
            g_component->FreeBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, iter->second->omxBuffer.get());
        ASSERT_EQ(ret, HDF_SUCCESS);
        iter = outputBuffers.erase(iter);
    }
}
// Release input buffer
/**
* @tc.name  HdfCodecHdiFreeBufferTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0690
* @tc.desc Test on normal release of the output port
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0690, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = inputBuffers.begin();
    if (iter != inputBuffers.end()) {
        auto omxBuffer = iter->second->omxBuffer;
        auto tempId = omxBuffer->bufferId;
        omxBuffer->bufferId = BUFFER_ID_ERROR;
        int32_t ret = g_component->FreeBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, omxBuffer.get());
        ASSERT_NE(ret, HDF_SUCCESS);
        omxBuffer->bufferId = tempId;
    }
}
/**
* @tc.name  HdfCodecHdiFreeBufferTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0700
* @tc.desc The buffer ID of the input port is incorrect
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0700, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    auto iter = inputBuffers.begin();
    while (iter != inputBuffers.end()) {
        int32_t ret =
            g_component->FreeBuffer(g_component, (uint32_t)PortIndex::PORT_INDEX_INPUT, iter->second->omxBuffer.get());
        ASSERT_EQ(ret, HDF_SUCCESS);
        iter = inputBuffers.erase(iter);
    }
}
// When ComponentDeInit, must change to Loaded State
/**
* @tc.name  HdfCodecHdiFreeBufferTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0710
* @tc.desc Test on normal release of the input port
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0710, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->SendCommand(g_component, OMX_CommandStateSet, OMX_StateLoaded, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
    // State changed OMX_StateIdle when release all this buffer
    OMX_STATETYPE state = OMX_StateInvalid;
    do {
        usleep(100);
        auto ret = g_component->GetState(g_component, &state);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } while (state != OMX_StateLoaded);
}
/**
* @tc.name  HdfCodecHdiIdleToLoadedTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0720
* @tc.desc The component enters the OMX_Loaded state
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0720, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret = g_component->ComponentDeInit(g_component);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiDeInitTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0730
* @tc.desc Deinitialize the component
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0730, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_component != nullptr);
    ASSERT_TRUE(g_manager != nullptr);
    int ret = g_manager->DestroyComponent(g_componentId);
    ASSERT_EQ(ret, HDF_SUCCESS);
    CodecComponentTypeRelease(g_component);
    g_component = nullptr;
}
/**
* @tc.name  HdfCodecHdiDestoryComponentTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0740
* @tc.desc Releasing Components
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0740, Function | MediumTest | Level3)
{
    int ret = g_manager->DestroyComponent(g_componentId);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
#endif  // SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiDestoryComponentTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0750
* @tc.desc he component does not exist
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0750, Function | MediumTest | Level3)
{
    ASSERT_TRUE(g_manager != nullptr);
    ASSERT_TRUE(g_callback != nullptr);
    CodecCallbackTypeStubRelease(g_callback);
    CodecComponentManagerRelease();
    g_callback = nullptr;
    g_manager = nullptr;
}
}  // namespace
