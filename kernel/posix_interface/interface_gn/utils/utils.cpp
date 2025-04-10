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

#include "utils.h"
#include <gtest/gtest.h>
#include "log.h"


// init rand seed at startup
__attribute__((constructor)) static void Initialize(void)
{
    LOGD("srand(time(0)) is called.");
    srand(time(nullptr));
}

int CheckValueClose(double target, double actual, double accuracy)
{
    double diff = actual - target;
    double pct;
    if (diff < 0) {
        diff = -diff;
    }
    if (actual == 0) {
        return 0;
    } else {
        pct = diff / actual;
    }
    LOGD("diff=%f, pct=%f\n", diff, pct);
    return (pct <= accuracy);
}

const int SYS_US_PER_MS = 1000;
void Msleep(int msec)
{
    usleep(msec * SYS_US_PER_MS);
}

int KeepRun(int msec)
{
    struct timespec time1 = { 0, 0 };
    struct timespec time2 = { 0, 0 };
    clock_gettime(CLOCK_MONOTONIC, &time1);
    LOGD("KeepRun start : tv_sec=%ld, tv_nsec=%ld\n", time1.tv_sec, time1.tv_nsec);
    int loop = 0;
    int runned = 0;
    int sectomsec = 1000;
    int nsectomsec = 1000000;
    while (runned < msec) {
        ++loop;
        clock_gettime(CLOCK_MONOTONIC, &time2);
        runned = (time2.tv_sec - time1.tv_sec) * sectomsec + (time2.tv_nsec - time1.tv_nsec) / nsectomsec;
    }
    LOGD("KeepRun end : tv_sec=%ld, tv_nsec=%ld\n", time2.tv_sec, time2.tv_nsec);
    return loop;
}

// check process state use 'waitpid'
int CheckProcStatus(pid_t pid, int *code, int flag)
{
    int status;
    int rt = waitpid(pid, &status, flag);
    errno = 0;
    int errorcode = -2;
    if (rt == -1) {
        LOGE("waitpid return -1, errno=%d:%s\n", errno, strerror(errno));
        return -1;
    } else if (rt == 0) {
        return 0;
    } else if (rt != pid) { // waitpid return error
        if (errno) {
            LOGE("waitpid return error, rt=%d, errno=%d:%s\n", rt, errno, strerror(errno));
        } else {
            LOGE("waitpid return error, errno is not set(no more info)\n");
        }
        return errorcode;
    }
    if (code == nullptr) {
        return -1;
    }
    int num2 = 2;
    int num3 = 3;
    int num4 = 4;
    if (WIFEXITED(status)) {
        *code = WEXITSTATUS(status);
        return 1;
    } else if (WIFSIGNALED(status)) {
        *code = WTERMSIG(status);
        return num2;
    } else if (WIFSTOPPED(status)) {
        *code = WSTOPSIG(status);
        return num3;
    }
    return num4;
}
void AssertProcAlive(pid_t pid)
{
    int exitCode;
    int procStat = CheckProcStatus(pid, &exitCode);
    ASSERT_EQ(procStat, 0) << "target process should still alive.\n";
}

void ExpectProcAlive(pid_t pid)
{
    int exitCode;
    int procStat = CheckProcStatus(pid, &exitCode);
    EXPECT_EQ(procStat, 0) << "target process should still alive.\n";
}

void AssertProcExitedOK(pid_t pid)
{
    int exitCode;
    int processExistd = 1;
    int procStat = CheckProcStatus(pid, &exitCode);
    EXPECT_EQ(procStat, processExistd);
    ASSERT_EQ(exitCode, 0) << "target process should exited 0.\n";
}

void ExpectProcExitedOK(pid_t pid)
{
    int exitCode;
    int processExistd = 1;
    int procStat = CheckProcStatus(pid, &exitCode);
    EXPECT_EQ(procStat, processExistd);
    EXPECT_EQ(exitCode, 0) << "target process should exited 0.\n";
}

