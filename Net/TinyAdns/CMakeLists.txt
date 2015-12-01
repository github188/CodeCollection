########################################################################################
#############              ���� tadns ���ļ�                               ############## 
########################################################################################
LIST (APPEND tadns_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/tadns.c  
    )



########################################################################################
#############               ���� tadns ��汾��                          ############## 
########################################################################################
set(tadns_LIB_VERSION     "1.42.0")
set(tadns_LIB_SOVERSION   "1")


    
########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Network
    ${PROJECT_SOURCE_DIR}/Network/TinyAdns
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
IF (MODULE_tadns)

    #���ɶ�̬��  ��̬���� STATIC  
    add_library (tadnslib SHARED      ${tadns_LIB_SRCS})  
    add_library (tadnss STATIC        ${tadns_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(tadnslib   PROPERTIES 
                                    VERSION     ${tadns_LIB_VERSION} 
                                    SOVERSION   ${tadns_LIB_SOVERSION} )
ELSE (MODULE_tadns)
    MESSAGE(STATUS "Not Include tadns module.")
ENDIF (MODULE_tadns)





########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_tadns)
    add_executable(TestTinyAdns.elf    tinyadnsTest.c)
    add_dependencies(TestTinyAdns.elf          tadnslib loglib)
    target_link_libraries(TestTinyAdns.elf     tadnslib loglib)

ELSE (TEST_MODULE_FLAG AND MODULE_tadns)
    MESSAGE(STATUS "Not Include tiny adns module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_tadns)

