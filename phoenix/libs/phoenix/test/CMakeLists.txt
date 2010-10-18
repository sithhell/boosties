
MACRO(ADD_PHOENIX_TEST name)
 ADD_EXECUTABLE(${name} ${name}.cpp)
 ADD_TEST(NAME ${name} COMMAND ${name})
ENDMACRO()

ADD_PHOENIX_TEST(simple)
ADD_PHOENIX_TEST(primitives_tests)
