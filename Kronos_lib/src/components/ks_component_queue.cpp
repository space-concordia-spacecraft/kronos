#include "ks_component_queue.h"
namespace kronos {
    QueuedComponent::QueuedComponent():m_SendYieldTicks(0),m_ReceiveYieldTicks(0) {

    }

    void QueuedComponent::clearQueue(){
        m_Queue.clear();
    }

    void QueuedComponent::handleNextInQueue() {
        CommandMessage * buffer;
        if( m_Queue.pop(buffer, m_ReceiveYieldTicks ) == pdPASS )
        {
            // TODO: Handle Success
        } else {
            // TODO: Handle Error
        }
    }

    void QueuedComponent::onReceive(int opcode, void * data) {
        CommandMessage message;
        message.opcode = opcode;
        message.data = data;
        message.size = sizeof(data);

        if( m_Queue.push( message, m_SendYieldTicks ) == pdPASS )
        {
            // TODO: Handle Success
        } else {
            // TODO: Handle Error
        }
    }
}
