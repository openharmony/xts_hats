/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include "dcamera_provider.h"
#include "dcamera_provider_callback_impl.h"
#include "dcameraprovidercase.h"
#include "dcameratest.h"
#include <gtest/gtest.h>

using namespace testing::ext;

namespace OHOS {
namespace DistributedHardware {
class DCameraProviderTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();

    std::shared_ptr<DCameraSourceDev> camDev_;
    std::shared_ptr<ICameraStateListener> stateListener_;
    sptr<IDCameraProviderCallback> hdiCallback_;
};

namespace {
const std::string testDeviceId = "bb536a637105409e904d4da83790a4a7";
const std::string testCameraDh = "camera_0";
const std::string testReqId = "0xFFFF";
const std::string testParam = R"({"CodecType": ["avenc_mpeg4"],
    "OutputFormat": {"Photo":[4], "Preview":[2, 3], "Video":[2, 3]},
    "Position": "BACK",
    "ProtocolVer": "1.0",
    "MetaData": "",
    "Resolution": {
    "2": ["1920*1080", "1504*720", "1440*1080", "1280*960", "1280*720", "1232*768", "1152*720", "960*720", "960*544",
    "880*720", "720*720", "720*480", "640*480", "352*288", "320*240"],
    "3": ["1920*1080", "1504*720", "1440*1080", "1280*960", "1280*720", "1232*768", "1152*720", "960*720", "960*544",
    "880*720", "720*720", "720*480", "640*480", "352*288", "320*240"],
    "4": ["3840*2160", "3264*2448", "3264*1840", "2304*1728", "2048*1536", "1920*1440", "1920*1080", "1744*1088",
    "1280*720", "1232*768", "1152*720", "640*480", "320*240"]}})";
const int32_t TEST_WIDTH = 1920;
const int32_t TEST_HEIGTH = 1080;
const int32_t TEST_STREAMID = 2;
} // namespace

std::vector<DCStreamInfo> streamInfos;
std::vector<DCCaptureInfo> captureInfos;
std::vector<DCameraSettings> settings;
std::vector<int> streamIds;
DCameraSettings result;
DHBase dhBase;
DCameraProvider cameraProvider;
DCameraBuffer buffer;
DCameraHDFEvent hdiEvent;

void DCameraProviderTest::SetUpTestCase(void)
{
    result.type_ = METADATA_RESULT;
    result.value_ = "SettingValue";

    DCStreamInfo streamInfo1;
    streamInfo1.streamId_ = 1;
    streamInfo1.width_ = TEST_WIDTH;
    streamInfo1.height_ = TEST_HEIGTH;
    streamInfo1.stride_ = 1;
    streamInfo1.format_ = 1;
    streamInfo1.dataspace_ = 1;
    streamInfo1.encodeType_ = ENCODE_TYPE_JPEG;
    streamInfo1.type_ = SNAPSHOT_FRAME;
    streamInfos.push_back(streamInfo1);

    streamIds.push_back(1);
    streamIds.push_back(TEST_STREAMID);

    DCCaptureInfo captureInfo1;
    captureInfo1.streamIds_.push_back(1);
    captureInfo1.width_ = TEST_WIDTH;
    captureInfo1.height_ = TEST_HEIGTH;
    captureInfo1.stride_ = 1;
    captureInfo1.format_ = 1;
    captureInfo1.dataspace_ = 1;
    captureInfo1.encodeType_ = ENCODE_TYPE_H265;
    captureInfo1.type_ = CONTINUOUS_FRAME;
    captureInfos.push_back(captureInfo1);

    settings.push_back(result);
}

void DCameraProviderTest::TearDownTestCase(void) {}

void DCameraProviderTest::SetUp(void)
{
    stateListener_ = std::make_shared<HdiDCameraSourceStateListener>();
    camDev_ = std::make_shared<HdiDCameraSourceDev>(testDeviceId, testCameraDh, stateListener_);
    hdiCallback_ = new (std::nothrow) DCameraProviderCallbackImpl(testDeviceId, testCameraDh, camDev_);
}

