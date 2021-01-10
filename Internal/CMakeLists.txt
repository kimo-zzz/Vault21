file(
    GLOB_RECURSE INTERNAL_SOURCE 
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c*"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h*"
)

add_library(Internal MODULE ${INTERNAL_SOURCE})

foreach(FileName IN ITEMS ${INTERNAL_SOURCE})
    get_filename_component(File_Path "${FileName}" PATH)
    file(RELATIVE_PATH File_Path_Rel "${CMAKE_CURRENT_SOURCE_DIR}" "${File_Path}")
    string(REPLACE "/" "\\" Group_Name "${File_Path_Rel}")
	target_include_directories(Internal PRIVATE "${Group_Name}")
    source_group("${Group_Name}" FILES "${FileName}")
endforeach()

target_link_libraries(Internal ${D3D9_LIBRARY} ${D3DX9_LIBRARY} minhook ImGui)