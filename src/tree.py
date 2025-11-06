

class Node:
    def __init__(self, val=None, index=None, thresh=None, left=None, right=None):
        self.val = val
        self.index = index
        self.thresh = thresh
        self.left = left
        self.right = right


def classify(node, fruit):
    if node.val is not None:
        return node.val
    if fruit[node.index] <= node.thresh:
        return classify(node.left, fruit)
    else:
        return classify(node.right, fruit)


root = Node(
    index=0,
    thresh=140,
    left=Node(
        index=1,
        thresh=0.5,
        left=Node(val="apple"),
        right=Node(val="orange")
    ),
    right=Node(
        index=1,
        thresh=0.5,
        left=Node(val="apple"),
        right=Node(val="orange")
    )
)


features = [
    [150, 0, 0],
    [130, 1, 1],
    [160, 0, 0],
    [120, 1, 1]
]
label = [0, 1, 0, 1]


fruit = [130, 1, 1]
x = classify(root, fruit)
print(f"fruit={x}")
