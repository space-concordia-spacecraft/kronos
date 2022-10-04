#include "ks_file_manager.h"

namespace kronos {

    ComponentFileManager::ComponentFileManager(const std::string& componentName, const std::string& volume)
            : ComponentPassive(
            componentName), m_Volume(volume) {}

    KsResult ComponentFileManager::Init() {
        ComponentPassive::Init();

        // Initialize Flash
        uint8_t mem_verified = 0;
        uint32_t __start_sp, idx;
        uint32_t (*__start_new)(void);
        uint32_t buffer[4];

        uint8_t *memory = (uint8_t *)QSPIMEM_ADDR;
        enum status_code status = STATUS_OK;

        /* Enable SMC peripheral clock */
        pmc_enable_periph_clk(ID_QSPI);

        /* QSPI memory mode configure */
        status = s25fl1xx_initialize(g_qspid.qspi_hw, &mode_config, 1);
        if (status == STATUS_OK) {
            Framework::LogInfo("QSPI driver initialized");
        } else {
            Framework::LogError("QSPI drivers initialize failed");
            return KS_ERROR_QSPI_DRIVER_INIT_FAILED;
        }

        /* Enable quad mode */
        s25fl1xx_set_quad_mode(&g_qspid, 1);

//        /* Flash the code to QSPI flash */
//        puts("Writing to Memory\r\n");
//
//        s25fl1xx_write(&g_qspid, (uint32_t *)buffercode, WRITE_SIZE, 0, 0);
//
//        printf("\rExample code written 0x%x bytes to Memory\r\n", WRITE_SIZE);
//
//        s25fl1xx_read(&g_qspid, buffer, sizeof(buffer), 0);
//        /* Start continuous read mode to enter in XIP mode*/
//        s25fl1xx_enter_continous_read_mode(&g_qspid);
//
//        puts("Verifying \r\n");
//        for (idx = 0; idx < WRITE_SIZE; idx++) {
//            if(*(memory) == buffercode[idx]) {
//                memory++;
//            } else {
//                mem_verified = 1;
//                printf("\nData does not match at 0x%x \r\n", (int)memory);
//                break;
//            }
//        }
//        if (!mem_verified) {
//            puts("Everything is OK \r\n");
//            /* Set PC and SP */
//            __start_new = (uint32_t(*) (void)) buffer[1];
//            __start_sp = buffer[0];
//
//            puts("\n\r Starting getting started example from QSPI flash \n\r");
//            puts("========================================================= \n\r");
//
//            __set_MSP(__start_sp);
//
//            __start_new();
//        }
//
//        puts("Verified failed \r\n");
//        while(1);

        // Initialize Reliance Edge
        int32_t initResult = red_init();

        if (initResult == KS_SUCCESS)
            initResult = red_format(m_Volume.data());

        if (initResult == KS_SUCCESS)
            initResult = red_mount(m_Volume.data());

        if (initResult != KS_SUCCESS) {
            return KS_ERROR_FILE_UNABLE_TO_INIT;
        }

        return KS_SUCCESS;
    }

    KsCmdResult ComponentFileManager::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_OPEN_FILE:
                auto* fileOpenMsg = reinterpret_cast<FileOpenMessage*>(message.data);
                File* file = Open(fileOpenMsg->path, fileOpenMsg->mode);
                delete fileOpenMsg;
                return file;
        }

        return KS_CMDRESULT_NORETURN;
    }

    File* ComponentFileManager::Open(const std::string& path, uint32_t mode) {
        std::string fullPath = m_Volume + path;
        int32_t fileId = red_open(fullPath.data(), mode);

        if (fileId == KS_FILE_INVALID_HANDLE)
            return nullptr;

        return new File(path, fileId);
    }

    ComponentFileManager& ComponentFileManager::Get() {
        static ComponentFileManager fileManager("File Manager", "C:");
        return fileManager;
    }

}