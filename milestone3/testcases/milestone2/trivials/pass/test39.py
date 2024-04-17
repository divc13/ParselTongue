def fun() -> None:

    a: int = 1
    b: int = 2
    c: int = 3
    d: int = 4

    e: int = a + b      #3
    print(e)
    e = a ** b #1
    print(e)
    e = 1
    e = e + a #2
    print(e)
    e = b - c + d * a #3
    print(e)
    e = a == b != c > d < a >= b <= c
    print(e)
    
if __name__ == "__main__":
    fun()
