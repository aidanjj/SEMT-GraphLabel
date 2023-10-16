# SEMT-GraphLabel

## Basic usage:
To use this class, you create a 2-D dynamic array where the ith row and jth column corresponds to the edge that connects the ith vertex to the jth vertex.<br>
Here is an example usage for the graph <i>C<sub>5</sub></i>.
```AGS Script
#include "GraphLabel.hpp"

int main(){
    // The graph initialized using a static array so that you can visually see the array.
    // Notice that there are 5 ones corresponding to the 5 edges.
    int staticGraph[5][5] =
    {
        {0,1,0,0,0},
        {0,0,1,0,0},
        {0,0,0,1,0},
        {0,0,0,0,1},
        {1,0,0,0,0}
    };
    
    // Initializing the 2-D dynamic array.
    int** dynamicGraph = new int*[5];
    for (int i = 0; i < 5; ++i){
        dynamicGraph[i] = new int[5];
    }
    
    // Copying the dynamic array from the static array.
    for (int i = 0; i < 5; ++i){
        for (int j = 0; j < 5; ++j){
            dynamicGraph[i][j] = staticGraph[i][j];
        }
    }
    
    GraphLabel g(5); // GraphLabel initialized with the number of vertices.
    g.setGraph(dynamicGraph);
    g.start();
}
```
Output:
```
{1,2,3,4,5}, C: 14
{2,1,3,4,5}, C: 14
{3,1,2,4,5}, C: 14
{1,3,2,4,5}, C: 14
{2,3,1,4,5}, C: 14
      ...
```
Note that C denotes the magic constant.
## Fixing labels to specific vertices.
It is possible to fix labels to specific vertices using `GraphLabel::fix(int index, int value)`.<br>
Here is an example of fixing vertices 1 and 2 to the values 3 and 2, respectively.<br>
```AGS Script
int main(){
    int** graph;
    create4Cycle(graph); // Designated graph initialization to another function.
    
    GraphLabel g(5);
    g.setGraph(graph);
    g.fix(0,3); // Fixed vertex 1 to a label of 3.
    g.fix(1,2); // Fixed vertex 2 to a label of 2.
    g.start();
}
```
Output:
```
{3,2,4,1,5}, C: 13
{3,2,5,1,4}, C: 13
```
## Random permutations
For graphs with large amounts of vertices, it may be better to use random permutations, as the solutions tend to be bunched together when using deterministic permutations.<br>
To do this, use `GraphLabel::setRandom(bool)`. Here is an example:
```AGS Script
int main(){
    int** graph;
    create4Cycle(graph);
    
    GraphLabel g(5);
    g.setGraph(graph);
    g.setRandom(true);
    g.start();
}
```
Note that this enters an infinite loop in this current implementation.

## Creating your own magic constant function
While the GraphLabel class can automatically determine the magic constant, this implementation requires iterating through an array with a size equal to the number of vertices.<br>
By creating your own magic constant function, you eliminate the need for iterating through an array, thus speeding up the program.<br>
To do this, use `GraphLabel::setConstant(double (*inConstant)(int*))`, where inConstant is your function that takes in a dynamic array of vertices.
Here is an example for <i>B<sub>5</sub></i>, where C = <sup>1</sup>&frasl;<sub>16</sub>(4(v<sub>1</sub> + v<sub>2</sub>)+484):
```AGS Script
double constantFunction(int* v){
    return (1.0/16)*(4*(v[0]+v[1])+484); // The way the graph was defined, v[0] and v[1] correspond to v_1 and v_2.
}

int main(){
    int** graph;
    createBook5(graph);
    GraphLabel g(5);
    g.setGraph(graph);
    g.setConstant(constantFunction);
    g.start();
}
```

