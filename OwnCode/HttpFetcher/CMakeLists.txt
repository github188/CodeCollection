
########################################################################################
#############               ���� http_fetch ���ļ�                           ############## 
########################################################################################
LIST (APPEND http_fetch_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/http_error_codes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/http_fetcher.c
#    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
    

    
########################################################################################
#############               ���� http_fetch ��汾��                         ############## 
########################################################################################
set(http_fetch_LIB_VERSION     "1.0.0")
set(http_fetch_LIB_SOVERSION   "1")


   
########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Network
    ${PROJECT_SOURCE_DIR}/Network/HttpFetcher
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
IF (MODULE_http_fetch)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (http_fetchlib SHARED          ${http_fetch_LIB_SRCS})  
    add_library (http_fetchs   STATIC          ${http_fetch_LIB_SRCS})  
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(http_fetchlib     PROPERTIES 
                                            VERSION     ${http_fetch_LIB_VERSION} 
                                            SOVERSION   ${http_fetch_LIB_SOVERSION} )
ELSE (MODULE_http_fetch)
    MESSAGE(STATUS "Not Include http_fetch module.")
ENDIF (MODULE_http_fetch)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_http_fetch)
    add_executable(TestHttpFetcher.elf  test_fetch.c)
    add_dependencies(TestHttpFetcher.elf    http_fetchlib)
    target_link_libraries(TestHttpFetcher.elf   http_fetchlib)

ELSE (TEST_MODULE_FLAG AND MODULE_http_fetch)
    MESSAGE(STATUS "Not Include http_fetch module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_http_fetch)



