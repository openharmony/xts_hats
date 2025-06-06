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

#ifndef OHOS_HDI_MOTION_V1_0_MOTIONCALLBACKIMPL_H
#define OHOS_HDI_MOTION_V1_0_MOTIONCALLBACKIMPL_H

#include "v1_0/imotion_callback.h"
#include "v1_0/motion_types.h"

namespace OHOS {
namespace HDI {
namespace Motion {
namespace V1_0 {
class MotionCallbackImpl : public IMotionCallback {
public:
    MotionCallbackImpl() = default;
    virtual ~MotionCallbackImpl() = default;
    int32_t OnDataEvent(const HdfMotionEvent &event) override;
};
} // namespace V1_0
} // namespace Motion
} // namespace HDI
} // namespace OHOS

#endif // OHOS_HDI_MOTION_V1_0_MOTIONCALLBACKIMPL_H