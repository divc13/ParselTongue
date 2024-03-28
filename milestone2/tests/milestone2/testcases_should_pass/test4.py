class class1:
    str1:str = "static var"

print(class1.str1)

def func2(a:int) -> int:
    return 10.0 + a

def func1() -> None:
    a:int = 10;
    a = func2(a)
    print(a)

    a = 10
    while(a > False):
        a -= 1
    
    if(a == False):
        print(a)

var1: int = 4

class cl1:
  
  def __init__(self):
    self.name:int = 5
  
  def func1(self) -> None:
    global var1
    var2: int = var1 + self.name
    print(var2)

def main():
    a:int = 10
    func1()

    obj1: cl1 = cl1();
    obj1.func1()

    b:int = func2(obj1.name)
    print(b)


if __name__ == "__main__":
  main()