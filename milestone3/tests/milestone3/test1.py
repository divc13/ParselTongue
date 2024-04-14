def power(base: int, exp: int) -> int:
    temp: int
    ans:int

    if exp == 0:
        ans = 1
    
    elif exp == 1:
        ans = base
    
    elif exp % 2 == 0:
        temp = power(base, exp // 2)
        ans = temp * temp
    
    else:
        temp = power(base, exp // 2)
        ans = base * temp * temp
    
    return ans
    

def sum_of_powers(n: int, exp: int) -> int:
    tot_sum: int = 0

    i: int = 200
    for i in range(1, n + 1):
        tot_sum += power(i, exp)

    
    return tot_sum



def display_power_table(n:int, exp:int) -> None:

    i:int
    for i in range(1, n+1):
        print(power(i, exp))


def main():
    print("abcd'")

    n:int = 3
    ans1:int = sum_of_powers(n, 2);

    print(ans1)

    ans2: int = sum_of_powers(n, 3)

    print(ans2)

    x:int = 10
    display_power_table(x, 2)



if __name__ == "__main__":
    main()


