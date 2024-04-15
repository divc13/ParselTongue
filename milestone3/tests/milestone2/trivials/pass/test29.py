def fun() -> int:
    a: int = 1
    c: bool = 1
    d: str = "hell"
    e: list[int] = [1, 2, 3]
    f: bool = e[5] / c
    print(f)
    c += c + a
    return c + a

if __name__ == "__main__":
    print(fun())


