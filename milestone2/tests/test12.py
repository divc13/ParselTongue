def main():
    # implicit line joining should be supported within list
    list1: list[int] = [1
                           # this comment should be ignored\
, 

2, 3\
    , 4, 5,

    "string",
    
]
    print(list1)

    True; False
    s:str = '\b'
    print(s)

    hexa:str = "\u89AB"
    print(hexa)

    hexa_8:str = "\U0003892F"
    print(hexa_8)

    # redeclaring same variable but should parse
    list1: list[int] = [1, 2, 3, 4, 5]

    list1.append(6)
    print(list1) 

    another_list: list[int] = [7, 8, 9]
    list1.extend(another_list)
    print(list1)

    list1.insert(2, 10)
    print(list1) 

    list1.remove(5)
    print(list1)  

    remove_int: int = list1.pop(3)
    print(remove_int)  
    print(list1)  

    list1.clear()
    print(list1) 

    list1: list[int] = [1, 2, 3, 4, 5]

    find_index4: int = list1.index(4)
    print(find_index4)  

    count_of_3: int = list1.count(3)
    print(count_of_3) 

    list1.sort()
    print(list1) 

    list1.reverse()
    print(list1) 

    list1_copy: list[int] = list1.copy()
    print(list1_copy)

    length: int = len(list1)
    print(length) 


if __name__ == "__main__":
    main()
    
