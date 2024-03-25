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

#ifndef OHOS_HDI_BLUETOOTH_A2DP_V1_0_BLUETOOTHAUDIOSESSIONSERVICE_H
#define OHOS_HDI_BLUETOOTH_A2DP_V1_0_BLUETOOTHAUDIOSESSIONSERVICE_H

#include "v1_0/ibluetooth_audio_session.h"

namespace OHOS {
namespace HDI {
namespace Bluetooth {
namespace A2dp {
namespace V1_0 {

using BluetoothAudioStatus = OHOS::HDI::Bluetooth::A2dp::V1_0::Status;

class BluetoothAudioSessionService : public IBluetoothAudioSession {
public:
    BluetoothAudioSessionService() = default;
    virtual ~BluetoothAudioSessionService() = default;

    int32_t StartSession(SessionType sessionType, const sptr<IBluetoothAudioCallback>& callbackObj,
         std::shared_ptr<SharedMemQueue<uint8_t>>& queue) override;

    int32_t StopSession(SessionType sessionType) override;

    int32_t RenderOperationResult(Operation operation, Status status) override;
protected:
    SessionType session_type_ = SessionType::UNKNOWN_TYPE;
    sptr<IBluetoothAudioCallback> stack_iface_;
};
} // V1_0
} // A2dp
} // Bluetooth
} // HDI
} // OHOS

#endif // OHOS_HDI_BLUETOOTH_A2DP_V1_0_BLUETOOTHAUDIOSESSIONSERVICE_H