########################################################################################
#############                ���� mountusb ���ļ�                         ############## 
########################################################################################
LIST (APPEND mountusb_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/mountusb.c
    )
    
    

########################################################################################
#############                ���� mountusb ��汾��                       ############## 
########################################################################################
set(mountusb_LIB_VERSION     "1.42.0")
set(mountusb_LIB_SOVERSION   "1")




########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Systerm
    ${PROJECT_SOURCE_DIR}/Systerm/MountUSB
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
IF (MODULE_mountusb)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (mountusblib SHARED      ${mountusb_LIB_SRCS})  
    add_library (mountusbs STATIC        ${mountusb_LIB_SRCS})  
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(mountusblib   PROPERTIES 
                                        VERSION     ${mountusb_LIB_VERSION} 
                                        SOVERSION   ${mountusb_LIB_SOVERSION} )
ELSE (MODULE_mountusb)
    MESSAGE(STATUS "Not Include mountusb module.")
ENDIF (MODULE_mountusb)




########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_mountusb)
    add_executable(TestMountUSB.elf    mountusbTest.c)
    add_dependencies(TestMountUSB.elf          mountusblib loglib)
    target_link_libraries(TestMountUSB.elf     mountusblib loglib)

ELSE (TEST_MODULE_FLAG AND MODULE_mountusb)
    MESSAGE(STATUS "Not Include mount usb module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_mountusb)