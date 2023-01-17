# Active Components

In addition to having a queue, active components also provide an execution thread. Active components process events in a
similar asynchronous manner to how queued components work. The difference is that instead of having their queue cleared
by another component, they process events in their queue one-by-one in their own execution thread. The implications are
the same as in the case of queued components, were the logic is not executed on the calling thread but is instead
executed on the component's own thread.

Active components are used when components need to coordinate other parts of the system and execute costly logic. These
components are the critical components of the system that must remain responsive and timely. In general, there will
usually be few active components in a system as maintaining more threads becomes more expensive for the OS' task
scheduler. A good example of an active component might be a component that would be in charge of running attitude
determination and control logic for the spacecraft. Such a component will need to call upon various other components and
is mission critical, warranting a separate execution thread.
