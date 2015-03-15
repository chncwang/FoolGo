#include "log_util.h"

#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>

using log4cplus::initialize;
using log4cplus::PropertyConfigurator;

namespace foolgo {
namespace util {

void InitLogConfig() {
  initialize();
  PropertyConfigurator::doConfigure("log4cplus.properties");
}

}
}
