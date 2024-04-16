# if-elif-else, for, while, range(start, stop), range(stop), len(), list[int]

def fibonacci(n: int) -> int:
    ans:int
    if n < 0:
        print("Fibonacci is only for non-negative numbers")

    elif n <= 1:
        ans = n

    else:
        prev: int = 0
        curr: int = 1

        i:int = 2
        while(i < n + 1):
            print(i)
            i += 1
            temp: int = prev
            prev = curr
            curr = prev + temp
        
        ans = curr

    return ans

def main() -> int:
    i:int

    numbers:list[int] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    
    # find sum of odd and even numbers
    even_sum:int = 0; odd_sum:int = 0
    for i in range(len(numbers)):
        if i % 2:
            even_sum += numbers[i]

        else:
            odd_sum += numbers[i]
    
    print(odd_sum); print(even_sum)

    # find fibonacci
    n:int = 10
    fib_n:int = fibonacci(n)
    print(fib_n)

    # find the count of unique numbers and 
    count:int = 1
    duplicate_numbers:list[int] = [1, 1, 1, 2, 2, 3, 4, 5, 5]

    prev:int = duplicate_numbers[0]
    for i in range(1, len(duplicate_numbers)):
        if prev == duplicate_numbers[i]:
            continue

        else:
            prev = duplicate_numbers[i]
            count += 1
    
    print(count)
    return 0

if __name__ == "__main__":
    main()
