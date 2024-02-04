/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
#include <gtest/gtest.h>
#include <hdf_log.h>
#include <securec.h>
#include <servmgr_hdi.h>
#include <vector>
#include <benchmark/benchmark.h>
#include "codec_omx_ext.h"
#include "v2_0/codec_callback_service.h"
#include "v2_0/icodec_component.h"
#include "v2_0/icodec_component_manager.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/display_buffer_type.h"
#include "v1_0/include/idisplay_buffer.h"

#define HDF_LOG_TAG codec_hdi_test
#define COMPONENTNUM 0
#define TUNNELECOMP 1002
#define TUNNELEDPORT 101

using namespace std;
using namespace testing::ext;
using OHOS::sptr;
using OHOS::HDI::Base::NativeBuffer;
using namespace OHOS::HDI::Codec::V2_0;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_0;
namespace {
constexpr int32_t WIDTH = 640;
#ifdef SUPPORT_OMX
constexpr uint32_t MAX_ROLE_INDEX = 1000;
#endif
constexpr int FD_DEFAULT = -1;
constexpr int64_t APP_DATA = 3;
constexpr int32_t HEIGHT = 480;
constexpr int32_t BUFFER_SIZE = WIDTH * HEIGHT * 3;
constexpr int32_t FRAMERATE = 30 << 16;
constexpr uint32_t BUFFER_ID_ERROR = 65000;
static IDisplayBuffer *gralloc_ = nullptr;
static sptr<ICodecComponent> component_ = nullptr;
static sptr<ICodecCallback> callback_ = nullptr;
static sptr<ICodecComponentManager> manager_ = nullptr;
static OHOS::HDI::Codec::V2_0::CodecVersionType version_;
static inline std::string compName_ = "";

class CodecBenchmarkOmxTest : public benchmark::Fixture {
public:
    enum class PortIndex { PORT_INDEX_INPUT = 0, PORT_INDEX_OUTPUT = 1 };
    template <typename T>
    void InitParam(T &param)
    {
        memset_s(&param, sizeof(param), 0x0, sizeof(param));
        param.nSize = sizeof(param);
        param.nVersion.nVersion = 1;
    }

    void InitOmxCodecBuffer(OmxCodecBuffer& buffer, CodecBufferType type)
    {
        buffer.bufferType = type;
        buffer.fenceFd = -1;
        buffer.version = version_;
        buffer.allocLen = BUFFER_SIZE;
        buffer.fd = FD_DEFAULT;
        buffer.bufferhandle = nullptr;
        buffer.pts = 0;
        buffer.flag = 0;
        buffer.size = sizeof(OmxCodecBuffer);
        buffer.type = READ_ONLY_TYPE;
    }

    void Init()
    {
        int32_t count = 0;
        (void)manager_->GetComponentNum(count);
        if (count > 0) {
            std::vector<CodecCompCapability> capList;
            auto err = manager_->GetComponentCapabilityList(capList, count);
            ASSERT_TRUE(err == HDF_SUCCESS);
            compName_ = capList[0].compName;
        }

        if (manager_ == nullptr) {
            return;
        }
        callback_ = new CodecCallbackService();
        if (callback_ == nullptr) {
            return;
        }
        if (compName_.empty()) {
            return;
        }

        auto ret = manager_->CreateComponent(component_, componentId_, compName_.data(),
                                             APP_DATA, callback_);
        ASSERT_TRUE(component_ != nullptr);
        if (ret != HDF_SUCCESS) {
            return;
        }
        struct CompVerInfo verInfo;
        ret = component_->GetComponentVersion(verInfo);
        ASSERT_TRUE(component_ != nullptr);
        if (ret != HDF_SUCCESS) {
            return;
        }
        version_ = verInfo.compVersion;
        return;
    }

    void SetUp(benchmark::State &state)
    {
        manager_ = ICodecComponentManager::Get();
        gralloc_ = IDisplayBuffer::Get();
        Init();
        if (manager_ == nullptr) {
            std::cout<<"GetCodecComponentManager ret nullptr"<<std::endl;
            return;
        }
    }

