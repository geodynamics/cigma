#include <iostream>
#include <cstdlib>
#include <cassert>
#include "CommandSet.h"
#include "HelpCmd.h"
#include "ExtractCmd.h"
#include "EvalCmd.h"
#include "CompareCmd.h"

cigma::CommandSet::CommandSet()
{
}

cigma::CommandSet::~CommandSet()
{
    for(CmdMap::iterator i = commands.begin(); i != commands.end(); ++i)
    {
        Command *cmd = i->second;
        assert(cmd != 0);
        delete cmd;
    }
}

void cigma::CommandSet::initialize()
{
    /* create help command separately */
    HelpCmd *help = new HelpCmd();

    /* assemble set of commands */
    addCommand(help);
    addCommand(new ExtractCmd());
    addCommand(new EvalCmd());
    addCommand(new CompareCmd());

    /* once assembled, pass set of commands to help command */
    help->setCmdMap(&commands);
}

void cigma::CommandSet::addCommand(Command *cmd)
{
    assert(cmd != 0);
    commands[cmd->name] = cmd;
}

cigma::Command *
cigma::CommandSet::getCommand(std::string name)
{
    Command *cmd = 0;
    CmdMap::iterator it = commands.find(name);
    if (it != commands.end())
    {
        cmd = it->second;
    }
    return cmd;
}

int cigma::CommandSet::main(int argc, char *argv[])
{
    AnyOption *opt = new AnyOption();
    Command *cmd = 0;
    int status = 0;
    bool helpFlag = false;

    /* Check argv[1] for a command, but intercept help requests */
    if (argc > 1)
    {
        if ((strcmp(argv[1], "--help") == 0) ||
            (strcmp(argv[1], "-h") == 0))
        {
            cmd = getCommand("help");
            helpFlag = true;
        }
        else
        {
            cmd = getCommand(argv[1]);
        }
    }
    else
    {
        cmd = getCommand("help");
        helpFlag = true;
    }

    if (cmd != 0)
    {
        cmd->setupOptions(opt);
        opt->processCommandArgs(argc-1, argv+1);

        if (helpFlag || cmd->helpFlag(opt))
        {
            opt->printUsage();
            status = 1;
        }
        else
        {
            cmd->configure(opt);
            status = cmd->run();
        }
    }
    else
    {
        /* argv[1] was not a valid command */

        assert(argc > 1);

        std::cerr << "Unknown "
                  << ((argv[1][0] == '-') ? "option" : "command")
                  << ": '" << argv[1] << "'"
                  << std::endl;

        status = 1;
    }

    delete opt;

    return status;
}
