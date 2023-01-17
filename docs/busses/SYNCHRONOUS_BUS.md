# Synchronous Busses

A synchronous bus is, as its name suggests, always invoked synchronously, meaning on the calling thread. This allows it
to provide a return value but also imposes some restrictions on the type of connections it can be used to create. Most
prominently, synchronous busses may _**ONLY HAVE AT MOST ONE RECEIVING COMPONENT**_ and that component _**MUST BE A
PASSIVE COMPONENT**_. This is with good reason as only passive components allow for execution on the calling thread, and
the invocation can therefore be treated just like a regular function call. A synchronous bus invocation will always
return a void pointer. The framework provides templated functions to automatically cast the result to the needed type.
This also means that to invoke a synchronous bus, a user _**MUST KNOW THE RETURN TYPE**_ of the operation they are
requesting. A synchronous bus may have _**MULTIPLE PUBLISHING COMPONENTS**_. It is also important to note that the
operation of emptying the queue of a queued component is itself a specific synchronous bus invocation.
