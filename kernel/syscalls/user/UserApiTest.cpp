/*
 * Copyright (C) 2024 HiHope Open Source Organization.
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

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <unistd.h>
#include <gtest/gtest.h>
#include <sys/fsuid.h>
#include <sys/personality.h>
#include <sys/stat.h>
#include <sys/types.h>


using namespace testing::ext;
using namespace std;

static const int MAX_GROUPS = 1024;
const unsigned long RETRIEVE_PERSONALITY = 0xffffffff;
const unsigned long DEFAULT_PERSONALITY = 0;

class UserApiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
private:
};
void UserApiTest::SetUp()
{
}
void UserApiTest::TearDown()
{
}
void UserApiTest::SetUpTestCase()
{
}
void UserApiTest::TearDownTestCase()
{
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_0100
 * @tc.name   : SetpGidSetGroupIDSuccess_0001
 * @tc.desc   : SetpGid Set group ID success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetpGidSetGroupIDSuccess_0001, Function | MediumTest | Level1)
{
    int ret;
    gid_t gid = getgid();
    EXPECT_TRUE(gid >= 0);

    ret = setpgid(gid, gid);
    EXPECT_EQ(ret, 0);

    ret = setpgid(0, gid);
    EXPECT_EQ(ret, 0);

    ret = setpgid(gid, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_0200
 * @tc.name   : SetpGidSetGroupIDFail_0002
 * @tc.desc   : SetpGid Set group ID fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, SetpGidSetGroupIDFail_0002, Function | MediumTest | Level2)
{
    gid_t invalidGid = -1;
    errno = 0;
    int ret = setpgid(0, invalidGid);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_0300
 * @tc.name   : GetgroupsGetGroupIDListSuccess_0003
 * @tc.desc   : Getgroups get list of supplementary group IDs success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, GetgroupsGetGroupIDListSuccess_0003, Function | MediumTest | Level1)
{
    int numGroups;
    gid_t groups[MAX_GROUPS];

    numGroups = getgroups(MAX_GROUPS, groups);
    EXPECT_TRUE(numGroups >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_0400
 * @tc.name   : GetgroupsGetGroupIDInNullFail_0004
 * @tc.desc   : Getgroups get list of supplementary group IDs in nullptr fail, errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, GetgroupsGetGroupIDInNullFail_0004, Function | MediumTest | Level2)
{
    errno = 0;
    int numGroups = getgroups(MAX_GROUPS, nullptr);
    EXPECT_EQ(numGroups, -1);
    EXPECT_EQ(errno, EFAULT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_0500
 * @tc.name   : GetpgidAcquireSpecifyProcessGroupIDSuccess_0005
 * @tc.desc   : Getpgid acquire specify process group ID success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, GetpgidAcquireSpecifyProcessGroupIDSuccess_0005, Function | MediumTest | Level1)
{
    pid_t pid = getpid();
    pid_t pgid = getpgid(pid);
    EXPECT_TRUE(pgid >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_0600
 * @tc.name   : GetpgidAcquireInvalidProcessGroupIDFail_0006
 * @tc.desc   : Getpgid acquire group ID of invalid process ID fail, errno ESRCH.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, GetpgidAcquireInvalidProcessGroupIDFail_0006, Function | MediumTest | Level2)
{
    int invalidPID = -1;
    errno = 0;
    pid_t pgid = getpgid(invalidPID);
    EXPECT_EQ(pgid, -1);
    EXPECT_EQ(errno, ESRCH);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_0700
 * @tc.name   : GetresuidGetReadEffectiveSavedUIDSuccess_0007
 * @tc.desc   : Getresuid get real uid effective uid and saved uid success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, GetresuidGetReadEffectiveSavedUIDSuccess_0007, Function | MediumTest | Level1)
{
    int ret;
    uid_t getRUid, getEUid, savedUid;
    uid_t realUid = getuid();
    uid_t effectiveUid = geteuid();
    ret = getresuid(&getRUid, &getEUid, &savedUid);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(realUid, getRUid);
    EXPECT_EQ(effectiveUid, getEUid);
    EXPECT_TRUE(savedUid >= 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_0800
 * @tc.name   : GetresuidGetReadEffectiveSavedUIDInNullptrFail_0008
 * @tc.desc   : Getresuid get real uid effective uid and saved uid in nullptr fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, GetresuidGetReadEffectiveSavedUIDInNullptrFail_0008, Function | MediumTest | Level2)
{
    int ret;
    uid_t realUid, effectiveUid, savedUid;

    errno = 0;
    ret = getresuid(nullptr, &effectiveUid, &savedUid);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    errno = 0;
    ret = getresuid(&realUid, nullptr, &savedUid);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
    errno = 0;
    ret = getresuid(&realUid, &effectiveUid, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}


/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_0900
 * @tc.name   : GetuidAcquireUIDSuccess_0009
 * @tc.desc   : Getuid acquire user ID success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, GetuidAcquireUIDSuccess_0009, Function | MediumTest | Level1)
{
    int ret;
    uid_t uid = getuid();
    EXPECT_TRUE(uid >= 0);

    uid_t setUid = uid + 1;
    ret = setuid(setUid);
    EXPECT_EQ(ret, 0);

    uid_t newUid = getuid();
    EXPECT_EQ(newUid, setUid);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1000
 * @tc.name   : PersonalitySetExecutionDomainSuccess_0010
 * @tc.desc   : Personality set the process execution domain success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, PersonalitySetExecutionDomainSuccess_0010, Function | MediumTest | Level1)
{
    int ret = personality(PER_LINUX32);
    EXPECT_EQ(ret, 0);
    unsigned long currentPersonality = static_cast<unsigned long>(personality(RETRIEVE_PERSONALITY));
    EXPECT_EQ(currentPersonality, PER_LINUX32);

    ret = personality(DEFAULT_PERSONALITY);
    EXPECT_GE(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1100
 * @tc.name   : SetfsgidCurrentSuccess_0011
 * @tc.desc   : Setfsgid set current gid.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetfsgidCurrentSuccess_0011, Function | MediumTest | Level1)
{
    int ret;
    ret = setfsgid(0);
    EXPECT_GE(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1200
 * @tc.name   : SetfsgidInvalidFsgidFailed_0012
 * @tc.desc   : Setfsgid set user ID success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, SetfsgidInvalidFsgidFailed_0012, Function | MediumTest | Level2)
{
    int ret;
    ret = setfsgid(-1);
    EXPECT_GE(ret, -1);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1300
 * @tc.name   : SetfsuidSetFSUserIDSuccess_0013
 * @tc.desc   : Setfsuid set user ID used for file system checks success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetfsuidSetFSUserIDSuccess_0013, Function | MediumTest | Level1)
{
    int ret;
    uid_t effectiveUid = geteuid();
    EXPECT_TRUE(effectiveUid >= 0);

    uid_t fsUid = effectiveUid + 1;
    ret = setfsuid(fsUid);
    EXPECT_GE(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1400
 * @tc.name   : SetgidSetGroupIDSuccess_0014
 * @tc.desc   : Setgid Set group ID success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetgidSetGroupIDSuccess_0014, Function | MediumTest | Level1)
{
    int ret;
    gid_t gid = getgid();
    EXPECT_TRUE(gid >= 0);

    gid_t setGid = gid + 1;
    ret = setgid(setGid);
    EXPECT_EQ(ret, 0);

    gid_t newGid = getgid();
    EXPECT_EQ(newGid, setGid);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1500
 * @tc.name   : SetgidSetGroupIDFail_0015
 * @tc.desc   : Setgid Set group ID fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, SetgidSetGroupIDFail_0015, Function | MediumTest | Level2)
{
    gid_t invalidGid = -1;
    errno = 0;
    int ret = setgid(invalidGid);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1600
 * @tc.name   : SetgroupsSetGroupIDListSuccess_0016
 * @tc.desc   : Setgroups set list of supplementary group IDs success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetgroupsSetGroupIDListSuccess_0016, Function | MediumTest | Level1)
{
    int numGroups;
    int ret;
    const int groupLength = 3;
    gid_t groups[groupLength] = { 1001, 1002, 1003 };
    gid_t groupsCheck[MAX_GROUPS];

    ret = setgroups(groupLength, groups);
    EXPECT_EQ(ret, 0);
    numGroups = getgroups(MAX_GROUPS, groupsCheck);
    EXPECT_EQ(numGroups, groupLength);
    for (int i = 0; i < numGroups; i++) {
        EXPECT_EQ(groups[i], groupsCheck[i]);
    }
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1700
 * @tc.name   : SetgroupsSetGroupIDNullptrFail_0017
 * @tc.desc   : Setgroups set list of supplementary group IDs in nullptr fail, errno EFAULT.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, SetgroupsSetGroupIDNullptrFail_0017, Function | MediumTest | Level2)
{
    errno = 0;
    int ret = setgroups(MAX_GROUPS, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);

    errno = 0;
    ret = getgroups(MAX_GROUPS, nullptr);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EFAULT);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1800
 * @tc.name   : SetregidSetRealEffectiveGIDSuccess_0018
 * @tc.desc   : Setregid Set real and effective group ID success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetregidSetRealEffectiveGIDSuccess_0018, Function | MediumTest | Level1)
{
    int ret;
    gid_t realGid = getgid();
    EXPECT_TRUE(realGid >= 0);
    gid_t effectiveGid = getegid();
    EXPECT_TRUE(effectiveGid >= 0);

    gid_t setRealGid = realGid + 1;
    gid_t setEffectiveGid = effectiveGid + 1;
    ret = setregid(setRealGid, setEffectiveGid);
    EXPECT_EQ(ret, 0);

    gid_t newRealGid = getgid();
    EXPECT_EQ(newRealGid, setRealGid);
    gid_t newEffectiveGid = getegid();
    EXPECT_EQ(newEffectiveGid, setEffectiveGid);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_1900
 * @tc.name   : SetresgidSetRealEffectiveSavedGIDSuccess_0019
 * @tc.desc   : Setresgid Set real effective and saved group ID success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetresgidSetRealEffectiveSavedGIDSuccess_0019, Function | MediumTest | Level1)
{
    int ret;
    pid_t pid = getpid();
    gid_t realGid = getgid();
    EXPECT_TRUE(realGid >= 0);
    gid_t effectiveGid = getegid();
    EXPECT_TRUE(effectiveGid >= 0);
    gid_t savedGid = getsid(pid);
    EXPECT_TRUE(savedGid >= 0);

    gid_t setRealGid = realGid + 1;
    gid_t setEffectiveGid = effectiveGid + 1;
    gid_t notModifySavedGid = -1;
    ret = setresgid(setRealGid, setEffectiveGid, notModifySavedGid);
    EXPECT_EQ(ret, 0);

    gid_t newRealGid = getgid();
    EXPECT_EQ(newRealGid, setRealGid);
    gid_t newEffectiveGid = getegid();
    EXPECT_EQ(newEffectiveGid, setEffectiveGid);
    gid_t newSavedGid = getsid(pid);
    EXPECT_EQ(newSavedGid, savedGid);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_2000
 * @tc.name   : SetresuidSuccess_0020
 * @tc.desc   : Setresuid set current uid.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetresuidSuccess_0020, Function | MediumTest | Level1)
{
    int ret;
    ret = setresuid(0, 0, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_2100
 * @tc.name   : SetreuidSuccess_0021
 * @tc.desc   : Setresuid Set current uid.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetreuidSuccess_0021, Function | MediumTest | Level1)
{
    int ret;
    ret = setreuid(0, 0);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_2200
 * @tc.name   : SetuidSetUserIDSuccess_0022
 * @tc.desc   : Setuid set user ID success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 1
 */
