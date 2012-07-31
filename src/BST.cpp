//
//  BST.cpp
//  Climber
//
//  Created by Arthur on 07/09/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BST.h"

BST::BST()
{
    //Initialise attributes
    m_pNode = NULL;
    m_pLeft = NULL;
    m_pRight = NULL;
}

#pragma mark -
#pragma mark Helpers

//Returns the max of two ints
int BST::max(int a, int b)
{
    int returnVal = (a > b) ? a : b;
    return returnVal;
}

//Removes the current node and transforms tree
void BST::removeNode(int key)
{
    //If its a leaf node just delete it and set it to NULL
    if(m_pLeft == NULL && m_pRight == NULL)
    {
        std::cout << "Case 1" << std::endl;
        delete m_pNode;
        m_pNode = NULL;
    }
    
    //If only right subtree, then replace current node with its right subtree
    else if( m_pLeft == NULL && m_pRight != NULL)
    {
        std::cout << "Case 2" << std::endl;
        BST* temp = m_pRight;
        
        m_pNode  = m_pRight->m_pNode;
        m_pLeft  = m_pRight->m_pLeft;
        m_pRight = m_pRight->m_pRight;
        
        delete temp;
    }
    
    //If only left subtree, then replace current node with its left subtree
    else if( m_pLeft != NULL && m_pRight == NULL)
    {   
        std::cout << "Case 3" << std::endl;
        BST* temp = m_pLeft;
        
        m_pNode  = m_pLeft->m_pNode;
        m_pRight = m_pLeft->m_pRight;
        m_pLeft  = m_pLeft->m_pLeft;
     
        delete temp;
    }
    
    //If there are two subtrees then replace current node with previous node in key order
    else 
    {
        std::cout << "Case 4" << std::endl;
        BST* previous = previousNode();
        
        std::cout << "found previous" << std::endl;
        m_pNode = previous->m_pNode;
        m_pLeft = previous->m_pLeft;
        
        //delete previous;
    }
    
}

//Returns the previous node from the tree and removes it from overall tree -
//NOTE:Pevious node refers to the one with value literally previous to the current one
//      hence the one in the rightmost branch of the left subtree!
// DO NOT BELIEVE THIS DEALS WITH PTRS VERY WELL, MIGHT MEMORY LEAK
BST* BST::previousNode()
{
    //Create the BST which will be returned
    BST* returnTree = new BST();
    returnTree->m_pLeft = this->m_pLeft;
    
    //Create a temp BST to traverse the tree
    BST* previous = this->m_pLeft;
    BST* temp = previous->m_pRight;
    
    //Find the previous node
    while (temp->m_pRight != NULL){
        previous->m_pRight = temp;
        temp = temp->m_pRight;
    }
    
    //Set previous to have the correct node
    returnTree->m_pNode = temp->m_pNode;
    
    //Remove 'previous' node and return the newly constructed BST with previous on top
    previous->m_pRight = NULL;
    delete temp;
    return returnTree;
}


#pragma mark -
#pragma mark Operations

//Searches for a key returning true if it exists
bool BST::search(int key)
{
    //If the tree does not have anything in it yet then return false
    if (m_pNode == NULL)
        return false;
    
    //Check if they key is equal, if so return true
    if (m_pNode->getKey() == key)
        return true;
    
    //If key is not equal and its less than current key go down left
    if (key < m_pNode->getKey())
    {
        if (m_pLeft != NULL)
            return m_pLeft->search(key);  
    }
    
    //Else check if the key exists down the right path
    else 
    {
        if (m_pRight != NULL)
            return m_pRight->search(key); 
    }
    
    //Otherwise the key does not exist and the search fails
    return false;
}

//Inserts a new node with given key - will replace nodes with the same key
void BST::insert(int key)
{
    //If node not initialised or the key is the same then replace node
    if (m_pNode == NULL || m_pNode->getKey() == key)
        m_pNode= new Node(key);
    
    //Else if key less than current key insert down left subtree
    else if (key < m_pNode->getKey())
    {
        if(m_pLeft == NULL)
            m_pLeft = new BST();
            
        m_pLeft->insert(key);
    }
    
    //Else insert down right subtree
    else
    {
        if(m_pRight == NULL)
            m_pRight = new BST();
        
        m_pRight->insert(key);
    }
}

//Delete a node from the tree - if it does not exist print this out to console and return false
bool BST::del(int key)
{
    if (m_pNode != NULL)
    {
        if (m_pNode->getKey() == key)
        {
            removeNode(key);
        }
        
        else if (key < m_pNode->getKey())
        {
            if( m_pLeft != NULL)
                m_pLeft->del(key);
        }
        
        else 
        {
            if( m_pRight != NULL)
                m_pRight->del(key);
        }
        
        return true;
    }
    
    else {
        std::cout << "Cannot delete Node with key = " << key << " because it does not exist!" << std::endl;
        return false;
    }
}

#pragma mark -
#pragma mark Verification Methods

//Returns true if empty
bool BST::isEmpty()
{
    return (m_pNode == NULL);
}

//Returns the size of the tree
int BST::size()
{
    int size = 0;
    
    if (m_pNode != NULL)
        size += 1;
    
    if (m_pLeft != NULL)
        size += m_pLeft->size();
    
    if (m_pRight != NULL)
        size += m_pRight->size();
    
    return size;
}

//Returns the depth of the tree
int BST::depth()
{
    int depth = 0;
    
    if (m_pNode == NULL)
        return depth;
    
    if (m_pLeft != NULL)
        depth = max(depth, m_pLeft->depth() + 1);
    
    if (m_pRight != NULL)
        depth = max(depth, m_pRight->depth() + 1);
    
    return depth;
}
                    
//Print tree
void BST::printTree()
{
    if( !isEmpty())
    {
        std::cout << m_pNode->getKey() << std::endl;
        std::cout << "left  subtree for " << m_pNode->getKey() << std::endl;
        if (m_pLeft != NULL)
            m_pLeft->printTree();
        else 
            std::cout << "Empty" << std::endl;
        std::cout << "right subtree for " << m_pNode->getKey() << std::endl;
        if (m_pRight != NULL)
            m_pRight->printTree();
        else 
            std::cout << "Empty" << std::endl;
    }
}

//Print values stored in tree
void BST::printFlatTree()
{
    if( !isEmpty())
    {
        std::cout << m_pNode->getKey() << ", ";
        if(m_pLeft != NULL)
            m_pLeft->printFlatTree();
        if(m_pRight != NULL)
            m_pRight->printFlatTree();
    }
}
