class cl1:

    def __init__(self, name:str):
        self.name: str = name;

    def print(self) -> None:
        print(self.name)
    
def create_obj() -> cl1:
    obj1:cl1 = cl1("obj1")
    return obj1

def create_objs() -> list[cl1]:
    obj1:cl1 = cl1("fhe");
    obj2:cl1 = cl1("fjie")
    
    # x:list[cl1] = [cl1("fei"), cl1("ifej")]
    x:list[cl1] = [obj1, obj2]
    return x

def func1() -> None:
    i: int
    for i in range(10):
        j:int = i
    
    print(j)

def main():
    obj1:cl1 = create_obj()

    obj1.print()

    obj2:list[cl1] = create_objs()
    a: cl1 = obj2[0]
    b: cl1 = obj2[1]
    a.print()
    print(b.name)


    

if __name__ == "__main__":
  main()
