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


def main():
    n:int = 5
    result:int = fibonacci(n)

    print(result)

if __name__ == "__main__":
  main()
