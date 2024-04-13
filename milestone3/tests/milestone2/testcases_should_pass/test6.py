def fib(n:int) -> int:
    if (n==1):
        return 1
    if (n==0):
        return 1
    return fib(n-1) + fib(n-2)

def main():
    print(range(4, 6))
    
if __name__ == "__main__":
    main()
