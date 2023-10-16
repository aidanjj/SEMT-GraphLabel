//
//  GraphLabel.hpp
//  SEMT_Graphs
//
//  Created by Aidan Jerdee on 10/14/23.
//

#ifndef GraphLabel_hpp
#define GraphLabel_hpp


class GraphLabel{
private:
    bool isConstantSet = false; bool isGraphSet = false; bool isRandom = false;
    
    int vertexCount = 0;
    
    int edgeCount = 0;
    
    int maxCol = 0; int maxRow = 0; int minCol = 0; int minRow = 0;
    
    unsigned long int permCount = 0;
    
    unsigned long int permCountMillions = 0;
    
    int* vertexList;
    
    int* freeVertexList; int* fixedValues; int* fixedIndexes; int freeVertexCount = 0; bool isFixed = false;
    
    int* verticeWeights;
     
    int** graph;
    
    void findVerticeWeights();
    
    int uniform(int n);
    
    int loadingInterval = 1E8;
    
    double (*constant)(int*);
    
    double getConstant();
    
    void permuteVertices(int n);
    
    void printSolution();
    
    void swap(int i, int j);
    
    void copyFixedList();
    
public:
    GraphLabel(int inVertexCount);
    
    virtual ~GraphLabel() = default;
    
    bool isSemt();
    
    void setRandom(bool inRandom);
    
    void setConstant(double (*inConstant)(int*));
    
    void setGraph(int** inGraph);
    
    bool setLoadingInterval(const int& inLoadingInterval);
    
    void fix(int index, int value);
    
    void start();
};

#endif /* GraphLabel_hpp */
