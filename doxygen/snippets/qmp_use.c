void *block1 = QMPool_get(&myMemPool1, 0U); /* asserts on empty pool */
/* block1 is guaranteed to be not NULL */
~ ~ ~
QMPool_put(&myMemPool1, block1);

void *block2 = QMpool_get(&myMemPool2, 5U); /* non-asserting version */
if (block2 != (void *)0) { /* allocation succeeded? */
   ~ ~ ~
}

~ ~ ~
QMPool_put(&myMemPool2, block2);