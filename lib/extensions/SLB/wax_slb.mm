//
//  wax_slb.m
//  HelloWorld
//
//  Created by Éverton Ribeiro on 25/11/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "wax_slb.h"

namespace Wax {
	Wrappers SLBManager::_wrappers;
	Wrappers SLBManager::_registereds;
	
	void SLBManager::addWrapper(WaxSLB_CFunction func) {
		_wrappers.push_back(func);
	}
	
	void SLBManager::addWrappers(WaxSLB_CFunction func, ...) {
		if (func) {
			_wrappers.push_back(func);
			
			va_list ap;
			va_start(ap, func);
			while((func = va_arg(ap, WaxSLB_CFunction))) _wrappers.push_back(func);
			
			va_end(ap);
		}
	}
	
	int SLBManager::doWrapper(lua_State *L) {
		SLB::Manager::getInstance().registerSLB(L);
		unsigned int i;
		
		for (i = 0; i < _wrappers.size(); i++) {
			_wrappers[i]();
			_registereds.push_back(_wrappers[i]);
		}
		
		_wrappers.clear();
		
		return 0;
	}
}

int luaopen_wax_slb(lua_State *L) {
	return Wax::SLBManager::doWrapper(L);
}