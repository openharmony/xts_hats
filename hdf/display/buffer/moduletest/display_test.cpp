/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "display_buffer_mt.h"

#include <unistd.h>
#include <vector>
#include <thread>
#include <cinttypes>
#include <securec.h>
#include <cstring>

#include "gtest/gtest.h"
#include "v1_0/display_composer_type.h"
#include "hdf_base.h"
#include "hdf_log.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace TEST {
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS;
using namespace testing::ext;

#define HDF_LOG_TAG display_buffer_module_test

void DisplayBuffer::SetUpTestCase() {}

#ifdef DISPLAY_COMMUNITY
HWTEST_F(DisplayBuffer, DisplayTest_0010, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispErrCode::DISPLAY_FD_ERR;
    EXPECT_EQ(-2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0020, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispErrCode::DISPLAY_PARAM_ERR;
    EXPECT_EQ(-3, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0030, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispErrCode::DISPLAY_NOT_SUPPORT;
    EXPECT_EQ(-5, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0040, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispErrCode::DISPLAY_SYS_BUSY;
    EXPECT_EQ(-7, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0050, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::DispErrCode::DISPLAY_NOT_PERM;
    EXPECT_EQ(-8, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0060, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::LayerType::LAYER_TYPE_GRAPHIC;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0070, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::LayerType::LAYER_TYPE_OVERLAY;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0080, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::LayerType::LAYER_TYPE_SDIEBAND;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0090, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::LayerType::LAYER_TYPE_BUTT;
    EXPECT_EQ(4, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0100, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_BLACK;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0110, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_NOTMERGEPEN;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0120, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_MASKNOTPEN;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0130, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_NOTCOPYPEN;
    EXPECT_EQ(3, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0140, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_MASKPENNOT;
    EXPECT_EQ(4, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0150, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_NOT;
    EXPECT_EQ(5, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0160, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_XORPEN;
    EXPECT_EQ(6, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0170, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_NOTMASKPEN;
    EXPECT_EQ(7, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0180, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_MASKPEN;
    EXPECT_EQ(8, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0190, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_NOTXORPEN;
    EXPECT_EQ(9, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0200, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_NOP;
    EXPECT_EQ(10, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0210, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_MERGENOTPEN;
    EXPECT_EQ(11, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0220, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_COPYPE;
    EXPECT_EQ(12, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0230, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_MERGEPENNOT;
    EXPECT_EQ(13, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0240, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_MERGEPEN;
    EXPECT_EQ(14, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0250, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_WHITE;
    EXPECT_EQ(15, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0260, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::RopType::ROP_BUTT;
    EXPECT_EQ(16, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0270, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorKey::CKEY_NONE;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0280, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorKey::CKEY_SRC;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0290, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorKey::CKEY_DST;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0300, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorKey::CKEY_BUTT;
    EXPECT_EQ(3, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0310, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::MirrorType::MIRROR_NONE;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0320, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::MirrorType::MIRROR_LR;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0330, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::MirrorType::MIRROR_TB;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0340, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::MirrorType::MIRROR_BUTT;
    EXPECT_EQ(3, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0350, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::Connection::CON_INVALID;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0360, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::Connection::CONNECTED;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0370, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::Connection::DISCONNECTED;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0380, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_HDMI;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0390, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_LCD;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0400, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_BT1120;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0410, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_BT656;
    EXPECT_EQ(3, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0420, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_YPBPR;
    EXPECT_EQ(4, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0430, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_RGB;
    EXPECT_EQ(5, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0440, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_CVBS;
    EXPECT_EQ(6, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0450, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_SVIDEO;
    EXPECT_EQ(7, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0460, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_VGA;
    EXPECT_EQ(8, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0470, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_MIPI;
    EXPECT_EQ(9, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0480, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_PANEL;
    EXPECT_EQ(10, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0490, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::InterfaceType::DISP_INTF_BUTT;
    EXPECT_EQ(11, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0500, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_INVALID;
    EXPECT_EQ(-1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0510, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_NATIVE;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0520, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_STANDARD_BT601;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0530, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_STANDARD_BT709;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0540, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_DCI_P3;
    EXPECT_EQ(3, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0550, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_SRGB;
    EXPECT_EQ(4, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0560, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_ADOBE_RGB;
    EXPECT_EQ(5, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0570, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_DISPLAY_P3;
    EXPECT_EQ(6, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0580, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_BT2020;
    EXPECT_EQ(7, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0590, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_BT2100_PQ;
    EXPECT_EQ(8, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0600, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_BT2100_HLG;
    EXPECT_EQ(9, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0610, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorGamut::COLOR_GAMUT_DISPLAY_BT2020;
    EXPECT_EQ(10, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0620, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::GamutMap::GAMUT_MAP_CONSTANT;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0630, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::GamutMap::GAMUT_MAP_EXPANSION;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0640, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::GamutMap::GAMUT_MAP_HDR_CONSTANT;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0650, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::GamutMap::GAMUT_MAP_HDR_EXPANSION;
    EXPECT_EQ(3, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0660, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::COLOR_DATA_SPACE_UNKNOWN;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0670, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_BT601;
    EXPECT_EQ(0x00000001, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0680, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_BT709;
    EXPECT_EQ(0x00000002, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0690, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_DCI_P3;
    EXPECT_EQ(0x00000003, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0700, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_SRGB;
    EXPECT_EQ(0x00000004, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0710, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_ADOBE_RGB;
    EXPECT_EQ(0x00000005, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0720, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_DISPLAY_P3;
    EXPECT_EQ(0x00000006, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0730, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_BT2020;
    EXPECT_EQ(0x00000007, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0740, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_BT2100_PQ;
    EXPECT_EQ(0x00000008, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0750, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_BT2100_HLG;
    EXPECT_EQ(0x00000009, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0760, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::GAMUT_DISPLAY_BT2020;
    EXPECT_EQ(0x0000000a, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0770, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::TRANSFORM_FUNC_UNSPECIFIED;
    EXPECT_EQ(0x00000100, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0780, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::TRANSFORM_FUNC_LINEAR;
    EXPECT_EQ(0x00000200, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0790, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::TRANSFORM_FUNC_SRGB;
    EXPECT_EQ(0x00000300, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0800, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::TRANSFORM_FUNC_SMPTE_170M;
    EXPECT_EQ(0x00000400, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0810, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::TRANSFORM_FUNC_GM2_2;
    EXPECT_EQ(0x00000500, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0820, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::TRANSFORM_FUNC_GM2_6;
    EXPECT_EQ(0x00000600, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0830, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::TRANSFORM_FUNC_GM2_8;
    EXPECT_EQ(0x00000700, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0840, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::TRANSFORM_FUNC_ST2084;
    EXPECT_EQ(0x00000800, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0850, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::TRANSFORM_FUNC_HLG;
    EXPECT_EQ(0x00000900, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0860, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::PRECISION_UNSPECIFIED;
    EXPECT_EQ(0x00010000, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0870, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::PRECISION_FULL;
    EXPECT_EQ(0x00020000, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0880, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::PRESION_LIMITED;
    EXPECT_EQ(0x00030000, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0890, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::PRESION_EXTENDED;
    EXPECT_EQ(0x00040000, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0900, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT601_SMPTE170M_FULL;
    EXPECT_EQ(1 | 1024 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0910, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT601_SMPTE170M_LIMITED;
    EXPECT_EQ(1 | 1024 | 196608, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0920, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT709_LINEAR_FULL;
    EXPECT_EQ(2 | 512 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0930, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT709_LINEAR_EXTENDED;
    EXPECT_EQ(2 | 512 | 262144, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0940, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT709_SRGB_FULL;
    EXPECT_EQ(2 | 768 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0950, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT709_SRGB_EXTENDED;
    EXPECT_EQ(2 | 768 | 262144, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0960, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT709_SMPTE170M_LIMITED;
    EXPECT_EQ(2 | 1024 | 196608, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0970, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::DCI_P3_LINEAR_FULL;
    EXPECT_EQ(3 | 512 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0980, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::DCI_P3_GAMMA26_FULL;
    EXPECT_EQ(3 | 1536 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_0990, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::DISPLAY_P3_LINEAR_FULL;
    EXPECT_EQ(6 | 512 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1000, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::DCI_P3_SRGB_FULL;
    EXPECT_EQ(3 | 768 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1010, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::ADOBE_RGB_GAMMA22_FULL;
    EXPECT_EQ(5 | 1280 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1020, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT2020_LINEAR_FULL;
    EXPECT_EQ(7 | 512 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1030, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT2020_SRGB_FULL;
    EXPECT_EQ(7 | 768 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1040, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT2020_SMPTE170M_FULL;
    EXPECT_EQ(7 | 1024 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1050, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT2020_ST2084_FULL;
    EXPECT_EQ(7 | 2048 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1060, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT2020_HLG_FULL;
    EXPECT_EQ(7 | 2304 | 131072, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1070, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::ColorDataSpace::BT2020_ST2084_LIMITED;
    EXPECT_EQ(7 | 2048 | 196608, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1080, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRFormat::NOT_SUPPORT_HDR;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1090, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRFormat::DOLBY_VISION;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1110, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRFormat::HDR10;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1120, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRFormat::HLG;
    EXPECT_EQ(3, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1130, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRFormat::HDR10_PLUS;
    EXPECT_EQ(4, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1140, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRFormat::HDR_VIVID;
    EXPECT_EQ(5, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1150, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_RED_PRIMARY_X;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1160, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_RED_PRIMARY_Y;
    EXPECT_EQ(1, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1170, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_GREEN_PRIMARY_X;
    EXPECT_EQ(2, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1180, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_GREEN_PRIMARY_Y;
    EXPECT_EQ(3, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1190, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_BLUE_PRIMARY_X;
    EXPECT_EQ(4, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1200, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_BLUE_PRIMARY_Y;
    EXPECT_EQ(5, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1210, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_WHITE_PRIMARY_X;
    EXPECT_EQ(6, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1220, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_WHITE_PRIMARY_Y;
    EXPECT_EQ(7, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1230, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_MAX_LUMINANCE;
    EXPECT_EQ(8, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1240, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_MIN_LUMINANCE;
    EXPECT_EQ(9, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1250, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_MAX_CONTENT_LIGHT_LEVEL;
    EXPECT_EQ(10, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1260, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_MAX_FRAME_AVERAGE_LIGHT_LEVEL;
    EXPECT_EQ(11, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1270, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_HDR10_PLUS;
    EXPECT_EQ(12, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1280, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::HDRMetadataKey::MATAKEY_HDR_VIVID;
    EXPECT_EQ(13, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1290, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::PresentTimestampType::HARDWARE_DISPLAY_PTS_UNSUPPORTED;
    EXPECT_EQ(0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1300, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::PresentTimestampType::HARDWARE_DISPLAY_PTS_DELAY;
    EXPECT_EQ(1 << 0, ret);
}

HWTEST_F(DisplayBuffer, DisplayTest_1310, TestSize.Level1)
{
    int ret;
    ret = OHOS::HDI::Display::Composer::V1_0::PresentTimestampType::HARDWARE_DISPLAY_PTS_TIMESTAMP;
    EXPECT_EQ(1 << 1, ret);
}

#endif // DISPLAY_COMMUNITY
} // OHOS
} // HDI
} // DISPLAY
} // TEST