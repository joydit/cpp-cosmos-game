/*
   Asynchronously handles inputs and printing to console.

   Any output during the command inputting state will be appended following the conclusion of an inputted command.
*/

#pragma once

#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>
#include <iostream>
#include <Singleton.h>
#include "Commands.h"

class Console: public Singleton<Console>
{
public:
   typedef std::ostream stream_t;

   enum stream_e { STREAM_DEFAULT, STREAM_ERROR, STREAM_LOG, STREAM_INFO, STREAM_MAX };
   enum log_e { LOG_COUT, LOG_LOG, LOG_ERROR, LOG_MAX };

   static void Print(const char* message);
   static void Print(std::string message, ...);
   static void PrintError(std::string message, ...);
   static void PrintLog(std::string message, ...);
   static void PrintInfo(std::string message, ...);

   Commands commands;

   Console();
   ~Console();
   void startThread();
   void stopThread();
   void print(std::string message); //Print to the default stream
   void print(stream_e stream, std::string message); //Print to a particular output stream
   void printFormat(stream_e stream, std::string message, ...); //Allow string format parameters for inserting into message

private:
   struct StreamData
   {
      stream_t& stream;
      const char* label;
      log_e log;
   };

   struct PendingData
   {
      stream_e stream;
      std::stringstream message;

      PendingData(stream_e stream);
   };

   struct LogData
   {
      std::string filename;
      std::ofstream file;
      bool enabled;

      LogData(std::string filename, bool enabled);
      void setEnabled(bool enabled);
   };

   std::mutex mtxInput;
   std::mutex mtxPending;
   volatile bool running;
   std::thread thread;
   StreamData streams[STREAM_MAX];
   LogData* logDatas[LOG_MAX];
   std::list<PendingData*> pending;
   char buffer[256];

   void update();
   template <typename T> void printFlush(stream_e stream, T message);
   template <typename T> void printFlushSafe(stream_e stream, T message);
   std::string getPrintFormat(StreamData& streamData, std::string message);
   void flushPending();
   void printFormat(stream_e stream, std::string message, va_list args);
   StreamData& getStreamData(stream_e stream);
   LogData& getLogData(log_e log);
};
