#pragma once

#include "ks_io.h"
#include "ks_module.h"
#include "ks_component_active.h"

namespace kronos {
    class Bus;

    class HealthMonitor;

    //! \class Framework
    //! \brief A class that implements the framework using the Singleton design pattern
    class Framework {
        friend class HealthMonitor;

    KS_SINGLETON(Framework);

    public:
        //! \brief Constructor that creates the instance of the Framework Singleton
        Framework() = default;

        //! \brief Destructor that deletes the instance to the Framework Singleton
        ~Framework() = default;

    public:
        //! \brief Convenience method for static calls. See _AddModule().
        template<typename T, typename... Args>
        static inline void AddModule(Args&& ... args) {
            return s_Instance->_AddModule<T>(std::forward<Args>(args)...);
        }

        //! \brief Convenience method for static calls. See _CreateComponent().
        template<typename T, typename... Args>
        static inline T* CreateComponent(const String& name, Args&& ... args) {
            return s_Instance->_CreateComponent<T, Args...>(name, std::forward<Args>(args)...);
        }

        //! \brief Convenience method for static calls. See _CreateSingletonComponent().
        template<typename T>
        static inline void CreateSingletonComponent() {
            return s_Instance->_CreateSingletonComponent<T>();
        }

        //! \brief Convenience method for static calls. See _CreateBus().
        template<typename T=Bus, typename... Args>
        static inline T* CreateBus(const String& name, Args&& ... args) {
            return s_Instance->_CreateBus<T, Args...>(name, std::forward<Args>(args)...);
        }

        //! \brief Convenience method for static calls. See _CreateDriver().
        template<typename T, typename... Args>
        static inline T* CreateDriver(const String& name, Args&& ... args) {
            return s_Instance->_CreateDriver<T, Args...>(name, std::forward<Args>(args)...);
        }

        template<class T>
        static inline EventMessage* CreateEventMessage(T&& data, KsEventCodeType eventCode, Bus* returnBus = nullptr) {
            return s_Instance->_CreateEventMessage<T>(std::forward<T>(data), eventCode, returnBus);
        }

        template<class T>
        static inline EventMessage*
        CreateEventMessage(const T& data, KsEventCodeType eventCode, Bus* returnBus = nullptr) {
            return s_Instance->_CreateEventMessage<T>(data, eventCode, returnBus);
        }

        //! \brief Convenience method for static calls. See _HasModule().
        template<typename T>
        static inline bool HasModule() {
            return s_Instance->_HasModule<T>();
        }

    public:
        //! \brief Convenience method for static calls. See _Start().
        KS_SINGLETON_EXPOSE_METHOD(_Start, void Start());

        //! \brief Convenience method for static calls. See _InitModules().
        KS_SINGLETON_EXPOSE_METHOD(_InitModules, bool InitModules());

        KS_SINGLETON_EXPOSE_METHOD(_GetBus, Bus* GetBus(const String& name), name);

        KS_SINGLETON_EXPOSE_METHOD(_GetDriver, IoDriver* GetDriver(const String& name), name);

        KS_SINGLETON_EXPOSE_METHOD(_CreateEventMessage,
                                   EventMessage*
                                       CreateEventMessage(KsEventCodeType eventCode, Bus * returnBus = nullptr),
                                   eventCode,
                                   returnBus);

        KS_SINGLETON_EXPOSE_METHOD(_DeleteEventMessage,
                                   void DeleteEventMessage(const EventMessage* eventMessage),
                                   eventMessage);

    private:
        //! \brief Initializes all the components and starts the FreeRTOS sched
        void _Start();

        //! \brief Initializes and adds a module to the framework.
        //!
        //! \tparam T The module class type.
        //! \tparam Args The types of the constructor arguments required to instantiate the module class.
        //! \param args The constructor arguments required to instantiate the module class.
        template<typename T, typename... Args>
        void _AddModule(Args&& ... args) {
            static_assert(std::is_base_of_v<IModule, T>, "T must extend IModule!");

            if (_HasModule<T>()) {
                // Module already added
                return;
            }

            // Initializing Module
            auto id = ClassID<T>();
            auto ptr = new T(std::forward<Args>(args)...);
            m_Modules[id] = Scope<T>(ptr);
        }

