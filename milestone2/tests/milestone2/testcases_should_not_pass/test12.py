class c1:
    a:list[int] = [3, 4, 5]

    def __init__(self):
        self.b:list[int] = [1,2, 3]


def main():
    obj1: c1 = c1()
    val:int = c1.a[0]
    val2:int = obj1.b[0]

    print(val)
    print(val2)



if __name__ == "__main__":
    main()


