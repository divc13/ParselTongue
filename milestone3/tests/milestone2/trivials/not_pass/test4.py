def fun() -> int:
    def gun() -> float:
        return fun() + True
    
    return gun()


a: float = fun()
