def func1(val1:int, val2:int) -> None:
    ans:int = val1 + val2
    print(ans)


def main():
    func1()         # incorrect no of args passed
    func1(3)
    func1(3, 4, 5)

    a:str = "fije"
    func1(a, 5)     # wrong type passed

    a:str = "fjei" + 5      # type don't match
    a = "fjei" + 3.3 - False
    a = "ffei" + True - 3.4

    





if __name__ == "__main__":
    main()