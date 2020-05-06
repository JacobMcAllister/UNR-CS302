#include <cstdlib>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <vector>
#include <string>

//Given Miles per gallon
//Nevada Gas Price - 2.324
//MAX used for making arrays
#define MPG 40
#define NPG 2.324
#define MAX 100

// Old Global
/*const char *cities[5] = {"Reno", 
                        "Salt Lake City", 
                        "Las Vegas", 
                        "San Fransico", 
                        "Seattle"};    
*/

// EdgeNode Class will connect node to node
// Stores key(ex: 0, 1, 2, 3)
// Stores Weight(mileage)
// Needs getters and setters
class EdgeNode{
    public:
        int key;
        int weight;
        std::string label;
        // For creating nodes, next being the next node in line.
        EdgeNode *next;
        // Constructors for EdgeNodes.
        EdgeNode(int);  // EdgeNode(key)
        EdgeNode(int, int);  // EdgeNode(key, weight)
        EdgeNode(int, int, std::string);  // EdgeNode(key, weight)
};
// Constucter with (key) parameter
EdgeNode::EdgeNode(int key){
    this->key = key;
    this->weight = 0;
    this->label = "N/A";
    this->next = NULL;    // Sets next null location to NULL
}
// Constructor with (key, weight) parameter
EdgeNode::EdgeNode(int key, int weight){
    this->key = key;
    this->weight = weight;
    this->label = "N/A";
    this->next = NULL;    // Sets next location to NULL
}

// Constructor with (key, weight) parameter
EdgeNode::EdgeNode(int key, int weight, std::string L){
    this->key = key;
    this->weight = weight;
    this->label = L;
    this->next = NULL;    // Sets next location to NULL
}

// Node Class, will be used to for Adjancey List
// Not yet used in implementation, will address.
// Needs Work, should track a head and whatever.
class Node{
public:
    int key;
    
    Node *next;
    Node(int);
};
// Node Constructor with (key) parameter
Node::Node(int key){
    this->key = key;
    this->next = NULL;
}

// WeightedGraph class (Graph with edges that contain values)
// Needs work with addressing some hard coding.
class WeightedGraph{
private: 
    // Holds the amount of vertexes for later calculations
    int vertexes;

public:
    // For identifying undirected and directered edges.
    bool directed;
    // Array that contains edges
    EdgeNode *edges[MAX];
    // Constructor with (directed/undirected) parameter.
    WeightedGraph(bool);
    
    void setVertexes(int V);
    // Solves Traveling SalesMan Problem (min array path, start node key) parameters
    int tsp(int **adjMatrix, int tspPath[], int start);
    // Prints all edges, ex: Reno to Seattle distance - 704 miles
    void printConnections();
    // Prints Adjancey Matrix of Edge Weights
    void printMatrix(int **adjMatrix);
    // Takes all edges and forms a matrix of weights.
    // Uses dijkstraPath to solve A to C = (A + B) + (B + C) connections.
    void matrix(int **adjMatrix);
    // Adds edges to edge array ( start, end, edge weight, undirected/directed )
    void addEdge(int x, int y, int weight, bool directed);
    void addEdge(int x, int y, int weight, std::string X, std::string Y, bool directed);
    // Finds shortest path between two points.
    int dijkstraPath(int end, int start);
};
// Constructor ( undirected/directed ) parameter
WeightedGraph::WeightedGraph(bool directed){
    this->directed = directed;
    this->vertexes = 0;
    // Defaults all edges to NULL
    for(int i = 0; i < MAX; i++){
        this->edges[i] = NULL;
    }
}

void WeightedGraph::setVertexes(int V){
    this->vertexes = V;
}
// addEdge will add a weighted edge to edges[] with ( start, end, edge weight, undirected/directed ) parameter
void WeightedGraph::addEdge(int x, int y, int weight, bool directed){
    // This will increment vertexes as the edges are added.
    if(edges[x]==NULL)
        vertexes++;
    // This will bounds check between 0  to vertexes. 
    if(x+1 > 0 && x < vertexes + 1 && y+1 > 0 && y <vertexes + 1){
        // This will create and set weight to y
        EdgeNode *edge = new EdgeNode(y, weight);
        // This will set created edge->next to x's node location in edges[] 
        edge->next = this->edges[x];
        // This will set edge[x] to edgeNode of y 
        this->edges[x] = edge;
        // This will effective effectively create a linked list of nodes
          
            // if its undirected, add to list of destination (y)
            if(!directed){
                addEdge(y, x, weight, true);
            }
    }
}

// addEdge will add a weighted edge to edges[] with ( start, end, edge weight, undirected/directed ) parameter
void WeightedGraph::addEdge(int x, int y, int weight,std::string X, std::string Y, bool directed){
    // This will increment vertexes as the edges are added.
    if(edges[x]==NULL)
        vertexes++;
    // This will bounds check between 0  to vertexes. 
    if(x+1 > 0 && x < vertexes + 1 && y+1 > 0 && y <vertexes + 1){
        // This will create and set weight to y
        EdgeNode *edge = new EdgeNode(y, weight, X);
        // This will set created edge->next to x's node location in edges[] 
        edge->next = this->edges[x];
        // This will set edge[x] to edgeNode of y 
        this->edges[x] = edge;
        // This will effective effectively create a linked list of nodes
          
            // if its undirected, add to list of destination (y)
            if(!directed){
                addEdge(y, x, weight, Y, X, true);
            }
    }
}

