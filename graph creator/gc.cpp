#include <iostream>
#include <cstring>

using namespace std;

//node struct
struct node{
  char* name;
  node(char* newName) {
    name = newName;
  }
};

//function prototypes
void addNode(node** &nodeList);
void delNode(node** &nodeList, int adjM[21][21]);
void addEdge(node** nodeList, int adjM[21][21]);
void delEdge(node** nodeList, int adjM[21][21]);
void findShortest(node** nodeList, int adjM[21][21]);
bool checkVis(bool status[21]);
node** path(int p[21], int end, node** nodeList);

int main(){

  //creating a place to store nodes
  node** nodes = new node*[21];
  for(int i = 0; i < 21; i++) {
    nodes[i] = NULL;
  }

  //initializing adjacency matrix
  int adjM[21][21];
  for(int i = 0; i < 21; i++) {
    for(int j = 0; j < 21; j++) {
      adjM[i][j] = 0;
    }
  }

  //looking for commands until the user wants to quit
  char command[80] = "x";
  while (strcmp(command, "quit") != 0) {

    cout << "what would you like to do: [add, delete, find, quit]" << endl;
    cin >> command;

    if (strcmp(command, "add") == 0) {//asking whether the user wants to add a node or edge

      cout << "what would you like to add [node, edge]" << endl;
      cin >> command;
      if (strcmp(command, "node") == 0) {
	addNode(nodes);
      }
      else if (strcmp(command, "edge") == 0) {
	addEdge(nodes, adjM);
      }
      
    }
    else if (strcmp(command, "delete") == 0) {//asking whether the user wants to delete a node or edge

      cout << "what would you like to delete [node, edge]" << endl;
      cin >> command;
      if (strcmp(command, "node") == 0) {
	delNode(nodes, adjM);
      }
      else if (strcmp(command, "edge") == 0) {
	delEdge(nodes, adjM);
      }
      
    }
    else if (strcmp(command, "find") == 0) {//finding the shortest path between 2 nodes

      findShortest(nodes, adjM);

    }
    else if (strcmp(command, "printnodes") == 0) {//debug purposes

      for(int i = 0;i<21;i++){
	if(nodes[i] != NULL){
	  cout <<nodes[i]->name << " ";
	}
	else{
	  cout << "NULL ";

	}
      }
      cout << endl;

    }
    else if (strcmp(command, "printadjm") == 0) {//debug purposes

      for(int i = 0;i<21;i++){
	for(int j = 0;j<21;j++){
	  cout << adjM[i][j] << " ";
	}
	cout << endl;

      }
      
    }
    else if (strcmp(command, "quit") != 0) {//if the user mistyped

      cout << "try typing again in all lowercase" << endl;

    }
  }
}

void addNode(node** &nodeList) {//function t add a node

  //creates a new node and adds it to the first NULL in the nodelist possible
  cout << "what would you like to name this node?" << endl;
  char* name = new char[80];
  cin >> name;
  node* newNode = new node(name);
  for(int i = 0;i < 21;i++) {
    if(nodeList[i] == NULL) {
      nodeList[i] = newNode;
      return;
    }
  }
  cout <<"no more space" << endl;
  return;
}

void delNode(node** &nodeList, int adjM[21][21]) {//deletes node

  //deletes node and updates the adjacency matrix
  cout << "what would you like to delete" << endl;
  char name[80];
  cin >> name;
  int index = -1;
  for(int i = 0; i < 21; i++) {
    if((nodeList[i] != NULL) && (strcmp(nodeList[i]->name, name) == 0)) {
      cout << "entered" << endl;
      delete(nodeList[i]);
      nodeList[i] = NULL;
      index = i;
      break;
    }
  }
  for(int i = 0; i < 21; i++) {
    adjM[i][index] = 0;
    adjM[index][i] = 0;
  }
}

void addEdge(node** nodeList, int adjM[21][21]) {//adds a 1 way edge

  //finds the beginning and end of an edge
  char name1[80];
  char name2[80];
  int index1;
  int index2;
  int weight;
  //cout << adjM[0][4] << endl;
  cout << "what is the name of the first vertex of the edge" << endl;
  cin >> name1;
  //cout << adjM[0][4] << endl;
  cout << "what is the name of the destination vertex of the edge" << endl;
  cin >> name2;
  cout << adjM[0][4] << endl;
  for(int i = 0; i < 21; i++) {
    if((nodeList[i] != NULL) && (strcmp(nodeList[i]->name, name1) == 0)) {
      index1 = i;
    }
    else if((nodeList[i] != NULL) && (strcmp(nodeList[i]->name, name2) == 0)) {
      index2 = i;
    }
  }
  /*
  for(int i = 0;i<21;i++){
    for(int j = 0;j<21;j++){
      cout << adjM[i][j] << " ";
    }
    cout << endl;

  }
  */
  //cout << adjM[0][4] << endl;
  cout << "what is the weight of the edge" << endl;
  cin >> weight;
  adjM[index1][index2] = weight;
  //cout << adjM[0][4] <<" " << endl;
}

