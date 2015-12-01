
########################################################################################
#############               ���� lodepng ���ļ�                           ############## 
########################################################################################
LIST (APPEND lodepng_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/lodepng.cpp
    )
    

    
########################################################################################
#############               ���� lodepng ��汾��                         ############## 
########################################################################################
set(lodepng_LIB_VERSION     "1.0.0")
set(lodepng_LIB_SOVERSION   "1")


   
########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Views
    ${PROJECT_SOURCE_DIR}/Views/PNG
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
IF (MODULE_lodepng)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (lodepnglib SHARED      ${lodepng_LIB_SRCS})  
    add_library (lodepngs   STATIC      ${lodepng_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(lodepnglib    PROPERTIES 
                                        VERSION     ${lodepng_LIB_VERSION} 
                                        SOVERSION   ${lodepng_LIB_SOVERSION} )
ELSE (MODULE_lodepng)
    MESSAGE(STATUS "Not Include lodepng module.")
ENDIF (MODULE_lodepng)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_lodepng)
    add_executable(TestLodePNG.elf    lodepngTest.cpp)
    add_dependencies(TestLodePNG.elf      loglib  viewslib lodepnglib )
    target_link_libraries(TestLodePNG.elf loglib  viewslib lodepnglib )

ELSE (TEST_MODULE_FLAG AND MODULE_lodepng)
    MESSAGE(STATUS "Not Include lodepng module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_lodepng)



