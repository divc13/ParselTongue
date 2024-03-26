"""different types of assert \
statements"""

v:str = "qwerty"
x:int = 5
assert(v == "qwerty" 
	   or "1"), "this should not print" + 5 and not 1 or "hello"
print(v)
assert (-
		#this should work
0 + 1), "failed"
assert ("fije"), ()
assert 2, -1
assert -0.00001
assert (True or False and True)

def func1(a:list[float] = [0, 0], b:int = 10)-> None:
	i:int = 0;

	for i in range(len(a)):
		print(a[i])

def func2(a:int , b: float=4.4) -> str:
	return "func2";\


def main() -> None:
	""
	s = "this variable is not initialised\n";#this should be checked in semantic analysis phase
	print(s)

	print("passing less param to func1")
	func1([1])
	print(func2("hello", 5, 6))
	print(func2(b=4, a=3.3, void="hello"))
	print( 
		# explicit line joining
		\
		func2(
"hello",
# implicit line joining
			 "hello")) # passing different type of argument than expected, this should pass in parser


if __name__ == "__main__":
	main()
	

