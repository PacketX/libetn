/**
 * Copyright 2020 PacketX Technology
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <libetn.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static void signal_cb(int sig);
static bool test_usleep_interrupted(void);

int main(int argc, char *argv[]) {
    int         i;
    bool        good;
    double      elapsed;
    uint64_t    microseconds;
    e_timer_t   *timer;

    /* test timer */
    microseconds = E_USEC_PER_SEC / 2; /* 0.5sec */

    timer = e_timer_new();
    e_assert_true(timer);
    e_assert_true(e_usleep(microseconds, false) == 0);
    e_assert_errno(E_OK, e_timer_elapsed(timer, &elapsed, NULL));
    e_assert_true(0.5 <= elapsed && elapsed < 1);
    e_timer_stop(timer);
    e_assert_true(e_usleep(microseconds, false) == 0);
    e_timer_reset(timer);
    e_assert_errno(E_OK, e_timer_continue(timer));
    e_assert_true(e_usleep(microseconds, false) == 0);
    e_assert_errno(E_OK, e_timer_elapsed(timer, &elapsed, NULL));
    e_timer_free(timer);

    /* try 10 times */
    for(i = 0; i < 10; i++) {
        good = test_usleep_interrupted();
        if(good) {
            break;
        }//end if
    }//end for
    if(i == 10) {
        e_assert_true(false && "test_usleep_interrupted()");
    }//end if

    return 0;
}//end main


/* ===== private function ===== */
static void signal_cb(int sig) {
    /* empty */
    printf("'%s' received\n", strsignal(sig));
}//end signal_cb

static bool test_usleep_interrupted(void) {
    int     status;
    bool    ret;
    pid_t   pid;
    double  seconds;

    /* test e_usleep() */
    ret = false;
    signal(SIGQUIT, signal_cb);
    pid = fork();
    if(pid == 0) {
        printf("child sleep 2 seconds...\n");
        e_assert_true(e_usleep(E_USEC_PER_SEC * 2, false) == 0);
        kill(getppid(), SIGQUIT);
        exit(0);
    }//end if child
    else if(pid > 0) {
        /* sleep 10 seconds, but interrupted after 1 second */
        printf("parent sleep 10 seconds...\n");
        seconds = e_usleep(E_USEC_PER_SEC * 10, true) / (double)E_USEC_PER_SEC;
        printf("parent woke up, remaining %f sec\n", seconds);
        ret = (7.9 < seconds && seconds < 8.1);
    }//end if parent
    else {
        perror("fork");
        abort();
    }//end else

    /* recycle it */
    waitpid(pid, &status, 0);
    if(ret && WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        return true;
    }//end if
    return false;
}//end test_usleep_interrupted
