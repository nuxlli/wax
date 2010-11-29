/*
 *  WaxSLB.cpp
 *  HelloWorld
 *
 *  Created by Éverton Ribeiro on 25/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "WaxScript.h"

namespace SLB {
	void WaxScript::setState(lua_State* L) {
		if(!_L) {
			_L = L;
			SLB::Manager::getInstance().registerSLB(_L);
		}
	}
}