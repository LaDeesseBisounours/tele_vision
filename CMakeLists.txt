cmake_minimum_required( VERSION 2.8 )
add_definitions( ${GTK2_CFLAGS_OTHER} )
project( nuagesDetection )
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS "-g")
find_package(OpenMP REQUIRED)
find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK2 REQUIRED gtk+-2.0)
include_directories( ${GTK2_INCLUDE_DIRS} )
link_directories( ${GTK2_LIBRARY_DIRS} )
include_directories( src )
#-----------compilation step
add_executable( imaProjet_auto.exe src/main_auto.c src/misc.c src/compute.c)

add_executable( imaProjet.exe src/main.c src/misc.c src/compute.c)
#-----------linking step
target_link_libraries( imaProjet_auto.exe ${GTK2_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
target_link_libraries( imaProjet.exe ${GTK2_LIBRARIES} ${OpenMP_CXX_LIBRARIES})

