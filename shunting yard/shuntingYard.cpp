#include <iostream>
#include <cstring>
#include <vector>

struct Node{
  char* value;
  Node* next;
  Node(char* newVal){
    value = newVal;
    next = NULL;
  }
};

struct TreeNode{
  char* value;
  TreeNode* right;
  TreeNode* left;
  TreeNode(char* newVal){
    value = newVal;
    right = NULL;
    left = NULL;
  }
};

void enqueue(char* num, Node* &head);
void dequeue(Node* &head);
void push(char* operation, Node* &head);
Node* pop(Node* &sHead);
char* peek(Node* head);
int findPrecedence(char sign);
void printIt(Node* aNode);
void postfixToTree(Node* head, TreeNode* &root);
void postToIn(TreeNode* root);
void postToPre(TreeNode* root);
void postToPost(TreeNode* root);

using namespace std;

int main(){
  /*
  Node* stackHead = new Node();
  stackHead->value = NULL;
  stackHead->next = NULL;
  Node* queueHead = new Node();
  queueHead->value = NULL;
  queueHead->next = NULL;
  */
  Node* queueHead = NULL;
  Node* stackHead = NULL;
  cout << "enter an equation with spaces between everything" << endl;
  cout << "please type an equals sign at the end so the program runs properly" << endl;
  cout << "example: 3 + 4 =" << endl;
  char* currChar = new char[80];
  
  while(cin >> currChar){
    cout << "ran through ";
    cout << currChar << endl;

    if(strcmp(currChar, "=")==0){
      break;
    }
    //cout << *currChar << endl;
    if(isdigit(currChar[0])){
      char* cc = strdup(currChar);
      //cout << "working" << endl;
      enqueue(cc, queueHead);
      cout << "printing new queue ";
      printIt(queueHead);
    }else if(strcmp(currChar, "(")== 0){
      char* pa = strdup(currChar);
      cout << "came here" << endl;
      push(pa, stackHead);
    }else if(strcmp(currChar, ")") == 0){
      cout << "came here 2 " << endl;
      Node* current = stackHead;
      cout <<"printing stack ";
      printIt(stackHead);
      while(strcmp(peek(stackHead), "(") !=0){
	enqueue(pop(stackHead)->value, queueHead);
	printIt(queueHead);
	cout << "looped" << endl;
      }
      cout << "exited 2" << endl;
      cout << stackHead->value << endl;
      pop(stackHead);
    }else if(!isdigit(currChar[0])){
      cout << "came here 3 " << endl;
      while((stackHead !=NULL) &&
          ((findPrecedence(stackHead->value[0]) > findPrecedence(currChar[0])) ||
	   ((findPrecedence(stackHead->value[0]) == findPrecedence(currChar[0])) && currChar[0] != '^')) &&
	    (stackHead->value[0] != '(')){
	cout << "popping" << endl;
	enqueue(pop(stackHead)->value, queueHead);
      }
      char* op = strdup(currChar);
      cout << "Team" << endl;
      push(op, stackHead);
	   
    }
  }
  while(stackHead != NULL){
    enqueue(pop(stackHead)->value, queueHead);
  }
  cout << "made it" << endl;
  printIt(queueHead);
  printIt(stackHead);

  TreeNode* treeRoot = NULL;
  postfixToTree(queueHead, treeRoot);
  char* command = new char[20];
  cout << "would you like to print to [infix, postfix, prefix]" << endl;
  cin >> command;
  if(strcmp(command, "infix") == 0){
    postToIn(treeRoot);
  }else if(strcmp(command, "postfix") == 0){
    postToPost(treeRoot);
  }else if(strcmp(command, "prefix") == 0){
    postToPre(treeRoot);
  }else{
    cout << "you typed it wrong, make sure everything is in lowercase next time" << endl;
  }
  return 0;
}

void postfixToTree(Node* head, TreeNode* &root){
  vector<TreeNode*> treeStack;
  Node* current = head;
  while(current != NULL){
    if(isdigit(current->value[0])){
      treeStack.push_back(new TreeNode(current->value));
    }else{
      TreeNode* newTreeNode = new TreeNode(current->value);
      newTreeNode->right = treeStack.back();
      treeStack.pop_back();
      newTreeNode->left = treeStack.back();
      treeStack.pop_back();
      treeStack.push_back(newTreeNode);
    }
    current = current->next;
  }
  root = treeStack[0];  
      
}

void postToIn(TreeNode* root){
  if(root != NULL){
    if(!isdigit(root->value[0])){
      cout << "(";
    }
    postToIn(root->left);
    cout << root->value;
    postToIn(root->right);
    if(!isdigit(root->value[0])){
      cout << ")";
    }
  }

}

void postToPre(TreeNode* root){
  if(root != NULL){
    cout << root->value << " ";
    postToPre(root->left);
    postToPre(root->right);
  }
}

void postToPost(TreeNode* root){
  if(root!= NULL){
    postToPost(root->left);
    postToPost(root->right);
    cout << root->value << " ";
    
  }
}
void printIt(Node* aNode){
  Node* current = aNode;
  while(current != NULL){
    cout << current->value << " ";
    current = current->next;
  }
  cout << endl;

}
int findPrecedence(char sign){
  if(sign == '-'){
    return 1;
  }else if(sign == '+'){
    return 1;
  }else if(sign == '*'){
    return 2;
  }else if(sign == '/'){
    return 2;
  }else if(sign == '^'){
    return 3;
  }
  return 5;
}

void enqueue(char* num, Node* &head){
  cout <<num<< endl;
  cout << "yks" << endl;
  if(head == NULL){
    Node* newNode = new Node(strdup(num));
    cout <<"head is empty" << endl;
    head = newNode;
    cout << "works" << endl;
    return;
  }else{
    cout << head->value << endl;
    enqueue(num, head->next);
    
  }
  /*
  cout <<"seg gaulting at " << newNode->value << endl;
  while(current->next != NULL){
    cout << "passes" << endl;
    cout << "currVal "  << current->value<< endl;
    current = current->next;
    cout << "gone through" << endl;
  }
  current->next = newNode;
  cout << current->value;
  cout << " " << current->next->value;
  */
}

void dequeue(Node* &head){
  Node* temp = head->next;
  delete(head);
  head = temp;
}

void push(char* operation, Node* &head){
  Node* newNode = new Node(strdup(operation));
  if(head == NULL){
    head = newNode;
    
  }else{
    Node* temp = head;
    newNode->next = temp;
    head = newNode;
  }
}

Node* pop(Node* &sHead){
  cout << sHead->value << endl;
  Node* newNext = sHead->next;
  Node* tbMoved = sHead;
  tbMoved->next= NULL;
  sHead = newNext;
  return tbMoved;
}

char* peek(Node* head){
  return head->value;
}
