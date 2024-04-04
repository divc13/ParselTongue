class A:
    x: int = 5
    def __init__(self):
        self.a:int = 10
        return
    
        
def main():
    a: A = A()
    a.x = 9
    b: A = A()
    print(b.x)
    return


