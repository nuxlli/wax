/*
 *  WaxSLB.h
 *  HelloWorld
 *
 *  Created by Éverton Ribeiro on 25/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "SLB.hpp"

extern "C"
#import "lua.h"

namespace SLB {
	class SLB_EXPORT WaxScript : Script {
	public:
		void setState(lua_State* L);
	};
}