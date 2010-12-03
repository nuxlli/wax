if not SLB then return end -- Only load if the wax.http extension has been loaded

require "wax.ext.table"

local table   = table
local getfenv = getfenv
local print   = print
local type    = type
local rawset  = rawset
local rawget  = rawget
local getmetatable = getmetatable
local setmetatable = setmetatable
local ipairs  = ipairs

module("SLB")

local function using_index(self, key)
  local result = nil
  
  local slb_modules = rawget(self, "__slb_modules")
  for i,t in ipairs(slb_modules) do
    result = t[key]
    if result ~= nil then
      break
    end
  end
  
  if (result == nil) then
    local meta = getmetatable(self)
    if (meta and rawget(meta, "__old_index")) then
      func = rawget(meta, "__old_index")
      if (type(func) == "table") then
        result = func[key]
      elseif func ~= nil then
        result = func(self, key)
      end
    end
  end
  
  return result
end

function using(module)
  if (module) then
    local _env = getfenv(2)
    if (rawget(_env, "__slb_modules") == nil) then
      local meta = getmetatable(_env)
      if (meta and rawget(meta, "__index")) then
        rawset(meta, "__old_index", rawget(meta, "__index"))
      else
        meta = meta or {}
      end
      
      setmetatable(_env, meta)
      rawset(meta, "__index", using_index)
      rawset(_env, "__slb_modules", {})
    end
    
    table.insert(rawget(_env, "__slb_modules"), module)
  end
end

function extensible(module, ...)
  if (module) then
    local meta = getmetatable(module)
    if (meta and rawget(meta, "__indexCache")) then
      rawset(meta, "__newindex", function(self, index, value)
        local meta = getmetatable(self)
        rawget(meta, "__indexCache")[index] = value
      end)
    end
    extensible(...)
  end
end

function createEnum(table)
  local k, v
  local enum = {}
  for k, v in ipairs(table) do
    rawset(enum, v, k - 1)
  end
  return enum;
end