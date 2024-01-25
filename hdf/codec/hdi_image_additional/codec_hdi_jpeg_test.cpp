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

#include "hdf_log.h"
#include "v1_0/codec_image_type.h"
#include "v1_0/codec_types.h"
#include "v1_0/display_buffer_type.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/icodec_image.h"
#include "v1_0/include/idisplay_buffer.h"
#include <buffer_handle.h>
#include <gtest/gtest.h>
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
constexpr uint32_t CODEC_IMAGE_MAX_BUFFER_SIZE = 50 * 1024 * 1024;
static OHOS::sptr<ICodecImage> hdiJpeg_;
static IDisplayBuffer *hdiBuffer_;
class CodecHdiJpegTestAdditional : public testing::Test {
public:
    void InitOutBuffer(CodecImageBuffer &outBuffer)
    {
        AllocInfo alloc = {.width = WIDTH,
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
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode009, TestSize.Level2)
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
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode010, TestSize.Level2)
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
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode011, TestSize.Level2)
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
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode012, TestSize.Level2)
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

/**
 * @tc.number : SUB_Driver_Codec_Init_0100
 * @tc.name   : testInit001
 * @tc.desc   : Determines the result of the function when the correct argument is invalid
 */
HWTEST_F(CodecHdiJpegTestAdditional, testInit001, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    auto ret = hdiJpeg_->Init(CODEC_IMAGE_INVALID);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_Init_0200
 * @tc.name   : testInit002
 * @tc.desc   : Determines the result of the function with CODEC_IMAGE_JPEG
 */
HWTEST_F(CodecHdiJpegTestAdditional, testInit002, Function | MediumTest | Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    auto ret = hdiJpeg_->DeInit(CODEC_IMAGE_JPEG);
    ret = hdiJpeg_->Init(CODEC_IMAGE_JPEG);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_Init_0300
 * @tc.name   : testInit003
 * @tc.desc   : Determines the result of the function with CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testInit003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    auto ret = hdiJpeg_->Init(CODEC_IMAGE_HEIF);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DeInit_0100
 * @tc.name   : testDeInit001
 * @tc.desc   : Determines the result of the function when the correct argument is invalid
 */
HWTEST_F(CodecHdiJpegTestAdditional, testDeInit001, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    auto ret = hdiJpeg_->DeInit(CODEC_IMAGE_INVALID);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DeInit_0200
 * @tc.name   : testDeInit002
 * @tc.desc   : Determines the result of the function with CODEC_IMAGE_JPEG
 */
HWTEST_F(CodecHdiJpegTestAdditional, testDeInit002, Function | MediumTest | Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    auto ret = hdiJpeg_->DeInit(CODEC_IMAGE_JPEG);
    ret = hdiJpeg_->DeInit(CODEC_IMAGE_JPEG);
    ret = hdiJpeg_->Init(CODEC_IMAGE_JPEG);
}

/**
 * @tc.number : SUB_Driver_Codec_DeInit_0300
 * @tc.name   : testDeInit003
 * @tc.desc   : Determines the result of the function with CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testDeInit003, Function | MediumTest | Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    auto ret = hdiJpeg_->DeInit(CODEC_IMAGE_HEIF);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_2100
 * @tc.name   : testCodecDoJpegDecode013
 * @tc.desc   : When the third argument CodecJpegDecInfo struct ->dataPrecision = 0, the result returns failure
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode013, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(outBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = 1200;
    decInfo.imageHeight = 960;
    decInfo.dataPrecision = 0;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(inBuffer), HDF_SUCCESS);
    ASSERT_EQ(hdiJpeg_->FreeInBuffer(outBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_2200
 * @tc.name   : testCodecDoJpegDecode014
 * @tc.desc   : When the second parameter CodecImageBuffer structure is empty, the result returns a failure
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode014, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = 1200;
    decInfo.imageHeight = 960;
    decInfo.dataPrecision = 8;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(inBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_2300
 * @tc.name   : testCodecDoJpegDecode015
 * @tc.desc   : When the first argument CodecImageBuffer structure is empty, the result returns a failure
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode015, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(outBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = 1200;
    decInfo.imageHeight = 960;
    decInfo.dataPrecision = 8;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(outBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_2400
 * @tc.name   : testCodecDoJpegDecode016
 * @tc.desc   : Verify that the result fails when the second entry of the first prefix function AllocateInBuffer is 0
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode016, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(inBuffer, 0, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(outBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = WIDTH;
    decInfo.imageHeight = HEIGHT;
    decInfo.dataPrecision = 8;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(outBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_2500
 * @tc.name   : testCodecDoJpegDecode017
 * @tc.desc   : Failure to verify that the third entry of the first prefix function AllocateInBuffer is CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode017, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_HEIF), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(outBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = WIDTH;
    decInfo.imageHeight = HEIGHT;
    decInfo.dataPrecision = 8;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(outBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_2600
 * @tc.name   : testCodecDoJpegDecode018
 * @tc.desc   : Verify that a failure is returned when the third entry to
 *              the first prefix function AllocateInBuffer is CODEC_IMAGE_INVALID
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode018, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_INVALID), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(outBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = WIDTH;
    decInfo.imageHeight = HEIGHT;
    decInfo.dataPrecision = 8;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(outBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_2700
 * @tc.name   : testCodecDoJpegDecode019
 * @tc.desc   : Validation failed when the second entry to the second prefix function AllocateInBuffer is 0
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode019, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(outBuffer, 0, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = WIDTH;
    decInfo.imageHeight = HEIGHT;
    decInfo.dataPrecision = 8;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(inBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_2800
 * @tc.name   : testCodecDoJpegDecode020
 * @tc.desc   : Verification fails when the second entry of the second prefix
 *              function AllocateInBuffer is 50 * 1024 * 1024 +1
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode020, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(outBuffer, CODEC_IMAGE_MAX_BUFFER_SIZE + 1, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = WIDTH;
    decInfo.imageHeight = HEIGHT;
    decInfo.dataPrecision = 8;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(inBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_2900
 * @tc.name   : testCodecDoJpegDecode021
 * @tc.desc   : Validation failed when the third entry to the second prefix
 *              function AllocateInBuffer is CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode021, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(outBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_HEIF), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = WIDTH;
    decInfo.imageHeight = HEIGHT;
    decInfo.dataPrecision = 8;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(inBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_3000
 * @tc.name   : testCodecDoJpegDecode022
 * @tc.desc   : Validation returns a failure when the third entry to the second prefix
 *              function AllocateInBuffer is CODEC_IMAGE_INVALID
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode022, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(outBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_INVALID), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = WIDTH;
    decInfo.imageHeight = HEIGHT;
    decInfo.dataPrecision = 8;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(inBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_DoJpegDecode_3100
 * @tc.name   : testCodecDoJpegDecode023
 * @tc.desc   : Verification fails when the second entry of the first prefix
 *              function AllocateInBuffer is 50 * 1024 * 1024 +1
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecDoJpegDecode023, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(inBuffer, CODEC_IMAGE_MAX_BUFFER_SIZE + 1, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecImageBuffer outBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(outBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    struct CodecJpegDecInfo decInfo;
    decInfo.imageWidth = WIDTH;
    decInfo.imageHeight = HEIGHT;
    decInfo.dataPrecision = 0;

    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);

    ASSERT_EQ(hdiJpeg_->FreeInBuffer(outBuffer), HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_GetImageCapability_3200
 * @tc.name   : testCodecGetImageCapability009
 * @tc.desc   : Verify that the GetImageCapability function returns a successful result when its argument is null
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability009, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_GetImageCapability_3300
 * @tc.name   : testCodecGetImageCapability010
 * @tc.desc   : Verify that when the GetImageCapability function argument CodecImageCapability
 *              structure->name = "abc", the result is returned successfully
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability010, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].name = "abc";
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(capList[0].name, "abc");
}

/**
 * @tc.number : SUB_Driver_Codec_GetImageCapability_3400
 * @tc.name   : testCodecGetImageCapability011
 * @tc.desc   : Verify that the GetImageCapability function parameter CodecImageCapability
 *              structure ->role = CODEC_IMAGE_JPEG returns a successful result
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability011, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].role = CODEC_IMAGE_JPEG;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_GetImageCapability_3500
 * @tc.name   : testCodecGetImageCapability012
 * @tc.desc   : Verify that the GetImageCapability function parameter CodecImageCapability
 *              structure ->role = CODEC_IMAGE_HEIF returns a successful result
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability012, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].role = CODEC_IMAGE_HEIF;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_GetImageCapability_3600
 * @tc.name   : testCodecGetImageCapability013
 * @tc.desc   : Verify that the GetImageCapability function parameter CodecImageCapability
 *              structure ->role = CODEC_IMAGE_INVALID returns a successful result
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability013, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].role = CODEC_IMAGE_INVALID;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_GetImageCapability_3700
 * @tc.name   : testCodecGetImageCapability014
 * @tc.desc   : Verify that the GetImageCapability function argument CodecImageCapability
 *              structure ->minWidth = 0 returns a successful result
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability014, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].minWidth = 0;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_GetImageCapability_3800
 * @tc.name   : testCodecGetImageCapability015
 * @tc.desc   : Verify that when the GetImageCapability function argument CodecImageCapability
 *              structure ->minWidth = 1000, the result is successful
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability015, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].minWidth = 1000;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_GetImageCapability_3900
 * @tc.name   : testCodecGetImageCapability016
 * @tc.desc   : Verify that when the GetImageCapability function argument CodecImageCapability
 *              structure ->minHeight = 0, the result is successful
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecGetImageCapability016, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList(1);
    capList[0].minHeight = 0;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_AllocateInBuffer_4000
 * @tc.name   : testCodecAllocateInBuffer007
 * @tc.desc   : Verify that the AllocateInBuffer function returns a failure
 *              when the second argument is 50 * 1024 * 1024 +1
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer007, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, CODEC_IMAGE_MAX_BUFFER_SIZE + 1, CODEC_IMAGE_JPEG);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_AllocateInBuffer_4100
 * @tc.name   : testCodecAllocateInBuffer008
 * @tc.desc   : Verify that failure is returned when the third argument of the AllocateInBuffer
 *              function is CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer008, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_HEIF);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_AllocateInBuffer_4200
 * @tc.name   : testCodecAllocateInBuffer009
 * @tc.desc   : Verify that a failure is returned when the third argument of
 *              the AllocateInBuffer function is CODEC_IMAGE_INVALID
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer009, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_INVALID);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_AllocateInBuffer_4300
 * @tc.name   : testCodecAllocateInBuffer010
 * @tc.desc   : Determines the result of the function with inBuffer.fenceFd = -1 and CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer010, Function | MediumTest | Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.fenceFd = -1;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_HEIF);
    if (ret == HDF_SUCCESS) {
        ret = hdiJpeg_->FreeInBuffer(inBuffer);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } else {
        ASSERT_NE(ret, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Codec_AllocateInBuffer_4400
 * @tc.name   : testCodecAllocateInBuffer011
 * @tc.desc   : Determines the result of the function with inBuffer.buffer = 0 and CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer011, Function | MediumTest | Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.buffer = 0;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_HEIF);
    if (ret == HDF_SUCCESS) {
        ret = hdiJpeg_->FreeInBuffer(inBuffer);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } else {
        ASSERT_NE(ret, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Codec_AllocateInBuffer_4500
 * @tc.name   : testCodecAllocateInBuffer012
 * @tc.desc   : Determines the result of the function with inBuffer.size = -1 and CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer012, Function | MediumTest | Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.size = -1;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_HEIF);
    if (ret == HDF_SUCCESS) {
        ret = hdiJpeg_->FreeInBuffer(inBuffer);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } else {
        ASSERT_NE(ret, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Codec_AllocateInBuffer_4600
 * @tc.name   : testCodecAllocateInBuffer013
 * @tc.desc   : Determines the result of the function with inBuffer.id = -1 and CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecAllocateInBuffer013, Function | MediumTest | Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.id = -1;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_HEIF);
    if (ret == HDF_SUCCESS) {
        ret = hdiJpeg_->FreeInBuffer(inBuffer);
        ASSERT_EQ(ret, HDF_SUCCESS);
    } else {
        ASSERT_NE(ret, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Codec_FreeInBuffer_4300
 * @tc.name   : testCodecFreeInBuffer004
 * @tc.desc   : Verify that the FreeInBuffer function returns a failure when its
 *              entry is the CodecImageBuffer structure ->size = 0
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecFreeInBuffer004, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.size = 0;
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_FreeInBuffer_4400
 * @tc.name   : testCodecFreeInBuffer005
 * @tc.desc   : Verify that the FreeInBuffer function returns a failure when its entry is
 *              CodecImageBuffer structure ->size = 50 * 1024 * 1024 +1
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecFreeInBuffer005, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.size = CODEC_IMAGE_MAX_BUFFER_SIZE + 1;
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_FreeInBuffer_4500
 * @tc.name   : testCodecFreeInBuffer006
 * @tc.desc   : Verify that when the input to the FreeInBuffer function is CodecImageBuffer structure ->size = 1000
 *              bufferRole = CODEC_IMAGE_JPEG, the result returns success
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecFreeInBuffer006, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_EQ(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG), HDF_SUCCESS);
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_FreeInBuffer_4600
 * @tc.name   : testCodecFreeInBuffer007
 * @tc.desc   : Verify that the FreeInBuffer function returns a failure when the entry to the function is
 *              CodecImageBuffer structure ->bufferRole = CODEC_IMAGE_HEIF
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecFreeInBuffer007, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_HEIF), HDF_SUCCESS);
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Codec_FreeInBuffer_4700
 * @tc.name   : testCodecFreeInBuffer008
 * @tc.desc   : Verify that the FreeInBuffer function returns a failure when the entry to the function is
 *              CodecImageBuffer structure ->bufferRole = CODEC_IMAGE_INVALID
 */
HWTEST_F(CodecHdiJpegTestAdditional, testCodecFreeInBuffer008, TestSize.Level2)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    ASSERT_NE(hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_INVALID), HDF_SUCCESS);
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_NE(ret, HDF_SUCCESS);
}
} // namespace