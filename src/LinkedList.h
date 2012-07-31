//
//  LinkedList.h
//  Climber
//
//  Created by Arthur on 07/09/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Climber_LinkedList_h
#define Climber_LinkedList_h

#include "Node.h"

//LinkedList Data structure in case it is needed later on
class LinkedList 
{
private:
    
    //Attributes
    Node* m_pNode;
    LinkedList* m_pNext;
    
public:
    
    //Constructor
    LinkedList();
    
    //Operations
    bool search(int key);
    void insert(int key);
    bool del(int key);
    
    //Verifying methods
    bool isEmpty();
    int length();
    void printList();
};


#endif
