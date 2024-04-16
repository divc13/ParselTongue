# operators and primitive data types

def main():
    a:int = 4
    b:int = 6

    # arithmetic operators
    result:int
    print("Testing arithmetic operators")
    result = a + b
    if (result == 10):
        print("test done for +")
    
    result = a - b
    if (result == -2):
        print("test done for -")
    
    result = a * b
    if (result == 24):
        print("test done for *")
    
    result = b // a
    if (result == 1):
        print("test done for //")
    
    result = b / 3
    if (result == 2):
        print("test done for /")
    
    result = b % a
    if (result == 2):
        print("test done for %")
    
    result = b ** a
    print(result)
    if (result == 1296):
        print("test done for **")

    # relational operators
    print("\nTesting arithmetic operators")
    if (a + 2 == b):
        print("test done for ==")

    if (a != b):
        print("test done for !=")

    if (b > a):
        print("test done for >")

    if (a < b):
        print("test done for <")

    if (a + 2 <= b):
        print("test done for <=")

    if (b - 2 >= a):
        print("test done for >=")

    # logical operators
    print("\nTesting logical operators")
    if (a + 2 == b) and (a != b):
        print("test done for and")

    if (a + 2 == b) or (b + 2 == a):
        print("test done for or")
    
    if not (a != 4):
        print("test done for not")
    
    # bitwise operators
    print("\nTesting bitwise operators")
    result = a & b
    if (result == 4):
        print("testing done for &")

    result = a | b
    if (result == 6):
        print("testing done for |")

    result = a ^ b
    if (result == 2):
        print("testing done for ^")

    result = ~a
    if (result == -5):
        print("testing done for ~")

    result = a << 1
    if (result == 8):
        print("testing done for <<")

    result = b >> 1
    if (result == 3):
        print("testing done for >>")


    # assignment operators
    print("\nTesting assignment operators")
    c:int = a
    if (c == a):
        print("test done for =")
    
    a += b
    if (a == 10):
        print("test done for +=")

    a -= b
    if (a == 4):
        print("test done for -=")

    a *= b
    if (a == 24):
        print("test done for *=")

    a /= b
    if (a == 4):
        print("test done for /=")

    b //= a
    if (b == 1):
        print("test done for //=")
    
    b = 6
    b %= a
    if (b == 2):
        print("test done for %=")

    b **= a
    print(b)
    if (b == 16):
        print("test done for **=")

    a = 4; b = 6
    b &= a
    if (b == 4):
        print("test done for &=")

    b |= a
    if (b == 4):
        print("test done for |=")

    b = 6
    b ^= a
    if (b == 2):
        print("test done for ^=")

    b <<= a
    if (b == 32):
        print("test done for <<=")

    b <<= a
    if (b == 512):
        print("test done for >>=")

if __name__ == "__main__":
    main()



