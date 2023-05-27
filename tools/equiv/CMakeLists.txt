add_executable (eq NativEq.cpp)
target_link_libraries (eq ${Z3_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${GMPXX_LIB} ${GMP_LIB})
install(TARGETS eq RUNTIME DESTINATION bin)
