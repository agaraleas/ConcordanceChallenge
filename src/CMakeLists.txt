set(Sources 
	"Concordance.cpp" 
	"OutputFormattings.cpp"
)

set(HeadersSubdir "include/")

set(Headers 
	${HeadersSubdir}Concordance.hpp 
	${HeadersSubdir}OutputFormattings.hpp 
)

add_library(Concordance ${Sources} ${Headers})
target_include_directories(Concordance PUBLIC include)