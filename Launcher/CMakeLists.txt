set(PROJECT_NAME Launcher)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "resource.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "launcher.c"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Launcher)

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
        $<$<CONFIG:ReleaseDynamicLinking>:
            MultiThreadedDLL
        >
        $<$<CONFIG:ReleaseStaticLinking>:
            MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:ReleaseDynamicLinking>,$<CONFIG:ReleaseStaticLinking>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:ReleaseDynamicLinking>:
            MultiThreadedDLL
        >
        $<$<CONFIG:ReleaseStaticLinking>:
            MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:ReleaseDynamicLinking>,$<CONFIG:ReleaseStaticLinking>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
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
                /SUBSYSTEM:CONSOLE;
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
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
                /SUBSYSTEM:CONSOLE;
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG
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
        COMMAND $<CONFIG:ReleaseDynamicLinking> IF EXIST "%BuildDir%" RMDIR /S /Q "%BuildDir%"
        COMMAND $<CONFIG:ReleaseDynamicLinking> IF NOT EXIST "%BuildDir%" MKDIR "%BuildDir%"
        COMMAND $<CONFIG:ReleaseDynamicLinking> IF EXIST "%BuildDir%\\$<TARGET_FILE_NAME:${PROJECT_NAME}>" DEL /F "%BuildDir%\\$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        COMMAND $<CONFIG:ReleaseDynamicLinking> XCOPY /Y /E /D "${OUTPUT_DIRECTORY}$<TARGET_FILE_NAME:${PROJECT_NAME}>" "%BuildDir%\\"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMANDS
        COMMAND $<CONFIG:ReleaseDynamicLinking> SET BuildDir=$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>build
        COMMAND $<CONFIG:ReleaseDynamicLinking> IF EXIST "%BuildDir%" RMDIR /S /Q "%BuildDir%"
        COMMAND $<CONFIG:ReleaseDynamicLinking> IF NOT EXIST "%BuildDir%" MKDIR "%BuildDir%"
        COMMAND $<CONFIG:ReleaseDynamicLinking> IF EXIST "%BuildDir%\\$<TARGET_FILE_NAME:${PROJECT_NAME}>" DEL /F "%BuildDir%\\$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        COMMAND $<CONFIG:ReleaseDynamicLinking> XCOPY /Y /E /D "${OUTPUT_DIRECTORY}$<TARGET_FILE_NAME:${PROJECT_NAME}>" "%BuildDir%\\"
    )
endif()

