Philo Philo_inst[N_PHILO];
. . .
for (uint8_t n = 0U; n < N_PHILO; ++n) {
    QS_OBJ_ARR_DICTIONARY(&Philo_inst[n], n);
    QS_OBJ_ARR_DICTIONARY(&Philo_inst[n].timeEvt, n);
}
