-- Copyright (C) 2013, Markus Klotzbuecher <markus.klotzbuecher@mech.kuleuven.be>
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions are met:
--     * Redistributions of source code must retain the above copyright
--       notice, this list of conditions and the following disclaimer.
--     * Redistributions in binary form must reproduce the above copyright
--       notice, this list of conditions and the following disclaimer in the
--       documentation and/or other materials provided with the distribution.
--     * Neither the name of the <organization> nor the
--       names of its contributors may be used to endorse or promote products
--       derived from this software without specific prior written permission.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
-- "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
-- LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
-- FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
-- <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
-- SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
-- LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
-- USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
-- ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
-- OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
-- OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
-- SUCH DAMAGE.

local ffi=require "ffi"
local reflect=require "reflect"

local M={}

M.struct2tab={}

M.struct2tab['struct ubx_block']=function(b) return ffi.string(b.name) end

--- Convert a FFI cdata to a Lua table.
-- @param cd FFI cdata to convert Lua
-- @param don'refct, don't use (internal recursive param)
-- @param table
function M.tolua(cd, refct)
   local res

   if type(cd)==nil then error("cdata is nil") end

   if type(cd)~='cdata' then return cd end
   refct = refct or reflect.typeof(cd)

   -- print("tolua:", tostring(cd), utils.tab2str(refct))

   local function  is_prim_num(refct)
      if refct.what=='int' or refct.what=='float' or refct.what=='void' then return true end
      return false
   end

   local function is_string(refct)
      if refct.element_type.what=='int' and refct.element_type.size==1 then return true end
      return false
   end

   local function do_struct(cd, refct)
      res = {}
      for field_refct in refct:members() do
	 res[field_refct.name]=M.tolua(cd[field_refct.name])
      end
      return res
   end

   local function do_array(cd, refct)
      if is_string(refct) then return ffi.string(cd) end
      local res={}
      local num_elem=refct.size/refct.element_type.size
      for i=0,num_elem-1 do res[i+1]=M.tolua(cd[i]) end
      return res
   end

   local function do_number(cd, refct) return tonumber(cd) end

   if refct.what=='int' or refct.what=='float' then res=do_number(cd)
   elseif refct.what=='struct' then
      local fun = M.struct2tab['struct '..refct.name]
      if fun then res=fun(cd)
      else res=do_struct(cd, refct) end
   elseif refct.what=='array' then res=do_array(cd, refct)
   elseif refct.what=='ref' then res=M.tolua(cd, refct.element_type)
   elseif  refct.what=='ptr' then
      -- Don't touch any char*, because we don't know if they are zero
      -- terminated or not
      if cd==nil then res='NULL'
      elseif is_prim_num(refct.element_type) then res=tonumber(cd[0])
      else res=M.tolua(cd, refct.element_type) end
   else print("can't handle "..refct.what..", ignoring.") end
   return res
end

--- Destructure a refct into a Lua table.
-- @param refct reflect ctype
-- @result lua table
function M.refct_destruct(refct)
   local res

   local function  is_prim_num(refct)
      if refct.what=='int' or refct.what=='float' or refct.what=='void' then return true end
      return false
   end

   local function is_string(refct)
      if refct.element_type.what=='int' and refct.element_type.size==1 then return true end
      return false
   end

   local function do_struct(refct)
      res = {}
      for field_refct in refct:members() do
	 res[field_refct.name]=M.refct_destruct(field_refct.type)
      end
      return res
   end

   local function do_array(refct)
      if is_string(refct) then return 'string' end
      local res={}
      local num_elem=refct.size/refct.element_type.size
      for i=0,num_elem-1 do res[i]=M.refct_destruct(refct.element_type) end
      return res
   end

   if refct.what=='int' or refct.what=='float' then res='number'
   elseif refct.what=='struct' then res=do_struct(refct)
   elseif refct.what=='array' then res=do_array(refct)
   elseif refct.what=='ref' then res=M.refct_destruct(refct.element_type)
   elseif  refct.what=='ptr' then
      -- Don't touch any char*, because we don't know if they are zero
      -- terminated or not
      if cd==nil then res='NULL'
      elseif is_prim_num(refct.element_type) then res='number'
      else res=M.refct_destruct(refct.element_type) end
   else print("can't handle "..refct.what..", ignoring.") end

   return res
end

function M.ctype_destruct(ctype)
   return M.refct_destruct(reflect.typeof(ctype))
end

--- Flatten table and subtable keys.
-- @param t table to flatten
-- @return table of {key="x.y.z", value="number|string"}
function M.flatten_keys(t)
   local function __flatten_keys(t, res, prefix)
      for k,v in pairs(t) do
	 if type(v)=='table' then
	    if prefix=="" then __flatten_keys(v, res, k)
	    else
	       if type(k)=='number' then
		  __flatten_keys(v, res, prefix..'['..k..']')
	       else
		  __flatten_keys(v, res, prefix..'.'..k)
	       end
	    end
	 else
	    if type(k)=='number' then
	       res[#res+1] = { key=prefix..'['..k..']', value=v }
	    else
	       if prefix=="" then
		  res[#res+1] = { key=k, value=v }
	       else
		  res[#res+1] = { key=prefix.."."..k, value=v }
	       end
	    end
	 end
      end
      return res
   end
   return __flatten_keys(t, {}, "")
end

--- Generate a fast logging function for the given ctype
-- @param ctype ffi ctype (ffi.typeof) for which the function shall be generated.
-- @return function(x, fd), x is cdata and fd is filedescriptor to write to
function M.gen_fast_ser(ctype)
   local flattab = M.flatten_keys(M.ctype_destruct(ctype))
   table.sort(flattab, function(t1,t2) return t1.key < t2.key end)

   -- add 'serfun' field holding serialization function
   utils.foreach(function(e)
		    if e.value == 'number' then e.serfun = "tonumber"
		    elseif e.value == 'string' then e.serfun = "ffi.string"
		    else
		       error("unkown value "..e.value.." for key "..e.key)
		    end
		 end, flattab)
   local ok, res = utils.preproc(
[[
return function(x, fd)
    if x=='header' then
    # for i=1,#flattab do
	fd:write("$(flattab[i].key)")
    #   if i<#flattab then
	  fd:write("$(separator)")
    #   end
    # end
      fd:write("\n")
      return
    end

    assert(tostring(ffi.typeof(x))=='$(tostring(ctype))',
	  "serializer: argument not a $(tostring(ctype)) but a "..tostring(ffi.typeof(x)))
    # for i=1,#flattab do
	fd:write($(flattab[i].serfun)(x$("."..flattab[i].key)))
    #   if i<#flattab then
	  fd:write("$(separator)")
    #   end
    # end
      fd:write("\n")
end
]], { io=io, table=table, ipairs=ipairs, flattab=flattab, tostring=tostring, ctype=ctype, separator=', ' })

   assert(ok, res)
   print(res)
   ok, res = utils.eval_sandbox(res, { print=print, ffi=ffi, io=io, assert=assert,
				       tostring=tostring, tonumber=tonumber })
   assert(ok, res)
   return res
end

return M