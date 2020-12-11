#include <iostream>
#include <cstring>
#include <vector>

//node struct for the shunting yard stack and queue
struct Node{
  char* value;
  Node* next;
  Node(char* newVal){
    value = newVal;
    next = NULL;
  }
};

//node struct for the expression tree
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

//shunting yard functions
void enqueue(char* num, Node* &head);
void dequeue(Node* &head);
void push(char* operation, Node* &head);
Node* pop(Node* &sHead);
char* peek(Node* head);
int findPrecedence(char sign);
void printIt(Node* aNode);

//expression tree functions
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

  //initialization
  Node* queueHead = NULL;
  Node* stackHead = NULL;
  cout << "enter an equation with spaces between everything" << endl;
  cout << "please type an equals sign at the end so the program runs properly" << endl;
  cout << "example: 3 + 4 =" << endl;
  char* currChar = new char[80];

  //for shunting yard
  while(cin >> currChar){
    
    if(strcmp(currChar, "=")==0){
      break;
    }
    
    if(isdigit(currChar[0])){//checking if the current char is an int
      char* cc = strdup(currChar);

      //enqueue the numbers
      enqueue(cc, queueHead);
      
    }else if(strcmp(currChar, "(")== 0){//checking if it's an open parenthesis
      char* pa = strdup(currChar);

      //just push it in the stack
      push(pa, stackHead);
      
    }else if(strcmp(currChar, ")") == 0){//checking if it's a closing parenthesis
      Node* current = stackHead;
      while(strcmp(peek(stackHead), "(") !=0){//until you reach the opening parenthesis, pop from stack and enqueue
	enqueue(pop(stackHead)->value, queueHead);
	
      }
      //pop the opening parenthesis out
      pop(stackHead);
      
    }else if(!isdigit(currChar[0])){//if it's an operator
      while((stackHead !=NULL) &&//if there is something in the stack and
	    ((findPrecedence(stackHead->value[0]) > findPrecedence(currChar[0])) ||//if it takes precedence over the top of the stack or
	     ((findPrecedence(stackHead->value[0]) == findPrecedence(currChar[0])) && currChar[0] != '^')) &&//if it has equal precedence and it's not power
	    (stackHead->value[0] != '(')){//and it's not an opening parenthesis

	//pop from the stack and enqueue in the queue
	enqueue(pop(stackHead)->value, queueHead);
      }
      //push the char in
      char* op = strdup(currChar);
      push(op, stackHead);
	   
    }
  }
  //get all the rest of the operators in the queue
  while(stackHead != NULL){
    enqueue(pop(stackHead)->value, queueHead);
  }

  //create the expression tree
  TreeNode* treeRoot = NULL;
  postfixToTree(queueHead, treeRoot);

  //ask for which fix it would like to convert to
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

//converts from queue to tree
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

//tree to infix
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

//tree to prefix
void postToPre(TreeNode* root){
  if(root != NULL){
    cout << root->value << " ";
    postToPre(root->left);
    postToPre(root->right);
  }
}

//tree to postfix
void postToPost(TreeNode* root){
  if(root!= NULL){
    postToPost(root->left);
    postToPost(root->right);
    cout << root->value << " ";
    
  }
}

//print function for debugging
void printIt(Node* aNode){
  Node* current = aNode;
  while(current != NULL){
    cout << current->value << " ";
    current = current->next;
  }
  cout << endl;

}

//finds precedence based off of pemdas
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

//enqueues
void enqueue(char* num, Node* &head){
  if(head == NULL){
    Node* newNode = new Node(strdup(num));
    head = newNode;
    return;
  }else{
    enqueue(num, head->next);
    
  }
}

//dequeues
void dequeue(Node* &head){
  Node* temp = head->next;
  delete(head);
  head = temp;
}

//push function into a stack
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

//pop function for a stack
Node* pop(Node* &sHead){
  Node* newNext = sHead->next;
  Node* tbMoved = sHead;
  tbMoved->next= NULL;
  sHead = newNext;
  return tbMoved;
}


//peek function
char* peek(Node* head){
  return head->value;
}
