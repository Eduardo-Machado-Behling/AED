# 508. [Most Frequent Subtree Sum](https://leetcode.com/problems/most-frequent-subtree-sum/)

Nome: Eduardo Machado Behling  
Turma: AED I - 2024/2 (M1)

### Problem

Given the root of a binary tree, return the most frequent subtree sum. If there is a tie, return all the values with the highest frequency in any order.

The subtree sum of a node is defined as the sum of all the node values formed by the subtree rooted at that node (including the node itself).

**Example 1:**

Input:

```
  5
 / \
2  -3
```

Output: `[2, -3, 4]`

**Example 2:**

Input:

```
  5
 / \
2  -5
```

Output: `[2]`

**Constraints:**

- The number of nodes in the tree is in the range `[1, 10^4]`.
- `-10^5 <= Node.val <= 10^5`

### How to build

Use CMake to build the solution:

```bash
$ cmake -S . -B build
$ cmake --build build
```

The executable will be in `./bin`, so to run, use:

```bash
$ ./bin/main
```

The output should look like:

```
${REPO_HOME}/AED/leetcode/medium/Most Frequent Subtree Sum/src/main.c:xx:Test1:PASS
...
${REPO_HOME}/AED/leetcode/medium/Most Frequent Subtree Sum/src/main.c:xx:Testn:PASS
-----------------------
n Tests 0 Failures 0 Ignored
OK
```

### Tests used

You can find the tests used in this problem in `./src/main.c` using the **Unity testing framework**. The test should look like:

```c
void Testx(void){
    // Inputs

    // Expected

    // Calling Function and getting outputs

    // Assert Expected == Result.
}
``` 