# Native [![Windows x64](https://github.com/MartinKuschnik/Native/actions/workflows/windows-x64.yml/badge.svg)](https://github.com/MartinKuschnik/Native/actions/workflows/windows-x64.yml) [![Windows x86](https://github.com/MartinKuschnik/Native/actions/workflows/windows-x86.yml/badge.svg)](https://github.com/MartinKuschnik/Native/actions/workflows/windows-x86.yml)

C++ framework


## Seting up new C++ Project

Following Changes must be done for all Configurations and Plattforms:
  
 - Set Output Directory
  
   Project (right-click your project name in solution explorer) > Properties > General > Output Directory: 
  
       $(ProjectDir)bin\$(Configuration)\$(Platform)\
      
 - Set Intermediate Directory
  
   Project (right-click your project name in solution explorer) > Properties > General > Intermediate Directory: 
  
       $(ProjectDir)obj\$(Configuration)\$(Platform)\  
 
- Enable Multi-Processor (Parallel) Builds

  Project (right-click your project name in solution explorer) > Properties > General > Multi-processor Compilation: 
  
      Yes (/MP)

- Set C++ Language Standard to C++20

  Project (right-click your project name in solution explorer) > Properties > General > C++ Language Standard: 
  
      ISO C++20 Standard (/std:c++20)
      
## Seting up new C++ Test Project
  
Project naming Pattern: `$(NameOfTheProjectToTest)-Tests`

After creation the project should be renamed to `$(NameOfTheProjectToTest) - Tests`. This must be done to achieve the correct order of the project without creating build errors (on the build system) caused by whitespaces.

Following Changes must be done additionaly to all C++ Test Projects for all Configurations and Plattforms:

- Adjust Target Name

    Project (right-click your project name in solution explorer) > Properties > General > Target Name: 

      $(MSBuildProjectName)

- Disable precompiled heasers

  Project (right-click your project name in solution explorer) > Properties > C++ > Precompiled Headers > Precompiled Header: 
  
      Not Using Precompiled Headers
      
Afterwards the preciled header can be deleted.
