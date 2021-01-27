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
    parent = NULL;
    left = NULL;
    right = NULL;
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
void findNode(Node* &head, int i);
void remNode(Node* &toDel);
//my own
void repairTree(Node* &newNode);
Node* getHead(Node* currNode);

//courtesy of nividh
int debug(Node* head, int count);

int main(){
  char command[10];
  Node* head = NULL;
  ifstream numFile;
  while(true){
    cout << "what would you like to do: [add,read,print]" << endl;
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
      head = NULL;
      }
    }
    else if(strcmp(command, "delete") == 0){
      cout << "what number would you like to delete?" << endl;
      int toDel;
      cin>>toDel;
      findNode(head, toDel);
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
  if(p == NULL){
    return NULL;
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
  newNode->left = NULL;
  newNode->right = NULL;
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
	p->parent = NULL;
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
	p->parent = NULL;
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

void findNode(Node* &head, int i){
  cout << head->val << " " << i <<endl;
  if(head->val > i){
    cout << "bigger" << endl;
    findNode(head->left, i);
  }
  else if(head->val < i){
    findNode(head->right, i);
  }
  else{
    cout << " entereing"<< endl;
    remNode(head);
  }
}
void remNode(Node* &toDel){
  /*if(toDel->left == NULL && toDel->right == NULL){
    cout <<"no children"<< endl;
    toDel = NULL;
    return;
    }*/
  if(toDel->left == NULL){
    cout << "left is empty" << endl;
    if(getColor(toDel) == true && getColor(toDel->right) == false){
      cout << "red, black" << endl;
      Node* temp = toDel->right;
      delete(toDel->right);
      toDel = temp;
      //temp->parent = toDel->parent;
      //toDel->right = NULL;
      //toDel->val = temp->val;
      //toDel->color = temp->color;
      return;
    }
    else if(getColor(toDel) == false && getColor(toDel->right) == true) {
      cout <<"black red" << endl;
      Node* temp = toDel->right;
      temp->color = false;
      temp->parent = toDel->parent;
      delete(toDel->right);
      toDel = temp;
      return;
      
    }
    else if(getColor(toDel) == false && getColor(toDel->right) == false) {
      cout << "black black" << endl;
      if(toDel->parent == NULL){
	if(toDel->right != NULL){
	  toDel->val = toDel->right->val;
	  toDel->right = NULL;
	}
	else{
	  toDel = NULL;
	}
	
	return;
      }
      else{
        if(toDel->right != NULL){
	  Node* temp = toDel->right;
	  temp->parent = toDel->parent;
	  toDel = temp;
	  if(getColor(getSibling(toDel)) == true){
	    toDel->parent->color = true;
	    getSibling(toDel)->color = false;
	    if(toDel == toDel->parent->left){
	      Node* p = getParent(toDel);
	      Node* s = getSibling(toDel);
	      Node* sLeft = getSibling(toDel)->left;

	      sLeft->parent = p;
	      p->right=sLeft;

	      s->parent = p->parent;
	      if(p == p->parent->left){
		p->parent->left = s;
	      }
	      else{
		p->parent->right = s;
	      }
	      
	      p->parent = s;
	      s->left = p;
	    }
	    else if(toDel == toDel->parent->right){
	      Node* p =getParent(toDel);
	      Node* s = getSibling(toDel);
	      Node* sRight = getSibling(toDel)->right;

	      sRight->parent = p;
	      p->left = sRight;

	      s->parent = p->parent;
              if(p == p->parent->left){
                p->parent->left = s;
              }
              else{
                p->parent->right = s;
              }

	      p->parent = s;
	      s->right = p;
	    }
	  }
	  if((toDel->parent->color == false) && (getColor(getSibling(toDel)) == false) && (getColor(getSibling(toDel)->right) == false) && (getColor(getSibling(toDel)->left) == false)){
	    getSibling(toDel)->color = true;
	    remNode(toDel->parent);
	  }
	  else if((toDel->parent->color == true) && (getColor(getSibling(toDel)) == false) && (getColor(getSibling(toDel)->right) == false) && (getColor(getSibling(toDel)->left) == false)){
	    getSibling(toDel)->color = true;
	    getParent(toDel)->color = false;
	    return;
	  }
	  else if(getSibling(toDel)->color == false){
	    if((getSibling(toDel) == getParent(toDel)->left) && (getColor(getSibling(toDel)) == false)
	       && (getColor(getSibling(toDel)->left) == false) && (getColor(getSibling(toDel)->right) == true)){
	      Node* s = getSibling(toDel);
	      Node* srl = getSibling(toDel)->right->left;
	      Node* sr = getSibling(toDel)->right;
	      s->color = true;
	      getSibling(toDel)->right->color = false;

	      srl->parent = s;
	      s->right = srl;

	      s->parent = sr;
	      sr->left = s;

	      getParent(toDel)->left = sr;
	      sr->parent = getParent(toDel);
	    }
	    else if((getSibling(toDel) == getParent(toDel)->right) && (getColor(getSibling(toDel)) == false)
		    && (getColor(getSibling(toDel)->left) == true) && (getColor(getSibling(toDel)->right) == false)){
	      Node* s = getSibling(toDel);
	      Node* slr = getSibling(toDel)->left->right;
	      Node* sl = getSibling(toDel)->left;

	      s->color = true;
	      getSibling(toDel)->left->color = false;

	      slr->parent = s;
	      s->left = slr;

	      s->parent = sl;
	      sl->right = s;

	      getParent(toDel)->right = sl;
	      sl->parent = getParent(toDel);
	    }
	  }
	  if((getColor(getSibling(toDel)) == false) && (getColor(getSibling(toDel)->left) == true) && (getColor(toDel) == false) && (toDel == toDel->parent->right)){
	    Node* p = getParent(toDel);
	    Node* sr = getSibling(toDel)->right;
	    Node* s = getSibling(toDel);
	    s->color = p->color;
	    p->color = false;
	    s->left->color = false;

	    sr->parent = p;
	    p->left = sr;

	    s->parent = p->parent;
	    if(p == p->parent->left){
	      p->parent->left = s;
	    }
	    else{
	      p->parent->right = s;
	    }

	    s->right = p;
	    p->parent = s;
	  }
	  else if((getColor(toDel) == false) && (getColor(getSibling(toDel)) == false) && (getColor(getSibling(toDel)->right) == true) && (toDel == toDel->parent->left)){
	    Node* p = getParent(toDel);
	    Node* sl = getSibling(toDel)->left;
	    Node* s = getSibling(toDel);

	    s->color = p->color;
	    p->color = false;
	    s->right->color = false;

	    sl->parent = p;
	    p->right = sl;

	    s->parent = p->parent;
            if(p == p->parent->left){
              p->parent->left = s;
            }
            else{
              p->parent->right = s;
            }

	    s->left = p;
	    p->parent = s;
	  }
	}
	else{
	  Node* p = getParent(toDel);
	  Node* s = getSibling(toDel);
	  toDel = NULL;
	  if(getColor(s) == true){
            p->color = true;
            s->color = false;
            if(toDel == p->left){
              Node* sLeft = s->left;

              sLeft->parent = p;
              p->right=sLeft;

	      s->parent = p->parent;
              if(p == p->parent->left){
                p->parent->left = s;
              }
              else{
                p->parent->right = s;
              }

              p->parent = s;
              s->left = p;
            }
            else if(toDel == p->right){
              Node* sRight = s->right;

              sRight->parent = p;
              p->left = sRight;

	      s->parent = p->parent;
              if(p == p->parent->left){
                p->parent->left = s;
              }
              else{
                p->parent->right = s;
              }

              p->parent = s;
              s->right = p;
            }
          }

	  if((p->color == false) && (getColor(s) == false) && (getColor(s->right) == false) && (getColor(s->left) == false)){
            s->color = true;
            remNode(p);
          }
          else if((p->color == true) && (getColor(s) == false) && (getColor(s->right) == false) && (getColor(s->left) == false)){
            s->color = true;
            p->color = false;
            return;
          }
          else if(s->color == false){
            if((s == p->left) && (getColor(s) == false)
               && (getColor(s->left) == false) && (getColor(s->right) == true)){
              Node* srl = s->right->left;
              Node* sr = s->right;
              s->color = true;
              s->right->color = false;

              srl->parent = s;
              s->right = srl;

              s->parent = sr;
              sr->left = s;

              p->left = sr;
              sr->parent = p;
            }
            else if((s == p->right) && (getColor(s) == false)
                    && (getColor(s->left) == true) && (getColor(s->right) == false)){
              Node* slr = getSibling(toDel)->left->right;
              Node* sl = getSibling(toDel)->left;

              s->color = true;
              s->left->color = false;

              slr->parent = s;
              s->left = slr;

              s->parent = sl;
              sl->right = s;

              p->right = sl;
              sl->parent = p;
            }
          }
	  if((getColor(s) == false) && (getColor(s->left) == true) && (getColor(toDel) == false) && (toDel == p->right)){
            Node* sr = s->right;
            s->color = p->color;
            p->color = false;
            s->left->color = false;

            sr->parent = p;
            p->left = sr;

            s->parent = p->parent;
            if(p == p->parent->left){
              p->parent->left = s;
            }
            else{
              p->parent->right = s;
            }

            s->right = p;
            p->parent = s;
          }
	  else if((getColor(toDel) == false) && (getColor(s) == false) && (getColor(s->right) == true) && (toDel == p->left)){
            Node* sl = s->left;

            s->color = p->color;
            p->color = false;
            s->right->color = false;

            sl->parent = p;
            p->right = sl;

            s->parent = p->parent;
            if(p == p->parent->left){
              p->parent->left = s;
            }
            else{
              p->parent->right = s;
            }

            s->left = p;
            p->parent = s;
          }
	}
      }
    }
  }
  else if(toDel->right == NULL){
    cout <<"right empty " << endl;
    if(toDel->color == true && toDel->left->color == false){
      Node* temp = toDel->left;
      temp->parent = toDel->parent;
      delete(toDel->left);
      toDel = temp;
      return;
    }
    else if(toDel->color == false && toDel->left->color == true){
      Node* temp = toDel->left;
      temp->parent = toDel->parent;
      delete(toDel->left);
      toDel=temp;
      return;
    }
  }
  else{
    Node* current = toDel->left;
    while(current->right != NULL){
      current = current->right;
    }
    remNode(current);
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
