#pragma once

// Kronos
#include "kronos.h"

// ASF
#include "driver_init.h"
#include "stdio_start.h"

#define KS_DEFAULT_TIMER_INTERVAL 100

namespace kronos {
    typedef TickType_t KsTickType;

    //! \class Framework
    //! \brief A class that implements the framework using the Singleton design pattern
    class Framework {
    KS_SINGLETON(Framework);

    public:
        //! \brief Convenience method for static calls. See _CreateComponent().
        template<class T, typename... Args>
        static inline T* CreateComponent(const std::string& name, Args&& ... args) {
            return s_instance->template _CreateComponent<T, Args...>(name, std::forward<Args...>(args)...);
        }

        //! \brief Convenience method for static calls. See _CreateBus().
        template<class T, typename... Args>
        static inline T* CreateBus(const std::string& name, Args&& ... args) {
            return s_instance->_CreateBus<T, Args...>(name, std::forward<Args...>(args)...);
        }

        //! \brief Convenience method for static calls. See _Run().
        KS_SINGLETON_EXPOSE_METHOD(_Run, void Run());

    public:
        //! \brief Constructor that creates the instance of the Framework Singleton
        Framework();

        //! \brief Destructor that deletes the instance to the Framework Singleton
        ~Framework();

    private:
        //! \brief Initializes all the components and starts the FreeRTOS scheduler
        void _Run();

        //! \brief Creates a new component and registers it into the framework.
        //!
        //! \tparam T The component class type.
        //! \tparam Args The types of the constructor arguments required to instantiate the component class.
        //! \param name The name of the new component (must be different from existing components).
        //! \param args The constructor arguments required to instantiate the component class.
        //! \return The component that was created, nullptr if there was an error.
        template<class T, typename... Args>
        T* _CreateComponent(const std::string& name, Args&& ... args) {
            if (m_Components.count(name))
                return nullptr;

            m_Components[name] = new T(name, args...);

            if constexpr (std::is_base_of_v<ComponentActive, T>) {
                m_HealthMonitor.RegisterActiveComponent(reinterpret_cast<T*>(m_Components[name]));
            }

            return reinterpret_cast<T*>(m_Components[name]);
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
            if (m_Busses.count(name))
                return nullptr;

            m_Busses[name] = new T(name, std::forward<Args...>(args)...);
            return reinterpret_cast<T*>(m_Busses[name]);
        }

    private:
        std::unordered_map<std::string, BusBase*> m_Busses;
        std::unordered_map<std::string, ComponentBase*> m_Components;

        BusAsync m_BusTick;

        ComponentHealthMonitor m_HealthMonitor;
    };

}
