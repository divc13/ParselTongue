# recursion

def factorial(n:int) -> int:
    ans:int
    if n < 0:
        print("Factorial is defined only for non-negative numbers")
        ans = -1
    
    elif n == 0:
        ans = 1
    
    else:
        ans = n * factorial(n - 1)

    return ans

def main():
    n:int = 5
    ans:int = factorial(n)

    if ans > 0:
        print(ans)

if __name__ == "__main__":
    main()


    

