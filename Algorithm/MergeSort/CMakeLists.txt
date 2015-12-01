########################################################################################
#############              ���� mergesort ���ļ�                               ############## 
########################################################################################
LIST (APPEND mergesort_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/LLMSort.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MergeSort.c
    ${CMAKE_CURRENT_SOURCE_DIR}/StaticMergeSort.c
#    ${CMAKE_CURRENT_SOURCE_DIR}/
    )



########################################################################################
#############               ���� mergesort ��汾��                          ############## 
########################################################################################
set(mergesort_LIB_VERSION     "1.0.0")
set(mergesort_LIB_SOVERSION   "1")



########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Settings
    ${PROJECT_SOURCE_DIR}/Settings/MergeSort
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
IF (MODULE_mergesort)

    #���ɶ�̬��  ��̬���� STATIC  
    add_library (mergesortlib     SHARED      ${mergesort_LIB_SRCS})  
    add_library (mergesorts       STATIC        ${mergesort_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(mergesortlib    PROPERTIES 
                                        VERSION     ${mergesort_LIB_VERSION} 
                                        SOVERSION   ${mergesort_LIB_SOVERSION} )
ELSE (MODULE_mergesort)
    MESSAGE(STATUS "Not Include mergesort module.")
ENDIF (MODULE_mergesort)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_mergesort)
    add_executable(TestMergeSort.elf  testMergeSort.c)
    add_dependencies(TestMergeSort.elf        mergesortlib  loglib)
    target_link_libraries(TestMergeSort.elf   mergesortlib  loglib )

ELSE (TEST_MODULE_FLAG AND MODULE_mergesort)
    MESSAGE(STATUS "Not Include mergesort module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_mergesort)



