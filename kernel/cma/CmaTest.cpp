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
#include <fcntl.h>
#include <cinttypes>
#include <climits>
#include <cstdio>
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/utsname.h>
#include "securec.h"

#ifndef BUFFER_LENGTH
const  int BUFFER_LENGTH =  4096;
#endif

const  int return_value = -2;

using namespace testing::ext;
using namespace std;

class CmaTest : public testing::Test {
public:

    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};

static bool handle_error(const std::string &msg) 
{
    perror(msg.c_str());
    return false;
}

void CmaTest::SetUp()
{
}
void CmaTest::TearDown()
{
}
void CmaTest::SetUpTestCase()
{
}
void CmaTest::TearDownTestCase()
{
}

int get_camfree_value(void)
{
    int camfree = -1;
    char tmp[20] = {0};
    FILE *fp = nullptr;
    char filename[64] = {0};
    char line[128] = {0};
    char path[128] = "/proc/meminfo";

    if (sprintf_s(filename, sizeof(filename), "%s", path) < 0) {
        printf("sprintf error");
    }
    fp = fopen(filename, "r");
    if (fp == nullptr) {
        handle_error("fopen fail\n");
        return return_value;
    }

    while (!feof(fp)) {
        if (fgets(line, sizeof(line), fp) == nullptr) {
            handle_error("fgets fail or EOF\n");
            break;
        }

        if (strstr(line, "CmaFree:")) {
            if (sscanf_s(line, "%s %d", tmp, sizeof(tmp), &camfree) > 0) {
                printf(">>>%s %d\r\n", tmp, camfree);
                break;
            }
        } else {
            continue;
        }
    }

    if (fclose(fp) != 0) {
        printf("io close error");
    }
    return camfree;
}

const int  MALLOC_MAX =  100;

int getread_value(void)
{
    int fd;
    int rand_value = 0;
    fd = open("/dev/random", O_RDONLY);
    if (fd > 0) {
        read(fd, &rand_value, sizeof(int));
    }
    close(fd);
    return rand_value;
}

int CamfreeTest(void)
{
    int camfree_beforemalloc, camfree_aftermalloc, ret;
    char *buffer_array[MALLOC_MAX];
    const int number_letters = 26;

    camfree_beforemalloc = get_camfree_value();
    if (camfree_beforemalloc == return_value) {
        ret = return_value;
    }

    if (camfree_beforemalloc == -1 || camfree_beforemalloc == 0) {
        ret = 1;
    }

    if (camfree_beforemalloc > 0) {
        for (int i = 0; i <MALLOC_MAX; i++) {
            buffer_array[i] = (char *)malloc(BUFFER_LENGTH); 
            if (buffer_array[i] == nullptr) {
                handle_error("malloc fail\n");
            }

            for (int j = 0; j < BUFFER_LENGTH; j++) {
                int read_value = getread_value();
                buffer_array[i][j] = read_value % number_letters + 'a';
            }
        }

        camfree_aftermalloc = get_camfree_value();
        if (camfree_aftermalloc < camfree_beforemalloc) {
            ret = 1;
        } else {
            ret = 0;
        }

        for (int i = 0; i < MALLOC_MAX; i++) {
            free(buffer_array[i]);
        }
        camfree_aftermalloc = get_camfree_value();
    }
    return ret;
}

/*
 * @tc.number CAM_FREE_FUN_0100
 * @tc.name Apply part of the memory camfree reduction
 * @tc.desc Test the basic features of CamFree
*/
HWTEST_F(CmaTest, CamfreeTest0100, Function|MediumTest|Level1)
{
    int ret = false;
    ret = CamfreeTest();
    ASSERT_TRUE(ret);
}
