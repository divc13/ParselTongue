def prime_factors(n: int) -> list[int]:
    factors: list[int] = []

    while n % 2 == 0:
        factors.append(2)
        n //= 2

    for i in range(3, n**2 + 1, 2):
        while n % i == 0:
            factors.append(i)
            n //= i

    if n > 2:
        factors.append(n)

    return factors

def triangular_number(n: int) -> int:
    return (n * (n + 1)) // 2

def is_perfect_square(n: int) -> bool:
    root: int = n ** 2
    return root * root == n

def sum_of_squares_of_prime_factors(n: int) -> int:
    factors: list[int] = prime_factors(n)
    sum_of_squares: int = 0

    for factor in factors:
        sum_of_squares += factor ** 2

    return sum_of_squares

def special_value(n: int) -> int:
    if is_perfect_square(sum_of_squares_of_prime_factors(n)):
        return triangular_number(n)
    else:
        return sum_of_squares_of_prime_factors(n)

result: int = special_value(100)
# print(result)

def main() -> None:
    pass

if __name__ == "__main__":
    main()
    
    