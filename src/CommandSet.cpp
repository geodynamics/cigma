#include <iostream>
#include <cstdlib>
#include <cassert>
#include "CommandSet.h"

/* basic commands */
#include "HelpCmd.h"
#include "ListCmd.h"
#include "ExtractCmd.h"
#include "EvalCmd.h"
#include "CompareCmd.h"

/* misc commands for debugging */
#include "SkelCmd.h"
#include "CubeCmd.h"
#include "SearchCmd.h"
#include "InfoCmd.h"

using namespace cigma;


// ---------------------------------------------------------------------------

CommandSet::CommandSet()
{
}

CommandSet::~CommandSet()
{
    for(CmdMap::iterator i = commands.begin(); i != commands.end(); ++i)
    {
        Command *cmd = i->second;
        assert(cmd != 0);
        delete cmd;
    }
}

void CommandSet::initialize()
{
    /* create help command separately */
    HelpCmd *help = new HelpCmd();

    /* assemble set of commands */
    addCommand(help);
    addCommand(new ListCmd());
    addCommand(new ExtractCmd());
    addCommand(new EvalCmd());
    addCommand(new CompareCmd());

    /* additional commands for debugging */
    const bool debug = true;
    if (debug)
    {
        //addCommand(new SkelCmd());
        //addCommand(new CubeCmd());
        //addCommand(new SearchCmd());
        addCommand(new InfoCmd());
    }

    /* once assembled, pass set of commands to help command */
    help->setCommandSet(this);
}

void CommandSet::addCommand(Command *cmd)
{
    assert(cmd != 0);
    names.push_back(cmd->name);
    commands[cmd->name] = cmd;
}

Command *
CommandSet::getCommand(std::string name)
{
    Command *cmd = 0;
    CmdMap::iterator it = commands.find(name);
    if (it != commands.end())
    {
        cmd = it->second;
    }
    return cmd;
}

int CommandSet::main(int argc, char *argv[])
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

// ---------------------------------------------------------------------------
