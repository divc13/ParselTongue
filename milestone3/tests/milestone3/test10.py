def f1(val:int) -> int:
    val1:int = val
    val2:int = 2 * val1
    val3:int = 3 * val2

    ans:int = 2

    ans **= 2
    ans = val1 + val2
    ans *= 2
    ans //= 3
    ans -= val1
    ans %= 100

    ans >>= 2
    ans = val1 & val2
    ans = val2 | val3
    ans *= ~val2
    ans >>= 15

    return ans

def sum(val:int) -> int:
    ans:int
    if val <= 0:
        ans = 0
    
    else:
        ans = val + sum(val - 1)
    
    return ans



def main():
    i:int
    ans:int = 0

    for i in range(50000000):       
        ans += f1(i)
        if ans > 100000000000000 or ans < -100000000000000:
            ans = 0

    print(ans)      # -3373131905643

    n:int = 500
    tot_sum:int = sum(n)  # recursion test

    print(tot_sum)  # 125250

if __name__ == "__main__":
    main()

