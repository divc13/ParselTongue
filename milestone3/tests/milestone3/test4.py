def find_gcd(a: int, b: int) -> int:
    temp:int

    while b != 0:
        temp = b
        b = a % b
        a = temp
    
    return a


def f1(n:int) -> int:
    ans:int = -1
    
    if n <= 1:
        ans = n
    
    else:
        ans = n + f1(n-1) * find_gcd(n + 1, 36)
    
    return ans



def main():
    ans:int = f1(10)
    print(ans)

if __name__ == "__main__":
    main()



