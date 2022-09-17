set(Sources 
	"Concordance.cpp" 
	"OutputFormattings.cpp"
	"TextDocumentTraveller.cpp"
	"WordSanitizer.cpp"
)

set(HeadersSubdir "include/")

set(Headers 
	${HeadersSubdir}Concordance.hpp 
	${HeadersSubdir}OutputFormattings.hpp 
	${HeadersSubdir}TextDocumentTraveller.hpp 
	${HeadersSubdir}WordSanitizer.hpp 
	${HeadersSubdir}Singleton.hpp 
)

add_library(Concordance ${Sources} ${Headers})
target_include_directories(Concordance PUBLIC include)