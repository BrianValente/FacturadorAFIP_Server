cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Afip VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories("/usr/local/opt/openssl/include")
include_directories("/usr/local/include")
#file(GLOB SOURCES "*.cpp")

SET (SOURCES
	src/main.cpp
	src/invoice.cpp
	src/invoice.hpp
	src/printer.cpp
	src/printer.hpp
	src/websocket.cpp
	src/websocket.hpp
    src/config.cpp
    src/config.hpp
    src/database.cpp
    src/database.hpp
	src/afip/login.cpp
	src/afip/login.hpp
	src/afip/wsfe.cpp
	src/afip/wsfe.hpp
	src/soap/afipC.cpp
	src/soap/soapLoginCmsSoapBindingProxy.cpp
	src/soap/afipLoginCmsSoapBindingProxy.cpp
	src/soap/soapServiceSoap12Proxy.cpp
	src/soap/afipServiceSoapProxy.cpp
	src/soap/soapServiceSoapProxy.cpp
	src/soap/soapC.cpp 
	src/soap/xcode.cpp)

#link_directories("/usr/local/lib")

include_directories("/usr/local/opt/openssl/include")
link_directories("/usr/local/opt/openssl/lib")

find_library(SSL_LIBRARY
    NAMES ssl libssl
    HINTS "/usr/local/opt/openssl/lib" "/usr/lib"
)

find_library(CRYPTO_LIBRARY
    NAMES crypto libcrypto
    HINTS "/usr/local/opt/openssl/lib" "/usr/lib"
)

add_executable("${PROJECT_NAME}" "${SOURCES}")
target_link_libraries("${PROJECT_NAME}" PUBLIC mariadb cairo cups gsoapssl++ tinyxml2 z pthread dl boost_filesystem "${SSL_LIBRARY}" "${CRYPTO_LIBRARY}")
target_link_options("${PROJECT_NAME}" PUBLIC "-L/usr/local/lib")