HWTEST_F(UserApiTest, SetuidSetUserIDSuccess_0022, Function | MediumTest | Level1)
{
    int ret;
    uid_t uid = getuid();
    EXPECT_TRUE(uid >= 0);

    uid_t setUid = uid + 1;
    ret = setuid(setUid);
    EXPECT_EQ(ret, 0);

    uid_t newUid = getuid();
    EXPECT_EQ(newUid, setUid);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_2300
 * @tc.name   : SetgidSetInvalidUIDFail_0023
 * @tc.desc   : Setgid set invalid user ID fail.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, SetgidSetInvalidUIDFail_0023, Function | MediumTest | Level2)
{
    uid_t invalidUid = -1;
    errno = 0;
    int ret = setuid(invalidUid);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(errno, EINVAL);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_2400
 * @tc.name   : SetsidSucc_0024
 * @tc.desc   : Setsid set sid success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, SetsidSucc_0024, Function | MediumTest | Level2)
{
    pid_t pid;
    int ret;
    pid = fork();

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    ret = setsid();
    EXPECT_TRUE(ret > 0);
}

/*
 * @tc.number : SUB_KERNEL_SYSCALL_USER_2500
 * @tc.name   : Getresgid_0001
 * @tc.desc   : Getresgid success.
 * @tc.size   : MediumTest
 * @tc.type   : Function
 * @tc.level  : Level 2
 */
HWTEST_F(UserApiTest, Getresgid_0001, Function | MediumTest | Level2)
{
    gid_t rgid, egid, sgid;
    int ret = getresgid(&rgid, &egid, &sgid);
    EXPECT_EQ(ret, 0);
}