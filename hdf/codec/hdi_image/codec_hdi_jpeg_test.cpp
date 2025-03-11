/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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
#include "v2_0/codec_image_type.h"
#include "v2_0/icodec_image.h"
#include "v3_0/codec_types.h"
#define HDF_LOG_TAG codec_jpeg_test

using namespace std;
using namespace testing::ext;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Codec::Image::V2_0;
using namespace OHOS::HDI::Codec::V3_0;
namespace {
constexpr int32_t WIDTH = 640;
constexpr int32_t HEIGHT = 480;
constexpr uint32_t NORMAL_BUFFER_SIZE = 1000;
constexpr uint32_t CODEC_IMAGE_MAX_BUFFER_SIZE = 50 * 1024 *1024;
static OHOS::sptr<ICodecImage> hdiJpeg_;
static IDisplayBuffer *hdiBuffer_;
class CodecHdiJpegTest : public testing::Test {
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
            auto ret = hdiJpeg_->Init(CODEC_IMAGE_JPEG);
            if (ret != HDF_SUCCESS) {
                GTEST_SKIP() << "jpeg is null!" << std::endl;
                return;
            }
        } else {
            GTEST_SKIP() << "hdiJpeg_ is null!" << std::endl;
            return;
        }
    }
    void TearDown()
    {
        if (hdiJpeg_ != nullptr) {
            hdiJpeg_->DeInit(CODEC_IMAGE_JPEG);
        }
    }
};

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_0100, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    std::vector<CodecImageCapability> capList;
    auto ret = hdiJpeg_->GetImageCapability(capList);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_0200, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, 0, CODEC_IMAGE_JPEG);
    ASSERT_NE(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_0300, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, CODEC_IMAGE_MAX_BUFFER_SIZE + 1, CODEC_IMAGE_JPEG);
    ASSERT_NE(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_0400, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_0500, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    inBuffer.id = -1;
    inBuffer.fenceFd = -1;
    auto ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_0600, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    inBuffer.fenceFd = -1;
    outBuffer.fenceFd = -1;
    auto ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    ASSERT_NE(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_0700, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG);
    ASSERT_EQ(ret, HDF_SUCCESS);

    struct CodecImageBuffer outBuffer;
    struct CodecJpegDecInfo decInfo;
    outBuffer.fenceFd = -1;
    ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    EXPECT_TRUE(ret != HDF_SUCCESS);
    ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_0800, TestSize.Level1)
{
    ASSERT_TRUE(hdiJpeg_ != nullptr);
    struct CodecImageBuffer inBuffer;
    auto ret = hdiJpeg_->AllocateInBuffer(inBuffer, NORMAL_BUFFER_SIZE, CODEC_IMAGE_JPEG);
    ASSERT_EQ(ret, HDF_SUCCESS);

    ASSERT_TRUE(hdiBuffer_ != nullptr);
    struct CodecImageBuffer outBuffer;
    InitOutBuffer(outBuffer);
    outBuffer.fenceFd = -1;

    struct CodecJpegDecInfo decInfo;
    ret = hdiJpeg_->DoJpegDecode(inBuffer, outBuffer, decInfo);
    EXPECT_TRUE(ret != HDF_SUCCESS);
    ret = hdiJpeg_->FreeInBuffer(inBuffer);
    ASSERT_EQ(ret, HDF_SUCCESS);
}

