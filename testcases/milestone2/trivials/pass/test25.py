def fun() -> int:
    return 1

def gun() -> float:
    return fun() + True

def hun() -> int:
    return gun() + 3

def iun() -> bool:
    hun()
    return hun() + -11
    
def jun() -> int:
    return ~iun()


if __name__ == "__main__":
    print(jun())


