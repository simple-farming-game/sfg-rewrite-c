# CMake 최소 버전 설정
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 및 버전 설정
project(SFG VERSION 1.0)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# FetchContent 모듈을 사용하여 외부 라이브러리 다운로드
include(FetchContent)

# glad 라이브러리 추가
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glad)

# GLFW 라이브러리 추가
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glfw)

# stb_image 헤더 파일 다운로드
FetchContent_Declare(
    stb_image
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb_image)

# glm 라이브러리 추가
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# 소스 파일 설정
file(GLOB SOURCES "src/*.cpp")

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})

# stb_image.h 파일 포함 경로 설정
target_include_directories(${PROJECT_NAME} PRIVATE ${stb_image_SOURCE_DIR})
# glm.hpp 포함 경로 설정
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})

# include 디렉토리 경로 설정
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# 운영체제별 OpenGL 라이브러리 설정
if(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32) # Windows용 OpenGL
elseif(APPLE)
    find_library(COCOA_LIB Cocoa REQUIRED)         # macOS의 Cocoa
    find_library(OpenGL_LIB OpenGL REQUIRED)       # macOS의 OpenGL
    find_library(IOKIT_LIB IOKit REQUIRED)         # macOS의 IOKit
    find_library(COREVIDEO_LIB CoreVideo REQUIRED) # macOS의 CoreVideo

    target_link_libraries(${PROJECT_NAME} ${COCOA_LIB} ${OpenGL_LIB} ${IOKIT_LIB} ${COREVIDEO_LIB})
else()
    target_link_libraries(${PROJECT_NAME} OpenGL::GL)  # Linux OpenGL
endif()

# GLFW와 glad 라이브러리 링크
target_link_libraries(${PROJECT_NAME} glfw glad)
