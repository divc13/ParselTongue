def basic_test() -> None:
    a:int = 10;
    b:int = 20
    c:int = 0;

    c = a + b
    print(c)    # 30
    b = c - a
    print(b)    #20
    a = 4 // 3
    print(a)    # 1
    c = 4 / 2
    print(c)    # 2
    c = 2
    b = 100 % c
    print(b)    # 0

    c = a * b   
    print(c)    # 0
    c = a ** 2 
    print(c)    # 1

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
    print(a)    # 1
    b <<= 1
    print(b)    # 4

    c = a & b;  print(c)    # 0
    c = a | b;  print(c)    # 5
    c = a ^ b;  print(c)    # 5
    c = ~a;      print(c)   # -2
    
    c += a + b;                 print(c)    # 3        
    c -= a - b;                 print(c)    # 6
    c *= a * b;                 print(c)    # 24
    c = 100;                    print(c)    # 100
    c /= 10 / 2;                print(c)    # 20
    c= 20;                      print(c)    #20
    c //= 10 // 3 ;             print(c)    # 6
    c %= 34 % 7;                print(c)    #0
    c **= a ** 3;               print(c)    #0
    c = 10  ;                   print(c)    #10
    c &= a & b;                 print(c)    #0
    c |= a | b;                 print(c)    #5
    c ^= a ^ b;                 print(c)    #0

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


