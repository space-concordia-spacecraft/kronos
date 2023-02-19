# Asynchronous Busses

In contrast, an asynchronous bus is generally invoked on one thread but the event sent is processed in another. Note
that it is still possible that an event sent through an asynchronous bus is processed on the same thread that it is sent
from. In such cases, the invocation is still processed asynchronously, as if it were sent and received on different
threads. Asynchronous bus invocations do not return values and are non-blocking as they execute on a separate thread.
Asynchronous busses are not restricted in terms of the number of publishing or receiving components. In addition,
components attached to an asynchronous bus are not restricted in type. An asynchronous bus invocation that elicits a
response may provide a return bus to which the response will be published. Similar to synchronous bus invocations, the
framework provides a templated function which copies a specific struct to be sent as input data to an asynchronous
invocation.