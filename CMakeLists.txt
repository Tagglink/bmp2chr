cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(bmp2chr VERSION 0.2)

find_package(PkgConfig MODULE REQUIRED)
find_package(OpenCV REQUIRED)

pkg_check_modules(ICU REQUIRED
	icu-uc
	icu-io
	)

list(APPEND REQUIRED_LIBS 
	${ICU_LIBRARIES}
	${OpenCV_LIBS}
	)

list(APPEND REQUIRED_INCLUDE
	${ICU_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS}
	)

list(APPEND SOURCE_FILES
	src/bmp2chr.cpp
	src/converter.cpp
	src/charset.cpp
	src/file.cpp
	)

configure_file(bmp2chr_config.h.in src/bmp2chr_config.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
	"${PROJECT_BINARY_DIR}"
	"${REQUIRED_INCLUDE}"
	)

target_link_libraries(${PROJECT_NAME} PUBLIC
	"${REQUIRED_LIBS}"
	)
