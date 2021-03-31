get_filename_component(TMP_LIB_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" LIB_NAME ${TMP_LIB_NAME})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${SDK_ROOT}/lib/drivers/include ${SDK_ROOT}/lib/bsp/include ${SDK_ROOT}/lib/utils/include)
FILE(GLOB_RECURSE CURRENT_LIB_SRCS
  "${CMAKE_CURRENT_LIST_DIR}/*.c"
  "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
  )

ADD_LIBRARY(${LIB_NAME} ${CURRENT_LIB_SRCS})
