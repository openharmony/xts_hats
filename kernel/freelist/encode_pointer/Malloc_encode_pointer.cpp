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


using namespace testing::ext;
using namespace std;

class MallocEncodePointer : public testing::Test {
public:

    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};

void MallocEncodePointer::SetUp()
{
}
void MallocEncodePointer::TearDown()
{
}
void MallocEncodePointer::SetUpTestCase()
{
}
void MallocEncodePointer::TearDownTestCase()
{
}


/*
 * @tc.number CAM_FREE_FUN_0100
 * @tc.name Apply part of the memory camfree reduction
 * @tc.desc Test the basic features of CamFree
*/
HWTEST_F(MallocEncodePointer, freelist0100, Function | MediumTest | Level1)
{
uintptr_t *c0 = (uintptr_t *)malloc(sizeof(uintptr_t) * 10);
    if (!c0) {
        printf("Malloc c0 failed: %s\n", strerror(errno));
    }
    /* Malloc dividing chunk to avoid combination of neighbouring freed chunk */
    void *d0 = malloc(sizeof(uintptr_t) * 10);
    if (!d0) {
        printf("Malloc d0 failed: %s\n", strerror(errno));
    }

    uintptr_t *c1 = (uintptr_t *)malloc(sizeof(uintptr_t) * 10);
    if (!c1) {
        printf("Malloc c1 failed: %s\n", strerror(errno));
    }
    /* Malloc dividing chunk to avoid combination of neighbouring freed chunk */
    void *d1 = malloc(sizeof(uintptr_t) * 10);
    if (!d1) {
        printf("Malloc d1 failed: %s\n", strerror(errno));
    }

    
    /* Free the chunk, with same size, they're put into the same bin */
    /*    --------      --------      --------
     *    |  c0  |      |  c1  |      |  bin |
     * -->| next |----->| next |----->| next |-----
     * |  | prev |<-----| prev |<-----| prev |    |
     * |  -------       -------       -------     |
     * --------------------------------------------
     */
    free(c0);
    free(c1);

    uintptr_t xoraddr = c0[0]; /* Get the next of c0 */
    uintptr_t realaddr = reinterpret_cast<uintptr_t>(reinterpret_cast<char *>(c1) - sizeof(uintptr_t) * 2);
    ASSERT_FALSE(xoraddr == realaddr) << "encoding pointer is equal to real pointer\n";

    free(d0);
    free(d1);
}

