cmake_minimum_required(VERSION 3.21)

project(IntegratedDemonlist VERSION 1.7.13)

# 1. Use the standard Geode find_package approach
find_package(Geode REQUIRED)

# 2. Add your source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# 3. Define the library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# 4. Link your mod and include resources
target_link_libraries(${PROJECT_NAME} PRIVATE geode::geode)

# 5. Link your custom tool
CPMAddPackage("gh:hiimjasmine00/jasmine-tools@1.5.0")
target_link_libraries(${PROJECT_NAME} PRIVATE jasmine-tools)

# 6. CRITICAL: Register your resources
geode_target_resources(${PROJECT_NAME}
    RESOURCES
        resources/*.png
        resources/*.frag
        resources/*.vert
)

# 7. Finalize the mod
setup_geode_mod(${PROJECT_NAME})
