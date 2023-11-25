target_sources(${PROJECT_NAME} PUBLIC
        TimeGenerator.cpp TimeGenerator.h
        Logger.cpp Logger.h
        ElementPicker.h
        Queue.cpp Queue.h
        )

add_subdirectory(ElementPickers)
add_subdirectory(Queue)
