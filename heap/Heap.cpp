#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

// function prototypes
void heapify(int* heap, int size, int node);
void heapSort(int* heap, int size);
void deleteFunc(int* heap, int toDelete);
void printFunc(int* heap, int location, int space);

int main(){
  //initialization
  int size = 0;
  int* heap = new int[100];
  for(int i = 0;i<100;i++){
    heap[i] = 0;
  }
  char input;
  cout << "how would you like to input your numbers?" <<endl;
  cout << "type 'F' for file input or 'U' for user input" << endl;
  cin >> input;
  
  if(input == 'F'){//getting file input
    cout << "what is your file name?" << endl;
    char fileName[80];
    cin >> fileName;
    ifstream numFile;
    numFile.open(fileName);
    int count = 0;
    while(numFile >> heap[count]){
      cout << heap[count] << endl;
      size++;
      count++;
    }
    numFile.close();
    heapSort(heap, 100);
  }else if(input == 'U'){//getting user input
    int inputNo = 0;
    for(int i = 0;i<100;i++){
      cout << "enter a number between 1 and 1000 into the heap or -1 to quit";
      cout << endl;
      cin >> inputNo;
      if(inputNo == -1){
	break;
      }else{
	heap[i] = inputNo;
	size++;
      }
    }
    heapSort(heap, 100);
  }else{
    cout << "that wasn't a valid format" << endl;
  }
  
  for(int i = 0;i<100;i++){
    cout << heap[i] << " ";
  }

  //asking for a command
  char command[80] = "cont";
  while(strcmp(command, "QUIT") != 0){
    cout << "what would you like to do: (ADD, DELETE, PRINT)" << endl;
    cin >> command;
    if(strcmp(command, "ADD") == 0){
      if(size >= 100){
	cout << "you already have the maximum numbers allowed" << endl;
      }else{
	cin >> heap[size];
	size++;
	heapSort(heap, 100);
      }
    }else if(strcmp(command, "DELETE") == 0){
      int deleteNum = 0;
      cin >> deleteNum;
      deleteFunc(heap, deleteNum);
    }else if(strcmp(command, "PRINT") == 0){
      
      printFunc(heap,0, 0); 
    }else if(strcmp(command, "QUIT") == 0){

    }else{
      cout << "not a valide command"<< endl;
    }
  }
}

//the heapify and heapSort functions were made with psuedo code provided by http://www.crazyforcode.com/heap-data-structure/://www.crazyforcode.com/heap-data-structure/~
//swapping nodes if it's larger than parent
void heapify(int* heap, int size, int node){
  int largest = node;
  int l = 2*node+1;
  int r = 2*node+2;

  if(l < size && heap[l] > heap[largest]){
    largest = l;
  }
  if(r < size && heap[r] > heap[largest]){
    largest = r;
  }
  if(largest != node){
    swap(heap[node], heap[largest]);
    heapify(heap, size, largest);
  }
}


//checking that every node is not smaller than it's children 
void heapSort(int* heap, int size){
  for(int i = size/2 - 1;i>=0;i--){
    heapify(heap, size, i);
  }
  for(int i = size-1;i>0;i--){
    //swap(heap[0], heap[i]);
    heapify(heap, i, 0);
  }
}

//deleting a number
void deleteFunc(int* heap, int toDelete){
  for(int i = 0;i<100;i++){
    if(heap[i] == toDelete){
      heap[i] =0;
      break;
    }
  }
  heapSort(heap, 100);
}

//printing it out in a tree shape
 void printFunc(int* heap, int location, int space){
  if(heap[location] == 0){
    return;
  }
  space += 10;

  printFunc(heap, location*2+1, space);

  cout << "\n" <<endl;
  for(int i = 10; i < space;i++){
    cout << " ";
  }
  cout <<heap[location] << "\n";
  printFunc(heap, location*2+2, space);
}
