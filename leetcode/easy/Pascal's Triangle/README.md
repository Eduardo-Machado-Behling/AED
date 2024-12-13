# 118. [Pascal's Triangle](https://leetcode.com/problems/pascals-triangle/description/)

### Problem
Given an integer numRows, return the first numRows of Pascal's triangle.

In Pascal's triangle, each number is the sum of the two numbers directly above it as shown:

![img](https://upload.wikimedia.org/wikipedia/commons/0/0d/PascalTriangleAnimated2.gif)

 
##### Example 1:

    Input: numRows = 5
    Output: [[1],[1,1],[1,2,1],[1,3,3,1],[1,4,6,4,1]]

##### Example 2:

    Input: numRows = 1
    Output: [[1]]

 

##### Constraints:
- 1 <= numRows <= 30

### How to build

use cmake to build solution:
```bash
$ cmake -S . -B build
$ cmake --build build
```
the executable will be in `./bin`, so to run use:
```bash
$ ./bin/main
```
Output should look like:
```
${REPO_HOME}/AED/leetcode/${PROBLEM_DIFFICULTY}/${PROBLEM_NAME}/src/main.c:xx:Test1:PASS
...
${REPO_HOME}/AED/leetcode/${PROBLEM_DIFFICULTY}/${PROBLEM_NAME}/src/main.c:xx:Testn:PASS
-----------------------
n Tests 0 Failures 0 Ignored
OK
```


### Tests used
you can find the tests used in this problem in `./src/main.c` using the **unity testing framework** the test should look like:
```C
void Testx(void){
    // Inputs

    // Expected

    // Calling Function and getting outputs

    // Assert Expected == Result.
}
```