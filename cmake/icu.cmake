if(NOT DEFINED _ICU_LIBRARY )

	set ( ICU_PATH ${PROJECT_ROOT}/src/3rd/icu )	     

	set ( ICU_INSTALL_PATH ${ICU_PATH}/install )

	set ( ICU_SOURCE_PATH ${ICU_PATH}/source )

	set ( ICU_INC ${ICU_INSTALL_PATH}/include )
	set ( ICU_LIB_PATH ${ICU_INSTALL_PATH}/lib )
    
    set ( ICU_LIB 
          ${ICU_LIB_PATH}/libicui18n.a
          ${ICU_LIB_PATH}/libicuio.a
          ${ICU_LIB_PATH}/libicule.a
          ${ICU_LIB_PATH}/libiculx.a
          ${ICU_LIB_PATH}/libicutest.a
          ${ICU_LIB_PATH}/libicutu.a
          ${ICU_LIB_PATH}/libicuuc.a	 
          ${ICU_LIB_PATH}/libicudata.a
        )     
	     
#----------------------------------------------
	message ( ${ICU_SOURCE_PATH} )

	find_file ( ICU_SOURCE_FOUND 
	            configure
	            PATHS  ${ICU_SOURCE_PATH} 
	          )
	            
	if( ${ICU_SOURCE_FOUND} STREQUAL "ICU_SOURCE_FOUND-NOTFOUND" )
		message(FATAL_ERROR "[E] icu source not found !!!" )
	endif()	            

#----------------------------------------------

	find_file ( ICU_FOUND 
	            libicuio.a
	            PATHS ${ICU_LIB_PATH} 
	          )

	if( ${ICU_FOUND} STREQUAL "ICU_FOUND-NOTFOUND" )

		execute_process( COMMAND ./configure --prefix=${ICU_INSTALL_PATH} --enable-shared=no --enable-static=yes --disable-samples CFLAGS=-fPIC CXXFLAGS=-fPIC
                         WORKING_DIRECTORY ${ICU_SOURCE_PATH} )
                         
		execute_process( COMMAND make
                         WORKING_DIRECTORY ${ICU_SOURCE_PATH}
                         RESULT_VARIABLE RESULT_MAKE_ICU )
                         
		execute_process( COMMAND make install
                         WORKING_DIRECTORY ${ICU_SOURCE_PATH} )
        endif()                 
        if( ${RESULT_MAKE_ICU} )
        	message(FATAL_ERROR "[E] building icu failed" )
        endif()

set ( _ICU_LIBRARY YES )

endif ()

#------------------------------------------------------














