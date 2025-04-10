/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef BLUETOOTH_HDI_H
#define BLUETOOTH_HDI_H


#ifdef __cplusplus
extern "C" {
#endif

// Hci hal backend status
typedef enum {
    SUCCESS,
    TRANSPORT_ERROR,
    INITIALIZATION_ERROR,
    UNKNOWN,
} BtInitStatus;

typedef enum {
    PACKET_TYPE_UNKNOWN = 0,
    PACKET_TYPE_CMD = 1,
    PACKET_TYPE_ACL = 2,
    PACKET_TYPE_SCO = 3,
    PACKET_TYPE_EVENT = 4
} BtPacketType;

// Hci packet received from backend
typedef struct BtPacket {
    uint8_t *data;
    uint32_t size;
} BtPacket;

// BluetoothHciCallbacks register to hal by upperlayer protocol
typedef struct BtHciCallbacks {
    void (*OnInited)(BtInitStatus status);
    void (*OnReceivedHciPacket)(BtPacketType type, const BtPacket *packet);
} BtHciCallbacks;


#ifdef __cplusplus
}
#endif

#endif

