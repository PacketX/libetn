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

int main(int argc, char *argv[]) {
    e_atomic_t i;

    e_atomic_set(&i, 64);
    e_assert_true(i == 64);
    e_assert_true(e_atomic_get(&i) == 64);
    (void)e_atomic_cmp_set(&i, i, 128);
    e_assert_true(i == 128);

    e_atomic_add(&i, 128);
    e_assert_true(i == 256);

    e_atomic_sub(&i, 512);
    e_assert_true(i == -256);

    e_atomic_add(&i, 256);
    e_assert_true(i == 0);

    e_atomic_inc(&i);
    e_atomic_inc(&i);
    e_atomic_inc(&i);
    e_assert_true(i == 3);

    e_atomic_dec(&i);
    e_atomic_dec(&i);
    e_assert_true(i == 1);
    e_assert_true(e_atomic_dec_and_test(&i));

    e_atomic_memory_barrier();
    e_atomic_cpu_pause();

    return 0;
}//end main
