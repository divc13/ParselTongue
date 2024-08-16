def fun() -> int:
    a:int = len("hello")

    a = len(range(len(range(a))))
    return a

if __name__ == "__main__":
    print(fun())


