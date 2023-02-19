# Components

As mentioned above, a component is a collection of logic used to achieve a targeted goal. For instance, if logging is a
requirement of your system, you may want to create a _**Logger**_ component which serves to handle logging to a standard
output and/or file, and possibly includes various log levels to easily filter incoming data.

It is important to note that components are self-contained, meaning that they are not aware of the source of the events
they are processing. This allows for modularity, as it is possible to add or remove certain components without affecting
the rest of the system.

There are three types of components: [Passive Components](#passive-components), [Queued Components](#queued-components),
and [Active Components](#active-components), explained in detail below.