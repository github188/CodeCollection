########################################################################################
##########            ���� stackinfolib ���ļ�                    ########### 
########################################################################################
LIST (APPEND stackinfo_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/FunctionStack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ProcessAuxiliaryInfo.c
    )


########################################################################################
#############               ���� stackinfolib ��汾��                    ############## 
########################################################################################
set(stackinfo_LIB_VERSION     "1.0.0")
set(stackinfo_LIB_SOVERSION   "1")



########################################################################################
##########            ���� ������ļ������ͷ�ļ�Ŀ¼                        ########### 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Log   
    ${PROJECT_SOURCE_DIR}/Log/StackInfoDebug
)  
  
  
########################################################################################
##########            ���� ���ӿ��ļ�Ŀ¼                                    ########### 
########################################################################################
link_directories(  
    /usr/local/lib   
    )  

    
########################################################################################
##########        ����Ŀ����ļ� ��������Ҫ������ ����������ϵ               ########### 
########################################################################################
IF (COMPONENT_stackinfo)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (stackinfolib  SHARED         ${stackinfo_LIB_SRCS})  
    add_library (stackinfos    STATIC            ${stackinfo_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(stackinfolib   PROPERTIES 
                            VERSION     ${stackinfo_LIB_VERSION} 
                            SOVERSION   ${stackinfo_LIB_SOVERSION} )

ELSE (COMPONENT_stackinfo)
    MESSAGE(STATUS "Not Include stackinfo module.")
ENDIF (COMPONENT_stackinfo)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG)
    add_executable(TestStackInfo.elf    stackinfoTest.c)
    add_dependencies(TestStackInfo.elf          stackinfolib)
    target_link_libraries(TestStackInfo.elf     stackinfolib)

ELSE (TEST_MODULE_FLAG)
    MESSAGE(STATUS "Not Include jsoncpp module.")
ENDIF (TEST_MODULE_FLAG)


