/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef OHOS_HDI_SENSOR_V3_0_SENSORCALLBACKIMPL_H
#define OHOS_HDI_SENSOR_V3_0_SENSORCALLBACKIMPL_H

#include <hdf_base.h>
#include "v3_0/isensor_plug_callback.h"
#include "hdf_log.h"

namespace OHOS {
namespace HDI {
namespace Sensor {
namespace V3_0 {
class SensorPlugCallbackImpl : public ISensorPlugCallback {
public:
    virtual ~SensorPlugCallbackImpl() {}

    int32_t OnSensorPlugEvent(const SensorPlugInfo& sensorPlugInfo) override
    {
        SensorHandle sensorHandle = sensorPlugInfo.deviceSensorInfo;
        std::string deviceSensorInfo = ("deviceId" + std::to_string((sensorHandle).deviceId) + "sensorType" +
            std::to_string((sensorHandle).sensorType) + "sensorId" + std::to_string((sensorHandle).sensorId) +
            "location" + std::to_string((sensorHandle).location));
        printf("OnSensorPlugEvent deviceSensorInfo[%s]deviceName[%s]status[%d]reserved[%d]",
               deviceSensorInfo.c_str(), sensorPlugInfo.deviceName.c_str(),
               sensorPlugInfo.status, sensorPlugInfo.reserved);
        HDF_LOGI("OnSensorPlugEvent deviceSensorInfo[%{public}s]deviceName[%{public}s]status[%{public}d]"
                 "reserved[%{public}d]",
                 deviceSensorInfo.c_str(), sensorPlugInfo.deviceName.c_str(),
                 sensorPlugInfo.status, sensorPlugInfo.reserved);
        return HDF_SUCCESS;
    }
};
} // V3_0
} // Sensor
} // HDI
} // OHOS

#endif // OHOS_HDI_SENSOR_V1_1_SENSORCALLBACKIMPL_H
