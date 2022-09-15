set(Sources 
	"Concordance.cpp" 
)

set(HeadersSubdir "include/")

set(Headers 
	${HeadersSubdir}Concordance.hpp 
)

add_library(Concordance ${Sources} ${Headers})
target_include_directories(Concordance PUBLIC include)