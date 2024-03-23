var: float = 4.4
var2: bool = True
var3: str = "hello"
var4: list[float] = [4.4, 1.2]
a:int =3;b:int =4

def a() -> None:
	return

def change_global() -> None:
	global var
	global var2, var3, var4
	var = 5.5
	var2 = False
	var3 = "world"
	var4 = [0]

print(var)
print(var2)
print(var3)
print(var4)
change_global()
print(var);	print(var2);print(var3);\
print(var4)

def outer() -> None:
	x: int = 5
	y: list[bool] = [True, False]
	z: str = "abc"

	def inner() -> None:
		nonlocal x
		nonlocal y, z
		x = 10
		y = [False]
		z = 'idfje\
			ifej'
	inner()
	print(x)
	print(y)
	print(z)

outer()	


def main() -> None:
	'''
	this acts
	as a multiline comment
	'''
	
	5;10;

	x: int = 5; y: int = 10;
	u: int = 5; t: int = 10
	print(x); 
	print(y)

	x: str = "changing the type of x from int to str"
	print(x)


if __name__ == "__main__":
	main()
	
