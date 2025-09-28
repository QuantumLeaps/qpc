QP_SAFE_EDITION = False # this is regular QP Framework
note(f"SafeQP: {QP_SAFE_EDITION}")

def test_safe_qp(title, opt = 0):
    skip(1)
    test(f"SafeQP-only test: {title}", opt)
