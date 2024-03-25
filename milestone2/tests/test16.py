class A:
  # a:int
  def __init__(self):
    a:int = 1
    self.x: int = 1
    self.y: float = 3.14
    self.t: float = 3.14
  a:int = 1
  def b()->None:
    b:int = 5

class B(A):

  def __init__(self):
    # a:int = 1
    self.x:int = 1
    # self.y:float = 3.14
    # self.z:str = """I am \
    # \n new hekk"""

class C(B):

  def __init__(self):
    # a:int = 1
    self.x:int = 1
    # self.y:float = 3.14
    self.z:str = """I am \
    \n new hekk"""

class D:

  def __init__(self):
    # a:int = 1
    self.x:int = 1
    # self.y:float = 3.14
    self.z:str = """I am \
    \n new hekk"""
    
class E:

  def __init__(self):
    # a:int = 1
    self.x:int = 1


def main():
  a: A = A()
  b: B = B()


if __name__ == '__main__':
  main()
