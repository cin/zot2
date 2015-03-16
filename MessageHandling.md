# Message Handling #
Zystems communicate with each other through an event based messaging system. All of the events are based off of Zmsgs and controlled by the Zmsgq. Zmsgq is a priority queue used to abstract message handling. Additionally, Zmsgq provides mutex protection for the queue itself and is a base member and friend of Zystem -- this allows the Zystem to easily access protected members of Zmsgq. Zmsgq keeps around a Zevent to indicate when a new message is in the queue. The Zevent is signaled whenever a new message is pushed into the queue.

The Zystem's post and push methods are the main functions used to add messages to the Zmsgq. The push method is protected and should only directly be called by Zapp, which is the only system that should be directly passing messages to other systems. The push method also does the message filtering checks required to determine if the system in question should respond to the message being passed in. The post method simply inserts the message into the parent's message queue if possible. If the system does not have a parent, then the message goes into the message queue of the system whose post method is being called.

After a Zystem has processed a message in the message queue, the message is deleted. This will obviously change once smart pointers are used.

## Features ##
  * Whenever Zystems are connected over a network, handle packing order automatically so don't have to think about it when writing the classes themselves.
  * Difference engine support for networked Zystems.

## Further Information ##
For those seeking further information on message handling techniques, Brandon found [Event Machine](http://rubyforge.org/projects/eventmachine) the other day and is similar to the approach taken in Zot2. This also lead to reading about [ACE](http://www.cs.wustl.edu/~schmidt/ACE-overview.html).

## Storage ##
  * Usage of smart pointers for Zmsgs.
    * Reduces heap fragmentation by reducing number of calls to new for messages being passed between systems.
    * If modification of a Zmsg is required by a Zystem, the Zystem must make a copy of the Zmsg in question.

## Processing ##