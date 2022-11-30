/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include "dcameraprovidercase.h"
#include "dcameratest.h" 
#include "dcamera_provider_callback_impl.h"
#include "dcamera_provider.h"

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
}

std::vector<DCStreamInfo> streamInfos;
std::vector<DCStreamInfo> streamInfos1;
std::vector<DCStreamInfo> streamInfos2;
std::vector<DCStreamInfo> streamInfos3;
std::vector<DCStreamInfo> streamInfos4;
std::vector<DCCaptureInfo> captureInfos;
std::vector<DCCaptureInfo> captureInfos1;
std::vector<DCCaptureInfo> captureInfos2;
std::vector<DCameraSettings> settings;
std::vector<int> streamIds;
DCameraSettings result;
DCameraSettings result1;
DCameraSettings result2;
DCameraSettings result3;
DCameraSettings result4;
DCameraSettings result5;
DCameraSettings result6;
DCStreamInfo streamInfo2;
DCStreamInfo streamInfo3;
DCStreamInfo streamInfo4;
DCStreamInfo streamInfo5;
DCCaptureInfo captureInfo2;
DCCaptureInfo captureInfo3;
DHBase dhBase;
DCameraProvider cameraProvider;
DCameraBuffer buffer;
DCameraHDFEvent hdiEvent;

void DCameraProviderTest::SetUpTestCase(void)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;

    hdiEvent.type_ = 0;
    hdiEvent.result_ = 1;
    hdiEvent.content_ = 9;

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
    hdiCallback_ = new (std::nothrow)DCameraProviderCallbackImpl(testDeviceId, testCameraDh, camDev_);
}

void DCameraProviderTest::TearDown(void)
{
    hdiCallback_ = nullptr;
    camDev_ = nullptr;
    stateListener_ = nullptr;
}

