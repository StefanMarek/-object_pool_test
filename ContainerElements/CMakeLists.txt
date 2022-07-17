set(PROJECT_NAME ContainerElements)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "Container.natvis"
    "cpp.hint"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "src/Definitions.h"
    "src/ErrorHandling.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__Container
    "src/FIFOPool.h"
    "src/HashPool.h"
    "src/HashPoolNode.h"
    "src/Heap.h"
    "src/HeapBase.h"
    "src/IndexHeap.h"
    "src/LIFOPool.h"
    "src/ObjectPool.h"
    "src/ObjectPoolPOD.h"
    "src/PairValues.h"
    "src/StringPool.h"
)
source_group("Header Files\\Container" FILES ${Header_Files__Container})

set(Header_Files__Tools
    "src/AllocationTools.h"
    "src/Allocator.h"
    "src/CompareTools.h"
    "src/ContainerTools.h"
    "src/HashTools.h"
    "src/MathTools.h"
    "src/ObjectPoolBuffer.h"
    "src/PODObjectPoolBuffer.h"
    "src/PoolGrowing.h"
    "src/SortingTools.h"
    "src/StringPoolTools.h"
)
source_group("Header Files\\Tools" FILES ${Header_Files__Tools})

set(Source_Files
    "src/ContainerElementsLib.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Header_Files__Container}
    ${Header_Files__Tools}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE ContainerElements)

# set(CMAKE_STATIC_LIBRARY_PREFIX "")

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            TARGET_NAME_DEBUG   "${PROJECT_NAME}-vc$ENV{PlatformToolsetVersion}-$ENV{PlatformShortName}-d"
            TARGET_NAME_RELEASE "${PROJECT_NAME}-vc$ENV{PlatformToolsetVersion}-$ENV{PlatformShortName}"
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            TARGET_NAME_DEBUG   "${PROJECT_NAME}-vc$ENV{PlatformToolsetVersion}-$ENV{PlatformShortName}-d"
            TARGET_NAME_RELEASE "${PROJECT_NAME}-vc$ENV{PlatformToolsetVersion}-$ENV{PlatformShortName}"
        )
    endif()
else()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "${PROJECT_NAME}d"
        TARGET_NAME_RELEASE "${PROJECT_NAME}"
    )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG   "${PROJECT_NAME}d"
        TARGET_NAME_RELEASE "${PROJECT_NAME}"
    )
    endif()
endif()    
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/lib/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib/"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/lib/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib/"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_LIB"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_LIB"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy;
                /Ot
            >
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gd;
                /Gy;
                /Ot
            >
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:WINDOWS
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:WINDOWS
        )
    endif()
endif()

# link_directories("${CMAKE_SOURCE_DIR}/lib")
# target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/lib")
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_SOURCE_DIR}/lib"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_SOURCE_DIR}/lib"
    )
endif()

