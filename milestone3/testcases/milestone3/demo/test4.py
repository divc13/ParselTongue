# if-elif-else, for, while, range(start, stop), range(stop), len(), list[int], continue, break

def fibonacci(n: int) -> int:       # uses if-elif-else, while loop
    ans:int
    if n < 0:
        print("Fibonacci is defined only for non-negative numbers")

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
    
    # find sum of odd and even numbers          # uses for loop, range(stop), if-else, len(), list[int]
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

    # find the count of unique numbers          # uses range(start, stop), continue
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

    # find if a number is present in a list of numbers      # uses if, break
    present:bool = False
    find_number:int = 5
    numbers_list:list[int] = [2, 4, 1, 0, -1, 4, 5, 1]

    for i in range(len(numbers_list)):
        if numbers_list[i] == find_number:
            present = True
            break
    
    if present:
        print("The number is present")

    else:
        print("The number is not present")
    
    return 0


if __name__ == "__main__":
    main()


