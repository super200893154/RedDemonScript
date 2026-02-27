# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appRedDemonScript_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appRedDemonScript_autogen.dir\\ParseCache.txt"
  "appRedDemonScript_autogen"
  )
endif()
