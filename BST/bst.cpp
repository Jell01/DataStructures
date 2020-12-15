#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

//bst node struct
struct Node{
  int value;
  Node* right;
  Node* left;
  Node(int newVal){
    value = newVal;
    right = NULL;
    left = NULL;
  }
};

//function prototypes
int search(Node* root, int num);
void insert(Node* &head, Node* newLeaf);
void print(Node* root, int spaces);
void deleteNode(Node* &root, int num);
Node* getSmallest(Node* root);
int main(){

  //initialization
  char inputMethod[80];
  Node* head = NULL;
  int currNum = 0;
  
  cout << "How would you like to input: [FILE, USER]" << endl;
  cin >> inputMethod;

  if(strcmp(inputMethod, "FILE") == 0){//getting file input and constructing the tree
    char fileName[80];
    cout << "what is your file name?" << endl;
    cin >> fileName;
    ifstream numFile;
    numFile.open(fileName);
    while(numFile >> currNum){
      Node* newNode = new Node(currNum);
      insert(head, newNode);
    }
  }
  else if(strcmp(inputMethod, "USER") == 0){//getting user input
    cout << "enter your numbers, enter 0 to stop entering" << endl;
    cout << "e.g. 1 2 3 4 0" << endl;
    while(cin >> currNum){
      if(currNum == 0) break;
      Node* newNode = new Node(currNum);
      insert(head, newNode);
    }
  }

  while(strcmp(inputMethod, "QUIT") != 0){//looking for commands after initially adding
    cout << "type in a command: [QUIT, ADD, PRINT, DELETE, CHECK]" << endl;
    cin >>inputMethod;
    if(strcmp(inputMethod, "ADD") == 0){//adding a node
      cout <<"what number would you like to add?" << endl;
      cin >> currNum;
      Node* newNode = new Node(currNum);
      insert(head, newNode);
    }
    else if(strcmp(inputMethod, "PRINT") == 0){//printing the tree in a tree format
      print(head, 0);
    }
    else if(strcmp(inputMethod, "DELETE") == 0){//deleteing a node
      cout << "what number would you like to delete?" << endl;
      cin >> currNum;
      deleteNode(head, currNum);
    }
    else if(strcmp(inputMethod, "CHECK") == 0){//checking for a number
      cout << "what number would you like to check for?" << endl;
      cin >> currNum;
      if(search(head, currNum) == 0){
	cout << currNum << " is not in the list" << endl;
      }
    }
    else if(strcmp(inputMethod, "QUIT") == 0){//exiting
      cout << "quitting" << endl;
      break;
    }
    else{
      cout << "you typed an incorrect command, try again" << endl;
    }
  }
}

//add function, adds bigger numbers to the right, and smaller or equal to the left
//code from geeksforgeeks
void insert(Node* &root, Node* newLeaf){
  if(root == NULL){
    root = newLeaf;
    return;
  }
  
  if(newLeaf->value > root->value){
    insert(root->right, newLeaf);
  }
  else{
    insert(root->left, newLeaf);
  }
}

//searches for the node and returns 1 for found and 0 for not found
int search(Node* root, int num){
  if(root == NULL) return 0;
  if(root->value == num){
    cout <<num << " is in the tree" << endl;
    return 1;
  }
  else if(num > root->value){
    return search(root->right, num);
  }
  else if(num < root->value){
    return search(root->left, num);
  }
  return 0;
}

//prints out the tree in a tree format
//code from geeks for geeks
void print(Node* root, int spaces){
  if(root == NULL){
    return;
  }
  spaces += 5;

  print(root->right, spaces);
  cout << endl;
  for(int i = 5; i < spaces; i++){
    cout << " ";
  }
  cout << root->value << "\n";
  print(root->left, spaces);
}

//deletes a node
void deleteNode(Node* &root, int num){
  if(root == NULL) return;
  if(num < root->value){
    deleteNode(root->left, num);
  }
  else if(num > root->value){
    deleteNode(root->right, num);
  }
  else{
    if(root->left==NULL && root->right == NULL){
      root = NULL;
    }
    else if(root->right == NULL){
      root = root->left;
    }
    else if(root ->left == NULL){
      root = root->right;
    }
    else{
      
      //Node* temp = root->left;
      //root = root->right;
      //insert(root, temp);
      Node* smallest = getSmallest(root->right);
      root->value = smallest->value;
      deleteNode(root->right, smallest->value);
    }
  }
}

Node* getSmallest(Node* root){
  Node* current = root;
  while(current != NULL && current->left !=NULL){
    current = current->left;
  }
  return current;
}
