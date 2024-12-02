# Glob all .c files into the SOURCES variable
FILE(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
message(STATUS "Source directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Globbed sources: ${SOURCES}")

# Iterate over each source file in the SOURCES list
foreach(Source ${SOURCES})
    # Extract the base name (filename without directory and extension)
    get_filename_component(BaseName "${Source}" NAME_WE)
    message(STATUS "Adding executable: ${BaseName} from source: ${Source}")

    # Add an executable for each source file
    add_executable(${BaseName} "${Source}")
endforeach()
