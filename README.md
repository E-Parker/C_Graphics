This project was made by Ethan Parker.

This is a game engine writen from scratch in C with OpenGL.




# Code Structure & Standards:

This is a set of rules I follow when writing code, and should explain why I
chose to do things the way I do.


## Do not Obscure Anything 

hidden things should always be visable. This is important for clarity and implementation.


## Keep it Simple

While there is merit for some degree of reinventing the wheel, in general the fewer extra 
steps needed to solve a problem, the better.

## Clear Naming of Structures, "Methods", and Functions

Functions should only do exactly the thing the label sugests and nothing more. Do not
intentionally include side effects.

When naming functions which modify a struct, use the struct name followed by an underscore
to denote that it is a "method" of that structure. 
This is so that when you go to type the name of the struct, 
intilisense will also show all the functions that are acociated with it.

Example:
StructureName_do_something()

when defining a function to set default values of a struct, use the "initialize" postfix. 
Always define another function to deinitialize as well to keep things consistant.
Structures with this function defined must always be initialized before use, 
and must be deinitialized before freeing.

Sturctures which do not have an initialize or create are assumed to be self-contianed, 
and can just be freed.

This rule also applies to systems which have static, global scope variables.

Example:
StructureName_initialize()
StructureName_deinitialize()

when defining a function to alocate or otherwise create a struct which is intended to be 
stand-alone (not created on the stack), use the postfix "create", and "destroy".

create functions must always return a pointer to the new struct, 
and destroy must take a pointer to a pointer. 

Destroying a struct:
Always set any pointers to NULL after freeing. It's a minor one but can catch some issues
if its set to NULL. In some cases, especially for any heap variables, the compiler will optomise
these out. You can configure the compiler to not do that for debuging. I find it usefull when inspecting
the memory in a hex editor to see exactly when a struct has been dealocated.


