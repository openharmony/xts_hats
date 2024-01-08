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
DHBase dhBaseTest;
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

/**
 * @tc.number: SUB_DH_Dcamera_HDI_OpenSession_0300
 * @tc.name: testDistributedCameraOpenSession001
 * @tc.desc: OpenSession, dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraOpenSession001, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.OpenSession(dhBase);
    EXPECT_NE(0, ret) << "OpenSession success";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_OpenSession_0400
 * @tc.name: testDistributedCameraOpenSession002
 * @tc.desc: OpenSession, dhBase.deviceId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraOpenSession002, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.OpenSession(dhBase);
    EXPECT_NE(0, ret) << "OpenSession fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_OpenSession_0500
 * @tc.name: testDistributedCameraOpenSession003
 * @tc.desc: OpenSession, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraOpenSession003, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.OpenSession(dhBase);
    EXPECT_NE(0, ret) << "OpenSession fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_CloseSession_0200
 * @tc.name: testDistributedCameraCloseSession001
 * @tc.desc: CloseSession, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraCloseSession001, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.CloseSession(dhBase);
    EXPECT_NE(0, ret) << "CloseSession fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_CloseSession_0300
 * @tc.name: testDistributedCameraCloseSession002
 * @tc.desc: CloseSession, dhBase.deviceId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraCloseSession002, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.CloseSession(dhBase);
    EXPECT_NE(0, ret) << "CloseSession fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_CloseSession_0400
 * @tc.name: testDistributedCameraCloseSession003
 * @tc.desc: CloseSession, dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraCloseSession003, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testCameraDh;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.CloseSession(dhBase);
    EXPECT_NE(0, ret) << "CloseSession fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_0700
 * @tc.name: testDistributedCameraConfigureStreams001
 * @tc.desc: ConfigureStreams, streamInfo2.encodeType_ is ENCODE_TYPE_MPEG4_ES
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams001, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_0800
 * @tc.name: testDistributedCameraConfigureStreams002
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = "", streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams002, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_0900
 * @tc.name: testDistributedCameraConfigureStreams003
 * @tc.desc: ConfigureStreams, dhBase.dhId_ = "", streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams003, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1000
 * @tc.name: testDistributedCameraConfigureStreams004
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams004, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1100
 * @tc.name: testDistributedCameraConfigureStreams005
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams005, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1200
 * @tc.name: testDistributedCameraConfigureStreams006
 * @tc.desc: ConfigureStreams, dhBase.dhId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams006, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1300
 * @tc.name: testDistributedCameraConfigureStreams007
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams007, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1400
 * @tc.name: testDistributedCameraConfigureStreams008
 * @tc.desc: ConfigureStreams, streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams008, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1500
 * @tc.name: testDistributedCameraConfigureStreams009
 * @tc.desc: ConfigureStreams, streamInfo2.encodeType_ = ENCODE_TYPE_JPEG, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams009, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_JPEG;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1600
 * @tc.name: testDistributedCameraConfigureStreams010
 * @tc.desc: ConfigureStreams, dhBase.dhId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_JPEG, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams010, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_JPEG;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1700
 * @tc.name: testDistributedCameraConfigureStreams011
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_JPEG, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams011, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_JPEG;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1800
 * @tc.name: testDistributedCameraConfigureStreams012
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_JPEG, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams012, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_JPEG;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_1900
 * @tc.name: testDistributedCameraConfigureStreams013
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_H265, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams013, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_H265;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2000
 * @tc.name: testDistributedCameraConfigureStreams014
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_H265, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams014, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_H265;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2100
 * @tc.name: testDistributedCameraConfigureStreams015
 * @tc.desc: ConfigureStreams, dhBase.dhId = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_H265, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams015, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_H265;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2200
 * @tc.name: testDistributedCameraConfigureStreams016
 * @tc.desc: ConfigureStreams, dhBase.dhId = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_H264, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams016, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_H264;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2300
 * @tc.name: testDistributedCameraConfigureStreams017
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = "", dhBase.dhId = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_H264, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams017, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_H264;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2400
 * @tc.name: testDistributedCameraConfigureStreams018
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = "", dhBase.dhId = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_H264, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams018, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_H264;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2500
 * @tc.name: testDistributedCameraConfigureStreams019
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = "", dhBase.dhId = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_NULL, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams019, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2600
 * @tc.name: testDistributedCameraConfigureStreams020
 * @tc.desc: ConfigureStreams, dhBase.deviceId_ = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_NULL, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams020, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2700
 * @tc.name: testDistributedCameraConfigureStreams021
 * @tc.desc: ConfigureStreams, dhBase.dhId = ""
 *           streamInfo2.encodeType_ = ENCODE_TYPE_NULL, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams021, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2800
 * @tc.name: testDistributedCameraConfigureStreams022
 * @tc.desc: ConfigureStreams, streamInfo2.dataspace_ = 8
 *           streamInfo2.encodeType_ = ENCODE_TYPE_NULL, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams022, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 8;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_2900
 * @tc.name: testDistributedCameraConfigureStreams023
 * @tc.desc: ConfigureStreams, streamInfo2.format_ = -1, streamInfo2.dataspace_ = 8
 *           streamInfo2.encodeType_ = ENCODE_TYPE_NULL, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams023, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = -1;
    streamInfo2.dataspace_ = 8;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_3000
 * @tc.name: testDistributedCameraConfigureStreams024
 * @tc.desc: ConfigureStreams, streamInfo2.streamId_ = 2147483647, streamInfo2.dataspace_ = 8
 *           streamInfo2.encodeType_ = ENCODE_TYPE_NULL, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams024, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 2147483647;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 8;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_3100
 * @tc.name: testDistributedCameraConfigureStreams025
 * @tc.desc: ConfigureStreams, streamInfo2.streamId_ = 2147483647, streamInfo2.format_ = -1
 *           streamInfo2.encodeType_ = ENCODE_TYPE_NULL, streamInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams025, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 2147483647;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = -1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = CONTINUOUS_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_3200
 * @tc.name: testDistributedCameraConfigureStreams026
 * @tc.desc: ConfigureStreams, streamInfo2.streamId_ = 2147483647, streamInfo2.format_ = -1
 *           streamInfo2.encodeType_ = ENCODE_TYPE_NULL, streamInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams026, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 2147483647;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = -1;
    streamInfo2.dataspace_ = 1;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_3300
 * @tc.name: testDistributedCameraConfigureStreams027
 * @tc.desc: ConfigureStreams, streamInfo2.streamId_ = 2147483647, streamInfo2.dataspace_ = 8
 *           streamInfo2.encodeType_ = ENCODE_TYPE_NULL, streamInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams027, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 2147483647;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 8;
    streamInfo2.encodeType_ = ENCODE_TYPE_NULL;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_3400
 * @tc.name: testDistributedCameraConfigureStreams028
 * @tc.desc: ConfigureStreams, streamInfo2.streamId_ = 2147483647, streamInfo2.dataspace_ = 8
 *           streamInfo2.encodeType_ = ENCODE_TYPE_H264, streamInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams028, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 2147483647;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 8;
    streamInfo2.encodeType_ = ENCODE_TYPE_H264;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_3500
 * @tc.name: testDistributedCameraConfigureStreams029
 * @tc.desc: ConfigureStreams, streamInfo2.streamId_ = 2147483647, streamInfo2.dataspace_ = 8
 *           streamInfo2.encodeType_ = ENCODE_TYPE_H265, streamInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams029, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 2147483647;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 8;
    streamInfo2.encodeType_ = ENCODE_TYPE_H265;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_3600
 * @tc.name: testDistributedCameraConfigureStreams030
 * @tc.desc: ConfigureStreams, streamInfo2.streamId_ = 2147483647, streamInfo2.dataspace_ = 8
 *           streamInfo2.encodeType_ = ENCODE_TYPE_JPEG, streamInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams030, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 2147483647;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 8;
    streamInfo2.encodeType_ = ENCODE_TYPE_JPEG;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_3700
 * @tc.name: testDistributedCameraConfigureStreams031
 * @tc.desc: ConfigureStreams, streamInfo2.streamId_ = 2147483647, streamInfo2.dataspace_ = 8
 *           streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES, streamInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams031, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = 2147483647;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 8;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ConfigureStreams_3800
 * @tc.name: testDistributedCameraConfigureStreams032
 * @tc.desc: ConfigureStreams, streamInfo2.streamId_ = -1, streamInfo2.dataspace_ = 8
 *           streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES, streamInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraConfigureStreams032, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamInfo2.streamId_ = -1;
    streamInfo2.width_ = TEST_WIDTH;
    streamInfo2.height_ = TEST_HEIGTH;
    streamInfo2.stride_ = 1;
    streamInfo2.format_ = 1;
    streamInfo2.dataspace_ = 8;
    streamInfo2.encodeType_ = ENCODE_TYPE_MPEG4_ES;
    streamInfo2.type_ = SNAPSHOT_FRAME;
    streamInfos1.clear();
    streamInfos1.push_back(streamInfo2);
    int32_t ret = cameraProvider.ConfigureStreams(dhBase, streamInfos1);
    EXPECT_NE(0, ret) << "ConfigureStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_0200
 * @tc.name: testDistributedCameraReleaseStreams001
 * @tc.desc: ReleaseStreams, streamIds.push_back(-2147483648)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams001, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamIds.pop_back();
    streamIds.push_back(-2147483648);
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_0300
 * @tc.name: testDistributedCameraReleaseStreams002
 * @tc.desc: ReleaseStreams, dhBase.deviceId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams002, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_0400
 * @tc.name: testDistributedCameraReleaseStreams003
 * @tc.desc: ReleaseStreams, dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams003, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_0500
 * @tc.name: testDistributedCameraReleaseStreams004
 * @tc.desc: ReleaseStreams, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams004, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_0600
 * @tc.name: testDistributedCameraReleaseStreams005
 * @tc.desc: ReleaseStreams, dhBase.deviceId_ = "", dhBase.dhId_ = "" , streamIds.push_back(0);
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams005, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamIds.pop_back();
    streamIds.push_back(0);
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_0700
 * @tc.name: testDistributedCameraReleaseStreams006
 * @tc.desc: ReleaseStreams, dhBase.dhId_ = "" , streamIds.push_back(0);
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams006, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_0800
 * @tc.name: testDistributedCameraReleaseStreams007
 * @tc.desc: ReleaseStreams, dhBase.deviceId_ = "", streamIds.push_back(0);
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams007, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_0900
 * @tc.name: testDistributedCameraReleaseStreams008
 * @tc.desc: ReleaseStreams, dhBase.deviceId_ = "", streamIds.push_back(-1)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams008, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    streamIds.pop_back();
    streamIds.push_back(-1);
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1000
 * @tc.name: testDistributedCameraReleaseStreams009
 * @tc.desc: ReleaseStreams, streamIds.push_back(-1)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams009, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1100
 * @tc.name: testDistributedCameraReleaseStreams010
 * @tc.desc: ReleaseStreams, dhBase.dhId_ = "", streamIds.push_back(-1)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams010, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1200
 * @tc.name: testDistributedCameraReleaseStreams011
 * @tc.desc: ReleaseStreams, dhBase.deviceId_ = "", dhBase.dhId_ = "", streamIds.push_back(-1)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams011, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1300
 * @tc.name: testDistributedCameraReleaseStreams012
 * @tc.desc: ReleaseStreams, streamIds.push_back(2147483647)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams012, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamIds.pop_back();
    streamIds.push_back(2147483647);
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1400
 * @tc.name: testDistributedCameraReleaseStreams013
 * @tc.desc: ReleaseStreams, dhBase.dhId_ = "", streamIds.push_back(2147483647)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams013, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1500
 * @tc.name: testDistributedCameraReleaseStreams014
 * @tc.desc: ReleaseStreams, dhBase.deviceId_ = "", streamIds.push_back(2147483647)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams014, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1600
 * @tc.name: testDistributedCameraReleaseStreams015
 * @tc.desc: ReleaseStreams, dhBase.dhId_ = "", dhBase.deviceId_ = "", streamIds.push_back(2147483647)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams015, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1700
 * @tc.name: testDistributedCameraReleaseStreams016
 * @tc.desc: ReleaseStreams, dhBase.dhId_ = "", dhBase.deviceId_ = "", streamIds.push_back(-2147483648)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams016, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamIds.pop_back();
    streamIds.push_back(-2147483648);
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1800
 * @tc.name: testDistributedCameraReleaseStreams017
 * @tc.desc: ReleaseStreams, dhBase.deviceId_ = "", streamIds.push_back(-2147483648)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams017, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_ReleaseStreams_1900
 * @tc.name: testDistributedCameraReleaseStreams018
 * @tc.desc: ReleaseStreams, dhBase.dhId_ = "", streamIds.push_back(-2147483648)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraReleaseStreams018, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.ReleaseStreams(dhBase, streamIds);
    EXPECT_NE(0, ret) << "ReleaseStreams fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_0500
 * @tc.name: testDistributedCameraStartCapture001
 * @tc.desc: StartCapture, dhBase.deviceId_ = "",
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H265, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture001, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H265;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_0600
 * @tc.name: testDistributedCameraStartCapture002
 * @tc.desc: StartCapture, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H265, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture002, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H265;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_0700
 * @tc.name: testDistributedCameraStartCapture003
 * @tc.desc: StartCapture, dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H265, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture003, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H265;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_0800
 * @tc.name: testDistributedCameraStartCapture004
 * @tc.desc: StartCapture, dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H265, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture004, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H265;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_0900
 * @tc.name: testDistributedCameraStartCapture005
 * @tc.desc: StartCapture, dhBase.deviceId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H265, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture005, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H265;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1000
 * @tc.name: testDistributedCameraStartCapture006
 * @tc.desc: StartCapture, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H265, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture006, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H265;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1100
 * @tc.name: testDistributedCameraStartCapture007
 * @tc.desc: StartCapture, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H264, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture007, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H264;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1200
 * @tc.name: testDistributedCameraStartCapture008
 * @tc.desc: StartCapture, dhBase.deviceId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H264, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture008, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H264;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1300
 * @tc.name: testDistributedCameraStartCapture009
 * @tc.desc: StartCapture, dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H264, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture009, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H264;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1400
 * @tc.name: testDistributedCameraStartCapture010
 * @tc.desc: StartCapture, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H264, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture010, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H264;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1500
 * @tc.name: testDistributedCameraStartCapture011
 * @tc.desc: StartCapture, dhBase.deviceId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H264, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture011, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H264;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1600
 * @tc.name: testDistributedCameraStartCapture012
 * @tc.desc: StartCapture, dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_H264, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture012, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_H264;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1700
 * @tc.name: testDistributedCameraStartCapture013
 * @tc.desc: StartCapture, dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_NULL, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture013, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_NULL;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1800
 * @tc.name: testDistributedCameraStartCapture014
 * @tc.desc: StartCapture, dhBase.dhId_ = "", dhBase.deviceId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_NULL, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture014, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_NULL;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_1900
 * @tc.name: testDistributedCameraStartCapture015
 * @tc.desc: StartCapture, dhBase.deviceId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_NULL, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture015, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_NULL;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_2000
 * @tc.name: testDistributedCameraStartCapture016
 * @tc.desc: StartCapture, captureInfo2.encodeType_ = ENCODE_TYPE_NULL, captureInfo2.type_ = CONTINUOUS_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture016, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_NULL;
    captureInfo2.type_ = CONTINUOUS_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_2100
 * @tc.name: testDistributedCameraStartCapture017
 * @tc.desc: StartCapture, captureInfo2.encodeType_ = ENCODE_TYPE_NULL, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture017, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_NULL;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_2200
 * @tc.name: testDistributedCameraStartCapture018
 * @tc.desc: StartCapture, dhBase.deviceId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_NULL, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture018, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_NULL;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_2300
 * @tc.name: testDistributedCameraStartCapture019
 * @tc.desc: StartCapture, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_NULL, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture019, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_NULL;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_2400
 * @tc.name: testDistributedCameraStartCapture020
 * @tc.desc: StartCapture, dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_NULL, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture020, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_NULL;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_2500
 * @tc.name: testDistributedCameraStartCapture021
 * @tc.desc: StartCapture, dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_JPEG, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture021, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_JPEG;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_2600
 * @tc.name: testDistributedCameraStartCapture022
 * @tc.desc: StartCapture, dhBase.deviceId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_JPEG, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture022, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_JPEG;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_2700
 * @tc.name: testDistributedCameraStartCapture023
 * @tc.desc: StartCapture, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 *           captureInfo2.encodeType_ = ENCODE_TYPE_JPEG, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture023, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_JPEG;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StartCapture_2800
 * @tc.name: testDistributedCameraStartCapture024
 *           StartCapture, captureInfo2.encodeType_ = ENCODE_TYPE_JPEG, captureInfo2.type_ = SNAPSHOT_FRAME
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStartCapture024, Function | MediumTest | Level1)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    captureInfo2.streamIds_.push_back(1);
    captureInfo2.width_ = TEST_WIDTH;
    captureInfo2.height_ = TEST_HEIGTH;
    captureInfo2.stride_ = 1;
    captureInfo2.format_ = 1;
    captureInfo2.dataspace_ = 1;
    captureInfo2.encodeType_ = ENCODE_TYPE_JPEG;
    captureInfo2.type_ = SNAPSHOT_FRAME;
    captureInfos1.clear();
    captureInfos1.push_back(captureInfo2);
    int32_t ret = cameraProvider.StartCapture(dhBase, captureInfos1);
    EXPECT_EQ(INVALID_ARGUMENT, ret) << "StartCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StopCapture_0200
 * @tc.name: testDistributedCameraStopCapture001
 * @tc.desc: StopCapture, dhBase.deviceId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStopCapture001, Function | MediumTest | Level1)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.StopCapture(dhBase, streamIds);
    EXPECT_NE(0, ret) << "StopCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StopCapture_0300
 * @tc.name: testDistributedCameraStopCapture002
 * @tc.desc: StopCapture, dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStopCapture002, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.StopCapture(dhBase, streamIds);
    EXPECT_NE(0, ret) << "StopCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StopCapture_0400
 * @tc.name: testDistributedCameraStopCapture003
 * @tc.desc: StopCapture, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStopCapture003, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.StopCapture(dhBase, streamIds);
    EXPECT_NE(0, ret) << "StopCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StopCapture_0500
 * @tc.name: testDistributedCameraStopCapture004
 * @tc.desc: StopCapture, streamIds.push_back(-1)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStopCapture004, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamIds.pop_back();
    streamIds.push_back(-1);
    int32_t ret = cameraProvider.StopCapture(dhBase, streamIds);
    EXPECT_NE(0, ret) << "StopCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StopCapture_0600
 * @tc.name: testDistributedCameraStopCapture005
 * @tc.desc: StopCapture, dhBase.deviceId_ = "", dhBase.dhId_ = "", streamIds.push_back(-1)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStopCapture005, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamIds.pop_back();
    streamIds.push_back(-1);
    int32_t ret = cameraProvider.StopCapture(dhBase, streamIds);
    EXPECT_NE(0, ret) << "StopCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StopCapture_0700
 * @tc.name: testDistributedCameraStopCapture006
 * @tc.desc: StopCapture, streamIds.push_back(0)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStopCapture006, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    streamIds.pop_back();
    streamIds.push_back(0);
    int32_t ret = cameraProvider.StopCapture(dhBase, streamIds);
    EXPECT_NE(0, ret) << "StopCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_StopCapture_0800
 * @tc.name: testDistributedCameraStopCapture007
 * @tc.desc: StopCapture, dhBase.deviceId_ = "", dhBase.dhId_ = "", streamIds.push_back(0)
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraStopCapture007, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    streamIds.pop_back();
    streamIds.push_back(0);
    int32_t ret = cameraProvider.StopCapture(dhBase, streamIds);
    EXPECT_NE(0, ret) << "StopCapture fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_0300
 * @tc.name: testDistributedCameraUpdateSettings001
 * @tc.desc: UpdateSettings, dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings001, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_0400
 * @tc.name: testDistributedCameraUpdateSettings002
 * @tc.desc: UpdateSettings, dhBase.deviceId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings002, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_0500
 * @tc.name: testDistributedCameraUpdateSettings003
 * @tc.desc: UpdateSettings, dhBase.deviceId_ = "", dhBase.dhId_ = ""
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings003, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_.clear();
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_0600
 * @tc.name: testDistributedCameraUpdateSettings004
 * @tc.desc: UpdateSettings, result.type_ = UPDATE_METADATA
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings004, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    result.type_ = UPDATE_METADATA;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_0700
 * @tc.name: testDistributedCameraUpdateSettings005
 * @tc.desc: UpdateSettings, dhBase.dhId_ = "", result.type_ = UPDATE_METADATA
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings005, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    result.type_ = UPDATE_METADATA;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_0800
 * @tc.name: testDistributedCameraUpdateSettings006
 * @tc.desc: UpdateSettings, dhBase.deviceId_ = "", result.type_ = UPDATE_METADATA
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings006, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    result.type_ = UPDATE_METADATA;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_0900
 * @tc.name: testDistributedCameraUpdateSettings007
 * @tc.desc: UpdateSettings, dhBase.deviceId_ = "", result.type_ = ENABLE_METADATA
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings007, Function | MediumTest | Level2)
{
    dhBase.deviceId_.clear();
    dhBase.dhId_ = testCameraDh;
    result.type_ = ENABLE_METADATA;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_1000
 * @tc.name: testDistributedCameraUpdateSettings008
 * @tc.desc: UpdateSettings, dhBase.dhId_ = "", result.type_ = ENABLE_METADATA
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings008, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    result.type_ = ENABLE_METADATA;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_1100
 * @tc.name: testDistributedCameraUpdateSettings009
 * @tc.desc: UpdateSettings, result.type_ = ENABLE_METADATA
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings009, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    result.type_ = ENABLE_METADATA;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_1200
 * @tc.name: testDistributedCameraUpdateSettings010
 * @tc.desc: UpdateSettings, result.type_ = DISABLE_METADATA
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings010, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    result.type_ = DISABLE_METADATA;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_1300
 * @tc.name: testDistributedCameraUpdateSettings011
 * @tc.desc: UpdateSettings, result.type_ = SET_FLASH_LIGHT
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings011, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    result.type_ = SET_FLASH_LIGHT;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_1400
 * @tc.name: testDistributedCameraUpdateSettings012
 * @tc.desc: UpdateSettings, result.type_ = FPS_RANGE
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings012, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    result.type_ = FPS_RANGE;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_1500
 * @tc.name: testDistributedCameraUpdateSettings013
 * @tc.desc: UpdateSettings, result.type_ = UPDATE_FRAME_METADATA
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings013, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_ = testCameraDh;
    result.type_ = UPDATE_FRAME_METADATA;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

/**
 * @tc.number: SUB_DH_Dcamera_HDI_UpdateSettings_1600
 * @tc.name: testDistributedCameraUpdateSettings014
 * @tc.desc: UpdateSettings, dhBase.dhId_ = "", result.type_ = UPDATE_FRAME_METADATA
 */
HWTEST_F(DCameraProviderTest, testDistributedCameraUpdateSettings014, Function | MediumTest | Level2)
{
    dhBase.deviceId_ = testDeviceId;
    dhBase.dhId_.clear();
    result.type_ = UPDATE_FRAME_METADATA;
    result.value_ = "SettingValue";
    settings.pop_back();
    settings.push_back(result);
    int32_t ret = cameraProvider.UpdateSettings(dhBase, settings);
    EXPECT_NE(0, ret) << "UpdateSettings fail";
}

} // namespace DistributedHardware
} // namespace OHOS