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

#include "display_buffer_mt.h"

#include <unistd.h>
#include <vector>
#include <thread>
#include <cinttypes>
#include <securec.h>
#include <cstring>

#include "gtest/gtest.h"
#include "v1_0/display_composer_type.h"
#include "hdf_base.h"
#include "hdf_log.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace TEST {
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS;
using namespace testing::ext;

static IDisplayBuffer* g_dispbuf = nullptr;
static const int SIZE_TIMES = 3;
static const int HANDLE_NUM_1 = 2;
static const int HANDLE_NUM_2 = 32;
static const int LOOP_COUNT = 10;
static const int INFO_WIDTH = 1024;
static const int INFO_HEIGHT = 1024;

#define HDF_LOG_TAG display_buffer_module_test

static void WriteBuffer(const BufferHandle& handle)
{
    const char VERIFY_MSG[] = "12345678, (*~*)";
    // write msg to display buffer fully.
    int strSize = strlen(VERIFY_MSG) + 1;
    int i = 0;
    char* ptr = reinterpret_cast<char *>(handle.virAddr);
    if (ptr == nullptr) {
        HDF_LOGE("cast ptr failed");
        return;
    }

    for (; i < handle.size - strSize;) {
        errno_t ret = memcpy_s(&ptr[i], sizeof(VERIFY_MSG), VERIFY_MSG, sizeof(VERIFY_MSG));
        if (ret != EOK) {
            HDF_LOGE("memcpy_s error : %d", ret);
            return;
        }
        i += strSize;
        ptr[i - 1] = 0;
    }
    for (i = i - 1; i < handle.size; i++) {
        ptr[i] = 'z';
    }

    // read all contents from buffer
    HDF_LOGD("%{public}s(%{public}d), buffer size:%{public}d, len:%{public}d",
             __func__, __LINE__, handle.size, strSize);
    // verify buffer contents from strings written.
    for (i = 0; i < handle.size - SIZE_TIMES * strSize; i = i + strSize) {
        if (strncmp(VERIFY_MSG, &ptr[i], sizeof(VERIFY_MSG)) != 0) {
            HDF_LOGD("%{public}s(%{public}d), buffer[%{public}d]:%{public}s",
                     __func__, __LINE__, i, &ptr[i]);
        }
    }

    for (i = handle.size - HANDLE_NUM_1; i < (handle.size - HANDLE_NUM_2); i++) {
        HDF_LOGD("%{public}s(%{public}d), i:%{public}d, str:%{public}s",
                 __func__, __LINE__, i, &ptr[i]);
    }
}

static void DumpBufferHandle(const BufferHandle& handle)
{
    // dump buffer handle infomation
    HDF_LOGD("-------------------------------------");
    HDF_LOGD("fd                =%{public}d",   handle.fd);
    HDF_LOGD("width             =%{public}d",   handle.width);
    HDF_LOGD("stride            =%{public}d",   handle.stride);
    HDF_LOGD("height            =%{public}d",   handle.height);
    HDF_LOGD("size              =0x%{public}x", handle.size);
    HDF_LOGD("format            =%{public}d",   handle.format);
    HDF_LOGD("usage             =0x%{public}" PRIx64 "", handle.usage);
    HDF_LOGD("reserveFds        =%{public}d",   handle.reserveFds);
    HDF_LOGD("reserveInts       =%{public}d",   handle.reserveInts);
    HDF_LOGD("-------------------------------------");
}

int32_t DisplayBufferMt::RunTest(const AllocInfo& info)
{
    BufferHandle* bHandle = nullptr;
    // AllocMem
    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    if (ec != HDF_SUCCESS || bHandle == nullptr) {
        HDF_LOGE("%{public}s, line=%{public}d, AllocMem failed. ec=0x%{public}x",
                 __func__, __LINE__, ec);
        return HDF_FAILURE;
    }

    // Mmap
    void* buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        HDF_LOGE("Mmap failed.");
        g_dispbuf->FreeMem(*bHandle);
        return HDF_FAILURE;
    }
    HDF_LOGD("Mmap successful");

    DumpBufferHandle(*bHandle);
    WriteBuffer(*bHandle);

    // InvalidateCache
    ec = g_dispbuf->InvalidateCache(*bHandle);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("InvalidateCache failed.");
        g_dispbuf->Unmap(*bHandle);
        g_dispbuf->FreeMem(*bHandle);
        return HDF_FAILURE;
    }
    // InvalidateCache
    ec = g_dispbuf->FlushCache(*bHandle);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("flushCache failed.");
        g_dispbuf->Unmap(*bHandle);
        g_dispbuf->FreeMem(*bHandle);
        return HDF_FAILURE;
    }
    HDF_LOGD("flush Cache success.");
    // free buffer
    g_dispbuf->Unmap(*bHandle);
    g_dispbuf->FreeMem(*bHandle);
    return HDF_SUCCESS;
}

