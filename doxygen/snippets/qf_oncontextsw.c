#ifdef QF_ON_CONTEXT_SW

/* NOTE: the context-switch callback is called with interrupts DISABLED */
void QF_onContextSw(QActive *prev, QActive *next) {
    (void)prev;
    if (next != (QActive *)0) { /* next is not the idle thread? */
        _impure_ptr = next->thread; /* switch to next TLS */
    }

    /* If you use QS software tracing, use the _NOCRIT() begin/end */
    QS_BEGIN_NOCRIT(CONTEXT_SW, 0U)
        QS_OBJ(prev);
        QS_OBJ(next);
    QS_END_NOCRIT()
}

#endif /* QF_ON_CONTEXT_SW */
