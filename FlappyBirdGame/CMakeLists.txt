set(PROJECT_NAME FlappyBirdGame)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../.gitattributes"
    "../.gitignore"
    "../README.md"
    "../vcpkg-configuration.json"
    "../vcpkg.json"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "headers/Base.h"
    "headers/Bird.h"
    "headers/Game.h"
    "headers/Land.h"
    "headers/Pipe.h"
    "headers/Sound.h"
    "resource.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Resource_Files
    "FlappyBirdGame.aps"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Source_Files
    "entrypoint.c"
    "sources/Base.cpp"
    "sources/Bird.cpp"
    "sources/Game.cpp"
    "sources/Land.cpp"
    "sources/Main.cpp"
    "sources/Pipe.cpp"
    "sources/Sound.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Resource_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE FlappyBirdGame)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASEDYNAMICLINKING "TRUE"
        INTERPROCEDURAL_OPTIMIZATION_RELEASESTATICLINKING  "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASEDYNAMICLINKING "TRUE"
        INTERPROCEDURAL_OPTIMIZATION_RELEASESTATICLINKING  "TRUE"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:ReleaseStaticLinking>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:ReleaseStaticLinking>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:ReleaseStaticLinking>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:ReleaseStaticLinking>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:ReleaseDynamicLinking>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseStaticLinking>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:ReleaseDynamicLinking>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:ReleaseStaticLinking>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:ReleaseDynamicLinking>:
                /Oi;
                /Gy
            >
            $<$<CONFIG:ReleaseStaticLinking>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:ReleaseDynamicLinking>:
                /Oi;
                /Gy
            >
            $<$<CONFIG:ReleaseStaticLinking>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /SUBSYSTEM:CONSOLE
            >
            $<$<CONFIG:ReleaseDynamicLinking>:
                /SUBSYSTEM:WINDOWS;
                /OPT:REF;
                /OPT:ICF
            >
            $<$<CONFIG:ReleaseStaticLinking>:
                /SUBSYSTEM:WINDOWS;
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:ReleaseDynamicLinking>:
                /OPT:REF;
                /OPT:ICF
            >
            $<$<CONFIG:ReleaseStaticLinking>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Post build events
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMANDS
        COMMAND $<CONFIG:ReleaseDynamicLinking> SET BuildDir=$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>build
        COMMAND $<CONFIG:ReleaseDynamicLinking> IF NOT EXIST "%BuildDir%" MKDIR "%BuildDir%"
        COMMAND $<CONFIG:ReleaseDynamicLinking> IF EXIST "%BuildDir%" RMDIR /S /Q "%BuildDir%"
        COMMAND $<CONFIG:ReleaseDynamicLinking> XCOPY /Y /E /D "${OUTPUT_DIRECTORY}$<TARGET_FILE_NAME:${PROJECT_NAME}>" "%BuildDir%\\bin\\"
        COMMAND $<CONFIG:ReleaseDynamicLinking> XCOPY /Y /E /D "${OUTPUT_DIRECTORY}*.dll" "%BuildDir%\\bin\\"
        COMMAND $<CONFIG:ReleaseDynamicLinking> XCOPY /Y /E /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>resources\\*" "%BuildDir%\\resources\\"
        COMMAND  $<CONFIG:ReleaseStaticLinking> SET BuildDir=$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>build
        COMMAND  $<CONFIG:ReleaseStaticLinking> IF NOT EXIST "%BuildDir%" MKDIR "%BuildDir%"
        COMMAND  $<CONFIG:ReleaseStaticLinking> IF EXIST "%BuildDir%" RMDIR /S /Q "%BuildDir%"
        COMMAND  $<CONFIG:ReleaseStaticLinking> XCOPY /Y /E /D "${OUTPUT_DIRECTORY}$<TARGET_FILE_NAME:${PROJECT_NAME}>" "%BuildDir%\\"
        COMMAND  $<CONFIG:ReleaseStaticLinking> XCOPY /Y /E /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>resources\\*" "%BuildDir%\\resources\\"
    )
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:ReleaseStaticLinking>:"
            "winmm;"
            "setupapi;"
            "imm32;"
            "version"
        ">"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

