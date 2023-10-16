//
//  GraphLabel.cpp
//  SEMT_Graphs
//
//  Created by Aidan Jerdee on 10/14/23.
//

#include "GraphLabel.hpp"
#include <iostream>

GraphLabel::GraphLabel(int inVertexCount){
    vertexCount = inVertexCount;
    vertexList = new int[vertexCount];
    freeVertexList = vertexList;
    fixedValues = new int[vertexCount];
    fixedIndexes = new int[vertexCount];
    freeVertexCount = vertexCount;
    for (int i = 0; i < vertexCount; ++i){
        vertexList[i] = i + 1;
        freeVertexList[i] = i + 1;
    }
}

void GraphLabel::fix(int index, int value){
    freeVertexCount -= 1;
    if (isFixed){
        delete[] freeVertexList;
    }
    freeVertexList = new int[freeVertexCount];
    isFixed = true;
    fixedValues[vertexCount - freeVertexCount - 1] = value;
    fixedIndexes[vertexCount - freeVertexCount - 1] = index;
    int count = 0;
    for (int i = 0; i < vertexCount; ++i){
        bool isFixed = false;
        for (int j = 0; j < vertexCount - freeVertexCount; ++j){
            if (i + 1 == fixedValues[j]){
                isFixed = true;
            }
        }
        if (!isFixed){
            freeVertexList[count] = i + 1;
            count++;
        }
    }
}

void GraphLabel::findVerticeWeights(){
    verticeWeights = new int[vertexCount];
    for (int i = 0; i < vertexCount; ++i){
        verticeWeights[i] = 0;
    }
    for (int i = 0; i < vertexCount; ++i){
        for (int j = 0; j < vertexCount; ++j){
            if (graph[i][j] == 1){
                verticeWeights[i] += 1;
                verticeWeights[j] += 1;
                edgeCount++;
            }
        }
    }
}

double GraphLabel::getConstant(){
    if (isConstantSet){
        return constant(vertexList);
    }
    int verticeSum = 0;
    int edgeSum = 0.5*(vertexCount + edgeCount)*(vertexCount+edgeCount+1)-0.5*vertexCount*(vertexCount+1);
    for (int i = 0; i < vertexCount; ++i){
        verticeSum += verticeWeights[i] * vertexList[i];
    }
    return double(verticeSum + edgeSum)/double(edgeCount);
}

void GraphLabel::copyFixedList(){
    int count = 0;
    int freeCount = 0;
    while (count < vertexCount){
        bool isFixed = false;
        for (int i = 0; i < vertexCount - freeVertexCount; ++i){
            if (count == fixedIndexes[i]){
                vertexList[count] = fixedValues[i];
                isFixed = true;
            }
        }
        if (!isFixed){
            vertexList[count] = freeVertexList[freeCount];
            freeCount++;
        }
        count++;
    }
}

bool GraphLabel::isSemt(){
    int curVertexSum;
    int count = 0;
    int* sumList = new int[edgeCount];
    permCount++;
    if (permCount % loadingInterval == 0){
        permCountMillions += permCount / 1000000;
        std::cout << "Iteration: " << permCountMillions << " million" << std::endl;
        permCount = 0;
    }
    if (isFixed){
        copyFixedList();
    }
    double constant = getConstant();
    if (constant != int(constant)){
        delete[] sumList;
        return false;
    }
    for (int i = minRow; i < maxRow; ++i){
        for (int j = minCol; j < maxCol; ++j){
            if (graph[i][j] == 1){
            curVertexSum = vertexList[i] + vertexList[j];
                if (constant - curVertexSum > vertexCount + edgeCount || constant - curVertexSum < vertexCount + 1){
                    delete[] sumList;
                    return false;
                }
            for (int k = 0; k < count; ++k){
                if (curVertexSum == sumList[k]){
                    delete[] sumList;
                    return false;
                }
            }
            sumList[count] = curVertexSum;
            count++;
        }
        }
    }
    delete[] sumList;
    return true;
}

void GraphLabel::start(){
    if (isGraphSet){
        findVerticeWeights();
        permuteVertices(freeVertexCount);
    }
    else {
        std::cerr << "Error: graph is not set. Use GraphLabel::setGraph(int**) to set the graph." << std::endl;
    }
}

void GraphLabel::setConstant(double (*inConstant)(int*)){
    constant = inConstant;
    isConstantSet = true;
}

void GraphLabel::setGraph(int **inGraph){
    int minRow = vertexCount;
    int minCol = vertexCount;
    graph = new int*[vertexCount];
    for (int i = 0; i < vertexCount; ++i){
        graph[i] = new int[vertexCount];
    }
    for (int i = 0; i < vertexCount; ++i){
        for (int j = 0; j < vertexCount; ++j){
            graph[i][j] = inGraph[i][j];
            if (graph[i][j] == 1){
                if (i < minRow){
                    minRow = i;
                }
                if (j < minCol){
                    minCol = j;
                }
                maxRow = i + 1;
                maxCol = j + 1;
            }
        }
    }
    isGraphSet = true;
}

bool GraphLabel::setLoadingInterval(const int& inLoadingInterval){
    if (inLoadingInterval > 0){
        loadingInterval = inLoadingInterval;
        return true;
    }
    return false;
}

void GraphLabel::swap(int i, int j){
    int temp;
    temp = freeVertexList[j];
    freeVertexList[j] = freeVertexList[i];
    freeVertexList[i] = temp;
}
    
void GraphLabel::printSolution(){
    std::cout << "{";
    for (int i = 0; i < vertexCount; ++i){
        std::cout << vertexList[i];
        if (i != vertexCount - 1){
            std::cout << ",";
        }
    }
    std::cout << "}, C: " << getConstant() << std::endl;
}

void GraphLabel::setRandom(bool inRandom){
    isRandom = inRandom;
}

int GraphLabel::uniform(int n){
    return rand() % n;
}

void GraphLabel::permuteVertices(int n){
    if (isRandom == false){
        if (n == 1){
            if (isSemt()){
                printSolution();
            }
        }
        else {
            for (int i = 0; i < n; ++i){
                permuteVertices(n-1);
                if (n % 2 == 0){
                    swap(n-1,i);
                }
                else {
                    swap(0,n-1);
                }
            }
        }
    }
    else {
        while (true){
        for (int i = 0; i <= freeVertexCount-2; i++) {
            int j = i+uniform(n-i);
            swap(i, j);
        }
            if (isSemt()){
                printSolution();
            }
        }
    }
}

