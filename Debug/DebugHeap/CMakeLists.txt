########################################################################################
##########         ���� debug_heaplib ���ļ�                                  ########### 
########################################################################################
LIST (APPEND log_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/DebugHeap.c
    )



########################################################################################
#############               ���� debug_heaplib ��汾��                    ############## 
########################################################################################
set(debug_heap_LIB_VERSION     "1.0.0")
set(debug_heap_LIB_SOVERSION   "1")



########################################################################################
##########            ���� ������ļ������ͷ�ļ�Ŀ¼                        ########### 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Log   
    ${PROJECT_SOURCE_DIR}/Log/DebugHeap
)  
  
  
########################################################################################
##########            ���� ���ӿ��ļ�Ŀ¼                                    ########### 
########################################################################################
link_directories(  
    /usr/local/lib   
    )  


########################################################################################
##########            ����Ŀ����ļ� �� �����汾��                           ########### 
########################################################################################
IF (COMPONENT_debug_heap)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (debug_heaplib SHARED          ${log_LIB_SRCS})  
    add_library (debug_heaps STATIC            ${log_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(debug_heaplib   PROPERTIES 
                            VERSION         ${debug_heap_LIB_VERSION} 
                            SOVERSION       ${debug_heap_LIB_SOVERSION} )
    
ELSE (COMPONENT_debug_heap)
    MESSAGE(STATUS "Not Include debug heap module.")
ENDIF (COMPONENT_debug_heap)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG)
    add_executable(Testdebug_heap.elf    debug_heapTest.c)
    add_dependencies(Testdebug_heap.elf          debug_heaplib )
    target_link_libraries(Testdebug_heap.elf     debug_heaplib )

ELSE (TEST_MODULE_FLAG)
    MESSAGE(STATUS "Not Include jsoncpp module.")
ENDIF (TEST_MODULE_FLAG)


