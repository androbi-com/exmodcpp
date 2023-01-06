# Type Erasure

This example follows Klaus Igelberger's talk https://youtu.be/qn6OqefuH08.
In a first step (main1.cpp), external polymorphism is introduced. Then (main2.cpp),
the unique pointer to the "concept" is incorporated into a wrapper class which 
also includes the "concept" and "model" as nested private classes. This 
enables value semantics for the end user of the class.

We also include what ChatGPT offers as an example for type erasure. Here the
wrapper class is a container that is capable of returning the value of the 
object added to the container by using the "get" interface.

# Compile

Edit the line 

    add_executable(ex_type_erasure chatgpt.cpp)

to use the desired version in `CMakeLists.txt` and build the project.

