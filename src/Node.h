//
//  Node.h
//  Climber
//
//  Created by Arthur on 24/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Climber_Node_h
#define Climber_Node_h

//Abstract node for any Data Structure with Nodes
//Will be extended to have values associated to keys
class Node
{
private:
    
    //Attributes
    int m_key;
    
public:
    
    //Constructors
    Node(int key);
    
    //Get & Set values
    int getKey();
    void setKey(int key);
};


#endif
