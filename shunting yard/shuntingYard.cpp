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
    cout << "ran through" << endl;
    cout << *currChar << endl;

    if(strcmp(currChar, "=")==0) break;
    cout << *currChar << endl;
    if(isdigit(currChar[0])){
      //cout << "working" << endl;
      enqueue(currChar, queueHead);
      cout << "printing new queue ";
      printIt(queueHead);
    }else if(strcmp(currChar, "(")== 0){
      push(currChar, stackHead);
    }else if(strcmp(currChar, ")") == 0){
      Node* current = stackHead;
      while(strcmp(current->value, ")") !=0){
	enqueue(pop(stackHead)->value, queueHead);
      }
      pop(stackHead);
    }else if(!isdigit(currChar[0])){
      while((stackHead->next !=NULL) &&
          ((findPrecedence(stackHead->next->value[0]) > findPrecedence(currChar[0])) ||
	   ((findPrecedence(stackHead->next->value[0]) == findPrecedence(currChar[0])) && currChar[0] != '^'))){
	enqueue(pop(stackHead)->value, queueHead);
      }
      push(currChar, stackHead);
	   
    }
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
  Node* newNode = new Node(num);
  cout <<*num<< endl;
  Node* current = head;
  cout << "yks" << endl;
  if(head == NULL){
    head = newNode;
    return;
  }
  cout <<"seg gaulting at " << newNode->value << endl;
  while(current->next != NULL){
    cout << "passes" << endl;
    //cout << "currVal "  << current->value<< endl;
    current = current->next;
    cout << "gone through" << endl;
  }
  current->next = newNode;
}

void dequeue(Node* &head){
  Node* temp = head->next;
  delete(head);
  head = temp;
}

void push(char* operation, Node* &head){
  Node* newNode = new Node(operation);
  newNode->value = operation;
  newNode->next = NULL;
  Node* temp = head->next;
  head->next = newNode;
  newNode->next = temp;
}

Node* pop(Node* &sHead){
  Node* newNext = sHead->next->next;
  Node* tbMoved = sHead->next;
  tbMoved->next= NULL;
  delete(sHead->next);
  sHead->next= newNext;
  return tbMoved;
}
