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
#include <buffer_handle.h>
#include <gtest/gtest.h>
#include <hdf_log.h>
#include <idisplay_gralloc.h>
#include <osal_mem.h>
#include <securec.h>
#include <servmgr_hdi.h>
#include "codec_callback_type_stub.h"
#include "codec_component_manager.h"
#include "codec_component_type.h"
#include "codec_omx_ext.h"
#include "hdf_io_service_if.h"

#define HDF_LOG_TAG codec_hdi_test

using namespace std;
using namespace testing::ext;
namespace {
constexpr int32_t WIDTH = 640;
constexpr int32_t HEIGHT = 480;
constexpr int32_t BUFFER_SIZE = WIDTH * HEIGHT * 3;
#ifndef SUPPORT_OMX
constexpr uint32_t MAX_ROLE_INDEX = 1000;
constexpr int32_t ROLE_LEN = 240;
#endif
constexpr int32_t FRAMERATE = 30 << 16;
constexpr uint32_t BUFFER_ID_ERROR = 65000;

static void InitCodecBuffer(OmxCodecBuffer& buffer, CodecBufferType type, OMX_VERSIONTYPE& version)
{
    buffer.bufferType = type;
    buffer.fenceFd = -1;
    buffer.version = version;
    buffer.allocLen = BUFFER_SIZE;
    buffer.buffer = 0;
    buffer.bufferLen = 0;
    buffer.pts = 0;
    buffer.flag = 0;
    buffer.type = READ_WRITE_TYPE;
}

class CodecHdiOmxTest : public testing::Test {
public:
    enum class PortIndex { PORT_INDEX_INPUT = 0, PORT_INDEX_OUTPUT = 1 };
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
            if (bufferHandle != nullptr && gralloc_ != nullptr) {
                gralloc_->FreeMem(*bufferHandle);
                bufferHandle = nullptr;
            }
        }
    };
    template <typename T>
    void InitParam(T &param)
    {
        memset_s(&param, sizeof(param), 0x0, sizeof(param));
        param.nSize = sizeof(param);
        param.nVersion = version_;
    }

    template <typename T>
    void InitExtParam(T &param)
    {
        memset_s(&param, sizeof(param), 0x0, sizeof(param));
        param.size = sizeof(param);
        param.version = version_;
    }

    void InitCodecBufferWithAshMem(enum PortIndex portIndex, int bufferSize, shared_ptr<OmxCodecBuffer> omxBuffer,
                                   shared_ptr<OHOS::Ashmem> sharedMem)
    {
        omxBuffer->size = sizeof(OmxCodecBuffer);
        omxBuffer->version = version_;
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

    bool UseBufferOnPort(enum PortIndex portIndex, int32_t bufferCount, int32_t bufferSize)
    {
        for (int i = 0; i < bufferCount; i++) {
            std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
            if (omxBuffer == nullptr) {
                return false;
            }

            int fd = OHOS::AshmemCreate(0, bufferSize);
            shared_ptr<OHOS::Ashmem> sharedMem = make_shared<OHOS::Ashmem>(fd, bufferSize);
            if (sharedMem == nullptr) {
                if (fd >= 0) {
                    close(fd);
                    fd = -1;
                }
                return false;
            }
            InitCodecBufferWithAshMem(portIndex, bufferSize, omxBuffer, sharedMem);
            auto err = component_->UseBuffer(component_, (uint32_t)portIndex, omxBuffer.get());
            if (err != HDF_SUCCESS) {
                sharedMem->UnmapAshmem();
                sharedMem->CloseAshmem();
                sharedMem = nullptr;
                omxBuffer = nullptr;
                return false;
            }
            omxBuffer->bufferLen = 0;
            std::shared_ptr<BufferInfo> bufferInfo = std::make_shared<BufferInfo>();
            bufferInfo->omxBuffer = omxBuffer;
            bufferInfo->sharedMem = sharedMem;
            if (portIndex == PortIndex::PORT_INDEX_INPUT) {
                inputBuffers_.emplace(std::make_pair(omxBuffer->bufferId, bufferInfo));
            } else {
                outputBuffers_.emplace(std::make_pair(omxBuffer->bufferId, bufferInfo));
            }
        }
        return true;
    }

    bool FreeBufferOnPort(enum PortIndex portIndex)
    {
        std::map<int32_t, std::shared_ptr<BufferInfo>> &buffer = inputBuffers_;
        if (portIndex == PortIndex::PORT_INDEX_OUTPUT) {
            buffer = outputBuffers_;
        }
        for (auto [bufferId, bufferInfo] : buffer) {
            auto ret = component_->FreeBuffer(component_, (uint32_t)portIndex, bufferInfo->omxBuffer.get());
            if (ret != HDF_SUCCESS) {
                return false;
            }
        }
        buffer.clear();
        return true;
    }
    static void SetUpTestCase()
    {
        manager_ = GetCodecComponentManager();
        gralloc_ = OHOS::HDI::Display::V1_0::IDisplayGralloc::Get();
        if (manager_ == nullptr) {
            std::cout<<"GetCodecComponentManager ret nullptr"<<std::endl;
            return;
        }
        auto count = manager_->GetComponentNum();
        if (count > 0) {
            CodecCompCapability *capList = (CodecCompCapability *)OsalMemAlloc(sizeof(CodecCompCapability) * count);
            ASSERT_TRUE(capList != nullptr);
            auto err = manager_->GetComponentCapabilityList(capList, count);
            ASSERT_TRUE(err == HDF_SUCCESS);
            compName_ = capList[0].compName;
            OsalMemFree(capList);
            capList = nullptr;
        }
    }
    static void TearDownTestCase()
    {
        CodecComponentManagerRelease();
        manager_ = nullptr;
    }
    void SetUp()
    {
        if (manager_ == nullptr) {
            return;
        }
        callback_ = CodecCallbackTypeStubGetInstance();
        if (callback_ == nullptr) {
            return;
        }
        if (compName_.empty()) {
            return;
        }

        auto ret = manager_->CreateComponent(&component_, &componentId_, compName_.data(),
                                             reinterpret_cast<int64_t>(this), callback_);
        if (ret != HDF_SUCCESS) {
            return;
        }
        struct CompVerInfo verInfo;
        ret = component_->GetComponentVersion(component_, &verInfo);
        if (ret != HDF_SUCCESS) {
            return;
        }
        version_ = verInfo.compVersion;
    }
    void TearDown()
    {
        if (manager_ != nullptr && component_ != nullptr) {
            manager_->DestroyComponent(componentId_);
        }
        if (callback_ != nullptr) {
            CodecCallbackTypeStubRelease(callback_);
            callback_ = nullptr;
        }
    }

public:
    struct CodecComponentType *component_ = nullptr;
    uint32_t componentId_ = 0;
    struct CodecCallbackType *callback_ = nullptr;

    static inline struct CodecComponentManager *manager_ = nullptr;
    static inline std::string compName_ = "";
    union OMX_VERSIONTYPE version_;
    std::map<int32_t, std::shared_ptr<BufferInfo>> inputBuffers_;
    std::map<int32_t, std::shared_ptr<BufferInfo>> outputBuffers_;
    static inline OHOS::HDI::Display::V1_0::IDisplayGralloc *gralloc_ = nullptr;
};
/**
* @tc.name  HdfCodecHdiGetVersionTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0080
* @tc.desc   Reads the version information of an open component
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0080, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    struct CompVerInfo verInfo;
    auto ret = component_->GetComponentVersion(component_, &verInfo);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiGetVersionTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0090
* @tc.desc  The input parameter is empty. GetComponentVersion is error.
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0090, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->GetComponentVersion(component_, nullptr);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0100
* @tc.desc  The input parameter structure pointer is null when the OMX_IndexParamVideoPortFormat parameter is read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0100, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    CodecVideoPortFormatParam pixFormat;
    InitExtParam(pixFormat);
    pixFormat.portIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    pixFormat.codecColorIndex = 0;
    auto ret = component_->GetParameter(component_, OMX_IndexCodecVideoPortFormat,
                                        reinterpret_cast<int8_t *>(&pixFormat), sizeof(pixFormat));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0110
* @tc.desc  The OMX_IndexParamVideoPortFormat parameter of the input port of the component is normally read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0110, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    CodecVideoPortFormatParam pixFormat;
    InitExtParam(pixFormat);
    pixFormat.portIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    pixFormat.codecColorIndex = 0;
    auto ret = component_->GetParameter(component_, OMX_IndexCodecVideoPortFormat,
                                        reinterpret_cast<int8_t *>(&pixFormat), sizeof(pixFormat));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
#endif
/**
* @tc.name  HdfCodecHdiGetParameterTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0120
* @tc.desc  The version information is not set for the input parameter. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0120, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->GetParameter(component_, OMX_IndexParamVideoPortFormat, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0130
* @tc.desc  The input parameter structure does not match the index. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0130, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    param.eCompressionFormat = OMX_VIDEO_CodingAVC;
    auto ret = component_->GetParameter(component_, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                        sizeof(param));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0140
* @tc.desc  The input parameter index is not supported. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0140, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    memset_s(&param, sizeof(param), 0, sizeof(param));
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    param.eCompressionFormat = OMX_VIDEO_CodingAVC;
    auto ret = component_->GetParameter(component_, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                        sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0150
* @tc.desc  The input parameter prot is not supported. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0150, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = component_->GetParameter(component_, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                        sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetParameterTest_007
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0160
* @tc.desc  The input parameter MX_IndexVideoStartUnused. As a result, the parameter fails to be read
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0160, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = component_->GetParameter(component_, OMX_IndexVideoStartUnused, reinterpret_cast<int8_t *>(&param),
                                        sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetParameterTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0170
* @tc.desc  Setting the OMX_IndexParamVideoPortFormat Parameter of the Component Input Port
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0170, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = component_->SetParameter(component_, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                        sizeof(param));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetParameterTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0180
* @tc.desc  The input parameter version information is not set. As a result, the parameter setting fails
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0180, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    memset_s(&param, sizeof(param), 0, sizeof(param));
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = component_->SetParameter(component_, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                        sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetParameterTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0190
* @tc.desc  The input parameter is  null
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0190, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->SetParameter(component_, OMX_IndexParamVideoPortFormat, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetParameterTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0200
* @tc.desc  Parameter does not match index
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0200, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = component_->SetParameter(component_, OMX_IndexParamVideoPortFormat, reinterpret_cast<int8_t *>(&param),
                                        sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetParameterTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0210
* @tc.desc  The parameter index is not supported
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0210, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_PARAM_PORTFORMATTYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = component_->SetParameter(component_, OMX_IndexVideoStartUnused, reinterpret_cast<int8_t *>(&param),
                                        sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiSetParameterTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0220
* @tc.desc
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0220, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    CodecVideoPortFormatParam pixFormat;
    InitExtParam(pixFormat);
    pixFormat.portIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    pixFormat.codecColorIndex = 0;
    auto ret = component_->GetParameter(component_, OMX_IndexCodecVideoPortFormat,
                                        reinterpret_cast<int8_t *>(&pixFormat), sizeof(pixFormat));
    ASSERT_EQ(ret, HDF_SUCCESS);
    pixFormat.codecColorFormat = PIXEL_FMT_RGB_555;
    ret = component_->SetParameter(component_, OMX_IndexCodecVideoPortFormat, reinterpret_cast<int8_t *>(&pixFormat),
                                   sizeof(pixFormat));
    ASSERT_EQ(ret, HDF_SUCCESS);
}
#endif
/**
* @tc.name  HdfCodecHdiGetConfigTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0230
* @tc.desc  The configuration is read normally
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0230, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    auto ret = component_->GetConfig(component_, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
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
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    auto ret = component_->GetConfig(component_, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
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
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->GetConfig(component_, OMX_IndexConfigVideoBitrate, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetConfigTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0260
* @tc.desc  The input parameter index is not supported
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0260, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    auto ret =
        component_->GetConfig(component_, OMX_IndexVideoStartUnused, reinterpret_cast<int8_t *>(&param), sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetConfigTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0270
* @tc.desc  The configuration is successful
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0270, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    param.nEncodeBitrate = FRAMERATE;
    auto ret = component_->SetConfig(component_, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
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
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_INPUT;
    param.nEncodeBitrate = FRAMERATE;
    auto ret = component_->SetConfig(component_, OMX_IndexConfigVideoBitrate, reinterpret_cast<int8_t *>(&param),
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
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->SetConfig(component_, OMX_IndexConfigVideoBitrate, nullptr, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetConfigTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0300
* @tc.desc  The input parameter index is not supported
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0300, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_VIDEO_CONFIG_BITRATETYPE param;
    InitParam(param);
    param.nPortIndex = (uint32_t)PortIndex::PORT_INDEX_OUTPUT;
    auto ret =
        component_->SetConfig(component_, OMX_IndexVideoStartUnused, reinterpret_cast<int8_t *>(&param), sizeof(param));
    ASSERT_NE(ret, HDF_SUCCESS);
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiGetExtensionIndexTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0310
* @tc.desc  Obtaining the extended index normally
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0310, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_INDEXTYPE indexType;
    auto ret =
        component_->GetExtensionIndex(component_, "OMX.Topaz.index.param.extended_video", (uint32_t *)&indexType);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
#endif
/**
* @tc.name  HdfCodecHdiGetExtensionIndexTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0320
* @tc.desc  The parameter name is null pointer
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0320, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_INDEXTYPE indexType;
    auto ret = component_->GetExtensionIndex(component_, nullptr, (uint32_t *)&indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetExtensionIndexTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0330
* @tc.desc  Unsupported parameter
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0330, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_INDEXTYPE indexType;
    auto ret = component_->GetExtensionIndex(component_, "OMX.Topaz.index.param.extended_test", (uint32_t *)&indexType);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetExtensionIndexTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0340
* @tc.desc  Index is null pointer
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0340, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->GetExtensionIndex(component_, "OMX.Topaz.index.param.extended_video", nullptr);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiGetStateTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0350
* @tc.desc  Reads the current status of the component
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0350, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_STATETYPE state;
    auto ret = component_->GetState(component_, &state);
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
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->GetState(component_, nullptr);
    ASSERT_NE(ret, HDF_SUCCESS);
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiTunnelRequestTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0370
* @tc.desc  The interface is not supported
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0370, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    const int32_t tunneledComp = 1002;
    const uint32_t tunneledPort = 101;
    OMX_TUNNELSETUPTYPE tunnelSetup;
    tunnelSetup.eSupplier = OMX_BufferSupplyInput;

    auto ret = component_->ComponentTunnelRequest(component_, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, tunneledComp,
                                                  tunneledPort, &tunnelSetup);
    ASSERT_NE(ret, HDF_SUCCESS);
}
#endif
/**
* @tc.name  HdfCodecHdiLoadedToExecutingTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0380
* @tc.desc The status is changed to OMX_StateExecuting
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0380, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateExecuting, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0390
* @tc.desc The input buffer type is CODEC_BUFFER_TYPE_INVALID
*/
struct OmxCodecBuffer allocBuffer;
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0390, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    InitCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_INVALID, version_);
    auto ret = component_->AllocateBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0410
