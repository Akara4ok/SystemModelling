target_sources(${PROJECT_NAME} PUBLIC
        Model.cpp Model.h
        ModelFactory.cpp ModelFactory.h
        )

add_subdirectory(Elements)
add_subdirectory(Utils)
