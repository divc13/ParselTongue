def main():
    c:list[int] = [2, 3]
    d:int = c[0][1]     # not 2d
    e:int = c
    f:int = c[-1]   #C-style

if __name__ == "__main__":
    main()