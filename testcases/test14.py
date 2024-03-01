class Node:
    def __init__(self, value: int):
        self.value: int = value
        self.left: 'Node' = None
        self.right: 'Node' = None

class BinaryTree:
    def __init__(self, values: list[int]):
        self.root: 'Node' = None
        for value in values:
            self.insert(value)

    def insert(self, value: int) -> None:
        if self.root is None:
            self.root = Node(value)
        else:
            self._insert_recursive(self.root, value)

    def _insert_recursive(self, node: 'Node', value: int) -> None:
        if value < node.value:
            if node.left is None:
                node.left = Node(value)
            else:
                self._insert_recursive(node.left, value)
        else:
            if node.right is None:
                node.right = Node(value)
            else:
                self._insert_recursive(node.right, value)

    def search(self, value: int) -> bool:
        return self._search_recursive(self.root, value)

    def _search_recursive(self, node: 'Node', value: int) -> bool:
        if node is None:
            return False
        elif node.value == value:
            return True
        elif value < node.value:
            return self._search_recursive(node.left, value)
        else:
            return self._search_recursive(node.right, value)

    def inorder_traversal(self) -> list[int]:
        result: list[int] = []
        self._inorder_traversal_recursive(self.root, result)
        return result

    def _inorder_traversal_recursive(self, node: 'Node', result: list[int]) -> None:
        if node is not None:
            self._inorder_traversal_recursive(node.left, result)
            result.append(node.value)
            self._inorder_traversal_recursive(node.right, result)


values: list[int] = [7, 3, 10, 5, 1, 9, 12]
tree: BinaryTree = BinaryTree(values)
print(tree.inorder_traversal()) 
print(tree.search(5))  
print(tree.search(8))  


if __name__ == "__main__":
    main()
