// ==================================================================================
// \title ks_component_active.h
// \brief The base for the active component.
// ==================================================================================

#pragma once

// Kronos includes
#include "ks_component_queued.h"

// Kernel includes
#include "FreeRTOS.h"
#include "task.h"

// Task Priorities
#define KS_COMPONENT_PRIORITY_HIGH      ( tskIDLE_PRIORITY + 3 )
#define KS_COMPONENT_PRIORITY_MEDIUM    ( tskIDLE_PRIORITY + 2 )
#define KS_COMPONENT_PRIORITY_LOW       ( tskIDLE_PRIORITY + 1 )

// Stack sizes
#define KS_COMPONENT_STACK_SIZE_XLARGE  configMINIMAL_STACK_SIZE * 8
#define KS_COMPONENT_STACK_SIZE_LARGE   configMINIMAL_STACK_SIZE * 4
#define KS_COMPONENT_STACK_SIZE_MEDIUM  configMINIMAL_STACK_SIZE * 2
#define KS_COMPONENT_STACK_SIZE_SMALL   configMINIMAL_STACK_SIZE

namespace kronos {

    //! \class ComponentActive
    //! \brief A class that implements the base of an active component
    //!
    //! This class is used as the basic block for all active components
    class ComponentActive : public ComponentQueued {
    public:
        //! \brief Creates a new activate component
        //!
        //! \param name the name of the component
        //! \param stackSize The number of words (not bytes!) to allocate for use as the task's stack
        //! \param priority the priority at which the created task will execute
        explicit ComponentActive(const std::string& name, size_t stackSize = KS_COMPONENT_STACK_SIZE_SMALL,
                                 uint16_t priority = KS_COMPONENT_PRIORITY_MEDIUM);

        //! @copydoc
        KsResult Init() override;

        //! @copydoc
        KsResult Destroy() override;

        //! Function containing main loop for the event processing thread.
        [[noreturn]] void Run();

        //! @copydoc
        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
        //! Stack size provided to the task
        size_t m_StackSize;
        //! Priority of the task
        uint16_t m_Priority;
        //! Task handle used to manipulate the task created by the FreeRTOS API
        TaskHandle_t m_Task = nullptr;

        //! \brief Starts the thread
        //!
        //! \param data void pointer used to run the thread
        static void Start(void* data);
    };

}

