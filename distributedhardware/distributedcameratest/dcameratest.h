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

#include "icamera_state_listener.h"
#include "distributed_camera_errno.h"

namespace OHOS {
namespace DistributedHardware {
class HdiDCameraSourceStateListener : public ICameraStateListener {
public:
    HdiDCameraSourceStateListener() {};
    ~HdiDCameraSourceStateListener() = default;

    int32_t OnRegisterNotify(const std::string& devId, const std::string& dhId, const std::string& reqId,
        int32_t status, std::string& data)
    {
        return DCAMERA_OK;
    }

    int32_t OnUnregisterNotify(const std::string& devId, const std::string& dhId, const std::string& reqId,
        int32_t status, std::string& data)
    {
        return DCAMERA_OK;
    }

    void SetCallback(sptr<IDCameraSourceCallback> callback)
    {
    }
};
}
}