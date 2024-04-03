class A:
    x: int = 5
    def __init__(self):
        self.a:int = 10
        return
    
        
def main():
    a: A = A()
    a.a = 9
    b: A = A()
    print(a.a)
    return
