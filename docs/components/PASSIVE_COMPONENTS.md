# Passive Components

A passive component is the simplest type of component. It does not provide its own thread or queue. A passive component
consists of a collection of functions that will always be synchronously invoked on the calling thread/component. It
essentially serves as a static library of functions.

Passive components are made to be invoked by active or queued components. An example of a passive component could be
a _**File Manager**_ component in charge of tracking files used and providing thread-safe file operations for other
components. Such operations do not need to run on the same thread and can instead be executed by the calling components.
