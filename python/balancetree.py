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
    

    @staticmethod
    def balance(node):
        if node is None:
            return 0
        return TreeBST.height(node.left) - TreeBST.height(node.right)

    @staticmethod
    def insert(node, val):
        #BST insert
        if node is None:
            return TreeNode(val)
        if val < node.val:
            node.left = TreeBST.insert(node.left, val)
        elif val > node.val:
            node.right = TreeBST.insert(node.right, val)
        else:
            print("duplicated val:", val)
            return node
        #update hight:
        node.height = 1 + max(TreeBST.height(node.left), TreeBST.height(node.right))

        #rebalance
        balance = TreeBST.balance(node)

        #left-left
        if balance>1 and val < node.left.val: 
            return TreeBST.rightRotate(node)
        #right-right
        if balance<-1 and val > node.right.val: 
            return TreeBST.leftRotate(node)
        #left-right
        if balance>1 and val > node.left.val:
            node.left = TreeBST.leftRotate(node.left)
            return TreeBST.rightRotate(node)
        #right-left
        if balance<-1 and val < node.right.val:
            node.right = TreeBST.rightRotate(node.right)
            return TreeBST.leftRotate(node)

        #unchanged
        return node
            
    @staticmethod
    def leftRotate(node):
        x = node.right
        xl = x.left
        
        x.left = node
        node.right = xl

        node.height = max(TreeBST.height(node.left), TreeBST.height(node.right))
        x.height = max(TreeBST.height(x.left), TreeBST.height(x.right))
        return x


    @staticmethod
    def rightRotate(node):
        x = node.left
        xr = x.right
        x.right = node
        node.left = xr
        
        node.height = max(TreeBST.height(node.left), TreeBST.height(node.right))
        x.height = max(TreeBST.height(x.left), TreeBST.height(x.right))

        return x



    def find(self, val):
        pass

    @staticmethod
    def inorder(node):
        if node is None:
            return
        TreeBST.inorder(node.left)
        print("node:", node.val, " height:", node.height)
        TreeBST.inorder(node.right)

    def next(self):
        pass


if __name__ == "__main__":
    print("build bst from a list")
    bst = TreeBST([5,4,3,2,1])
    bst.inorder(bst.root)

    print("build bst with insertion")
    root = None
    root = TreeBST.insert(root, 10)
    root = TreeBST.insert(root, 20)
    root = TreeBST.insert(root, 30)
    root = TreeBST.insert(root, 40)
    root = TreeBST.insert(root, 50)
    root = TreeBST.insert(root, 25)
    TreeBST.inorder(root)

