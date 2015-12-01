########################################################################################
#############               ���� base64cpp ���ļ�                            ############## 
########################################################################################
LIST (APPEND base64cpp_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cdecode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cencode.c
    )
    
   
       
########################################################################################
#############               ���� base64cpp ��汾��                          ############## 
########################################################################################
set(base64cpp_LIB_VERSION     "1.2.1")
set(base64cpp_LIB_SOVERSION   "1")



########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Crypto
    ${PROJECT_SOURCE_DIR}/Crypto/Base64Cpp
    ${PROJECT_SOURCE_DIR}/Crypto/Base64Cpp/includes   
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
IF (MODULE_base64cpp)

    #���ɶ�̬��  ��̬���� STATIC  
    add_library (base64cpplib  SHARED          ${base64cpp_LIB_SRCS})  
    add_library (base64cpps    STATIC          ${base64cpp_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(base64cpplib     PROPERTIES 
                                        VERSION     ${base64cpp_LIB_VERSION} 
                                        SOVERSION   ${base64cpp_LIB_SOVERSION} )
ELSE (MODULE_base64cpp)
    MESSAGE(STATUS "Not Include base64cpp module.")
ENDIF (MODULE_base64cpp)




########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_base64cpp)
    add_executable(TestBase64cpp.elf    test/base64cppTest.c)
    add_dependencies(TestBase64cpp.elf          base64cpplib loglib)
    target_link_libraries(TestBase64cpp.elf     base64cpplib loglib)

ELSE (TEST_MODULE_FLAG AND MODULE_base64cpp)
    MESSAGE(STATUS "Not Include base64c module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_base64cpp)





