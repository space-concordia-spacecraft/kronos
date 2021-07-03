#pragma once

/* Kronos includes. */
#include "ks_component_passive.h"
#include "ks_queue.h"

namespace kronos {

    class QueuedComponent : private PassiveComponent {
    public:
        QueuedComponent();
        ~QueuedComponent();

        void clearQueue();
        void handleNextInQueue();
        virtual void onReceive(int opcode, void * data) override;

        void setSendYieldTicks(TickType_t ticks);
        void setReceiveYieldTicks(TickType_t ticks);

        TickType_t getSendYieldTicks();
        TickType_t getReceiveYieldTicks();
    private:
        kronos::Queue<CommandMessage> m_Queue;
        TickType_t m_SendYieldTicks;
        TickType_t m_ReceiveYieldTicks;
    };
}