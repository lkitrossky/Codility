========================================================================
    CONSOLE APPLICATION : Titanic Project Overview
========================================================================

There are icebergs, the ship goes from A to B, can touch IB but not intersect. The shortest way.


The first algorithm was as follows.
1.	Every polygon orders is edges and vortices. Not so easy, done sorting lines from one edge to others by determinant of coefficients.
2.	Function forbidden line segment: for every edge checked that is is not paralleled and there is not intersection inside both of segment and edge (via distance to ends). In addition forbidden are diagonals.
3.	The main part is recursive: find for every legal neighbours the shortest path, add the pass to the neighbor and compare to the previous. If it is non-zero and less, substitute.
End of recursion: straight to finish or no neighbors.
4.	Important: there is also Dejkstra algorithm. 
https://en.wikipedia.org/wiki/Shortest_path_problem
http://www.maxburstein.com/blog/introduction-to-graph-theory-finding-shortest-path/ 
https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm 





