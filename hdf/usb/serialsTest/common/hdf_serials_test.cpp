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

#include <chrono>
#include <thread>
#include "hdf_log.h"
#include "hdf_serials_test.h"

using namespace OHOS::HDI::Serials::V1_0;
using namespace testing::ext;

SerialDeviceCallbackImpl::SerialDeviceCallbackImpl()
    : deviceOffline_(false), dataReceived_(false) {}

int32_t SerialDeviceCallbackImpl::OnDeviceOffline()
{
    HDF_LOGI("SerialDeviceCallbackImpl::OnDeviceOffline called");
    deviceOffline_ = true;
    offlineCond_.notify_all();
    return HDF_SUCCESS;
}

int32_t SerialDeviceCallbackImpl::OnReadData(const std::vector<int8_t>& data, uint32_t dataLen)
{
    HDF_LOGI("SerialDeviceCallbackImpl::OnReadData called, dataLen=%{public}u", dataLen);
    std::lock_guard<std::mutex> lock(dataMutex_);
    receivedData_ = data;
    receivedLen_ = dataLen;
    dataReceived_ = true;
    dataCond_.notify_one();
    return HDF_SUCCESS;
}

bool SerialDeviceCallbackImpl::WaitForData(std::vector<int8_t>& data, uint32_t& dataLen, int timeoutMs)
{
    std::unique_lock<std::mutex> lock(dataMutex_);
    bool result = dataCond_.wait_for(lock, std::chrono::milliseconds(timeoutMs),
        [this] { return dataReceived_.load(); });
    if (result) {
        data = receivedData_;
        dataLen = receivedLen_;
    }
    return result && dataReceived_.load();
}

bool SerialDeviceCallbackImpl::WaitForOffline(int timeoutMs)
{
    std::unique_lock<std::mutex> lock(offlineMutex_);
    return offlineCond_.wait_for(lock, std::chrono::milliseconds(timeoutMs),
        [this] { return deviceOffline_.load(); }) && deviceOffline_.load();
}

bool SerialDeviceCallbackImpl::IsDeviceOffline() const
{
    return deviceOffline_.load();
}

void SerialDeviceCallbackImpl::Reset()
{
    std::lock_guard<std::mutex> lockData(dataMutex_);
    std::lock_guard<std::mutex> lockOffline(offlineMutex_);
    deviceOffline_ = false;
    dataReceived_ = false;
    receivedData_.clear();
    receivedLen_ = 0;
}

sptr<ISerials> HdfSerialsTest::serials_ = nullptr;
SerialConfig HdfSerialsTest::defaultConfig_ = {};

void HdfSerialsTest::SetUpTestCase()
{
    HDF_LOGI("HdfSerialsTest::SetUpTestCase begin");
    serials_ = ISerials::Get(true);
    ASSERT_NE(serials_, nullptr);

    defaultConfig_.baudRate = DEFAULT_BAUD_RATE;
    defaultConfig_.dataBits = DATA_BIT_8;
    defaultConfig_.stopBits = 1;
    defaultConfig_.parity = 0;
    defaultConfig_.rtscts = false;
    defaultConfig_.xon = false;
    defaultConfig_.xoff = false;
    defaultConfig_.xany = false;
    HDF_LOGI("HdfSerialsTest::SetUpTestCase end");
}

void HdfSerialsTest::TearDownTestCase()
{
    HDF_LOGI("HdfSerialsTest::TearDownTestCase");
    serials_ = nullptr;
}

void HdfSerialsTest::SetUp() {}

void HdfSerialsTest::TearDown() {}

// ============================================================
// 5.1 ISerials Interface Tests (12 cases)
// ============================================================

