def fun(a: int, b: int, c: bool) -> None:
    print(c)
    print(b)

def main():
    a:int = 1
    b:bool = -1
    print(b)
    c:int = 5
    b += c
    print(b + c)
    fun(a, b, c)

if __name__ == "__main__":
    main()
