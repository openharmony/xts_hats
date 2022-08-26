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
#ifndef FINGERPRINT_AUTH_HDI_H
#define FINGERPRINT_AUTH_HDI_H

#include <cstddef>
#include <cstdint>
#include <cinttypes>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "parcel.h"
#include "executor_impl.h"
#include "fingerprint_auth_defines.h"
#include "fingerprint_auth_interface_service.h"

class UserIamFingerprintAuthTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

class FingerPrintTypeTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
#endif
