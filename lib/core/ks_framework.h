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

        //! \brief Registers a component into the framework
        //!
        //! \param component ComponentBase pointer to the component being registered
        //! \return KS_SUCCESS if the operation was successful
        KsResult RegisterComponent(ComponentBase* component);

        //! \brief Gets the component with the given name
        //!
        //! \param name The name of the component
        //! \param component ComponentBase pointer used to store the component found
        //! \return KS_SUCCESS if the operation was successful
        static KsResult GetComponent(const std::string& name, ComponentBase** component);

        //! \brief Registers a synchronous bus to the framework
        //!
        //! \param bus BusSync object to register to the framework
        //! \return KS_SUCCESS if the operation was successful
        KsResult RegisterBus(BusSync* bus);

        //! \brief Registers an asynchronous bus to the framework
        //!
        //! \param bus BusAsync object to register to the framework
        //! \return KS_SUCCESS if the operation was successful
        KsResult RegisterBus(BusAsync* bus);

        //! \brief Getter for synchronous buses
        //!
        //! \param name The name of the bus that needs to get retrieved
        //! \param bus BusSync pointer to store the bus found in the list
        //! \return KS_SUCCESS if the operation was successful
        static KsResult GetSyncBus(const std::string& name, BusSync** bus);

        //! \brief Getter for asynchronous buses
        //!
        //! \param name The name of the bus that needs to get retrieved
        //! \param bus BusAsync pointer to store the bus found in the list
        //! \return KS_SUCCESS if the operation was successful
        static KsResult GetAsyncBus(const std::string& name, BusAsync** bus);

        static Framework* s_Instance;

    private:
        std::unordered_map<std::string, BusSync*> m_SyncBuses;
        std::unordered_map<std::string, BusAsync*> m_AsyncBuses;
        std::unordered_map<std::string, ComponentBase*> m_Components;
    };

}
