/**
 * Test Case 11: Signal Handling
 * Tests: signal(), sigaction(), SIGINT, SIGTERM, SIGALRM, custom handlers
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

volatile sig_atomic_t signal_count = 0;
volatile sig_atomic_t alarm_triggered = 0;

// Simple signal handler
void signal_handler(int sig) {
    signal_count++;
    printf("\n[HANDLER] Caught signal %d (count: %d)\n", sig, signal_count);
}

// SIGINT handler
void sigint_handler(int sig) {
    printf("\n[SIGINT] Interrupt signal received (Ctrl+C)\n");
    printf("Press Ctrl+C again within 3 seconds to exit\n");
    
    // Reset handler for next signal
    signal(SIGINT, sigint_handler);
}

// SIGALRM handler
void alarm_handler(int sig) {
    alarm_triggered = 1;
    printf("[ALARM] Alarm signal received!\n");
}

// SIGUSR1 custom handler
void usr1_handler(int sig) {
    printf("[USR1] Custom user signal 1 received\n");
}

// SIGUSR2 custom handler
void usr2_handler(int sig) {
    printf("[USR2] Custom user signal 2 received\n");
}

// Advanced signal handler with sigaction
void advanced_handler(int sig, siginfo_t *info, void *context) {
    printf("[ADVANCED] Signal %d from PID %d\n", sig, info->si_pid);
}

// Demonstration of different signal handling methods
void demo_basic_signals() {
    printf("\n1. Basic Signal Handling:\n");
    
    // Register SIGUSR1 handler
    if (signal(SIGUSR1, usr1_handler) == SIG_ERR) {
        perror("signal");
        return;
    }
    
    // Send signal to self
    printf("Sending SIGUSR1 to self...\n");
    raise(SIGUSR1);
    
    // Register SIGUSR2 handler
    signal(SIGUSR2, usr2_handler);
    printf("Sending SIGUSR2 to self...\n");
    raise(SIGUSR2);
}

void demo_alarm() {
    printf("\n2. Alarm Signal (SIGALRM):\n");
    
    signal(SIGALRM, alarm_handler);
    
    printf("Setting alarm for 2 seconds...\n");
    alarm(2);
    
    // Wait for alarm
    while (!alarm_triggered) {
        printf("Waiting...\n");
        sleep(1);
    }
    
    printf("Alarm completed\n");
}

void demo_sigaction() {
    printf("\n3. Using sigaction (advanced):\n");
    
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = advanced_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        return;
    }
    
    printf("Sending SIGUSR1 with sigaction...\n");
    raise(SIGUSR1);
}

void demo_signal_blocking() {
    printf("\n4. Signal Blocking:\n");
    
    sigset_t set, oldset;
    
    // Block SIGUSR1
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    
    printf("Blocking SIGUSR1...\n");
    sigprocmask(SIG_BLOCK, &set, &oldset);
    
    printf("Sending SIGUSR1 (should be blocked)...\n");
    raise(SIGUSR1);
    printf("Signal sent (but blocked)\n");
    
    printf("Unblocking SIGUSR1...\n");
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    printf("Signal should now be delivered\n");
    
    sleep(1);
}

void demo_signal_waiting() {
    printf("\n5. Signal Waiting (pause and sigsuspend):\n");
    
    signal(SIGUSR1, usr1_handler);
    
    printf("Setting alarm for 2 seconds...\n");
    alarm(2);
    
    printf("Calling pause() - waiting for signal...\n");
    pause();  // Will be interrupted by alarm
    
    printf("Returned from pause()\n");
}

void demo_multiple_signals() {
    printf("\n6. Multiple Signals:\n");
    
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    
    printf("Sending multiple signals...\n");
    for (int i = 0; i < 3; i++) {
        raise(SIGUSR1);
        raise(SIGUSR2);
    }
    
    printf("Total signals caught: %d\n", signal_count);
}

void demo_signal_ignore() {
    printf("\n7. Ignoring Signals:\n");
    
    printf("Setting SIGUSR1 to SIG_IGN...\n");
    signal(SIGUSR1, SIG_IGN);
    
    printf("Sending SIGUSR1 (should be ignored)...\n");
    raise(SIGUSR1);
    printf("Signal ignored successfully\n");
    
    // Restore handler
    signal(SIGUSR1, usr1_handler);
}

void demo_signal_default() {
    printf("\n8. Restoring Default Handler:\n");
    
    signal(SIGUSR1, usr1_handler);
    printf("Custom handler set, sending signal...\n");
    raise(SIGUSR1);
    
    printf("Restoring default handler...\n");
    signal(SIGUSR1, SIG_DFL);
    printf("Default handler restored (would terminate if sent now)\n");
}

int main() {
    printf("=== Signal Handling Test ===\n");
    printf("PID: %d\n", getpid());
    
    // Run demonstrations
    demo_basic_signals();
    demo_alarm();
    demo_sigaction();
    demo_signal_blocking();
    demo_signal_waiting();
    demo_multiple_signals();
    demo_signal_ignore();
    demo_signal_default();
    
    printf("\n=== Signal Handling Test Complete ===\n");
    printf("Note: Some signals (like SIGINT from Ctrl+C) require user interaction\n");
    
    return 0;
}
