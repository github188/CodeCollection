########################################################################################
#############              ���� hash ���ļ�                               ############## 
########################################################################################
LIST (APPEND hash_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/hash.c
#    ${CMAKE_CURRENT_SOURCE_DIR}/
    )



########################################################################################
#############               ���� hash ��汾��                          ############## 
########################################################################################
set(hash_LIB_VERSION     "1.0.0")
set(hash_LIB_SOVERSION   "1")



########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Settings
    ${PROJECT_SOURCE_DIR}/Settings/Hash
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
IF (MODULE_hash)

    #���ɶ�̬��  ��̬���� STATIC  
    add_library (hashlib     SHARED      ${hash_LIB_SRCS})  
    add_library (hashs       STATIC        ${hash_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(hashlib    PROPERTIES 
                                        VERSION     ${hash_LIB_VERSION} 
                                        SOVERSION   ${hash_LIB_SOVERSION} )
ELSE (MODULE_hash)
    MESSAGE(STATUS "Not Include hash module.")
ENDIF (MODULE_hash)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_hash)
    add_executable(TestHash.elf  test.c)
    add_dependencies(TestHash.elf        hashlib  loglib)
    target_link_libraries(TestHash.elf   hashlib  loglib )

ELSE (TEST_MODULE_FLAG AND MODULE_hash)
    MESSAGE(STATUS "Not Include hash module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_hash)



