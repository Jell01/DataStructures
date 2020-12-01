#include <iostream>
#include <cstring>
#include "Student.h"

using namespace std;

void Student::setFirstName(const char* newFirst){//sets the first name of a student
  strcpy(firstName, newFirst);
}

char* Student::getFirstName(){//returns the first name of a student
  return firstName;
}

void Student::setLastName(const char* newLast){//sets the last name of a student
  strcpy(lastName, newLast);
}

char* Student::getLastName(){//returns the last name of a student
  return lastName;
}

void Student::setID(int newID){//sets the id of a student
  id = newID;
}

int* Student::getID(){//returns the student ID
  return &id;
}

void Student::setGPA(float newGPA){
  gpa = newGPA;
}

float* Student::getGPA(){
  return &gpa;
}
