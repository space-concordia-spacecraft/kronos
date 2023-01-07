#pragma once

// Kronos
#include "kronos.h"

// ASF
#include "driver_init.h"
#include "stdio_start.h"

namespace kronos {
    //! \class Framework
    //! \brief A class that implements the framework using the Singleton design pattern
    class Framework {
    public:
        //! \brief Constructor that creates the instance of the Framework Singleton
        Framework();

        //! \brief Destructor that deletes the instance to the Framework Singleton
        ~Framework();

        //! \brief Initializes framework utilities such as the console
        static void Init();

        //! \brief Initializes all the components and starts the FreeRTOS scheduler
        void Run();

        //! \brief Creates a component into the framework
        //!
        //! \param component ComponentBase pointer to the component being registered
        //! \return the component that was created, nullptr if there was an error.
        template<class T, typename... Args>
        T* CreateComponent(const std::string& name, Args&& ... args) {
            if (m_Components.count(name))
                return nullptr;

            m_Components[name] = new T(name, args...);
            return reinterpret_cast<T*>(m_Components[name]);
        }

        //!
        template<class T, typename... Args>
        T* CreateBus(const std::string& name, Args&& ... args) {
            if (m_Busses.count(name))
                return nullptr;

            m_Busses[name] = new T(name, args...);
            return reinterpret_cast<T*>(m_Busses[name]);
        }

        //!
        template<class T>
        static T* GetBus(const std::string& name) {
            if (!s_Instance->m_Busses.count(name))
                return nullptr;

            return reinterpret_cast<T*> (s_Instance->m_Busses[name]);
        }

        static Framework* s_Instance;

    private:
        std::unordered_map<std::string, BusBase*> m_Busses;
        std::unordered_map<std::string, ComponentBase*> m_Components;

        BusAsync* m_LoggerBus = nullptr;
    };

}