// This function solve the Traveling salemans probrlem.
// Uses an adaption of hamiltons circuit algorithm
// (correct path array, starting point) parameters
// returns int
int WeightedGraph::tsp(int **adjMatrix, int tspPath[], int start){
    // Vector used to hold indexs of edges
    std::vector<int> vertex;
    for(int i = 0; i < vertexes; i++){
        if (i != start)
            vertex.push_back(i);
    }
        // Array to copy correct current path into
        // Min cost path container
        int tempArr[vertexes];
        int minPath = INT_MAX; 
         
        // For this project, output of all paths and mileage was required.
        // This information will be sent to a text file "FinalProjectText"
        // Reason:  To avoid clutting of the screen
        std::ofstream out;
        out.open("FinalProjectText.txt");
        out<< "OUTPUT OF ALL PATHS AND MILES:\nnote that in scenarios A to C = (A to B) + (B to C), B is not printed as a destination\n\n";
        
        // Do While Loop computing all paths
        do{
            // Tracks current of path
            // temp for purpose of traverse of matrix
            int currPathWeight = 0;
            int temp = start;
          
            out<< this->edges[temp]->label<< " -> ";
            // This loop will traverse the matrix adding current path weight
            for (int i = 0; i <vertex.size()+1; i++){
                currPathWeight += adjMatrix[temp][vertex[i]];
                temp = vertex[i];
                tempArr[i] = temp;           
            }
            // Add path weight back to origin
            currPathWeight += adjMatrix[temp][start];
            
            // This loop is for outputting current path to text file.
            // vertex - 1 due to start has been removed from path
            for(int i = 0;i < vertexes-1;i++){
                if( i == vertexes-2)
                    out<< this->edges[tempArr[i]]->label<< " -> ";
                else
                    out<< this->edges[tempArr[i]]->label << " -> ";
            }
            // Output final stop and current path weight
            out<< this->edges[temp]->label;
            out<<  "\tMiles - "<< currPathWeight <<std::endl;
            
            // If current path is new minimal cost path, replace tspPath
            minPath = std::min(minPath, currPathWeight);
            if(minPath == currPathWeight){
                for(int i = 0; i < vertexes-1;i++){
                    tspPath[i] = tempArr[i];
                }
            }
        // next_permutation will circle til all computations are made.
        }while(next_permutation(vertex.begin(), vertex.end()));        
   out.close();
// return minimal cost as int
return minPath;
}

// Prints the matrix to terminal created via matrix()
void WeightedGraph::printMatrix(int **adjMatrix){
    std::cout << std::setw(18)<<" "<<"City x City Matrix by miles\n\n";
    std::cout << std::setw(20) << " " << std::setw(5) << "Reno"<< std::setw(5) << "SLC"
              << std::setw(7) << "LV" << std::setw(7) << "SF" << std::setw(10) << "Seattle" << std::endl;
    std::cout << "    " << std::setfill('-')<< std::setw(52)<<" \n"<<std::setfill(' ');
    for(int i = 0;i < vertexes; i++){
        std::cout << std::setw(16) << this->edges[i]->label;
        for(int j =0; j < vertexes;j++){
            std::cout<<" - "<< std::setw(4) << adjMatrix[j][i];
        }
        std::cout<< "\n";
    }
}

// Creates a matrix stored in adjMatrix
void WeightedGraph::matrix(int **adjMatrix){
    // Counters
    int tempNum, tracker = 0;
    // Connection is to track missing direct connections.
    // Note that the number of edges = (n*(n-1))/2 where n = # of vertexes
    // If we give each vertex a value 0-4, where 4 is does not connect to X
    // Then (# of edges) - 4 - 3 - 2 - 0 = x = 1
    // We then can use this with dijkstraPath to get a weight for (4,X)
    int connection = (vertexes*(vertexes-1))/2;
    
    // Converts list into matrix.
    for(int i = 0; i < vertexes; i++){
        // If end not end of edges array compute
        if(this->edges[i] != NULL){
                // First, set node to it's as 0, ex: reno to reno = 0
                adjMatrix[tracker][i] = 0;
                // connection - current position
                connection -= i;
                
                // Loop till reach end of list. 
                EdgeNode *curr = this->edges[i];
                while(curr != NULL){
                    connection -= curr->key;
                    adjMatrix[curr->key][i] = curr->weight;
                    curr = curr->next;
                }
                // If a missed connect is found then, compute it.
                if(connection != 0){
                tempNum = dijkstraPath(connection, i);
                adjMatrix[connection][i] = tempNum;
                }
        }
    // Reset connection
    connection = (vertexes*(vertexes-1))/2;
    tracker++;
    }    
}
// Prints all connections to terminal
void WeightedGraph::printConnections(){

    for(int i = 0; i < vertexes; i++){
        if(this->edges[i] != NULL){
            std::cout << std::setw(10) << this->edges[i]->label << " connects to: \n";
            EdgeNode *curr = this->edges[i];
            while(curr != NULL){
                std::cout <<std::setw(14) << this->edges[curr->key]->label << " distance - " << curr->weight << " miles"<< std::endl;
                curr = curr->next;
            }
            std::cout<<std::endl;
        }
    }
}

