#Data-Oriented Design
Subsystems which perform the "heavy lifting" (code which is called every, or near every frame) within Monsoon should take a Data-Oriented approach. This mean avoiding things like heap allocation, indirection/random access, and heavy use of OOP design.

#Entity Component System
Objects within Monsoon are simply the sum of its individual system parts. Each system manages its own component data and that data can be accessed, or system operations can be performed on entities, via a 32-bit Entity Id (Monsoon::Entity). Once an entity no longer has any registered components to any system, it is implicitly destroyed.