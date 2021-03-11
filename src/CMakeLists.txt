add_compile_options(-Wall -Wextra -pedantic -Wno-narrowing -Wno-sign-compare) #-fsanitize=address -fno-omit-frame-pointer -O1)
#add_link_options(-fsanitize=address -fno-omit-frame-pointer)

add_executable(vm main.cpp parser.cpp instructions.cpp unix_terminal.cpp)
add_executable(vm_tests main.cpp parser.cpp instructions.cpp unix_terminal.cpp test_decl_main.cpp test_instructions.cpp test_parser.cpp)
