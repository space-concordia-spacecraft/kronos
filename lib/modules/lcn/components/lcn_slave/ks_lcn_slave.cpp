#include "ks_lcn_slave.h"
#include "ks_framework.h"

namespace kronos {

    LcnSlave::LcnSlave(const std::string& name, IoDriver* ioDriver) :
        ComponentQueued(name),
        m_IoDriver(ioDriver) {
    }

    void LcnSlave::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_toggle_led:
                CheckNetwork();
                break;
        }
    }

    void LcnSlave::CheckNetwork() {
        static uint8_t buf[5];
        auto len = m_IoDriver->Read(buf, 1);
        if(len > 0)
            m_IoDriver->Write((uint8_t *)70, 1);
    }

}
