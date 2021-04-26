set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)
set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/src)

# Find all the public headers
#get_target_property(CAT_CUTIFIER_PUBLIC_HEADER_DIR CatCutifier INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE OpenVIII_CPP_WIP_PUBLIC_HEADERS
        ${DOXYGEN_INPUT_DIR}/*.h
        ${DOXYGEN_INPUT_DIR}/*.c
        ${DOXYGEN_INPUT_DIR}/*.hpp
        ${DOXYGEN_INPUT_DIR}/*.cpp
        )

set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen won't create this for us
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# Only regenerate Doxygen when the Doxyfile or public headers change
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
        DEPENDS ${OpenVIII_CPP_WIP_PUBLIC_HEADERS}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        COMMENT "Generating docs"
        VERBATIM)

# Nice named target so we can run the job easily
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source)
file(GLOB_RECURSE SPHINX_SOURCE_SOURCE_CODE ${SPHINX_SOURCE}/*.md ${SPHINX_SOURCE}/*.rst)
set(SPHINX_BUILD ${PROJECT_SOURCE_DIR}/docs)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when:
# - Doxygen has rerun
# - Our doc files have been updated
# - The Sphinx config has been updated
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
        COMMAND
        ${SPHINX_EXECUTABLE} -b html
        # Tell Breathe where to find the Doxygen output
        -Dbreathe_projects.OpenVIII_CPP_WIP=${DOXYGEN_OUTPUT_DIR}/xml
        ${SPHINX_SOURCE} ${SPHINX_BUILD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}

        # Other docs files you want to track should go here (or in some variable)
        DEPENDS ${SPHINX_SOURCE_SOURCE_CODE}

        ${DOXYGEN_INDEX_FILE}
        MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
        COMMENT "Generating documentation with Sphinx")

# Nice named target so we can run the job easily
add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})

# Add an install target to install the docs
include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD}
        DESTINATION ${CMAKE_INSTALL_DOCDIR})

set(NOJEKYLL_IN ${CMAKE_CURRENT_SOURCE_DIR}/.nojekyll)
set(NOJEKYLL_OUT ${SPHINX_BUILD}/.nojekyll)
# Replace variables inside @@ with the current values
configure_file(${NOJEKYLL_IN} ${NOJEKYLL_OUT} @ONLY)