
########################################################################################
#############               ���� sqlite3 ���ļ�                           ############## 
########################################################################################
LIST (APPEND sqlite3_LIB_SRCS
    #${CMAKE_CURRENT_SOURCE_DIR}/sqlite/shell.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlite/sqlite3.c
    )
    

    
########################################################################################
#############               ���� sqlite3 ��汾��                         ############## 
########################################################################################
set(sqlite3_LIB_VERSION     "3.0.0")
set(sqlite3_LIB_SOVERSION   "1")


   
########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Utils
    ${PROJECT_SOURCE_DIR}/Utils/Sqlite3
    ${PROJECT_SOURCE_DIR}/Utils/Sqlite3/sqlite
)  



########################################################################################
#############               ���� ���ӿ��ļ�Ŀ¼                           ############## 
########################################################################################
link_directories(  
    /usr/local/lib   
    )  


    
########################################################################################
#############           ����Ŀ����ļ� �� �����汾��                      ############## 
########################################################################################
IF (MODULE_sqlite)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (sqlite3lib SHARED      ${sqlite3_LIB_SRCS})  
    add_library (sqlite3s   STATIC      ${sqlite3_LIB_SRCS})  
    
    # ����������ϵ�������ǰ������ײ�Ĺ����⣬����Ҫ����
    add_dependencies (sqlite3lib   utilslib  dl  pthread)
    add_dependencies (sqlite3s     utilslib  dl  pthread)
        
    # ����Ҫ���ӵĹ�����, ���˳����Ǳ�����������ʱ˳��
    target_link_libraries (sqlite3lib   utilslib  dl  pthread)
    target_link_libraries (sqlite3lib   utilslib  dl  pthread)
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(sqlite3lib    PROPERTIES 
                                        VERSION     ${sqlite3_LIB_VERSION} 
                                        SOVERSION   ${sqlite3_LIB_SOVERSION} )
ELSE (MODULE_sqlite)
    MESSAGE(STATUS "Not Include sqlite3 module.")
ENDIF (MODULE_sqlite)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_sqlite)
    add_executable(TestSqlite.elf      sqlite3Test.c)
    add_dependencies(TestSqlite.elf         loglib utilslib sqlite3lib  dl  pthread)
    target_link_libraries(TestSqlite.elf    loglib utilslib sqlite3lib  dl  pthread)

ELSE (TEST_MODULE_FLAG AND MODULE_sqlite)
    MESSAGE(STATUS "Not Include sqlite3 module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_sqlite)



