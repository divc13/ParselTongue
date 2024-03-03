def _func1__(var1: (3+4)) -> None:
	print("func1")

class abc():

	def __init__(self):
		self.var1: float = 3.14

	def get_var1(self) -> float:
		return self.var1 

def insertion_sort(arr: list[int]) -> list[int]:
	for i in range(1, len(arr)):
		key = arr[i]
		j = i - 1
		while j >= 0 and arr[j] > key:
			arr[j + 1] = arr[j]
			j -= 1
		arr[j + 1] = key
	return arr


def main() -> (5 + 3):
	x: 5;\
	_func1__(3)
	obj1: abc = abc();
	
	print(obj1.get_var1())
	print(obj1.var1)

	list1: list[int] = [12, 11, 13, 5, 6]
	sorted_list: list[int] = insertion_sort(list1)

	i:int
	for i in range(len(sorted_list)):
		print(sorted_list[i])

	return 8
	

if __name__ == "__main__":
	main()

