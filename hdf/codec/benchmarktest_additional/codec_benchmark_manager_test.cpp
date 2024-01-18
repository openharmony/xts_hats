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
#include "codec_omx_ext.h"
#include "v2_0/codec_callback_service.h"
#include "v1_0/display_buffer_type.h"
#include "v1_0/display_composer_type.h"
#include "v2_0/icodec_callback.h"
#include "v2_0/icodec_component.h"
#include "v2_0/icodec_component_manager.h"
#include "v1_0/include/idisplay_buffer.h"
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <OMX_Video.h>
#include <OMX_VideoExt.h>
#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include <hdf_log.h>
#include <securec.h>
#include <servmgr_hdi.h>
#include <vector>

using namespace std;
using namespace testing::ext;
using OHOS::sptr;
using namespace OHOS::HDI::Codec::V2_0;
namespace {
constexpr CodecType TYPE = CodecType::VIDEO_ENCODER;
constexpr AvCodecRole ROLE = MEDIA_ROLETYPE_VIDEO_AVC;
static sptr<ICodecComponent> g_component = nullptr;
static sptr<ICodecCallback> g_callback = nullptr;
static sptr<ICodecComponentManager> g_manager = nullptr;
static CodecVersionType g_version;
static std::string g_compName = "";
uint32_t g_componentId = 0;
enum class PortIndex { PORT_INDEX_INPUT = 0, PORT_INDEX_OUTPUT = 1 };
const static uint32_t INPUT_INDEX = static_cast<uint32_t>(PortIndex::PORT_INDEX_INPUT);
const static uint32_t OUTPUT_INDEX = static_cast<uint32_t>(PortIndex::PORT_INDEX_OUTPUT);
constexpr int32_t WIDTH = 640;
constexpr int32_t HEIGHT = 480;
constexpr int32_t BUFFER_SIZE = WIDTH * HEIGHT * 3;
constexpr int FD_DEFAULT = -1;
constexpr uint32_t BUFFER_ID_ERROR = 65000;

template <typename T>
void InitParam(T &param)
{
    memset_s(&param, sizeof(param), 0x0, sizeof(param));
    param.size = sizeof(param);
    param.version.nVersion = 1;
}

template <typename T>
void ObjectToVector(T &param, std::vector<int8_t> &vec)
{
    int8_t *paramPointer = reinterpret_cast<int8_t *>(&param);
    vec.insert(vec.end(), paramPointer, paramPointer + sizeof(param));
}

class CodecBenchmarkManagerTestAdditional : public testing::Test {
public:
    void InitOmxCodecBuffer(OmxCodecBuffer &buffer, CodecBufferType type)
    {
        buffer.bufferType = type;
        buffer.fenceFd = -1;
        buffer.version = g_version;
        buffer.allocLen = BUFFER_SIZE;
        buffer.fd = FD_DEFAULT;
        buffer.bufferhandle = nullptr;
        buffer.pts = 0;
        buffer.flag = 0;
        buffer.size = sizeof(OmxCodecBuffer);
        buffer.type = READ_ONLY_TYPE;
    }
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
    static void TearDownTestCase() { g_manager = nullptr; }
    void SetUp()
    {
        if (g_manager == nullptr) {
            return;
        }
        g_callback = new CodecCallbackService();
        if (g_callback == nullptr) {
            return;
        }
        if (g_compName.empty()) {
            return;
        }

        auto ret = g_manager->CreateComponent(g_component, g_componentId, g_compName.data(),
                                              reinterpret_cast<int64_t>(this), g_callback);
        if (ret != HDF_SUCCESS) {
            return;
        }
        struct CompVerInfo verInfo;
        ret = g_component->GetComponentVersion(verInfo);
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
        std::vector<int8_t> cmdData;
        if (g_component != nullptr) {
            g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_LOADED, cmdData);
        }
        g_callback = nullptr;
        g_component = nullptr;
    }
};

