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
#ifndef USBD_ACCESSORY_TEST_H
#define USBD_ACCESSORY_TEST_H

#include <gtest/gtest.h>
#include "UsbSubscriberTest.h"
#include "v1_0/iusbd_subscriber.h"
#include "v1_0/usb_types.h"

using OHOS::HDI::Usb::V1_0::UsbDev;

class UsbdAccessoryTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

    static UsbDev dev_;
    static OHOS::sptr<OHOS::USB::UsbSubscriberTest> subscriber_;
};

#endif // USBD_ACCESSORY_TEST_H
