pass
def func1():
	pass
	quit("ur4jf3wr049j")


def func2(param1:int = 10, param2:str = "default") -> None:
	print(param1)
	print(param2)
	return "hi";

def func3(void, param1:str,  param2:int = 10, param3: list[bool] = [False, False], ):

	
	pass;pass;pass;\
	return -1 + len("ehriwu") and not 1;	
	print("this should not be printed")

def func4(x: int, /):
	print("func4")

def func5(x:int, /, ):
	print("func5")

def func6(x:int = 7, /, y:int = 8, ):
	print("func6")

def func7(x:int, /, y:int, ):
	print("func7")

def func8(x:int, /, y:int, z: int,  * , z2:int = 10):
	print("func8")

def func9(x:int, *, z:int):
	print("func9")

def func10(x: int , /, p: int  , y:int = 2, *, z: int = 100):
	print("func10")
	

def main() -> None:
	func2   # should not print anything
	func2() # should print default values
	func3(1, 2, 3)
	func3("\n\n", 30)

	a:int; b:int; c: float
	a = b = c = ([5, 3], ), 	# this should still parse correctly
	print(a)
	func4(1)
	func5(-1)
	func6
	func6(7, y = 10)
	func7(7, 5)
	func7(7, y = 2)
	func8(9, 0, z = 9,  z2 = 100)
	func9(1, z = 100)
	func10(1, 1)




	func1()		# quits


if __name__ == "__main__":
	main()