#include "jsapi.h"
#include "js/Initialization.h"

struct Robot {
    char *name;
};

static bool robot_constructor(JSContext *cx, unsigned argc, JS::Value *vp);
static bool robot_echo(JSContext *cx, unsigned argc, JS::Value *vp);
static void robot_finalizer(JSFreeOp *fop, JSObject *obj);

static JSClass Robot_class = {
    "Robot", JSCLASS_HAS_PRIVATE,
    nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, robot_finalizer,
};

static JSFunctionSpec Robot_funcs[] = {
    JS_FN("echo", robot_echo, 1, JSPROP_ENUMERATE | JSPROP_PERMANENT),
    JS_FS_END
};

static bool robot_constructor(JSContext *cx, unsigned argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (!args.isConstructing()) {
        JS_ReportError(cx, "Bad constructor");
        return false;
    }

    if (argc != 1 || !args[0].isString()) {
        JS_ReportError(cx, "Invalid arguments");
        return false;
    }

    // Create the JS object for the Robot
    JS::RootedObject robot(cx, JS_NewObjectForConstructor(cx, &Robot_class, args));

    // Create the C object for the Robot
    Robot *nativeRobot = new Robot();

    // Convert the JS string to a C char array
    // and assign the name to the Robot struct
    JSAutoByteString name(cx, args[0].toString());
    if (!name) {
        return false;
    }
    nativeRobot->name = strdup(name.ptr());

    // Store a pointer to the Robot struct
    // inside the JS object for later use
    JS_SetPrivate(robot, nativeRobot);

    // Returns to the JS the newly created object
    args.rval().setObjectOrNull(robot);

    return true;
}

static bool robot_echo(JSContext *cx, unsigned argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

    // Retrieve |this| object
    JS::RootedValue thisValue(cx, args.thisv());
    JS::RootedObject thisObj(cx, thisValue.toObjectOrNull());
    if (!thisObj) {
        JS_ReportError(cx, "Invalid context");
        return false;
    }

    // Get the pointer to our C Robot struct
    Robot *robot = static_cast<Robot *>(JS_GetInstancePrivate(cx, thisObj, &Robot_class, &args));
    if (!robot) {
        return false;
    }

    // Arguments checking and converting
    if (argc != 1 || !args[0].isString()) {
        JS_ReportError(cx, "Invalid arguments");
        return false;
    }

    JS::RootedString str(cx, args[0].toString());
    if (!str) {
        return false;
    }

    JSAutoByteString bytes(cx, str);
    if (!bytes) {
        return false;
    }

    printf("Robot %s says : \"%s\"\n", robot->name, bytes.ptr());
    return true;
}

void robot_finalizer(JSFreeOp *fop, JSObject *obj)
{
    Robot *robot = static_cast<Robot *>(JS_GetPrivate(obj));
    if (!robot) {
        return;
    }

    free(robot->name);
    delete robot;
}

static JSClass global_class = {
    "global",
    JSCLASS_GLOBAL_FLAGS,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    JS_GlobalObjectTraceHook
};
void reportError(JSContext *cx, const char *message, JSErrorReport *report)
{
    fprintf(stderr, "%s:%u:%s\n",
            report->filename ? report->filename : "[no filename]",
            (unsigned int) report->lineno,
            message);
}

int run(JSContext *cx)
{
    // Enter a request before running anything in the context.
    JSAutoRequest ar(cx);

    // Create the global object and a new compartment.
    JS::RootedObject global(cx);
    global = JS_NewGlobalObject(cx, &global_class, nullptr,
                                JS::DontFireOnNewGlobalHook);
    if (!global) {
        return 1;
    }

    // Enter the new global object's compartment.
    JSAutoCompartment ac(cx, global);

    // Populate the global object with the standard globals, like Object and
    // Array.
    if (!JS_InitStandardClasses(cx, global)) {
        return 1;
    }

    // Expose Robot JS object on the global object
    JS_InitClass(cx, global, nullptr, &Robot_class, robot_constructor, 0, nullptr, Robot_funcs, nullptr, nullptr);

    JS::RootedValue rval(cx);
    JS::CompileOptions opts(cx);

    bool ok = JS::Evaluate(cx, opts, "robot.js", &rval);

    return ok ? 0 : 1;
}

int main(int argc, const char *argv[]) {
    if (!JS_Init()) {
       return 1;
    }

    JSRuntime *rt = JS_NewRuntime(8L * 1024L * 1024L);
    if (!rt) {
       return 1;
    }

    JSContext *cx = JS_NewContext(rt, 8192);
    if (!cx) {
       return 1;
    }

    JS_SetErrorReporter(rt, reportError);

    int status = run(cx);

    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);
    JS_ShutDown();

    return status;
}
