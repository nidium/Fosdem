/*
   Copyright 2016 Nidium Inc. All rights reserved.
   Use of this source code is governed by a MIT license
   that can be found in the LICENSE file.
*/


#include "JSRobot.h"
#include <Binding/ClassMapper.h>


namespace Nidium {
namespace Binding {

// {{{ Start Constructor
JSRobot *JSRobot::Constructor(JSContext *cx, JS::CallArgs &args,
                              JS::HandleObject obj)
{
    unsigned argc = args.length();
    unsigned argcMin = ((1 > argc) ? (argc) : (1));

    switch (argcMin)
        {
            case 1:
                {
                    /* Start argument conversion */
                    /* Handle argument #0 of type "cstring" */
                    if (args[0].isNull())
                        {
                            JS_ReportError(cx, "TypeError");
                            return nullptr;
                        }
                    //.. cstring
                    JS::RootedString __curstr(cx, JS::ToString(cx, args[0]));
                    if (!__curstr)
                        {
                            JS_ReportError(cx, "TypeError");
                            return nullptr;
                        }
                    JSAutoByteString __curstr_c;
                    __curstr_c.encodeUtf8(cx, __curstr);

                    char *inArg_0 = __curstr_c.ptr();

                    /* End of argument conversion */

                    JSRobot *n_JSRobot = new JSRobot(inArg_0);

                    return n_JSRobot;
                    break;
                }
            default:
                JS_ReportError(cx, "TypeError: wrong number of arguments");
                return nullptr;
                break;
        }

    return nullptr;
}
// }}} End Constructor

// {{{ Methods
// List normal methods
JSFunctionSpec *JSRobot::ListMethods()
{
    static JSFunctionSpec funcs[] = { CLASSMAPPER_FN(JSRobot, echo, 1),

                                      JS_FS_END };

    return funcs;
}


// Methods implementation
// {{{ Start method echo
bool JSRobot::JS_echo(JSContext *cx, JS::CallArgs &args)
{
    unsigned argc = args.length();
    unsigned argcMin = ((1 > argc) ? (argc) : (1));

    switch (argcMin)
        {
            case 1:
                {
                    /* Start argument conversion */
                    /* Handle argument #0 of type "cstring" */
                    if (args[0].isNull())
                        {
                            JS_ReportError(cx, "TypeError");
                            return false;
                        }

                    //.. cstring
                    JS::RootedString __curstr(cx, JS::ToString(cx, args[0]));
                    if (!__curstr)
                        {
                            JS_ReportError(cx, "TypeError");
                            return false;
                        }
                    JSAutoByteString __curstr_c;
                    __curstr_c.encodeUtf8(cx, __curstr);

                    char *inArg_0 = __curstr_c.ptr();


                    /* End of argument conversion */

                    this->echo(inArg_0);
                    break;
                }
            default:
                JS_ReportError(cx, "TypeError: wrong number of arguments");
                return false;
                break;
        }

    return true;
}
// }}} End method echo

// }}} End Methods

// {{{ Registration
void JSRobot::RegisterObject(JSContext *cx)
{

    JSRobot::ExposeClass(cx, "Robot");
}
// }}}

} // namespace Binding
} // namespace Nidium
