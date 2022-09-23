
project( tests )

add_subdirectory( auto )

if( BUILD_MD4QT_BENCHMARK )
	add_subdirectory( md_benchmark )
endif( BUILD_MD4QT_BENCHMARK )
