def main():
    b: int = 600
    if False:
        b: int =  500   # redeclaration
    print(b)

if __name__ == "__main__":
       main()
