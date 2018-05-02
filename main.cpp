#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

using namespace std;

struct node{
  int value = 0;
  int color = -1;
  node* left = NULL;
  node* right = NULL;
  node* parent = NULL;
};

void print(node* current, int number);
void rotateLeft(node* n);
void rotateRight(node* n);
node* parent(node* n);
node* grandParent(node* n);
node* sibling(node* n);
node* uncle(node* n);
node* insert(node* root, node* n);
void insertRecursive(node* root, node* n);
void insertRepairTree(node* n);
void insertCase1(node* n);
void insertCase2(node* n);
void insertCase3(node* n);

node* parent(node* n){
  return n->parent;
}

node* grandParent(node* n){
  node* p = parent(n);
  if(p == NULL){
    return NULL;
  }
  return parent(p);
}

node* sibling(node* n){
  node* p = parent(n);
  if (p == NULL){
    return NULL;
  }
  if (n == p->left){
    return p->right;
  }
  else{
    p->left;
  }
}

node* uncle(node* n){
  node* p = parent(n);
  node* g = grandParent(n);
  if(g == NULL){
    return NULL;
  }
  return sibling(p);
}

const int RED = 0;
const int BLACK = 1;

//constructor
int main(){
  char* input = new char[20];
  char* number = new char[10];
  int numbers[100];
  for(int i = 1; i <= 99; i++){
    numbers[i] = -1;
  }
  node* heap[100];
  heap[0] = NULL;
  int i;
  bool quit = false;
  node* head = NULL;
  
  cout << "Input numbers or the word 'filename' to add to the tree. Type delete to delete a number from the tree and quit to quit the program." << endl;
  while(quit == false){
    //get input from user
    cout << "Input a number, or the words filename, delete, or quit" << endl;
    cin.getline(input, 10);
    if(strcmp(input, "quit") == 0){
      quit = true;
    }
    else if(strcmp(input, "delete") == 0){
      cout << "What number would you like to delete?" << endl;
      cin.getline(number, 5);
      if(head != NULL){
	//Delete(head, head, atoi(number));
      }
      if(head != NULL){
	print(head, 0);
      }
      else{
	cout << "There is nothing in the tree." << endl;
      }
    }
    else if(strcmp(input, "filename") == 0){
      cout << "What is the file name?" << endl;
      char fileName[200];
      char input[200];
      cin.getline(fileName, 100);
      ifstream file(fileName);
      if(file.is_open()){
	file.getline(input, 200);
      }
      else{
	cout << "Could not find the file, try again." << endl;
      }
      int counter = 0;
      for(int i = 0; i < strlen(input); i++){
	//if it is a digit, then convert it into a single token
	if(isdigit(input[i]) == true){
	  //keep track of the start of the number and the length of the number
	  int start = i;
	  int length = 1;
	  while(input[i+1] != ' '){
	    length++;
	    i++;
	  }
	  //now convert the numbers into a single number
	  char* number = new char[length];
	  for(int j = 0; j != length; j++){
	    number[j] = input[start+j];
	  }
	  //create node and add it to the tree
	  node* Node = new node;
	  Node->value = atoi(number);
	  head = insert(head, Node);
	  print(head, 0);
	}
      }
    }
    else{
      node* Node = new node;
      Node->value = atoi(input);
      //add a node to the tree
      head = insert(head, Node);
      
      //print out the tree
      print(head, 0);
    }
  }
}

//print the tree
void print(node* current, int number){
  if(current->right != NULL){
    print(current->right, ++number);
    number--;
  }

  for(int i = 1; i <= number; i++){
    cout << "\t";
  }
  cout << current->value;
  if(current->color == 0){
    cout << "R" << endl;
  }
  else if(current->color == 1){
    cout << "B" << endl;
  }

  if(current->left != NULL){
    print(current->left, ++number);
    number--;
  }
}

void rotateLeft(node* n){
  node* nnew = n->right;
  n->right = nnew->left;
  if(nnew->left != NULL){
    nnew->left->parent = n;
  }
  nnew->left = n;
  nnew->parent = n->parent;
  if(n->parent != NULL){
    if(n->parent->right == n){
      n->parent->right = nnew;
    }
    else if (n->parent->left == n){
      n->parent->left = nnew;
    }
  }
  if(nnew->parent != NULL){
    if(nnew->parent->right == nnew){
      nnew->parent->right = nnew;
    }
    else if (nnew->parent->left == nnew){
      nnew->parent->left = nnew;
    }
  }
  n->parent = nnew;
  //the other related parent and child links would also have to be updated
}

void rotateRight(node* n){
  node* nnew = n->left;
  n->left = nnew->right;
  if(nnew->right != NULL){
    nnew->right->parent = n;
  }
  nnew->right = n;
  nnew->parent = n->parent;
  if(n->parent != NULL){
    if(n->parent->right == n){
      n->parent->right = nnew;
    }
    else if (n->parent->left == n){
      n->parent->left = nnew;
    }
  }
  if(nnew->parent != NULL){
    if(nnew->parent->right == nnew){
      nnew->parent->right = nnew;
    }
    else if (nnew->parent->left == nnew){
      nnew->parent->left = nnew;
    }
  }
  n->parent = nnew;
  //the other related parent and child links would also have to be updated
}

node* insert(node* root, node* n){
  insertRecursive(root, n);
  insertRepairTree(n);
  root = n;
  while(parent(root) != NULL){
    root = parent(root);
  }
  return root;
}

void insertRecursive(node* root, node* n){
  //recursively descend the tree until a leaf is found
  if(root == NULL){
    return;
  }
  if(n->value < root->value){
    if(root->left != NULL){
      insertRecursive(root->left, n);
      return;
    }
    else{
      root->left = n;
    }
  }
  else if(n->value > root->value){
    if(root->right != NULL){
      insertRecursive(root->right, n);
      return;
    }
    else{
      root->right = n;
    }
  }
  
  //insert new node
  n->parent = root;
  n->left = NULL;
  n->right = NULL;
  n->color = RED;
}

void insertRepairTree(node* n){
  if (parent(n) == NULL){
    insertCase1(n);
  }
  else if (parent(n)->color == BLACK){
    //do nothing because tree is still valid
  }
  else if (uncle(n) != NULL && uncle(n)->color == RED){
    insertCase2(n);
  }
  else if((uncle(n) == NULL || (uncle(n) != NULL && uncle(n)->color == BLACK)) && (parent(n) != NULL && parent(n)->color == RED) && (grandParent(n) != NULL)){
    insertCase3(n);
  }
}

void insertCase1(node* n){
  n->color = BLACK;
}

void insertCase2(node* n){
  cout << "case2" << endl;
  cout << n->value << endl;
  parent(n)->color = BLACK;
  uncle(n)->color = BLACK;
  grandParent(n)->color = RED;
  insertRepairTree(grandParent(n));
}

void insertCase3(node* n){
  cout << "case3" << endl;
  node* p = parent(n);
  node* gP = grandParent(n);
  
  if (gP->left != NULL && n == gP->left->right){
    rotateLeft(p);
    n = n->left;
  }
  else if(gP->right != NULL && n == gP->right->left){
    rotateRight(p);
    n = n->right;
  }
  if(n == parent(n)->left){
    rotateRight(gP);
  }
  else if(n == parent(n)->right){
    rotateLeft(gP);
  }
  parent(n)->color = BLACK;
  n->color = RED;
  gP->color = RED;
}
