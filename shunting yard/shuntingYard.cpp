#include <iostream>
#include <cstring>

struct Node{
  char* value;
  Node* next;
  Node(char* newVal){
    value = newVal;
    next = NULL;
  }
};

void enqueue(char* num, Node* &head);
void dequeue(Node* &head);
void push(char* operation, Node* &head);
Node* pop(Node* &sHead);
int findPrecedence(char sign);
void printIt(Node* aNode);

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
    cout << *currChar << endl;

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
      while(strcmp(current->value, "(") !=0){
	enqueue(pop(stackHead)->value, queueHead);
	cout << "looped" << endl;
      }
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
  return 0;
}
void printIt(Node* aNode){
  Node* current = aNode;
  while(current != NULL){
    cout << *current->value << " ";
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
  cout <<*num<< endl;
  cout << "yks" << endl;
  if(head == NULL){
    Node* newNode = new Node(num);
    cout <<"head is empty" << endl;
    head = newNode;
    cout << "works" << endl;
    return;
  }else{
    cout << *head->value << endl;
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
  Node* newNode = new Node(operation);
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

