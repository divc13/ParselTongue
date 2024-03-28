def class1():
    print("function")

class class1:   # function and class have same name

    def __init__(self):
        print("class")
    
var1: int = 10

func1: float = 23. # same function and var name

def func1() -> None:
    global var1     # changing the type of global var
    var1: str = "hello"
    print(var1)

print(var1)

def main():
    class1()
    obj1:class1 = class1()

    x:int = 4       
    x:float = 5.
    x: str = "fjei"     # changing the type of var in same scope

    y:float = 5.34 + True   # float <-> bool
    print(y)

    y:float = 3.14  # re-declaring

    

if __name__ == "__main__":
  main()