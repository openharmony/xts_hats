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
#ifndef USBD_DEVICE_TEST_H
#define USBD_DEVICE_TEST_H

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include "v1_0/usb_types.h"

using OHOS::HDI::Usb::V1_0::UsbDev;

class HdfUsbdBenchmarkDeviceTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);

    static struct UsbDev dev_;
};
#endif // USBD_DEVICE_TEST_H