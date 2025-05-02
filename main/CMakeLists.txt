#idf_component_register(SRCS "main.c" "esp_driver.c" "fonts.c" "gui_simple.c"
#                    INCLUDE_DIRS ".")


file(GLOB_RECURSE SOURCES "*.c")

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS ".")
