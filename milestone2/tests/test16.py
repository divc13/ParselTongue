class class1:

    def __init__(self, value: int):
        self.value1:int = value

# x:list[class1] = [class1(1), class1(2), class1(3)]

# i:int
# for i in range(len(x)):
#     print(x[i].value1)

class class2(class1):
    ans:int = 0

    def __init__(self, value1: int, value2: int):
        self.value2:float = value2
        class1.__init__(self, value1)
    

    def add(self) -> None:
        self.ans = self.value1 + self.value2
        print(self.ans)


class class3(class2):

    def __init__(self, value1:int, value2:int):
        class2.__init__(self, value1, value2)

    def sub(self) -> None:
        self.ans = self.value1 - self.value2
        print(self.ans)

    

def main():
    obj1: class2 = class2(10, 5)
    obj1.add()
    print(class2.ans)

    obj2: class3 = class3(10, 5)
    obj2.add()
    obj2.sub()

    print(obj2.ans)
    print(obj1.ans) 
    print(class2.ans)   

if __name__ == "__main__":
    main()
