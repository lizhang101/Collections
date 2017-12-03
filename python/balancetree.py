class TreeNode:
    def __init__(self, d=0):
        self.val = d;
        self.left = None
        self.right = None
        self.height = 1


class TreeBST:

    #@classmethod
    #def FromList(cls, x, l, r):
    #    pass

    @staticmethod
    def ListToBST(x, l, r):
        if l > r :
            return None
        m = (l + r)//2
        root = TreeNode(x[m])
        root.left = TreeBST.ListToBST(x, l, m-1)
        root.right = TreeBST.ListToBST(x, m+1, r)
        root.height = 1 + max(TreeBST.height(root.left), TreeBST.height(root.right))
        return root
    @staticmethod
    def height(node):
        if node is None:
            return 0
        return node.height

    def __init__(self, l=None):
        if l:
            x = sorted(l)
            self.root = TreeBST.ListToBST(x, 0, len(x)-1);
        else:
            self.root = None

    def root(self):
        return self.root
    

    def balance(self):
        pass

    def insert(self, k):
        #BST insert
        if node is None:
            return TreeNode(val)
        if val < node.val:
            node.left = insertBST(node.left, val)
        elif val > node.val:
            node.right = insertBST(node.right, val)
        else:
            print("duplicated val:", val)
            return node
        #update hight:
            
        #Rebalance
        pass

    def find(self, val):
        pass

    def inorder(self, node):
        if node is None:
            return
        self.inorder(node.left)
        print("node:", node.val)
        self.inorder(node.right)

    def next(self):
        pass


if __name__ == "__main__":
    bst = TreeBST([5,4,3,2,1])
    bst.inorder(bst.root)
