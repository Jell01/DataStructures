#include <iostream>
#include <cstring>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include "Student.h"
#include "Node.h"

using namespace std;

//function prototypes
void addFunc(Node** &studentList, int size);
void delFunc(Node* head, Node* &current, int id);
void printFunc(Node** &studentList, int size);
void rehashFunc(Node** &studentList, int &size);
int hashFunc(int studentID, int size);
void debugPrint(Node** studentList, int size);

int main(){
  //initialization
  Node** students = new Node*[101];
  int size = 101;
  char command[10];
  srand(time(NULL));
  ifstream firstNameFile("FirstNames.txt");
  ifstream lastNameFile("LastNames.txt");
  for(int i = 0;i<101;i++){
    students[i] = NULL;
  }
  int randStudentID = 1;

  //looking for a command
  while(strcmp(command, "QUIT") != 0){
    cout << "Type in a command (ADD, PRINT, DELETE, QUIT)" << endl;
    cin >> command;
    if(strcmp(command, "ADD") == 0){//if they would like to add a student
      cout << "How would you like to add your student? (RANDOM, INPUT)" << endl;
      char addCom[10];
      cin >> addCom;
      if(strcmp(addCom, "RANDOM") == 0){//if they want to add a random student
	cout << "how many students would you like to add?"<< endl;
	int numStudents;
	cin >> numStudents;
	for(int i = 0; i<numStudents;i++){//adding random students
	  int firstNameNo = rand()%100 +1;
	  int lastNameNo = rand()%100 +1;
	  Student* newStudent = new Student();
	  char firstName[80];
	  for(int j = 0; j<100;j++){
	    if(firstNameNo == j+1){
	      firstNameFile >> firstName;
	      newStudent->setFirstName(firstName);
	    }else{
	      firstNameFile >> firstName;
	    }
	    if(lastNameNo == j+1){
	      lastNameFile >> firstName;
	      newStudent->setLastName(firstName);
	    }else{
	      lastNameFile >> firstName;
	    }
	    
	  }
	  newStudent->setGPA(rand()%5);
	  newStudent->setID(randStudentID);
	  int location = hashFunc(randStudentID, size);
	  Node* newNode = new Node(newStudent);
	  if(students[location] == NULL){//adding the students into the hashtable
	    students[location] = newNode;
	  }else{
	    Node* temp = students[location]->getNext();
	    newNode->setNext(temp);
	    students[location]->setNext(newNode);
	  }

	  randStudentID++;
	  firstNameFile.close();
	  firstNameFile.open("FirstNames.txt");
	  lastNameFile.close();
	  lastNameFile.open("LastNames.txt");
	  
	}
      }else if(strcmp(addCom, "INPUT") == 0){//if you want to input a student
	addFunc(students, size);
      }else{
	cout << "that was not a valid choice, try again later" << endl;
      }
      //rehashing
      for(int i = 0; i<size;i++){
	int chainLen = 1;
	if(students[i] != NULL){
	  Node* current = students[i];
	  while(current->getNext() != NULL){
	    chainLen++;
	    current = current->getNext();
	  }
	}
	if( chainLen >=3){
	  rehashFunc(students, size);
	  break;
	}
      }

    }else if(strcmp(command, "PRINT") == 0){//if you want to print all your students
      printFunc(students, size);
    }else if(strcmp(command, "DELETE") == 0){//if you want to delete your students
      cout << "what is the id of the student you would like to delete?" << endl;
      int id = -1;
      cin >> id;
      int key = hashFunc(id, size);
      delFunc(students[key], students[key], key);
    }else if(strcmp(command, "QUIT") == 0){

    }else if(strcmp(command, "DEBUG") == 0){//debugging thing
      debugPrint(students, size);
    }else{
      cout << "that is not a valid command, make sure all letters are capital";
      cout << endl;
    }
  }
}

//hash function
int hashFunc(int studentID, int size){
  return studentID % size;
}

//adding a student
void addFunc(Node** &studentList, int size){
  Student* newStudent = new Student();

  //prompts for information for a student pointer
  cout << "what is the student's first name?" <<endl;
  cin >> newStudent->getFirstName();
  cout << "what is the student's last name?" <<endl;
  cin >> newStudent->getLastName();
  cout << "what is the student's student ID?" <<endl;
  cin >> *newStudent->getID();
  cout << "what is the student's GPA?" <<endl;
  cin >> *newStudent->getGPA();

  Node* newNode = new Node(newStudent);
  int location = hashFunc(*newStudent->getID(), size);
  if(studentList[location] == NULL){
    studentList[location] = newNode;
  }else{
    Node* temp = studentList[location]->getNext();
    newNode->setNext(temp);
    studentList[location]->setNext(newNode);
  }
  
}

//printing out students and their info
void printFunc(Node** &studentList, int size){
  for(int i = 0; i< size;i++){
    if(studentList[i] != NULL){
      Node* current = studentList[i];
      while(current != NULL){
	cout << current->getStudent()->getFirstName() << " ";
	cout << current->getStudent()->getLastName() << " ";
	cout << *current->getStudent()->getID() << " ";
	cout << *current->getStudent()->getGPA() << endl;
	current = current->getNext();
      }
    }
  }
}

//deleting a student
void delFunc(Node* head, Node* &current, int id){
  if(head == NULL){//if the list is empty
    cout<< "you have no students"<<endl;
  }else if(head == current && id == *(current->getStudent()->getID())){//if it is your first student
    cout << "deleting your first student" << endl;
    Node* newHead = current->getNext();
    current->~Node();
    current = newHead;
  }else if(current->getNext() == NULL){//if you're at the end of the list
    cout << "no matching IDs" << endl;
  }else if(*(current->getNext()->getStudent()->getID()) == id){//if the ids match
    cout << "deleting student: " << *(current->getNext()->getStudent()->getID()) << endl;
    Node* newNext = current->getNext()->getNext();
    current->getNext()->~Node();
    current->setNext(newNext);
  }else{//keep recursing
    Node* next = current->getNext();
    delFunc(head, next, id);
  }
}

//rehashing
void rehashFunc(Node** &studentList, int &size){
  //creating a new hashtable
  int oldSize = size;
  size=size*7;
  Node** temp = new Node*[size];
  for(int i = 0; i<size;i++){
    temp[i] = NULL;
  }

  //transferring the students over
  for(int i = 0;i<oldSize;i++){
    if(studentList[i] != NULL){
      Node* current = studentList[i];
      while(current != NULL){
        Node* tempnew = new Node(current -> getStudent());
        int newLoc = hashFunc(*current->getStudent()->getID(), size);
        if(temp[newLoc] == NULL){
          temp[newLoc] = tempnew;
        }else{
          Node* tempNode = temp[newLoc]->getNext();
          Node* newNode = tempnew;
          tempnew->setNext(tempNode);
          temp[newLoc]->setNext(tempnew);
        }
        current = current->getNext();
      }
    }
  }
  delete[] studentList;
  studentList = temp;

}

//debugging
void debugPrint(Node** studentList, int size){
  for(int i = 0;i<size;i++){
    if(studentList[i] != NULL){
      Node* current = studentList[i];
      while(current != NULL){
	cout << current->getStudent()->getFirstName()<< " ";
	current= current->getNext();
      }
      cout <<endl;
    }
  }
}
