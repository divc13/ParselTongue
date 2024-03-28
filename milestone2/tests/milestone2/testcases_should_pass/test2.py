class class1:
    a:str = "fjid";
    b:bool = False
    c:float
    d:float
    ans:int

    def __init__(self, var1:int, var2:float = 10.00E-1):
        self.c = var1
        self.d = var2
        # c = var1
        # d = var2
    
    def divide_(self) -> None:
        ans = self.c // self.d
        print(ans)

        self.ans = self.c // self.d
        print(self.ans)


def divide_(var1:int, var2:float = 1.) -> None: # function name same as class method
    ans:int = var1 // var2
    print(ans)
    print(var1 // var2)


def main():

    obj1: class1 = class1(5, 2)
    obj1.divide_();

    divide_(5, 2);

    i:int
    for i in range(0, 2):
        print(i)

    # i = 1;
    for i in range(2):
        print(i)
    
    x:list[int] = [3, 4, 5, ]

    for i  in range(len(x)):
        print(x[i])

    s1: str = "abc"
    for i in range(len(s1)):
        print(s1[i])

    if(0 < True):
        print("fie")

    var1:int = (True + True) * True - False + 10.0
    print(var1)        

if __name__ == "__main__":
  main()