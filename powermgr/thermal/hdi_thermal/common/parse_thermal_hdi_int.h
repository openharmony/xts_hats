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

#ifndef XTS_HATS_PARSE_THERMAL_HDI_INT_H
#define XTS_HATS_PARSE_THERMAL_HDI_INT_H

#include <charconv>
#include <cstdint>
#include <string>
#include <string_view>
#include <system_error>

namespace OHOS {
namespace ThermalHdiTest {
/*
 * Parse a whole-token decimal int32 from thermal HDI sysfs/text.
 * Leading/trailing whitespace (including the trailing newline sysfs files
 * typically include) is trimmed first. After trim, leftover partial tokens
 * such as "1994100abc" are rejected. Valid in-range values keep the same
 * numeric result as std::stoi on fully-consumed digit-only input.
 */
inline std::string_view TrimThermalHdiIntText(std::string_view text)
{
    const char *first = text.data();
    const char *last = first + text.size();
    while (first < last && (*first == ' ' || *first == '\t' || *first == '\n' || *first == '\r')) {
        ++first;
    }
    while (last > first && (*(last - 1) == ' ' || *(last - 1) == '\t' || *(last - 1) == '\n' ||
        *(last - 1) == '\r')) {
        --last;
    }
    return std::string_view(first, static_cast<size_t>(last - first));
}

inline bool ParseThermalHdiInt32(std::string_view text, int32_t &out)
{
    text = TrimThermalHdiIntText(text);
    if (text.empty()) {
        return false;
    }
    int32_t value = 0;
    auto result = std::from_chars(text.data(), text.data() + text.size(), value);
    if (result.ec != std::errc() || result.ptr != text.data() + text.size()) {
        return false;
    }
    out = value;
    return true;
}

inline bool ParseThermalHdiInt32(const std::string &text, int32_t &out)
{
    return ParseThermalHdiInt32(std::string_view(text), out);
}
} // namespace ThermalHdiTest
} // namespace OHOS
#endif // XTS_HATS_PARSE_THERMAL_HDI_INT_H
