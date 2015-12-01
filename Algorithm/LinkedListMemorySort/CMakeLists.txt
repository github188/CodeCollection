########################################################################################
#############              ���� llmsort ���ļ�                               ############## 
########################################################################################
LIST (APPEND llmsort_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/LLMSort.c
#    ${CMAKE_CURRENT_SOURCE_DIR}/
    )



########################################################################################
#############               ���� llmsort ��汾��                          ############## 
########################################################################################
set(llmsort_LIB_VERSION     "1.0.0")
set(llmsort_LIB_SOVERSION   "1")



########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Settings
    ${PROJECT_SOURCE_DIR}/Settings/LinkedListMemorySort
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
IF (MODULE_llmsort)

    #���ɶ�̬��  ��̬���� STATIC  
    add_library (llmsortlib     SHARED      ${llmsort_LIB_SRCS})  
    add_library (llmsorts       STATIC        ${llmsort_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(llmsortlib    PROPERTIES 
                                        VERSION     ${llmsort_LIB_VERSION} 
                                        SOVERSION   ${llmsort_LIB_SOVERSION} )
ELSE (MODULE_llmsort)
    MESSAGE(STATUS "Not Include llmsort module.")
ENDIF (MODULE_llmsort)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_llmsort)
    add_executable(TestLLMSort.elf  llmsortTest.c)
    add_dependencies(TestLLMSort.elf        llmsortlib  loglib)
    target_link_libraries(TestLLMSort.elf   llmsortlib  loglib )

ELSE (TEST_MODULE_FLAG AND MODULE_llmsort)
    MESSAGE(STATUS "Not Include jsoncpp module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_llmsort)



