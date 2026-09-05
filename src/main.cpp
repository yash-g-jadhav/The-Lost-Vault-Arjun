#include "core/Application.h"
#include "core/Log.h"

int main() {
    Application app;
    if (!app.Initialize(1280, 720, "The Lost Vault")) {
        LOG_ERROR("Main", "Application initialization failed");
        return -1;
    }

    app.Run();
    app.Shutdown();
    return 0;
}
