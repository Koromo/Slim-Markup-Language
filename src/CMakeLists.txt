set(SML_HEADERS sml.h
                smldef.h
                smlobj.h
                smlparse.h)

add_custom_target(sml SOURCES ${SML_HEADERS})

install(FILES ${SML_HEADERS} DESTINATION include)