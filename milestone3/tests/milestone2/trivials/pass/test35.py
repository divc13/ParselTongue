class A:
    def __init__(self):
        self.a:int = 10
        return
    
class B(A):
    def __init__(self):
        self.a = 8
        return
    
        
def main():
    a: B = B()
    return
