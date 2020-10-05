

#include <boost/program_options.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

struct GenericOptions {
    bool debug_;
};

struct LsCommand : public GenericOptions {
    bool hidden_;
    std::string path_;
};

struct ChmodCommand : public GenericOptions {
    bool recurse_;
    std::string perms_;
    std::string path_;
};

typedef boost::variant<LsCommand, ChmodCommand> Command;

Command ParseOptions(int argc, char *argv[])
{
    namespace po = boost::program_options;

    po::options_description global("Global options");
    global.add_options()
        ("debug", "Turn on debug output")
        ("command", po::value<std::string>(), "command to execute")
        ("subargs", po::value<std::vector<std::string> >(), "Arguments for command");

    po::positional_options_description pos;
    pos.add("command", 1).
        add("subargs", -1);

    po::variables_map vm;

    po::parsed_options parsed = po::command_line_parser(argc, argv).
        options(global).
        positional(pos).
        allow_unregistered().
        run();

    po::store(parsed, vm);

    std::string cmd = vm["command"].as<std::string>();

    if (cmd == "ls")
    {
        // ls command has the following options:
        po::options_description ls_desc("ls options");
        ls_desc.add_options()
            ("hidden", "Show hidden files")
            ("path", po::value<std::string>(), "Path to list");

        // Collect all the unrecognized options from the first pass. This will include the
        // (positional) command name, so we need to erase that.
        std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
        opts.erase(opts.begin());

        // Parse again...
        po::store(po::command_line_parser(opts).options(ls_desc).run(), vm);

        LsCommand ls;
        ls.debug_ = vm.count("debug");
        ls.hidden_ = vm.count("hidden");
        ls.path_ = vm["path"].as<std::string>();

        return ls;
    }
    else if (cmd == "chmod")
    {
        // Something similar
    }

    // unrecognised command
    throw po::invalid_option_value(cmd);
}

int main(int argc, char* argv[])
{
    ParseOptions(argc, argv);
}