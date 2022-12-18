#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <asm/param.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


float LinuxParser::MemoryUtilization() {

  float mem_utilization, mem_total, mem_free;
  string line, name;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line1stream(line);
    line1stream >> name >> mem_total;

    std::getline(stream, line);
    std::getline(stream, line);
    std::istringstream line3stream(line);
    line3stream >> name >> mem_free;
  }
  mem_utilization = ((mem_total - mem_free) / mem_total * 100.0) / 100.0;
  return mem_utilization;
}


long LinuxParser::UpTime() {

  long up_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> up_time;
  }
  return up_time;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {

  long up_time = UpTime();
  auto hz = sysconf(_SC_CLK_TCK);

  double utime, stime, cutime, cstime, starttime, total_time, seconds;
  long ld_var, cpu_usage;
  unsigned int u_var;
  unsigned long lu_var;
  unsigned long long llu_var;
  char c_var;
  int i_var;
  string line, s_var;

  int idx = 1;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream line_stream(line);
      while(idx <= 23){
        if (idx == 1 || (4 <= idx && idx <= 8)){
          line_stream >> i_var;
        } else if (idx == 2){
          line_stream >> s_var;
        } else if (idx == 3){
          line_stream >> c_var;
        } else if (idx == 9){
          line_stream >> u_var;
        } else if ((10 <= idx && idx <= 15) || idx == 23){
          line_stream >> lu_var;
        } else if (16 <= idx && idx <= 21){
          line_stream >> ld_var;
        } else if (idx == 22){
          line_stream >> llu_var;
        }

        switch(idx){
          case 14:
            utime = lu_var;
            break;
          case 15:
            stime = lu_var;
            break;
          case 16:
            cutime = ld_var;
            break;
          case 17:
            cstime = ld_var;
            break;
          case 22:
            starttime = llu_var;
            break;
          default:
            break;
        }

        idx += 1;
        if (idx == 23) {
          total_time = utime + stime + cutime + cstime;
          seconds = up_time - (starttime / hz);
          cpu_usage = 100 * ((total_time / hz) / seconds);

          return cpu_usage;
        }
      }
    }
  }
  return cpu_usage;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kUser_]) +
         stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) +
         stol(jiffies[CPUStates::kIOwait_]) +
         stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kIdle_]);
}


vector<string> LinuxParser::CpuUtilization() {
  vector<string> times{};
  string line, cpu, time;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> cpu;
    while(line_stream >> time){
      times.push_back(time);
    }
  }
  return times;
}


int LinuxParser::TotalProcesses() {

  int num_processes;
  string line;
  string name = " ";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream line_stream(line);
      line_stream >> name;
      if (name == "processes") {
        line_stream >> num_processes;
        break;
      }
    }
  }
  return num_processes;
}


int LinuxParser::RunningProcesses() {

  int num_processes;
  string line;
  string name = " ";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream line_stream(line);
      line_stream >> name;
      if (name == "procs_running") {
        line_stream >> num_processes;
        break;
      }
    }
  }
  return num_processes;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {

  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {

  string line, mum_usage;
  string name = " ";
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream line_stream(line);
      line_stream >> name;
      if (name == "VmSize:") {
        line_stream >> mum_usage;
        break;
      }
    }
  }
  return mum_usage;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {

  string line, uid;
  string name = " ";
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream line_stream(line);
      line_stream >> name;
      if (name == "Uid:") {
        line_stream >> uid;
        break;
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid  = Uid(pid);
  string search_term = ":x:" + uid + ":" + uid + ":";
  string line, name;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find(search_term) != string::npos) {
        auto idx = line.find(":");
        name = line.substr(0, idx);
        break;
      }
    }
  }
  return name;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {

  auto hz = sysconf(_SC_CLK_TCK);
  long time;
  string line;
  int idx = 1;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream line_stream(line);
      while(line_stream >> time){
        if (idx == 22) {
          return time / hz;
        }
        idx += 1;
      }
    }
  }
  return time;
}
