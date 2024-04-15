class c1:

    def __init__(self, a:list[int]):
        self.a:list[int] = a
    
    def sort_class(self) -> list[int]:
        # selection sort
        n:int = len(self.a)

        x:list[int] = self.a
        i:int
        for i in range(n):
            min:int
            min = i
            j:int = 1000

            for j in range(i+1, n):
                if x[j] < x[min]:
                    min = j
            
            temp:int
            temp = x[i]
            x[i] = x[min]
            x[min] = temp

        return x


def sort_func(a:list[int]) -> None:
    # insertion sort

    n:int = len(a)
    i:int;  key:int; j:int

    for i in range(1, n):
        key = a[i]
        j = i - 1

        while j >= 0 and a[j] > key:
            a[j + 1] = a[j]
            j -= 1

        a[j + 1] = key

    return



def main():
    a:list[int] = [2, 1, 4, 5, 3]
    obj1:c1 = c1(a)

    b:list[int] = obj1.sort_class()
    i:int

    sort_func(a)

    # for i in range(len(a)):
    #     print(a[i])

    # for i in range(len(b)):
    #     print(b[i])

    for i in range(len(a)):
        if(b[i] != a[i]):
            print("not working")


if __name__ == "__main__":
    main()

