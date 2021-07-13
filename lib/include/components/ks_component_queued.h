#pragma once

// Kronos includes
#include "ks_component_passive.h"
#include "ks_queue.h"

namespace kronos {

    /// Class implementation for Queued components. These components have a queue but no thread.
    ///
    /// Queued components store the events being sent to it into a queue until the clear queue event message is sent to it. Once it receives the clear queue event, it processes all of the events on the thread of the calling component.
    class ComponentQueued : public ComponentPassive {
    public:
        /// @copydoc
        explicit ComponentQueued(const String& name);

        /// Function to clear pop all events from the queue.
        void ProcessEventQueue();

        /// @copydoc
        void Init() override;

        /// @copydoc
        void Destroy() override;

        /// @copydoc
        KsCmdResult ReceiveEvent(const EventMessage& message) override;

    protected:
        /// Queue that stores events being sent to the component.
        kronos::Queue<EventMessage> m_Queue;
    };

}