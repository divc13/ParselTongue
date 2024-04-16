class c1:

    def __init__(self, val:str):
        self.a:int = True
        self.b:str = val

    def func_c1(self) -> int:
        if self.b < "a":
            print(self.b)

        elif self.b == "b":
            print("b")

        elif self.b < '''abc''':
            print('''abc''')
        
        else:
            print('abcd')
        
        return -1
    
class c2(c1):

    def __init__(self, val: int):
        self.c:bool = val
        self.d:int = self.c

class c3(c2):

    def __init__(self, val: int):
        # c2.__init__(self, val)                # ye pakka support nhi karna hai na?
        self.e:int = val;

    def _print(self) -> None:
        print("c3")

class c4(c2):

    def __init__(self, val: int):
        self.e:int = val
    
    def func_c4(self) -> int:
        return self.e + 10
    
class c5(c4):

    def __init__(self, val: int):
        self.f:int = val
    
    def func_c5(self) -> int:
        print(self.f)
        return self.f
    
def f2(obj1:c5) -> c2:

    obj2:c2 = c2(obj1.f)
    print(obj2.c)
    print(obj2.d)

    return obj2



def f1(val:int) -> None:
    if val <= 0:
        return
    

    i:int
    for i in range(1, 42):
        if(i == 1):
            f1(val - 1)

        elif (i == 2):
            print(i)

        elif (i == 3):
            x:int = 3
            continue

        elif (i == 4):
            j:int
            for j in range(4):
                print(j)

        elif (i == 5):
            j = 5
            while(j > 0):
                j -= 1
                print(j)

        elif (i == 6):
            continue

        elif (i == 7):
            x >>= 1
            print(x)

        elif (i == 8):
            print(i)
            obj1:c1 = c1("abcdef")
            print(obj1.a)
            x = obj1.func_c1()
            print(x)

        elif (i == 9):
            print(i)

            obj3:c3 = c3(10)
            obj3._print()
            
        elif (i == 10):
            print(i)
        elif (i == 11):
            print(i)
        elif (i == 12):
            print(i)

        elif (i == 13):
            obj5:c5 = c5(10)
            print(obj5.func_c5())
            


        elif (i == 14):
            
            obj2:c2 = f2(obj5)
            print(obj2.c)
            print(obj2.d)

        elif (i == 15):
            print(i)
        elif (i == 16):
            print(i)
        elif (i == 17):
            print(i)
        elif (i == 18):
            print(i)
        elif (i == 19):
            print(i)
        elif (i == 20):
            print(i)
        elif (i == 21):
            print(i)
        elif (i == 22):
            print(i)

        elif (i == 23):
            print(i)
        elif (i == 24):
            print(i)
        elif (i == 25):
            print(i)
        elif (i == 26):
            print(i)
        elif (i == 27):
            print(i)
        elif (i == 28):
            print(i)
        elif (i == 29):
            print(i)
        elif (i == 30):
            main()

def main():
    print("in main")


if __name__ == "__main__":
    main()
    f1(2)



