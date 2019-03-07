
aux_source_directory(. DIR_QUEUE_SRCS)

add_library(queue ${DIR_QUEUE_SRCS})

target_link_libraries(queue pthread)
