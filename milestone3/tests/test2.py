# class and multilevel inheritance

class Shape:

    def __init__(self):
        self.name:str = "Shape class"

    def area_of_shape(self) -> None:
        print("area of shape")
        return
    
    def perimeter_of_shape(self) -> None:
        print("perimeter of shape")
        return

class Rectangle(Shape):

    def __init__(self, length:int, width:int):
        self.length:int = length
        self.width:int = width

    def area(self) -> int:
        return self.length * self.width

    def perimeter(self) -> int:
        return 2 * (self.length + self.width)
    
class Square(Shape):

    def __init__(self, length:int):
        self.length:int = length

    def area(self) -> int:
        return self.length * self.length

    def perimeter(self) -> int:
        return 4 * self.length
    
def main():
    square_obj:Square = Square(5)
    rect_obj:Rectangle = Rectangle(3, 4)

    square_obj.area_of_shape()
    print(square_obj.area())
    print(rect_obj.area())

    rect_obj.perimeter_of_shape()
    print(square_obj.perimeter())
    print(rect_obj.perimeter())


if __name__ == "__main__":
    main()
    

