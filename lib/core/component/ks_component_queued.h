// ==================================================================================
// \title ks_component_queued.h
// \brief A component that implements the base for the queued component
// ==================================================================================

#pragma once

#include "ks_component_passive.h"
#include "ks_queue.h"

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
        explicit ComponentQueued(const std::string& name);

        //! \brief Pops all events from the queue and processes them
        KsResultType ProcessEventQueue();

        //! @copydoc
        KsResultType Initialize() override;

        //! @copydoc
        KsResultType Destroy() override;

        //! @copydoc
        KsCmdResult ReceiveEvent(const EventMessage& message) override;

    protected:
        //! Queue that stores events being sent to the component.
        std::shared_ptr<Queue<EventMessage>> m_Queue;
    };

}