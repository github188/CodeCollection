########################################################################################
##########               ���� system_instrumentlib ���ļ�                    ########### 
########################################################################################
LIST (APPEND system_instrument_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemInstrument.c
    )


########################################################################################
###########              ���� system_instrumentlib ��汾��                  ########### 
########################################################################################
set(system_instrument_LIB_VERSION     "1.0.0")
set(system_instrument_LIB_SOVERSION   "1")



########################################################################################
##########            ���� ������ļ������ͷ�ļ�Ŀ¼                        ########### 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Log   
    ${PROJECT_SOURCE_DIR}/Log/InstrumentFunctions
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
IF (MODULE_system_instrument)
    add_definitions("-finstrument-functions")
    
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (system_instrumentlib SHARED         ${system_instrument_LIB_SRCS})  
    add_library (system_instruments STATIC            ${system_instrument_LIB_SRCS})  
    
    # ����������ϵ�������ǰ������ײ�Ĺ����⣬����Ҫ����
    add_dependencies (system_instrumentlib   pthread)
    add_dependencies (system_instruments     pthread)
    
    # ����Ҫ���ӵĹ�����, ���˳����Ǳ�����������ʱ˳��
    target_link_libraries (system_instrumentlib  pthread)
    target_link_libraries (system_instruments    pthread)
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(system_instrumentlib   PROPERTIES 
                            VERSION     ${system_instrument_LIB_VERSION} 
                            SOVERSION   ${system_instrument_LIB_SOVERSION} )

ELSE (MODULE_system_instrument)
    MESSAGE(STATUS "Not Include system_instrument module.")
ENDIF (MODULE_system_instrument)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_system_instrument)
    add_definitions("-finstrument-functions")

    add_executable(TestSystemInstrument.elf    system_instrumentTest.cpp)
    add_dependencies(TestSystemInstrument.elf          system_instrumentlib loglib pthread)
    target_link_libraries(TestSystemInstrument.elf     system_instrumentlib loglib pthread)

ELSE (TEST_MODULE_FLAG AND MODULE_system_instrument)
    MESSAGE(STATUS "Not Include system_instrument module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_system_instrument)


