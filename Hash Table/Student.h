#include <iostream>
#include <cstring>

#ifndef STUDENT_H
#define STUDENT_H

using namespace std;

class Student{

 public:
  //function prototypes
  void setFirstName(const char* newFirst);
  char* getFirstName();
  void setLastName(const char* newLast);
  char* getLastName();
  void setID(int newID);
  int* getID();
  void setGPA(float newGPA);
  float* getGPA();
  
 private:
  //initialization
  char firstName[80];
  char lastName[80];
  int id;
  float gpa;
};
#endif
