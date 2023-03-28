#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Framework);

    KsResult Framework::_Start() {
        // Init Components
        for (auto& component: m_Components) {
            KS_TRY(ks_error_component_initialize, component.second->Init());
        }

        // PostInit Components
        for (auto& component: m_Components) {
            KS_TRY(ks_error_component_post_initialize, component.second->PostInit());
        }

        return {};
    }

    KsResult Framework::_InitModules() {
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
            KS_THROW(ks_error_module_cyclic_dependency);
        }

        // Initialize modules
        for (const auto& module: m_ModuleList) {
            KS_TRY(ks_error_module_initialize, m_Modules[module]->Init());
        }

        return {};
    }

    Bus* Framework::_GetBus(const String& name) {
        KS_ASSERT(m_Busses.contains(name), "Bus with name doesn't exist")

        KS_MAP_FIND(m_Busses, name, it) {
            return it->second.get();
        }
        // TODO: Bus should be maybe sent back by ref?
        // KS_THROW(ks_error_bus_missing);
    }

    IoDescriptor* Framework::_GetDescriptor(const String& name) {
        KS_ASSERT(m_Drivers.contains(name), "Driver with name doesn't exist")

        KS_MAP_FIND(m_Drivers, name, it) {
            return it->second.get();
        }
        // TODO: Pass by ref?
        // KS_THROW(ks_error_driver_missing);
    }

    EventMessage* Framework::_CreateEventMessage(KsEventCodeType eventCode, Bus* returnBus) {
        Scope <EventMessage> eventMessage = CreateScope<EventMessage>();
        eventMessage->eventCode = eventCode;
        eventMessage->returnBus = returnBus;

        auto* eventMessagePtr = eventMessage.get();
        m_EventMessages.emplace(
            eventMessagePtr,
            std::forward<Scope<EventMessage>>(eventMessage)
        );
        return eventMessagePtr;
    }

    KsResult Framework::_DeleteEventMessage(const EventMessage* eventMessage) {
        if(m_EventMessages.erase(eventMessage) == 0) KS_THROW(ks_error_event_message_missing);

        return ks_success;
    }

}