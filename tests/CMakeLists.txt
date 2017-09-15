set(TEST_SOURCES example.cpp
                 main.cpp)

include_directories(SYSTEM ${SML_INCLUDE_DIR} ${CPPUTEST_INCLUDE_DIR})
link_directories(${CPPUTEST_LIB_DIR})

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests cpputest cpputestext winmm)

install(TARGETS tests RUNTIME DESTINATION bin)
install(FILES example.sml DESTINATION bin)