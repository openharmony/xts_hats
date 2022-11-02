/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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
#include <cinttypes>
#include <climits>
#include <cstdio>
#include <unistd.h>
#include <string>

#define SIZE_ALIGN (4 * sizeof(size_t))
#define THRESHOLD (0x1c00 * SIZE_ALIGN)
const int ITER_TIME = 20;
const int TIME_SPEC_LEN = 2;

using namespace testing::ext;
using namespace std;

class MallocPressure : public testing::Test {
public:

    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};

void MallocPressure::SetUp()
{
}
void MallocPressure::TearDown()
{
}
void MallocPressure::SetUpTestCase()
{
}
void MallocPressure::TearDownTestCase()
{
}

/*
 * @tc.number CAM_FREE_FUN_0100
 * @tc.name Apply part of the memory camfree reduction
 * @tc.desc Test the basic features of CamFree
*/
HWTEST_F(MallocPressure, pressureTest0100, Function | MediumTest | Level1)
{
    struct timespec ts[TIME_SPEC_LEN];
    clock_gettime(CLOCK_REALTIME, &ts[0]);
    for (int i = 0; i < ITER_TIME; ++i) {
        for (size_t size = 0; size < THRESHOLD; size += SIZE_ALIGN + 1) {
            void *ptr = malloc(size);
            if (!ptr) {
                printf("Malloc failed for size %zu\n", size);
                ASSERT_TRUE(true) << "malloc error = " << errno;
            }
            free(ptr);
            ASSERT_TRUE(true) << "Abnormal operation of the system, errno" << errno;
        }
    }
}