    void TearDown(benchmark::State &state)
    {
        if (manager_ != nullptr && component_ != nullptr) {
            manager_->DestroyComponent(componentId_);
        }
        component_ = nullptr;
        callback_ = nullptr;
        manager_ = nullptr;
        gralloc_ = nullptr;
    }

public:
    uint32_t componentId_ = 0;
    const static uint32_t inputIndex = static_cast<uint32_t>(PortIndex::PORT_INDEX_INPUT);
    const static uint32_t outputIndex = static_cast<uint32_t>(PortIndex::PORT_INDEX_OUTPUT);
};

template <typename T>
void ObjectToVector(T &param, std::vector<int8_t> &vec)
{
    int8_t *paramPointer = reinterpret_cast<int8_t *>(&param);
    vec.insert(vec.end(), paramPointer, paramPointer + sizeof(param));
}

BENCHMARK_F(CodecBenchmarkOmxTest, GetComponentVersion)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    struct CompVerInfo verInfo;
    for (auto _ : state) {
        ret = component_->GetComponentVersion(verInfo);
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, GetComponentVersion)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, GetParameter)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    std::vector<int8_t> inParam;
    std::vector <int8_t> outParam;
    for (auto _ : state) {
        ret = component_->GetParameter(OMX_IndexParamVideoPortFormat, inParam, outParam);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, GetParameter)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, SetParameter)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    InitParam(param);
    param.nPortIndex = inputIndex;
    std::vector<int8_t> paramVec;
    ObjectToVector(param, paramVec);
    for (auto _ : state) {
        ret = component_->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, SetParameter)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, GetConfig)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = outputIndex;
    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    for (auto _ : state) {
        ret = component_->GetConfig(OMX_IndexConfigVideoBitrate, inParam, outParam);
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, GetConfig)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, SetConfig)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = inputIndex;
    param.nEncodeBitrate = FRAMERATE;
    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    for (auto _ : state) {
        ret = component_->SetConfig(OMX_IndexConfigVideoBitrate, inParam);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, SetConfig)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, GetExtensionIndex)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    uint32_t indexType = 0;
    for (auto _ : state) {
        ret = component_->GetExtensionIndex("OMX.Topaz.index.param.extended_test", indexType);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, GetExtensionIndex)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, GetState)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    CodecStateType codecState =CODEC_STATE_INVALID;
    for (auto _ : state) {
        ret = component_->GetState(codecState);
    }
    ASSERT_EQ(codecState, CODEC_STATE_LOADED);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, GetState)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

#ifdef SUPPORT_OMX
BENCHMARK_F(CodecBenchmarkOmxTest, ComponentTunnelRequest)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    const int32_t tunneledComp = TUNNELECOMP;
    const uint32_t tunneledPort = TUNNELEDPORT;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_INPUT;
    for (auto _ : state) {
    ret = component_->ComponentTunnelRequest(outputIndex, tunneledComp, tunneledPort,
        tunnelSetup, tunnelSetup);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, ComponentTunnelRequest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
#endif

BENCHMARK_F(CodecBenchmarkOmxTest, SendCommand)(benchmark::State &state)
{
    std::vector<int8_t> cmdData;
    int32_t ret;
    for (auto _ : state) {
        Init();
        ret = component_->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
        manager_->DestroyComponent(componentId_);
        if (manager_ != nullptr && component_ != nullptr) {
            manager_->DestroyComponent(componentId_);
        }
        component_ = nullptr;
        callback_ = nullptr;
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, SendCommand)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, AllocateBuffer)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_VIRTUAL_ADDR);
    struct OmxCodecBuffer outBuffer;
    for (auto _ : state) {
        ret = component_->AllocateBuffer(inputIndex, allocBuffer, outBuffer);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, AllocateBuffer)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, UseBuffer)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_INVALID);
    struct OmxCodecBuffer outBuffer;
    for (auto _ : state) {
        ret = component_->UseBuffer(inputIndex, omxBuffer, outBuffer);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, UseBuffer)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, UseEglImage)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    for (auto _ : state) {
        ret = component_->UseEglImage(inputIndex, omxBuffer, outbuffer, eglImageVec);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
    eglImage = nullptr;
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, UseEglImage)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, FillThisBuffer)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.bufferId = BUFFER_ID_ERROR;
    for (auto _ : state) {
        ret = component_->FillThisBuffer(omxBuffer);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, FillThisBuffer)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, EmptyThisBuffer)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.bufferId = BUFFER_ID_ERROR;
    for (auto _ : state) {
        ret = component_->EmptyThisBuffer(omxBuffer);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, EmptyThisBuffer)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkOmxTest, SetCallbacks)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    callback_ = new CodecCallbackService();
    ASSERT_TRUE(callback_ != nullptr);
    for (auto _ : state) {
        ret = component_->SetCallbacks(callback_, APP_DATA);
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, SetCallbacks)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

#ifdef SUPPORT_OMX
BENCHMARK_F(CodecBenchmarkOmxTest, ComponentRoleEnum)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    std::vector<uint8_t> role;
    for (auto _ : state) {
        ret = component_->ComponentRoleEnum(role, MAX_ROLE_INDEX);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, ComponentRoleEnum)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
#endif

BENCHMARK_F(CodecBenchmarkOmxTest, FreeBuffer)(benchmark::State &state)
{
    ASSERT_TRUE(component_ != nullptr);
    int32_t ret;
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.bufferId = BUFFER_ID_ERROR;
    for (auto _ : state) {
        ret = component_->FreeBuffer(outputIndex, omxBuffer);
    }
    ASSERT_NE(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, FreeBuffer)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

#ifdef SUPPORT_OMX
BENCHMARK_F(CodecBenchmarkOmxTest, ComponentDeInit)(benchmark::State &state)
{
    std::vector<int8_t> cmdData;
    int32_t ret;
    for (auto _ : state) {
        Init();
        ret = component_->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_LOADED, cmdData);
        CodecStateType state = CODEC_STATE_INVALID;
        do {
            usleep(100);
            ret = component_->GetState(state);
        } while (state != CODEC_STATE_LOADED);
        ret = component_->ComponentDeInit();
        if (manager_ != nullptr && component_ != nullptr) {
            manager_->DestroyComponent(componentId_);
        }
        component_ = nullptr;
        callback_ = nullptr;
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkOmxTest, ComponentDeInit)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
#endif
}  // namespace
BENCHMARK_MAIN();
