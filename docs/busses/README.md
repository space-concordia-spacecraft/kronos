# Busses
## Introduction
Busses are used to connect the various components of a system together. They are capable of transporting any event code and event type during runtime. While this does create some potentially unsafe behavior, it is a tradeoff made to facilitate software development. 

Busses are designed to cover all possible connection cases between components. This means that a bus can be used as a one-to-one, many-to-one, one-to-many, and many-to-many connection. They are mainly used throughout the framework as a means to allow partial dependency. 

To further illustrate this idea, picture having component **A** and **B** that are attached via a bus. Component **A** subscribes to a Bus named **TestBus**. Component **B** then publishes to that same bus. If in the near future, the developer swaps component **A** with a different component **C**, as long as the new component is subscribed to the intended bus, and it manages the same events, it will continue to work as intended. This is mainly done in case both component **A** and **C** have the same inputs but require different logic.

## Publishing

```c++
Framework::GetBus("B_LOGGER")->Publish(ks_event_log_toggle_echo);
```


```c++
Framework::GetBus("B_FILE_MANAGER")->Publish(
                    String((char*)packet.Payload), ks_event_file_downlink_begin);
``` 

## Subscribing

```c++
CommandDispatcher::CommandDispatcher(const String& name) : ComponentActive(name, KS_QUEUE_DEFAULT_WAIT_TIME),
                                                           m_Bus(Framework::CreateBus("B_CMD_DISPATCH")) {
    m_Bus->AddReceivingComponent(this);
}
```
