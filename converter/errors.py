class ParenthesisNotMatchError(Exception):
    """Parenthesis not match"""
    def __init__(self, idx, msg="Parenthesis not match at index {idx}"):
        super().__init__(msg.format(idx=idx))
