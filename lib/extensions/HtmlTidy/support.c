/* HTML Tidy Lua 5.0 Bindings
   Copyright (c) 2006 Promixis, LLC 
   Author: Ron Bessems
   Date: Oct 26, 2006

   Licensed under MIT License.   

   Supporting functions.
*/

#include "support.h"


void createmeta (lua_State *L, const char * name) 
{
	luaL_newmetatable(L, name);
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -2);
	lua_rawset(L, -3);
}


/* ******************************************************************************************* */
/* Book keeping of nodes referenced from Lua */
/* we'll need to keep track of all the nodes handed out to Lua
   this is because when the tidyDoc gets garbage collected all 
   the nodes are no longer valid, however lua might still have
   references to them. So at that point we'll go through that
   list and NULL them out. */
/* ******************************************************************************************* */
void doc_register_node ( pTidy t, pTidyNode n )
{
    pNodeListItem i;

	if ( (!n) || ( !t ) )    
        return;

    i = (pNodeListItem)malloc ( sizeof ( sNodeListItem ));
    if (!i) 
        return;
    i->node = n;
    i->next = t->nodeList;
    t->nodeList = i;
}

// removes the node from the list and frees the storage
void doc_unregister_node ( pTidyNode n )
{
	pNodeListItem prev;
	pNodeListItem i;

    if ( !n ) 
        return;
    if ( !n->tidy)
        return;
       
    prev = NULL;
    i = n->tidy->nodeList;
    while ( i )
    {    
        if ( i->node == n ) 
        {
            if (prev)
            {
                prev->next = i->next;
            }
            else
            {
                n->tidy->nodeList = i->next;
            }
            free(i);
            return;
        }
        prev = i;
        i = i->next;
    }
    
}

// this goes through all the nodes and sets them to NULL so
// that subsequent access from Lua will simply error out instead of
// crash. This does NOT free the nodes!
void doc_clear_all_nodes (pTidy t)
{
    pNodeListItem next,i;

	if ( !t )
        return;
    
    
    i = t->nodeList;
    while ( i )
    {        
        next = i->next;
        
        i->node->node=NULL;
        free(i);
        
        i = next;
    }
}



/* Node operations ***********/
// this creates a new node object inside Lua.
int push_node( lua_State *L , TidyNode Node, pTidy t)
{
    pTidyNode n;

	if ( Node == 0 ) 
        return 0;
    
    n = (pTidyNode)lua_newuserdata(L, sizeof(sTidyNode));
	n->node = Node;
    n->tidy = t;
	luaL_getmetatable(L, LUATIDY_NODE_HANDLE);
	lua_setmetatable(L,-2);	    
    doc_register_node(t,n);
    return 1;
}

/* pushes a table with attributes onto the stack
   if it returns 1, if it returns 0 there where no
   attributes and nothing was pushed onto the stack.
*/
int push_node_attributes(lua_State *L, TidyNode node)
{
    TidyAttr t;
    int i=1;
    t = tidyAttrFirst ( node );
    if ( !t ) return 0;
    
    lua_newtable(L);
    
    while ( t ) 
    {
        lua_pushstring(L, tidyAttrName (t));
        lua_pushstring(L, tidyAttrValue (t));
        lua_settable(L,-3);
        
        lua_pushnumber(L, i);
        lua_pushstring(L, tidyAttrName(t));
        lua_settable(L,-3);
        i++;
        
        t = tidyAttrNext ( t );
    }
    return 1;
}

/* This function pushes the text of a tidynode onto the stack, if any.
   returns 1 if it pushed the text onto the stack and zero if not.
*/
int push_node_text(lua_State *L, TidyDoc doc, TidyNode node )
{
    int ret;
    TidyBuffer output;
    tidyBufInit(&output);
        
    if ( tidyNodeGetText (doc,node , &output) )
    {
        lua_pushlstring(L, (char*)output.bp, output.size );        
        ret = 1;
    }
    else
    {
        ret = 0;
    }
        
    tidyBufFree( &output );        
    return ret;
}

/* ---------------------------------------------------------------------------
  Name       : replace - Search & replace a substring by another one. 
  Creation   : Thierry Husson, Sept 2010
  Parameters :
      str    : Big string where we search
      oldstr : Substring we are looking for
      newstr : Substring we want to replace with
      count  : Optional pointer to int (input / output value). NULL to ignore.  
               Input:  Maximum replacements to be done. NULL or < 1 to do all.
               Output: Number of replacements done or -1 if not enough memory.
  Returns    : Pointer to the new string or NULL if error.
  Notes      : 
     - Case sensitive - Otherwise, replace functions "strstr" by "strcasestr"
     - Always allocates memory for the result.
--------------------------------------------------------------------------- */
char* replace(const char *str, const char *oldstr, const char *newstr, int *count)
{
   const char *tmp = str;
   char *result;
   int   found = 0;
   int   length, reslen;
   int   oldlen = strlen(oldstr);
   int   newlen = strlen(newstr);
   int   limit = (count != NULL && *count > 0) ? *count : -1; 

   tmp = str;
   while ((tmp = strstr(tmp, oldstr)) != NULL && found != limit)
      found++, tmp += oldlen;
   
   length = strlen(str) + found * (newlen - oldlen);
   if ( (result = (char *)malloc(length+1)) == NULL) {
      fprintf(stderr, "Not enough memory\n");
      found = -1;
   } else {
      tmp = str;
      limit = found; /* Countdown */
      reslen = 0; /* length of current result */ 
      /* Replace each old string found with new string  */
      while ((limit-- > 0) && (tmp = strstr(tmp, oldstr)) != NULL) {
         length = (tmp - str); /* Number of chars to keep intouched */
         strncpy(result + reslen, str, length); /* Original part keeped */ 
         strcpy(result + (reslen += length), newstr); /* Insert new string */
         reslen += newlen;
         tmp += oldlen;
         str = tmp;
      }
      strcpy(result + reslen, str); /* Copies last part and ending nul char */
   }
   if (count != NULL) *count = found;
   return result;
}