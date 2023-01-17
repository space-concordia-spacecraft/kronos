# Busses

Busses are used to connect the various components of a system together. Each bus is limited to a carrying single event
type and may not switch the event type it is carrying at runtime.

Busses are designed to cover all possible connection cases between components. This means that a bus can be used as a
one-to-one, many-to-one, one-to-many, and many-to-many connection. There are two main types of busses (synchronous and
asynchronous), each with its own restrictions.

It is important to note that busses are used through bus invocations. A bus invocation may be synchronous or
asynchronous and is determined by the type of the bus that is being invoked. A bus invocation may need an input message
struct depending on the type of message being sent. A bus invocation may also have a return type, only if it is
synchronous. In _**ALL CASES**_, it is the _**RECEIVING COMPONENT's JOB**_ to delete any message data received, as it
is _**ALWAYS**_ copied before being sent.
