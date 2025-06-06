/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include "bluetooth_hci.h"
namespace OHOS {
namespace HDI {
namespace Bluetooth {
namespace Hci {
namespace V1_0 {
int32_t HciCallbackImpl::OnInited(BtStatus status)
{
        BtInitStatus initStatus = BtInitStatus::SUCCESS;
        if (status != BtStatus::SUCCESS) {
            initStatus = BtInitStatus::INITIALIZATION_ERROR;
        }    
    return 0;
}

int32_t HciCallbackImpl::OnReceivedHciPacket(BtType type, const std::vector<uint8_t> &data)
{
        BtPacketType packetType = BtPacketType::PACKET_TYPE_UNKNOWN;
        switch (type) {
            case BtType::HCI_EVENT:
                packetType = BtPacketType::PACKET_TYPE_EVENT;
                break;
            case BtType::ACL_DATA:
                packetType = BtPacketType::PACKET_TYPE_ACL;
                break;
            case BtType::SCO_DATA:
                packetType = BtPacketType::PACKET_TYPE_SCO;
                break;
            default:
                break;
        }
        return 0;
}    

} // V1_0
} // Hci
} // Bluetooth
} // HDI
} // OHOS