void DisplayBufferMt::SetUpTestCase()
{
    g_dispbuf = IDisplayBuffer::Get();
    ASSERT_TRUE(g_dispbuf != nullptr);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0010, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBA_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

#ifdef DISPLAY_COMMUNITY
HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0020, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_CLUT8;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0030, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_CLUT1;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0040, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_CLUT4;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0050, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGB_565;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0060, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBA_5658;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0070, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGB_444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0080, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBX_5551;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0090, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBA_5551;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0100, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGB_555;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0110, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YUV_422_I;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0120, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YCBCR_422_SP;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0130, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YCRCB_422_SP;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0140, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YCBCR_422_P;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0150, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YCRCB_422_P;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0160, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YUYV_422_PKG;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0170, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_UYVY_422_PKG;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0180, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YVYU_422_PKG;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0190, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_VYUY_422_PKG;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0200, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_VENDER_MASK;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0210, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BUTT;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0220, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0230, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0240, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0250, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0260, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0270, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0280, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0290, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0300, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0310, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0320, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0330, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0340, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0350, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0360, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0370, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0380, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0390, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0400, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0410, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0420, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0430, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0440, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0450, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0460, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0470, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0480, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0490, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0500, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0510, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0520, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0530, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0540, TestSize.Level1)
{
    AllocInfo info;
    info.width  = INFO_WIDTH;
    info.height = INFO_HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGBX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0550, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_PREPARE_DISPLAY_LAYERS;
    EXPECT_EQ(64, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0560, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_DISPLAY_CLIENT_BUFFER;
    EXPECT_EQ(65, ret);
}


HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0570, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_DISPLAY_CLIENT_DAMAGE;
    EXPECT_EQ(66, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0580, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_COMMIT;
    EXPECT_EQ(67, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0590, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_ALPHA;
    EXPECT_EQ(68, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0600, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_REGION;
    EXPECT_EQ(69, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0610, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_CROP;
    EXPECT_EQ(70, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0620, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_ZORDER;
    EXPECT_EQ(71, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0630, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_PREMULTI;
    EXPECT_EQ(72, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0640, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_TRANSFORM_MODE;
    EXPECT_EQ(73, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0650, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_DIRTY_REGION;
    EXPECT_EQ(74, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0660, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_VISIBLE_REGION;
    EXPECT_EQ(75, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0670, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_BUFFER;
    EXPECT_EQ(76, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0680, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_COMPOSITION_TYPE;
    EXPECT_EQ(77, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0690, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_BLEND_TYPE;
    EXPECT_EQ(78, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0700, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_VISIBLE;
    EXPECT_EQ(79, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0710, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_MASK_INFO;
    EXPECT_EQ(80, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0720, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_SET_LAYER_COLOR;
    EXPECT_EQ(81, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0730, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REQUEST_CMD_BUTT;
    EXPECT_EQ(82, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0740, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REPLY_CMD_SET_ERROR;
    EXPECT_EQ(512, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0750, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REPLY_CMD_PREPARE_DISPLAY_LAYERS;
    EXPECT_EQ(513, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0760, TestSize.Level1)
{    
    int ret;    
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REPLY_CMD_COMMIT;
    EXPECT_EQ(514, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0770, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::REPLY_CMD_BUTT;
    EXPECT_EQ(515, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0780, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::CONTROL_CMD_REQUEST_BEGIN;
    EXPECT_EQ(1024, ret);
}


HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0790, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::CONTROL_CMD_REPLY_BEGIN;
    EXPECT_EQ(1025, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0800, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::CONTROL_CMD_REQUEST_END;
    EXPECT_EQ(1026, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0810, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::CONTROL_CMD_REPLY_END;
    EXPECT_EQ(1027, ret);
}

HWTEST_F(DisplayBufferMt, SUB_DriverSystem_DisplayBuffer_0820, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispCmd::CONTROL_CMD_BUTT;
    EXPECT_EQ(1028, ret);
}

#endif // DISPLAY_COMMUNITY
} // OHOS
} // HDI
} // DISPLAY
} // TEST