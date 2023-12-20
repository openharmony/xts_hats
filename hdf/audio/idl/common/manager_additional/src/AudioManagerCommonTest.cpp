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

#include <climits>
#include <gtest/gtest.h>

#include "hdf_dlist.h"
#include "osal_mem.h"
#include "v2_0/iaudio_manager.h"

using namespace std;
using namespace testing::ext;
namespace {
static const uint32_t g_audioAdapterNumMax = 5;
struct IAudioManager *g_manager = nullptr;

class HdfAudioUtManagerTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    void AudioAdapterDescriptorFree(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen);
};

void HdfAudioUtManagerTestAdditional::AudioAdapterDescriptorFree(struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
{
    if (dataBlock == nullptr) {
        return;
    }

    if (dataBlock->adapterName != nullptr) {
        OsalMemFree(dataBlock->adapterName);
        dataBlock->adapterName = nullptr;
    }

    if (dataBlock->ports != nullptr) {
        OsalMemFree(dataBlock->ports);
    }

    if (freeSelf) {
        OsalMemFree(dataBlock);
    }
}

void HdfAudioUtManagerTestAdditional::ReleaseAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen)
{
    if ((descsLen > 0) && (descs != nullptr) && ((*descs) != nullptr)) {
        for (uint32_t i = 0; i < descsLen; i++) {
            AudioAdapterDescriptorFree(&(*descs)[i], false);
        }
        OsalMemFree(*descs);
        *descs = nullptr;
    }
}

void HdfAudioUtManagerTestAdditional::SetUp()
{
}

void HdfAudioUtManagerTestAdditional::TearDown()
{
}

void HdfAudioUtManagerTestAdditional::SetUpTestCase()
{
    g_manager = IAudioManagerGet(false);
    ASSERT_NE(g_manager, nullptr);
}

void HdfAudioUtManagerTestAdditional::TearDownTestCase()
{
    IAudioManagerRelease(g_manager, false);
}

/**
 * @tc.number : SUB_Driver_Audio_GetAllAdapters_0800
 * @tc.name   : testGetAllAdapters001
 * @tc.desc   : Reliability of function(GetAllAdapters)
 */
HWTEST_F(HdfAudioUtManagerTestAdditional, testGetAllAdapters001, Function | MediumTest | Level1)
{
    uint32_t size = g_audioAdapterNumMax;
    struct AudioAdapterDescriptor *descs = (struct AudioAdapterDescriptor *)OsalMemCalloc(
        sizeof(struct AudioAdapterDescriptor) * (g_audioAdapterNumMax));
    int i = 0;
    int32_t ret = 0;
    for(i = 0; i < 1000;i++){
        ret |= g_manager->GetAllAdapters(g_manager, descs, &size);
    }
    EXPECT_EQ(0, ret);
    ReleaseAdapterDescs(&descs, g_audioAdapterNumMax);
}
/**
 * @tc.number : SUB_Driver_Audio_LoadAdapter_0700
 * @tc.name   : testLoadAdapter001
 * @tc.desc   : Reliability of function(LoadAdapter)
 */
HWTEST_F(HdfAudioUtManagerTestAdditional, testLoadAdapter001, Function | MediumTest | Level1)
{
    uint32_t size = g_audioAdapterNumMax;
    struct IAudioAdapter *adapter = nullptr;

    struct AudioAdapterDescriptor *descs = (struct AudioAdapterDescriptor *)OsalMemCalloc(
        sizeof(struct AudioAdapterDescriptor) * (g_audioAdapterNumMax));
    int i = 0;
    int32_t ret = 0;
    ASSERT_EQ(HDF_SUCCESS, g_manager->GetAllAdapters(g_manager, descs, &size));
    if (size > g_audioAdapterNumMax) {
        ReleaseAdapterDescs(&descs, g_audioAdapterNumMax);
        ASSERT_GE(g_audioAdapterNumMax, size);
    }
    for(i = 0; i < 1000;i++){
        ret |= g_manager->LoadAdapter(g_manager, descs, &adapter);
        g_manager->UnloadAdapter(g_manager, descs[0].adapterName);
    }
    EXPECT_EQ(HDF_SUCCESS,ret);
    ReleaseAdapterDescs(&descs, g_audioAdapterNumMax);
}
/**
 * @tc.number : SUB_Driver_Audio_UnloadAdapter_0300
 * @tc.name   : testUnloadAdapter001
 * @tc.desc   : Reliability of function(UnloadAdapter)
 */
HWTEST_F(HdfAudioUtManagerTestAdditional, testUnloadAdapter001, Function | MediumTest | Level1)
{
    uint32_t size = g_audioAdapterNumMax;
    struct IAudioAdapter *adapter = nullptr;

    struct AudioAdapterDescriptor *descs = (struct AudioAdapterDescriptor *)OsalMemCalloc(
        sizeof(struct AudioAdapterDescriptor) * (g_audioAdapterNumMax));
    int i = 0;
    int32_t ret = 0;
    ASSERT_EQ(HDF_SUCCESS, g_manager->GetAllAdapters(g_manager, descs, &size));
    if (size > g_audioAdapterNumMax) {
        ReleaseAdapterDescs(&descs, g_audioAdapterNumMax);
        ASSERT_GE(g_audioAdapterNumMax, size);
    }
    for(i = 0; i < 1000;i++){
        g_manager->LoadAdapter(g_manager, descs, &adapter);
        ret |= g_manager->UnloadAdapter(g_manager, descs[0].adapterName);
    }
    EXPECT_EQ(HDF_SUCCESS,ret);
    ReleaseAdapterDescs(&descs, g_audioAdapterNumMax);
}
/**
 * @tc.number : SUB_Driver_Audio_ReleaseAudioManagerObject_0200
 * @tc.name   : testReleaseAudioManagerObject001
 * @tc.desc   : Reliability of function(ReleaseAudioManagerObject)
 */
HWTEST_F(HdfAudioUtManagerTestAdditional, testReleaseAudioManagerObject001, Function | MediumTest | Level2)
{
    int i = 0;
    int32_t ret = 0;
    for(i = 0; i < 1000;i++){
        ret |= g_manager->ReleaseAudioManagerObject(nullptr);
    }
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT,ret);
}
}