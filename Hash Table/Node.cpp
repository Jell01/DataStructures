#include <iostream>
#include <cstring>
#include "Student.h"
#include "Node.h"

using namespace std;

Node::Node(Student* newStudent) {//this is a constructor
  student = newStudent;
  next = NULL;
}

Node::~Node(){//this is a destructor
  delete &student;
  next = NULL;
}

Node* Node::getNext(){//this gets the next node in the LL
  return next;
}

Student* Node::getStudent(){//this gets the student of the node
  return student;
}

void Node::setNext(Node* nextNode){//this sets the next node of a node
  next = nextNode;
}
