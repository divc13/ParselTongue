arg2: int = 0
def foo() -> int: pass
class bar(): pass

# static polymorphism
class abc():
	def __init__(self):
		self.var1: float = 3.14

	def get_var1(self) -> float:
		return self.var1 + arg2 
	
	def print_var2(self) -> None:
		print("var2")
		return "this is the return value for an empty return function but works"
	
	def add(self, a: int = 2, b: int = 5, /,) -> int:
		return a + b;

	def add(self, a: float = 5.00e0, b: float = -1., c: float = -1) -> float:
		return a + b + c;

	

class abc_child(abc):
	def __init__(self):
		self.var1: str = "eiofjowi"

	def get_var1(self) -> str:
		return self.var1

def main() -> None:
	pass
	obj1: abc = abc()
	obj2: abc_child = abc_child()

	print(obj1.get_var1)		# does not get the value but still valid syntax
	print(obj2.get_var1())
	

	print(obj1.add(10, 5));
	print(obj1.add(5.0, b = -1, c = -1))
	print(abc.add(2, 3, 4, 5 ))		# takes the last defined

	print(abc.print_var2(abc))
	print(abc.print_var2("any expr"))



if __name__ == '__main__':
	main()