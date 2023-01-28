#include <iostream>
#include <span>
#include <algorithm>

#include "Concordance.hpp"
#include "OutputFormattings.hpp"

//INTERNAL CLASS DECLARATIONS
namespace
{

//==========================================================================|
//								CommandLineArg								|
//==========================================================================|
// @brief: Wrapper object which holds the command line args that app started|
//==========================================================================|
struct CommandLineArg
{
    std::string key;
    std::vector<std::string> values;
};

//==========================================================================|
//								  AppExecutor								|
//==========================================================================|
// @brief: Base class that provides an interface that will control the run  |
//         mode of the application according to the command line arguments  |
//                                                                          |
//         example: Application will run differently if -h or --help is     |
//         provided as input and differently if -f or --files are given     |
//==========================================================================|
class AppExecutor
{
public:
    AppExecutor(const std::vector<CommandLineArg> &execution_args);
    virtual ~AppExecutor() {}
    virtual int execute() = 0;

protected:
    const std::vector<CommandLineArg> &getArgs() const;

private:
    std::vector<CommandLineArg> m_args;
};

//==========================================================================|
//							        HelpExecutor							|
//==========================================================================|
// @brief: Concrete class that wrans the application in help mode (prints   |
//         the necessary command line arguments                             |
//==========================================================================|
class HelpExecutor : public AppExecutor
{
public:
    HelpExecutor(const std::vector<CommandLineArg> &execution_args);
    int execute() override;
};

//==========================================================================|
//					      ConcordanceGenerationExecutor			    		|
//==========================================================================|
// @brief: Concrete class that creates a concordance of a given file        |
//==========================================================================|
class ConcordanceGenerator : public AppExecutor
{
public:
    ConcordanceGenerator(const std::vector<CommandLineArg> &execution_args);
    int execute() override;
};

//==========================================================================|
//					            ExecutionFactory			        		|
//==========================================================================|
// @brief: Retrieves the proper application executor according to the given |
//         command line arguments                                           |
//==========================================================================|
class ExecutionFactory
{
public:
    std::unique_ptr<AppExecutor> createExecutor(const std::vector<CommandLineArg> &execution_args);
};

//END OF INTERNAL CLASS DECLARATIONS

//INTERNAL AUX FUNCTIONS
static inline bool isKey(const std::string &input)
{
    return input.starts_with("-");
}

static inline bool hasFilledKey(const CommandLineArg &arg)
{
    return arg.key.size();
}

static inline bool hasFilledValues(const CommandLineArg &arg)
{
    return arg.values.size();
}

static void pushFilledBuffer(CommandLineArg &buffer, std::vector<CommandLineArg> &parsed_args)
{
    if( hasFilledKey(buffer) || hasFilledValues(buffer) ){
        parsed_args.push_back(std::move(buffer));
        buffer = CommandLineArg();
    }
}

static std::vector<CommandLineArg> parseCommandLineArguments(const std::span<char *> &entries)
{
    std::vector<CommandLineArg> args;

    CommandLineArg buffer;

    for( auto it = entries.begin() + 1; it != entries.end(); ++it ){
        std::string input = *it;

        if( isKey(input) ){
            pushFilledBuffer(buffer, args);
            buffer.key = input;
        
        } else if ( input.size() ){
            buffer.values.push_back(input);
        }
    }

    pushFilledBuffer(buffer, args);

    return args;
}

static void printApplicationHelp()
{
    std::cout << "|==========================================================================|" << std::endl;
    std::cout << "|             C O N C O R D A N C E   C R E A T O R  v0.0                  |" << std::endl;
    std::cout << "|==========================================================================|" << std::endl;
    std::cout << std::endl;
    std::cout << "Applicable Arguments:" << std::endl;
    std::cout << "-h, --help: Help of application" << std::endl;
    std::cout << "-f, --file: Plain text document that will generate a concordance" << std::endl;

}

static bool refersToFile(const CommandLineArg &arg)
{
    if( arg.key == "-f" ) return true;
    if( arg.key == "--file" ) return true;
    return false;
}

static std::vector<std::string> joinValues(const std::vector<CommandLineArg> &all_args)
{
    std::vector<std::string> joined;

    std::for_each(all_args.begin(), all_args.end(), [&joined](const CommandLineArg &arg){
        std::copy(arg.values.begin(), arg.values.end(), std::back_inserter(joined));
    });

    return joined;
}

static bool existHelpFlag(const std::vector<CommandLineArg> &all_args)
{
    auto is_help = [](const CommandLineArg &arg){
        return arg.key == "-h" || arg.key == "--help";
    };

    return std::any_of(all_args.begin(), all_args.end(), is_help);
}

static void printConcordance(const Concordance &concordance)
{
    auto print_to_console = [](WordIndex index, const Word &word, const Occurrences &occurrences){
        std::cout <<  joinConcordanceLine(index, word, occurrences) << std::endl;
    };

    concordance.forEachWord(print_to_console);
}
//END OF INTERNAL AUX FUNCTIONS


//INTERNAL CLASS DEFINITIONS
AppExecutor::AppExecutor(const std::vector<CommandLineArg> &execution_args) : m_args(execution_args)
{
}
const std::vector<CommandLineArg> &AppExecutor::getArgs() const
{
    return m_args;
}

HelpExecutor::HelpExecutor(const std::vector<CommandLineArg> &execution_args) : AppExecutor(execution_args)
{
}

int HelpExecutor::execute()
{
    printApplicationHelp();
    return 0;
}

ConcordanceGenerator::ConcordanceGenerator(const std::vector<CommandLineArg> &execution_args) : AppExecutor(execution_args)
{
}

int ConcordanceGenerator::execute()
{
    auto does_not_refer_to_file = [](const CommandLineArg &arg){
        return arg.key.size() && !refersToFile(arg);
    };

    std::vector<CommandLineArg> args = getArgs();
    
    auto removed = std::remove_if(args.begin(), args.end(), does_not_refer_to_file);
    args.erase(removed, args.end());
    
    std::vector<std::string> filepaths = joinValues(args);

    if( filepaths.size() == 1 ){
        Concordance concordance = Concordance::makeFromFile(filepaths.front());
        printConcordance(concordance);
        return 0;
    
    } else {
        HelpExecutor executor(args);
        executor.execute();
        return -1;
    }
}



std::unique_ptr<AppExecutor> ExecutionFactory::createExecutor(const std::vector<CommandLineArg> &execution_args)
{
    if( execution_args.empty() ){
        return std::make_unique<HelpExecutor>(execution_args);
    } else if( existHelpFlag(execution_args) ){
        return std::make_unique<HelpExecutor>(execution_args);
    } else {
        return std::make_unique<ConcordanceGenerator>(execution_args);
    }
}

//END OF INTERNAL CLASS DEFINITIONS
}//END OF ANONYMOUS NAMESPACE


int main(int argc, char *argv[])
{
    std::vector<CommandLineArg> args = parseCommandLineArguments({argv, static_cast<size_t>(argc)});

    std::unique_ptr<AppExecutor> executor = ExecutionFactory().createExecutor(args);
    return executor->execute();
}
