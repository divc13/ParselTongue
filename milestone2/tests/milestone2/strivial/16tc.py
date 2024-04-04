class A:
    def __init__(self):
        self.a: int = 1

a: A = A()
b: list[int] = range(a.a)
