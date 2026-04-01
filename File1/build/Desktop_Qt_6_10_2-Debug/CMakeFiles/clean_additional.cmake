# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/File1_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/File1_autogen.dir/ParseCache.txt"
  "File1_autogen"
  )
endif()
