class A:
    def __init__(self):
        self.a:int = 10
        return
    
class B:
    def __init__(self):
        b: A = A()
        b.a = 5
        self.a:int = 10
        return
    def hell(self) -> None:
        b: int = 0
        return
    
        
def main():
    a: B = B()
    a.hell()
    a.hell()
    print(a.a)
    return


if __name__ == "__main__":
    main()


