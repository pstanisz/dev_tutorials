# Copyright (c) 2022, Piotr Staniszewski

set(COMPONENTS
#    component
)

foreach(COMPONENT ${COMPONENTS})
    add_subdirectory(${COMPONENT})
endforeach(COMPONENT)