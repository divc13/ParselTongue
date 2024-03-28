def func1(val1: int, val2:int) -> int:
  return val1 + val2

def func1(val1:int, val2: int, val3: int) -> int: # same function name; does not support function overloading
  return val1 + val2 + val3

class cl1:
  def __init__(self):
    self.a:int = 1;

  def func1(self) -> None:
    self.b:int = 2;       # declaring var in non-init method

def func3() -> None:
  x:int = 6 + b;  # b is declared later
  print(x)

b:int = 3

def main():
  val1: int  = func1(3, 4)
  val2: int = func1(1, 2, 3)

  print(val1)
  print(val2)

  func3()




if __name__ == "__main__":
  main()