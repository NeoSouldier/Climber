//
//  Node.cpp
//  Climber
//
//  Created by Arthur on 24/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Node.h"

Node::Node(int key)
{
    this->m_key = key;
}

//Return Key
int Node::getKey()
{
    return m_key;
}

//Set key
void Node::setKey(int key)
{
    this->m_key = key;
}