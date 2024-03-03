# this test case tests different operators, recursive and list operations


def basic_test() -> None:
  i: int = 0

          # prints 
# 1 to 5
  
  for i   \
      \
\
  in range(1   
          
          # this should 
#work
                , 6):
    print   (
                    
i)

  string1: str = "this is a valid \
\
single line string"
  
  string2: str = '''this 
  is \
a valid 
multiline string'''

  print(string1)
  print(string2)

  pi: float
  pi = 3.142

  check: bool = False

  if not check:
    if True:
      assert 0xA + 0xB == 0x15
      assert 10_1 - 101 == 00_0_0_0
      assert 10 * 5 == 5_0
      assert 10. / 5.0_0 == 2.
      assert 10 // 3 == 3
      assert 10 % 3 == 1
      assert 0o17 ** 2 == 0o341
      assert 10. == 10.00_0E0
      assert 10 != 5
      assert 10e1 > 5.E-00_0
      assert 10 < 20
      assert 10E0 >= 10.
      assert 10 <= 10      

    if (10 > 5) and (pi < 10.0):
      if (1 > 2) or (2 > 1):
        assert 0b1010 & 0b0110 == 0b0010
        assert 0b1010 | 0b0110 == 0b1110
        assert 0o17 ^ 0o15 == 0o2
        assert ~0o17 == -0o20
        assert 0xA << 2 == 0x28
        assert 0xA >> 2 == 0x2

      if False:
        print("This should not print")

      elif False:
        print("This should not print")
      
      else:
        x: int = 10_00
        x += 5
        assert x == 10_05
        x -= 3
        assert x == 1002
        x *= 2
        assert x == 2004

        f: float = 24.00_0
        f /= 4.
        assert f == 6.0e-0
        f //= 2.0_0E0_0
        assert f == 3.
        f %= 2.0
        assert f == 1.0
        f = 10.0
        f **= 3.              
        assert f == 1000.0
        
        y: int = 0b110
        y &= 0b10_1
        assert y == 0B_10_0
        y |= 0B011
        assert y == 0b111
        y ^= 0b010
        assert y == 0b101
        y <<= 1
        assert y == 0b1010
        y >>= 1
        assert y == 0b101

        z: int = 0xA
        z &= 0XB
        assert z == 0x_A
        z |= 0x5
        assert z == 0xF
        z ^= 0xC
        assert z == 0x3
        z <<= 1
        assert z == 0x6
        z >>= 1
        assert z == 0x_3

        w: int = 0o_17
        w &= 0o1_5
        assert w == 0o_15
        w |= 0o11
        assert w == 0o15
        w ^= 0o6
        assert w == 0o13
        w <<= 1
        assert w == 0o26
        w >>= 1
        assert w == 0O_1_3

  return
  

def misc() -> None:
  return


def main() -> None:
  basic_test()
  misc()        # empty function call

if __name__ == "__main__":
  main()

  
