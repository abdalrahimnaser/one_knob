include(${CMAKE_CURRENT_LIST_DIR}/../file_list_gen.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../user_config.cmake)

idf_component_register(
    SRCS ${LV_EDITOR_PROJECT_SOURCES}
    INCLUDE_DIRS
        "${CMAKE_CURRENT_LIST_DIR}/.."
        "${CMAKE_CURRENT_LIST_DIR}/../screens"
        "${CMAKE_CURRENT_LIST_DIR}/../fonts"
        "${CMAKE_CURRENT_LIST_DIR}/../images"
        "${CMAKE_CURRENT_LIST_DIR}/../widgets"
    REQUIRES lvgl
)

target_include_directories(${COMPONENT_LIB} PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/../../managed_components/lvgl__lvgl"
    "${CMAKE_CURRENT_LIST_DIR}/../../managed_components/lvgl__lvgl/src"
)

target_compile_definitions(${COMPONENT_LIB} PUBLIC
    LV_LVGL_H_INCLUDE_SIMPLE
    LV_CONF_INCLUDE_SIMPLE
)