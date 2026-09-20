execute_process(COMMAND "${PROGRAM}" "${LOG_FILE}"
  RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
if(NOT "${result}" STREQUAL "0")
  message(FATAL_ERROR "Diagnostics test failed (${result}): ${output}\n${error}")
endif()
if(NOT output STREQUAL "" OR NOT error STREQUAL "")
  message(FATAL_ERROR "Diagnostics escaped the selected stream: ${output}\n${error}")
endif()
file(READ "${LOG_FILE}" log)
foreach(expected IN ITEMS
    "OpaqueSuccess"
    "OpaqueEqualityFailure"
    "OpaqueInequalityFailure"
    "Actual values: <unprintable> != <unprintable>"
    "Actual values: <unprintable> == <unprintable>"
    "Actual values: <unprintable> != 9"
    "Actual values: Printable(1) != Printable(2)"
    "Actual values: left != right"
    "Assertion failed: Opaque{1} == Opaque{2}"
    "diagnostics.cpp:"
    "Unexpected exception: diagnostic error"
    "Unexpected non-standard exception")
  string(FIND "${log}" "${expected}" position)
  if(position EQUAL -1)
    message(FATAL_ERROR "Missing diagnostic '${expected}': ${log}")
  endif()
endforeach()
