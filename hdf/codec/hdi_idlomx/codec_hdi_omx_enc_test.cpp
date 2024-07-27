/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include <securec.h>
#include <servmgr_hdi.h>
#include <vector>
#include "codec_function_utils.h"
#include "v3_0/codec_callback_service.h"

#define ERR_COUNT (-1)

using namespace std;
using namespace testing::ext;
using OHOS::sptr;
using OHOS::HDI::Base::NativeBuffer;
using namespace OHOS::HDI::Codec::V3_0;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_0;

namespace {
constexpr CodecType TYPE = CodecType::VIDEO_ENCODER;
constexpr AvCodecRole ROLE = MEDIA_ROLETYPE_VIDEO_AVC;
static sptr<ICodecComponent> g_component = nullptr;
static sptr<ICodecCallback> g_callback = nullptr;
static sptr<ICodecComponentManager> g_manager = nullptr;
static OHOS::HDI::Codec::V3_0::CodecVersionType g_version;
static std::string g_compName = "";

class CodecHdiOmxEncTest : public testing::Test {
public:
    static void SetUpTestCase()
    {
        g_manager = ICodecComponentManager::Get();
        int32_t count = 0;
        auto ret = g_manager->GetComponentNum(count);
        ASSERT_EQ(ret, HDF_SUCCESS);
        if (count <= 0) {
            return;
        }

        std::vector<CodecCompCapability> capList;
        auto err = g_manager->GetComponentCapabilityList(capList, count);
        ASSERT_TRUE(err == HDF_SUCCESS);
        for (auto cap : capList) {
            if (cap.type == TYPE && cap.role == ROLE) {
                g_compName = cap.compName;
                break;
            }
        }
    }

    static void TearDownTestCase()
    {
        g_manager = nullptr;
    }

    void SetUp()
    {
        ASSERT_TRUE(g_manager != nullptr && !g_compName.empty());
        g_callback = new CodecCallbackService();
        ASSERT_TRUE(g_callback != nullptr);
        auto ret = g_manager->CreateComponent(g_component, componentId_, g_compName.data(), APP_DATA, g_callback);
        ASSERT_EQ(ret, HDF_SUCCESS);
        ret = g_manager->CreateComponent(g_component, componentId_, "", APP_DATA, g_callback);
        ASSERT_TRUE(ret != HDF_SUCCESS);
        struct CompVerInfo verInfo;
        ret = g_component->GetComponentVersion(verInfo);
        ASSERT_EQ(ret, HDF_SUCCESS);
        g_version = verInfo.compVersion;

        func_ = new FunctionUtil(g_version);
        ASSERT_TRUE(func_ != nullptr);
    }

    void TearDown()
    {
        std::vector<int8_t> cmdData;
        if (g_component != nullptr) {
            g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_LOADED, cmdData);
        }
        if (g_manager != nullptr && g_component != nullptr) {
            g_manager->DestroyComponent(componentId_);
        }
        g_component = nullptr;
        g_callback = nullptr;
        func_ = nullptr;
    }

public:
    uint32_t componentId_ = 0;
    sptr<FunctionUtil> func_ = nullptr;
    const static uint32_t inputIndex = static_cast<uint32_t>(PortIndex::INDEX_INPUT);
    const static uint32_t outputIndex = static_cast<uint32_t>(PortIndex::INDEX_OUTPUT);
};

