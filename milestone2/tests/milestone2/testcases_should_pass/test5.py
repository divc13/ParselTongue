class cl1:

    def __init__(self, name:str):
        self.name = name;

    def print(self) -> None:
        print(self.name)
    
def create_obj() -> cl1:
    obj1:cl1 = cl1("obj1")
    return obj1

def create_objs() -> list[cl1]:
    x:list[cl1] = [cl1("fei"), cl1("ifej")]
    return x

def func1() -> None:
    i: int
    for i in range(10):
        j:float = i
    
    print(j)

i: int
for i in range(10):
    print(i)

def main():
    obj1:cl1 = create_obj()

    obj1.print()

    obj2:list[cl1] = create_objs()
    obj2[0].print()
    print(obj2[1].name)


    

if __name__ == "__main__":
  main()