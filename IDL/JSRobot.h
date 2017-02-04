/*
   Copyright 2016 Nidium Inc. All rights reserved.
   Use of this source code is governed by a MIT license
   that can be found in the LICENSE file.
*/

#include "JSRobotBase.h"
#include <Binding/ClassMapper.h>
#include <Binding/JSUtils.h>
#include <Core/Utils.h>

#ifndef binding_JSRobot_h__
#define binding_JSRobot_h__

namespace Nidium {
namespace Binding {

class JSRobot : public ClassMapper<JSRobot>, public JSRobotBase
{
public:
    JSRobot(char *name) : JSRobotBase(name) { }

    // Constructor
    static JSRobot *Constructor(JSContext *cx, JS::CallArgs &args,
                                JS::HandleObject obj);
    // Listings
    static JSFunctionSpec *ListMethods();
    // Registration
    static void RegisterObject(JSContext *cx);

protected:
    NIDIUM_DECL_JSCALL(echo);
};
} // namespace Binding
} // namespace Nidium

#endif