void WaitProcExitedOK(pid_t pid)
{
    int exitCode;
    int processExistd = 1;
    int procStat = CheckProcStatus(pid, &exitCode, 0);
    EXPECT_EQ(procStat, processExistd);
    EXPECT_EQ(exitCode, 0) << "target process should exited 0.\n";
}

void ExpectProcKilled(pid_t pid, int signum)
{
    int exitCode;
    int killedBySignal = 2;
    int procStat = CheckProcStatus(pid, &exitCode);
    EXPECT_EQ(procStat, killedBySignal);
    EXPECT_EQ(exitCode, signum) << "target process should killed by " << signum;
}

void AssertProcKilled(pid_t pid, int signum)
{
    int exitCode;
    int killedBySignal = 2;
    int procStat = CheckProcStatus(pid, &exitCode);
    EXPECT_EQ(procStat, killedBySignal);
    EXPECT_EQ(exitCode, signum) << "target process should killed by " << signum;
}

void WaitProcKilled(pid_t pid, int signum)
{
    int exitCode;
    int killedBySignal = 2;
    int procStat = CheckProcStatus(pid, &exitCode, 0);
    ASSERT_EQ(procStat, killedBySignal) << "target process should killed by " << signum;
    ASSERT_EQ(exitCode, signum) << "target process should killed by " << signum;
}
// start an elf, only check if execve success or not
static int StartElf(const char *fname, char * const argv[], char * const envp[])
{
    int pid = fork();
    if (pid == -1) {
        LOGE("ERROR: Fork Error, errno=%d, err=%s\n", errno, strerror(errno));
        return -1;
    } else if (pid == 0) { // child
        errno = 0;
        int rt = execve(fname, argv, envp);
        if (rt == -1) {
            LOGE("ERROR: execve return -1, errno=%d, err=%s\n", errno, strerror(errno));
            _Exit(EXECVE_RETURN_ERROR);
        }
        LOGE("ERROR: execve should never return on success. rt=%d, errno=%d, err=%s\n", rt, errno, strerror(errno));
        _Exit(EXECVE_RETURN_OK);
    }
    return pid;
}

int RunElf(const char *fname, char * const argv[], char * const envp[], int timeoutSec)
{
    int isTimeout = 0;
    int exitCode;
    sigset_t set;
    sigset_t origMask;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    
    if (sigprocmask(SIG_BLOCK, &set, &origMask) < 0) {
        LOG("sigprocmask");
    }
    int pid = StartElf(fname, argv, envp);
    if (pid == -1) { // fork error
        return -1;
    }

    if (timeoutSec > 0) {
        struct timespec time1 = { timeoutSec, 0 };
        if (sigtimedwait(&set, nullptr, &time1) == -1) {
            if (errno == EAGAIN) {
                isTimeout = 1;
            } else {
                LOGE("ERROR: sigtimedwait FAIL: %s\n", strerror(errno));
                return -1;
            }
            if (kill(pid, SIGTERM) == -1) {
                LOGE("ERROR: kill child FAIL: %s\n", strerror(errno));
                return -1;
            }
        }
        // else: sigtimedwait return ok, child has exited already, nothing else to do
    }
    int errorcode1 = -1;
    int errorcode2 = -2;
    int rt = CheckProcStatus(pid, &exitCode, 0);
    if ((rt <= 0) || (exitCode == EXECVE_RETURN_OK) || (exitCode == EXECVE_RETURN_ERROR)) {
        return errorcode1;
    }
    if (isTimeout) {
        LOGE("ERROR: child execute timed out!\n");
        return errorcode2;
    }
    return exitCode;
}

int StartExecveError(const char *fname, char * const argv[], char * const envp[])
{
    int sleeptime = 30;
    pid_t pid = fork();
    if (pid == -1) {
        LOGE("ERROR: Fork Error, errno=%d, err=%s\n", errno, strerror(errno));
        return -1;
    } else if (pid == 0) { // child
        int rt = execve(fname, argv, envp);
        if (rt == -1) {
            LOG("ERROR: execve return -1, errno=%d, err=%s\n", errno, strerror(errno));
            _Exit(errno);
        }
        LOGE("ERROR: execve should never return on success. rt=%d, errno=%d, err=%s\n", rt, errno, strerror(errno));
        _Exit(EXECVE_RETURN_OK);
    }
    // parent
    Msleep(sleeptime);
    int exitCode;
    int errorcode1 = -2;
    int errorcode2 = -3;
    int procStat = CheckProcStatus(pid, &exitCode);
    LOG("procStat=%d, exitCode=%d\n", procStat, exitCode);
    if (procStat != 1) {
        return errorcode2;
    } else if (exitCode == EXECVE_RETURN_OK) {
        return errorcode1;
    } else {
        return exitCode;
    }
}

