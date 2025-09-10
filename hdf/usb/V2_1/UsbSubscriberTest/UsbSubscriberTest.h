/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef USBSUBSCRIBERTEST_H
#define USBSUBSCRIBERTEST_H

#include "v2_0/iusbd_subscriber.h"

using OHOS::HDI::Usb::V2_0::PortInfo;
using OHOS::HDI::Usb::V2_0::USBDeviceInfo;

enum UsbdDeviceAction {
    ACT_DEVUP = 0,
    ACT_DEVDOWN,
    ACT_UPDEVICE,
    ACT_DOWNDEVICE,
};

struct UsbdDeviceDescriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
} __attribute__((packed));

struct DevBusInfo {
    int32_t busNum;
    int32_t devAddr;
};
namespace OHOS {
namespace USB {
class UsbSubscriberTest : public OHOS::HDI::Usb::V2_0::IUsbdSubscriber {
public:
    UsbSubscriberTest() = default;
    ~UsbSubscriberTest() = default;
    int32_t DeviceEvent(const OHOS::HDI::Usb::V2_0::USBDeviceInfo &info) override;
    int32_t PortChangedEvent(const OHOS::HDI::Usb::V2_0::PortInfo &info) override
    {
        return 0;
    };

    int32_t busNum_ = 0;
    int32_t devAddr_ = 0;

    std::vector<DevBusInfo> busInfos;
};
} // namespace USB
} // namespace OHOS

#endif