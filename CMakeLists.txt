cmake_minimum_required(VERSION 3.31.11) # define a versao minima do cmake 

project(image_lib) # cria o projeto

message(STATUS "Raiz do projeto: ${CMAKE_SOURCE_DIR}")
message(STATUS "Nome do projeto: ${PROJECT_NAME}")

set(IMAGE_LIB Image_lib) # Define a variavel PESSOA_LIBRARY

add_executable(${PROJECT_NAME}) # cria a entidade pro executavel
add_library(${IMAGE_LIB}) # cria a entidade pra biblioteca

target_sources(
    ${PROJECT_NAME}
    PRIVATE
        src/main.cpp
) # Define os arquivos fontes para o nosso projeto

target_sources(
    ${IMAGE_LIB} # Biblioteca
    PRIVATE
        src/image.cpp # implementacoes
        src/perl.cpp
        src/utils.cpp
        src/core/bmp/imageBmp.cpp
    
    PUBLIC # Configuracao para os headers
        FILE_SET HEADERS 
        BASE_DIRS include
        FILES
            include/bmpImageHeaders.hpp
            include/core.hpp
            include/image.hpp
            include/perl.hpp
            include/utils.hpp
) # Difine o targe para a biblioteca pessoa

target_link_libraries( # linka bilioteca com executaveis 
    ${PROJECT_NAME}
    PRIVATE 
        ${IMAGE_LIB}   
)

set_target_properties( # Muda as propriedade de um target qualquer
    ${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)