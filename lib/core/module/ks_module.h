#pragma once

namespace kronos {

    //! \brief
    class Module {

    public:
        //! \brief
        //!
        //! \param name
        explicit Module(String name);

        //! \brief
        virtual ~Module() = default;

        //! \brief
        virtual void Init() const;

        //! \brief
        [[nodiscard]] const String& GetName() const;

    private:
        String m_Name;

    };

}