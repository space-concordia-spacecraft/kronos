#include "kronos.h"

using namespace kronos;

int main() {
    Framework::Init();
    // REGISTER COMPONENTS HERE

    Framework::Run();
    return ks_success;
}