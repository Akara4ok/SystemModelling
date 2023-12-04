target_sources(${PROJECT_NAME} PUBLIC
        Distribution.h
        ExpDist.cpp ExpDist.h
        HardcodedDistribution.cpp HardcodedDistribution.h
        Logger.cpp Logger.h
        ElementPicker.h
        Queue.cpp Queue.h
        )

add_subdirectory(ElementPickers)
add_subdirectory(Queue)
