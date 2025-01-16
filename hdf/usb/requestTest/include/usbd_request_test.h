/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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
#ifndef USBD_REQUEST_TEST_H
#define USBD_REQUEST_TEST_H

#include <gtest/gtest.h>
#include "UsbSubscriberTest.h"
#include "v1_0/iusbd_subscriber.h"
#include "v1_0/usb_types.h"
#include "v1_0/iusbd_bulk_callback.h"

using OHOS::HDI::Usb::V1_0::UsbDev;

/**
 * Used to signify direction of data for USBEndpoint is IN, device to host
 */
const int32_t USB_ENDPOINT_DIR_IN = 0x80;

/**
 * Used to signify direction of data for USBEndpoint is OUT, host to device
 */
const int32_t USB_ENDPOINT_DIR_OUT = 0;

namespace {
class UsbdRequestTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

    static UsbDev dev_;
    static OHOS::sptr<OHOS::USB::UsbSubscriberTest> subscriber_;
};
class UsbdBulkCallbackTest : public OHOS::HDI::Usb::V1_0::IUsbdBulkCallback {
public:
    UsbdBulkCallbackTest() = default;
    ~UsbdBulkCallbackTest() = default;
    int32_t OnBulkWriteCallback(int32_t status, int32_t actLength) override
    {
        return 0;
    };
    int32_t OnBulkReadCallback(int32_t status, int32_t actLength) override
    {
        return 0;
    };
};
}
#endif // USBD_REQUEST_TEST_H
