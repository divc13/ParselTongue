
# comment

class c1:

    def __init__(self):
        self.a:str = "abc\
            def"
        self.b:list[bool]
    
    def f1_c1(self) -> None:
        print("in f1 of c1")
        return 
    
class c2(c1):

    def __init__(self, val: int):
        self.c:bool = val

        
    def f1_c2(self) -> None:
        print("in f1 of c2")
        return 

class c3(c2):

    def __init__(self, val: int):
        self.e:int = val

    def f1_c3(self) -> None:
        print("in f1 of c3")


def f1() -> None:
    print(len('''"abcd"'''))    # 6
    print("'abcd'")


def main(val:int, a:list[int], b:bool, c:str):
    if val <= 0:
        return

    b = val + val + val + val + val + val + val + val + val + val+ val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val + val

    print(b + True - False)  

    main(val - 1, a, False, "def")


if __name__ == "__main__":
    a:list[int] = [1, 2, 3]
    main(5, a, False, "abc")

    f1()
    obj3:c3 = c3(10)
    obj3.f1_c1()
    obj3.f1_c2()
    obj3.f1_c3()




