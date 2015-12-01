
########################################################################################
#############               ���� pic_convertion ���ļ�                          ############## 
########################################################################################
LIST (APPEND pic_convertion_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/convert.c
    ${CMAKE_CURRENT_SOURCE_DIR}/string_parser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tools.c
    )
    
    

########################################################################################
#############               ����Դ����Ŀ¼                                ############## 
########################################################################################
#add_subdirectory( Gif_2_HexData )
#add_subdirectory( HexData_2_Gif )
#add_subdirectory( text_split )

#MESSAGE("${CMAKE_CURRENT_SOURCE_DIR} status.")


 
########################################################################################
#############               ���� pic_convertion ��汾��                        ############## 
########################################################################################
set(pic_convertion_LIB_VERSION     "1.0.0")
set(pic_convertion_LIB_SOVERSION   "1")


   
########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Views
    ${PROJECT_SOURCE_DIR}/Views/PicConversion
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
IF (MODULE_pic_convertion)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (pic_convertionlib SHARED      ${pic_convertion_LIB_SRCS})  
    add_library (pic_convertions   STATIC      ${pic_convertion_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(pic_convertionlib   PROPERTIES 
                                        VERSION     ${pic_convertion_LIB_VERSION} 
                                        SOVERSION   ${pic_convertion_LIB_SOVERSION} )
ELSE (MODULE_pic_convertion)
    MESSAGE(STATUS "Not Include pic_convertion module.")
ENDIF (MODULE_pic_convertion)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_pic_convertion)
    add_executable(TestPicConversion.elf    convertTest.c)
    add_dependencies(TestPicConversion.elf      loglib  viewslib pic_convertionlib )
    target_link_libraries(TestPicConversion.elf loglib  viewslib pic_convertionlib )

ELSE (TEST_MODULE_FLAG AND MODULE_pic_convertion)
    MESSAGE(STATUS "Not Include pic_convertion module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_pic_convertion)



