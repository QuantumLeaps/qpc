#ifdef QXK_ON_CONTEXT_SW
/* NOTE: the context-switch callback is called with interrupts DISABLED */
void QXK_onContextSw(QActive *prev, QActive *next) {
    (void)prev;
    if (next != (QActive *)0) { /* next is not the QXK idle thread? */
        _impure_ptr = next->thread; /* switch to next TLS */
    }

    /* If you use QS software tracing, use the _NOCRIT() begin/end */
    QS_BEGIN_NOCRIT(ON_CONTEXT_SW, 0U)
        QS_OBJ(prev);
        QS_OBJ(next);
    QS_END_NOCRIT()
}
#endif /* QXK_ON_CONTEXT_SW */
