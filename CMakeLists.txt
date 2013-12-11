cmake_minimum_required (VERSION 2.8)
project (Https-Tryout)
add_executable (HelloWorld helloWorld.cxx)
add_executable (HttpServer httpServer.cxx
		syncHttpEchoHandler.cxx 
		asyncHttpEchoHandler.cxx
		syncHttp401Handler.cxx
		synchronicity.cxx
		handlers.cxx
		runServer.cxx)
add_executable (HttpClient
		httpClient.cxx)

set(CMAKE_CXX_FLAGS "-std=c++11")

# Use Boost
set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME OFF)
find_package (Boost 1.54.0 REQUIRED system thread regex program_options serialization)
include_directories (${Boost_INCLUDE_DIRS})
set (LIBS ${LIBS} ${Boost_LIBRARIES})


#Use OpenSSL
find_package (OpenSSL 1.0.1 REQUIRED)
include_directories (${OPENSSL_INCLUDE_DIR})
set (LIBS ${LIBS} ${OPENSSL_LIBRARIES})


#Use C++-netlib
find_package (cppnetlib 0.11.0 REQUIRED)
include_directories (${CPPNETLIB_INCLUDE_DIRS})
set (LIBS ${LIBS} ${CPPNETLIB_LIBRARIES})



target_link_libraries (HelloWorld ${LIBS})
target_link_libraries (HttpServer ${LIBS})
target_link_libraries (HttpClient ${LIBS})


# Generate demo certificates
file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/certs)
find_program (OPENSSL_BIN NAMES openssl)
set (KEYFORMAT PEM)
set (CERT_DIR ${CMAKE_BINARY_DIR}/certs)

function (createPKI_keypair STEM)
	file (TO_NATIVE_PATH ${CERT_DIR}/${STEM}.priv.${KEYFORMAT} KEY_PRIV)
	file (TO_NATIVE_PATH ${CERT_DIR}/${STEM}.pub.${KEYFORMAT} KEY_PUB)
	execute_process (
		COMMAND ${OPENSSL_BIN} genpkey -algorithm RSA -outform ${KEYFORMAT} 
		OUTPUT_FILE ${KEY_PRIV})
	execute_process (
		COMMAND ${OPENSSL_BIN} pkey -inform ${KEYFORMAT} -outform ${KEYFORMAT} -pubout 
		INPUT_FILE ${KEY_PRIV}
		OUTPUT_FILE ${KEY_PUB})
endfunction(createPKI_keypair)

function (selfSign STEM)
	file (TO_NATIVE_PATH ${CERT_DIR}/${STEM}.priv.${KEYFORMAT} KEY_PRIV)
	file (TO_NATIVE_PATH ${CERT_DIR}/${STEM}.crt CERT)
	execute_process (
		COMMAND ${OPENSSL_BIN} req -inform ${KEYFORMAT} -outform ${KEYFORMAT}
			-new -subj "/C=NL/L=Maastricht/O=Maastro/OU=I\\&S/CN=SWObjects\ Demo\ ${STEM}"
			-key ${KEY_PRIV} -x509
		OUTPUT_FILE ${CERT})
endfunction(selfSign)

function (createCert REQUEST_STEM CA_STEM PARENT_CA_STEM)
	file (TO_NATIVE_PATH ${CERT_DIR}/${REQUEST_STEM}.priv.${KEYFORMAT} REQ_PRIV)
	file (TO_NATIVE_PATH ${CERT_DIR}/${REQUEST_STEM}-${CA_STEM}.crt CERT)
	file (TO_NATIVE_PATH ${CERT_DIR}/${CA_STEM}.priv.${KEYFORMAT} CA_PRIV)
	file (TO_NATIVE_PATH ${CERT_DIR}/${CA_STEM}.ser CA_SER)

	if (${PARENT_CA_STEM})
		file (TO_NATIVE_PATH ${CERT_DIR}/${CA_STEM}-${PARENT_CA_STEM}.crt CA_CERT)
	else (${PARENT_CA_STEM})
		file (TO_NATIVE_PATH ${CERT_DIR}/${CA_STEM}.crt CA_CERT)
	endif (${PARENT_CA_STEM})

	execute_process(
		COMMAND ${OPENSSL_BIN} req -inform ${KEYFORMAT} -outform ${KEYFORMAT}
			-new -subj "/C=NL/L=Maastricht/O=Maastro/OU=I\\&S/CN=SWObjects\ Demo\ ${REQUEST_STEM}"
			-key ${REQ_PRIV} 
		COMMAND ${OPENSSL_BIN} x509 -req -inform ${KEYFORMAT} -outform ${KEYFORMAT}
			-CA ${CA_CERT} -CAkey ${CA_PRIV} 
			-CAserial ${CA_SER} -CAcreateserial
		OUTPUT_FILE ${CERT})
endfunction(createCert)

createPKI_keypair(root-ca)
selfSign(root-ca)

createPKI_keypair(server)
createCert(server root-ca "")

createPKI_keypair(client)
createCert(client root-ca "")

createPKI_keypair(secondRoot-ca)
selfSign(secondRoot-ca)
# vim:set tabstop=4 shiftwidth=4 :
