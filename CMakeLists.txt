cmake_minimum_required(VERSION 3.20)
project(eliott-ecs VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_TOOLCHAIN_FILE "C:/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "C:/Dev/vcpkg/installed/x64-windows")

# Standard C++20 obligatoire
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Intégration vcpkg ---
if(DEFINED ENV{VCPKG_ROOT})
    include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake OPTIONAL)
endif()

# --- Dépendances ---
find_package(doctest CONFIG REQUIRED)

# --- Lib statique ---
add_library(eliott-ecs STATIC
    src/EntityManager.cpp
)

target_include_directories(eliott-ecs PUBLIC
    include/
)

# --- Tests ---
add_executable(tests
    tests/test_entity.cpp
    tests/test_component.cpp
    tests/test_componentManager.cpp
    tests/test_systemManager.cpp    
    tests/test_world.cpp
)

target_link_libraries(tests PRIVATE
    eliott-ecs
    doctest::doctest
)