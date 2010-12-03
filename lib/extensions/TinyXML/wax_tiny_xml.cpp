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
		.set("cStr", &TiXmlPrinter::CStr)
		.static_inherits< TiXmlVisitor >()
	;
	
	#define DefaultTiXmlNodeMethods(C) \
		.set("type", &C::Type) \
		.set("value", &C::Value) \
		.set< TiXmlNode, void, const char* >("setValue", &C::SetValue) \
		.set< TiXmlNode >("linkEndChild", &C::LinkEndChild) \
		.static_inherits< TiXmlNode >() \
		.set< C, bool, TiXmlVisitor* >("accept", &C::Accept) \
		.set< C, TiXmlNode* >("firstChild", &C::FirstChild) \
		.set< C, TiXmlNode*, const char* >("firstChildWithTag", &C::FirstChild) \
		.set< C, TiXmlNode*, const TiXmlNode* >("IterateChildren", &C::IterateChildren) \
		.set< C, TiXmlNode* >("parent", &C::Parent) \
		.set("insertAfterChild", &C::InsertAfterChild) \
		.set("insertBeforeChild", &C::InsertBeforeChild) \
		.set("noChildren", &C::NoChildren) \
		.set("removeChild", &C::RemoveChild) \
		.set("replaceChild", &C::ReplaceChild)
	
	SLB::Class< TiXmlDeclaration >("tinyxml::Declaration")
		.constructor< const char*, const char*, const char* >()
		DefaultTiXmlNodeMethods(TiXmlDeclaration)
	;
	
	SLB::Class< TiXmlComment >("tinyxml::Comment")
		.constructor< const char* >()
		DefaultTiXmlNodeMethods(TiXmlComment)
	;
	
	SLB::Class< TiXmlDocument >("tinyxml::Document")
		.constructor()
		DefaultTiXmlNodeMethods(TiXmlDocument)
		.set("parse", reinterpret_cast< const char* (TiXmlDocument::*) (const char*, int, TiXmlEncoding) >( &TiXmlDocument::Parse ))
		.set< TiXmlDocument, bool, const char* >("saveFile", &TiXmlDocument::SaveFile)
		.set< TiXmlDocument, bool, const char*, TiXmlEncoding >("loadFile", &TiXmlDocument::LoadFile)
		.set< TiXmlDocument, TiXmlElement* >("rootElement", &TiXmlDocument::RootElement)
		.enumValue("Encoding::TIXML_ENCODING_UNKNOWN", TIXML_ENCODING_UNKNOWN)
		.enumValue("Encoding::TIXML_ENCODING_UTF8"   , TIXML_ENCODING_UTF8)
		.enumValue("Encoding::TIXML_ENCODING_LEGACY" , TIXML_ENCODING_LEGACY)
	;
	
	SLB::Class< TiXmlElement >("tinyxml::Element")
		.constructor<const char*>()
		DefaultTiXmlNodeMethods(TiXmlElement)
		.set< TiXmlElement, TiXmlAttribute* >("firstAttribute", &TiXmlElement::FirstAttribute)
		.set< TiXmlElement, TiXmlAttribute* >("lastAttribute", &TiXmlElement::LastAttribute)
		.set< TiXmlElement, void, const char*>("removeAttribute", &TiXmlElement::RemoveAttribute)
		.set("getText", &TiXmlElement::GetText)
	;
	
	SLB::Class< TiXmlText >("tinyxml::Text")
		.constructor<const char*>()
		DefaultTiXmlNodeMethods(TiXmlText)
		.set("CDATA", &TiXmlText::CDATA)
		.set("setCDATA", &TiXmlText::SetCDATA)
	;
	
	SLB::Class< TiXmlAttribute >("tinyxml::Attibute")
		.constructor<const char*, const char*>()
		.set("name", &TiXmlAttribute::Name)
		.set< TiXmlAttribute, void, const char*>("setName", &TiXmlAttribute::SetName)
		.set("value", &TiXmlAttribute::Value)
		.set< TiXmlAttribute, void, const char*>("setValue", &TiXmlAttribute::SetValue)
		.set< TiXmlAttribute, const TiXmlAttribute*>("next", &TiXmlAttribute::Next)
		.set< TiXmlAttribute, const TiXmlAttribute*>("previous", &TiXmlAttribute::Previous)
		.static_inherits< TiXmlBase >()
	;
};