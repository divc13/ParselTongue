def fun() -> int:
    a:int = 3
    b:int = 8
    c:int = 19
    d:int = -10
    c = d = a = b = 3
    return a


if __name__ == "__main__":
    print(fun())


