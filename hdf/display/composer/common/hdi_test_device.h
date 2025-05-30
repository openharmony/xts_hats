/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef HDI_TEST_DEVICE
#define HDI_TEST_DEVICE
#include <map>
#include <memory>
#include "v1_0/include/idisplay_buffer.h"
#include "v1_2/include/idisplay_composer_interface.h"
#include "v1_2/display_composer_type.h"
#include "hdi_test_device_common.h"
#include "hdi_test_display.h"
#include "hdi_test_layer.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace TEST {
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_2;
class HdiTestDevice {
public:
    static HdiTestDevice& GetInstance();
    static void HotPlug(uint32_t outputId, bool connected, void* data);

    int32_t InitDevice();

    std::shared_ptr<IDisplayBuffer> GetGrallocInterface()
    {
        return gralloc_;
    }

    sptr<Composer::V1_2::IDisplayComposerInterface> &GetDeviceInterface()
    {
        return displayDevice_;
    }
    std::shared_ptr<HdiTestDisplay> GetDisplayFromId(uint32_t id);
    std::shared_ptr<HdiTestDisplay> GetFirstDisplay();
    void Clear() const;
    std::vector<uint32_t> GetDevIds() const;

private:
    std::shared_ptr<HdiTestDisplay> FindDisplayOrCreate(uint32_t id);

    std::map<uint32_t, std::shared_ptr<HdiTestDisplay>> displays_;
    sptr<Composer::V1_2::IDisplayComposerInterface> displayDevice_;
    std::shared_ptr<IDisplayBuffer> gralloc_ = nullptr;
    std::vector<uint32_t> displayIds_;
};
} // OHOS
} // HDI
} // Display
} // TEST

#endif // HDI_TEST_DEVICE