// Get a pid number that currently not exist
// by creat a child process and exit.
pid_t GetNonExistPid()
{
    int sleeptime5 = 5;
    int sleeptime20 = 20;
    pid_t pid = fork();
    if (pid < 0) {
        LOG("fork error, wait 5 seconds than try angain...");
        sleep(sleeptime5);
        pid = fork();
        if (pid < 0) {
            LOG("still fork error!");
            return -1;
        }
    }
    if (pid > 0) { // parent
        Msleep(sleeptime20);
        if (waitpid(pid, nullptr, 0) != pid) {
            LOG("waitpid failed, errno = %d", errno);
            return -1;
        }
    } else { // child
        _Exit(0);
    }
    return pid;
}

// return n: 0 < n <= max
uint32_t GetRandom(uint32_t max)
{
    int fd;
    int r = 0;
    if (max == 0 || max == 1) {
        return 1;
    }
    fd = open("/dev/random", O_RDONLY);
    if (fd == -1) {
        LOG("open /dev/random failed, errno = %d", errno);
    }
    if (read(fd, &r, sizeof(int)) == -1) {
        LOG("read failed, errno = %d", errno);
    }
    if (close(fd) == -1) {
        LOG("close failed, errno = %d", errno);
    }
    return (r % max) + 1;
}

// get cur-time plus ms
void GetDelayedTime(struct timespec *ts, unsigned int ms)
{
    const unsigned int nsecPerSec = 1000000000;
    unsigned int setTimeNs = ms * 1000000;
    struct timespec tsNow = { 0 };
    clock_gettime(CLOCK_REALTIME, &tsNow);
    ts->tv_sec = tsNow.tv_sec + (tsNow.tv_nsec + setTimeNs) / nsecPerSec;
    ts->tv_nsec = (tsNow.tv_nsec + setTimeNs) % nsecPerSec;
}

// calculate time difference, in ms
int GetTimeDiff(struct timespec ts1, struct timespec ts2)
{
    const unsigned int nsecPerSec = 1000000000;
    const unsigned int nsectomsec = 1000000;
    int ms = (ts1.tv_sec - ts2.tv_sec) * nsecPerSec + (ts1.tv_nsec - ts2.tv_nsec);
    ms = ms / nsectomsec;
    return ms;
}

int GetCpuCount(void)
{
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    int temp = sched_getaffinity(getpid(), sizeof(cpu_set_t), &cpuset);
    if (temp != 0) {
        printf("%s %d Error : %d\n", __FUNCTION__, __LINE__, temp);
    }

    return CPU_COUNT(&cpuset);
}

int FixCurProcessToOneCpu(int cpuIndex, cpu_set_t *pOldMask)
{
    int ret;
    cpu_set_t setMask;
    CPU_ZERO(pOldMask);
    ret = sched_getaffinity(0, sizeof(cpu_set_t), pOldMask);
    if (ret != 0) {
        LOG("sched_getaffinity failed, ret = %d", ret);
        return -1;
    }
    if (CPU_ISSET(0, pOldMask)) {
        LOG("before affinity cpu is 0");
    }
    if (CPU_ISSET(1, pOldMask)) {
        LOG("before affinity cpu is 1");
    }
    CPU_ZERO(&setMask);
    CPU_SET(cpuIndex, &setMask);
    LOG("fix cpu to %d", cpuIndex);
    ret = sched_setaffinity(0, sizeof(setMask), &setMask);
    if (ret != 0) {
        LOG("sched_setaffinity failed, ret = %d", ret);
        return -1;
    }
    return 0;
}
