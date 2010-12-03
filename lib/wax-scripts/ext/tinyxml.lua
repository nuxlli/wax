if not SLB.tinyxml then return end -- Only load if the wax.http extension has been loaded

require 'wax.ext.table'

module(..., package.seeall)

SLB.using(SLB.tinyxml)
SLB.extensible(SLB.tinyxml, Document, Declaration, Comment, Element, Text)

-- Node
SLB.tinyxml.NodeType = SLB.createEnum({
  "DOCUMENT",
  "ELEMENT",
  "COMMENT",
  "UNKNOWN",
  "TEXT",
  "DECLARATION",
  "STYLESHEETREFERENCE",
  "TYPECOUNT"
})

-- Common extension

local v, k, i, f
local nodes_classes  = { Document, Declaration, Comment, Element, Text }

local nodes_ext = {
  toString = function(self)
    local printer = Printer()
    self:accept(printer)
    return printer:cStr()
  end, 

  unlink = function(self)
    self:parent():removeChild(self)
  end,

  traverse = function(self, func)
    if not self:noChildren() then
      local child = self:IterateChildren(0)
      while child do
        child:traverse(func)
        child = self:IterateChildren(child)
      end
    end
    func(self)
  end
}

-- Define is_[type]
for k, v in pairs(SLB.tinyxml.NodeType) do
  rawset(nodes_ext, "is_" .. k:lower(), function(self)
    return self:type() == v
  end)
end

for k, v in ipairs(nodes_classes) do
  for i, f in pairs(nodes_ext) do
    v[i] = f
  end
end

-- Document

local __parse = Document.parse
function Document.parse(self, body, ...)
  args = table.merge({
    0,
    Document.Encoding.TIXML_ENCODING_UTF8
  }, {...})
  if (self == Document) then
    doc = Document()
    doc:parse(body, ...)
    return doc
  else
    __parse(self, body, args[1], args[2])
  end
end

-- Element
local _firstChild = Element.firstChild
function Element.firstChild(self, ...)
  return (#{...} > 0) and self:firstChildWithTag(...) or _firstChild(self)
end

function Element.contentToString(self)
  local result = ""
  
  local child = self:IterateChildren(0)
  while child do
    result = result .. child:toString()
    child = self:IterateChildren(child)
  end
  
  return result
end