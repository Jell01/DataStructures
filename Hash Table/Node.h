#include <iostream>
#include <cstring>
#include "Student.h"

//header guard
#ifndef NODE_H
#define NODE_H

using namespace std;

class Node{
 public:
  //function prototypes
  Node(Student*);
  ~Node();
  Node* getNext();
  Student* getStudent();
  void setNext(Node*);
 private:
  //variable initialization
  Student* student;
  Node* next;
  
};
#endif