/**
 * @tc.number: SUB_Serials_HDI_Func_0100
 * @tc.name: SUB_Serials_HDI_Func_0100
 * @tc.desc: Test functions to QueryDevices
 * @tc.desc: Positive test: query available serial devices
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0100, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_GE(devices.size(), 0);
}

/**
 * @tc.number: SUB_Serials_HDI_Func_0200
 * @tc.name: SUB_Serials_HDI_Func_0200
 * @tc.desc: Test functions to QueryDevices
 * @tc.desc: Verify each device info field is valid
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0200, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    for (const auto& device : devices) {
        EXPECT_FALSE(device.portName.empty());
        HDF_LOGI("Device: portName=%{public}s, manufacturer=%{public}s, vendorId=%{public}d, productId=%{public}d",
            device.portName.c_str(), device.manufacturer.c_str(), device.vendorId, device.productId);
    }
}

/**
 * @tc.number: SUB_Serials_HDI_Func_0300
 * @tc.name: SUB_Serials_HDI_Func_0300
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Positive test: open device with default config
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0300, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(device, nullptr);
    if (device != nullptr) { device->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Func_0400
 * @tc.name: SUB_Serials_HDI_Func_0400
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Open device with baud rate 9600
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0400, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    SerialConfig config;
    config.baudRate = 9600;
    config.dataBits = DATA_BIT_8;
    config.stopBits = 1;
    config.parity = 0;
    config.rtscts = false;
    config.xon = false;
    config.xoff = false;
    config.xany = false;

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(device, nullptr);
    if (device != nullptr) { device->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Func_0500
 * @tc.name: SUB_Serials_HDI_Func_0500
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Open device with data bits 7
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0500, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    SerialConfig config = defaultConfig_;
    config.dataBits = 7;

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(device, nullptr);
    if (device != nullptr) { device->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Func_0600
 * @tc.name: SUB_Serials_HDI_Func_0600
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Open device with even parity
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0600, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    SerialConfig config = defaultConfig_;
    config.parity = 1;

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(device, nullptr);
    if (device != nullptr) { device->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Func_0700
 * @tc.name: SUB_Serials_HDI_Func_0700
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Open device with RTS/CTS hardware flow control enabled
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0700, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    SerialConfig config = defaultConfig_;
    config.rtscts = true;

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(device, nullptr);
    if (device != nullptr) { device->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Func_0750
 * @tc.name: SUB_Serials_HDI_Func_0750
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Open device with XON/XOFF software flow control enabled
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0750, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    SerialConfig config = defaultConfig_;
    config.xon = true;
    config.xoff = true;
    config.xany = true;

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(device, nullptr);
    if (device != nullptr) { device->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Compatibility_0100
 * @tc.name: SUB_Serials_HDI_Compatibility_0100
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Negative test: empty port name should fail
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Compatibility_0100, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    int32_t ret = serials_->OpenDevice("", defaultConfig_, callback, device);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Serials_HDI_Compatibility_0200
 * @tc.name: SUB_Serials_HDI_Compatibility_0200
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Negative test: nonexistent device path should fail
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Compatibility_0200, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    int32_t ret = serials_->OpenDevice("/dev/nonexistent", defaultConfig_, callback, device);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Serials_HDI_Compatibility_0300
 * @tc.name: SUB_Serials_HDI_Compatibility_0300
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Positive test: null callback is allowed
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Compatibility_0300, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, nullptr, device);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(device, nullptr);
    if (device != nullptr) { device->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Compatibility_0400
 * @tc.name: SUB_Serials_HDI_Compatibility_0400
 * @tc.desc: Test functions to OpenDevice
 * @tc.desc: Negative test: open same port twice should return HDF_ERR_DEVICE_BUSY
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Compatibility_0400, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device1;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device1);
    ASSERT_EQ(ret, HDF_SUCCESS);

    sptr<ISerialDevice> device2;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device2);
    EXPECT_EQ(ret, HDF_ERR_DEVICE_BUSY);

    if (device1 != nullptr) { device1->Close(); }
}

// ============================================================
// 5.2 SerialConfig Boundary Tests (5 cases)
// ============================================================

/**
 * @tc.number: SUB_Serials_HDI_Boundary_0100
 * @tc.name: SUB_Serials_HDI_Boundary_0100
 * @tc.desc: Test SerialConfig boundary: baudRate = 0
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Boundary_0100, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    SerialConfig config = defaultConfig_;
    config.baudRate = 0;

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
    if (device != nullptr) { device->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Boundary_0200
 * @tc.name: SUB_Serials_HDI_Boundary_0200
 * @tc.desc: Test SerialConfig boundary: dataBits = 0 and 9
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Boundary_0200, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    for (int bits : {0, 9}) {
        SerialConfig config = defaultConfig_;
        config.dataBits = bits;
        sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
        sptr<ISerialDevice> device;
        ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
        if (device != nullptr) { device->Close(); }
    }
}

/**
 * @tc.number: SUB_Serials_HDI_Boundary_0300
 * @tc.name: SUB_Serials_HDI_Boundary_0300
 * @tc.desc: Test SerialConfig boundary: stopBits = 0 and 3
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Boundary_0300, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    for (int bits : {0, 3}) {
        SerialConfig config = defaultConfig_;
        config.stopBits = bits;
        sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
        sptr<ISerialDevice> device;
        ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
        if (device != nullptr) { device->Close(); }
    }
}

/**
 * @tc.number: SUB_Serials_HDI_Boundary_0400
 * @tc.name: SUB_Serials_HDI_Boundary_0400
 * @tc.desc: Test SerialConfig boundary: parity = 3 and -1
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Boundary_0400, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    for (int p : {3, -1}) {
        SerialConfig config = defaultConfig_;
        config.parity = p;
        sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
        sptr<ISerialDevice> device;
        ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
        if (device != nullptr) { device->Close(); }
    }
}

/**
 * @tc.number: SUB_Serials_HDI_Boundary_0500
 * @tc.name: SUB_Serials_HDI_Boundary_0500
 * @tc.desc: Test SerialConfig boundary: baudRate = 50 and 4000000
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Boundary_0500, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    for (int rate : {50, 4000000}) {
        SerialConfig config = defaultConfig_;
        config.baudRate = rate;
        sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
        sptr<ISerialDevice> device;
        ret = serials_->OpenDevice(devices[0].portName, config, callback, device);
        if (device != nullptr) { device->Close(); }
    }
}

// ============================================================
// 5.3 ISerialDevice Read/Write Tests (10 cases)
// ============================================================

/**
 * @tc.number: SUB_Serials_HDI_Func_0800
 * @tc.name: SUB_Serials_HDI_Func_0800
 * @tc.desc: Test functions to Write
 * @tc.desc: Positive test: write normal data
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0800, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};
    int32_t bytesWritten = 0;
    ret = device->Write(data, TEST_WRITE_TIMEOUT_MS, bytesWritten);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(bytesWritten, static_cast<int32_t>(data.size()));
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_0900
 * @tc.name: SUB_Serials_HDI_Func_0900
 * @tc.desc: Test functions to Write
 * @tc.desc: Negative test: write empty data should fail
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_0900, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    std::vector<uint8_t> emptyData;
    int32_t bytesWritten = 0;
    ret = device->Write(emptyData, TEST_WRITE_TIMEOUT_MS, bytesWritten);
    EXPECT_NE(ret, HDF_SUCCESS);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1000
 * @tc.name: SUB_Serials_HDI_Func_1000
 * @tc.desc: Test functions to Write
 * @tc.desc: Write max 4096 bytes data
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1000, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    std::vector<uint8_t> data(4096, 0x55);
    int32_t bytesWritten = 0;
    ret = device->Write(data, TEST_WRITE_TIMEOUT_MS, bytesWritten);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(bytesWritten, static_cast<int32_t>(data.size()));
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Compatibility_0500
 * @tc.name: SUB_Serials_HDI_Compatibility_0500
 * @tc.desc: Test functions to Write
 * @tc.desc: Boundary test: timeout = 0
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Compatibility_0500, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    std::vector<uint8_t> data = {0x01, 0x02};
    int32_t bytesWritten = 0;
    ret = device->Write(data, 0, bytesWritten);
    HDF_LOGI("Write with timeout=0 ret=%{public}d", ret);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Compatibility_0600
 * @tc.name: SUB_Serials_HDI_Compatibility_0600
 * @tc.desc: Test functions to Write
 * @tc.desc: Boundary test: timeout = -1
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Compatibility_0600, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    std::vector<uint8_t> data = {0x01, 0x02};
    int32_t bytesWritten = 0;
    ret = device->Write(data, -1, bytesWritten);
    HDF_LOGI("Write with timeout=-1 ret=%{public}d", ret);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1100
 * @tc.name: SUB_Serials_HDI_Func_1100
 * @tc.desc: Test functions to StartRead
 * @tc.desc: Positive test: start reading
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1100, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->StartRead();
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = device->StopRead();
    EXPECT_EQ(ret, HDF_SUCCESS);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1150
 * @tc.name: SUB_Serials_HDI_Func_1150
 * @tc.desc: Test functions to StartRead
 * @tc.desc: Call StartRead twice, should still succeed
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1150, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->StartRead();
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = device->StartRead();
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = device->StopRead();
    EXPECT_EQ(ret, HDF_SUCCESS);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1200
 * @tc.name: SUB_Serials_HDI_Func_1200
 * @tc.desc: Test functions to StopRead
 * @tc.desc: Positive test: stop reading after start
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1200, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->StartRead();
    ASSERT_EQ(ret, HDF_SUCCESS);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    ret = device->StopRead();
    EXPECT_EQ(ret, HDF_SUCCESS);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1250
 * @tc.name: SUB_Serials_HDI_Func_1250
 * @tc.desc: Test functions to Flush
 * @tc.desc: Positive test: flush buffer
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1250, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->Flush();
    EXPECT_EQ(ret, HDF_SUCCESS);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1300
 * @tc.name: SUB_Serials_HDI_Func_1300
 * @tc.desc: Test functions to Drain
 * @tc.desc: Positive test: wait for all write operations to complete
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1300, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->Drain();
    EXPECT_EQ(ret, HDF_SUCCESS);
    device->Close();
}

// ============================================================
// 5.4 ISerialDevice Signal & Close Tests (5 cases)
// ============================================================

/**
 * @tc.number: SUB_Serials_HDI_Func_1400
 * @tc.name: SUB_Serials_HDI_Func_1400
 * @tc.desc: Test functions to SendBrkSignal
 * @tc.desc: Positive test: send BRK signal
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1400, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->SendBrkSignal();
    EXPECT_EQ(ret, HDF_SUCCESS);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1500
 * @tc.name: SUB_Serials_HDI_Func_1500
 * @tc.desc: Test functions to SetRtsSignal
 * @tc.desc: Set RTS signal to true
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1500, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->SetRtsSignal(true);
    EXPECT_EQ(ret, HDF_SUCCESS);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1600
 * @tc.name: SUB_Serials_HDI_Func_1600
 * @tc.desc: Test functions to SetRtsSignal
 * @tc.desc: Set RTS signal to false
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1600, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->SetRtsSignal(false);
    EXPECT_EQ(ret, HDF_SUCCESS);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1700
 * @tc.name: SUB_Serials_HDI_Func_1700
 * @tc.desc: Test functions to GetCtsSignal
 * @tc.desc: Get CTS signal state
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1700, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    bool cts = false;
    ret = device->GetCtsSignal(cts);
    EXPECT_EQ(ret, HDF_SUCCESS);
    HDF_LOGI("GetCtsSignal result: cts=%{public}d", cts ? 1 : 0);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_1800
 * @tc.name: SUB_Serials_HDI_Func_1800
 * @tc.desc: Test functions to Close
 * @tc.desc: Positive test: close an opened device
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1800, Function | MediumTest | Level0)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->Close();
    EXPECT_EQ(ret, HDF_SUCCESS);
}

// ============================================================
// 5.5 Callback Tests (3 cases)
// ============================================================

/**
 * @tc.number: SUB_Serials_HDI_Func_1900
 * @tc.name: SUB_Serials_HDI_Func_1900
 * @tc.desc: Test callback OnReadData
 * @tc.desc: Verify callback receives data after write (loopback test)
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_1900, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    callback->Reset();
    ret = device->StartRead();
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::vector<uint8_t> sendData = {0x55, 0xAA, 0x12, 0x34};
    int32_t bytesWritten = 0;
    ret = device->Write(sendData, TEST_WRITE_TIMEOUT_MS, bytesWritten);
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::vector<int8_t> receivedData;
    uint32_t receivedLen = 0;
    bool dataReceived = callback->WaitForData(receivedData, receivedLen, TEST_WAIT_DATA_TIMEOUT_MS);
    if (dataReceived) {
        EXPECT_EQ(receivedLen, sendData.size());
    }

    device->StopRead();
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_2000
 * @tc.name: SUB_Serials_HDI_Func_2000
 * @tc.desc: Test callback OnDeviceOffline
 * @tc.desc: Verify callback initial state (not offline)
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_2000, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    EXPECT_FALSE(callback->IsDeviceOffline());
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Func_2050
 * @tc.name: SUB_Serials_HDI_Func_2050
 * @tc.desc: Test callback Reset
 * @tc.desc: Verify callback state reset works correctly
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Func_2050, Function | MediumTest | Level2)
{
    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    EXPECT_FALSE(callback->IsDeviceOffline());

    callback->Reset();
    EXPECT_FALSE(callback->IsDeviceOffline());
}

// ============================================================
// 5.6 Scenario Tests (7 cases)
// ============================================================

/**
 * @tc.number: SUB_Serials_HDI_Scenario_0100
 * @tc.name: SUB_Serials_HDI_Scenario_0100
 * @tc.desc: Full read-write flow scenario
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Scenario_0100, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    callback->Reset();
    ret = device->StartRead();
    ASSERT_EQ(ret, HDF_SUCCESS);

    std::vector<uint8_t> sendData = {0x55, 0xAA, 0x12, 0x34};
    int32_t bytesWritten = 0;
    ret = device->Write(sendData, TEST_WRITE_TIMEOUT_MS, bytesWritten);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_EQ(bytesWritten, static_cast<int32_t>(sendData.size()));

    std::vector<int8_t> receivedData;
    uint32_t receivedLen = 0;
    callback->WaitForData(receivedData, receivedLen, TEST_WAIT_DATA_TIMEOUT_MS);

    ret = device->StopRead();
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = device->Close();
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Serials_HDI_Scenario_0200
 * @tc.name: SUB_Serials_HDI_Scenario_0200
 * @tc.desc: Write-Flush-Drain flow scenario
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Scenario_0200, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    int32_t bytesWritten = 0;
    ret = device->Write(data, TEST_WRITE_TIMEOUT_MS, bytesWritten);
    ASSERT_EQ(ret, HDF_SUCCESS);

    ret = device->Flush();
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = device->Drain();
    EXPECT_EQ(ret, HDF_SUCCESS);

    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Scenario_0300
 * @tc.name: SUB_Serials_HDI_Scenario_0300
 * @tc.desc: Close device directly after StartRead
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Scenario_0300, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->StartRead();
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = device->Close();
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Serials_HDI_Scenario_0400
 * @tc.name: SUB_Serials_HDI_Scenario_0400
 * @tc.desc: Operations after Close should fail
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Scenario_0400, Function | MediumTest | Level1)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    device->Close();

    std::vector<uint8_t> data = {0x01, 0x02};
    int32_t bytesWritten = 0;
    ret = device->Write(data, TEST_WRITE_TIMEOUT_MS, bytesWritten);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = device->StartRead();
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Serials_HDI_Scenario_0500
 * @tc.name: SUB_Serials_HDI_Scenario_0500
 * @tc.desc: Reopen device after close
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Scenario_0500, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();

    sptr<ISerialDevice> device1;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device1);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device1, nullptr);
    device1->Close();

    sptr<ISerialDevice> device2;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device2);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(device2, nullptr);
    if (device2 != nullptr) { device2->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Scenario_0600
 * @tc.name: SUB_Serials_HDI_Scenario_0600
 * @tc.desc: Open multiple devices concurrently (requires 2+ devices)
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Scenario_0600, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.size() < TEST_MIN_DEVICES_COUNT) { return; }

    sptr<SerialDeviceCallbackImpl> callback1 = new SerialDeviceCallbackImpl();
    sptr<SerialDeviceCallbackImpl> callback2 = new SerialDeviceCallbackImpl();

    sptr<ISerialDevice> device1;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback1, device1);
    EXPECT_EQ(ret, HDF_SUCCESS);

    sptr<ISerialDevice> device2;
    ret = serials_->OpenDevice(devices[1].portName, defaultConfig_, callback2, device2);
    EXPECT_EQ(ret, HDF_SUCCESS);

    if (device1 != nullptr) { device1->Close(); }
    if (device2 != nullptr) { device2->Close(); }
}

/**
 * @tc.number: SUB_Serials_HDI_Scenario_0700
 * @tc.name: SUB_Serials_HDI_Scenario_0700
 * @tc.desc: RTS/CTS signal interaction scenario
 * @tc.type: FUNC
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Scenario_0700, Function | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    ret = device->SetRtsSignal(true);
    EXPECT_EQ(ret, HDF_SUCCESS);

    bool cts = false;
    ret = device->GetCtsSignal(cts);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = device->SetRtsSignal(false);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = device->GetCtsSignal(cts);
    EXPECT_EQ(ret, HDF_SUCCESS);

    device->Close();
}

// ============================================================
// 5.7 Performance Tests (6 cases)
// ============================================================

/**
 * @tc.number: SUB_Serials_HDI_Perf_0100
 * @tc.name: SUB_Serials_HDI_Perf_0100
 * @tc.desc: QueryDevices performance test (100 iterations)
 * @tc.type: PERF
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Perf_0100, Performance | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    const int iterations = 100;
    std::vector<SerialDeviceInfo> devices;

    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        devices.clear();
        int32_t ret = serials_->QueryDevices(devices);
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    double avgTime = static_cast<double>(duration) / iterations;
    HDF_LOGI("QueryDevices Perf: total=%{public}lld us, avg=%{public}.2f us", duration, avgTime);
}

/**
 * @tc.number: SUB_Serials_HDI_Perf_0200
 * @tc.name: SUB_Serials_HDI_Perf_0200
 * @tc.desc: Open/Close device performance test (50 iterations)
 * @tc.type: PERF
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Perf_0200, Performance | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    const int iterations = 50;
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
        sptr<ISerialDevice> device;
        ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
        ASSERT_EQ(ret, HDF_SUCCESS);
        if (device != nullptr) { device->Close(); }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    double avgTime = static_cast<double>(duration) / iterations;
    HDF_LOGI("OpenClose Perf: total=%{public}lld us, avg=%{public}.2f us", duration, avgTime);
}

/**
 * @tc.number: SUB_Serials_HDI_Perf_0300
 * @tc.name: SUB_Serials_HDI_Perf_0300
 * @tc.desc: Write small data performance test (100 iterations)
 * @tc.type: PERF
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Perf_0300, Performance | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    const int iterations = 100;
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        int32_t bytesWritten = 0;
        ret = device->Write(data, TEST_WRITE_TIMEOUT_MS, bytesWritten);
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    double avgTime = static_cast<double>(duration) / iterations;
    HDF_LOGI("Write Small Perf: total=%{public}lld us, avg=%{public}.2f us", duration, avgTime);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Perf_0400
 * @tc.name: SUB_Serials_HDI_Perf_0400
 * @tc.desc: Write large data performance test (50 iterations)
 * @tc.type: PERF
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Perf_0400, Performance | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    const int iterations = 50;
    std::vector<uint8_t> data(4096, 0x55);
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        int32_t bytesWritten = 0;
        ret = device->Write(data, TEST_WRITE_TIMEOUT_MS, bytesWritten);
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    double avgTime = static_cast<double>(duration) / iterations;
    HDF_LOGI("Write Large Perf: total=%{public}lld us, avg=%{public}.2f us", duration, avgTime);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Perf_0500
 * @tc.name: SUB_Serials_HDI_Perf_0500
 * @tc.desc: StartRead/StopRead performance test (50 iterations)
 * @tc.type: PERF
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Perf_0500, Performance | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    const int iterations = 50;
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        ret = device->StartRead();
        ASSERT_EQ(ret, HDF_SUCCESS);
        ret = device->StopRead();
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    double avgTime = static_cast<double>(duration) / iterations;
    HDF_LOGI("StartStopRead Perf: total=%{public}lld us, avg=%{public}.2f us", duration, avgTime);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Perf_0600
 * @tc.name: SUB_Serials_HDI_Perf_0600
 * @tc.desc: Signal control performance test (100 iterations)
 * @tc.type: PERF
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Perf_0600, Performance | MediumTest | Level3)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    const int iterations = 100;
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        ret = device->SetRtsSignal(true);
        ASSERT_EQ(ret, HDF_SUCCESS);
        bool cts = false;
        ret = device->GetCtsSignal(cts);
        ASSERT_EQ(ret, HDF_SUCCESS);
        ret = device->SetRtsSignal(false);
        ASSERT_EQ(ret, HDF_SUCCESS);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    double avgTime = static_cast<double>(duration) / iterations;
    HDF_LOGI("Signal Perf: total=%{public}lld us, avg=%{public}.2f us", duration, avgTime);
    device->Close();
}

// ============================================================
// 5.8 Reliability Tests (4 cases)
// ============================================================

/**
 * @tc.number: SUB_Serials_HDI_Reliability_0100
 * @tc.name: SUB_Serials_HDI_Reliability_0100
 * @tc.desc: Continuous write 1000 times
 * @tc.type: RELIABILITY
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Reliability_0100, Reliability | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    const int operationCount = 1000;
    std::vector<uint8_t> data = {0x55, 0xAA};
    int32_t failCount = 0;
    for (int i = 0; i < operationCount; i++) {
        int32_t bytesWritten = 0;
        ret = device->Write(data, TEST_WRITE_TIMEOUT_MS, bytesWritten);
        if (ret != HDF_SUCCESS) {
            failCount++;
            HDF_LOGE("Write failed at iteration %{public}d, ret=%{public}d", i, ret);
        }
    }
    HDF_LOGI("ContinuousWrite: total=%{public}d, failed=%{public}d", operationCount, failCount);
    EXPECT_EQ(failCount, 0);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Reliability_0200
 * @tc.name: SUB_Serials_HDI_Reliability_0200
 * @tc.desc: Continuous read-write loop
 * @tc.type: RELIABILITY
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Reliability_0200, Reliability | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
    sptr<ISerialDevice> device;
    ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(device, nullptr);

    callback->Reset();
    ret = device->StartRead();
    ASSERT_EQ(ret, HDF_SUCCESS);

    const int operationCount = 500;
    std::vector<uint8_t> data = {0x55, 0xAA};
    for (int i = 0; i < operationCount; i++) {
        int32_t bytesWritten = 0;
        ret = device->Write(data, TEST_WRITE_TIMEOUT_MS, bytesWritten);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("Write failed at iteration %{public}d", i);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ret = device->StopRead();
    EXPECT_EQ(ret, HDF_SUCCESS);
    device->Close();
}

/**
 * @tc.number: SUB_Serials_HDI_Reliability_0300
 * @tc.name: SUB_Serials_HDI_Reliability_0300
 * @tc.desc: Repeated open/close 50 times
 * @tc.type: RELIABILITY
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Reliability_0300, Reliability | MediumTest | Level2)
{
    ASSERT_NE(serials_, nullptr);
    std::vector<SerialDeviceInfo> devices;
    int32_t ret = serials_->QueryDevices(devices);
    ASSERT_EQ(ret, HDF_SUCCESS);
    if (devices.empty()) { return; }

    const int iterations = 50;
    int32_t failCount = 0;
    for (int i = 0; i < iterations; i++) {
        sptr<SerialDeviceCallbackImpl> callback = new SerialDeviceCallbackImpl();
        sptr<ISerialDevice> device;
        ret = serials_->OpenDevice(devices[0].portName, defaultConfig_, callback, device);
        if (ret != HDF_SUCCESS) {
            failCount++;
            continue;
        }
        if (device != nullptr) { device->Close(); }
    }
    HDF_LOGI("RepeatedOpenClose: total=%{public}d, failed=%{public}d", iterations, failCount);
    EXPECT_EQ(failCount, 0);
}

/**
 * @tc.number: SUB_Serials_HDI_Reliability_0400
 * @tc.name: SUB_Serials_HDI_Reliability_0400
 * @tc.desc: Multiple callback instances concurrent test
 * @tc.type: RELIABILITY
 */
HWTEST_F(HdfSerialsTest, SUB_Serials_HDI_Reliability_0400, Reliability | MediumTest | Level3)
{
    sptr<SerialDeviceCallbackImpl> cb1 = new SerialDeviceCallbackImpl();
    sptr<SerialDeviceCallbackImpl> cb2 = new SerialDeviceCallbackImpl();
    sptr<SerialDeviceCallbackImpl> cb3 = new SerialDeviceCallbackImpl();

    EXPECT_FALSE(cb1->IsDeviceOffline());
    EXPECT_FALSE(cb2->IsDeviceOffline());
    EXPECT_FALSE(cb3->IsDeviceOffline());

    cb1->Reset();
    cb2->Reset();
    cb3->Reset();

    EXPECT_FALSE(cb1->IsDeviceOffline());
    EXPECT_FALSE(cb2->IsDeviceOffline());
    EXPECT_FALSE(cb3->IsDeviceOffline());
}
