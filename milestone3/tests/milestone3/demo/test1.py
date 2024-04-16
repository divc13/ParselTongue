# recursion

def factorial(n:int) -> int:
    ans:int
    if n < 0:
        ans = -1
    
    elif n == 0:
        ans = 1
    
    else:
        ans = n * factorial(n - 1)

    return ans

def main():
    n:int = 5
    print(factorial(n))

if __name__ == "__main__":
    main()


    

