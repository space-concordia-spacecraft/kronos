# Event Message

An event message is the structure holding the message that needs to be transported through a bus. 

As mentioned in the introduction, busses are capable of carrying any type of data and any type of event code. The following describes the structure of an event message.

```c++
struct EventMessage {
        KsEventCodeType eventCode = ks_event_invalid;
        std::any data{};
        Bus* returnBus = nullptr;

        template<typename T>
        T Cast() const {
            return std::any_cast<T>(data);
        }
    };
```

The reader may familiarize themselves with the struct, but does not require a deep understanding of it as the framework provides helpers to manage this struct. Given event messages are eventually stored in queues (read components), and that they may hold large amounts of data, these event messages are created by the framework and managed by it.

To be more specific, the busses don't actually carry an event message, but rather a pointer that is managed by the framework itself.
