#include "../common.h"
#include "../API.h"

typedef NTSTATUS(WINAPI *pNtDelayExecution)(IN BOOLEAN, IN PLARGE_INTEGER);
// Recommended delay acquired from Al-Khaser. Should be 600 for Seconds but makes testing difficult, so is currently reduced to allow easier testing for others.
unsigned int delayInSeconds = 1U;
unsigned int delayInMillis = delayInSeconds * 1000U;

//Trying a git pull request

/// @brief Uses NtDelayExecution library to timing attack
/// @return True if it successfully passed the delay, false if it cannot NtDelay at all
bool ntDelay() {

    API::Init();
    PLARGE_INTEGER DelayInterval;

    long long Timeout = delayInMillis * (-10000LL);
    DelayInterval->QuadPart = Timeout;

    if (!API::IsAvailable(API_IDENTIFIER::API_NtDelayExecution)) {

        return false;

    }

    auto NtDelayExecution = (pNtDelayExecution)(API::GetAPI(API_IDENTIFIER::API_NtDelayExecution));
    NtDelayExecution(FALSE, DelayInterval);

    return true;

}

/// @brief Uses the integrated Sleep function to timing attack via looping through multiple separate sleeps
/// @return Returns true as long as the delay is passed through
bool sleepLoop() {

    int separateDelay = delayInMillis/1000;

    // Should be iterating up to 1000 (i < 1000) but is shorter to make further testing easier.
    for (int i = 0; i < 50; i++) {

        Sleep(separateDelay);

    }

    return true;

}

/// @brief Uses the rdtsc function to check how long certain functions (GetProcessHeap and CloseHandle) take to complete. A virtual machine and a bare metal machine
/// act differently in this case
/// @return Returns true if it takes sufficiently longer to do CloseHandle compared to GetProcessHeap, otherwise returns false
bool rdtscDiff() {

    long x;
    long y;
    long z;
    
    // After testing, the check actually messes up commonly. By checking 5 times, you are accounting for any mistaken checks, and repeats also affect the difference
    // between the closehandle and getprocessheap checks on bare metal.
    for (int i = 0; i < 5; i++) {

        x = __rdtsc();

        GetProcessHeap();

        y = __rdtsc();

        CloseHandle(0);

        z = __rdtsc();

        if((z-y)/(y-x) >= 10) {

            return true;

        }

    }
    
    return false;

}

/// @brief Uses the rdtsc function to check how long a certain function (__cpuid) takes to complete. A virtual machine and a bare metal machine are different in 
/// this case
/// @return Returns true if it takes the correct amount expected off a bare metal machine, otherwise returns false, expecting a VM.
bool rdtscCPUID() {

    long x;
    long y;
    long sum;

    int cpuInfo[4] = {};

    x = __rdtsc();

    __cpuid(cpuInfo, 0);

    y = __rdtsc();

    sum = y-x;

    if (sum < 1000 && sum > 0) {

        return true;

    } else {

        return false;

    }

}

/// @brief Uses an icmp echo call and reply to timing attack
/// @return Returns true if delay succeeds. Returns false if it cannot create an Icmp file
bool icmpEcho() {

    HANDLE icmp;
    char data[32] = "Data Buffer";
    DWORD size = sizeof(ICMP_ECHO_REPLY) + sizeof(data) + 8;
    LPVOID buffer = (void *)malloc(size);
    const char ipaddr[] = "224.0.0.0";

    icmp = IcmpCreateFile();
    if (icmp == INVALID_HANDLE_VALUE) {

        printf("IcmpCreateFile failed.\n");
        return false;

    }

    IcmpSendEcho(icmp, INADDR_NONE, data, sizeof(data), NULL, buffer, 0, delayInMillis);
    IcmpCloseHandle(icmp);
    free(buffer);

    return true;

}

/// @brief Uses the CreateWaitableTimer function to create a delay using a timer. Waits as a timing attack
/// @return Returns true if delay is successful. If timer is not created, it cannot set a timer, or it cannot wait, returns false
bool waitableTimer() {

    HANDLE timer = CreateWaitableTimer(NULL, true, NULL);
    LARGE_INTEGER time;
    time.QuadPart = delayInMillis * -10000LL;

    if (timer == NULL) {

        return false;

    }

    if (SetWaitableTimer(timer, &time, 0, NULL, NULL, false) == false) {

        return false;

    } else {

        if (WaitForSingleObject(timer, INFINITE) != WAIT_OBJECT_0) {

            return false;

        }

    }

    CancelWaitableTimer(timer);
    CloseHandle(timer);
    return true;

}

HANDLE event;

/// @brief Callback function for TimerQueueTimer
/// @return Sets an event
VOID CALLBACK Callback(PVOID x, BOOLEAN timer) {

    if (timer && x == (PVOID)NULL) {

        SetEvent(event);

    }

}

/// @brief Uses the CreateTimerQueueTimer function to timing attack
/// @return Returns true if delay is successful. Returns false if unable to create event, unable to create timer queue, unable to create timer, or unable to wait
bool queueTimer() {

    HANDLE timerQueue;
    HANDLE timerQueueTimer;
    bool test;

    event = CreateEvent(NULL, false, false, NULL);
    if (event == NULL) {

        return true;

    }

    timerQueue = CreateTimerQueue();
    if (timerQueue == NULL) {

        return false;

    }

    test = CreateTimerQueueTimer(&timerQueueTimer, timerQueue, &Callback, (PVOID)NULL, delayInMillis, 0, WT_EXECUTEDEFAULT);
    if (!test) {

        return false;

    } else {

        if (WaitForSingleObject(event, delayInMillis * 10) != WAIT_OBJECT_0) {

            return false;

        }

    }

    DeleteTimerQueueEx(timerQueue, NULL);
    CloseHandle(event);

    return true;

}
