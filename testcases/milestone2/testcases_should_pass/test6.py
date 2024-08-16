def fib(n:int) -> int:
    if (n==1):
        return 1
    if (n==0):
        return 1
    return fib(n-1) + fib(n-2)

def main():
    i:int
    for i in range(12):
        print(fib(i))
if __name__ == "__main__":
    main()
