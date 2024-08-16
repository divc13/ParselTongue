class c1:

    def __init__(self, val:int):
        self.val:int = val        


class c2(c1):

    def __init__(self, obj1:c1):
        self.obj1:c1 = obj1
        
    def print_(self) -> None:
        temp_obj:c1 = self.obj1
        print(temp_obj.val)
        return


def main():
    obj1:c1 = c1(-1)

    obj2:c2 = c2(obj1)
    obj2.print_()    # -1


if __name__ == "__main__":
    main()

    