#include "ks_can.h"

namespace kronos {

    ComponentCanDriver::ComponentCanDriver(const String& componentName, struct mcan_module *mcan_mod): ComponentPassive(componentName) {
    /*
         *  Setup rx filtering to accept messages into FIFO1 with extended format
         *  this accepts all messages
         */
        struct mcan_extended_message_filter_element et_filter;
        mcan_get_extended_message_filter_element_default(&et_filter);
        et_filter.F1.reg = MCAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F1_EFID2(0) |
                           MCAN_EXTENDED_MESSAGE_FILTER_ELEMENT_F1_EFT_CLASSIC;
        mcan_set_rx_extended_filter(mcan_mod, &et_filter, 0);
    }



    KsCmdResult ComponentCanDriver::ProcessEvent(const EventMessage& message) {
        switch (message.opcode) {
            case KS_EVENT_CODE_READ:

                break;
            case KS_EVENT_CODE_SEND:
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResult ComponentCanDriver::Send(struct mcan_module* mcan_mod, uint32_t id_value, uint8_t *data, uint32_t data_length)
    {
        uint32_t status = mcan_tx_get_fifo_queue_status(mcan_mod);

        //check if fifo is full
        if(status & MCAN_TXFQS_TFQF) {
            return false;
        }

        //get the put index where we put the next packet
        uint32_t put_index = (status & MCAN_TXFQS_TFQPI_Msk) >> MCAN_TXFQS_TFQPI_Pos;

        struct mcan_tx_element tx_element;
        mcan_get_tx_buffer_element_defaults(&tx_element);

        tx_element.T0.reg |= MCAN_TX_ELEMENT_T0_EXTENDED_ID(id_value) | MCAN_TX_ELEMENT_T0_XTD;
        tx_element.T1.bit.DLC = data_length;

        for (uint32_t i = 0; i < data_length; i++) {
            tx_element.data[i] = data[i];
        }

        mcan_set_tx_buffer_element(mcan_mod, &tx_element, put_index);
        mcan_tx_transfer_request(mcan_mod, (1 << put_index));

        return true;

    }


    KsResult ComponentCanDriver::Read(struct mcan_module* mcan_mod, uint32_t* id_value, uint8_t* data, uint8_t* data_length)
    {
        uint32_t status = mcan_rx_get_fifo_status(mcan_mod, CONF_MCAN0_RX_FIFO_0_NUM);

        uint32_t num_elements = status & MCAN_RXF1S_F1FL_Msk;
        uint32_t get_index = (status & MCAN_RXF1S_F1GI_Msk) >> MCAN_RXF1S_F1GI_Pos;

        struct mcan_rx_element_fifo_1 rx_element;

        if (num_elements > 0) {
            mcan_get_rx_fifo_1_element(mcan_mod, &rx_element, get_index);
            mcan_rx_fifo_acknowledge(mcan_mod, CONF_MCAN0_RX_FIFO_0_NUM, get_index);

            *id_value = rx_element.R0.bit.ID;
            if( rx_element.R1.bit.DLC < *data_length ) {
                *data_length = rx_element.R1.bit.DLC;
            }

            for (size_t i = 0; i < *data_length; i++) {
                data[i] = rx_element.data[i];
            }

            printf("%d",data_length);

            return true;
        }

        return false;
    }
}

