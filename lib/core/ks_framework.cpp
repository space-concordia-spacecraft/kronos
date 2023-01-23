#include "ks_framework.h"
#include "ks_log_module.h"
#include "ks_logger.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Framework);

    void Framework::_Start() {
        // Init Components
        for (auto& component: m_Components) {
            component.second->Init();
        }

        // Init Components
        for (auto& component: m_Components) {
            component.second->PostInit();
        }

        // Log active modules
//        if (_HasModule<LogModule>()) {
//            Logger::Trace(KS_TERM_CYAN);
//            Logger::Trace(R"($$\   $$\ $$$$$$$\   $$$$$$\  $$\   $$\  $$$$$$\   $$$$$$\  )");
//            Logger::Trace(R"($$ | $$  |$$  __$$\ $$  __$$\ $$$\  $$ |$$  __$$\ $$  __$$\ )");
//            Logger::Trace(R"($$ |$$  / $$ |  $$ |$$ /  $$ |$$$$\ $$ |$$ /  $$ |$$ /  \__|)");
//            Logger::Trace(R"($$$$$  /  $$$$$$$  |$$ |  $$ |$$ $$\$$ |$$ |  $$ |\$$$$$$\  )");
//            Logger::Trace(R"($$  $$<   $$  __$$< $$ |  $$ |$$ \$$$$ |$$ |  $$ | \____$$\ )");
//            Logger::Trace(R"($$ |\$$\  $$ |  $$ |$$ |  $$ |$$ |\$$$ |$$ |  $$ |$$\   $$ |)");
//            Logger::Trace(R"($$ | \$$\ $$ |  $$ | $$$$$$  |$$ | \$$ | $$$$$$  |\$$$$$$  |)");
//            Logger::Trace(R"(\__|  \__|\__|  \__| \______/ \__|  \__| \______/  \______/)");
//            Logger::Trace(KS_TERM_RESET);
//
//            Logger::Info("Initialized framework with %u module(s): ", m_ModuleList.size());
//            for (const auto& module: m_ModuleList) {
//                Logger::Info("-- %s", m_Modules[module]->GetName().data());
//            }
//        }
        Logger::Info("Test");
    }

    bool Framework::_InitModules() {
        Map <KsIdType, Set<KsIdType>> moduleParents;
        Map <KsIdType, Set<KsIdType>> moduleChildren;

        Stack <KsIdType> moduleStack;

        // Create dependency graph
        for (const auto&
            [id, module]: m_Modules) {
            const auto& dependencies = module->GetModuleDependencies();
            for (const auto& dependency: dependencies) {
                moduleParents[id].insert(dependency.id);
                moduleChildren[dependency.id].insert(id);
            }
        }

        // Push modules with no dependencies on the stack
        for (const auto&
            [id, module]: m_Modules) {
            if (moduleParents[id].empty()) {
                moduleStack.push(id);
            }
        }

        // Loop while we still have modules in the stack
        while (!moduleStack.empty()) {
            // Pop a module from the stack
            KsIdType module = moduleStack.top();
            moduleStack.pop();

            // Add the module to the list
            m_ModuleList.push_back(module);

            // Loop over each module that has a dependency on this module
            for (auto& child: moduleChildren[module]) {
                // Remove the dependency from the graph
                moduleParents[child].erase(module);

                // If the child module has no dependencies left to resolve, push it on the stack
                if (moduleParents[child].empty()) {
                    moduleStack.push(child);
                }
            }
        }

        // Cyclic dependency check
        if (std::any_of(
            moduleParents.begin(),
            moduleParents.end(),
            [](const auto& pair) { return !pair.second.empty(); }
        )) {
            return false;
        }

        // Initialize modules
        for (const auto& module: m_ModuleList) {
            m_Modules[module]->Init();
        }

        return true;
    }

    Bus* Framework::_GetBus(const String& name) {
        KS_ASSERT(m_Busses.contains(name), "Bus with name doesn't exist");

        KS_MAP_FIND(m_Busses, name, it) {
            return it->second.get();
        }
        return nullptr;
    }

    IoDriver* Framework::_GetDriver(const String& name) {
        KS_ASSERT(m_Drivers.contains(name), "Driver with name doesn't exist");

        KS_MAP_FIND(m_Drivers, name, it) {
            return it->second.get();
        }
        return nullptr;
    }

    EventMessage* Framework::_CreateEventMessage(KsEventCodeType eventCode, Bus* returnBus) {
        Scope <EventMessage> eventMessage = std::make_unique<EventMessage>();
        eventMessage->eventCode = eventCode;
        eventMessage->returnBus = returnBus;

        auto* eventMessagePtr = eventMessage.get();
        m_EventMessages.emplace(
            eventMessagePtr,
            std::forward<Scope<EventMessage>>(eventMessage)
        );
        return eventMessagePtr;
    }

    void Framework::_DeleteEventMessage(const EventMessage* eventMessage) {
        m_EventMessages.erase(eventMessage);
    }

}