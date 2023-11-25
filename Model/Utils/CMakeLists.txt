target_sources(${PROJECT_NAME} PUBLIC
        TimeGenerator.cpp TimeGenerator.h
        Logger.cpp Logger.h
        ElementPicker.h
        TypedParticle.h
        Queue.cpp Queue.h
        )

add_subdirectory(ElementPickers)
add_subdirectory(Queue)
