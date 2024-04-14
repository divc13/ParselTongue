class rect:
    # var:str = "static var from rect"
    # c:int = 0

    def __init__(self, length: int, width: int):
        self.length:int = length
        self.width:int = width
        self.area:int = -1
        self.perimeter:int = -1
        self.z:int = 100

    def find_area(self) -> int:
        self.area = self.length * self.width
        return self.area

    def find_perimeter(self) -> int:
        self.perimeter = 2 * (self.length + self.width)
        return self.perimeter
    
    def f1(self) -> None:
        print("class:rect - f1")

    def f2(self) -> None:
        print("class:rect - f2")
    
class c1(rect):
    # a:int = 10  # static
    # d:bool = True

    def __init__(self, val:int):
        self.b:str = "c1"
        self.val:int = val
    
    def f1(self) -> None:
        print("class:c1")
        print(self.b)
        print(self.val)
        

def main():
    obj1: rect = rect(3, 4)
    print(obj1.find_area())
    print(obj1.find_perimeter())

    obj2:c1 = c1(5)
    obj2.f1()
    obj2.f2()

    print(obj2.b)



if __name__ == "__main__":
    main()

