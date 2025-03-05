# Sandbox repository. 

This is experimental repository. There is nothing meaningful information except I note. 

## Details
This commit demonstrate the collision between weakly bound function and inline function. 

A weakly bound function will be overloaded by normal function at link phase. On the other hand, in the case of the overloading by an inline function, the definition of the weakly bound function must avoid the ODR(One Definition Rule) of C++. In the other word, you must not include the head with the inline function from the file which has weakly bound function. 

This rule is demonstrated by the [src/CMakeLists.txt](src/CMakeLists.txt).

The line 14 control the figure of the program. 
```
                       -DDEFINE_AT_MAIN_CPP=false
```
If the DEFINE_AT_MAIN_CPP is set to true, the weakly bound function is included from main.cpp. In this case, compile errors occur because the the inline definition is included in the main.cpp. 

If the DEFINE_AT_MAIN_CPP is set to false, the weakly bound function is defined inside foo.cpp. In this case, no compile error occurs because the incline function is not included from foo.cpp. 
a