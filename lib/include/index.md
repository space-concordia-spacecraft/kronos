{#mainpage} Spacecraft Framework
============

This framework was originally developed by [Space Concordia](http://www.spaceconcordia.ca/en/) members to be used in [SC-ODIN](http://www.spaceconcordia.ca/en/division/spacecraft/).

## Overview

Kronos is a component-driven framework inspired by the [Fprime](https://github.com/nasa/fprime) architecture. The framework splits up a complex system into various components. A component is defined as an isolated collection of logic created to achieve a single specific goal. Components communicate between each other through the use of busses by publishing or listening to events carried through them. Each bus may only carry one event type and is used to directionally link various components together. 

More detail about each part of the architecture is included in the sections below.

The framework is currently built around FreeRTOS and Reliance Edge but, as a future endeavour, it will be modulated to allow for support of other operating systems and file systems.

## Installation

This upcoming section will be dedicated to the installation of a build environment for Kronos on most popular OS' to date.

#### Pre-requisites

#### Windows

#### Linux

#### Mac OS

## Components

As mentioned above, a component is a collection of logic used to achieve a targeted goal. For instance, if logging is a requirement of your system, you may want to create a _**Logger**_ component which serves to handle logging to a standard output and/or file, and possibly includes various log levels to easily filter incoming data.

It is important to note that components are self-contained, meaning that they are not aware of the source of the events they are processing. This allows for modularity, as it is possible to add or remove certain components without affecting the rest of the system.

There are three types of components: Passive, Queued, and Active, explained in detail below.

#### Passive Components

A passive component is the simplest type of component. It does not provide its own thread or queue. A passive component consists of a collection of functions that will always be synchronously invoked on the calling thread/component. It essentially serves as a static library of functions. 

Passive components are made to be invoked by active or queued components. An example of a passive component could be a File Manager component in charge of tracking files used and providing thread-safe file operations for other components. Such operations do not need to run on the same thread and can instead be executed by the calling components.

#### Queued Components

A queued component implements a queue used to store incoming events which must be cleared by another active component. When its queue is cleared, all events in the queue are processed and the queue is emptied, ready to receive more events. Queued components are similar to passive components in the sense that they do not provide their own thread. However, unlike passive components, incoming events are processed asynchronously and not on the calling component's thread. Events are instead processed by another active component whose job is to intermittently clear the queued component's event queue. 

Queued components are useful in applications where specific inexpensive logic must be executed periodically but does not warrant its own thread. A good example is a scheduled component such as a health monitoring component whose job is to regularly make sure that all active threads are performing their tasks. 

#### Active

In addition to having a queue, active components also provide an execution thread. Active components process events in a similar asynchronous manner to how queued components work. The difference is that instead of having their queue cleared by another component, they process events in their queue one-by-one in their own execution thread. The implications are the same as in the case of queued components, were the logic is not executed on the calling thread but is instead executed on the component's own thread.

Active components are used when components need to coordinate other parts of the system and execute costly logic. These components are the critical components of the system that must remain responsive and timely. In general, there will usually be few active components in a system as maintaining more threads becomes more expensive for the OS' task scheduler. A good example of an active component might be a component that would be in charge of running Attitude Determination and Control logic for the spacecraft. Such a component will need to call upon various other components and is mission critical.

## Busses

Busses are used to connect the various components of a system together. Each bus is limited to a carrying single event type and may not switch the event type it is carrying at runtime.

Busses are designed to cover all possible connection cases between components. This means that a bus can be used as a one-to-one, many-to-one, one-to-many, and many-to-many connection. However, there are two main types of busses (synchronous and asynchronous), each with its own restrictions.

It is important to note that busses are used through bus invocations. A bus invocation may be synchronous or asynchronous and is determined by the type of the bus that is being invoked. A bus invocation may need an input message struct depending on the type of message being sent. A bus invocation may also have a return type, if it is synchronous. In _**ALL CASES**_, it is the _**RECEIVING COMPONENT's JOB**_ to delete any message data received, as it is _**ALWAYS**_ copied before being sent.

#### Synchronous Busses

A synchronous bus is, as its name suggests, always invoked synchronously, meaning on the calling thread. This allows it to provide a return value but also imposes some restrictions on the type of connections it can be used to create. Most prominently, synchronous busses may _**ONLY HAVE AT MOST ONE RECEIVING COMPONENT**_ and that component _**MUST BE A PASSIVE COMPONENT**_. This is with good reason as only passive components allow for execution on the calling thread, and can therefore be treated just like a regular function call. A synchronous bus invocation will always return a void pointer. The framework does provide templated functions to automatically cast the result to the needed type. This also means that to invoke a synchronous bus, a user _**MUST KNOW THE RETURN TYPE**_ of the operation they are requesting. A synchronous bus may have _**MULTIPLE PUBLISHING COMPONENTS**_. It is also important to note that the operation of emptying the queue of a queued component is itself a specific synchronous bus invocation.

#### Asynchronous Busses

In contrast, an asynchronous bus is generally invoked on one thread but the event sent is processed in another. Note that it is still possible that an event sent through an asynchronous bus is processed on the same thread that it is sent from. In such cases, the invocation is still processed asynchronously, as if it were received and sent from different threads. Asynchronous bus invocations do not return values and are not blocking as they execute on a separate thread. Asynchronous busses are not restricted in terms of the number of publishing or receiving components. In addition, components attached to an asynchronous bus are not restricted in type. An asynchronous bus invocation that elicits a response may provide a return bus to which the response will be published. Similar to synchronous bus invocations, the frameworks provides a templated function which copies a specific struct to be sent as input data.