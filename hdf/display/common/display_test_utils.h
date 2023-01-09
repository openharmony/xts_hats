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

#ifndef DISPLAY_TEST_UTILS_H
#define DISPLAY_TEST_UTILS_H
#include <vector>
#include "display_test.h"
namespace OHOS {
namespace HDI {
namespace DISPLAY {
namespace TEST {
void SaveFile(const char *fileName, uint8_t *data, int size);
void SetUint32(uint32_t &dst, uint32_t value);
void SetPixel(const BufferHandle &handle, int x, int y, uint32_t color);
void ClearColor(const BufferHandle &handle, uint32_t color);
uint32_t GetPixelValue(const BufferHandle &handle, int x, int y);
uint32_t CheckPixel(const BufferHandle &handle, int x, int y, uint32_t color);
std::vector<IRect> SplitBuffer(const BufferHandle &handle, std::vector<uint32_t> &colors);

class HdiDisplay {
public:
    static int32_t SetDisplayBacklight(uint32_t devId, uint32_t &level) {
        return DISPLAY_NOT_SUPPORT;
    };
	static int32_t SetDisplayClientCrop(uint32_t devId, IRect *rect) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t SetDisplayClientDestRect(uint32_t devId, IRect *rect) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t SetDisplayMode(uint32_t devId, uint32_t modeId) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t SetDisplayPowerStatus(uint32_t devId, DispPowerStatus status) {
		return DISPLAY_NOT_SUPPORT;
	};	
    static int32_t GetDisplayBacklight(uint32_t devId, uint32_t &level) {
		return DISPLAY_NOT_SUPPORT;
	};
	static int32_t SetDisplayProperty(uint32_t devId, uint32_t id, uint64_t value) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t DestroyVirtualDisplay(uint32_t devId) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t GetDisplayPowerStatus(uint32_t devId, DispPowerStatus status) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t GetDisplayProperty(uint32_t devId, uint32_t id, uint64_t value) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t SetVirtualDisplayBuffer(uint32_t devId, BufferHandle *buffer, int32_t fence) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t DestroyLayer(uint32_t devId, uint32_t layerId) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t InitCmdRequest(uint32_t &request) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t CreateVirtualDisplay(uint32_t width, uint32_t height, int32_t format, uint32_t devId) {
		return DISPLAY_NOT_SUPPORT;
	};	
	static int32_t CmdRequest(uint32_t inEleCnt, uint32_t &inFds, uint32_t &outEleCnt) {
		return DISPLAY_NOT_SUPPORT;
	};	
    static int32_t GetCmdReply(int32_t reply) {
		return DISPLAY_NOT_SUPPORT;
	};	
};

class DisplayRegisterCallbackBase {
public:
    static int32_t OnHotplugIn(uint32_t outputId, bool connected) {
		return DISPLAY_NOT_SUPPORT;
	};	
    static int32_t OnVBlankCallback(unsigned int sequence, uint64_t ns) {
		return DISPLAY_NOT_SUPPORT;
	};	
    static int32_t OnRefreshCallback(uint32_t devId) {
		return DISPLAY_NOT_SUPPORT;
	};
};
} // OHOS
} // HDI
} // DISPLAY
} // TEST


#endif // HDI_TEST_RENDER_UTILS_H
