/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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
#ifndef HDF_SERIAL_TEST_H
#define HDF_SERIAL_TEST_H

#include <gtest/gtest.h>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <atomic>
#include "v1_0/iserials.h"
#include "v1_0/iserial_device.h"
#include "v1_0/iserial_device_callback.h"


constexpr int32_t TEST_WAIT_DATA_TIMEOUT_MS = 1000;
constexpr int32_t TEST_WAIT_OFFLINE_TIMEOUT_MS = 5000;
constexpr int32_t TEST_WRITE_TIMEOUT_MS = 1000;
constexpr int32_t TEST_LARGE_DATA_SIZE = 5000;
constexpr int32_t TEST_MIN_DEVICES_COUNT = 2;
constexpr int32_t DEFAULT_BAUD_RATE = 115200;
constexpr int32_t DATA_BIT_8 = 8;

class SerialDeviceCallbackImpl : public OHOS::HDI::Serial::V1_0::ISerialDeviceCallback {
public:
    SerialDeviceCallbackImpl();
    ~SerialDeviceCallbackImpl() override = default;

    int32_t OnDeviceOffline() override;
    int32_t OnReadData(const std::vector<int8_t>& data, uint32_t dataLen) override;

    bool WaitForData(std::vector<int8_t>& data, uint32_t& dataLen, int timeoutMs = TEST_WAIT_DATA_TIMEOUT_MS);
    bool WaitForOffline(int timeoutMs = TEST_WAIT_OFFLINE_TIMEOUT_MS);
    bool IsDeviceOffline() const;
    void Reset();

private:
    std::atomic<bool> deviceOffline_;
    std::atomic<bool> dataReceived_;
    std::mutex dataMutex_;
    std::mutex offlineMutex_;
    std::condition_variable dataCond_;
    std::condition_variable offlineCond_;
    std::vector<int8_t> receivedData_;
    uint32_t receivedLen_ = 0;
};

class HdfSerialTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

    static OHOS::sptr<OHOS::HDI::Serial::V1_0::ISerials> serial_;
    static OHOS::HDI::Serial::V1_0::SerialConfig defaultConfig_;
};

#endif // HDF_SERIAL_TEST_H
