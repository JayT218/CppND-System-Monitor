#include "processor.h"

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::string line, key;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> key;
        if (key == "cpu") {
            linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        }
    }

    Idle = idle + iowait;

    NonIdle = user + nice + system + irq + softirq + steal;

    Total = Idle + NonIdle;

    totald = Total - PrevTotal;
    idled = Idle - PrevIdle;

    PrevTotal = Total;
    PrevIdle = Idle;

    return (totald - idled) / totald;
}