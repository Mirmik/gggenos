#ifndef GENOS_SUBSCRIBER_CORE_H
#define GENOS_SUBSCRIBER_CORE_H

#include "genos.h"
#include "genos/io/stream.h"
#include "genos/container/dlist.h"

class Subscriber
{
	virtual void send_message();
};

class TopicSub
{
	dlist_head lst;
	Subscriber* sub;
};

class Topic
{
	dlist_head lst;
	dlist<TopicSub, &TopicSub::lst> subscribers;
};

class MessageCore
{
	dlist<Topic, Topic::lst> topicList;
};

#endif