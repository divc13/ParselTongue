def _00(void) -> None:
	exit("fjieo");

def _func1_(var1: int) -> None:
	"oifej"

def _() -> str:
	return "_ is function"

class abc(): pass;

class __abc(): 
	def __init__(self):
		self.__priv: int = 5;

	def get_intval(self) -> int:
		return self.__priv

def main() -> None:
	_func1_(5 + 3)

	print("😀");	# should parse

	print(_())
	__: str
	__ = "is string"

	
	
	
	print(__)

	x:list[int] = [9, 0, 0, "abcdef", ]		# this should still parse
	i: int = 0
	for i in range(len(x)):
		print(x[i])

	obj1: __abc = __abc()
	print(obj1.get_intval())

	_00("anything");

if __name__ == "__main__":
	main()