* @tc.desc The input buffer type is CODEC_BUFFER_TYPE_VIRTUAL_ADDR
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0410, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    InitCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_VIRTUAL_ADDR, version_);
    auto ret = component_->AllocateBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0420
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_INVALID
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0420, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    InitCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_INVALID, version_);
    auto ret = component_->AllocateBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0430
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_VIRTUAL_ADDR
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0430, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    InitCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_VIRTUAL_ADDR, version_);
    auto ret = component_->AllocateBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0440
* @tc.desc The input buffer type is CODEC_BUFFER_TYPE_INVALID
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0440, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = version_;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_INVALID;
    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    auto err = component_->UseBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_INPUT, omxBuffer.get());
    ASSERT_NE(err, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0450
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_INVALID
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0450, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = version_;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_INVALID;
    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    auto err = component_->UseBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, omxBuffer.get());
    ASSERT_NE(err, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0460
* @tc.desc The input buffer type is CODEC_BUFFER_TYPE_VIRTUAL_ADDR
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0460, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = version_;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_VIRTUAL_ADDR;
    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    auto err = component_->UseBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_INPUT, omxBuffer.get());
    ASSERT_NE(err, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0470
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_VIRTUAL_ADDR
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0470, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = version_;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_VIRTUAL_ADDR;
    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    auto err = component_->UseBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, omxBuffer.get());
    ASSERT_NE(err, HDF_SUCCESS);
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiUseBufferTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0480
* @tc.desc The intput buffer type is CODEC_BUFFER_TYPE_AVSHARE_MEM_FD
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0480, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_INPUT;
    auto err = component_->GetParameter(component_, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int32_t bufferSize = param.nBufferSize;
    int32_t bufferCount = param.nBufferCountActual;
    auto ret = UseBufferOnPort(PortIndex::PORT_INDEX_INPUT, bufferCount, bufferSize);
    ASSERT_TRUE(ret);
    FreeBufferOnPort(PortIndex::PORT_INDEX_INPUT);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0490
* @tc.desc The intput buffer type is CODEC_BUFFER_TYPE_HANDLE
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0490, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto err = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateIdle, NULL, 0);
    ASSERT_EQ(err, HDF_SUCCESS);
    AllocInfo alloc = {.width = WIDTH,
                       .height = HEIGHT,
                       .usage = HBM_USE_CPU_READ | HBM_USE_CPU_WRITE | HBM_USE_MEM_DMA,
                       .format = PIXEL_FMT_YCBCR_420_SP};
    ASSERT_TRUE(gralloc_ != nullptr);
    BufferHandle *bufferHandle = nullptr;
    err = gralloc_->AllocMem(alloc, bufferHandle);
    ASSERT_EQ(err, DISPLAY_SUCCESS);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = version_;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_HANDLE;
    omxBuffer->bufferLen = sizeof(BufferHandle);
    omxBuffer->buffer = reinterpret_cast<uint8_t *>(bufferHandle);
    omxBuffer->allocLen = sizeof(BufferHandle);
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    err = component_->UseBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_INPUT, omxBuffer.get());
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
    inputBuffers_.emplace(std::make_pair(omxBuffer->bufferId, bufferInfo));
    FreeBufferOnPort(PortIndex::PORT_INDEX_INPUT);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_007
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0500
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_AVSHARE_MEM_FD
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0500, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_OUTPUT;
    auto err = component_->GetParameter(component_, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int32_t bufferSize = param.nBufferSize;
    int32_t bufferCount = param.nBufferCountActual;
    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_OUTPUT, bufferCount, bufferSize);
    ASSERT_TRUE(ret);
    ret = FreeBufferOnPort(PortIndex::PORT_INDEX_OUTPUT);
    ASSERT_TRUE(ret);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_008
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0510
* @tc.desc The output buffer type is CODEC_BUFFER_TYPE_DYNAMIC_HANDLE
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0510, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto err = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateIdle, NULL, 0);
    ASSERT_EQ(err, HDF_SUCCESS);
    std::shared_ptr<OmxCodecBuffer> omxBuffer = std::make_shared<OmxCodecBuffer>();
    ASSERT_TRUE(omxBuffer != nullptr);
    omxBuffer->size = sizeof(OmxCodecBuffer);
    omxBuffer->version = version_;
    omxBuffer->bufferType = CODEC_BUFFER_TYPE_DYNAMIC_HANDLE;
    omxBuffer->bufferLen = 0;
    omxBuffer->buffer = nullptr;
    omxBuffer->allocLen = 0;
    omxBuffer->fenceFd = -1;
    omxBuffer->pts = 0;
    omxBuffer->flag = 0;

    err = component_->UseBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, omxBuffer.get());
    if (err != HDF_SUCCESS) {
        HDF_LOGE("%{public}s failed to UseBuffer with  input port", __func__);
        omxBuffer = nullptr;
    }
    ASSERT_EQ(err, HDF_SUCCESS);
    omxBuffer->bufferLen = 0;
    std::shared_ptr<BufferInfo> bufferInfo = std::make_shared<BufferInfo>();
    ASSERT_TRUE(bufferInfo != nullptr);
    bufferInfo->omxBuffer = omxBuffer;
    outputBuffers_.emplace(std::make_pair(omxBuffer->bufferId, bufferInfo));
    FreeBufferOnPort(PortIndex::PORT_INDEX_OUTPUT);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_009
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0520
* @tc.desc The input buffer is full
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0520, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_INPUT;
    auto err = component_->GetParameter(component_, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_INPUT, param.nBufferCountActual, param.nBufferSize);
    ASSERT_TRUE(ret);
    ret = UseBufferOnPort(PortIndex::PORT_INDEX_INPUT, 1, param.nBufferSize);
    FreeBufferOnPort(PortIndex::PORT_INDEX_INPUT);
    ASSERT_FALSE(ret);
}
/**
* @tc.name  HdfCodecHdiUseBufferTest_010
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0530
* @tc.desc The output buffer is full
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0530, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_OUTPUT;
    auto err = component_->GetParameter(component_, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);
    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_OUTPUT, param.nBufferCountActual, param.nBufferSize);
    ASSERT_TRUE(ret);
    ret = UseBufferOnPort(PortIndex::PORT_INDEX_OUTPUT, 1, param.nBufferSize);
    FreeBufferOnPort(PortIndex::PORT_INDEX_OUTPUT);
    ASSERT_FALSE(ret);
}
#endif
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_005
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0540
* @tc.desc The input buffer is full
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0540, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto err = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateIdle, NULL, 0);
    ASSERT_EQ(err, HDF_SUCCESS);
    InitCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD, version_);
    err = component_->AllocateBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    err = component_->FreeBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiAllocateBufferTest_006
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0550
* @tc.desc The output buffer is full
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0550, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto err = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateIdle, NULL, 0);
    ASSERT_EQ(err, HDF_SUCCESS);
    InitCodecBuffer(allocBuffer, CODEC_BUFFER_TYPE_AVSHARE_MEM_FD, version_);
    allocBuffer.type = READ_WRITE_TYPE;
    err = component_->AllocateBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
    err = component_->FreeBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_EQ(err, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiUseEglImageTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0560
* @tc.desc The interface is invoked successfully. The OMX does not support this function
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0560, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    struct OmxCodecBuffer buffer;
    buffer.fenceFd = -1;
    buffer.version = version_;
    buffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    buffer.allocLen = BUFFER_SIZE;
    buffer.buffer = 0;
    buffer.bufferLen = 0;
    buffer.pts = 0;
    buffer.flag = 0;
    buffer.type = READ_ONLY_TYPE;
    auto eglImage = std::make_unique<int8_t[]>(BUFFER_SIZE);
    ASSERT_TRUE(eglImage != nullptr);
    auto ret = component_->UseEglImage(component_, &buffer, (uint32_t)PortIndex::PORT_INDEX_INPUT, eglImage.get(),
                                       BUFFER_SIZE);
    ASSERT_NE(ret, HDF_SUCCESS);
    eglImage = nullptr;
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiFillThisBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0580
* @tc.desc FillThisBuffer test
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0580, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto err = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateIdle, NULL, 0);
    ASSERT_EQ(err, HDF_SUCCESS);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_INPUT;
    err = component_->GetParameter(component_, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    bool ret = UseBufferOnPort(PortIndex::PORT_INDEX_INPUT, param.nBufferCountActual, param.nBufferSize);
    ASSERT_TRUE(ret);
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_OUTPUT;
    err = component_->GetParameter(component_, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);
    ret = UseBufferOnPort(PortIndex::PORT_INDEX_OUTPUT, param.nBufferCountActual, param.nBufferSize);
    ASSERT_TRUE(ret);
    err = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateExecuting, NULL, 0);

    OMX_STATETYPE state = OMX_StateInvalid;
    do {
        usleep(10);
        auto ret = component_->GetState(component_, &state);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } while (state != OMX_StateExecuting);

    auto iter = outputBuffers_.begin();
    if (iter != outputBuffers_.end()) {
        auto ret = component_->FillThisBuffer(component_, iter->second->omxBuffer.get());
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
    iter = inputBuffers_.begin();
    if (iter != inputBuffers_.end()) {
        auto ret = component_->EmptyThisBuffer(component_, iter->second->omxBuffer.get());
        ASSERT_EQ(ret, HDF_SUCCESS);
    }

    err = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateIdle, nullptr, 0);
    FreeBufferOnPort(PortIndex::PORT_INDEX_INPUT);
    FreeBufferOnPort(PortIndex::PORT_INDEX_OUTPUT);
    
    err = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateLoaded, nullptr, 0);
    do {
        usleep(10);
        auto ret = component_->GetState(component_, &state);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } while (state != OMX_StateLoaded);
    component_->ComponentDeInit(component_);
}
#endif
/**
* @tc.name  HdfCodecHdiFillThisBufferTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0590
* @tc.desc The buffer ID is incorrect
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0590, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    allocBuffer.fenceFd = -1;
    allocBuffer.version = version_;
    allocBuffer.allocLen = BUFFER_SIZE;
    allocBuffer.buffer = 0;
    allocBuffer.bufferLen = 0;
    allocBuffer.pts = 0;
    allocBuffer.flag = 0;
    allocBuffer.type = READ_ONLY_TYPE;
    allocBuffer.bufferId = BUFFER_ID_ERROR;
    auto ret = component_->FillThisBuffer(component_, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiEmptyThisBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0600
* @tc.desc EmptyThisBuffer test
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0600, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    allocBuffer.fenceFd = -1;
    allocBuffer.version = version_;
    allocBuffer.allocLen = BUFFER_SIZE;
    allocBuffer.buffer = 0;
    allocBuffer.bufferLen = 0;
    allocBuffer.pts = 0;
    allocBuffer.flag = 0;
    allocBuffer.type = READ_ONLY_TYPE;
    allocBuffer.bufferId = BUFFER_ID_ERROR;
    auto ret = component_->EmptyThisBuffer(component_, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiSetCallbackTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0620
* @tc.desc Setting Component Callbacks
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0620, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    if (callback_ != nullptr) {
        CodecCallbackTypeStubRelease(callback_);
    }
    callback_ = CodecCallbackTypeStubGetInstance();
    ASSERT_TRUE(callback_ != nullptr);
    auto ret = component_->SetCallbacks(component_, callback_, (int64_t)this);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiSetCallbackTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0630
* @tc.desc The callback pointer is null
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0630, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    uint8_t role[ROLE_LEN] = {0};
    auto ret = component_->ComponentRoleEnum(component_, role, ROLE_LEN, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
#endif
/**
* @tc.name  HdfCodecHdiRoleEnumTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0640
* @tc.desc Obtaining Component Roles Based on Indexes
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0640, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->ComponentRoleEnum(component_, nullptr, 0, 0);
    ASSERT_NE(ret, HDF_SUCCESS);
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiRoleEnumTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0650
* @tc.desc The role is null
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0650, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    uint8_t role[ROLE_LEN] = {0};
    auto ret = component_->ComponentRoleEnum(component_, role, ROLE_LEN, MAX_ROLE_INDEX);
    ASSERT_NE(ret, HDF_SUCCESS);
}
#endif
/**
* @tc.name  HdfCodecHdiRoleEnumTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0660
* @tc.desc The index is too large
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0660, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateIdle, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
/**
* @tc.name  HdfCodecHdiExecutingToIdleTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0670
* @tc.desc The component enters the OMX_Idle state
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0670, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    allocBuffer.fenceFd = -1;
    allocBuffer.version = version_;
    allocBuffer.allocLen = BUFFER_SIZE;
    allocBuffer.buffer = 0;
    allocBuffer.bufferLen = 0;
    allocBuffer.pts = 0;
    allocBuffer.flag = 0;
    allocBuffer.type = READ_ONLY_TYPE;
    allocBuffer.bufferId = BUFFER_ID_ERROR;
    auto ret = component_->FreeBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_OUTPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiFreeBufferTest_001
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0680
* @tc.desc The buffer ID of the output port is incorrect
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0680, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_OUTPUT;
    auto err = component_->GetParameter(component_, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int32_t bufferSize = param.nBufferSize;
    int32_t bufferCount = param.nBufferCountActual;
    auto ret = UseBufferOnPort(PortIndex::PORT_INDEX_OUTPUT, bufferCount, bufferSize);
    ASSERT_TRUE(ret);
    ret = FreeBufferOnPort(PortIndex::PORT_INDEX_OUTPUT);
    ASSERT_TRUE(ret);
}
#endif
/**
* @tc.name  HdfCodecHdiFreeBufferTest_002
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0690
* @tc.desc Test on normal release of the output port
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0690, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    allocBuffer.bufferType = CODEC_BUFFER_TYPE_AVSHARE_MEM_FD;
    allocBuffer.fenceFd = -1;
    allocBuffer.version = version_;
    allocBuffer.allocLen = BUFFER_SIZE;
    allocBuffer.buffer = 0;
    allocBuffer.bufferLen = 0;
    allocBuffer.pts = 0;
    allocBuffer.flag = 0;
    allocBuffer.type = READ_ONLY_TYPE;
    allocBuffer.bufferId = BUFFER_ID_ERROR;
    auto ret = component_->FreeBuffer(component_, (uint32_t)PortIndex::PORT_INDEX_INPUT, &allocBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
#ifndef SUPPORT_OMX
/**
* @tc.name  HdfCodecHdiFreeBufferTest_003
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0700
* @tc.desc The buffer ID of the input port is incorrect
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0700, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    OMX_PARAM_PORTDEFINITIONTYPE param;
    InitParam(param);
    param.nPortIndex = (OMX_U32)PortIndex::PORT_INDEX_INPUT;
    auto err = component_->GetParameter(component_, OMX_IndexParamPortDefinition, (int8_t *)&param, sizeof(param));
    ASSERT_EQ(err, HDF_SUCCESS);

    int32_t bufferSize = param.nBufferSize;
    int32_t bufferCount = param.nBufferCountActual;
    auto ret = UseBufferOnPort(PortIndex::PORT_INDEX_INPUT, bufferCount, bufferSize);
    ASSERT_TRUE(ret);
    ret = FreeBufferOnPort(PortIndex::PORT_INDEX_INPUT);
    ASSERT_TRUE(ret);
}
// When ComponentDeInit, must change to Loaded State
/**
* @tc.name  HdfCodecHdiFreeBufferTest_004
* @tc.number  SUB_DriverSystem_CodecHdi_V2_0710
* @tc.desc Test on normal release of the input port
*/
HWTEST_F(CodecHdiOmxTest, SUB_DriverSystem_CodecHdi_V2_0710, Function | MediumTest | Level3)
{
    ASSERT_TRUE(component_ != nullptr);
    auto ret = component_->SendCommand(component_, OMX_CommandStateSet, OMX_StateLoaded, nullptr, 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
    // State changed OMX_StateIdle when release all this buffer
    OMX_STATETYPE state = OMX_StateInvalid;
    do {
        usleep(100);
        ret = component_->GetState(component_, &state);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } while (state != OMX_StateLoaded);
    ret = component_->ComponentDeInit(component_);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
#endif

}  // namespace