void delEdge(node** nodeList, int adjM[21][21]) {//deletes an edge
  //deletes and edge and updates the adjacency matrix
  char name1[80];
  char name2[80];
  int index1;
  int index2;

  cout << "what is the name of the first vertex of the edge" << endl;
  cin >> name1;
  cout << "what is the name of the destination vertex of the edge" << endl;
  cin >> name2;
  
  for(int i = 0; i < 21; i++) {
    if((nodeList[i] != NULL) && (strcmp(nodeList[i]->name, name1) == 0)) {
      index1 = i;
    }
    else if((nodeList[i] != NULL) && (strcmp(nodeList[i]->name, name2) == 0)) {
      index2 = i;
    }
  }

  adjM[index1][index2] = 0;
}

void findShortest(node** nodeList, int adjM[21][21]) {//uses dijkstra's algorithm to find the shortest path between 2 nodes

  //finding the two nodes that the user wants
  char name1[80];
  char name2[80];
  int index1;
  int index2;

  int distances[21];
  int previous[21];
  bool visited[21];
  cout << "what is the name of the first vertex of the edge" << endl;
  cin >> name1;
  cout << "what is the name of the destination vertex of the edge" << endl;
  cin >> name2;

  for(int i = 0; i < 21; i++) {
    if((nodeList[i] != NULL) && (strcmp(nodeList[i]->name, name1) == 0)) {
      index1 = i;
    }
    else if((nodeList[i] != NULL) && (strcmp(nodeList[i]->name, name2) == 0)) {
      index2 = i;
    }
  }

  //making all the distances infinity and makes all the nodes set to unvisited
  for(int i = 0; i< 21; i++){
    if(i == index1){
      distances[i] = 0;
      visited[i] = false;
      previous[i] = -1;
    }
    else if(nodeList[i] == NULL){//if its a null node you never want to go to it
      distances[i] = 1000000;
      visited[i] = true;
    }
    else{
      distances[i] = 1000000;
      visited[i] = false;
    }
  }

  //debugging purposes
  /*
  for(int i = 0; i < 21;i++){
    cout << distances[i] << " " << visited[i] << endl;
  }
  */

  //dijkstra's agorithm
  while(!checkVis(visited)){
    int current = 2147483647;
    int ind = 0;

    //find the minimum neighbor
    for(int i = 0; i < 21; i++){
      if((visited[i] == false) && (distances[i] < current)) {
	current = distances[i];
	ind = i;
      }
    }
    //changes its path value
    for(int i = 0; i < 21; i++){
      if(adjM[ind][i] != 0){
	if(current+adjM[ind][i] < distances[i]){
	  distances[i] = current + adjM[ind][i];
	  previous[i] = ind;
	}
      }
    }
    visited[ind] = true;
  }

  //prints the minimum distance
  cout << distances[index2] << endl;

  //finding and printing the shortest path
  node** shortestPath = path(previous, index2, nodeList);
  int lastIndex = 0;
  while((lastIndex+1 < 22) && (shortestPath[lastIndex+1] != NULL)){
    lastIndex ++;
  }
  for(int i = lastIndex; i >= 0; i--) {
    cout << shortestPath[i]->name;
  }
  cout << endl;
  
}

bool checkVis(bool status[21]){//checks if all nodes have been visited yet
  for(int i = 0;i< 21;i++) {
    if (!status[i]){
      return false;
    }
  }
  return true;
}

node** path(int p[21], int end, node** nodeList){//returns the backwards path travelled
  node** pathway = new node*[21];
  int current = end;
  int pInd = 0;
  while(current != -1){
    //cout << nodeList[current]->name << endl;
    pathway[pInd] = nodeList[current];
    current = p[current];
    pInd++;
    //cout << current << endl;
    //cout << nodeList[current]->name << endl;
  }
  return pathway;
    
    
}
