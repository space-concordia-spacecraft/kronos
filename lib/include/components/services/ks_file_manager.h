#include "ks_component_active.h"

#include "redposix.h"

namespace kronos {
    class ComponentFileManager : public ComponentActive {
    public:
        ComponentFileManager(String & componentName, String & volume);

        void Init() override;

        void ProcessCommand(const CommandMessage& message) override;

        KsResult CreateFile();
        KsResult RemoveFile();

        KsResult CreateDirectory();
        KsResult RemoveDirectory();

        KsResult Mount();
        KsResult Unmount();
    };
}