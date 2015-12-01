########################################################################################
#############               ���� base64c ���ļ�                            ############## 
########################################################################################
LIST (APPEND base64c_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.c
    )
    
   
       
########################################################################################
#############               ���� base64c ��汾��                          ############## 
########################################################################################
set(base64c_LIB_VERSION     "1.0.0")
set(base64c_LIB_SOVERSION   "1")



########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Crypto
    ${PROJECT_SOURCE_DIR}/Crypto/Base64C   
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
IF (MODULE_base64c)

    #���ɶ�̬��  ��̬���� STATIC  
    add_library (base64clib  SHARED          ${base64c_LIB_SRCS})  
    add_library (base64cs    STATIC          ${base64c_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(base64clib     PROPERTIES 
                                        VERSION     ${base64c_LIB_VERSION} 
                                        SOVERSION   ${base64c_LIB_SOVERSION} )
ELSE (MODULE_base64c)
    MESSAGE(STATUS "Not Include base64c module.")
ENDIF (MODULE_base64c)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_base64c)
    add_executable(TestBase64c.elf    base64cTest.c)
    add_dependencies(TestBase64c.elf          base64clib loglib)
    target_link_libraries(TestBase64c.elf     base64clib loglib)

ELSE (TEST_MODULE_FLAG AND MODULE_base64c)
    MESSAGE(STATUS "Not Include base64c module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_base64c)


