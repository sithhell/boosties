
MACRO(ADD_PROTO_TEST name)
 ADD_EXECUTABLE(${name} ${name}.cpp)
 ADD_TEST(NAME ${name} COMMAND ${name})
ENDMACRO()

ADD_PROTO_TEST(simple_test)
ADD_PROTO_TEST(phoenix_test)
ADD_PROTO_TEST(splitter)
