#include <iostream>
#include <cassert>
#include "HelpCmd.h"
#include "CommandSet.h"

using namespace std;
using namespace cigma;

// ---------------------------------------------------------------------------

HelpCmd::HelpCmd()
{
    name = "help";
    cmdset = 0;
}


HelpCmd::~HelpCmd()
{
}


void HelpCmd::setCommandSet(CommandSet *cmdset)
{
    this->cmdset = cmdset;

    /* prepare usage list from current set of commands */
    usageList.clear();
    std::string prefix = "   ";

    CommandSet::CmdNames::iterator it;
    for (it = cmdset->names.begin(); it != cmdset->names.end(); ++it)
    {
        usageList.push_back(prefix + (*it));
    }
}


// ---------------------------------------------------------------------------

void HelpCmd::setupOptions(AnyOption *opt)
{
    //std::cout << "Calling cigma::HelpCmd::setupOptions()" << std::endl;

    assert(opt != 0);

    /* prepare preamble */
    opt->addUsage("Usage: cigma <subcommand> [options] [args]");
    opt->addUsage("Type 'cigma help <subcommand>' for help on a specific subcommand.");
    opt->addUsage("");
    opt->addUsage("Available subcommands");

    /* pass contents of usage list to opt object */
    std::vector<std::string>::iterator it;
    for (it = usageList.begin(); it != usageList.end(); ++it)
    {
        opt->addUsage(it->c_str());
    }

    // XXX: need to set at least one flag or option (bug in AnyOption)
    opt->setFlag("help",'h');
}


void HelpCmd::configure(AnyOption *opt)
{
    //std::cout << "Calling cigma::HelpCmd::configure()" << std::endl;

    assert(opt != 0);

    int argc = opt->getArgc();

    subcommand = "";
    if (argc == 0)
    {
        subcommand = "help";
    }
    else
    {
        subcommand = opt->getArgv(0);
        if (argc >= 2)
        {
            // XXX: too many args!
            std::cerr << "Too many arguments! "
                      << "Taking only '"
                      << subcommand
                      << "'" << std::endl;
        }
    }
}


int HelpCmd::run()
{
    //std::cout << "Calling cigma::HelpCmd::run()" << std::endl;


    /* display help for appropriate subcommand */
    CommandSet::CmdMap::iterator it = cmdset->commands.find(subcommand);

    if (it != cmdset->commands.end())
    {
        AnyOption opt;
        Command *cmd = it->second;
        cmd->setupOptions(&opt);
        opt.printUsage();
        return 0;
    }
    else
    {
        cerr << "Unknown command: '" << subcommand << "'" << endl;
    }

    return 1;
}

