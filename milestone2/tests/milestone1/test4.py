def generate_permutations(nums: list[int]) -> list[int]:
    permutations: list[int] = [1, 2, 3]
    a:int = 10

    # function inside a function
    def backtrack(rem_nums: list[int], curr_permutation: list[int]) -> None:
        # if  rem_nums:
            # permutations.extend(curr_permutation)
            # return
        
        i: int
        for i in range(len(rem_nums)):
            num: int = rem_nums[i]
            # curr_permutation.append(num)
            # rem_nums.remove(num)

            backtrack(rem_nums, curr_permutation)

            # rem_nums.insert(i, num)
            # curr_permutation.pop()

    # backtrack(nums, [])

    return permutations

def handle_list() -> None:
    start_list: list[int] = [1, 2, 3]
    # start_list.append(4)
    # start_list.extend([5, 6])
    # start_list.sort()
    # start_list.reverse()
    # start_list.pop()

    print("Start list is [6, 5, 4, 3, 2]")

    # all the permutations are stored in a single list
    permutations: list[int] = generate_permutations(start_list)
    print("Permutations of the list is calulated")

    i: int = 0
    while i < len(permutations):
        sum: int = permutations[i] + permutations[i+1] + permutations[i+2] + permutations[i+3] + permutations[i+4]
        if sum > 10:
            print("First permutation with sum greater than 10")
            print(permutations[i])
            print(permutations[i+1])
            print(permutations[i+2])
            print(permutations[i+3])
            print(permutations[i+4])
            break
        else:
            i += 5
            continue

def main() -> None:
  handle_list()  # recursion and list operations testing

if __name__ == "__main__":
  main()


