def f(a:int) -> None:
    if a == 1:
        b: int = 500    #redeclaration
    elif a == 2:
        b : str = "hello"
    else:
        b: float = 3.14

def main():
    f()

if __name__ == "__main__":
       main()
