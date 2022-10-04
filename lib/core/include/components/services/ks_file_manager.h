#pragma once

#include "ks_component_passive.h"
#include "ks_file.h"
#include "ks_framework.h"
#include "s25fl1xx.h"

namespace kronos {

    //! \class ComponentFileManager
    //! \brief
    class ComponentFileManager : public ComponentPassive {
    public:
        ComponentFileManager(const std::string& componentName, const std::string& volume);

        ComponentFileManager(const ComponentFileManager&) = delete;

        void operator=(const ComponentFileManager&) = delete;

        KsResult Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        File* Open(const std::string& path, uint32_t mode);

        static ComponentFileManager& Get();

    private:
        std::string m_Volume;

        struct qspid_t g_qspid = {QSPI, 0, 0, 0};
        struct qspi_config_t mode_config = {static_cast<qspi_run_mode>(QSPI_MR_SMM_MEMORY), false, false, QSPI_LASTXFER, 0, 0, 0, 0, 0, 0, 0, false, false, 0};

    };

}