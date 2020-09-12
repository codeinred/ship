# Ship: Making Software Portable

Ship is a user-level package manager that makes it easy to create 100% fully portable packages.

## Building Ship

NB: ship requires clang 10.0.0 or another compiler that supports C++20 coroutines

```
git clone --recursive https://github.com/functionalperez/ship.git

cd ship

cmake -Bbuild \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=ON

cmake --build build
```
