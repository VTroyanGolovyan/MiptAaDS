import graphviz



dot = graphviz.Digraph('Graph', format='svg')

n = int(input())
for i in range(n):
    dot.node(str(i), str(i))

mst = set()
m = int(input())
for i in range(m):
    a, b, w = tuple(map(int, input().split()))
    dot.edge(str(a), str(b), label=f"{w}", color="red")
    mst.add(str(a) + ":" + str(b))


m = int(input())
for i in range(m):
    a, b, w = tuple(map(int, input().split()))
    # if not (str(a) + ":" + str(b)) in mst:
    dot.edge(str(a), str(b), label=f"{w}")


dot.render()