#ifdef DISPLAY_COMMUNITY
HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_0100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest001: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::VIDEO_DECODER == 0);
    printf("HdfCodecHdiEnumTest001: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_0200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest002: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::VIDEO_ENCODER == 1);
    printf("HdfCodecHdiEnumTest002: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_0300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest003: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_DECODER == 2);
    printf("HdfCodecHdiEnumTest003: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_0400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest004: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_ENCODER == 3);
    printf("HdfCodecHdiEnumTest004: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_0500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest005: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::INVALID_TYPE == 4);
    printf("HdfCodecHdiEnumTest005: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_0600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest006: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_IMAGE_JPEG == 0);
    printf("HdfCodecHdiEnumTest006: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_0700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest007: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_VIDEO_AVC == 1);
    printf("HdfCodecHdiEnumTest007: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_0800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest008: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_VIDEO_HEVC == 2);
    printf("HdfCodecHdiEnumTest008: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_0900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest009: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_AUDIO_FIRST == 0x10000);
    printf("HdfCodecHdiEnumTest009: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1000, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest010: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_AUDIO_AAC == 0x10000);
    printf("HdfCodecHdiEnumTest010: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest011: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_AUDIO_G711A == (0x10000) + 1);
    printf("HdfCodecHdiEnumTest011: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest012: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_AUDIO_G711U == (0x10000) + 2);
    printf("HdfCodecHdiEnumTest012: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest013: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_AUDIO_G726 == (0x10000) + 3);
    printf("HdfCodecHdiEnumTest013: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest014: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_AUDIO_PCM == (0x10000) + 4);
    printf("HdfCodecHdiEnumTest014: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest015: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_AUDIO_MP3 == (0x10000) + 5);
    printf("HdfCodecHdiEnumTest015: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest016: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::MEDIA_ROLETYPE_INVALID == (0x10000) + 6);
    printf("HdfCodecHdiEnumTest016: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest017: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::INVALID_PROFILE == 0);
    printf("HdfCodecHdiEnumTest017: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest018: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AAC_LC_PROFILE == 0x1000);
    printf("HdfCodecHdiEnumTest018: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_1900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest019: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AAC_MAIN_PROFILE == (0x1000) + 1);
    printf("HdfCodecHdiEnumTest019: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2000, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest020: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AAC_HE_V1_PROFILE == (0x1000) + 2);
    printf("HdfCodecHdiEnumTest020: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest021: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AAC_HE_V2_PROFILE == (0x1000) + 3);
    printf("HdfCodecHdiEnumTest021: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest022: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AAC_LD_PROFILE == (0x1000) + 4);
    printf("HdfCodecHdiEnumTest022: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest023: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AAC_ELD_PROFILE == (0x1000) + 5);
    printf("HdfCodecHdiEnumTest023: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest024: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AVC_BASELINE_PROFILE == (0x2000));
    printf("HdfCodecHdiEnumTest024: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest025: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AVC_MAIN_PROFILE == (0x2000) + 1);
    printf("HdfCodecHdiEnumTest025: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest026: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AVC_HIGH_PROFILE == (0x2000) + 2);
    printf("HdfCodecHdiEnumTest026: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest027: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::HEVC_MAIN_PROFILE == (0x3000));
    printf("HdfCodecHdiEnumTest027: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest028: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::HEVC_MAIN_10_PROFILE == (0x3000) + 1);
    printf("HdfCodecHdiEnumTest028: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_2900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest029: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_CAP_ADAPTIVE_PLAYBACK == 0x1);
    printf("HdfCodecHdiEnumTest029: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3000, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest030: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_CAP_SECURE_PLAYBACK == 0x2);
    printf("HdfCodecHdiEnumTest030: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest031: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_CAP_TUNNEL_PLAYBACK == 0x4);
    printf("HdfCodecHdiEnumTest031: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest032: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_CAP_MULTI_PLANE == 0x10000);
    printf("HdfCodecHdiEnumTest032: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest033: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_8000 == 8000);
    printf("HdfCodecHdiEnumTest033: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest034: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_12000 == 12000);
    printf("HdfCodecHdiEnumTest034: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest035: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_11025 == 11025);
    printf("HdfCodecHdiEnumTest035: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest036: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_16000 == 16000);
    printf("HdfCodecHdiEnumTest036: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest037: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_22050 == 22050);
    printf("HdfCodecHdiEnumTest037: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest038: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_24000 == 24000);
    printf("HdfCodecHdiEnumTest038: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_3900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest039: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_32000 == 32000);
    printf("HdfCodecHdiEnumTest039: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4000, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest040: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_44100 == 44100);
    printf("HdfCodecHdiEnumTest040: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest041: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_48000 == 48000);
    printf("HdfCodecHdiEnumTest041: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest042: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_64000 == 64000);
    printf("HdfCodecHdiEnumTest042: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest043: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_96000 == 96000);
    printf("HdfCodecHdiEnumTest043: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest044: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUD_SAMPLE_RATE_INVALID == 96000 + 1);
    printf("HdfCodecHdiEnumTest044: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest045: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_U8 == 0);
    printf("HdfCodecHdiEnumTest045: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest046: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_S16 == 1);
    printf("HdfCodecHdiEnumTest046: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest047: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_S32 == 2);
    printf("HdfCodecHdiEnumTest047: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest048: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_FLOAT == 3);
    printf("HdfCodecHdiEnumTest048: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_4900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest049: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_DOUBLE == 4);
    printf("HdfCodecHdiEnumTest049: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5000, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest050: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_U8P == 5);
    printf("HdfCodecHdiEnumTest050: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest051: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_S16P == 6);
    printf("HdfCodecHdiEnumTest051: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest052: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_S32P == 7);
    printf("HdfCodecHdiEnumTest052: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest053: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_FLOATP == 8);
    printf("HdfCodecHdiEnumTest053: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest054: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_DOUBLEP == 9);
    printf("HdfCodecHdiEnumTest054: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest055: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::AUDIO_SAMPLE_FMT_INVALID == 10);
    printf("HdfCodecHdiEnumTest055: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest056: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::PROCESS_BLOCKING_INPUT_BUFFER == 0x1);
    printf("HdfCodecHdiEnumTest056: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest057: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::PROCESS_BLOCKING_OUTPUT_BUFFER == 0x2);
    printf("HdfCodecHdiEnumTest057: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest058: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::PROCESS_BLOCKING_CONTROL_FLOW == 0x4);
    printf("HdfCodecHdiEnumTest058: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_5900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest059: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::PROCESS_NONBLOCKING_INPUT_BUFFER == 0x100);
    printf("HdfCodecHdiEnumTest059: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6000, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest060: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::PROCESS_NONBLOCKING_OUTPUT_BUFFER == 0x200);
    printf("HdfCodecHdiEnumTest060: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest061: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::PROCESS_NONBLOCKING_CONTROL_FLOW == 0x400);
    printf("HdfCodecHdiEnumTest061: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest062: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::READ_WRITE_TYPE == 0x1);
    printf("HdfCodecHdiEnumTest062: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest063: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::READ_ONLY_TYPE == 0x2);
    printf("HdfCodecHdiEnumTest063: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest064: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::BIT_RATE_MODE_INVALID == 0);
    printf("HdfCodecHdiEnumTest064: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest065: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::BIT_RATE_MODE_VBR == 1);
    printf("HdfCodecHdiEnumTest065: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest066: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::BIT_RATE_MODE_CBR == 2);
    printf("HdfCodecHdiEnumTest066: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest067: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::BIT_RATE_MODE_CQ == 3);
    printf("HdfCodecHdiEnumTest067: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest068: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::BIT_RATE_MODE_VCBR == 4);
    printf("HdfCodecHdiEnumTest068: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_6900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest069: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::BIT_RATE_MODE_ABR == 5);
    printf("HdfCodecHdiEnumTest069: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7000, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest070: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_CMD_COMPLETE == 0);
    printf("HdfCodecHdiEnumTest070: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest071: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_ERROR == 1);
    printf("HdfCodecHdiEnumTest071: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest072: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_MARK == 2);
    printf("HdfCodecHdiEnumTest072: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest073: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_PORT_SETTINGS_CHANGED == 3);
    printf("HdfCodecHdiEnumTest073: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest074: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_BUFFER_FLAG == 4);
    printf("HdfCodecHdiEnumTest074: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest075: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_RESOURCES_ACQUIRED == 5);
    printf("HdfCodecHdiEnumTest075: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest076: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_COMPONENT_RESUMED == 6);
    printf("HdfCodecHdiEnumTest076: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest077: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_DYNAMIC_RESOURCES_AVAILABLE == 7);
    printf("HdfCodecHdiEnumTest077: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest078: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_PORT_FORMAT_DETECTED == 8);
    printf("HdfCodecHdiEnumTest078: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_7900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest079: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_KHRONOS_EXTENSIONS == 0x6F000000);
    printf("HdfCodecHdiEnumTest079: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8000, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest080: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_VENDOR_START_UNUSED == 0x7F000000);
    printf("HdfCodecHdiEnumTest080: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest081: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_EVENT_MAX == 0x7FFFFFFF);
    printf("HdfCodecHdiEnumTest081: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest082: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_COMMAND_STATE_SET == 0);
    printf("HdfCodecHdiEnumTest082: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest083: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_COMMAND_FLUSH == 1);
    printf("HdfCodecHdiEnumTest083: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest084: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_COMMAND_PORT_DISABLE == 2);
    printf("HdfCodecHdiEnumTest084: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest085: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_COMMAND_PORT_ENABLE == 3);
    printf("HdfCodecHdiEnumTest085: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest086: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_COMMAND_MARK_BUFFER == 4);
    printf("HdfCodecHdiEnumTest086: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest087: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_COMMAND_KHRONOS_EXTENSIONS == 0x6F000000);
    printf("HdfCodecHdiEnumTest087: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest088: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_COMMAND_VENDOR_START_UNUSED == 0x7F000000);
    printf("HdfCodecHdiEnumTest088: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_8900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest089: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_COMMAND_MAX == 0x7FFFFFFF);
    printf("HdfCodecHdiEnumTest089: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9000, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest090: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_STATE_INVALID == 0);
    printf("HdfCodecHdiEnumTest090: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest091: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_STATE_LOADED == 1);
    printf("HdfCodecHdiEnumTest091: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest092: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_STATE_IDLE == 2);
    printf("HdfCodecHdiEnumTest092: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest093: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_STATE_EXECUTING == 3);
    printf("HdfCodecHdiEnumTest093: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest094: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_STATE_PAUSE == 4);
    printf("HdfCodecHdiEnumTest094: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest095: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_STATE_WAIT_FOR_RESOURCES == 5);
    printf("HdfCodecHdiEnumTest095: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9600, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest096: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_STATE_KHRONOS_EXTENSIONS == 0x6F000000);
    printf("HdfCodecHdiEnumTest096: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9700, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest097: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_STATE_VENDOR_START_UNUSED == 0x7F000000);
    printf("HdfCodecHdiEnumTest097: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9800, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest098: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_STATE_MAX == 0x7FFFFFFF);
    printf("HdfCodecHdiEnumTest098: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest_9900, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest099: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_BUFFER_SUPPLY_UNSPECIFIED == 0);
    printf("HdfCodecHdiEnumTest099: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest1_0100, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest100: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_BUFFER_SUPPLY_INPUT == 1);
    printf("HdfCodecHdiEnumTest100: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest1_0200, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest101: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_BUFFER_SUPPLY_OUTPUT == 2);
    printf("HdfCodecHdiEnumTest101: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest1_0300, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest102: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_BUFFER_SUPPLY_KHRONOS_EXTENSIONS == 0x6F000000);
    printf("HdfCodecHdiEnumTest102: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest1_0400, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest103: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_BUFFER_SUPPLY_VENDOR_START_UNUSED == 0x7F000000);
    printf("HdfCodecHdiEnumTest103: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiEnumTest1_0500, TestSize.Level3)
{
    printf("HdfCodecHdiEnumTest104: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::V3_0::CODEC_BUFFER_SUPPLY_MAX == 0x7FFFFFFF);
    printf("HdfCodecHdiEnumTest104: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiJpegEnumTest_0100, TestSize.Level3)
{
    printf("HdfCodecHdiJpegEnumTest001: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::Image::V2_0::CODEC_IMAGE_JPEG == 0);
    printf("HdfCodecHdiJpegEnumTest001: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiJpegEnumTest_0200, TestSize.Level3)
{
    printf("HdfCodecHdiJpegEnumTest002: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::Image::V2_0::CODEC_IMAGE_HEIF == 1);
    printf("HdfCodecHdiJpegEnumTest002: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiJpegEnumTest_0300, TestSize.Level3)
{
    printf("HdfCodecHdiJpegEnumTest003: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::Image::V2_0::CODEC_IMAGE_INVALID == 2);
    printf("HdfCodecHdiJpegEnumTest003: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiJpegEnumTest_0400, TestSize.Level3)
{
    printf("HdfCodecHdiJpegEnumTest004: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::Image::V2_0::CODEC_IMAGE_TYPE_DECODER == 0);
    printf("HdfCodecHdiJpegEnumTest004: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiJpegEnumTest_0500, TestSize.Level3)
{
    printf("HdfCodecHdiJpegEnumTest005: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::Image::V2_0::CODEC_IMAGE_TYPE_ENCODER == 1);
    printf("HdfCodecHdiJpegEnumTest005: end.");
}

HWTEST_F(CodecHdiJpegTest, SUB_Driver_Codec_ImageCodecHDI_HdiJpegEnumTest_0600, TestSize.Level3)
{
    printf("HdfCodecHdiJpegEnumTest006: start.");
    EXPECT_TRUE(OHOS::HDI::Codec::Image::V2_0::CODEC_IMAGE_TYPE_INVALID == 2);
    printf("HdfCodecHdiJpegEnumTest006: end.");
}
#endif
}  // namespace