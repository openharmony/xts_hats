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

#ifndef HDI_NNRT_TEST_H
#define HDI_NNRT_TEST_H

#include <string>
#include <vector>
#include <v1_0/innrt_device.h>

#include "gtest/gtest.h"

#include "interfaces/kits/c/neural_network_runtime.h"
#include "frameworks/native/memory_manager.h"

namespace V1_0 = OHOS::HDI::Nnrt::V1_0;

namespace OHOS::NeuralNetworkRuntime::Test {

class HDINNRtTest : public testing::Test {
public:
    // device ptr
    OHOS::sptr<V1_0::INnrtDevice> device_;
    std::vector<void*> buffers_;

    void SetUp() override
    {
        device_ = V1_0::INnrtDevice::Get();
        if (device_ == nullptr) {
            // std::cout << "Get HDI device failed." << std::endl;
            GTEST_SKIP() << "Get HDI device failed.";
        }
    }

    void TearDown() override
    {
        device_.clear();
    }
};
} // namespace OHOS::NeuralNetworkRuntime::Test

#endif // HDI_NNRT_TEST_H