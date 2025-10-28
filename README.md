# ecpplib - Eryk's cpp libraries

## What is that?

This repository contains set of reusable libraries I'm using when creating my C++
programs. The idea is that each library should have zero dependencies between each other
so they can be dropped into any code base and used without worrying about grabbing dependencies.

The root folder of repository is split into two folders.
* libs - this is where source code of each lib is located, you can copy any folder from that directory into your project and it should work out of the box as long as you have C++20 compatible compiler. Some libraries can work on earlier version however I'm using C++20 compatible compilers myself.
* tests - this is where I'm writing test code for libraries, you can check it for examples but the idea is that tests project must compile and return 0 exit code as a way to validate it works as expected ;)

## License

This project is licensed under the terms of the MIT license.
