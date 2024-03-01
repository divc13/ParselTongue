# assignment of classes

def func1() -> None:
    print("first implementation")

func1()

def func1() -> str:
    return "second implementation"

class abc():
    static_variable: int = 10

    def __init__(self, attr3: bool = True, ):        self.attr1: int = 1;        self.attr2: bool = attr3;

    @staticmethod
    def static_method():
        print("static method")
        print(abc.static_variable)
        # cant print(self.attr1)

    def instance_method(self):
        print("instance method")
        print(self.attr2)

def main():
    print(func1())

    __abc: abc = abc();
    __abc2: abc = abc(False);

    print(__abc.attr1);print(__abc.attr2)
    print(__abc2.attr1); print(__abc2.attr2)

    __abc.static_method()
    __abc.instance_method()

    __abc2.static_method()
    __abc2.instance_method()

    __abc_copy: abc = __abc2
    __abc_copy.static_method()
    __abc_copy.instance_method()


if __name__ == '__main__':
    main()

