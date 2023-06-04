# 2001 Thorup, Zwick - Compact routing schemes

Weighted undirected graphs
Improving routing table size (inside vertice information) for various stretch and small headers
Tree Covers approach is well-known before this paper

Section 2 - Routing in trees via heavy-light decomposition
Balls and clusters first used in L. Cowen. “Compact routing with minimum stretch.” ? (named here bunches and clusters)
Algorithm to construct landmarks W: E[|W|] <= 2s log n and |B_W(v)| < 4n/s

Possible to derandomize it? (2005 Approximate Distance Oracles) - reference to future paper :)

Focus on routing schemes, expanding work:
L. Cowen. Compact routing with minimum stretch. Journal of Algorithms, pages 170–183, 2001. Special issue for SODA’99.

# 2005 Thorup, Zwick - Approximate Distance Oracles

Weighted undirected graphs with non-negative edge weight
No (2,1) stretch oracle here

For each k there is algorithm preprocessing graph in expected time O(km(n^(1/k)) producing data structure of size O(n^(1 + 1/k)), query time O(k) answering approximate distance between two nodes, if real distance is d then returned in between d and d*(2k - 1), paths no longer then estimate can be retrieved in constant time per edge.

Space requirements optimal.
This produces steach 3, space O(n^(3/2)) Oracle.

# D. Dor, S. Halperin, and U. Zwick. All pairs almost shortest paths.
SIAM J. Computing, 29:1740–1759, 2000.

Earlier paper about oracles
Introducing Tree Covers to Distance Oracles?
Approximate Distance Oracles for Metric Spaces

Preprocessing:

A_0 = V. A_k = {}
for i <- 1 to k - 1
let A_i contain each element of A_(i-1) with probability n^(-1/k)
p_i(v): p_i(v) in A_i, dist(v, p_i(v) = dist(v, A_i))
B(v) <- sum i=0 ->k-1 {w in A_i - A_(i+1) | dist(w, v) < dist(A_(i+1), v)}
Oracle_dist(u,v):
w <- u, i <- 0
while w not in B(v)
i <- i+1
(u,v) <- (v,u)
w <- p_i(u)
return dist(w, u) + dist(w, v)

Derandomization (Works only for metric spaces):
LEMMA 3.6. Let N_1,..., N_n subsets of U be a collection of sets with
|U| = u and |N_i| >= s, for 1 <= i <= n. Then, a set A of size at most u/s
s (ln (ns/u) + 1) <= u/s (ln n + 1) such that elements in both N_i an A =/= 0 for 1 <= i <= n,
can be found, deterministically, in O(u + sum i= ->n (|N_i|)) time.
The set A, whose existence is claimed in Lemma 3.6, is obtained
by repeatedly adding to A elements of U that hit as many unhit sets
as possible, until only u/s
sets are unhit. The construction of A is
then completed by adding an element from each one of the unhit
sets.

More info at
N. Alon and J. Spencer. The probabilistic method. Wiley, 1992.

# 2006 Approximate Distance Oracles for Unweighted Graphs in Expected O(n^2) time
Here oracles are (3, 0) distance oracles
Claims that Thorup and Zwick first came up with Approximate Distance Oracle idea
Here is also presented first linear time algorithm for computing an optimal size (2, 1) -spanner of an unweighted graph.

2008 paper in known here
Description of work done in other papers, improvements to Thorup and Zwick oracle - preprocessing time
Very nicely written

# 2008 Baswana, Goyal, Sen - All-Pairs Nearly 2-Approximate Shortest Paths in O(n^2 polylog n) Time∗

(2,1) distance oracle on undirected unweighted graph
Inexplicitly first (2, 1) distance oracle of subquadratic space?