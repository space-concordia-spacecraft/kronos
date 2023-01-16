#pragma once

#include "ks_component_base.h"

namespace kronos {

    //! \brief
    class IModule {

    public:
        //! \brief
        virtual ~IModule() = default;

        //! \brief
        virtual void Init() const = 0;

        //! \brief
        //!
        //! \return
        [[nodiscard]] virtual TypeInfo GetModuleInfo() const = 0;

        //! \brief
        //!
        //! \return
        [[nodiscard]] virtual List<TypeInfo> GetModuleDependencies() const = 0;

        //! \brief
        //!
        //! \return
        [[nodiscard]] virtual List<TypeInfo> GetExportedComponents() const = 0;

        //! \brief
        //!
        //! \return
        [[nodiscard]] virtual StringView GetName() const = 0;

    };

    //! \brief
    template<typename ModuleClass>
    class Module : public IModule {

    public:
        //! \copydoc
        void Init() const override {}

        //! \copydoc
        [[nodiscard]] TypeInfo GetModuleInfo() const final {
            return GetTypeInfo<ModuleClass>();
        }

        //! \copydoc
        [[nodiscard]] List<TypeInfo> GetModuleDependencies() const override {
            return {};
        }

        //! \copydoc
        [[nodiscard]] List<TypeInfo> GetExportedComponents() const override {
            return {};
        }

        //! \copydoc
        [[nodiscard]] StringView GetName() const override {
            return NAMEOF_SHORT_TYPE(ModuleClass);
        }

        //! \brief
        //!
        //! \tparam Dependencies
        //! \return
        template<typename... Dependencies>
        static inline List<TypeInfo> DependsOn() {
            static_assert(
                std::conjunction_v<std::is_base_of<IModule, Dependencies>...>,
                "Dependencies must extend IModule!"
            );
            return {GetTypeInfo<Dependencies>()...};
        }

        //! \brief
        //!
        //! \tparam Components
        //! \return
        template<typename... Components>
        static inline List<TypeInfo> ExportComponents() {
            static_assert(
                std::conjunction_v<std::is_base_of<ComponentBase, Components>...>,
                "Components must extend ComponentBase!"
            );
            return {GetTypeInfo<Components>()...};
        }

    };

}