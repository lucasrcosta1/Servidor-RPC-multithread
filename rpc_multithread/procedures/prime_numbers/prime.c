#include <stdio.h>
#include <stdlib.h>
#include "prime.h"

int miller_rabin(mpz_t n, unsigned int k) {
    if (mpz_cmp_ui(n, 2) < 0) {
        return 0;
    }
    if (mpz_cmp_ui(n, 2) == 0) {
        return 1;
    }
    mpz_t d, x, limit;
    mpz_inits(d, x, limit, NULL);

    // Write (n - 1) as d * 2^s
    mpz_sub_ui(d, n, 1);
    unsigned int s = 0;
    while (mpz_even_p(d)) {
        mpz_div_2exp(d, d, 1);
        s++;
    }

    // Repeat the test k times
    while (k--) {
        // Choose a random integer a in the range [2, n - 2]
        gmp_randstate_t state;
        gmp_randinit_default(state);
        mpz_urandomm(x, state, d);
        mpz_add_ui(x, x, 2);

        // Compute x^d mod n using the binary exponentiation algorithm
        mpz_powm(x, x, d, n);
        mpz_set(limit, d);
        mpz_mul_2exp(limit, limit, s);
        if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, limit) == 0) {
            continue;
        }
        for (unsigned int r = 1; r < s; r++) {
            mpz_powm_ui(x, x, 2, n);
            if (mpz_cmp_ui(x, 1) == 0) {
                return 0;
            }
            if (mpz_cmp(x, limit) == 0) {
                break;
            }
        }
        if (mpz_cmp(x, limit) != 0) {
            return 0;
        }
    }
    // n is probably prime
    return 1;
}