HWTEST_F(DCameraProviderTest, DCameraCase_0010, TestSize.Level1)
{
    int32_t ret = cameraProvider.EnableDCameraDevice(dhBase, testParam, hdiCallback_);
    EXPECT_EQ(0, ret) << "EnableDCameraDevice fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0020, TestSize.Level1)
{
    int32_t ret = cameraProvider.OpenSession(dhBase);
    EXPECT_NE(0, ret) << "OpenSession fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0030, TestSize.Level1)
{
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0040, TestSize.Level1)
{
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos);
    EXPECT_NE(0, ret) << "StartCapture fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0050, TestSize.Level1)
{
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result);
    EXPECT_NE(0, ret) << "OnSettingsResult fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0060, TestSize.Level1)
{
    result1.type_ = UPDATE_METADATA;
    result1.value_ = "SettingValue";
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result1);
    EXPECT_NE(0, ret) << "OnSettingsResult fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0070, TestSize.Level1)
{
    result2.type_ = ENABLE_METADATA;
    result2.value_ = "SettingValue";
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result2);
    EXPECT_NE(0, ret) << "OnSettingsResult fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0080, TestSize.Level1)
{
    result3.type_ = DISABLE_METADATA;
    result3.value_ = "SettingValue";
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result3);
    EXPECT_NE(0, ret) << "OnSettingsResult fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0090, TestSize.Level1)
{
    result4.type_ = SET_FLASH_LIGHT;
    result4.value_ = "SettingValue";
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result4);
    EXPECT_NE(0, ret) << "OnSettingsResult fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0100, TestSize.Level1)
{
    result5.type_ = FPS_RANGE;
    result5.value_ = "SettingValue";
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result5);
    EXPECT_NE(0, ret) << "OnSettingsResult fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0110, TestSize.Level1)
{
    result6.type_ = UPDATE_FRAME_METADATA;
    result6.value_ = "SettingValue";
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result6);
    EXPECT_NE(0, ret) << "OnSettingsResult fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0120, TestSize.Level1)
{
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0130, TestSize.Level1)
{
    int32_t ret = cameraProvider.AcquireBuffer(dhBase, TEST_STREAMID, buffer);
    EXPECT_NE(0, ret) << "AcquireBuffer fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0140, TestSize.Level1)
{
    int32_t ret = cameraProvider.Notify(dhBase, hdiEvent);
    EXPECT_EQ(0, ret) << "Notify fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0150, TestSize.Level1)
{
    int32_t ret = cameraProvider.ShutterBuffer(dhBase, TEST_STREAMID, buffer);
    EXPECT_NE(0, ret) << "ShutterBuffer fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0160, TestSize.Level1)
{
    int32_t ret = cameraProvider.StopCapture(dhBase, streamIds);
    EXPECT_NE(0, ret) << "StopCapture fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0170, TestSize.Level1)
{
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}


HWTEST_F(DCameraProviderTest, DCameraCase_0180, TestSize.Level1)
{
    int32_t ret = cameraProvider.CloseSession(dhBase);
    EXPECT_NE(0, ret) << "CloseSession fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0190, TestSize.Level1)
{
    int32_t ret = cameraProvider.DisableDCameraDevice(dhBase);
    EXPECT_EQ(0, ret) << "DisableDCameraDevice fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0200, TestSize.Level1)
{
    int32_t ret = cameraProvider.OpenSession(dhBase);
    EXPECT_NE(SUCCESS, ret) << "OpenSession fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0210, TestSize.Level1)
{
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos);
    EXPECT_NE(CAMERA_BUSY, ret) << "ConfigureStreams fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0220, TestSize.Level1)
{
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0230, TestSize.Level1)
{
    int32_t ret = cameraProvider.OnSettingsResult(dhBase, result);
    EXPECT_NE(METHOD_NOT_SUPPORTED, ret) << "OnSettingsResult fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0240, TestSize.Level1)
{
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(CAMERA_OFFLINE, ret) << "UpdateSettings fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0250, TestSize.Level1)
{
    int32_t ret = cameraProvider.AcquireBuffer(dhBase, TEST_STREAMID, buffer);
    EXPECT_NE(EXCEED_MAX_NUMBER, ret) << "AcquireBuffer fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0260, TestSize.Level1)
{
    int32_t ret = cameraProvider.Notify(dhBase, hdiEvent);
    EXPECT_NE(DEVICE_NOT_INIT, ret) << "Notify fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0270, TestSize.Level1)
{
    int32_t ret = cameraProvider.ShutterBuffer(dhBase, TEST_STREAMID, buffer);
    EXPECT_NE(FAILED, ret) << "ShutterBuffer fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0280, TestSize.Level1)
{
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_0290, TestSize.Level1)
{
    streamInfo3.streamId_ = 1;
    streamInfo3.width_ = TEST_WIDTH;
    streamInfo3.height_ = TEST_HEIGTH;
    streamInfo3.stride_ = 1;
    streamInfo3.format_ = 1;
    streamInfo3.dataspace_ = 1;
    streamInfo3.encodeType_ = ENCODE_TYPE_H264;
    streamInfo3.type_ = SNAPSHOT_FRAME;
    streamInfos2.push_back(streamInfo3);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos2);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_3000, TestSize.Level1)
{
    streamInfo4.streamId_ = 1;
    streamInfo4.width_ = TEST_WIDTH;
    streamInfo4.height_ = TEST_HEIGTH;
    streamInfo4.stride_ = 1;
    streamInfo4.format_ = 1;
    streamInfo4.dataspace_ = 1;
    streamInfo4.encodeType_ = ENCODE_TYPE_H265;
    streamInfo4.type_ = SNAPSHOT_FRAME;
    streamInfos3.push_back(streamInfo4);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos3);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_3001, TestSize.Level1)
{
    streamInfo5.streamId_ = 1;
    streamInfo5.width_ = TEST_WIDTH;
    streamInfo5.height_ = TEST_HEIGTH;
    streamInfo5.stride_ = 1;
    streamInfo5.format_ = 1;
    streamInfo5.dataspace_ = 1;
    streamInfo5.encodeType_ = ENCODE_TYPE_JPEG;
    streamInfo5.type_ = SNAPSHOT_FRAME;
    streamInfos4.push_back(streamInfo5);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos4);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_3002, TestSize.Level1)
{
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H265;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

HWTEST_F(DCameraProviderTest, DCameraCase_3003, TestSize.Level1)
{
    captureInfo3.streamIds_.push_back(1);
    captureInfo3.width_ = TEST_WIDTH;
    captureInfo3.height_ = TEST_HEIGTH;
    captureInfo3.stride_ = 1;
    captureInfo3.format_ = 1;
    captureInfo3.dataspace_ = 1;
    captureInfo3.encodeType_ = ENCODE_TYPE_H265;
    captureInfo3.type_ = SNAPSHOT_FRAME;
    captureInfos2.push_back(captureInfo3);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos2);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}
}
}