/*
 *  wax_tiny_xml.cpp
 *  HelloWorld
 *
 *  Created by Éverton Ribeiro on 26/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "wax_tiny_xml.hpp"

void waxwrapper_tinyxml() {
	
	SLB::Class< TiXmlPrinter >("tinyxml::Printer")
		.constructor()
		.set("CStr", &TiXmlPrinter::CStr)
		.static_inherits< TiXmlVisitor >()
	;
	
	SLB::Class< TiXmlDocument >("tinyxml::Document")
		.constructor()
		.set("Parse", reinterpret_cast< const char* (TiXmlDocument::*) (const char*, int, TiXmlEncoding) >( &TiXmlDocument::Parse ))
		.set< TiXmlNode >("LinkEndChild", &TiXmlDocument::LinkEndChild)
		.set< TiXmlDocument, bool, const char* >("SaveFile", &TiXmlDocument::SaveFile)
		.set< TiXmlDocument, bool, const char*, TiXmlEncoding >("LoadFile", &TiXmlDocument::LoadFile)
		.set< TiXmlDocument, bool, TiXmlVisitor* >("Accept", &TiXmlDocument::Accept)
		.enumValue("Encoding::TIXML_ENCODING_UNKNOWN", TIXML_ENCODING_UNKNOWN)
		.enumValue("Encoding::TIXML_ENCODING_UTF8"   , TIXML_ENCODING_UTF8)
		.enumValue("Encoding::TIXML_ENCODING_LEGACY" , TIXML_ENCODING_LEGACY)
		.static_inherits< TiXmlNode >()
	;
	
	SLB::Class< TiXmlDeclaration >("tinyxml::Declaration")
		.constructor< const char*, const char*, const char* >()
		.static_inherits< TiXmlNode >()
	;
	
	SLB::Class< TiXmlElement >("tinyxml::Element")
		.constructor<const char*>()
		.set< TiXmlNode >("LinkEndChild", &TiXmlElement::LinkEndChild)
		.static_inherits< TiXmlNode >()
	;
	
	SLB::Class< TiXmlText >("tinyxml::Text")
		.constructor<const char*>()
		.static_inherits< TiXmlNode >()
	;
};