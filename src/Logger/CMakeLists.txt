project( Logger )

SET( OUTPUT_NAME Logger )

add_library( ${OUTPUT_NAME} SHARED
  CLoggerMsg.cpp
  CLoggerMsg.h
  CSimpleLogger.cpp
  CSimpleLogger.h
  ILogger.h
  Logger.h
)

target_include_directories( ${OUTPUT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
