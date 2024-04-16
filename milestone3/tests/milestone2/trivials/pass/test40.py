def fun() -> None:

    a: int = 1123
    b: int = 2223
    c: int = 3234
    d: int = 4678
    e: int = 1374
    mod:int = 99997
    
    e = ((((((((((e * a) % mod) * d) % mod) * b) % mod) *c ) % mod) * ((((((((e * a) % mod) * d) % mod) * b) % mod) *c ) % mod) % mod) * ((((((((e * a) % mod) * d) % mod) * b) % mod) *c ) % mod) % mod)
    print(e)

   
if __name__ == "__main__":
    fun()
