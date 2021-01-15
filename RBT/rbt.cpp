#include <iostream>
#include <cstring>

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
Node* getParent(Node* n);
Node* getGrandParent(Node* n);
Node* getSibling(Node* n);
Node* getUncle(Node* n);
bool getColor(Node* n);
void normInsert(Node* head, Node* newNode);
void printTree(Node* root, int spaces);
Node* insert(Node* head, Node* newNode);
void repairTree(Node* &newNode);
Node* getHead(Node* currNode);
int main(){
  char command[10];
  Node* head = NULL;
  while(true){
    cout << "what would you like to do: [add,read,print]" << endl;
    cin >> command;
    if(strcmp(command, "add") == 0){
      int num = 0;
      cout << "what number would you like to add?" << endl;
      cin >> num;
      Node* newNode = new Node(num);
      head = insert(head, newNode);
    }
    else if(strcmp(command, "read") == 0){

    }
    else if(strcmp(command, "print") == 0){
      printTree(head, 0);
    }
    else{
      cout << "invalid command, make sure everything is lowercase" << endl;
    }
  }
}
Node* getParent(Node* n){
  return n == nullptr ? nullptr : n->parent;
}
Node* getGrandParent(Node* n){
  return getParent(getParent(n));
}
Node* getSibling(Node* n){
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
Node* getUncle(Node* n){
  Node* p = getParent(n);

  return getSibling(p);
}
bool getColor(Node* n){
  if(n == NULL){
    return false;
  }
  return n->color;
}

void normInsert(Node* head, Node* newNode){
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
  cout << "inserted" << endl;
  repairTree(newNode);
  cout << " repaired" << endl;
  if(getParent(newNode) != NULL){
    cout <<"p" << getParent(newNode)->val << endl;
  }
  if(getUncle(newNode) != NULL){
    cout << "u" <<getUncle(newNode)->val << endl;
  }
  cout << "n" <<newNode->val << endl;
  head = getHead(newNode);
  return head;
}
Node* getHead(Node* currNode){
  Node* current = currNode;
  while(getParent(current) != NULL){
    current = getParent(current);
  }
  return current;
}
void printTree(Node* root, int spaces){
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

void repairTree(Node* &newNode){
  if(getParent(newNode) == NULL){
    cout << "root" << endl;
    newNode->color = false;
  }
  else if(getParent(newNode)->color == false){
    cout << " idc" << endl;
    return;
  }
  else if(getColor(getParent(newNode)) == true && getColor(getUncle(newNode)) == true){
    cout << "flipping colors" << endl;
    getParent(newNode)->color = false;
    getUncle(newNode)->color = false;
    getGrandParent(newNode)->color = true;
    Node* gp = getGrandParent(newNode);
    repairTree(gp);
  }
  else if(getColor(getUncle(newNode)) == false && getColor(getParent(newNode)) == true){
    cout << "alt sides" << endl;
    if((getGrandParent(newNode)->left == getParent(newNode) && getParent(newNode)->right == newNode)){
      cout << "left right" << endl;
      Node* gp = getGrandParent(newNode);
      Node* p = getParent(newNode);
      Node* nLeft = newNode->left;
      Node* pLeft = getParent(newNode)->left;
      if(nLeft != NULL){
	nLeft->parent = p;
	p->right = nLeft;
      }
      p->parent = newNode;
      newNode->left = p;
      newNode->parent = gp;
      gp->left = newNode;
      repairTree(newNode->left);
      return;
    }
    else if((getGrandParent(newNode)->right == getParent(newNode) && getParent(newNode)->left == newNode)){
      cout << "right left" << endl;
      Node* gp = getGrandParent(newNode);
      Node* p = getParent(newNode);
      Node* nRight = newNode->right;
      Node* pRight = getParent(newNode)->right;
      if(nRight != NULL){
	nRight->parent = p;
	p->left = nRight;
      }
      p->parent = newNode;
      newNode->right = p;
      gp->right = newNode;
      repairTree(newNode->right);
      return;
    }
    //}
    //else if(getColor(newNode) == true && getColor(getParent(newNode)) == true && getColor(getUncle(newNode)) == false){
    cout << "same side" <<endl;
    if(getGrandParent(newNode)->left == getParent(newNode) && getParent(newNode)->left == newNode){
      cout << "left left" << endl;
      Node* p = getParent(newNode);
      Node* gp = getGrandParent(newNode);
      Node* ggp = getParent(gp);
      Node* pRight = p->right;
      cout <<"1. P: " << p->val << "GP: " << gp->val << endl;
      p->color = false;
      gp->color = true;
      if(ggp != NULL){
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
      cout <<"P: " << p->val << "GP: " << gp->val << endl;
      cout << 1 << endl;
      if(pRight != NULL){
	pRight->parent = gp;
	cout << 2 << endl;
	gp->left = pRight;
      }
      cout <<"P: " << p->val << "GP: " << gp->val << endl;
      cout << 3 << endl;
      gp->parent = p;
      cout <<"P: " << p->val << "GP: " << gp->val << endl;
      cout << 4 << endl;
      p->right = gp;
      cout <<"P: " << p->val << "GP: " << gp->val <<" pLeft: " << p->left->val << " pRight: " << p->right->val << endl;

      //printTree(p,0);
    }
    else if(getGrandParent(newNode)->right == newNode && getParent(newNode)->right == newNode){
      cout << "right right " << endl;
      Node* p = getParent(newNode);
      Node* gp = getGrandParent(newNode);
      Node* ggp = getParent(gp);
      Node* pLeft = p->left;
      p->color = false;
      gp->color = true;
      if(ggp != NULL){
        if(ggp->right == gp){
          p->parent = ggp;
          ggp->right = p;
        }
        else if(ggp->left == gp){
          p->parent = ggp;
          ggp->left = p;
        }
      }
      cout << "got here" << endl;
      pLeft->parent = gp;
      gp->right = pLeft;
      gp->parent = p;
      p->left = gp;
    }
  }
}

