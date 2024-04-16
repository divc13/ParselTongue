# static polymorphism via method overloading

class Calculator:

    def __init__(self):
        self.ans:int

    def add(self, x:int, y:int) -> int:
        return x + y
    
    def add(self, x:int, y:int, z:int) -> int:
        return x + y + z
    
    def multiply(self, x:int, y:int) -> int:
        return x * y
    
    def multiply(self, x:int, y:int, z:int) -> int:
        return x * y * z
    
    def subtract(self, x:int, y:int) -> int:
        return x - y
    
    def divide(self, x:int, y:int) -> int:
        return x // y
    
def main():
    calc:Calculator = Calculator()

    result1:int = calc.add(2, 3)
    print(result1)

    result2:int = calc.add(2, 3, 4)
    print(result2)

    result3:int = calc.multiply(2, 3)
    print(result3)

    result4:int = calc.multiply(2, 3, 4)
    print(result4)


if __name__ == "__main__":
    main()