        //! \brief Creates a new component and registers it into the framework.
        //!
        //! \tparam T The component class type.
        //! \tparam Args The types of the constructor arguments required to instantiate the component class.
        //! \param name The name of the new component (must be different from existing components).
        //! \param args The constructor arguments required to instantiate the component class.
        //! \return The component that was created, nullptr if there was an error.
        template<class T, typename... Args>
        T* _CreateComponent(const std::string& name, Args&& ... args) {
            static_assert(std::is_base_of_v<ComponentBase, T>, "T must extend ComponentBase!");

            if (m_Components.contains(name)) {
                // Component already created
                return nullptr;
            }

            auto ref = CreateRef<T>(name, std::forward<Args>(args)...);
            m_Components[name] = ref;

            if constexpr (std::is_base_of_v<ComponentActive, T> && !std::is_base_of_v<HealthMonitor, T>) {
                m_ActiveComponents.push_back(name);
            }

            return ref.get();
        }

        template<typename T>
        void _CreateSingletonComponent() {
            static_assert(std::is_base_of_v<ComponentBase, T>, "T must extend ComponentBase!");

            T::CreateInstance();
            auto ref = T::GetInstanceRef();
            const auto& name = ref->GetName();

            if (m_Components.contains(name)) {
                // Component already created
                return;
            }

            m_Components[name] = ref;
            if constexpr (std::is_base_of_v<ComponentActive, T> && !std::is_base_of_v<HealthMonitor, T>) {
                m_ActiveComponents.push_back(name);
            }
        }

        //! \brief Creates a new bus and registers it into the framework.
        //!
        //! \tparam T The bus class type.
        //! \tparam Args The types of the constructor arguments required to instantiate the bus class.
        //! \param name The name of the new component (must be different from existing busses).
        //! \param args The constructor arguments required to instantiate the bus class.
        //! \return The bus that was created, nullptr if there was an error.
        template<class T, typename... Args>
        T* _CreateBus(const std::string& name, Args&& ... args) {
            static_assert(std::is_base_of_v<Bus, T>, "T must extend Bus!");

            if (m_Busses.contains(name)) {
                // Bus already created
                return nullptr;
            }

            auto ref = CreateRef<T>(name, std::forward<Args>(args)...);
            m_Busses[name] = ref;
            return ref.get();
        }

        template<class T, typename... Args>
        T* _CreateDriver(const String& name, Args&& ... args) {
            static_assert(std::is_base_of_v<IoDriver, T>, "T must extend IoDriver!");

            if (m_Drivers.contains(name)) {
                // Bus already created
                return nullptr;
            }

            auto ref = CreateRef<T>(std::forward<Args>(args)...);
            m_Drivers[name] = ref;
            return ref.get();
        }

        template<class T>
        EventMessage* _CreateEventMessage(T&& data, KsEventCodeType eventCode, Bus* returnBus = nullptr) {
            Scope <EventMessage> eventMessage = std::make_unique<EventMessage>();
            eventMessage->eventCode = eventCode;
            eventMessage->data = std::make_any<T>(std::forward<T>(data));
            eventMessage->returnBus = returnBus;

            auto* eventMessagePtr = eventMessage.get();
            m_EventMessages.emplace(eventMessagePtr, std::forward<Scope < EventMessage>>
            (eventMessage));
            return eventMessagePtr;
        }

        template<class T>
        EventMessage* _CreateEventMessage(const T& data, KsEventCodeType eventCode, Bus* returnBus = nullptr) {
            Scope <EventMessage> eventMessage = std::make_unique<EventMessage>();
            eventMessage->eventCode = eventCode;
            eventMessage->data = std::make_any<T>(data);
            eventMessage->returnBus = returnBus;

            auto* eventMessagePtr = eventMessage.get();
            m_EventMessages.emplace(eventMessagePtr, std::forward<Scope < EventMessage>>
            (eventMessage));
            return eventMessagePtr;
        }

        EventMessage* _CreateEventMessage(KsEventCodeType eventCode, Bus* returnBus = nullptr);

        void _DeleteEventMessage(const EventMessage* eventMessage);

        //!
        //! \tparam T
        //! \return
        template<typename T>
        bool _HasModule() {
            return m_Modules.contains(ClassID<T>());
        }

        //! \brief
        //!
        //! \param moduleList
        //! \return
        bool _InitModules();

        Bus* _GetBus(const String& name);

        IoDriver* _GetDriver(const String& name);

    private:
        List <KsIdType> m_ModuleList;
        Map <KsIdType, Scope<IModule>> m_Modules;
        Map <String, Ref<ComponentBase>> m_Components;
        List <String> m_ActiveComponents;
        Map <String, Ref<Bus>> m_Busses;
        Map <String, Ref<IoDriver>> m_Drivers;
        Map<const EventMessage*, Scope < EventMessage>> m_EventMessages;

    };

}
