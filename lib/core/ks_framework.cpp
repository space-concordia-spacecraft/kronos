#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Framework);

    Framework::Framework() {
        // Initialize ASF and system
        atmel_start_init();
    }

    void Framework::_Start() {
        // Log active modules
        if (HasModule<LogModule>()) {
            Logger::Trace(KS_TERM_CYAN);
            Logger::Trace(R"($$\   $$\ $$$$$$$\   $$$$$$\  $$\   $$\  $$$$$$\   $$$$$$\  )");
            Logger::Trace(R"($$ | $$  |$$  __$$\ $$  __$$\ $$$\  $$ |$$  __$$\ $$  __$$\ )");
            Logger::Trace(R"($$ |$$  / $$ |  $$ |$$ /  $$ |$$$$\ $$ |$$ /  $$ |$$ /  \__|)");
            Logger::Trace(R"($$$$$  /  $$$$$$$  |$$ |  $$ |$$ $$\$$ |$$ |  $$ |\$$$$$$\  )");
            Logger::Trace(R"($$  $$<   $$  __$$< $$ |  $$ |$$ \$$$$ |$$ |  $$ | \____$$\ )");
            Logger::Trace(R"($$ |\$$\  $$ |  $$ |$$ |  $$ |$$ |\$$$ |$$ |  $$ |$$\   $$ |)");
            Logger::Trace(R"($$ | \$$\ $$ |  $$ | $$$$$$  |$$ | \$$ | $$$$$$  |\$$$$$$  |)");
            Logger::Trace(R"(\__|  \__|\__|  \__| \______/ \__|  \__| \______/  \______/)");
            Logger::Trace(KS_TERM_RESET);

            Logger::Info("Initializing framework with modules: ");
            for (const auto& module: m_Modules) {
                Logger::Info("-- %s", module.second->GetName().c_str());
            }
        }

        // Init Components
        for (auto& component: m_Components) {
            component.second->Init();
            if (HasModule<LogModule>()) {
                Logger::Info("Initializing component '%s'...", component.second->GetName().c_str());
            }
        }
    }

    void Framework::_AddDefaultModules() {

    }

}