cmake_minimum_required(VERSION 3.12)

project(ChatServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#опция CMAKE_INCLUDE_CURRENT_DIR обязательно должна быть включена,
#если вы не меняли пути генерации файлов для Qt.
#при использовании .ui файлов, Qt на их основе генерирует промежуточные
#файлы исходников, и будет складывать их в директории сборки.
#Чтобы среда их видела, необходимо добавить в директории для включения
#(include, директории, хранящие подключаемые .h файлы), директорию сборки:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if ( CMAKE_COMPILER_IS_GNUCC )
   set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
if ( MSVC )
   set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

#здесь мы создаем переменную CHAT_LIBRARIES, в которой будем хранить
#все подключаемые к проекту библиотеки, в т.ч. и библиотеки Qt
set(CHAT_LIBRARIES)

#здесь мы создаем и заполняем список подключаемых модулей Qt
#если нужно добавить модули, добавьте их в этот список по образцу:
#set(Qt5Modules [модуль 1] [модуль 2] ... [модуль n])
set(Qt6Modules Core Widgets Network Sql)

#здесь мы формируем из списка модулей список библиотек,
#которые требуется подключить, и складываем их в CHAT_LIBRARIES
foreach(QtModule IN LISTS Qt6Modules)
 list(APPEND CHAT_LIBRARIES Qt6::${QtModule})
endforeach(QtModule)

#здесь проверяется наличие всех необходимых модулей и компонентов Qt
find_package(Qt6 COMPONENTS ${Qt6Modules} REQUIRED LinguistTools)
#здесь мы формируем список всех исходников, ассоциированных с
#проектом Chat.
#на самом деле, обязательными являются только SRC RC и UI,
#но я добавляю так же HEADERS, что бы они так же отображались
#в дереве проектов.

#!!! не забывайте, что при добавлении/удалении файлов  обязательно
#нужно запускать CMake
file(GLOB_RECURSE SRC_FILES *.cpp)
file(GLOB_RECURSE HEADERS_FILES *.h)
file(GLOB_RECURSE RC_FILES *.qrc)
file(GLOB_RECURSE UI_FILES *.ui)


#здесь мы включаем обязательные для нас опции
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#вызываем обработку .ui и .qrc файлов. Здесь же происходит генерация
#промежуточных исходников.
qt6_wrap_ui(QUI ${UI_FILES})
qt6_add_resources(QRC ${RC_FILES})



#подключаем файлы трансляции(переводов)
qt6_add_translation(QM_FILES ${TS_FILES})

add_executable(ChatServer ${QM_FILES} ${SRC_FILES} ${HEADERS_FILES}
 ${RC_FILES} ${UI_FILES}
 mainwindow.h mainwindow.cpp mainwindow.ui

 dbconfiguratation.h dbconfiguratation.cpp dbconfiguratation.ui



 IJsonCodable.h

 TCPServer.h
 ConnectionHandler.h
 ConnectionHandler.cpp
 TCPServer.cpp
 mysqldatabase.h mysqldatabase.cpp

 userwidget.h userwidget.cpp userwidget.ui
 messagewidget.h messagewidget.cpp messagewidget.ui
 administrationdb.h administrationdb.cpp administrationdb.ui

)

#подключаем к проекту библиотеки
target_link_libraries(ChatServer
 PUBLIC
 ${CHAT_LIBRARIES}
 )