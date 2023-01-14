#pragma once

// Kronos
#include "kronos.h"

namespace kronos {

    //! \class Framework
    //! \brief A class that implements the framework using the Singleton design pattern
    class Framework {
    KS_SINGLETON(Framework);

    public:
        //! \brief Constructor that creates the instance of the Framework Singleton
        Framework();

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
        static inline T* CreateComponent(const std::string& name, Args&& ... args) {
            return s_Instance->_CreateComponent<T, Args...>(name, std::forward<Args>(args)...);
        }

        //! \brief Convenience method for static calls. See _CreateBus().
        template<typename T, typename... Args>
        static inline T* CreateBus(const std::string& name, Args&& ... args) {
            return s_Instance->_CreateBus<T, Args...>(name, std::forward<Args>(args)...);
        }

        //! \brief Convenience method for static calls. See _HasModule().
        template<typename T>
        static inline bool HasModule() {
            return s_Instance->template _HasModule<T>();
        }

        //! \brief Convenience method for static calls. See _Run().
        KS_SINGLETON_EXPOSE_METHOD(_Run, void Run());

    private:
        //! \brief Initializes all the components and starts the FreeRTOS sched
        void _Run();

        //! \brief Initializes and adds a module to the framework.
        //!
        //! \tparam T The module class type.
        //! \tparam Args The types of the constructor arguments required to instantiate the module class.
        //! \param args The constructor arguments required to instantiate the module class.
        template<typename T, typename... Args>
        void _AddModule(Args&& ... args) {
            static_assert(std::is_base_of_v<Module, T>, "T must extend Module!");

            auto id = ClassID<T>();
            auto ptr = new T(std::forward<Args>(args)...);
            const String& name = ptr->GetName();

            if (m_Modules.contains(id)) {
                Logger::Warn("Module '{}' already added. Ignoring subsequent addition.", name);
                delete ptr;
                return;
            }

            Logger::Info("Initializing module '{}'...", name);
            ptr->Init();
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
                Logger::Warn("Component '{}' already created. Ignoring subsequent creation.", name);
                return nullptr;
            }

            auto ref = CreateRef<T>(name, std::forward<Args>(args)...);
            m_Components[name] = ref;

            if constexpr (std::is_base_of_v<ComponentActive, T>) {
                if (_HasModule<HealthModule>()) {
                    Logger::Info("Component '{}' detected as ACTIVE. Linking to Health Monitor.", name);
                    HealthMonitor::GetInstance().RegisterActiveComponent(ref);
                }
            }

            return ref.get();
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
            static_assert(std::is_base_of_v<BusBase, T>, "T must extend BusBase!");

            if (m_Busses.contains(name)) {
                Logger::Warn("Bus '{}' already created. Ignoring subsequent creation.", name);
                return nullptr;
            }

            auto ref = CreateRef<T>(name, std::forward<Args>(args)...);
            m_Busses[name] = ref;
            return ref.get();
        }

        //!
        //! \tparam T
        //! \return
        template<typename T>
        bool _HasModule() {
            return m_Modules.contains(ClassID<T>());
        }

    private:
        Map <KsIdType, Scope<Module>> m_Modules;
        Map <String, Ref<ComponentBase>> m_Components;
        Map <String, Ref<BusBase>> m_Busses;

    };

}
