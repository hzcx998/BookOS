cmake_minimum_required(VERSION 2.8)

project(SDL2snake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS, "-Wall")

if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "My multi config types" FORCE)
elseif(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RELEASE)
endif()

if(UNIX)
	find_package( PkgConfig REQUIRED )
	pkg_check_modules( sdl2 REQUIRED IMPORTED_TARGET sdl2 )
	pkg_check_modules( SDL2_ttf REQUIRED IMPORTED_TARGET SDL2_ttf )
endif()

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIR})
file(GLOB SDL2snake_SRCS "src/*.cpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SDL2snake_SRCS})

if(UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::sdl2 )
	target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::SDL2_ttf )
endif()

if(UNIX)
	set(CMAKE_INSTALL_PREFIX /usr/local)
	install(TARGETS ${PROJECT_NAME} DESTINATION bin)
	install(FILES res/lazy.ttf DESTINATION bin)
elseif(WIN32)
	set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/")
	install(TARGETS ${PROJECT_NAME} DESTINATION bin)
	install(FILES res/lazy.ttf DESTINATION bin)
endif()
