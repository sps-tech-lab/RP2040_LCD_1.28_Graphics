aux_source_directory(. DIR_QMI8658_SRCS)

add_library(QMI8658 ${DIR_QMI8658_SRCS})
target_link_libraries(QMI8658 PUBLIC hardware_i2c)