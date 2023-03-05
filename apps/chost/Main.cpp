#include <juce_core/juce_core.h>


#include "PluginHost.h"

int main (int argc, char* argv[])
{
    PluginHost host(argc,argv);

    std::cerr << "Ready to exit?" << std::endl;
    std::cin.get();

    return 0;
}
