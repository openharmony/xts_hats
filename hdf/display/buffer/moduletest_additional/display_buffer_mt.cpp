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
static const int MAX_VALUE = 1024;
static const int MIN_VALUE = 1;
static const int SLOTID_1 = 0;

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

int32_t DisplayBufferMtAdditional::AfterCache(BufferHandle *handle)
{
    // free buffer
    g_dispbuf->Unmap(*handle);
    g_dispbuf->FreeMem(*handle);
    return HDF_SUCCESS;
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_0100
 * @tc.name   : testDisplayBufferMmap001
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap001, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_0200
 * @tc.name   : testDisplayBufferMmap002
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap002, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_0300
 * @tc.name   : testDisplayBufferMmap003
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap003, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_0400
 * @tc.name   : testDisplayBufferMmap004
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap004, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_0500
 * @tc.name   : testDisplayBufferMmap005
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap005, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_0600
 * @tc.name   : testDisplayBufferMmap006
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap006, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_0700
 * @tc.name   : testDisplayBufferMmap007
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap007, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_0800
 * @tc.name   : testDisplayBufferMmap008
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap008, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_0900
 * @tc.name   : testDisplayBufferMmap009
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap009, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1000
 * @tc.name   : testDisplayBufferMmap010
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap010, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1100
 * @tc.name   : testDisplayBufferMmap011
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap011, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1200
 * @tc.name   : testDisplayBufferMmap012
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap012, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1300
 * @tc.name   : testDisplayBufferMmap013
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap013, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED;
    info.format = PIXEL_FMT_RGBA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1400
 * @tc.name   : testDisplayBufferMmap014
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap014, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1500
 * @tc.name   : testDisplayBufferMmap015
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap015, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1600
 * @tc.name   : testDisplayBufferMmap016
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap016, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1700
 * @tc.name   : testDisplayBufferMmap017
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap017, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1800
 * @tc.name   : testDisplayBufferMmap018
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap018, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_1900
 * @tc.name   : testDisplayBufferMmap019
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap019, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2000
 * @tc.name   : testDisplayBufferMmap020
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap020, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2100
 * @tc.name   : testDisplayBufferMmap021
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap021, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBA_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2200
 * @tc.name   : testDisplayBufferMmap022
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap022, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2300
 * @tc.name   : testDisplayBufferMmap023
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap023, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2400
 * @tc.name   : testDisplayBufferMmap024
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap024, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGB_888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2500
 * @tc.name   : testDisplayBufferMmap025
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap025, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGR_565;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2600
 * @tc.name   : testDisplayBufferMmap026
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap026, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2700
 * @tc.name   : testDisplayBufferMmap027
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap027, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2800
 * @tc.name   : testDisplayBufferMmap028
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap028, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRX_5551;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_2900
 * @tc.name   : testDisplayBufferMmap029
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap029, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_5551;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_3000
 * @tc.name   : testDisplayBufferMmap030
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap030, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRX_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_3100
 * @tc.name   : testDisplayBufferMmap031
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap031, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_3200
 * @tc.name   : testDisplayBufferMmap032
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap032, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_422_SP;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_3300
 * @tc.name   : testDisplayBufferMmap033
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap033, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_422_SP;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_3400
 * @tc.name   : testDisplayBufferMmap034
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap034, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_420_SP;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_3500
 * @tc.name   : testDisplayBufferMmap035
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap035, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_420_SP;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_3600
 * @tc.name   : testDisplayBufferMmap036
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap036, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_422_P;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_3700
 * @tc.name   : testDisplayBufferMmap037
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap037, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_422_P;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Mmap_3800
 * @tc.name   : testDisplayBufferMmap038
 * @tc.desc   : Reliability of function(Mmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferMmap038, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_420_P;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_Unmap_0100
 * @tc.name   : testDisplayBufferUnmap001
 * @tc.desc   : Reliability of function(Unmap)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferUnmap001, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_420_P;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_NE(nullptr, buffer);
    int32_t ret = g_dispbuf->Unmap(*bHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_0100
 * @tc.name  : testDisplayBufferAllocMem001
 * @tc.desc  : Verify the reliability of the AllocMem function info.width as SLOTID_1.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem001, Function | MediumTest | Level2)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = SLOTID_1;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_4444;
    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_NE(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_0200
 * @tc.name  : testDisplayBufferAllocMem002
 * @tc.desc  : Verify the reliability of the AllocMem function info.width as MAX_VALUE.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem002, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = MAX_VALUE;
    info.height = MAX_VALUE;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_0300
 * @tc.name  : testDisplayBufferAllocMem003
 * @tc.desc  : Verify the reliability of the AllocMem function info.width as MIN_VALUE.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem003, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = MIN_VALUE;
    info.height = MAX_VALUE;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_0400
 * @tc.name  : testDisplayBufferAllocMem004
 * @tc.desc  : Verify the reliability of the AllocMem function info.height as SLOTID_1.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem004, Function | MediumTest | Level2)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.height = SLOTID_1;
    info.width = MAX_VALUE;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_NE(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_0500
 * @tc.name  : testDisplayBufferAllocMem005
 * @tc.desc  : Verify the reliability of the AllocMem function info.height as MAX_VALUE.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem005, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_0600
 * @tc.name  : testDisplayBufferAllocMem006
 * @tc.desc  : Verify the reliability of the AllocMem function info.height as MIN_VALUE.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem006, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.height = MIN_VALUE;
    info.width = MAX_VALUE;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_0700
 * @tc.name  : testDisplayBufferAllocMem007
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_CPU_READ.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem007, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_0800
 * @tc.name  : testDisplayBufferAllocMem008
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_CPU_WRITE.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem008, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_0900
 * @tc.name  : testDisplayBufferAllocMem009
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_MEM_MMZ.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem009, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1000
 * @tc.name  : testDisplayBufferAllocMem010
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_MEM_DMA.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem010, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1100
 * @tc.name  : testDisplayBufferAllocMem011
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_MEM_SHARE.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem011, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1200
 * @tc.name  : testDisplayBufferAllocMem012
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_MEM_MMZ_CACHE.
 */

HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem012, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ_CACHE;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1300
 * @tc.name  : testDisplayBufferAllocMem013
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_MEM_FB.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem013, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_FB;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1400
 * @tc.name  : testDisplayBufferAllocMem014
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_ASSIGN_SIZE.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem014, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_ASSIGN_SIZE;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1500
 * @tc.name  : testDisplayBufferAllocMem015
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_HW_RENDER.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem015, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_RENDER;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1600
 * @tc.name  : testDisplayBufferAllocMem016
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_HW_TEXTURE.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem016, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_TEXTURE;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1700
 * @tc.name  : testDisplayBufferAllocMem017
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_HW_COMPOSER.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem017, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1800
 * @tc.name  : testDisplayBufferAllocMem018
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_PROTECTED.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem018, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_PROTECTED;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_1900
 * @tc.name  : testDisplayBufferAllocMem019
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_CAMERA_READ.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem019, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_READ;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2000
 * @tc.name  : testDisplayBufferAllocMem020
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_CAMERA_WRITE.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem020, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CAMERA_WRITE;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2100
 * @tc.name  : testDisplayBufferAllocMem021
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_VIDEO_ENCODER.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem021, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_ENCODER;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2200
 * @tc.name  : testDisplayBufferAllocMem022
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_VIDEO_DECODER.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem022, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VIDEO_DECODER;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2300
 * @tc.name  : testDisplayBufferAllocMem023
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_CPU_READ_OFTEN.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem023, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ_OFTEN;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2400
 * @tc.name  : testDisplayBufferAllocMem024
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_VENDOR_PRI0.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem024, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2500
 * @tc.name  : testDisplayBufferAllocMem025
 * @tc.desc  : Verify the reliability of the AllocMem function info.usage as HBM_USE_VENDOR_PRI0.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem025, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    int32_t ec = 0;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    for (int i = 0; i < 100; i++) {
        ec = g_dispbuf->AllocMem(info, bHandle);
        EXPECT_EQ(HDF_SUCCESS, ec);
        g_dispbuf->FreeMem(*bHandle);
    }
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2600
 * @tc.name  : testDisplayBufferAllocMem026
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_RGBX_4444.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem026, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2700
 * @tc.name  : testDisplayBufferAllocMem027
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_RGBA_4444.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem027, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBA_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2800
 * @tc.name  : testDisplayBufferAllocMem028
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_RGBX_8888.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem028, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBX_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_2900
 * @tc.name  : testDisplayBufferAllocMem029
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_RGBA_8888.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem029, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGBA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3000
 * @tc.name  : testDisplayBufferAllocMem030
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_RGB_888.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem030, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_RGB_888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3100
 * @tc.name  : testDisplayBufferAllocMem031
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_BGR_565.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem031, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_BGR_565;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3200
 * @tc.name  : testDisplayBufferAllocMem032
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_BGRX_4444.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem032, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_BGRX_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3300
 * @tc.name  : testDisplayBufferAllocMem033
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_BGRA_4444.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem033, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_BGRA_4444;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3400
 * @tc.name  : testDisplayBufferAllocMem034
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_BGRX_5551.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem034, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_BGRX_5551;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3500
 * @tc.name  : testDisplayBufferAllocMem035
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_BGRA_5551.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem035, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_BGRA_5551;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3600
 * @tc.name  : testDisplayBufferAllocMem036
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_BGRX_8888.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem036, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_BGRX_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3700
 * @tc.name  : testDisplayBufferAllocMem037
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_BGRA_8888.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem037, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_BGRA_8888;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3800
 * @tc.name  : testDisplayBufferAllocMem038
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_YCBCR_422_SP.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem038, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_YCBCR_422_SP;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_3900
 * @tc.name  : testDisplayBufferAllocMem039
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_YCRCB_422_SP.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem039, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_YCRCB_422_SP;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_4000
 * @tc.name  : testDisplayBufferAllocMem040
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_YCBCR_420_SP.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem040, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_YCBCR_420_SP;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_4100
 * @tc.name  : testDisplayBufferAllocMem041
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_YCRCB_420_SP.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem041, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_YCRCB_420_SP;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_4200
 * @tc.name  : testDisplayBufferAllocMem042
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_YCBCR_422_P.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem042, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_YCBCR_422_P;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_4300
 * @tc.name  : testDisplayBufferAllocMem043
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_YCRCB_422_P.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem043, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_YCRCB_422_P;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number: SUB_Driver_Display_Buffer_AllocMem_4400
 * @tc.name  : testDisplayBufferAllocMem044
 * @tc.desc  : Verify the reliability of the AllocMem function info.format as PIXEL_FMT_YCBCR_420_P.
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferAllocMem044, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.height = MAX_VALUE;
    info.width = MAX_VALUE;
    info.format = PIXEL_FMT_YCBCR_420_P;

    int32_t ec = g_dispbuf->AllocMem(info, bHandle);
    EXPECT_EQ(HDF_SUCCESS, ec);
    g_dispbuf->FreeMem(*bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_0100
 * @tc.name   : testDisplayBufferFlushCache001
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache001, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_4444;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_0200
 * @tc.name   : testDisplayBufferFlushCache002
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache002, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBA_4444;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_0300
 * @tc.name   : testDisplayBufferFlushCache003
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache003, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_8888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_0400
 * @tc.name   : testDisplayBufferFlushCache004
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache004, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBA_8888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_0500
 * @tc.name   : testDisplayBufferFlushCache005
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache005, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGB_888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_0600
 * @tc.name   : testDisplayBufferFlushCache006
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache006, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGR_565;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_0700
 * @tc.name   : testDisplayBufferFlushCache007
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache007, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRX_4444;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_0800
 * @tc.name   : testDisplayBufferFlushCache008
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache008, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_4444;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_0900
 * @tc.name   : testDisplayBufferFlushCache009
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache009, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRX_5551;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1000
 * @tc.name   : testDisplayBufferFlushCache010
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache010, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_5551;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1100
 * @tc.name   : testDisplayBufferFlushCache011
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache011, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRX_8888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1200
 * @tc.name   : testDisplayBufferFlushCache012
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache012, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_8888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1300
 * @tc.name   : testDisplayBufferFlushCache013
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache013, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_422_SP;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1400
 * @tc.name   : testDisplayBufferFlushCache014
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache014, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_422_SP;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1500
 * @tc.name   : testDisplayBufferFlushCache015
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache015, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_420_SP;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1600
 * @tc.name   : testDisplayBufferFlushCache016
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache016, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_420_SP;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1700
 * @tc.name   : testDisplayBufferFlushCache017
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache017, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_422_P;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1800
 * @tc.name   : testDisplayBufferFlushCache018
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache018, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_422_P;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_1900
 * @tc.name   : testDisplayBufferFlushCache019
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache019, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_420_P;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FlushCache_2000
 * @tc.name   : testDisplayBufferFlushCache020
 * @tc.desc   : Reliability of function(FlushCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFlushCache020, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_420_P;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->FlushCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_0100
 * @tc.name   : testDisplayBufferInvalidateCache001
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache001, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_4444;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_0200
 * @tc.name   : testDisplayBufferInvalidateCache002
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache002, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBA_4444;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_0300
 * @tc.name   : testDisplayBufferInvalidateCache003
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache003, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBX_8888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_0400
 * @tc.name   : testDisplayBufferInvalidateCache004
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache004, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGBA_8888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_0500
 * @tc.name   : testDisplayBufferInvalidateCache005
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache005, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_RGB_888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_0600
 * @tc.name   : testDisplayBufferInvalidateCache006
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache006, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGR_565;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_0700
 * @tc.name   : testDisplayBufferInvalidateCache007
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache007, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRX_4444;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_0800
 * @tc.name   : testDisplayBufferInvalidateCache008
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache008, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_4444;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_0900
 * @tc.name   : testDisplayBufferInvalidateCache009
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache009, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRX_5551;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1000
 * @tc.name   : testDisplayBufferInvalidateCache010
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache010, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_5551;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1100
 * @tc.name   : testDisplayBufferInvalidateCache011
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache011, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRX_8888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1200
 * @tc.name   : testDisplayBufferInvalidateCache012
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache012, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_BGRA_8888;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1300
 * @tc.name   : testDisplayBufferInvalidateCache013
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache013, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_422_SP;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1400
 * @tc.name   : testDisplayBufferInvalidateCache014
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache014, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_422_SP;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1500
 * @tc.name   : testDisplayBufferInvalidateCache015
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache015, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_420_SP;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1600
 * @tc.name   : testDisplayBufferInvalidateCache016
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache016, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_420_SP;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1700
 * @tc.name   : testDisplayBufferInvalidateCache017
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache017, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_422_P;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1800
 * @tc.name   : testDisplayBufferInvalidateCache018
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache018, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_422_P;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_1900
 * @tc.name   : testDisplayBufferInvalidateCache019
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache019, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCBCR_420_P;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_InvalidateCache_2000
 * @tc.name   : testDisplayBufferInvalidateCache020
 * @tc.desc   : Reliability of function(InvalidateCache)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferInvalidateCache020, Function | MediumTest | Level1)
{
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage =
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_MMZ | OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_SHARE |
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
        OHOS::HDI::Display::Composer::V1_0::HBM_USE_VENDOR_PRI0;
    info.format = PIXEL_FMT_YCRCB_420_P;

    BufferHandle *bHandle = nullptr;
    g_dispbuf->AllocMem(info, bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    if (buffer == nullptr) {
        g_dispbuf->FreeMem(*bHandle);
    }
    int32_t ret = g_dispbuf->InvalidateCache(*bHandle);
    EXPECT_EQ(ret, HDF_SUCCESS);
    AfterCache(bHandle);
}

/**
 * @tc.number : SUB_Driver_Display_Buffer_FreeMem_0100
 * @tc.name   : testDisplayBufferFreeMem001
 * @tc.desc   : Reliability of function(FreeMem)
 */
HWTEST_F(DisplayBufferMtAdditional, testDisplayBufferFreeMem001, Function | MediumTest | Level1)
{
    BufferHandle *bHandle = nullptr;
    AllocInfo info;
    info.width = 1024;
    info.height = 1024;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = PIXEL_FMT_BGRA_8888;

    g_dispbuf->AllocMem(info, bHandle);
    g_dispbuf->FreeMem(*bHandle);
    void *buffer = g_dispbuf->Mmap(*bHandle);
    EXPECT_TRUE((reinterpret_cast<void *>(0xffffffff) == buffer) || (nullptr == buffer));
}

} // namespace TEST
} // namespace Display
} // namespace HDI
} // namespace OHOS