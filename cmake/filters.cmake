include_guard(GLOBAL)

function(create_filter filter folder)
    source_group("Header Files\\${filter}" REGULAR_EXPRESSION "include/${folder}/.*\.(hpp|h)")
    source_group("Source Files\\${filter}" REGULAR_EXPRESSION "src/${folder}/.*\.(cpp|cc|m|mm)")
endfunction()

function(create_filter_lib filter folder lib)
    source_group("Header Files\\${filter}" REGULAR_EXPRESSION "include/${lib}/${folder}/.*\.(hpp|h)")
    source_group("Source Files\\${filter}" REGULAR_EXPRESSION "src/${folder}/.*\.(cpp|cc|m|mm)")
endfunction()