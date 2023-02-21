# Busses
Busses are used to connect the various components of a system together. They are capable of transporting any event code and event type during runtime. While this does create some potentially unsafe behavior, it is a tradeoff made to facilitate software development. 

Busses are designed to cover all possible connection cases between components. This means that a bus can be used as a one-to-one, many-to-one, one-to-many, and many-to-many connection. They are mainly used throughout the framework as a means to allow partial dependency. 

To further illustrate this idea, picture having component **A** and **B** that are attached via a bus. Component **A** subscribes to a Bus named **TestBus**. Component **B** then publishes to that same bus. If in the near future, the developer swaps component **A** with a different component **C**, as long as the new component is subscribed to the intended bus, and it manages the same events, it will continue to work as intended. This is mainly done in case both component **A** and **C** have the same inputs but require different logic.

## Creating a Bus
To create a bus it's quite simple, it only requires the name of the bus. Keep in mind the name of the bus is used as an identifier for that bus, meaning the name should be unique. Kronos uses the naming convention of `B_NAME`, where `B` symbolizes that it's a bus and `NAME` references the logic that it's attached to.

```c++
Framework::CreateBus("B_NAME")
```

## Publishing
To simplify the use of busses, there are two different ways of publishing to a bus. One publish function takes only the event code and the other takes a code and the message.

It's important to ensure the message is the same type that is used when being referenced by the subscriber (look at the preceding section for an example).

The following is an example of publishing an event code to a bus.

{% code title="ks_command_dispatcher.cpp" overflow="wrap" lineNumbers="true" %}
```c++
Framework::GetBus("B_LOGGER")->Publish(ks_event_log_toggle_echo);
```
{% endcode %}

The following is an example of publishing an event code and an event message to a bus.

{% code title="ks_command_dispatcher.cpp" overflow="wrap" lineNumbers="true" %}
```c++
Framework::GetBus("B_FILE_MANAGER")->Publish( String((char*)packet.Payload), ks_event_file_downlink_begin);
``` 
{% endcode %}

## Subscribing
To subscribe to a bus it's quite simple, simply fetch the bus and call the `AddReceivingComponent(component)` function. The parameter that it takes in is a pointer to the component subscribing to the bus.

```c++
Framework::GetBus("B_NAME")->AddReceivingComponent(component);
```

## Processing a Message
When a message is published to a bus, the bus creates the event message through the framework and then passes on the event message to all components subscribed to that bus.

{% code title="ks_bus.h" overflow="wrap" lineNumbers="true" %}
```c++
for (auto component: m_ReceivingComponents) {
    EventMessage* message = Framework::CreateEventMessage(eventCode, returnBus);
    component->ReceiveEvent(message);
}
```
{% endcode %}

Notice the for loop calls the `ReceiveEvent(message)` function implemented by the component. This message is then processed immediately, or added to a queue (check components to further understand the difference).

Finally, when the respective component is ready to process the message, it calls the following function.

{% code title="ks_command_dispatcher.cpp" overflow="wrap" lineNumbers="true" %}
```c++
void CommandDispatcher::ProcessEvent(const EventMessage& message) {
    switch (message.eventCode) {
        case ks_event_comms_dispatch:
            ProcessCommand(message.Cast<Packet>());
            break;
    }

    ComponentActive::ProcessEvent(message);
}
```
{% endcode %}

The `ProcessEvent()` function takes in the message. To extract the data being transported, the developer can simply call the `Cast<T>()` function implemented by `EventMessage`. Make sure that `T` is the same type being sent by the `Publish()` function.
