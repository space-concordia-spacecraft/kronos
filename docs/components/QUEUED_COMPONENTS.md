# Queued Components

A queued component implements a queue used to store incoming events which must be cleared by another active component.
When its queue is cleared, all events in the queue are processed and the queue is emptied, ready to receive more events.
Queued components are similar to passive components in the sense that they do not provide their own thread. However,
unlike passive components, incoming events are processed asynchronously and not on the calling component's thread.
Events are instead processed by another active component whose job is to intermittently clear the queued component's
event queue.

Queued components are useful in applications where specific inexpensive logic must be executed periodically but does not
warrant its own thread. A good example is a scheduled component such as a health monitoring component whose job is to
regularly make sure that all active threads are performing their tasks.
