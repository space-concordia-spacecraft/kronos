#pragma once

/* Kronos includes. */
#include "ks_component_passive.h"
#include "ks_queue.h"

namespace kronos {

    class ComponentQueued : public ComponentPassive {
    public:

        void ProcessCommandQueue();

        virtual void Init() override;
        virtual void Destroy() override;

        virtual void ReceiveCommand(const CommandMessage& commandMessage) override;

    private:
        kronos::Queue<CommandMessage> m_Queue;
    };
}