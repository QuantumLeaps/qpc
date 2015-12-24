QMPool myMemPool1; /* memory pool object #1 (global) */
static uint8_t memPoolSto1[512]; /* storage for a memory pool #1 */

QMPool myMemPool2; /* memory pool object #2 (global) */
static uint8_t memPoolSto2[1024]; /* storage for a memory pool #2 */

QMPool_init(&myMemPool1,
            memPoolSto1,
            sizeof(memPoolSto1),
            10U);  /* memory blocks of 10 bytes each */

QMPool_init(&myMemPool2,
            memPoolSto2,
            sizeof(memPoolSto2),
            25U);  /* memory blocks of 25 bytes each */
