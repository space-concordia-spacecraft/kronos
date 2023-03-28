#pragma once

#include "ks_component_queued.h"
#include "ks_apollo_format.h"
#include "ks_file.h"

#define KS_PARAM_DB_FILENAME "/params.apl"

namespace kronos {

    //! \class ComponentParameterDatabase Class
    class ParameterDatabase : public ComponentQueued {
    KS_SINGLETON(ParameterDatabase);

    public:
        //! \brief Constructor for ParameterDatabase. Attempts to read a file called "params" and parses the data.
        ParameterDatabase();

        KsResult Init() override;
        KsResult ProcessEvent(const EventMessage& message) override;

    public:
        template<typename T = uint32_t>
        static inline T GetParam(const String& key) {
            return s_Instance->_GetParam<T>(key);
        }

        template<typename T = uint32_t>
        static inline bool GetParam(const String& key, T* value) {
            return s_Instance->_GetParam<T>(key, value);
        }

        template<typename T = uint32_t>
        static inline void SetParam(const String& key, T newValue) {
            return s_Instance->_SetParam<T>(key, newValue);
        }

        KS_SINGLETON_EXPOSE_METHOD(_SaveParams, KsResult SaveParams());

    private:
        template<typename T>
        T _GetParam(const String& key) {
            T value{};
            KS_TRY(ks_error, _GetParam(key, &value));
            return value;
        }

        template<typename T>
        KsResult _GetParam(const String& key, T* value) {
            static_assert(sizeof(T) == sizeof(uint32_t), "Type T has to be of size uint32_t.");
            if (!m_Parameters.count(key)) {
                KS_THROW(ks_error);
            }

            *value = *reinterpret_cast<T*>(&m_Parameters[key]);
        }

        //! \brief Function to set Parameter
        //! \param key is the Key to be added
        //! \param newValue is the value to be added
        //! \return KS_SUCCESS if true else otherwise
        template<typename T>
        void _SetParam(const String& key, T newValue) {
            static_assert(sizeof(T) == sizeof(uint32_t), "Type T has to be of size uint32_t.");

            m_Parameters[key] = *reinterpret_cast<uint32_t*>(&newValue);
        }

        KsResult _SaveParams();

    private:
        Map <String, uint32_t> m_Parameters;
        ApolloExporter m_Exporter;

    };

}