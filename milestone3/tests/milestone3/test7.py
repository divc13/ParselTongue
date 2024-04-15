class c1:

    def __init__(self):
        self.a:str = "c1"
        self.b:list[int] = [1, 2, 3]

    def f1(self) -> int:
        print("c1")
        return 10 + 10;

    def f2(self) -> None:
        print("abc")
    
class c2(c1):

    def __init__(self):
        self.c:str = "c2"

    def f2(self) -> None:
        print("abc")
        

def main():
    obj1:c1 = c1()
    obj3:c1 = obj1

    i:int 
    x:list[int]

    print(obj3.a)
    x = obj3.b
    # for i in range(len(obj3.b)):
    #     print(x[i])

    print(obj1.a)
    
    # x = obj1.b
    # for i in range(len(obj1.b)):
    #     print(x[i])

    for i in range(len(obj1.b)):
        temp:list[bool] = obj1.b
        if temp[i] != x[i]:
            print("not working")

    
    obj2:c2 = c2()
    obj3.f2()
    obj2.f2()
    y:int = obj2.f1()
    print(y + 10)



if __name__ == "__main__":
    main()


