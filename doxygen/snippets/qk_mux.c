QMutex l_rndMutex;  /* mutex to protect the random number generator */


void BSP_randomSeed(uint32_t seed) {
    QMutex_init(&l_rndMutex, N_PHILO); /* ceiling == max Philo priority */
    l_rnd = seed;
}

uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd;

    QMutex_lock(&l_rndMutex); /* <== lock the shared random seed */

    /* "Super-Duper" Linear Congruential Generator (LCG) */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; /* set for the next time */

    QMutex_unlock(&l_rndMutex); /* <== unlock the shared random seed */

    return rnd;
}