void DCameraProviderTest::TearDown(void)
{
    hdiCallback_ = nullptr;
    camDev_ = nullptr;
    stateListener_ = nullptr;
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_EnableDCameraDevice_0200
 * @tc.name   : testDCameraEnableDCameraDevice001
 * @tc.desc   : EnableDCameraDevice, dhBase.deviceId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraEnableDCameraDevice001, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = "";
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.EnableDCameraDevice(dhBase, testParam, hdiCallback_);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_EnableDCameraDevice_0300
 * @tc.name   : testDCameraEnableDCameraDevice002
 * @tc.desc   : EnableDCameraDevice, dhBase.dhId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraEnableDCameraDevice002, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = "";
    int32_t ret = cameraProvider.EnableDCameraDevice(dhBase, testParam, hdiCallback_);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_EnableDCameraDevice_0400
 * @tc.name   : testDCameraEnableDCameraDevice003
 * @tc.desc   : EnableDCameraDevice, testParam = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraEnableDCameraDevice003, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    std::string testParamNull = "";
    int32_t ret = cameraProvider.EnableDCameraDevice(dhBase, testParamNull, hdiCallback_);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_EnableDCameraDevice_0500
 * @tc.name   : testDCameraEnableDCameraDevice004
 * @tc.desc   : EnableDCameraDevice, hdiCallback_ = nullptr, return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraEnableDCameraDevice004, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.EnableDCameraDevice(dhBase, testParam, nullptr);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_DisableDCameraDevice_0200
 * @tc.name   : testDCameraDisableDCameraDevice001
 * @tc.desc   : DisableDCameraDevice, dhBase.deviceId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraDisableDCameraDevice001, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.DisableDCameraDevice(dhBase);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_DisableDCameraDevice_0300
 * @tc.name   : testDCameraDisableDCameraDevice002
 * @tc.desc   : DisableDCameraDevice, dhBase.dhId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraDisableDCameraDevice002, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.DisableDCameraDevice(dhBase);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_AcquireBuffer_0300
 * @tc.name   : testDCameraAcquireBuffer001
 * @tc.desc   : AcquireBuffer, dhBase.deviceId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraAcquireBuffer001, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.AcquireBuffer(dhBase, TEST_STREAMID, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_AcquireBuffer_0400
 * @tc.name   : testDCameraAcquireBuffer002
 * @tc.desc   : AcquireBuffer, dhBase.dhId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraAcquireBuffer002, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.AcquireBuffer(dhBase, TEST_STREAMID, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_AcquireBuffer_0500
 * @tc.name   : testDCameraAcquireBuffer003
 * @tc.desc   : AcquireBuffer, TEST_STREAMID = -1, return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraAcquireBuffer003, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.AcquireBuffer(dhBase, -1, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_AcquireBuffer_0600
 * @tc.name   : testDCameraAcquireBuffer004
 * @tc.desc   : AcquireBuffer, TEST_STREAMID = 2147483647, return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraAcquireBuffer004, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.AcquireBuffer(dhBase, 2147483647, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_AcquireBuffer_0700
 * @tc.name   : testDCameraAcquireBuffer005
 * @tc.desc   : AcquireBuffer, TEST_STREAMID = 0, return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraAcquireBuffer005, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.AcquireBuffer(dhBase, 0, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_ShutterBuffer_0300
 * @tc.name   : testDCameraShutterBuffer001
 * @tc.desc   : ShutterBuffer, hBase.deviceId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraShutterBuffer001, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.ShutterBuffer(dhBase, TEST_STREAMID, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_ShutterBuffer_0400
 * @tc.name   : testDCameraShutterBuffer002
 * @tc.desc   : ShutterBuffer, hBase.dhId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraShutterBuffer002, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.ShutterBuffer(dhBase, TEST_STREAMID, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_ShutterBuffer_0500
 * @tc.name   : testDCameraShutterBuffer003
 * @tc.desc   : ShutterBuffer, TEST_STREAMID = -1, return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraShutterBuffer003, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.ShutterBuffer(dhBase, -1, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_ShutterBuffer_0600
 * @tc.name   : testDCameraShutterBuffer004
 * @tc.desc   : ShutterBuffer, TEST_STREAMID = 2147483647, return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraShutterBuffer004, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.ShutterBuffer(dhBase, 2147483647, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_ShutterBuffer_0700
 * @tc.name   : testDCameraShutterBuffer005
 * @tc.desc   : ShutterBuffer, TEST_STREAMID = 0, return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraShutterBuffer005, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.ShutterBuffer(dhBase, 0, buffer);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_OnSettingsResult_0900
 * @tc.name   : testDCameraOnSettingsResult001
 * @tc.desc   : OnSettingsResult, dhBase.deviceId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraOnSettingsResult001, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_OnSettingsResult_1000
 * @tc.name   : testDCameraOnSettingsResult002
 * @tc.desc   : OnSettingsResult, dhBase.dhId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraOnSettingsResult002, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_OnSettingsResult_1100
 * @tc.name   : testDCameraOnSettingsResult003
 * @tc.desc   : OnSettingsResult, result.value_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraOnSettingsResult003, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    result.value_ = "";
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_Notify_0300
 * @tc.name   : testDCameraNotify001
 * @tc.desc   : Notify, hBase.deviceId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraNotify001, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.Notify(dhBase, hdiEvent);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}

/**
 * @tc.number : SUB_DH_Dcamera_HDI_Notify_0400
 * @tc.name   : testDCameraNotify002
 * @tc.desc   : Notify, dhBase.dhId_ = "", return invalid_argument
 */
HWTEST_F(DCameraProviderTest, testDCameraNotify002, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.Notify(dhBase, hdiEvent);
    EXPECT_EQ(INVALID_ARGUMENT, ret);
}
} // namespace DistributedHardware
} // namespace OHOS