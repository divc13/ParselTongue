def fun() -> None:
    a: int = 1
    b: int = 2
    c: int = 3
    d: int = 4

    e: int = a + b      #3
    print(e)
    e = a ** b #1
    print(e)
    e = e + a #2
    print(e)
    e = b - c + d * a #3
    print(e)
    e = b - c + d * a // b // d % c
    print(e)
    e = a == b != c > d < a >= b <= c
    print(e)
    e = a and b and not c or not d 
    print(e)
    e = not e
    print(e)
    e = ~ a & b | c ^ d << a >> d
    print(e)
    e += a
    print(e)
    e -= a
    print(e)
    e *= a
    print(e)
    e /= b
    print(e)
    e //= a
    print(e)
    e %= a
    print(e)
    e **= a
    print(e)
    e &= a
    print(e)
    e |= a
    print(e)
    e ^= a
    print(e)
    e <<= d
    print(e)
    e >>= a
    print(e)


if __name__ == "__main__":
    fun()

