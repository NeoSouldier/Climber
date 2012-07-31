//
//  LinkedList.cpp
//  Climber
//
//  Created by Arthur on 07/09/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "LinkedList.h"

LinkedList::LinkedList()
{
    m_pNode = NULL;
    m_pNext = NULL;
}


#pragma mark -
#pragma mark Operations

//Returns true if key exists in list
bool LinkedList::search(int key)
{
    if(m_pNode != NULL)
    {
        if( m_pNode->getKey() == key)
            return true;
    }
    
    if (m_pNext != NULL)
        return m_pNext->search(key);
    
    return false;
}

//Inserts node with chosen key into list, if key already there this function will replace it
void LinkedList::insert(int key)
{
    Node* newNode = new Node(key);
    
    if(m_pNode == NULL)
        m_pNode = newNode;
    
    else
    {
        LinkedList* curr = NULL;
        LinkedList* nxt = this;
        
        while ( nxt != NULL && nxt->m_pNode->getKey() < key)
        {
            curr = nxt;
            nxt  = nxt->m_pNext;
        }
        
        //If we are at the end of the list
        if(nxt == NULL)
        {
            nxt = new LinkedList();
            curr->m_pNext = nxt;
            nxt->m_pNode = newNode;
        }
        
        //If we found an element with the same value as the key
        else if(nxt->m_pNode->getKey() == key)
        {
            nxt->m_pNode = newNode;
        }
        
        //If we are at the head of the list
        else if(curr == NULL)
        {
            curr = new LinkedList();
            curr->m_pNode = this->m_pNode;
            curr->m_pNext = this->m_pNext;
            this->m_pNode = newNode;
            this->m_pNext = curr;
        }
        
        //If we have found the space where the new element must fit into
        else
        {
            LinkedList* elem = new LinkedList();
            elem->m_pNode = newNode;
            elem->m_pNext = nxt;
            curr->m_pNext = elem;
        }
    }
}

//Deletes node with chosen key from list
bool LinkedList::del(int key)
{
    //If the list is empty then you cannot delete anything from it
    if(m_pNode == NULL)
        return false;
    
    
    LinkedList* curr = NULL;
    LinkedList* nxt = this;
    
    while ( nxt != NULL && nxt->m_pNode->getKey() < key)
    {
        curr = nxt;
        nxt  = nxt->m_pNext;
    }
    
    //If we found an element with the same value as the key - remove it
    if(nxt != NULL && nxt->m_pNode->getKey() == key)
    {
        //If we are at the head must treat it differently
        if(curr == NULL)
        {
            delete(m_pNode);
            m_pNode = m_pNext->m_pNode;
            m_pNext = m_pNext->m_pNext;
        }
        
        else 
        {
            curr->m_pNext = nxt->m_pNext;
            delete(nxt);
        }
    
        return true;
    }

    //Any other case will be false
    return false;
}

#pragma mark -
#pragma mark Verifying Methods

//Returns true if list is empty and false otherwise
bool LinkedList::isEmpty()
{
    return (m_pNode == NULL);
}

//Returns the length of the list
int LinkedList::length()
{
    int length = 0;
    
    if(m_pNode != NULL)
        length += 1;
    
    if(m_pNext != NULL)
        length += m_pNext->length();
    
    return length;
}

//Prints the list sequentially
void LinkedList::printList()
{
    if( m_pNode!= NULL)
        std::cout << m_pNode->getKey() << ", ";
    
    if( m_pNext!= NULL)
        m_pNext->printList();
}