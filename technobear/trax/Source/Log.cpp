#include "Log.h"

#include <juce_core/juce_core.h>

#ifdef __APPLE__
#include <sstream>
#include <thread>
void log(const std::string &m) {
    std::stringstream msg;
    msg << std::this_thread::get_id() << " : " << m;
    juce::Logger::writeToLog(msg.str());
}
#else
#include <fstream>
#include <thread>
void log(const std::string &m) {
    std::ofstream s("/dev/kmsg");
    s << std::this_thread::get_id() << " : " << m << std::endl;
}
#endif
