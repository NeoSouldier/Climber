//
//  BST.h
//  Climber
//
//  Created by Arthur on 07/09/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Climber_BST_h
#define Climber_BST_h

#include "Node.h"

//This is a reasonable BST - but Delete method is currently very poorly implemented, need to check for possible mem leaks
//Can be used if storing of data is needed later on
class BST 
{
    
private:
  
    //Attributes
    Node* m_pNode;
    BST* m_pLeft, *m_pRight;
    
    //Helpers
    int max(int a, int b);
    void removeNode(int key);
    BST* previousNode();
    
public:
    
    //Constructor
    BST();
    
    //Operations
    bool search(int key);
    void insert(int key);
    bool del(int key);
    
    //Verifying methods
    bool isEmpty();
    int size();
    int depth();
    void printTree();
    void printFlatTree();
    
};

#endif
