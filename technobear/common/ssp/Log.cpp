#include "Log.h"

#include <juce_core/juce_core.h>

#ifdef __APPLE__
#include <sstream>
#else
#include <fstream>
#endif
#include <thread>

namespace ssp {

#ifdef __APPLE__
void log(const std::string &m) {
    std::stringstream msg;
    msg << std::this_thread::get_id() << " : " << m;
    juce::Logger::writeToLog(msg.str());
}
#else
void log(const std::string &m) {
    std::ofstream s("/dev/kmsg");
    s << std::this_thread::get_id() << " : " << m << std::endl;
}
#endif

}  // namespace ssp
