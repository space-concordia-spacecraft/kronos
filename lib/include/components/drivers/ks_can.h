#pragma once

#include "ks_component_passive.h"
#include "mcan.h"
#include "conf_mcan.h"

namespace kronos {

    class ComponentCanDriver : public ComponentPassive {
    public:
        ComponentCanDriver(const String& componentName, struct mcan_module* mcan_mod);
        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
        struct mcan_module* mcan_mod;

        /**
         * \brief Queues up a packet for transmission.
         *
         * \param can Pointer to mcan_module that was initialized
         * \param id_value		Message id value
         * \param data			Data array to transmit
         * \param data_length	Length of data to transmit
         *
         * \return True if successfully queued (space available), false otherwise
         */
        KsResult Send(struct mcan_module* mcan_mod, uint32_t id_value, uint8_t *data, uint32_t data_length);

        /**
         * \brief Get a packet from the software reception buffer
         *
         * \param can Pointer to mcan_module that was initialized
         * \param id_value		Message id value
         * \param data			Data array, should be big enough for message, 8 for CAN, 64 for FD. Otherwise data will be dropped on the copy to array (See data_length param)
         * \param data_length	Data buffer size (set before calling the function), this gets updated in the function call to the actual length of data received if its smaller than the max
         *
         * \return True if packet retrieved, false if no packet available
         */
        KsResult Read(struct mcan_module* mcan_mod, uint32_t* id_value, uint8_t* data, uint8_t* data_length);



    };

}