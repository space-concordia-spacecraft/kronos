// ==================================================================================
// \title ks_component_queued.h
// \brief A component that implements the base for the queued component
// ==================================================================================

#pragma once

#include "ks_queue.h"
#include "ks_component_passive.h"

namespace kronos {

    //! \class ComponentQueued
    //! \brief A class that implements the base for all queued components
    //!
    //! This class is used as the base block for all queued components
    class ComponentQueued : public ComponentPassive {

    public:
        //! \brief Creates a new queued component
        //!
        //! \param name the name of the component
        explicit ComponentQueued(const String& name, KsTickType queueTicksToWait = 0);

        //! \brief Pops all events from the queue and processes them
        ErrorOr<void> ProcessEventQueue();

        //! @copydoc
        ErrorOr<void> Init() override;

        //! @copydoc
        ErrorOr<void> Destroy() override;

        //! @copydoc
        ErrorOr<void> ReceiveEvent(const EventMessage* message) override;

    protected:
        //! Queue that stores events being sent to the component.
        std::shared_ptr<Queue<const EventMessage*>> m_Queue;
        KsTickType m_QueueTicksToWait;
    };

}