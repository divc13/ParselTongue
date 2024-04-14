class c1:

    def __init__(self, a:list[int]):
        self.a:list[int] = a
    
    def sort_class(self) -> list[int]:
        # selection sort
        n:int = len(self.a)

        i:int
        for i in range(n):
            min:int
            min = i
            j:int = 1000

            for j in range(i+1, n):
                if self.a[j] < self.a[min]:
                    min = j
            
            temp:int
            temp = self.a[i]
            self.a[i] = self.a[min]
            self.a[min] = temp

        return self.a


def sort_func(a:list[int]) -> list[int]:
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

    return a



def main():
    a:list[int] = [2, 1, 4, 5, 3]
    obj1:c1 = c1(a)

    b:list[int] = obj1.sort_class()
    i:int

    c:list[int] = sort_func(a)

    for i in range(len(a)):
        if(b[i] != c[i]):
            print("not working")

if __name__ == "__main__":
    main()

