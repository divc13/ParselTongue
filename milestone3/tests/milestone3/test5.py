def basic_test() -> None:
    a:int = 10;b:int = 20
    c:int = 0;

    c = a + b
    b = c - a
    a = 4 // 3
    c = 4 / 2
    c = 2
    b = 100 % c

    c = a * b
    c = a ** 2

    if 5  > 4:
        print("working")
    
    if 2 < 3:
        print("working")

    if 3 == 3:
        print("working")

    if 3 <= 5:
        print("working")

    if 5 >= 3:
        print("working")

    if 1 != 2:
        print("working")
    
    if (True and False):
        print("not working")

    if (True or False):
        print("working")
    
    if not False:
        print("working")

    a = 3
    b = 2
    a >>= 1
    b <<= 1
    c = a & b; c = a | b; c = a ^ b;c = ~a
    
    c += a + b; c -= a - b; c *= a * b; c = 100; c /= 10 / 2; c= 20;  c //= 10 // 3 ; c %= 34 % 7; c **= a ** 3; 
    c = 10
    c &= a & b; c |= a | b; 
    c ^= a ^ b; 

    i: int 
    for i in range(1, 4):
        if (i == 1):
            j:int = 3
            while(j > 0):
                print(j)
                j -= 1
            print('working')
        
        elif (i == 2):
            continue;
            print("not working")

        else:
            break
            print("not working")


def main():
    basic_test()

if __name__ == "__main__":
    main()


