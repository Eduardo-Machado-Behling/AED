# 1792. [Maximum Average Pass Ratio](https://leetcode.com/problems/maximum-average-pass-ratio/description/)

Nome: Eduardo Machado Behling
Turma: AED I - 2024/2 (M1)

### Problem
There is a school that has classes of students and each class will be having a final exam. You are given a 2D integer array classes, where classes[i] = [passi, totali]. You know beforehand that in the ith class, there are totali total students, but only passi number of students will pass the exam.

You are also given an integer extraStudents. There are another extraStudents brilliant students that are guaranteed to pass the exam of any class they are assigned to. You want to assign each of the extraStudents students to a class in a way that maximizes the average pass ratio across all the classes.

The pass ratio of a class is equal to the number of students of the class that will pass the exam divided by the total number of students of the class. The average pass ratio is the sum of pass ratios of all the classes divided by the number of the classes.

Return the maximum possible average pass ratio after assigning the extraStudents students. Answers within 10-5 of the actual answer will be accepted.

 
##### Example 1:

    Input: classes = [[1,2],[3,5],[2,2]], extraStudents = 2
    Output: 0.78333
    Explanation: You can assign the two extra students to the first class. The average pass ratio will be equal to (3/4 + 3/5 + 2/2) / 3 = 0.78333.

##### Example 2:

    Input: classes = [[2,4],[3,9],[4,5],[2,10]], extraStudents = 4
    Output: 0.53485


 

##### Constraints:

- `1 <= classes.length <= 105 `
- `classes[i].length == 2     `
- `1 <= passi <= totali <= 105`
- `1 <= extraStudents <= 105  `


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