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

#ifndef EFFECT_COMMON_H
#define EFFECT_COMMON_H

#include "v1_0/effect_types.h"

void EffectControllerReleaseDesc(struct EffectControllerDescriptor *desc);
void EffectControllerReleaseDescs(struct EffectControllerDescriptor *descs, const uint32_t *descsLen);

#endif