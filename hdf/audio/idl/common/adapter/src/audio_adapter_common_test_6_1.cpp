/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include "osal_mem.h"
#include "v6_1/iaudio_adapter.h"
#include "v6_1/iaudio_manager.h"
#include "v6_1/audio_types.h"

using namespace std;
using namespace testing::ext;

namespace {
constexpr uint32_t g_audioAdapterNumMax = 5;
[[maybe_unused]]constexpr int32_t AUDIO_ADAPTER_BUF_TEST = 1024;

class HdfAudioUtAdapterTest_6_1 : public testing::Test {
public:
    struct IAudioManager *manager_ = nullptr;
    struct IAudioAdapter *adapter_ = nullptr;
    struct AudioAdapterDescriptor *adapterDescs_ = nullptr;
    virtual void SetUp();
    virtual void TearDown();
    void AudioAdapterDescriptorFree(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen);
};

void HdfAudioUtAdapterTest_6_1::AudioAdapterDescriptorFree(struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
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

void HdfAudioUtAdapterTest_6_1::ReleaseAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen)
{
    if ((descsLen > 0) && (descs != nullptr) && ((*descs) != nullptr)) {
        for (uint32_t i = 0; i < descsLen; i++) {
            AudioAdapterDescriptorFree(&(*descs)[i], false);
        }
        OsalMemFree(*descs);
        *descs = nullptr;
    }
}

void HdfAudioUtAdapterTest_6_1::SetUp()
{
    uint32_t size = g_audioAdapterNumMax;
    manager_ = IAudioManagerGetV6_1(false);
    if (manager_ == nullptr) {
        GTEST_SKIP()<< "Audio HDI 6.1 not support" << std::endl;
    }

    adapterDescs_ = (struct AudioAdapterDescriptor *)OsalMemCalloc(
        sizeof(struct AudioAdapterDescriptor) * (g_audioAdapterNumMax));
    ASSERT_NE(adapterDescs_, nullptr);

    ASSERT_EQ(HDF_SUCCESS, manager_->GetAllAdapters(manager_, adapterDescs_, &size));
    if (size > g_audioAdapterNumMax) {
        ReleaseAdapterDescs(&adapterDescs_, g_audioAdapterNumMax);
        ASSERT_LT(size, g_audioAdapterNumMax);
    }

    if (manager_->LoadAdapter(manager_, &adapterDescs_[0], &adapter_) != HDF_SUCCESS) {
        ReleaseAdapterDescs(&adapterDescs_, g_audioAdapterNumMax);
        ASSERT_TRUE(false);
    }
    if (adapter_ == nullptr) {
        ReleaseAdapterDescs(&adapterDescs_, g_audioAdapterNumMax);
        ASSERT_TRUE(false);
    }
}

void HdfAudioUtAdapterTest_6_1::TearDown()
{
    if (manager_ == nullptr) {
        GTEST_SKIP()<< "Audio HDI 6.1 not support" << std::endl;
    }

    manager_->UnloadAdapter(manager_, adapterDescs_[0].adapterName);
    ReleaseAdapterDescs(&adapterDescs_, g_audioAdapterNumMax);
    adapter_ = nullptr;
    IAudioManagerReleaseV6_1(manager_, false);
    manager_ = nullptr;
}

/**
 * @tc.number: SUB_Driver_Audio_AdapterHdi_0100
 * @tc.name  : SUB_Driver_Audio_AdapterHdi_0100
 * @tc.desc  : Verify IAudioAdapter CreateCallTransfer
 */
HWTEST_F(HdfAudioUtAdapterTest_6_1, SUB_Driver_Audio_AdapterHdi_0100, TestSize.Level1)
{
    if (adapter_ == nullptr) {
        GTEST_SKIP()<< "Audio HDI 6.1 not support" << std::endl;
    }
    int32_t ret = adapter_->CreateCallTransfer(nullptr);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        GTEST_SKIP()<< "CreateCallTransfer not support" << std::endl;
    }
    ASSERT_TRUE(ret == HDF_ERR_INVALID_OBJECT);
}

/**
 * @tc.number: SUB_Driver_Audio_AdapterHdi_0200
 * @tc.name  : SUB_Driver_Audio_AdapterHdi_0200
 * @tc.desc  : Verify IAudioAdapter CreateCallTransfer
 */
HWTEST_F(HdfAudioUtAdapterTest_6_1, SUB_Driver_Audio_AdapterHdi_0200, TestSize.Level0)
{
    if (adapter_ == nullptr) {
        GTEST_SKIP()<< "Audio HDI 6.1 not support" << std::endl;
    }
    int32_t ret = adapter_->CreateCallTransfer(adapter_);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        GTEST_SKIP()<< "CreateCallTransfer not support" << std::endl;
    }
    ASSERT_TRUE(ret == HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number: SUB_Driver_Audio_AdapterHdi_0300
 * @tc.name  : SUB_Driver_Audio_AdapterHdi_0300
 * @tc.desc  : Verify IAudioAdapter SetPhoneCallScene
 */
HWTEST_F(HdfAudioUtAdapterTest_6_1, SUB_Driver_Audio_AdapterHdi_0300, TestSize.Level1)
{
    if (adapter_ == nullptr) {
        GTEST_SKIP()<< "Audio HDI 6.1 not support" << std::endl;
    }
    int32_t ret = adapter_->SetPhoneCallScene(nullptr, SCENE_TYPE_TRANSFER);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        GTEST_SKIP()<< "SetPhoneCallScene not support" << std::endl;
    }
    ASSERT_TRUE(ret == HDF_ERR_INVALID_OBJECT);
}

/**
 * @tc.number: SUB_Driver_Audio_AdapterHdi_0400
 * @tc.name  : SUB_Driver_Audio_AdapterHdi_0400
 * @tc.desc  : Verify IAudioAdapter SetPhoneCallScene
 */
HWTEST_F(HdfAudioUtAdapterTest_6_1, SUB_Driver_Audio_AdapterHdi_0400, TestSize.Level0)
{
    if (adapter_ == nullptr) {
        GTEST_SKIP()<< "Audio HDI 6.1 not support" << std::endl;
    }
    int32_t ret = adapter_->SetPhoneCallScene(adapter_, SCENE_TYPE_TRANSFER);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        GTEST_SKIP()<< "SetPhoneCallScene not support" << std::endl;
    }
    ASSERT_TRUE(ret == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AdapterHdi_0500
 * @tc.name  : SUB_Driver_Audio_AdapterHdi_0500
 * @tc.desc  : Verify IAudioAdapter SetPhoneCallScene
 */
HWTEST_F(HdfAudioUtAdapterTest_6_1, SUB_Driver_Audio_AdapterHdi_0500, TestSize.Level0)
{
    if (adapter_ == nullptr) {
        GTEST_SKIP()<< "Audio HDI 6.1 not support" << std::endl;
    }
    int32_t ret = adapter_->SetPhoneCallScene(adapter_, SCENE_TYPE_MODEM);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        GTEST_SKIP()<< "SetPhoneCallScene not support" << std::endl;
    }
    ASSERT_TRUE(ret == HDF_SUCCESS);
}
}

