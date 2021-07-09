#pragma once

// Kronos includes
#include "ks_component_passive.h"
#include "ks_queue.h"

namespace kronos {

    class ComponentQueued : public ComponentPassive {
    public:
        explicit ComponentQueued(const String& name);

        void ProcessCommandQueue();

        void Init() override;
        void Destroy() override;

        void ReceiveCommand(const CommandMessage& message) override;

    protected:
        kronos::Queue<CommandMessage> m_Queue;
    };

}