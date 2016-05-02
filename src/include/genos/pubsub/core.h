#ifndef GENOS_SUBSCRIBER_CORE_H
#define GENOS_SUBSCRIBER_CORE_H

#include "genos.h"
#include "genos/io/stream.h"
#include "genos/container/dlist.h"

class Gate
{
public:
};

class Node
{
public:
	dlist_head lst;
};

class MessageBroker
{
public:
	dlist<Node, &Node::lst> nodeList;
};

class MessageCore
{

};

#endif