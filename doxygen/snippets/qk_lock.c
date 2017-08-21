uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    uint32_t rnd;
    QSchedStatus lockStat; /* <=== QK scheduler lock status */

    lockStat = QK_schedLock(N_PHILO); /* <=== lock scheduler up to N_PHILO prio */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd; /* set for the next time */
    QK_schedUnlock(lockStat); /* <=== unlock the scheduler */

    return (rnd >> 8);
}
