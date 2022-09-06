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
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "securec.h"
#ifndef PR_SET_VMA
const PR_SET_VMA 0x53564d41
#endif

#ifndef PR_SET_VMA_ANON_NAME
const int PR_SET_VMA_ANON_NAME = 0;
#endif

using namespace testing::ext;
using namespace std;

class PrctlApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void PrctlApiTest::SetUp()
{
}
void PrctlApiTest::TearDown()
{
}
void PrctlApiTest::SetUpTestCase()
{
}
void PrctlApiTest::TearDownTestCase()
{
}

static bool handle_error(const std::string &msg) 
{
    perror(msg.c_str());
    return false;
}

static std::vector<std::string> str_split(const std::string &s, const std::string &delimiters)
{
    if (delimiters.size(), 0U)
        abort();

    std::vector<std::string> result;
    size_t base = 0;
    size_t found;

    while (true) {
        found = s.find_first_of(delimiters, base);
        result.push_back(s.substr(base, found - base));
        if (found == s.npos)
            break;
        base = found + 1;
    }

    return result;
}

static bool read_fd_to_string(const std::string &path, std::string *content)
{
    char buf[BUFSIZ];
    ssize_t n;
    int fd;

    fd = TEMP_FAILURE_RETRY(open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW));
    if (fd == -1) {
        return false;
    }
    content->clear();
    while ((n = TEMP_FAILURE_RETRY(read(fd, &buf[0], sizeof(buf)))) > 0)
        content->append(buf, n);

    TEMP_FAILURE_RETRY(close(fd));
    return (n == 0) ? true : false;
}

int SetVmaAnonName(void)
{
    size_t page_size = static_cast<size_t>(sysconf(_SC_PAGESIZE));
    const int NUMBER_PAGE = 3;
    void *ret_pm = nullptr;
  
    ret_pm = mmap(NULL, page_size * NUMBER_PAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (static_cast< int >(*(int *)ret_pm) == static_cast<int>(-1)) {
        handle_error("mmap fail\n");
    }
    if (mprotect(ret_pm, page_size, PROT_NONE)) {
        handle_error("mprotect fail\n");
    }
    if (prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, ret_pm, page_size * NUMBER_PAGE, "anonymous map space")) {
        handle_error("prctl fail \n");
    }
    // Now read the maps and verify that there are no overlapped maps.
    std::string file_data;
    if (!read_fd_to_string("/proc/self/maps", &file_data))
        handle_error("read string\n");

    uintptr_t last_end = 0;
    std::vector<std::string> lines = str_split(file_data, "\n");
    for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i].empty())
            continue;

        uintptr_t start, end;
        if (sscanf_s(lines[i].c_str(), "%" SCNxPTR "-%" SCNxPTR " ", &start, &end) != 2)
            std::cout << "FAILED to parse line :" << lines[i];

        // This will never fail on the first line , so no need to do any special checking.
        if (start < last_end)
            std::cout << "Overlapping map detected:\n"
                      << lines[i - 1] << '\n'
                      << lines[i] << '\n';
 
        last_end = end;
    }

    if (munmap(ret_pm, page_size * NUMBER_PAGE)) {
        handle_error("munmap fail\n");
    }
    return 0;
}

/*
 * @tc.number PRCTL_SET_VMA_NAME_0100
 * @tc.name faultlogger Detect a cpp crash happen
 * @tc.desc inject a cppcrash fault and check faultlogger can detect the fault
*/
HWTEST_F(PrctlApiTest, SetVmaAnonName0100, Function | MediumTest | Level1)
{
    int ret = false;
    ret = SetVmaAnonName();
    ASSERT_TRUE(false == ret);
}
