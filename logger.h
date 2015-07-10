#ifndef SIMPLE_STREAM_LOGGER_HPP
#define SIMPLE_STREAM_LOGGER_HPP

#include <fstream>
#include <sstream>
#include <chrono>
#include <mutex>

namespace streamlogger { 

enum class loglevel : unsigned char{
  DEBUG,
  INFO,
  WARN,
  ERROR,
  CRITICAL,
  FATAL
};

class logger {
private:
   loglevel d_loglevel;
   std::ofstream d_out;
   std::ostringstream d_oss;
   char d_buf[4096];
   std::mutex d_mtx;
   
private:
   template <typename T>
   int log_helper(T&& t); 
   
   template <typename T, typename... Rest>
   void log(loglevel lvl, T&& t, Rest&&... rest); 
   
public:

   logger(loglevel lvl, std::string const& fname, std::ios::openmode mode = std::ios::app) : 
      d_loglevel{lvl}, d_out{fname.c_str(), mode}, d_buf{0}, d_mtx{}
   {
      d_oss.rdbuf()->pubsetbuf(d_buf, sizeof(d_buf));
   }
   
   inline void set_level(loglevel l)
   {
      d_loglevel = l;
   }
   
   template<typename... Args>
   inline void debug(Args&&... args); 
   
   template<typename... Args>
   inline void info(Args&&... args); 
   
   template<typename... Args>
   inline void warn(Args&&... args); 
   
   template<typename... Args>
   inline void error(Args&&... args); 
   
   template<typename... Args>
   inline void critical(Args&&... args); 
   
   template<typename... Args>
   inline void fatal(Args&&... args); 
};

template <typename T>
int logger::log_helper(T&& t)
{
   this->d_oss << " " << t;
   return 0;
}

template <typename T, typename... Rest>
void logger::log(loglevel lvl, T&& t, Rest&&... args)
{
   if(this->d_loglevel <= lvl){
      std::lock_guard<std::mutex> lock(d_mtx);
      this->d_oss << t;
      auto _ = {log_helper(args)...};
      d_out << d_oss.str().c_str();
      d_oss.str("");
   }
}

template<typename... Args>
void logger::debug(Args&&... args)
{
   log(loglevel::DEBUG, std::forward<Args>(args)...);
} 

template<typename... Args>
void logger::info(Args&&... args)
{
   log(loglevel::INFO, std::forward<Args>(args)...);
} 

template<typename... Args>
void logger::warn(Args&&... args)
{
   log(loglevel::WARN, std::forward<Args>(args)...);
} 

template<typename... Args>
void logger::error(Args&&... args)
{
   log(loglevel::ERROR, std::forward<Args>(args)...);
} 

template<typename... Args>
void logger::critical(Args&&... args)
{
   log(loglevel::CRITICAL, std::forward<Args>(args)...);
} 

template<typename... Args>
void logger::fatal(Args&&... args)
{
   log(loglevel::FATAL, std::forward<Args>(args)...);
} 

}

#define TS std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())

#define CMN_MACROS __FILE__, ":", __FUNCTION__, ":", __LINE__

#define log_debug(log, ...)    log.debug(TS, ":<DEBUG:[", CMN_MACROS, "]>: ", ##__VA_ARGS__)
#define log_info(log, ...)     log.info(TS, ":<INFO:[", CMN_MACROS, "]>: ", ##__VA_ARGS__)
#define log_warn(log, ...)     log.warn(TS, ":<WARN:[", CMN_MACROS, "]>: ", ##__VA_ARGS__)
#define log_error(log, ...)    log.error(TS, ":<ERROR:[", CMN_MACROS, "]>: ", ##__VA_ARGS__)
#define log_critical(log, ...) log.critical(TS, ":<CRITICAL:[", CMN_MACROS, "]>: ", ##__VA_ARGS__)
#define log_fatal(log, ...)    log.fatal(TS, ":<FATAL:[", CMN_MACROS, "]>: ", ##__VA_ARGS__)

#endif /* SIMPLE_STREAM_LOGGER_HPP */


