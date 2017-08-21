uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd;
    QSchedStatus lockStat;

    lockStat = QXK_schedLock(N_PHILO); /* lock scheduler around shared seed */

    /* "Super-Duper" Linear Congruential Generator (LCG) */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; /* set for the next time */

    QXK_schedLock(lockStat); /* unlock the scheduler */

    return rnd;
}
