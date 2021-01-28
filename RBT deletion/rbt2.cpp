#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

struct Node{

  Node* parent;
  Node* left;
  Node* right;
  bool color; //true for red, false for black
  int val;
  Node(int value){
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = true;
    val = value;
  }
};

//functions copied from wikipedia
Node* getParent(Node* n);
Node* getGrandParent(Node* n);
Node* getSibling(Node* n);
Node* getUncle(Node* n);
bool getColor(Node* n);
void normInsert(Node* head, Node* newNode);
void printTree(Node* root, int spaces);
Node* insert(Node* head, Node* newNode);
void findNode(Node* &head, int i, Node* &root);
void remNode(Node* &toDel, bool editTree, Node* &head);
//my own
void repairTree(Node* &newNode);
Node* getHead(Node* currNode);

//courtesy of nividh
int debug(Node* head, int count);

int main(){
  char command[10];
  Node* head = nullptr;
  ifstream numFile;
  while(true){
    cout << "what would you like to do: [add,read,print,delete]" << endl;
    cin >> command;
    if(strcmp(command, "add") == 0){//if the user wants to add
      int num = 0;
      cout << "what number would you like to add?" << endl;
      cin >> num;
      Node* newNode = new Node(num);
      head = insert(head, newNode);
    }
    else if(strcmp(command, "read") == 0){//reading numbers from a file
      char fName[80];
      cout << "what is the file name?" << endl;
      cin >> fName;
      numFile.open(fName);
      int currNum = 0;
      while(numFile>>currNum){
	Node* newNode = new Node(currNum);
	head = insert(head, newNode);
      }
      numFile.close();
    }
    else if(strcmp(command, "print") == 0){//displaying
      printTree(head, 0);
    }
    else if(strcmp(command, "debug") == 0){//debug purposes
      srand(0);
      for (int i = 0; i < 50; i++) {
	for(int j = 0; j < 50; j++) {
	  insert(head,new Node((i+j)%50));
	  insert(head, new Node(rand()));
	  if(debug(head, 0)) {
	    printTree(head, 0);
	    cout << "Wrong black node count" << endl;
	    return -1;
	  } 
        }
      head = nullptr;
      }
    }
    else if(strcmp(command, "delete") == 0){
      cout << "what number would you like to delete?" << endl;
      int toDel;
      cin>>toDel;
      findNode(head, toDel, head);
    }
    else{
      cout << "invalid command, make sure everything is lowercase" << endl;
    }
  }
}
Node* getParent(Node* n){//returns the parent if there is one
  return n == nullptr ? nullptr : n->parent;
}
Node* getGrandParent(Node* n){//returns the grandparent if there is one
  return getParent(getParent(n));
}
Node* getSibling(Node* n){//returns the sibling if there is one
  Node* p = getParent(n);
  if(p == nullptr){
    return nullptr;
  }
  if(n == p->left){
    return p->right;
  }
  else{
    return p->left;
  }
}
Node* getUncle(Node* n){//returns the uncle if there is one
  Node* p = getParent(n);

  return getSibling(p);
}
bool getColor(Node* n){//returns the color if there is a node
  if(n == NULL){
    return false;
  }
  return n->color;
}

void normInsert(Node* head, Node* newNode){//inserting it into the tree without fixing tree
  if (head != NULL)
  {
    if (newNode->val < head->val) {
      if (head->left != NULL) {
        normInsert(head->left, newNode);
        return;
      } else {
        head->left = newNode;
      }
    } else { // n->key >= root->key
      if (head->right != NULL) {
        normInsert(head->right, newNode);
        return;
      } else {
        head->right = newNode;
      }
    }
  }

  // Insert new Node n.
  newNode->parent = head;
  newNode->left = nullptr;
  newNode->right = nullptr;
  newNode->color = true;
}

Node* insert(Node* head, Node* newNode){
  normInsert(head, newNode);
  //cout << "inserted" << endl;
  repairTree(newNode);
  //cout << " repaired" << endl;
  //if(getParent(newNode) != NULL){
  //  cout <<"p" << getParent(newNode)->val << endl;
  //}
  //if(getUncle(newNode) != NULL){
  //  cout << "u" <<getUncle(newNode)->val << endl;
  //}
  //cout << "n" <<newNode->val << endl;
  head = getHead(newNode);
  return head;
}
Node* getHead(Node* currNode){//gets the head of the tree
  Node* current = currNode;
  while(getParent(current) != NULL){
    current = getParent(current);
  }
  return current;
}
void printTree(Node* root, int spaces){//prints the tree out in a shape
  if (root == NULL)
    return;
  spaces += 5;
  printTree(root->right, spaces);
  for (int i = 5; i < spaces; i++)
    cout<<" ";
  cout<<root->val;
  if(root->color == true){
    cout << "(R)" << endl;
  }else{
    cout << "(B)" << endl;
  }
  printTree(root->left, spaces);
}

