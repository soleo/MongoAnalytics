
#include <string>
#include <boost/version.hpp>

#include "mongo/util/version.h"

namespace mongo {
    const char * gitVersion() { return "7ea27bae26b61e16439a36431e71d030842acd2c"; }
    const char * compiledJSEngine() { return "V8"; }
    const char * allocator() { return "tcmalloc"; }
    const char * loaderFlags() { return "-fPIC -pthread -rdynamic"; }
    const char * compilerFlags() { return "-Wnon-virtual-dtor -Woverloaded-virtual -fPIC -fno-strict-aliasing -ggdb -pthread -Wall -Wsign-compare -Wno-unknown-pragmas -Winvalid-pch -Werror -pipe -fno-builtin-memcmp -O3"; }
    std::string sysInfo() { return "Linux ip-10-2-29-40 2.6.21.7-2.ec2.v1.2.fc8xen #1 SMP Fri Nov 20 17:48:28 EST 2009 x86_64 BOOST_LIB_VERSION=" BOOST_LIB_VERSION ; }
}  // namespace mongo
