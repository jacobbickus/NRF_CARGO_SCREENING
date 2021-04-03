#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(mantis)
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON) # Can switch to OFF if not building with vis drivers --> NOT recommended

# Determine the Geant4 Version
execute_process(COMMAND bash -c "geant4-config --version"
                OUTPUT_VARIABLE MY_GEANT4_VERSION
                )
# Determine the ROOT CERN Version
execute_process(COMMAND bash -c "root --version"
                OUTPUT_VARIABLE MY_ROOT_VERSION
                )

string(SUBSTRING "${MY_GEANT4_VERSION}" 0 4 THE_VERSION)
string(COMPARE EQUAL "${THE_VERSION}" "10.7" VERSION_BOOL)

# If the version if > 10.7 then add the compile definition to avoid depreceated code!
if(${VERSION_BOOL})
  add_compile_definitions(G4_OPTPARAM)
  message(STATUS "Version is at least 10.7")
endif()

if(${MYVERBOSE})
  message(STATUS "Running make with Full Verbosity")
else()
  message(STATUS "Subduing some verbosity.")
  add_compile_options(-Wno-unused-variable -Wno-shadow -Wno-unused-parameter)
endif()

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 10.5 REQUIRED ui_all vis_all)
  message(STATUS "Geant4 Version: ${MY_GEANT4_VERSION}")
  message(STATUS "Built Geant4 with UI and Vis Drivers")
else()
  find_package(Geant4 10.5 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package)

find_package(ROOT CONFIG REQUIRED)
if(ROOT_FOUND)
    message(STATUS "${MY_ROOT_VERSION}")
else()
    message(STATUS "ROOT NOT Found. --> MAKE WILL FAIL!")
endif()

#----------------------------------------------------------------------------
# Locate Sources and Headers
include("${ROOT_USE_FILE}")
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 and ROOT libraries
#
add_executable(mantis mantis.cc ${sources} ${headers})
target_link_libraries(mantis ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build mantis. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(Runtime_SCRIPTS
  mantis.in
  mantisOff.in
  vis_save.mac
  )

set(Run_Analysis_SCRIPTS
stitch.sh
)

set(Slurm_Run_Files_SCRIPTS
runBatch.sh
submit_geant4.slurm
)

foreach(_script ${Runtime_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/Runtime/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
foreach(_script ${Run_Analysis_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/Run_Analysis/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
foreach(_script ${Slurm_Run_Files_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/Slurm_Run_Files/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS mantis DESTINATION bin)
