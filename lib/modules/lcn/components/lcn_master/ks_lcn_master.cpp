#include "ks_lcn_master.h"
#include "ks_framework.h"

namespace kronos {

    LcnMaster::LcnMaster(const std::string& name, IoDriver* ioDriver) :
        ComponentQueued(name),
        m_IoDriver(ioDriver) {
    }

    void LcnMaster::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_toggle_led:
                Ping();
                break;
        }
    }

    void LcnMaster::Ping() {
        m_IoDriver->Write((uint8_t *)69, 1);
    }

}
