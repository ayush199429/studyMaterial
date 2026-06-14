You are given an m x n grid initially filled with water.
You are given a list of queries.
Each query gives a cell (r, c).
Convert that cell from water to land.
After each query, return the current number of islands.

Soln approach:
1. If already land:
       return current islandCount

2. Mark current cell as land

3. islandCount++

4. Check 4 neighbours

5. If neighbour is land and belongs to a different component:
       union(current, neighbour)
       islandCount--

6. Append islandCount to answer
