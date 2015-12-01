
########################################################################################
#############               ���� tinyhttpd ���ļ�                           ############## 
########################################################################################
LIST (APPEND tinyhttpd_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/TinyHttpd.c
    ##${CMAKE_CURRENT_SOURCE_DIR}/
    )
    

    
########################################################################################
#############               ���� tinyhttpd ��汾��                         ############## 
########################################################################################
set(tinyhttpd_LIB_VERSION     "1.0.0")
set(tinyhttpd_LIB_SOVERSION   "1")


   
########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Monitors
    ${PROJECT_SOURCE_DIR}/Monitors/TinyHttpd
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
IF (MODULE_tinyhttpd)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (tinyhttpdlib SHARED      ${tinyhttpd_LIB_SRCS})  
    add_library (tinyhttpds   STATIC      ${tinyhttpd_LIB_SRCS})  
    
    # ����������ϵ�������ǰ������ײ�Ĺ����⣬����Ҫ����
    add_dependencies (tinyhttpdlib  loglib threadlib pthread)
    add_dependencies (tinyhttpds    loglib threadlib pthread)
    
    # ����Ҫ���ӵĹ�����, ���˳����Ǳ�����������ʱ˳��
    target_link_libraries (tinyhttpdlib  loglib threadlib pthread)
    target_link_libraries (tinyhttpds    loglib threadlib pthread)
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(tinyhttpdlib  PROPERTIES 
                                        VERSION     ${tinyhttpd_LIB_VERSION} 
                                        SOVERSION   ${tinyhttpd_LIB_SOVERSION} )
ELSE (MODULE_tinyhttpd)
    MESSAGE(STATUS "Not Include tinyhttpd module.")
ENDIF (MODULE_tinyhttpd)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_tinyhttpd)
    add_executable(TestTinyHttpd.elf    tinyhttpdTest.c)
    add_dependencies(TestTinyHttpd.elf      loglib monitorlib threadlib pthread tinyhttpdlib)
    target_link_libraries(TestTinyHttpd.elf loglib monitorlib threadlib pthread tinyhttpdlib)

ELSE (TEST_MODULE_FLAG AND MODULE_tinyhttpd)
    MESSAGE(STATUS "Not Include tinyhttpd module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_tinyhttpd)



