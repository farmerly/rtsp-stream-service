set(project_include_dirs
    "${CMAKE_HOME_DIRECTORY}/include/rtsp"
    "${CMAKE_HOME_DIRECTORY}/include/socket"
    "${CMAKE_HOME_DIRECTORY}/include/utils")

add_subdirectory(test_rtsp)
add_subdirectory(test_rtsp_parse)
add_subdirectory(test_strutils)