void repairTree(Node* &newNode){//repairs the tree based on rules
  if(getParent(newNode) == NULL){//if the node is at the root, make it black
    //cout << "root" << endl;
    newNode->color = false;
  }
  else if(getParent(newNode)->color == false){//if the parent is black, dont do anything
    //cout << " idc" << endl;
    return;
  }
  else if(getColor(getParent(newNode)) == true && getColor(getUncle(newNode)) == true){//if the parent and uncle are red, flip the colors of the gp, p, and u
    //cout << "flipping colors" << endl;
    getParent(newNode)->color = false;
    getUncle(newNode)->color = false;
    getGrandParent(newNode)->color = true;
    Node* gp = getGrandParent(newNode);
    repairTree(gp);
  }
  else if(getColor(getUncle(newNode)) == false && getColor(getParent(newNode)) == true){//if u is black and p is red, rotate the tree
    //cout << "alt sides" << endl;
    //cout << "N: " << newNode->val << " P: " << getParent(newNode)->val << " GP: " << getGrandParent(newNode)->val <<endl;
    if((getGrandParent(newNode)->left == getParent(newNode) && getParent(newNode)->right == newNode)){//if the parent is left node and the child is the right node
      //cout << "left right" << endl;
      Node* gp = getGrandParent(newNode);
      Node* p = getParent(newNode);
      Node* nLeft = newNode->left;
      Node* pLeft = getParent(newNode)->left;

      //rotate the tree
      if(nLeft != NULL){
	nLeft->parent = p;
      }
      p->right = nLeft;
      p->parent = newNode;
      newNode->left = p;
      newNode->parent = gp;
      gp->left = newNode;
      repairTree(newNode->left);
      return;
    }
    else if((getGrandParent(newNode)->right == getParent(newNode) && getParent(newNode)->left == newNode)){//if the parent is the right and the child is left
      //cout << "right left" << endl;
      Node* gp = getGrandParent(newNode);
      Node* p = getParent(newNode);
      Node* nRight = newNode->right;
      Node* pRight = getParent(newNode)->right;

      //rotate the tree
      if(nRight != NULL){
	//cout << 1 << endl;
	nRight->parent = p;
      }

      p->left = nRight;
      p->parent = newNode;
      newNode->right = p;
      newNode->parent = gp;
      gp->right = newNode;
      repairTree(newNode->right);
      //cout << 4 << endl;
      return;
    } 
    //}
    //else if(getColor(newNode) == true && getColor(getParent(newNode)) == true && getColor(getUncle(newNode)) == false){
    //cout << "same side" <<endl;
    if(getGrandParent(newNode)->left == getParent(newNode) && getParent(newNode)->left == newNode){//if the parent is left and the node is left
      //cout << "left left" << endl;
      Node* p = getParent(newNode);
      Node* gp = getGrandParent(newNode);
      Node* ggp = getParent(gp);
      Node* pRight = p->right;
      //cout <<"1. P: " << p->val << "GP: " << gp->val << endl;
      p->color = false;
      gp->color = true;
      if(ggp != NULL){//move the parent up to the top of the subtree
	if(ggp->right == gp){
	  p->parent = ggp;
	  ggp->right = p;
	}
	else if(ggp->left == gp){
	  p->parent = ggp;
	  ggp->left = p;
	}
      }else{
	p->parent = nullptr;
      }
      //cout <<"P: " << p->val << "GP: " << gp->val << endl;
      //cout << 1 << endl;
      if(pRight != NULL){//move the right node of the parent
	pRight->parent = gp;
	//	cout << 2 << endl;
      }
      gp->left = pRight;
      
      //cout <<"P: " << p->val << "GP: " << gp->val << endl;
      //cout << 3 << endl;
      gp->parent = p;
      //cout <<"P: " << p->val << "GP: " << gp->val << endl;
      //cout << 4 << endl;
      p->right = gp;
      //cout <<"P: " << p->val << "GP: " << gp->val <<" pLeft: " << p->left->val << " pRight: " << p->right->val << endl;

      //printTree(p,0);
    }
    else if(getGrandParent(newNode)->right == getParent(newNode) && getParent(newNode)->right == newNode){//if the node and parents are right nodes
      //cout << "right right " << endl;
      Node* p = getParent(newNode);
      Node* gp = getGrandParent(newNode);
      Node* ggp = getParent(gp);
      Node* pLeft = p->left;
      p->color = false;
      gp->color = true;
      if(ggp != NULL){//make the parent the root of the subtree
        if(ggp->right == gp){
          p->parent = ggp;
          ggp->right = p;
        }
        else if(ggp->left == gp){
          p->parent = ggp;
          ggp->left = p;
        }
      }else{
	p->parent = nullptr;
      }
      
      //cout << "got here" << endl;
      if(pLeft !=NULL){
	pLeft->parent = gp;
      }
      gp->right = pLeft;
      gp->parent = p;
      p->left = gp;
    }
  }
}

