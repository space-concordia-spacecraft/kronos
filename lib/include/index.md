Framework                         {#mainpage}
============

This framework was originally developed by [Space Concordia](http://www.spaceconcordia.ca/en/) members to be used in [SC-ODIN](http://www.spaceconcordia.ca/en/division/spacecraft/).

## Overview

Kronos is a component-driven framework inspired by the [Fprime](https://github.com/nasa/fprime) architecture. The main difference is that the components in Kronos communicate through busses rather than through ports. 

The architecture revolves around publishing an event onto a bus and having each component process the event. There are 3 types of components, Passive, Queued, and Active, and 2 types of busses, Synchronous, and Asynchronous.

The framework is currently built around FreeRTOS, and Reliance Edge, but it's possible that in the future it will support other Operating Systems and FileSystems.

## Installation



## Components

Components are simply a collection of functions and variables used for specific behavior. For instance, if you wanted to implement logging behavior, you would want to create a component called logger and implement all functions related to logging within that component.

It's important to note that components don't really know who is sending the information, they just know how to process the event. This allows for modularity, as it would be possible to remove certain components without crashing the whole system.

As mentioned previously, there are three types of components: Passive, Queued, and Active. 

#### Passive

Passive components are the most basic type of component. These are a collection of functions that run on the thread of the component that sends the event.

#### Queued

Queued components are derived from the passive component. These components have a queue used to store events being sent to it. When it receives the event to clear the queue, a function is called to pop each of the events in the queue until it's empty. Just like the passive components, they run on the thread of the component that publishes the event.

#### Active

Active components are derived from the queued component. These components behave just like the queued components, the only difference is that they have their own thread to clear the queue. All events being sent to the component are also processed on the thread of the component.

## Busses

Busses act as the connections between components. 

#### Synchronous

#### Asynchronous