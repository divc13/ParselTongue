class class1:
    nfs:int = 0
    def __init__(self, value: int):
        self.value1:int = value

# obj1:class1 = class1(1)
# obj2:class1 = class1(2)
# obj3:class1 = class1(3)

# a:list[int] = [1, 2, 3];
# # a[3.14]


# # x:list[class1] = [class1(1), class1(2), class1(3)]
# x:list[class1] = [obj1, obj2, obj3]

# i:int
# for i in range(len(x)):
#     b: class1 = x[i]
#     print(b.value1)

class class2(class1):
    ans:int = 0

    def __init__(self, value1: int, value2: int):
        self.value2:float = value2
        class1.__init__(self, value1)


#     def add(self) -> None:
#         class2.ans = self.value1 + self.value2
#         print(class2.ans)
    

class class3(class2):

    # def __init__(self, value1:int):
    #     a:int = 10
        # class2.__init__(self, value1, value2)
        
     def __init__(self, value1:int, value2:int):
         a:int = 17
    #     class2.__init__(self, value1, value2)

#     def sub(self) -> None:
#         class2.ans += - self.value2
#         print(class2.ans)

    

# def main():
#     obj1: class2 = class2(10, 5)
#     obj1.add()
#     print(class2.ans)

#     obj2: class3 = class3(10, 5)
#     obj2.add()
#     obj2.sub()

#     print(obj2.ans)
#     print(obj1.ans) 
#     print(class2.ans)   

# if __name__ == "__main__":
#     main()
