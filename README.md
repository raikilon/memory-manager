# C++ Memory Manager

# Features

- **Separate Files**: It is implemented in a separate class and separate files.
- **No includes**: The memory manager works automatically in all compiled files without being included.
- **Debug config only**: It should work only in the debug configuration.
- **Standard new & delete**: Only the standard *new*, *new[]*, *delete* and *delete[]* operators should be overridden.
- **Log**: In debug config it should log all allocation and deallocations in the std::out with:
  - Type of allocation and deallocation (array or not) 
  - Size
  - Memory address
  - Allocation number
  - A warning is printed if the type of allocation and deallocation defer.
- **atexit**: The memory manager attaches a memory leak reporting function to the atexit() routine. This routine is called after the main function has terminated and can be used to print out the remaining memory allocations that where not deallocated.
  - The following information should be printed on the atexit call:
    - NO. of memory leaks
    - Total size of memory not deallocated.
    - Maximum total size of memory allocated on the heap. 
    - Information per memory leak:
      - Allocation number
      - Memory address
      - Size

- **Tests**: Test the memory manager with numerous allocations and deallocations.

  - Test allocations and deallocations of single variables and arrays of standard data types.

  - Test allocations and deallocations of single variables and arrays of own class types. Check if the destructors are also executed.

  - Test allocations and deallocations of single variables and arrays of STL containers.

  - Test wrong deallocation of allocation (array vs. single new/delete).