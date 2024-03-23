# this test case is a code to check whether the tree is balanced or not

class TreeNode:
  def __init__(self, val: int) -> None:
    self.val: int = val
    self.left: TreeNode = None
    self.right: TreeNode = None
    return
  

def height(node: TreeNode) -> int:
  if node is None:
    return 0
  return 1 + max(height(node.left), height(node.right))

def is_balanced(root: TreeNode) -> bool:
  if root is None:
    return True
  
  left_height: int = height(root.left)
  right_height: int = height(root.right)
  
  if abs(left_height - right_height) <= 1 and is_balanced(root.left) and is_balanced(root.right):
    return True
  return False

def main() -> None:
  root: TreeNode = TreeNode(1)
  root.left = TreeNode(2)
  root.right = TreeNode(3)
  left_child: TreeNode = root.left
  right_child: TreeNode = root.right
  left_child.left = TreeNode(4)
  right_child.right = TreeNode(5)

  if is_balanced(root):
    print("Tree is balanced")

  unbalanced_root: TreeNode = TreeNode(1)
  unbalanced_root.left = TreeNode(2)
  left_child: TreeNode = unbalanced_root.left
  left_child.left = TreeNode(3)

  if(not is_balanced(unbalanced_root)):
    print("Tree is not balanced")
    
if __name__ == "__main__":
  main()



  
