#pragma once

#include "ks_component_passive.h"
#include "ks_apollo_format.h"
#include "ks_file.h"
#include "ks_macros.h"
#include "ks_logger.h"

#include <unordered_map>

#define KS_PARAM_DB_FILENAME "/parameters.apl"

namespace kronos {
    //! \class ComponentParameterDatabase Class
    class ParameterDatabase : public ComponentPassive {
    KS_SINGLETON(ParameterDatabase);

    public:
        /**
         *  Constructor for ParameterDatabase. Attempts to read a file called "parameters" and parses the data.
         */
        ParameterDatabase();
        ~ParameterDatabase() = default;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        template<typename T=uint32_t>
        static inline T GetParam(const std::string& key) {
            return s_Instance->_GetParam<T>(key);
        }

        template<typename T=uint32_t>
        static inline bool GetParam(const std::string& key, T* value) {
            return s_Instance->_GetParam<T>(key, value);
        }

        template<typename T=uint32_t>
        static inline void SetParam(const std::string& key, T newValue) {
            return s_Instance->_SetParam<T>(key, newValue);
        }

        KS_SINGLETON_EXPOSE_METHOD(_SaveParams, KsResultType SaveParams());

    private:
        std::unordered_map<std::string, uint32_t> m_Parameters;

        template<typename T>
        T _GetParam(const std::string& key) {
            T value{};
            _GetParam(key, &value);
            return value;
        }

        template<typename T>
        bool _GetParam(const std::string& key, T* value) {
            static_assert(sizeof(T) == sizeof(uint32_t), "gay");
            if(!m_Parameters.count(key)) {
                Logger::Warn("Non existing key: '%s'", key.c_str());
                return false;
            }

            *value = *reinterpret_cast<T*>(&m_Parameters[key]);
            return true;
        }

        //! \brief Function to set Parameter
        //! \param key is the Key to be added
        //! \param newValue is the value to be added
        //! \return KS_SUCCESS if true else otherwise
        template<typename T>
        void _SetParam(const std::string& key, T newValue) {
            static_assert(sizeof(T) == sizeof(uint32_t), "gay");

            m_Parameters[key] = *reinterpret_cast<uint32_t*>(&newValue);
        }

        //! \brief Function to Save Parameter
        //! \param key is the Key to be added
        //! \return KS_SUCCESS if successful, else otherwise
        KsResultType _SaveParams();
    };

}