
MACRO(ADD_OPTION_TEST name)
 ADD_EXECUTABLE(${name} ${name}.cpp)
 ADD_TEST(NAME ${name} COMMAND ${name})
ENDMACRO()

ADD_OPTION_TEST(default)
ADD_OPTION_TEST(options)
ADD_OPTION_TEST(proto_params)
