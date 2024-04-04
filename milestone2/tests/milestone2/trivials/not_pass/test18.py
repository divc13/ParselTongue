class A:
    def __init__(self):
        self.a:int = 10
    
        class B:
            def __init__(self):
                self.b: A = A()
                self.a:int = 10
                return
            def hell(self) -> None:
                b: int = 0
                return
        return
    
        
def main():
    a: B = B()
    a.hell()
    return