/**
* @tc.number : SUB_Driver_Codec_GetParameter_0200
* @tc.name   : testGetParameter001
* @tc.desc   : when the index = OMX_IndexCodecVideoPortFormat, structure inParamStruct> portIndex = INPUT_INDEX,
                If inParamStruct->codecColorIndex = 0, the command is successfully sent.
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetParameter001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    param.codecColorIndex = 0;
    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetParameter_0300
* @tc.name   : testGetParameter002
* @tc.desc   : When the index = OMX_IndexParamVideoQuantization, structure inParamStruct->portIndex = INPUT_INDEX,
                inParamStruct->codecColorIndex = 0, Failed to send the command.
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetParameter002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    param.codecColorIndex = 0;
    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexParamVideoQuantization, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetParameter_0400
* @tc.name   : testGetParameter003
* @tc.desc   : When index=OMX_IndexParamVideoFastUpdate, Structure inParamStruct->portIndex=INPUT_INDEX,
                inParamStruct->codecColorIndex=0, sending command failed.
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetParameter003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    param.codecColorIndex = 0;
    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexParamVideoFastUpdate, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetParameter_0500
* @tc.name   : testGetParameter004
* @tc.desc   : When index = OMX_IndexParamVideoBitrate, structure inParamStruct->portIndex = INPUT_INDEX,
                inParamStruct->codecColorIndex = 0,Failed to send command.
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetParameter004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    param.codecColorIndex = 0;
    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexParamVideoBitrate, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetParameter_0600
* @tc.name   : testGetParameter005
* @tc.desc   : When the index = OMX_IndexCodecVideoPortFormat, structure inParamStruct->portIndex = INPUT_INDEX,
                inParamStruct->codecColorIndex = 0,Send command successful.
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetParameter005, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetParameter_0700
* @tc.name   : testGetParameter006
* @tc.desc   : When the index = OMX_IndexCodecVideoPortFormat, structure inParamStruct->portIndex = OUTPUT_INDEX,
                not initialize,  send the command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetParameter006, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    param.portIndex = OUTPUT_INDEX;
    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetParameter_0800
* @tc.name   : testGetParameter007
* @tc.desc   : When the index = OMX_IndexParamVideoIntraRefresh,structure inParamStruct not initialize,
                send the command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetParameter007, Function | MediumTest | Level2)
{
    int32_t ret;
    std::vector<int8_t> inParam;
    std::vector<int8_t> outParam;
    ret = g_component->GetParameter(OMX_IndexParamVideoIntraRefresh, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetParameter_0900
* @tc.name   : testGetParameter008
* @tc.desc   : When the index = OMX_IndexCodecVideoPortFormat, structure inParamStruct->portIndex = OUTPUT_INDEX,
                send the command succeeds
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetParameter008, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;
    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetParameter(OMX_IndexCodecVideoPortFormat, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetParameter_0200
* @tc.name   : testSetParameter001
* @tc.desc   : When the index = OMX_IndexParamVideoPortFormat paramStruct. PortIndex = INPUT_INDEX,
                send the command succeeds
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetParameter001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret;
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    std::vector<int8_t> paramVec;
    ObjectToVector(param, paramVec);
    ret = g_component->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetParameter_0300
* @tc.name   : testSetParameter002
* @tc.desc   : When the index = OMX_IndexParamVideoPortFormat paramStruct. Size = 0,
                send the command failure.
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetParameter002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret;
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    param.size = 0;
    std::vector<int8_t> paramVec;
    ObjectToVector(param, paramVec);
    ret = g_component->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_SetParameter_0400
 * @tc.name   : testSetParameter003
 * @tc.desc   : When paramStruct. CodecColorIndex = 0, send the command succeeds.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetParameter003, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret;
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    param.codecColorIndex = 0;
    std::vector<int8_t> paramVec;
    ObjectToVector(param, paramVec);
    ret = g_component->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_SetParameter_0500
 * @tc.name   : testSetParameter004
 * @tc.desc   : When paramStruct->CodecColorFormat = 20, send the command succeeds.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetParameter004, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret;
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    param.codecColorFormat = 20;
    std::vector<int8_t> paramVec;
    ObjectToVector(param, paramVec);
    ret = g_component->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetParameter_0600
* @tc.name   : testSetParameter005
* @tc.desc   : When the index = OMX_IndexParamVideoPortFormat, paramStruct->CodecColorFormat = 0,
                send the command succeeds.
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetParameter005, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret;
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    param.codecColorFormat = 0;
    std::vector<int8_t> paramVec;
    ObjectToVector(param, paramVec);
    ret = g_component->SetParameter(OMX_IndexParamVideoPortFormat, paramVec);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_SetParameter_0700
 * @tc.name   : testSetParameter006
 * @tc.desc   : When the index = OMX_IndexParamVideoQuantization, send the command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetParameter006, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    int32_t ret;
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;
    std::vector<int8_t> paramVec;
    ObjectToVector(param, paramVec);
    ret = g_component->SetParameter(OMX_IndexParamVideoQuantization, paramVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetConfig_0200
* @tc.name   : testGetConfig001
* @tc.desc   : If index = OMX_IndexConfigVideoBitrate and structure inParamStruct->portIndex = OUTPUT_INDEX,
                the command is successfully sent
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetConfig001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetConfig(OMX_IndexConfigVideoBitrate, inParam, outParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetConfig_0300
* @tc.name   : testGetConfig002
* @tc.desc   : When the index = OMX_IndexParamVideoPortFormat, structure inParamStruct->portIndex = OUTPUT_INDEX,
                send the command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetConfig002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetConfig(OMX_IndexParamVideoPortFormat, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetConfig_0400
* @tc.name   : testGetConfig003
* @tc.desc   : When index=OMX_IndexParamVideoBitrate, structure inParamStruct->portIndex=OUTPUT_INDEX,
                sending command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetConfig003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetConfig(OMX_IndexParamVideoBitrate, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetConfig_0500
* @tc.name   : testGetConfig004
* @tc.desc   : When index=OMX_IndexParamVideoFastUpdate, Structure inParamStruct->portIndex=OUTPUT_INDEX,
                sending command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetConfig004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetConfig(OMX_IndexParamVideoFastUpdate, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_GetConfig_0600
* @tc.name   : testGetConfig005
* @tc.desc   : When index=OMX_IndexParamVideoQuantization, structure inParamStruct->portIndex=OUTPUT_INDEX,
                sending command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetConfig005, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    std::vector<int8_t> outParam;
    auto ret = g_component->GetConfig(OMX_IndexParamVideoQuantization, inParam, outParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetConfig_0200
* @tc.name   : testSetConfig001
* @tc.desc   : When index=OMX_IndexConfigVideoBitrate, structure inParamStruct->portIndex=OUTPUT_INDEX,
                command sent successfully
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetConfig001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexConfigVideoBitrate, inParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetConfig_0300
* @tc.name   : testSetConfig002
* @tc.desc   : When index=OMX_IndexConfigVideoBitrate, structure inParamStruct->portIndex=INPUT_INDEX,
                sending command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetConfig002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = INPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexConfigVideoBitrate, inParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetConfig_0400
* @tc.name   : testSetConfig003
* @tc.desc   : When index=OMX_IndexParamVideoQuantization, structure inParamStruct->portIndex=OUTPUT_INDEX,
                sending command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetConfig003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexParamVideoQuantization, inParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetConfig_0500
* @tc.name   : testSetConfig004
* @tc.desc   : When index=OMX_IndexVideoStartUnused, Structure inParamStruct->portIndex=OUTPUT_INDEX,
                sending command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetConfig004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexVideoStartUnused, inParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetConfig_0600
* @tc.name   : testSetConfig005
* @tc.desc   : When index=OMX_IndexCodecVideoPortFormat, Structure inParamStruct->portIndex=OUTPUT_INDEX,
                sending command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetConfig005, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexCodecVideoPortFormat, inParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetConfig_0700
* @tc.name   : testSetConfig006
* @tc.desc   : When index=OMX_IndexParamVideoFastUpdate, Structure inParamStruct->portIndex=OUTPUT_INDEX,
                sending command failed
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetConfig006, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexParamVideoFastUpdate, inParam);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.number : SUB_Driver_Codec_SetConfig_0800
* @tc.name   : testSetConfig007
* @tc.desc   : When index=OMX_IndexConfigVideoBitrate, structure inParamStruct->codecColorIndex=0,
                command sent successfully
*/
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetConfig007, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;
    param.codecColorIndex = 0;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexConfigVideoBitrate, inParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_SetConfig_0900
 * @tc.name   : testSetConfig008
 * @tc.desc   : When index=OMX_IndexConfigVideoBitrate, structure inParamStruct->size=0, command sent successfully
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetConfig008, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;
    param.size = 0;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexConfigVideoBitrate, inParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_SetConfig_1000
 * @tc.name   : testSetConfig009
 * @tc.desc   : When index=OMX_IndexConfigVideoBitrate, structure inParamStruct->codecColorFormat=0,
 *              command sent successfully
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetConfig009, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecVideoPortFormatParam param;
    InitParam(param);
    param.portIndex = OUTPUT_INDEX;
    param.codecColorFormat = 0;

    std::vector<int8_t> inParam;
    ObjectToVector(param, inParam);
    auto ret = g_component->SetConfig(OMX_IndexConfigVideoBitrate, inParam);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_0200
 * @tc.name   : testGetExtensionIndex001
 * @tc.desc   : When the paramName is "OMX.Topaz.index.param.extended_video" and the indexType = 0,
 *              sending the command is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("OMX.Topaz.index.param.extended_video", indexType);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_0300
 * @tc.name   : testGetExtensionIndex002
 * @tc.desc   : Sending command failed when paramName is "" and indexType = 0.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_0400
 * @tc.name   : testGetExtensionIndex003
 * @tc.desc   : When the paramName is "ＯＭＸ．Ｔｏｐａｚ．ｉｎｄｅｘ．ｐａｒａｍ．ｅｘｔｅｎｄｅｄ＿ｖｉｄｅｏ" and the
 * indexType = 0 Verify full width letters and symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex(
        "ＯＭＸ．Ｔｏｐａｚ．ｉｎｄｅｘ．ｐａｒａｍ．ｅｘｔｅｎｄｅｄ＿ｖｉｄｅｏ", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_0500
 * @tc.name   : testGetExtensionIndex004
 * @tc.desc   : When the paramName is "!@#$%^&**" and the indexType = 0 Verify half width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("!@#$%^&**", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_0600
 * @tc.name   : testGetExtensionIndex005
 * @tc.desc   : When the paramName is "！＠＃＄％＾＆＊＊" and the indexType = 0 Verify full width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex005, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("！＠＃＄％＾＆＊＊", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_0700
 * @tc.name   : testGetExtensionIndex006
 * @tc.desc   : When the paramName is "12345" and the indexType = 0 Verify Half width Numbers
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex006, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("12345", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_0800
 * @tc.name   : testGetExtensionIndex007
 * @tc.desc   : When the paramName is "12345MX.Topaz" and the indexType = 0 Verify half width numbers,
 * half width letters, and half width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex007, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("12345MX.Topaz", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_0900
 * @tc.name   : testGetExtensionIndex008
 * @tc.desc   : When the paramName is "!@#$%^&**OMX.Topaz" and the indexType = 0 Verify half width symbols and
 * half width letters
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex008, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("!@#$%^&**OMX.Topaz", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1000
 * @tc.name   : testGetExtensionIndex009
 * @tc.desc   : When the paramName is "１２３４５" and the indexType = 0 Verify full angle digits
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex009, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("１２３４５", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1100
 * @tc.name   : testGetExtensionIndex010
 * @tc.desc   : When the paramName is "１２３４５ＯＭＸ．Ｔｏｐａｚ" and the indexType = 0 Verify full width digits,
 * full width letters, and full width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex010, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("１２３４５ＯＭＸ．Ｔｏｐａｚ", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1200
 * @tc.name   : testGetExtensionIndex011
 * @tc.desc   : When the paramName is "１２３４５!@#$%" and the indexType = 0 Verify full width digits and
 * half width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex011, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("１２３４５Ｏ!@#$%", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1300
 * @tc.name   : testGetExtensionIndex012
 * @tc.desc   : When the paramName is "１２３４５！＠＃＄" and the indexType = 0 Verify full width digits, full width
 * symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex012, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("１２３４５！＠＃＄", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1400
 * @tc.name   : testGetExtensionIndex013
 * @tc.desc   : When the paramName is "１２３４５OMX.Topaz" and the indexType = 0 Verify full width numbers,
 * half width letters, and half width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex013, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("１２３４５OMX.Topaz", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1500
 * @tc.name   : testGetExtensionIndex014
 * @tc.desc   : When the paramName is "！＠＃＄％OMX.Topaz" and the indexType = 0 Verify full width symbols,
 * half width letters, and half width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex014, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("！＠＃＄％OMX.Topaz", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1600
 * @tc.name   : testGetExtensionIndex015
 * @tc.desc   : When the paramName is "！＠＃＄％ＯＭＸ．Ｔｏｐａｚ" and the indexType = 0 Verify full width symbols and
 * letters
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex015, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("！＠＃＄％ＯＭＸ．Ｔｏｐａｚ", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1700
 * @tc.name   : testGetExtensionIndex016
 * @tc.desc   : When the paramName is "12345ＯＭＸ．Ｔｏｐａｚ" and the indexType = 0 Verify half width numbers,
 * full width letters, and full width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex016, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("12345ＯＭＸ．Ｔｏｐａｚ", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1800
 * @tc.name   : testGetExtensionIndex017
 * @tc.desc   : When the paramName is "12345!@#$%" and the indexType = 0 Verify half width numbers
 * and half width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex017, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("12345!@#$%", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetExtensionIndex_1900
 * @tc.name   : testGetExtensionIndex018
 * @tc.desc   : When the paramName is "12345！＠＃＄％" and the indexType = 0 Verify half width numbers
 * and full width symbols
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetExtensionIndex018, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    uint32_t indexType = 0;
    auto ret = g_component->GetExtensionIndex("12345！＠＃＄％", indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_GetState_0200
 * @tc.name   : testGetState001
 * @tc.desc   : When the initial value of CodecStateType is CODEC_STATE_INVALID, does the function modify it.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetState001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    CodecStateType state = CODEC_STATE_INVALID;
    int32_t ret = g_component->GetState(state);
    ASSERT_EQ(state, CODEC_STATE_LOADED);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_0200
 * @tc.name   : testComponentTunnelRequest001
 * @tc.desc   : When setting the component status to OMX_StateInvalid, input port=OUTPUT_INDEX, tunneledComp=1,
 *              tunneledPort=1,inTunnelSetup=CODEC_BUFFER_SUPPLY_INPUT, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest001, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateInvalid, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1;
    const uint32_t tunneledPort = 1;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_INPUT;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_0300
 * @tc.name   : testComponentTunnelRequest002
 * @tc.desc   : When setting the component status to OMX_StateIdle, input port=INPUT_INDEX, tunneledComp=1002,
 *              tunneledPort=101,inTunnelSetup=CODEC_BUFFER_SUPPLY_INPUT,Sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateIdle, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_INPUT;

    ret = g_component->ComponentTunnelRequest(INPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_0400
 * @tc.name   : testComponentTunnelRequest003
 * @tc.desc   : When setting the component status to OMX_ tatePause, input port=INPUT_INDEX, tunneledComp=1002,
 *              tunneledPort=101,inTunnelSetup=CODEC_BUFFER_SUPPLY_OUTPUT, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StatePause, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_OUTPUT;

    ret = g_component->ComponentTunnelRequest(INPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_0500
 * @tc.name   : testComponentTunnelRequest004
 * @tc.desc   : When setting the component status to OMX_StateWaitForResources, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_OUTPUT, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateWaitForResources, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_OUTPUT;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_0600
 * @tc.name   : testComponentTunnelRequest005
 * @tc.desc   : When setting the component status to OMX_StateLoaded, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_OUTPUT, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest005, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_OUTPUT;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_0700
 * @tc.name   : testComponentTunnelRequest006
 * @tc.desc   : When setting the component status to OMX_StateExecuting, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_OUTPUT, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest006, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateExecuting, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_OUTPUT;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_0800
 * @tc.name   : testComponentTunnelRequest007
 * @tc.desc   : When setting the component status to OMX_StateKhronosExtensions, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_OUTPUT, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest007, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateKhronosExtensions, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_OUTPUT;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_0900
 * @tc.name   : testComponentTunnelRequest008
 * @tc.desc   : When setting the component status to OMX_StateVendorStartUnused, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_OUTPUT, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest008, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateVendorStartUnused, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_OUTPUT;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_1000
 * @tc.name   : testComponentTunnelRequest009
 * @tc.desc   : When setting the component status to OMX_StateMax, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_OUTPUT, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest009, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateMax, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_OUTPUT;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_1100
 * @tc.name   : testComponentTunnelRequest010
 * @tc.desc   : When setting the component status to OMX_StateMax, input port=INPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_UNSPECIFIED, sending command
 * failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest010, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateMax, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_UNSPECIFIED;

    ret = g_component->ComponentTunnelRequest(INPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_1200
 * @tc.name   : testComponentTunnelRequest011
 * @tc.desc   : When setting the component status to OMX_StateMax, input port=INPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_KHRONOS_EXTENSIONS, sending
 * command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest011, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateMax, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_KHRONOS_EXTENSIONS;

    ret = g_component->ComponentTunnelRequest(INPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_1300
 * @tc.name   : testComponentTunnelRequest012
 * @tc.desc   : When setting the component status to OMX_StateMax, input port=INPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_VENDOR_START_UNUSED, sending
 * command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest012, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateMax, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_VENDOR_START_UNUSED;

    ret = g_component->ComponentTunnelRequest(INPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_1400
 * @tc.name   : testComponentTunnelRequest013
 * @tc.desc   : When setting the component status to OMX_StateMax, input port=INPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_MAX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest013, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateMax, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_MAX;

    ret = g_component->ComponentTunnelRequest(INPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_1500
 * @tc.name   : testComponentTunnelRequest014
 * @tc.desc   : When setting the component status to OMX_StateMax, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_UNSPECIFIED, sending command
 * failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest014, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateMax, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_UNSPECIFIED;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_1600
 * @tc.name   : testComponentTunnelRequest015
 * @tc.desc   : When setting the component status to OMX_StateMax, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_KHRONOS_EXTENSIONS, sending
 * command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest015, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateMax, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_KHRONOS_EXTENSIONS;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_1700
 * @tc.name   : testComponentTunnelRequest016
 * @tc.desc   : When setting the component status to OMX_StateMax, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_VENDOR_START_UNUSED, sending
 * command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest016, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateMax, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_VENDOR_START_UNUSED;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentTunnelRequest_1800
 * @tc.name   : testComponentTunnelRequest017
 * @tc.desc   : When setting the component status to OMX_StateMax, input port=OUTPUT_INDEX,
 *              tunneledComp=1002,tunneledPort=101, inTunnelSetup=CODEC_BUFFER_SUPPLY_MAX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentTunnelRequest017, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateMax, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OHOS::HDI::Codec::V2_0::CodecTunnelSetupType tunnelSetup;
    tunnelSetup.supplier = OHOS::HDI::Codec::V2_0::CODEC_BUFFER_SUPPLY_MAX;

    ret = g_component->ComponentTunnelRequest(OUTPUT_INDEX, tunneledComp, tunneledPort, tunnelSetup, tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseBuffer_0200
 * @tc.name   : testUseBuffer001
 * @tc.desc   : When setting the component status to CODEC_State_IDLE, portIndex=INPUT_INDEX,command sent successfully
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseBuffer001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    InitOmxCodecBuffer(*omxBuffer.get(), CODEC_BUFFER_TYPE_DYNAMIC_HANDLE);

    OmxCodecBuffer outBuffer;
    ret = g_component->UseBuffer(INPUT_INDEX, *omxBuffer.get(), outBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseBuffer_0300
 * @tc.name   : testUseBuffer002
 * @tc.desc   : When setting the component status to CODEC_State_MAX, portIndex=INPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseBuffer002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_MAX, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    InitOmxCodecBuffer(*omxBuffer.get(), CODEC_BUFFER_TYPE_HANDLE);

    OmxCodecBuffer outBuffer;
    ret = g_component->UseBuffer(INPUT_INDEX, *omxBuffer.get(), outBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseBuffer_0400
 * @tc.name   : testUseBuffer003
 * @tc.desc   : When setting the component status to CODEC_STATE_INVALID, portIndex=INPUT_INDEX,sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseBuffer003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_INVALID, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    InitOmxCodecBuffer(*omxBuffer.get(), CODEC_BUFFER_TYPE_HANDLE);

    OmxCodecBuffer outBuffer;
    ret = g_component->UseBuffer(INPUT_INDEX, *omxBuffer.get(), outBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseBuffer_0500
 * @tc.name   : testUseBuffer004
 * @tc.desc   : setting the component status to CODEC_STATE_LOADED, portIndex=INPUT_INDEX,sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseBuffer004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_LOADED, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    InitOmxCodecBuffer(*omxBuffer.get(), CODEC_BUFFER_TYPE_HANDLE);

    OmxCodecBuffer outBuffer;
    ret = g_component->UseBuffer(INPUT_INDEX, *omxBuffer.get(), outBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseBuffer_0600
 * @tc.name   : testUseBuffer005
 * @tc.desc   : setting the component status to CODEC_STATE_EXECUTING, portIndex=INPUT_INDEX,sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseBuffer005, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_EXECUTING, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    InitOmxCodecBuffer(*omxBuffer.get(), CODEC_BUFFER_TYPE_HANDLE);

    OmxCodecBuffer outBuffer;
    ret = g_component->UseBuffer(INPUT_INDEX, *omxBuffer.get(), outBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseBuffer_0700
 * @tc.name   : testUseBuffer006
 * @tc.desc   : setting the component status to CODEC_STATE_PAUSE, portIndex=INPUT_INDEX,sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseBuffer006, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_PAUSE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    InitOmxCodecBuffer(*omxBuffer.get(), CODEC_BUFFER_TYPE_HANDLE);

    OmxCodecBuffer outBuffer;
    ret = g_component->UseBuffer(INPUT_INDEX, *omxBuffer.get(), outBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseBuffer_0800
 * @tc.name   : testUseBuffer007
 * @tc.desc   : setting the component status to CODEC_STATE_VENDOR_START_UNUSED, portIndex=INPUT_INDEX,
 *              sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseBuffer007, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_VENDOR_START_UNUSED, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    InitOmxCodecBuffer(*omxBuffer.get(), CODEC_BUFFER_TYPE_HANDLE);

    OmxCodecBuffer outBuffer;
    ret = g_component->UseBuffer(INPUT_INDEX, *omxBuffer.get(), outBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseBuffer_0900
 * @tc.name   : testUseBuffer008
 * @tc.desc   : setting the component status to CODEC_STATE_WAIT_FOR_RESOURCES, portIndex=INPUT_INDEX,
 *              sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseBuffer008, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_WAIT_FOR_RESOURCES, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    InitOmxCodecBuffer(*omxBuffer.get(), CODEC_BUFFER_TYPE_HANDLE);

    OmxCodecBuffer outBuffer;
    ret = g_component->UseBuffer(INPUT_INDEX, *omxBuffer.get(), outBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseBuffer_1000
 * @tc.name   : testUseBuffer009
 * @tc.desc   : setting the component status to CODEC_STATE_IDLE, portIndex=OUTPUT_INDEX,sending command Success.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseBuffer009, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    InitOmxCodecBuffer(*omxBuffer.get(), CODEC_BUFFER_TYPE_DYNAMIC_HANDLE);

    OmxCodecBuffer outBuffer;
    ret = g_component->UseBuffer(OUTPUT_INDEX, *omxBuffer.get(), outBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_AllocateBuffer_0200
 * @tc.name   : testAllocateBuffer001
 * @tc.desc   : Set component status to CODEC_State_IDLE, portIndex=INPUT_INDEX,command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testAllocateBuffer001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_AllocateBuffer_0300
 * @tc.name   : testAllocateBuffer002
 * @tc.desc   : Set component status to CODEC_State_IDLE, portIndex=INPUT_INDEX,
 *              The structure OmxCodecBuffer->bufferId=0,command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testAllocateBuffer002, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    allocBuffer.bufferId = 0;
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_AllocateBuffer_0400
 * @tc.name   : testAllocateBuffer003
 * @tc.desc   : Set component status to CODEC_STATE_LOADED, portIndex=INPUT_INDEX,command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testAllocateBuffer003, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_LOADED, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_AllocateBuffer_0500
 * @tc.name   : testAllocateBuffer004
 * @tc.desc   : Set component status to CODEC_STATE_IDLE, portIndex=OUTPUT_INDEX,command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testAllocateBuffer004, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(OUTPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    err = g_component->FreeBuffer(OUTPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_0200
 * @tc.name   : testFreeBuffer001
 * @tc.desc   : When the function is called directly for release, sending the command fails.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer001, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto ret = g_component->FreeBuffer(OUTPUT_INDEX, omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_0300
 * @tc.name   : testFreeBuffer002
 * @tc.desc   : Set the component status to OMX first_ StateLoaded, call the AllocateBuffer function to apply for space,
 *              then call the FreeBuffer function,send the command successfully.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer002, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_0400
 * @tc.name   : testFreeBuffer003
 * @tc.desc   : Structure OmxCodecBuffer->bufferId = BUFFER_ID_ERROR, sending command failed
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    outBuffer.bufferId = BUFFER_ID_ERROR;
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_NE(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_0500
 * @tc.name   : testFreeBuffer004
 * @tc.desc   : Structure OmxCodecBuffer->size = 0, sending command success
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer004, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    outBuffer.size = 0;
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_0600
 * @tc.name   : testFreeBuffer005
 * @tc.desc   : Structure OmxCodecBuffer->bufferType=CODEC_BUFFER_TYPE_AVSHARE_MEM_FD,command sent successfully.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer005, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    outBuffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_0700
 * @tc.name   : testFreeBuffer006
 * @tc.desc   : The structure OmxCodecBuffer->allocLen = 0,the command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer006, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    outBuffer.allocLen = 0;
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_0800
 * @tc.name   : testFreeBuffer007
 * @tc.desc   : The structure OmxCodecBuffer->filledLen = 0,the command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer007, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    outBuffer.filledLen = 0;
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_0900
 * @tc.name   : testFreeBuffer008
 * @tc.desc   : The structure OmxCodecBuffer->type = READ_WRITE_TYPE,the command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer008, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    outBuffer.type = READ_WRITE_TYPE;
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_1000
 * @tc.name   : testFreeBuffer009
 * @tc.desc   : The structure OmxCodecBuffer->type = READ_ONLY_TYPE,the command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer009, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    outBuffer.type = READ_ONLY_TYPE;
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FreeBuffer_1100
 * @tc.name   : testFreeBuffer010
 * @tc.desc   : Set the component status to OMX_StateExecuting
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFreeBuffer010, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateExecuting, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);

    struct OmxCodecBuffer allocBuffer;
    InitOmxCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    struct OmxCodecBuffer outBuffer;
    err = g_component->AllocateBuffer(INPUT_INDEX, allocBuffer, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    err = g_component->FreeBuffer(INPUT_INDEX, outBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_EmptyThisBuffer_0200
 * @tc.name   : testEmptyThisBuffer001
 * @tc.desc   : Set the component status to OMX_StateLoaded
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testEmptyThisBuffer001, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto ret = g_component->EmptyThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_EmptyThisBuffer_0300
 * @tc.name   : testEmptyThisBuffer002
 * @tc.desc   : Set component status to OMX_StateExecuting, structure OmxCodecBuffer->bufferId=BUFFER_ID_ERROR
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testEmptyThisBuffer002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateExecuting, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.bufferId = BUFFER_ID_ERROR;
    auto ret = g_component->EmptyThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_EmptyThisBuffer_0400
 * @tc.name   : testEmptyThisBuffer003
 * @tc.desc   : Set component status to OMX_StateIdle
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testEmptyThisBuffer003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateIdle, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto ret = g_component->EmptyThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_EmptyThisBuffer_0500
 * @tc.name   : testEmptyThisBuffer004
 * @tc.desc   : Set component status to OMX_StateInvalid
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testEmptyThisBuffer004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateInvalid, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto ret = g_component->EmptyThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_EmptyThisBuffer_0600
 * @tc.name   : testEmptyThisBuffer005
 * @tc.desc   : Set component status to OMX_StateWaitForResources
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testEmptyThisBuffer005, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateWaitForResources, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto ret = g_component->EmptyThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FillThisBuffer_0200
 * @tc.name   : testFillThisBuffer001
 * @tc.desc   : Set component status to OMX_StatePause,structure OmxCodecBuffer->bufferType=CODEC_BUFFER_TYPE_INVALID
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFillThisBuffer001, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StatePause, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.bufferType = CODEC_BUFFER_TYPE_INVALID;
    auto ret = g_component->FillThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FillThisBuffer_0300
 * @tc.name   : testFillThisBuffer002
 * @tc.desc   : Set component status to OMX_StateExecuting,
 *              structure OmxCodecBuffer->bufferType=CODEC_BUFFER_TYPE_INVALID
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFillThisBuffer002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateExecuting, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.bufferType = CODEC_BUFFER_TYPE_INVALID;
    auto ret = g_component->FillThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FillThisBuffer_0400
 * @tc.name   : testFillThisBuffer003
 * @tc.desc   : Set component status to OMX_StateExecution, structure OmxCodecBuffer->size=0
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFillThisBuffer003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateExecuting, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.size = 0;
    auto ret = g_component->FillThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FillThisBuffer_0500
 * @tc.name   : testFillThisBuffer004
 * @tc.desc   : Set component status to OMX_StateExecution, structure OmxCodecBuffer->bufferId = BUFFER_ID_ERROR
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFillThisBuffer004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateExecuting, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    omxBuffer.bufferId = BUFFER_ID_ERROR;
    auto ret = g_component->FillThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_FillThisBuffer_0600
 * @tc.name   : testFillThisBuffer005
 * @tc.desc   : Set component status to OMX_StateIdle
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testFillThisBuffer005, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    auto err = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateIdle, cmdData);
    ASSERT_EQ(err, HDF_SUCCESS);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto ret = g_component->FillThisBuffer(omxBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentDeInit_0200
 * @tc.name   : testComponentDeInit001
 * @tc.desc   : When directly calling the ComponentDeInit function, the command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentDeInit001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    auto ret = g_component->ComponentDeInit();
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentDeInit_0300
 * @tc.name   : testComponentDeInit002
 * @tc.desc   : Set the condition to OMX_StateLoaded, the command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentDeInit002, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    OMX_STATETYPE status = OMX_StateLoaded;
    std::cout << "status = " << status << std::endl;
    auto ret = g_component->ComponentDeInit();
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentRoleEnum_0200
 * @tc.name   : testComponentRoleEnum001
 * @tc.desc   : When index=0, the command was successfully sent.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentRoleEnum001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<uint8_t> role;
    auto ret = g_component->ComponentRoleEnum(role, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_ComponentRoleEnum_0300
 * @tc.name   : testComponentRoleEnum002
 * @tc.desc   : When index=65000, sending command failed
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testComponentRoleEnum002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<uint8_t> role;
    auto ret = g_component->ComponentRoleEnum(role, 65000);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_0200
 * @tc.name   : testUseEglImage001
 * @tc.desc   : Initialize the structure OmxCodecBuffer with portIndex=INPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage001, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    int32_t ret = g_component->UseEglImage(INPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_0300
 * @tc.name   : testUseEglImage002
 * @tc.desc   : Set component status to OMX_StatePause, portIndex=INPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage002, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer omxBuffer;
    OMX_STATETYPE status = OMX_StatePause;
    std::cout << "status = " << status << std::endl;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    int32_t ret = g_component->UseEglImage(INPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_0400
 * @tc.name   : testUseEglImage003
 * @tc.desc   : Set component status to OMX_StateLoaded, portIndex=INPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer omxBuffer;
    OMX_STATETYPE status = OMX_StateLoaded;
    std::cout << "status = " << status << std::endl;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    int32_t ret = g_component->UseEglImage(INPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_0500
 * @tc.name   : testUseEglImage004
 * @tc.desc   : Set component status to OMX_StateWaitForResources, portIndex=INPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage004, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    struct OmxCodecBuffer omxBuffer;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateWaitForResources, cmdData);
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    ret = g_component->UseEglImage(INPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_0600
 * @tc.name   : testUseEglImage005
 * @tc.desc   : Set component status to OMX_StateExecuting, portIndex=INPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage005, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    struct OmxCodecBuffer omxBuffer;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateExecuting, cmdData);
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    ret = g_component->UseEglImage(INPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_0700
 * @tc.name   : testUseEglImage006
 * @tc.desc   : Set component status to OMX_StateLoaded, with CODEC_BUFFER_TYPE_VIRTUAL_ADDR.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage006, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    struct OmxCodecBuffer omxBuffer;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_VIRTUAL_ADDR);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    ret = g_component->UseEglImage(INPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_0800
 * @tc.name   : testUseEglImage007
 * @tc.desc   : Set component status to OMX_StateExecuting, with CODEC_BUFFER_TYPE_VIRTUAL_ADDR
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage007, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    struct OmxCodecBuffer omxBuffer;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateExecuting, cmdData);
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_VIRTUAL_ADDR);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    ret = g_component->UseEglImage(INPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_0900
 * @tc.name   : testUseEglImage008
 * @tc.desc   : Set component status to OMX_StateWaitForResources, with CODEC_BUFFER_TYPE_HANDLE
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage008, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    struct OmxCodecBuffer omxBuffer;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateWaitForResources, cmdData);
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_HANDLE);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    ret = g_component->UseEglImage(INPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_1000
 * @tc.name   : testUseEglImage009
 * @tc.desc   : Set component status to OMX_StateLoaded, with CODEC_BUFFER_TYPE_DYNAMIC_HANDLE
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage009, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    struct OmxCodecBuffer omxBuffer;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateLoaded, cmdData);
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_DYNAMIC_HANDLE);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    ret = g_component->UseEglImage(INPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_1100
 * @tc.name   : testUseEglImage010
 * @tc.desc   : Initialize the structure OmxCodecBuffer with portIndex=OUTPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage010, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer omxBuffer;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    int32_t ret = g_component->UseEglImage(OUTPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_1200
 * @tc.name   : testUseEglImage011
 * @tc.desc   : Set component status to OMX_StatePause, portIndex=OUTPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage011, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer omxBuffer;
    OMX_STATETYPE status = OMX_StatePause;
    std::cout << "status = " << status << std::endl;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    int32_t ret = g_component->UseEglImage(OUTPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_1300
 * @tc.name   : testUseEglImage012
 * @tc.desc   : Set component status to OMX_StateLoaded, portIndex=OUTPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage012, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    struct OmxCodecBuffer omxBuffer;
    OMX_STATETYPE status = OMX_StateLoaded;
    std::cout << "status = " << status << std::endl;
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    int32_t ret = g_component->UseEglImage(OUTPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_1400
 * @tc.name   : testUseEglImage013
 * @tc.desc   : Set component status to OMX_StateWaitForResources, portIndex=OUTPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage013, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    struct OmxCodecBuffer omxBuffer;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateWaitForResources, cmdData);
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    ret = g_component->UseEglImage(OUTPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_UseEglImage_1500
 * @tc.name   : testUseEglImage014
 * @tc.desc   : Set component status to OMX_StateExecuting, portIndex=OUTPUT_INDEX, sending command failed.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testUseEglImage014, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    struct OmxCodecBuffer omxBuffer;
    auto ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, OMX_StateExecuting, cmdData);
    InitOmxCodecBuffer(omxBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD);
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    std::vector<int8_t> eglImageVec;
    eglImageVec.assign(eglImage.get(), eglImage.get() + BUFFER_SIZE);
    struct OmxCodecBuffer outbuffer;
    ret = g_component->UseEglImage(OUTPUT_INDEX, omxBuffer, outbuffer, eglImageVec);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_SetCallbacks_0200
 * @tc.name   : testSetCallbacks001
 * @tc.desc   : Set the function entry to appData = -1
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetCallbacks001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    g_callback = new CodecCallbackService();
    ASSERT_TRUE(g_callback != nullptr);
    auto ret = g_component->SetCallbacks(g_callback, -1);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_SetCallbacks_0300
 * @tc.name   : testSetCallbacks002
 * @tc.desc   : Set the function entry to appData = 0
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetCallbacks002, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    g_callback = new CodecCallbackService();
    ASSERT_TRUE(g_callback != nullptr);
    auto ret = g_component->SetCallbacks(g_callback, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_SetCallbacks_0400
 * @tc.name   : testSetCallbacks003
 * @tc.desc   : Set the function entry to appData = 1000
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetCallbacks003, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    g_callback = new CodecCallbackService();
    ASSERT_TRUE(g_callback != nullptr);
    auto ret = g_component->SetCallbacks(g_callback, 1000);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Codec_SetCallbacks_0500
 * @tc.name   : testSetCallbacks004
 * @tc.desc   : Set the function entry to appData = 1 and cycle 100 times
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSetCallbacks004, Function | MediumTest | Level1)
{
    int32_t i = 0;
    int32_t ret;
    ASSERT_TRUE(g_component != nullptr);
    g_callback = new CodecCallbackService();
    ASSERT_TRUE(g_callback != nullptr);
    for (i = 0; i < 100; i++) {
        ret = g_component->SetCallbacks(g_callback, 1);
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
}
/**
 * @tc.number  SUB_Driver_Codec_GetComponentVersion_0100
 * @tc.name  testGetComponentVersion001
 * @tc.desc  The input parameter is empty.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testGetComponentVersion001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    struct CompVerInfo verInfo;
    auto ret = g_component->GetComponentVersion(verInfo);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_0100
 * @tc.name  testSendCommand001
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and CODEC_STATE_INVALID. SendCommand is
 * successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand001, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_INVALID, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_0200
 * @tc.name  testSendCommand002
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and CODEC_STATE_LOADED. SendCommand is
 * successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand002, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_LOADED, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_0300
 * @tc.name  testSendCommand003
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and CODEC_STATE_IDLE. SendCommand is
 * successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand003, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_IDLE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_0400
 * @tc.name  testSendCommand004
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and CODEC_STATE_EXECUTING. SendCommand is
 * successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand004, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_EXECUTING, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_0500
 * @tc.name  testSendCommand005
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and CODEC_STATE_PAUSE. SendCommand is
 * successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand005, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_PAUSE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_0600
 * @tc.name  testSendCommand006
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and CODEC_STATE_WAIT_FOR_RESOURCES. SendCommand
 * is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand006, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_WAIT_FOR_RESOURCES, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_0700
 * @tc.name  testSendCommand007
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and CODEC_STATE_KHRONOS_EXTENSIONS. SendCommand
 * is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand007, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_KHRONOS_EXTENSIONS, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_0800
 * @tc.name  testSendCommand008
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and CODEC_STATE_VENDOR_START_UNUSED.
 * SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand008, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_VENDOR_START_UNUSED, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_0900
 * @tc.name  testSendCommand009
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and CODEC_STATE_MAX. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand009, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, CODEC_STATE_MAX, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1000
 * @tc.name  testSendCommand010
 * @tc.desc  The first and second parameters are CODEC_COMMAND_STATE_SET and -1. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand010, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_STATE_SET, -1, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1100
 * @tc.name  testSendCommand011
 * @tc.desc  The first and second parameters are CODEC_COMMAND_FLUSH and -1. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand011, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_FLUSH, -1, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1200
 * @tc.name  testSendCommand012
 * @tc.desc  The first and second parameters are CODEC_COMMAND_FLUSH and 0. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand012, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_FLUSH, 0, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1300
 * @tc.name  testSendCommand013
 * @tc.desc  The first and second parameters are CODEC_COMMAND_FLUSH and 1. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand013, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_FLUSH, 1, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1400
 * @tc.name  testSendCommand014
 * @tc.desc  The first and second parameters are CODEC_COMMAND_FLUSH and CODEC_STATE_PAUSE. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand014, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_FLUSH, CODEC_STATE_PAUSE, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1500
 * @tc.name  testSendCommand015
 * @tc.desc  The first and second parameters are CODEC_COMMAND_PORT_DISABLE and -1. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand015, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_PORT_DISABLE, -1, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1600
 * @tc.name  testSendCommand016
 * @tc.desc  The first and second parameters are CODEC_COMMAND_PORT_DISABLE and 0. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand016, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_PORT_DISABLE, 0, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1700
 * @tc.name  testSendCommand017
 * @tc.desc  The first and second parameters are CODEC_COMMAND_PORT_DISABLE and 1. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand017, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_PORT_DISABLE, 1, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1800
 * @tc.name  testSendCommand018
 * @tc.desc  The first and second parameters are CODEC_COMMAND_PORT_DISABLE and CODEC_STATE_PAUSE. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand018, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_PORT_DISABLE, CODEC_STATE_PAUSE, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_1900
 * @tc.name  testSendCommand019
 * @tc.desc  The first and second parameters are CODEC_COMMAND_PORT_ENABLE and -1. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand019, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_PORT_ENABLE, -1, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2000
 * @tc.name  testSendCommand020
 * @tc.desc  The first and second parameters are CODEC_COMMAND_PORT_ENABLE and 0. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand020, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_PORT_ENABLE, 0, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2100
 * @tc.name  testSendCommand021
 * @tc.desc  The first and second parameters are CODEC_COMMAND_PORT_ENABLE and 1. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand021, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_PORT_ENABLE, 1, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2200
 * @tc.name  testSendCommand022
 * @tc.desc  The first and second parameters are CODEC_COMMAND_PORT_ENABLE and CODEC_STATE_PAUSE. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand022, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_PORT_ENABLE, CODEC_STATE_PAUSE, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2300
 * @tc.name  testSendCommand023
 * @tc.desc  The first and second parameters are CODEC_COMMAND_MARK_BUFFER and -1. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand023, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_MARK_BUFFER, -1, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2400
 * @tc.name  testSendCommand024
 * @tc.desc  The first and second parameters are CODEC_COMMAND_MARK_BUFFER and 0. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand024, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_MARK_BUFFER, 0, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2500
 * @tc.name  testSendCommand025
 * @tc.desc  The first and second parameters are CODEC_COMMAND_MARK_BUFFER and 1. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand025, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_MARK_BUFFER, 1, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2600
 * @tc.name  testSendCommand026
 * @tc.desc  The first and second parameters are CODEC_COMMAND_MARK_BUFFER and CODEC_STATE_PAUSE. SendCommand is error.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand026, Function | MediumTest | Level2)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_MARK_BUFFER, CODEC_STATE_PAUSE, cmdData);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2700
 * @tc.name  testSendCommand027
 * @tc.desc  The first and second parameters are CODEC_COMMAND_KHRONOS_EXTENSIONS and -1. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand027, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_KHRONOS_EXTENSIONS, -1, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2800
 * @tc.name  testSendCommand028
 * @tc.desc  The first and second parameters are CODEC_COMMAND_KHRONOS_EXTENSIONS and 0. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand028, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_KHRONOS_EXTENSIONS, 0, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_2900
 * @tc.name  testSendCommand029
 * @tc.desc  The first and second parameters are CODEC_COMMAND_KHRONOS_EXTENSIONS and 1. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand029, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_KHRONOS_EXTENSIONS, 1, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_3000
 * @tc.name  testSendCommand030
 * @tc.desc  The first and second parameters are CODEC_COMMAND_KHRONOS_EXTENSIONS and CODEC_STATE_PAUSE. SendCommand is
 * successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand030, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_KHRONOS_EXTENSIONS, CODEC_STATE_PAUSE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_3100
 * @tc.name  testSendCommand031
 * @tc.desc  The first and second parameters are CODEC_COMMAND_VENDOR_START_UNUSED and -1. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand031, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_VENDOR_START_UNUSED, -1, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_3200
 * @tc.name  testSendCommand032
 * @tc.desc  The first and second parameters are CODEC_COMMAND_VENDOR_START_UNUSED and 0. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand032, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_VENDOR_START_UNUSED, 0, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_3300
 * @tc.name  testSendCommand033
 * @tc.desc  The first and second parameters are CODEC_COMMAND_VENDOR_START_UNUSED and 1. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand033, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_VENDOR_START_UNUSED, 1, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_3400
 * @tc.name  testSendCommand034
 * @tc.desc  The first and second parameters are CODEC_COMMAND_VENDOR_START_UNUSED and CODEC_STATE_PAUSE. SendCommand is
 * successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand034, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_VENDOR_START_UNUSED, CODEC_STATE_PAUSE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_3500
 * @tc.name  testSendCommand035
 * @tc.desc  The first and second parameters are CODEC_COMMAND_MAX and -1. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand035, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_MAX, -1, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_3600
 * @tc.name  testSendCommand036
 * @tc.desc  The first and second parameters are CODEC_COMMAND_MAX and 0. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand036, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_MAX, 0, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_3700
 * @tc.name  testSendCommand037
 * @tc.desc  The first and second parameters are CODEC_COMMAND_MAX and 1. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand037, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_MAX, 1, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number  SUB_Driver_Codec_SendCommand_3800
 * @tc.name  testSendCommand038
 * @tc.desc  The first and second parameters are CODEC_COMMAND_MAX and CODEC_STATE_PAUSE. SendCommand is successful.
 */
HWTEST_F(CodecBenchmarkManagerTestAdditional, testSendCommand038, Function | MediumTest | Level1)
{
    ASSERT_TRUE(g_component != nullptr);
    std::vector<int8_t> cmdData;
    int32_t ret = g_component->SendCommand(CODEC_COMMAND_MAX, CODEC_STATE_PAUSE, cmdData);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
} // namespace