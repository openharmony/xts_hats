/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "display_buffer_mt.h"
#include "hdf_base.h"
#include "hdf_log.h"
#include "v1_0/display_composer_type.h"
#include "gtest/gtest.h"
#include <cinttypes>
#include <cstring>
#include <securec.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace OHOS {
namespace HDI {
namespace Display {
namespace TEST {
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS;
using namespace testing::ext;

static IDisplayBuffer *g_dispbuf = nullptr;
static const int SIZE_TIMES = 3;
static const int HANDLE_NUM_1 = 2;
static const int HANDLE_NUM_2 = 32;
static const int LOOP_COUNT = 10;

#define HDF_LOG_TAG display_buffer_module_test
static void WriteBuffer(const BufferHandle &handle)
{
    const char verifyMsg[] = "12345678, (*~*)";
    int strSize = strlen(verifyMsg) + 1;
    int i = 0;
    char *ptr = reinterpret_cast<char *>(handle.virAddr);
    if (ptr == nullptr) {
        HDF_LOGE("cast ptr failed");
        return;
    }

    for (; i < handle.size - strSize;) {
        errno_t ret = memcpy_s(&ptr[i], sizeof(verifyMsg), verifyMsg, sizeof(verifyMsg));
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

    HDF_LOGD("%{public}s(%{public}d), buffer size:%{public}d, len:%{public}d", __func__, __LINE__, handle.size,
             strSize);
    for (i = 0; i < handle.size - SIZE_TIMES * strSize; i = i + strSize) {
        if (strncmp(verifyMsg, &ptr[i], sizeof(verifyMsg)) != 0) {
            HDF_LOGD("%{public}s(%{public}d), buffer[%{public}d]:%{public}s", __func__, __LINE__, i, &ptr[i]);
        }
    }

    for (i = handle.size - HANDLE_NUM_1; i < (handle.size - HANDLE_NUM_2); i++) {
        HDF_LOGD("%{public}s(%{public}d), i:%{public}d, str:%{public}s", __func__, __LINE__, i, &ptr[i]);
    }
}

static void DumpBufferHandle(const BufferHandle &handle)
{
    HDF_LOGD("-------------------------------------");
    HDF_LOGD("fd                =%{public}d", handle.fd);
    HDF_LOGD("width             =%{public}d", handle.width);
    HDF_LOGD("stride            =%{public}d", handle.stride);
    HDF_LOGD("height            =%{public}d", handle.height);
    HDF_LOGD("size              =0x%{public}x", handle.size);
    HDF_LOGD("format            =%{public}d", handle.format);
    HDF_LOGD("usage             =0x%{public}" PRIx64 "", handle.usage);
    HDF_LOGD("reserveFds        =%{public}d", handle.reserveFds);
    HDF_LOGD("reserveInts       =%{public}d", handle.reserveInts);
    HDF_LOGD("-------------------------------------");
}

int32_t DisplayBufferMtAdditional::RunTest(const AllocInfo &info)
{
    BufferHandle *bHandle = nullptr;
    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    if (ec != HDF_SUCCESS || bHandle == nullptr) {
        HDF_LOGE("%{public}s, line=%{public}d, AllocMem failed. ec=0x%{public}x", __func__, __LINE__, ec);
        return HDF_FAILURE;
    }

    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        HDF_LOGE("Mmap failed.");
        g_dispbuf->FreeMem(*bHandle);
        return HDF_FAILURE;
    }
    HDF_LOGD("Mmap successful");

    DumpBufferHandle(*bHandle);
    WriteBuffer(*bHandle);

    ec = g_dispbuf->InvalidateCache(*bHandle);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("InvalidateCache failed.");
        g_dispbuf->Unmap(*bHandle);
        g_dispbuf->FreeMem(*bHandle);
        return HDF_FAILURE;
    }
    ec = g_dispbuf->FlushCache(*bHandle);
    if (ec != HDF_SUCCESS) {
        HDF_LOGE("flushCache failed.");
        g_dispbuf->Unmap(*bHandle);
        g_dispbuf->FreeMem(*bHandle);
        return HDF_FAILURE;
    }
    HDF_LOGD("flush Cache success.");
    g_dispbuf->Unmap(*bHandle);
    g_dispbuf->FreeMem(*bHandle);
    return HDF_SUCCESS;
}

void DisplayBufferMtAdditional::SetUpTestCase()
{
    g_dispbuf = IDisplayBuffer::Get();
    ASSERT_TRUE(g_dispbuf != nullptr);
}

/**
 * @tc.number : SUB_Driver_Display_8300
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo001
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo001, TestSize.Level2)
{
    AllocInfo info;
    info.width = SIZE_MAX;
    info.height = SIZE_MAX;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBA_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_8400
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo002
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo002, TestSize.Level2)
{
    AllocInfo info;
    info.width = -1;
    info.height = -1;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBA_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_8500
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo003
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo003, TestSize.Level2)
{
    AllocInfo info;
    info.width = 0;
    info.height = 0;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBA_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_8600
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo004
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo004, TestSize.Level2)
{
    AllocInfo *info = nullptr;
    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(*info);
        EXPECT_EQ(HDF_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_8700
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo005
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo005, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBX_4444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_8800
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo006
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo006, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBA_4444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_8900
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo007
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo007, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGB_888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9000
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo008
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo008, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BGR_565;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9100
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo009
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo009, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BGRX_4444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9200
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo010
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo010, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BGRA_4444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9300
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo011
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo011, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BGRX_5551;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9400
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo012
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo012, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BGRA_5551;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9500
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo013
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo013, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BGRX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9600
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo014
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo014, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BGRA_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9700
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo015
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo015, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YCBCR_420_SP;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9800
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo016
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo016, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YCRCB_420_SP;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_9900
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo017
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo017, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YCBCR_420_P;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0010
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo018
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo018, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_YCRCB_420_P;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0020
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo019
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo019, TestSize.Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                 OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_RGBA_1010102;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

#ifdef DISPLAY_COMMUNITY
/**
 * @tc.number : SUB_Driver_Display_0030
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo020
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo020, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_CLUT8;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0040
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo021
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo021, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_CLUT1;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0050
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo022
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo022, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_CLUT4;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0060
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo023
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo023, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGB_565;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0070
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo024
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo024, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGBA_5658;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0080
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo025
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo025, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGBX_4444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0090
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo026
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo026, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGBA_4444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0100
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo027
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo027, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGB_444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0110
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo028
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo028, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGBX_5551;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0120
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo029
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo029, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGBA_5551;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0130
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo030
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo030, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGB_555;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0140
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo031
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo031, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGBA_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0150
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo032
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo032, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGB_888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0160
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo033
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo033, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_BGR_565;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0170
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo034
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo034, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_BGRX_4444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0180
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo035
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo035, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_BGRA_4444;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0190
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo036
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo036, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_BGRX_5551;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0200
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo037
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo037, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_BGRA_5551;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0210
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo038
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo038, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_BGRX_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0220
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo039
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo039, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_BGRA_8888;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0230
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo040
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo040, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YUV_422_I;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0240
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo041
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo041, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YCBCR_422_SP;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0250
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo042
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo042, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YCRCB_422_SP;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0260
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo043
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo043, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YCBCR_420_SP;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0270
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo044
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo044, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YCRCB_420_SP;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0280
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo045
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo045, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YCBCR_422_P;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0290
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo046
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo046, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YCRCB_422_P;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0300
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo047
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo047, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YCBCR_420_P;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0310
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo048
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo048, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YCRCB_420_P;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0320
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo049
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo049, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YUYV_422_PKG;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0330
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo050
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo050, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_UYVY_422_PKG;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0340
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo051
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo051, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_YVYU_422_PKG;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0350
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo052
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo052, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_VYUY_422_PKG;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0360
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo053
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo053, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_RGBA_1010102;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0370
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo054
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testHatsHdfDisplayBufferMtTestAdditionalAllocInfo054, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_VENDER_MASK;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Display_0380
 * @tc.name   : testHatsHdfDisplayBufferMtTestAdditionalAllocInfo055
 * @tc.desc   : Reliability of function(AllocInfo、FreeMem、Mmap、Unmap、FlushCache、InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, SUB_Driver_Display_0550, TestSize.Level2)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER | OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI1 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI2 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI3 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI4 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI5 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI6 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI7 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI8 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI9 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI10 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI11 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI12 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI13 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI14 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI15 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI16 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI17 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI18 |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI19;
    info.format = PIXEL_FMT_BUTT;

    for (int i = 0; i < LOOP_COUNT; i++) {
        int32_t ret = RunTest(info);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
    }
}

#endif
}
}
}
}