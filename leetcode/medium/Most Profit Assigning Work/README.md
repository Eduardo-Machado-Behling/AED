# 826. [Most Profit Assigning Work](https://leetcode.com/problems/most-profit-assigning-work/)

Nome: Eduardo Machado Behling  
Turma: AED I - 2024/2 (M1)

### Problem
You have `n` jobs and `m` workers. Given three arrays:
- `difficulty[i]` represents the difficulty of the `i`th job.
- `profit[i]` represents the profit of the `i`th job.
- `worker[j]` represents the maximum difficulty of a job that the `j`th worker can handle.

Every worker can be assigned at most one job but can also choose to do no job. Each job can be completed by multiple workers. The goal is to assign jobs to workers to maximize the total profit.

Return the maximum total profit possible.

##### Example 1:

    Input: difficulty = [2,4,6,8,10], profit = [10,20,30,40,50], worker = [4,5,6,7]
    Output: 100
    Explanation: Workers can complete jobs with the highest available profit they can handle.

##### Example 2:

    Input: difficulty = [85,47,57], profit = [24,66,99], worker = [40,25,25]
    Output: 0

##### Constraints:

- `1 <= n, m <= 10^5`
- `difficulty.length == profit.length == n`
- `worker.length == m`
- `1 <= difficulty[i], profit[i], worker[j] <= 10^5`

### How to build

Use CMake to build the solution:
```bash
$ cmake -S . -B build
$ cmake --build build
```
The executable will be in `./bin`, so to run use:
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
You can find the tests used in this problem in `./src/main.c` using the **Unity testing framework**. The test should look like:
```C
void Testx(void){
    // Inputs

    // Expected

    // Calling Function and getting outputs

    // Assert Expected == Result.
}
```

