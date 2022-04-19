void *block = QMpool_get(&myMemPool2, 5U); /* non-asserting version */
if (block != (void *)0) { /* allocation succeeded? */
   ~ ~ ~
}

~ ~ ~
QMPool_put(&myMemPool2, block);