class A:
    def __init__(self):
        self.a: int = 1

def main():
    a: A = A()
    b: list[int] = range(a.a)
    print(b)


if __name__ == "__main__":
    main()


