class c1:

    def __init__(self):
        self.val: int = 1

    def add(self) -> int:       # multiple same methods
        return self.val + 1
    
    def add(self) -> int:
        return -1
    

def main():
    obj1: c1 = c1()
    val:int = obj1.add()

    print(val)


if __name__ == "__main__":
    main()
