add_executable (mchc MemCHCUtils.cpp)
target_link_libraries (mchc ${Z3_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${GMPXX_LIB} ${GMP_LIB} ${ARMADILLO_LIBRARIES})
install(TARGETS mchc RUNTIME DESTINATION bin)
