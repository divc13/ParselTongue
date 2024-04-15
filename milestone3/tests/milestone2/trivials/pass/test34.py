def hellNo(a: float, b: bool, c: int) -> int:
    return 3.14

def fun() -> int:
    if hellNo(1, 1, 1):
        return hellNo(1, 1, 1)


if __name__ == "__main__":
    print(fun())