// Test GetComponentVersion
/**
* @tc.number : SUB_Driver_Codec_idlomx_0900
* @tc.name   : HdfCodecHdiGetVersionTest001
* @tc.desc   : Verify the GetComponentVersion function when the input parameter is valid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetVersionTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    struct CompVerInfo verInfo;
    auto ret = g_component->GetComponentVersion(verInfo);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_1000
* @tc.name   : HdfCodecHdiGetParameterTest001
* @tc.desc   : Verify the GetParameter function when the input parameter pixFormat.portIndex is outputIndex.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetParameterTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam pixFormat;
    func_->InitExtParam(pixFormat);
    pixFormat.portIndex = outputIndex;
    pixFormat.codecColorIndex = 0;

    std::vector<int8_t> inParam;
    func_->ObjectToVector(pixFormat, inParam);

    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_1100
* @tc.name   : HdfCodecHdiGetParameterTest002
* @tc.desc   : Verify the GetParameter function when the input parameter pixFormat.portIndex is inputIndex.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetParameterTest002, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam pixFormat;
    func_->InitExtParam(pixFormat);
    pixFormat.portIndex = inputIndex;
    pixFormat.codecColorIndex = 0;

    std::vector<int8_t> inParam;
    func_->ObjectToVector(pixFormat, inParam);

    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

// Test GetParameter
/**
* @tc.number : SUB_Driver_Codec_idlomx_1200
* @tc.name   : HdfCodecHdiGetParameterTest003
* @tc.desc   : Verify the GetParameter function when the input parameter is valid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetParameterTest003, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> inParam;
    std::vector <int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexParamVideoPortFormat, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_1300
* @tc.name   : HdfCodecHdiGetParameterTest004
* @tc.desc   : Verify the GetParameter function when the input parameter is valid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetParameterTest004, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    func_->InitParam(param);
    param.nPortIndex = inputIndex;
    param.eCompressionFormat = OMX_VIDEO_CodingAVC;
    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);

    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_1400
* @tc.name   : HdfCodecHdiGetParameterTest005
* @tc.desc   : Verify the GetParameter function when the input parameter is invalid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetParameterTest005, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    int32_t ret = memset_s(&param, sizeof(param), 0, sizeof(param));
    ASSERT_EQ(ret, EOK);
    param.nPortIndex = inputIndex;
    param.eCompressionFormat = OMX_VIDEO_CodingAVC;
    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);

    std::vector<int8_t> outParam;
    ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_1500
* @tc.name   : HdfCodecHdiGetParameterTest006
* @tc.desc   : Verify the GetParameter function when the input parameter is invalid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetParameterTest006, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    func_->InitParam(param);
    param.nPortIndex = inputIndex;
    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);

    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_1600
* @tc.name   : HdfCodecHdiGetParameterTest007
* @tc.desc   : Verify the GetParameter function when the input parameter is invalid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetParameterTest007, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    func_->InitParam(param);
    param.nPortIndex = inputIndex;
    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);

    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexVideoStartUnused, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_1700
* @tc.name   : HdfCodecHdiSetParameterTest001
* @tc.desc   : Verify the SetParameter function when the input parameter is valid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetParameterTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    func_->InitParam(param);
    param.nPortIndex = inputIndex;
    std::vector<int8_t> paramVec;
    func_->ObjectToVector(param, paramVec);
    auto ret = g_component->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_1800
* @tc.name   : HdfCodecHdiSetParameterTest002
* @tc.desc   : Verify the SetParameter function when the input parameter is isvalid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetParameterTest002, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    int32_t ret = memset_s(&param, sizeof(param), 0, sizeof(param));
    ASSERT_EQ(ret, EOK);
    param.nPortIndex = inputIndex;
    std::vector<int8_t> paramVec;
    func_->ObjectToVector(param, paramVec);
    ret = g_component->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_1900
* @tc.name   : HdfCodecHdiSetParameterTest003
* @tc.desc   : Verify the SetParameter function when the input parameter is isvalid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetParameterTest003, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> paramVec;
    auto ret = g_component->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_2000
* @tc.name   : HdfCodecHdiSetParameterTest004
* @tc.desc   : Verify the SetParameter function when the input parameter param.nPortIndex = inputIndex.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetParameterTest004, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    func_->InitParam(param);
    param.nPortIndex = inputIndex;
    std::vector<int8_t> paramVec;
    func_->ObjectToVector(param, paramVec);
    auto ret = g_component->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_2100
* @tc.name   : HdfCodecHdiSetParameterTest005
* @tc.desc   : Verify the SetParameter function when the input parameter OMX_IndexVideoStartUnused.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetParameterTest005, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    func_->InitParam(param);
    param.nPortIndex = inputIndex;
    std::vector<int8_t> paramVec;
    func_->ObjectToVector(param, paramVec);
    auto ret = g_component->SetParameter(OMX_IndexVideoStartUnused, paramVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_2200
* @tc.name   : HdfCodecHdiSetParameterTest006
* @tc.desc   : Verify the SetParameter function when the input parameter is valid.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetParameterTest006, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam pixFormat;
    func_->InitExtParam(pixFormat);
    pixFormat.portIndex = inputIndex;
    pixFormat.codecColorIndex = 0;
    std::vector<int8_t> inParam;
    func_->ObjectToVector(pixFormat, inParam);

    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);

    pixFormat.codecColorFormat = PIXEL_FMT_YCBCR_420_SP;
    std::vector<int8_t> paramVec;
    func_->ObjectToVector(pixFormat, paramVec);
    ret = g_component->SetParameter(OMX_IndexCodecVideoPortFormat, paramVec);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

#ifdef SUPPORT_DMA_BUFFER
/**
* @tc.number : SUB_Driver_Codec_idlomx_2300
* @tc.name   : HdfCodecHdiDMABufferTest001
* @tc.desc   : Verify the codec support DMA buffer process.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiDMABufferTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    SupportBufferType bufferType;
    func_->InitExtParam(bufferType);
    bufferType.portIndex = outputIndex;
    std::vector<int8_t> inParam, outParam;
    func_->ObjectToVector(bufferType, inParam);
    auto ret = g_component->GetParameter(OMX_IndexParamSupportBufferType, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
    func_->VectorToObject(outParam, bufferType);
    ASSERT_TRUE(bufferType.bufferTypes & CODEC_BUFFER_TYPE_DMA_MEM_FD) ;
}
#endif

#ifdef SUPPORT_OMX_EXTEND
/**
* @tc.number : SUB_Driver_Codec_idlomx_2400
* @tc.name   : HdfCodecHdiUseBufferAndFreeBufferTest001
* @tc.desc   : Verify Use buffer on input index error when OMX_ErrorInsufficientResources.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiUseBufferAndFreeBufferTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    OMX_PARAM_PORTDEFINITIONTYPE param;
    func_->GetPortParameter(g_component, PortIndex::INDEX_INPUT, param);

    auto err = func_->UseBufferOnPort(g_component, PortIndex::INDEX_INPUT, param.nBufferCountActual,
        param.nBufferSize);
    ASSERT_TRUE(err);
    err = func_->UseBufferOnPort(g_component, PortIndex::INDEX_INPUT, 1, param.nBufferSize);
    ASSERT_FALSE(err);
    err = func_->FreeBufferOnPort(g_component, PortIndex::INDEX_INPUT);
    ASSERT_TRUE(err);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_2500
* @tc.name   : HdfCodecHdiUseBufferAndFreeBufferTest002
* @tc.desc   : Verify Use buffer on output index error when OMX_ErrorInsufficientResources.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiUseBufferAndFreeBufferTest002, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    OMX_PARAM_PORTDEFINITIONTYPE param;
    func_->GetPortParameter(g_component, PortIndex::INDEX_OUTPUT, param);

    auto err = func_->UseBufferOnPort(g_component, PortIndex::INDEX_OUTPUT, param.nBufferCountActual,
        param.nBufferSize);
    ASSERT_TRUE(err);
    err = func_->UseBufferOnPort(g_component, PortIndex::INDEX_OUTPUT, 1, param.nBufferSize);
    ASSERT_FALSE(err);
    err = func_->FreeBufferOnPort(g_component, PortIndex::INDEX_OUTPUT);
    ASSERT_TRUE(err);
}
#endif

/**
* @tc.number : SUB_Driver_Codec_idlomx_2600
* @tc.name   : HdfCodecHdiEmptyAndFillBufferTest001
* @tc.desc   : Verify the encode EmptyAndFillBuffer process.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiEmptyAndFillBufferTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    OMX_PARAM_PORTDEFINITIONTYPE param;
    auto err = func_->InitBufferHandleParameter(g_component, param, inputIndex, CODEC_BUFFER_TYPE_DYNAMIC_HANDLE);
    ASSERT_TRUE(err);
    ret = func_->GetPortParameter(g_component, PortIndex::INDEX_INPUT, param);
    ASSERT_EQ(ret, HDF_SUCCESS);
    err = func_->UseDynaBuffer(g_component, PortIndex::INDEX_INPUT, param.nBufferCountActual, param.nBufferSize);
    ASSERT_TRUE(err);

    ret = func_->GetPortParameter(g_component, PortIndex::INDEX_OUTPUT, param);
    ASSERT_EQ(ret, HDF_SUCCESS);
    err = func_->UseBufferOnPort(g_component, PortIndex::INDEX_OUTPUT, param.nBufferCountActual, param.nBufferSize);
    ASSERT_TRUE(err);

    ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_EXECUTING, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    err = func_->WaitState(g_component, CODEC_STATE_EXECUTING);
    ASSERT_TRUE(err);
    err = func_->FillAndEmptyAllBuffer(g_component, CODEC_BUFFER_TYPE_DYNAMIC_HANDLE);
    ASSERT_TRUE(err);

    ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    err = func_->WaitState(g_component, CODEC_STATE_IDLE);
    ASSERT_TRUE(err);
    ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_LOADED, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    err = func_->WaitState(g_component, CODEC_STATE_IDLE);
    ASSERT_TRUE(err);

    err = func_->FreeBufferOnPort(g_component, PortIndex::INDEX_INPUT);
    ASSERT_TRUE(err);
    err = func_->FreeBufferOnPort(g_component, PortIndex::INDEX_OUTPUT);
    ASSERT_TRUE(err);
    err = func_->WaitState(g_component, CODEC_STATE_LOADED);
    ASSERT_TRUE(err);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_2700
* @tc.name   : HdfCodecHdiFreeBufferTest001
* @tc.desc   : Verify the encode Release output buffer.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiFreeBufferTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer omxBuffer;
    func_->InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.bufferId = BUFFER_ID_ERROR;
    auto ret = g_component->FreeBuffer(outputIndex, omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_2800
* @tc.name   : HdfCodecHdiFreeBufferTest002
* @tc.desc   : Verify the encode Release input buffer.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiFreeBufferTest002, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer omxBuffer;
    func_->InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.bufferId = BUFFER_ID_ERROR;
    auto ret = g_component->FreeBuffer(inputIndex, omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}

#ifdef SUPPORT_OMX_EXTEND
/**
* @tc.number : SUB_Driver_Codec_idlomx_2900
* @tc.name   : HdfCodecHdiDeInitTest001
* @tc.desc   : Verify When ComponentDeInit, must change to Loaded State.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiDeInitTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    auto ret = g_component->ComponentDeInit();
    ASSERT_EQ(ret, HDF_SUCCESS);
}
#endif

#ifdef SUPPORT_HIGH_WORK_FREQUENCY
/**
* @tc.number : SUB_Driver_Codec_idlomx_3000
* @tc.name   : HdfCodecHdiHighWorkingFrequencyTest001
* @tc.desc   : Verify the encode support HIGH_WORK_FREQUENCY process.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiHighWorkingFrequencyTest001, TestSize.Level1)
{
    const std::string processName = "cast_engine_service";
    std::vector<int8_t> paramVec;

    ASSERT_TRUE(g_component != nullptr);

    ProcessNameParam nameParam;
    func_->InitExtParam(nameParam);
    int32_t ret = strcpy_s(nameParam.processName, sizeof(nameParam.processName), processName.c_str());
    ASSERT_TRUE(ret == EOK);
    func_->ObjectToVector(nameParam, paramVec);
    ret = g_component->SetParameter(OMX_IndexParamProcessName, paramVec);
    ASSERT_TRUE(ret == HDF_SUCCESS);

    WorkingFrequencyParam freqParam;
    std::vector<int8_t> inParam;
    std::vector<int8_t> outParam;

    func_->InitExtParam(freqParam);
    func_->ObjectToVector(freqParam, inParam);
    ret = g_component->GetParameter(OMX_IndexParamWorkingFrequency, inParam, outParam);
    ASSERT_TRUE(ret == HDF_SUCCESS);
    func_->VectorToObject(outParam, freqParam);

    // 设置为最高档
    freqParam.level = freqParam.level - 1;
    func_->ObjectToVector(freqParam, inParam);
    ret = g_component->SetParameter(OMX_IndexParamWorkingFrequency, inParam);
    ASSERT_TRUE(ret == HDF_SUCCESS);
}
#endif

/**
* @tc.number : SUB_Driver_Codec_idlomx_3100
* @tc.name   : HdfCodecHdiGetConfigTest001
* @tc.desc   : Verify Set parameters is outputindex  under the structure of OMX_VIDEO_CONFIG_BITRATETYPE.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetConfigTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    func_->InitParam(param);
    param.nPortIndex = outputIndex;

    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetConfig(OMX_IndexConfigVideoBitrate, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_3200
* @tc.name   : HdfCodecHdiGetConfigTest002
* @tc.desc   : Verify Set parameters is inputIndex under the structure of OMX_VIDEO_CONFIG_BITRATETYPE.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetConfigTest002, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    func_->InitParam(param);
    param.nPortIndex = inputIndex;

    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetConfig(OMX_IndexConfigVideoBitrate, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_3300
* @tc.name   : HdfCodecHdiGetConfigTest003
* @tc.desc   : Verify param not initialized and not set structure.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetConfigTest003, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> inParam;
    std::vector<int8_t> outParam;
    auto ret = g_component->GetConfig(OMX_IndexConfigVideoBitrate, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_3400
* @tc.name   : HdfCodecHdiGetConfigTest004
* @tc.desc   : Verify that the structure does not match the index.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiGetConfigTest004, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    func_->InitParam(param);
    param.nPortIndex = outputIndex;

    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetConfig(OMX_IndexVideoStartUnused, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_3500
* @tc.name   : HdfCodecHdiSetConfigTest001
* @tc.desc   : Verify param is nPortIndex is outputIndex and nEncodeBitrate is FRAMERATE.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetConfigTest001, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    func_->InitParam(param);
    param.nPortIndex = outputIndex;
    param.nEncodeBitrate = FRAMERATE;

    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexConfigVideoBitrate, inParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_3600
* @tc.name   : HdfCodecHdiSetConfigTest002
* @tc.desc   : Verify param is nPortIndex is inputIndex and nEncodeBitrate is FRAMERATE.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetConfigTest002, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    func_->InitParam(param);
    param.nPortIndex = inputIndex;
    param.nEncodeBitrate = FRAMERATE;

    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexConfigVideoBitrate, inParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_3700
* @tc.name   : HdfCodecHdiSetConfigTest003
* @tc.desc   : Verify param not initialized and not set structure.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetConfigTest003, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> inParam;
    auto ret = g_component->SetConfig(OMX_IndexConfigVideoBitrate, inParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
* @tc.number : SUB_Driver_Codec_idlomx_3800
* @tc.name   : HdfCodecHdiSetConfigTest004
* @tc.desc   : Verify that the structure does not match the index.
  @tc.type: FUNC
*/
HWTEST_F(CodecHdiOmxEncTest, HdfCodecHdiSetConfigTest004, TestSize.Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    func_->InitParam(param);
    param.nPortIndex = outputIndex;

    std::vector<int8_t> inParam;
    func_->ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexVideoStartUnused, inParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
}  // namespace
