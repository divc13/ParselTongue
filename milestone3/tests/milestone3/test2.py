def factorial(n:int) -> int:
    i:int; ans:int = 1

    for i in range (1, n+1):
        ans *= i

    return ans


def fibonacci(n: int) -> int:
    if n <= 1:
        return n

    prev: int = 0
    curr: int = 1

    i:int
    for i in range(2, n + 1):
        temp: int = prev
        prev = curr
        curr = prev + temp

    return curr

def f1() -> None:
    b:int = 382

    x:int = 300
    max_iter:int = 10
    iter:int = 0 

    while(b > 0 and iter < max_iter):
        iter += 1
        x //= 32 - 4 % 2 + 3 * False - 2 - True
        b -= x
        print(b)
    
        i: int

        a:list[bool] = [True, False, False]

        if (a[0]):
            for i in range(3):
                print(i)
        
        elif (True):
            y:int = 20

    print("completed f1")


def main():
    a:str = "do nothing"


if __name__ == "__main__":
    main()

    n:int = 10

    print(factorial(n) - fibonacci(n))

    f1()



