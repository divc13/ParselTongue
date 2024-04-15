# static polymorphism
def f1(val1:int, val2:int) -> int:
    print(val1 + val2)
    return val1 + val2

def f1(val1:str, val2:str) -> int:
    print(val1)
    print(val2)

    return 0



def main():
    f1(3, 4)
    f1("a", 'b')

    a:list[int] = [3, 2, 1]
    b:list[bool] = [False, True, False]
    
    i:int
    for i in range(len(a)):
        a[i] = a[i] + b[i]

    for i in range(len(a)):
        print(a[i])

    if f1(2, 4) < 10:
        temp:int = f1("working", "fine")
        print(temp)
        print(f1("working", "fine"))
    
    print("")

if __name__ == "__main__":
    main()

