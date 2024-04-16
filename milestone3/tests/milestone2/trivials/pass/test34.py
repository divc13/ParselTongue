def hellNo(a: int , b: bool, c: int) -> int:
    return 3

def fun() -> int:
    if hellNo(1, 1, 1):
        return hellNo(1, 1, 1)
    return -1


if __name__ == "__main__":
    print(fun())