void findNode(Node* &head, int i, Node* &root){
  //cout << head->val << " " << i <<endl;
  if(head->val > i){
    //cout << "bigger" << endl;
    findNode(head->left, i, head);
  }
  else if(head->val < i){
    findNode(head->right, i, head);
  }
  else{
    //cout << " entereing"<< endl;
    remNode(head, true, root);
  }
}

void remNode(Node* &toDel, bool editTree, Node* &head){
  Node* current = toDel;
  //cout << "toDel: " << toDel->val<<endl;
  if(editTree){
    if(toDel->right != NULL && toDel->left != NULL){
      //cout << "two nodes" << endl;
      current=current->right;
      while(current->left != NULL){
	current = current->left;
      }
    }
    toDel->val = current->val;
  }
  //cout <<current->val << endl;
  Node* child = (current->right == NULL) ? current->left:current->right;
  bool didCase = false;
  if(getColor(current) == false){
    //cout << "black deleted node" << endl;
    if(editTree && getColor(child)  == true){
      //cout << "black, red" << endl;
      child->color = false;
    }
    else{
      //cout << "black black" << endl;
      child = current;
      //cout << "child: " << child->val << endl;
      if(child->parent == NULL){
	//cout << "case 1: root" << endl;
        didCase = true;
      }
      
      if((didCase == false) && (getColor(getSibling(child)) == true)){
	//cout << " case 2: sib is red" << endl;
      
	getParent(child)->color = true;
	getSibling(child)->color = false;

	if(getParent(child)->left == child){
	  //cout <<"child is left node" << endl;
	  Node* sl = getSibling(child)->left;
	  Node* p = getParent(child);
	  Node* s = getSibling(child);

	  if(sl != NULL){
	    sl->parent = p;
	  }
	  p->right = sl;

	  s->parent = p->parent;
	  if(p->parent == NULL){
	    head = s;
	  }
	  else if(p == p->parent->left){
	    p->parent->left = s;
	  }
	  else{
	    p->parent->right = s;
	  }

	  p->parent = s;
	  s->left = p;
	}
	else{

	  //cout << "child is right node" << endl;
	  Node* sr = getSibling(child)->right;
	  Node* p = getParent(child);
	  Node* s = getSibling(child);
	  //printTree(p,0);
	  //return;
	  if(sr != NULL){
	    sr->parent = p;
	  }
	  p->left = sr;

	  s->parent = p->parent;
	  if(p->parent == NULL){
	    head = s;
	  }
	  else if(p == p->parent->left){
	    p->parent->left = s;
	  }
	  else{
	    p->parent->right = s;
	  }

	  p->parent = s;
	  s->right = p;

	  //printTree(p, 0);
	}
      }

      if((didCase == false) && (getColor(getParent(child)) == false) && (getColor(getSibling(child)) == false)
	 && (getColor(getSibling(child)->left) == false) && (getColor(getSibling(child)->right) == false)){

	//cout <<"case 3: all black, recurse on parent" << endl;
	getSibling(child)->color = true;
	remNode(child->parent, false, head);
        
      }
	   
      if((didCase == false) && (getColor(getParent(child)) == true) && (getColor(getSibling(child)) == false) &&
	 (getColor(getSibling(child)->left) == false) && (getColor(getSibling(child)->right) == false)){

	//cout << "case 4: p is red" << endl;
	getParent(child)->color = false;
	getSibling(child)->color = true;
        didCase = true;
      }
      //cout << didCase<< " " << child->val << endl;
      if((didCase == false) && (getColor(getSibling(child)) == false)){
	//cout << "case 5: sibling is black ";
	if((child == getParent(child)->right) && (getColor(getSibling(child)->left) == false) && (getColor(getSibling(child)->right) == true)){

	  //cout << "and sibling is left and left child is black and right child is red" << endl;
	  getSibling(child)->color =true;
	  getSibling(child)->left->color = false;
	  
	  Node* srl = getSibling(child)->right->left;
	  Node* sr = getSibling(child)->right;

	  getSibling(child)->right = srl;
	  srl->parent = getSibling(child);

	  getSibling(child)->parent = sr;
	  sr->left = getSibling(child);

	  sr->parent = getParent(child);
	  getParent(child)->left = sr;
	}
	else if((child == getParent(child)->left) && (getColor(getSibling(child)->left) == true) && (getColor(getSibling(child)->right) == false)){

	  //cout << "and sibling is right and left is red and right is black" << endl;
	  getSibling(child)->color =true;
          getSibling(child)->left->color = false;
	  
          Node* slr = getSibling(child)->left->right;
          Node* sl = getSibling(child)->left;

	  if(slr != NULL){
	    slr->parent = getSibling(child);
	  }
	  getSibling(child)->left = slr;

	  getSibling(child)->parent = sl;
	  sl->right = getSibling(child);
	  
	  sl->parent = getParent(child);
	  getParent(child)->right = sl;
	  
	}
      }
      //printTree(child->parent, 0);
      if((didCase == false) && (getColor(getSibling(child)) == false)){
	//cout << "case 6: sibling is black ";
	if(getColor(getSibling(child)->left) == true){
	  //cout << "left child is red" << endl;

	  Node* sr = getSibling(child)->right;
	  Node* p = getParent(child);
	  Node* s = getSibling(child);

	  s->color = p->color;
	  p->color = false;
	  s->left->color = false;
	  
	  p->left = sr;
	  if(sr !=NULL){
	    sr->parent = p;
	  }
	  
	  s->parent = getGrandParent(child);

	  if(getGrandParent(child) == NULL){
	    head = s;
	  }
	  else if(getGrandParent(child)->left == getParent(child)){

	    getGrandParent(child)->left = s;
	  }
	  else{
	    getGrandParent(child)->right = s;
	  }
	  
	  p->parent = s;
	  s->right = p;
	  //return;
	  didCase = true;
	}
	else if(getColor(getSibling(child)->right) == true){
	  //cout << "right child is red" << endl;

	  Node* sl = getSibling(child)->left;
	  Node* p = getParent(child);
	  Node* s = getSibling(child);

	  s->color = p->color;
	  p->color = false;
	  s->right->color = false;

	  p->right = sl;
	  if(sl !=NULL){
	    sl->parent = p;
	  }

	  s->parent = getGrandParent(child);
	  if(getGrandParent(child) == NULL){
	    head = s;
	  }
          else if(getGrandParent(child)->left == getParent(child)){

            getGrandParent(child)->left = s;
          }
          else{
            getGrandParent(child)->right = s;
          }

	  p->parent = s;
	  s->left = p;
	  //return;
	  didCase = true;
	}
      }
    }
  }
  //cout <<"end: " << current ->val << endl;
  if(editTree){
    child = (current->right == NULL) ? current->left:current->right;
    if(child == NULL){
      //  cout <<"no Nodes after" << endl;
      if(current->parent == NULL){
	toDel = NULL;
      }
      else if(current->parent->left == current){
	current->parent->left = nullptr;
      }
      else{
	current->parent->right = nullptr;
      }
    }
    else{
      //cout << "1 node after" << endl;
      child->parent = current->parent;
      if(current->parent->left == current){
	current->parent->left = child;
      }
      else{
	current->parent->right = child;
      }
    }
  }
}
int debug(Node* head, int count) {//debug by adding a lot of numbers
  if(head == NULL) {
    return count;
  }
  if(getColor(head) == false) {
    count++;
  }
  int i = debug(head->left, count);
  int j = debug(head->right, count);

  if(i == j && i != -1) {
    return i;
  }
  return -1;

}
