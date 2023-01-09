#include "kronos.h"

int main() {
    kronos::Framework::Init();
    auto* framework = new kronos::Framework();



    framework->Run();
    return kronos::ks_success;
}