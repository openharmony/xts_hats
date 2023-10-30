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

#include <buffer_handle.h>
#include <gtest/gtest.h>
#include "hdf_log.h"
#include "v1_0/include/idisplay_buffer.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/display_buffer_type.h"
#include "v1_0/codec_types.h"
#include "v1_0/codec_image_type.h"
#include "v1_0/icodec_image.h"
#define HDF_LOG_TAG codec_jpeg_test

using namespace std;
using namespace testing::ext;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Codec::V1_0;
using namespace OHOS::HDI::Codec::Image::V1_0;
namespace {
constexpr int32_t WIDTH = 640;
constexpr int32_t HEIGHT = 480;
constexpr uint32_t NORMAL_BUFFER_SIZE = 1000;
static OHOS::sptr<ICodecImage> hdiJpeg_;
static IDisplayBuffer *hdiBuffer_;
class CodecHdiJpegTestAdditional : public testing::Test {
public:

    void InitOutBuffer(CodecImageBuffer &outBuffer)
    {
        AllocInfo alloc = {
            .width = WIDTH,
            .height = HEIGHT,
            .usage = HBM_USE_CPU_READ | HBM_USE_CPU_WRITE | HBM_USE_MEM_DMA,
            .format = PIXEL_FMT_YCBCR_420_SP};

        BufferHandle *bufferHandle = nullptr;
        auto err = hdiBuffer_->AllocMem(alloc, bufferHandle);
        if (err != HDF_SUCCESS) {
            return;
        }
        outBuffer.buffer = new NativeBuffer(bufferHandle);
    }

    static void SetUpTestCase()
    {
        hdiJpeg_ = ICodecImage::Get();
        hdiBuffer_ = IDisplayBuffer::Get();
    }
    static void TearDownTestCase()
    {
        hdiJpeg_ = nullptr;
        hdiBuffer_ = nullptr;
    }
    void SetUp()
    {
        if (hdiJpeg_ != nullptr) {
            hdiJpeg_->Init(CODEC_IMAGE_JPEG);
        }
    }
    void TearDown()
    {
        if (hdiJpeg_ != nullptr) {
            hdiJpeg_->DeInit(CODEC_IMAGE_JPEG);
        }
    }
};

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_0900
 * @tc.name   : testCodecDoJpegDecode001
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode001, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    inBuffer.size = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1000
 * @tc.name   : testCodecDoJpegDecode002
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode002, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    inBuffer.buffer = 0;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1100
 * @tc.name   : testCodecDoJpegDecode003
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode003, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    inBuffer.fenceFd = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1200
 * @tc.name   : testCodecDoJpegDecode004
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode004, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    outBuffer.id = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1300
 * @tc.name   : testCodecDoJpegDecode005
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode005, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    outBuffer.size = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1400
 * @tc.name   : testCodecDoJpegDecode006
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode006, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    outBuffer.buffer = 0;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1500
 * @tc.name   : testCodecDoJpegDecode007
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode007, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    outBuffer.fenceFd = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1600
 * @tc.name   : testCodecDoJpegDecode008
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode008, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1700
 * @tc.name   : testCodecDoJpegDecode009
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode0009, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    decInfo.imageHeight = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1800
 * @tc.name   : testCodecDoJpegDecode010
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode0010, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    decInfo.dataPrecision = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_1900
 * @tc.name   : testCodecDoJpegDecode011
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode0011, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    decInfo.numComponents = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2000
 * @tc.name   : testCodecDoJpegDecode012
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode0012, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    decInfo.restartInterval = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2100
 * @tc.name   : testCodecFreeInBuffer001
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecFreeInBuffer001, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.size = -1;
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2200
 * @tc.name   : testCodecFreeInBuffer002
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecFreeInBuffer002, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.buffer = 0;
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_EQ(ret, HDF_FAILURE);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2300
 * @tc.name   : testCodecFreeInBuffer003
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecFreeInBuffer003, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.fenceFd = -1;
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2400
 * @tc.name   : testCodecGetImageCapability001
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability001, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].role = CODEC_IMAGE_INVALID;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2500
 * @tc.name   : testCodecGetImageCapability002
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability002, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].type = CODEC_IMAGE_TYPE_INVALID;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(!capList[0].name.empty());
}
/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2600
 * @tc.name   : testCodecGetImageCapability003
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability003, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].widthAlignment = -1;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_TRUE(capList[0].widthAlignment >= 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2700
 * @tc.name   : testCodecGetImageCapability004
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability004, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].heightAlignment = -1;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_TRUE(capList[0].heightAlignment >= 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2800
 * @tc.name   : testCodecGetImageCapability005
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability005, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].maxSample = -1;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_TRUE(capList[0].maxSample >= 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_2900
 * @tc.name   : testCodecGetImageCapability006
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability006, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].maxWidth = -1;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_TRUE(capList[0].maxWidth >= 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_3000
 * @tc.name   : testCodecGetImageCapability007
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability007, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].maxHeight = -1;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_TRUE(capList[0].maxHeight >= 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_3100
 * @tc.name   : testCodecGetImageCapability008
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability008, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].maxInst = -1;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_TRUE(capList[0].maxInst >= 0);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_3200
 * @tc.name   : testCodecAllocateInBuffer001
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer001, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.id = -1;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_3300
 * @tc.name   : testCodecAllocateInBuffer002
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer002, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.size = -1;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_3400
 * @tc.name   : testCodecAllocateInBuffer003
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer003, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.buffer = 0;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_3500
 * @tc.name   : testCodecAllocateInBuffer004
 * @tc.desc   : Determine whether existing values of parameter structures affect function results
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer004, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.fenceFd = -1;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_3600
 * @tc.name   : testCodecAllocateInBuffer005
 * @tc.desc   : Determines the result of the function when the wrong argument is passed
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer005, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, -1, CODEC_IMAGE_JPEG);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_ImageCodecHDI_3700
 * @tc.name   : testCodecAllocateInBuffer006
 * @tc.desc   : Determines the result of the function when the correct argument is passed in
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer006, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, 200, CODEC_IMAGE_JPEG);
    ASSERT_EQ(ret, HDF_SUCCESS);
}
}  // namespace