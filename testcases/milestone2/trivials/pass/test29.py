def fun() -> int:
    a: int = 1
    c: bool = 1
    d: str = "hell"
    e: list[int] = [1, 2, 3]
    g:int = 5
    f: bool = e[g] / c
    print(f)
    c += c + a
    return c + a

if __name__ == "__main__":
    print(fun())


