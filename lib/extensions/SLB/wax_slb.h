//
//  wax_slb.h
//  HelloWorld
//
//  Created by Éverton Ribeiro on 25/11/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SLB/SLB.hpp"

namespace Wax {

	typedef void (*WaxSLB_CFunction) ();
	typedef std::vector<WaxSLB_CFunction> Wrappers;
	
	class SLBManager {
	public:
		static void addWrapper(WaxSLB_CFunction func); //{ _wrappers.push_back(func); }
		static void addWrappers(WaxSLB_CFunction func, ...);
		static int doWrapper(lua_State *L);
		
	private:
		SLBManager();
		~SLBManager();
		
		static Wrappers _wrappers;
		static Wrappers _registereds;
	};
}

int luaopen_wax_slb(lua_State *L);