// Uses Dijkstra's algorithm to find shortest path.
int WeightedGraph::dijkstraPath(int end, int start){
    // Containers used for tracking.
    bool mark[MAX];
    int distance[MAX];
    int parent[MAX];
    EdgeNode *curr;
    int currVertexes, linkVertexes, weight, minPath;
        // Set all containers to defaults.
        for(int i = 0; i < (MAX); i ++){
            mark[i] = false;
            distance[i] = INT_MAX;
            parent[i] = -1;
        }
    // Set to start position
    distance[start] = 0;
    currVertexes = start;
    
    // Alogrithm that computes shortest path.
    while(mark[currVertexes] == false){

        mark[currVertexes] = true;
        curr = this->edges[currVertexes];

        while(curr != NULL){

            linkVertexes = curr->key;
            weight = curr->weight;

            if((distance[currVertexes] + weight) < distance[linkVertexes]){
                distance[linkVertexes] = distance[currVertexes] + weight;
                parent[linkVertexes] = currVertexes;
            }
            curr = curr->next;
        }

        //Changes current vertex to vertex with shortest distance.
        minPath = INT_MAX;
        for(int i = 0; i < (MAX); i ++){
            if(!mark[i] && (distance[i] < minPath)){
                currVertexes = i;
                minPath = distance[i];
            }
        }
    }

// returns shortest path distance to end.
return distance[end];
}

// Prints out solution to traveling salesman problem to terminal and outfile
void tspPrint(int tspPath[], int miles, int vertexes, WeightedGraph* work){

    std::cout << "\n\nMinimial cost path: " << work->edges[0]->label <<" -> ";
    for(int i =0; i < vertexes-1; i++){
        if(i == vertexes-2)
            std::cout << work->edges[tspPath[i]]->label << " -> " << work->edges[0]->label;
        else{
            std::cout << work->edges[tspPath[i]]->label << " -> ";
        }

    }
    std::cout << "\nMiles: " << miles;
    std::cout << "\nGallons of gas (40 mpg): " << miles/MPG;
    std::cout << "\nCost using Nevada average per gallon (2.324): " << (miles/MPG)*NPG<<std::endl;
  
    // This will open previously created file and appean to it.
    std::ofstream out;
    out.open ("FinalProjectText.txt", std::ios::out | std::ios::app);
        
        out << "\n\nMinimial cost path: " << work->edges[0]->label <<" -> ";
        for(int i =0; i < vertexes-1; i++){
            if(i == vertexes-2)
                out << work->edges[tspPath[i]]->label << " -> " << work->edges[0]->label;
            else{
                out << work->edges[tspPath[i]]->label << " -> ";
            }

        }
        out << "\nMiles: " << miles;
        out << "\nGallons of gas (40 mpg): " << miles/MPG;
        out << "\nCost using Nevada average per gallon (" << NPG << "): " << (miles/MPG)*NPG<<std::endl;
        
    out.close();

        std::cout << work->edges[0]->label;
}


int main(){
// Initilized a weighted graph as undirected.
WeightedGraph *work = new WeightedGraph(false);

// Setting all edges with mileage I googled, false = undirected

// Load labels, might move this to Node/NodeEdge class.
// 0 - Reno, 1 - Salt Lake City, 2 - Las Vegas, 3 - San Fran, 4 - Seattle

// May change, add into edge node a incrementer to keep count of vertexes
// int vertexes =5;

// work->setVertexes(vertexes);

work->addEdge(0, 1, 518, "Reno", "Salt Lake City", false);
work->addEdge(0, 2, 444, "Reno", "Las Vegas", false);
work->addEdge(0, 3, 218, "Reno", "San Fransico", false);
work->addEdge(0, 4, 704, "Reno", "Seattle", false);
work->addEdge(1, 4, 841, "Salt Lake City", "Seattle", false);
work->addEdge(2, 3, 571, "Las Vegas", "San Franisco", false);
work->addEdge(2, 1, 420, "Las Vegas", "Salt Lake City", false);
work->addEdge(3, 4, 808, "San Francisco", "Seattle", false);

// Create a 2D array for purpose of solving TSP problem.
int vertexes = 5;
int **adjMatrix;
adjMatrix = new int *[vertexes];
for(int i = 0; i<vertexes; i++)
    adjMatrix[i] = new int[vertexes];

// Do some work.
work->printConnections();
work->matrix(adjMatrix);
work->printMatrix(adjMatrix);

// 0 - Reno
// vertexes = 5
int tspPath[vertexes];
int tspMiles = work->tsp(adjMatrix, tspPath, 0);
tspPrint(tspPath, tspMiles, vertexes, work);
}