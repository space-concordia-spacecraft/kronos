#include "ks_string.h"

namespace kronos {
    class File {
    public:
        File();

        void Open();
        void Close();

        void Read();
        void Write();

        void SetName(String& name);
        size_t GetSize();
    private:
        String m_Path;
        String m_Name;

        RedFileHandle m_FileHandler;
    };
}