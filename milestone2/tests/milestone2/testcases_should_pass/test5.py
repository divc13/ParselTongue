class rect:

    def __init__(self, length: int, width: int):
        self.length:int = length
        self.width:int = width
        self.area:int = -1
        self.perimeter:int = -1

    def find_area(self) -> int:
        self.area = self.length * self.width
        return self.area

    def find_perimeter(self) -> int:
        self.perimeter = 2 * (self.length + self.width)
        return self.perimeter


# class square(rect):

#     def __init__(self, side: int):
#         rect.__init__(self, side, side)

def main():
    obj1: rect = rect(3, 4)
    print(obj1.find_area())
    print(obj1.find_perimeter())
    perimeter: int = obj1.find_perimeter()
    # obj2 = square(4)
    # print(obj2.find_area())
    # print(obj2.find_perimeter())
    

if __name__ == "__main__":
  main()
