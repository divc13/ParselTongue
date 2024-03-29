var1: int = 3
var2: int = 1
var3:int = 5

def func1() -> None:  #recursion
  var1 -= 1;
  var2 *= 2
  var3 = 10

  if(var1 == 0):
    print(var2)
  if(var1):
    print("ieofem")
    func1()
  
if(var1 == 3):
  print(var1)

print(var3)

class class1:

  def __init__(self, float_var: float):
    self.var2: float = float_var
  
  def add(self) -> None:
    self.var2 += self.var2
    print(self.var2)


def main():
  a:str = """c"""
  b:str = 'D'

  if("a" < "b"):
    if ('''a''' <= 'b'):
      if("D" > 'C'):
        if(a >= b):
          print ("fjoief")

  print("a" < "b")
          
  if(a == 'c'):
    if(b != 'd'):
      print("fjiefmk")

  func1()
  obj1:class1 = class1(3.00)
  obj1.add()


if __name__ == "__main__":
  main()
  