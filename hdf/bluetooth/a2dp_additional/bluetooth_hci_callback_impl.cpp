/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "bluetooth_hci_callback_impl.h"
namespace OHOS {
namespace HDI {
namespace Bluetooth {
namespace A2dp {
namespace V1_0 {
bool g_startRenderFlag = false;
bool g_suspendRenderFlag = false;

int32_t SeClientCallback::StartRender()
{
    g_startRenderFlag = true;
    return HDF_SUCCESS;
}
int32_t SeClientCallback::SuspendRender()
{
    g_suspendRenderFlag = true;
    return HDF_SUCCESS;
}
int32_t SeClientCallback::StopRender()
{
    return HDF_SUCCESS;
}

} // namespace V1_0
} // namespace A2dp
} // namespace Bluetooth
} // namespace HDI
} // namespace OHOS
