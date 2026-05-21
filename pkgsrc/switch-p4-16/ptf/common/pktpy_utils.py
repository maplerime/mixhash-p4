def pktpy_skip(cls):
    """
    Test Case decorator that marks the test as being disabled for
    specific vars. In this case is designed to skip test which is not ready
    to work with bf_pkpty (replacement for the Scapy).
    """
    import os
    if os.getenv("PKTPY", "true").lower() != "false":
        cls._disabled = True
    return cls


def pktpy_skip_test(func):
    """
    Test Case decorator that marks the test as being skipped.
    In this case is designed to skip test which is not ready to work
    with bf_pkpty (replacement for the Scapy).
    """
    def inner(*args, **kwargs):
        try:
            print("Test {test_name} was "
                  "skipped due to incompatibility "
                  "with bf-pktpy".format(test_name=func.__name__))
        except:
            print(
                "Test was skipped due to incompatibility with bf-pktpy")
    return inner
