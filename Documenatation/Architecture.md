#Data-Oriented Design
Subsystems which perform the "heavy lifting" (code called every or nearly every frame) should take a Data-Oriented approach. This mean avoiding things like heap allocation, pointer indirection, and heavy use of OOP idioms. Instead, favor tightly packed contiguous allocations (ObjectPool/PackedPool), PoD (Plain Old Data) structures, and tight "cache friendly" update loops.

#Entity Component System
Objects within Monsoon are simply the sum of their existing "system parts" (components). Each system manages a single component, how it is allocated, freed, accessed, and mutated.