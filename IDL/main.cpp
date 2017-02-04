#include <Core/Context.h>
#include <Binding/NidiumJS.h>
#include <ape_netlib.h>
#include "JSRobot.h"

unsigned long _ape_seed = time(NULL) ^ (getpid() << 16);

using namespace Nidium::Core;
using namespace Nidium::Binding;

int main() {
    ape_global *net = APE_init();
    Context ctx(net);
    NidiumJS *njs = ctx.getNJS();

    JSRobot::RegisterObject(njs->getJSContext());

    njs->LoadScript("../robot.js");
}
