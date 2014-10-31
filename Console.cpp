#include <cstdarg>
#include "Console.h"
#include "Shared/Engine.h"
#include "Global.h"

using namespace std;

///Public
void Console::Print(const char* message)
{
   Get().print(STREAM_DEFAULT, message);
}

va_list args;
void Console::Print(string message, ...)
{
   va_start(args, 0);
   Get().printFormat(STREAM_DEFAULT, message, args);
   va_end(args);
}

void Console::PrintError(string message, ...)
{
   va_start(args, 0);
   Get().printFormat(STREAM_ERROR, message, args);
   va_end(args);
}

void Console::PrintLog(string message, ...)
{
   va_start(args, 0);
   Get().printFormat(STREAM_LOG, message, args);
   va_end(args);
}

void Console::PrintInfo(string message, ...)
{
   va_start(args, 0);
   Get().printFormat(STREAM_INFO, message, args);
   va_end(args);
}

Console::Console(): running(false), streams{{cout, "", LOG_COUT}, {cerr, "Error: ", LOG_ERROR}, {clog, "Log: ", LOG_LOG}, {cout, "Info: ", LOG_COUT}}
{
   logDatas[LOG_COUT] = new LogData("out", false);
   logDatas[LOG_LOG] = new LogData("log", false);
   logDatas[LOG_ERROR] = new LogData("error", false);
}

Console::~Console()
{
   //Write log closure info
   chrono::time_point<chrono::system_clock> end;
   end = chrono::system_clock::now();
   time_t end_time = chrono::system_clock::to_time_t(end);
   string info = "End: " + (string)ctime(&end_time);

   for(auto i = 0; i < LOG_MAX; i++)
   {
      auto& log = getLogData((log_e)i);
      auto& file = log.file;
      if (log.enabled && file.is_open())
      {
         file << info;
         file.close();
      }
   }
}

void Console::startThread()
{
   if (!running)
   {
      running = true;
      thread = std::thread(&Console::update, this);
   }
}

void Console::stopThread()
{
   if (running)
   {
      running = false; //Will terminate thread when safe
      thread.join(); //Wait until this thread has terminated

      Console::PrintInfo("Console Thread Stopped Gracefully.");
   }
}

void Console::print(string message)
{
   print(STREAM_DEFAULT, message);
}

void Console::print(stream_e stream, string message)
{
   printFlushSafe(stream, getPrintFormat(getStreamData(stream), message));
}

void Console::printFormat(stream_e stream, string message, ...)
{
   va_list args;
   va_start(args, 0);
   printFormat(stream, message, args);
   va_end(args);
}

///Private
void Console::update()
{
   string input;
   char c;
   bool inputBlocking = false; //Prepare the thread for joining

   while(running)
   {
      if (!inputBlocking)
      {
         do
         {
            c = getchar();
            cin.clear();

            if (c == -1)
               inputBlocking = true;
         } while(c != '\r' && c != '\n' && running && !inputBlocking);

         if (running && !inputBlocking)
         {
            mtxInput.lock(); //Lock the console for input only

            cout << "Command: ";
            std::getline(cin, input);

            mtxInput.unlock();

            Engine::mtxGame.lock();

            if (input.length() > 0)
               commands.run(input);

            Engine::mtxGame.unlock();

            flushPending(); //Flush all pending messages created during lock
         }
      }

      sleep(1);
   }
}

template <typename T>
   void Console::printFlush(stream_e stream, T message)
{
   mtxPending.lock();

   auto& streamData = getStreamData(stream);
   auto& logData = getLogData(streamData.log);
   streamData.stream << message;

   if (logData.enabled && logData.file.is_open())
      logData.file << message;

   mtxPending.unlock();
}

template <typename T>
   void Console::printFlushSafe(stream_e stream, T message)
{
   if (mtxInput.try_lock())
   {
      printFlush(stream, message);
      mtxInput.unlock();
   }
   else
   {
      mtxPending.lock();

      PendingData* pendingData = new PendingData(stream);
      pendingData->message << message;

      pending.push_back(pendingData);

      mtxPending.unlock();
   }
}

string Console::getPrintFormat(StreamData& streamData, string message)
{
   return streamData.label + message + '\n';
}

void Console::flushPending()
{
   mtxPending.lock();

   for(auto pendingData: pending)
   {
      printFlush(pendingData->stream, pendingData->message.str());
      delete pendingData;
   }

   pending.clear();

   mtxPending.unlock();
}

void Console::printFormat(stream_e stream, string message, va_list args)
{
   StreamData& streamData = getStreamData(stream);
   const char* format = getPrintFormat(streamData, message).c_str();

   vsprintf(buffer, format, args);

   printFlushSafe(stream, buffer);
}

Console::StreamData& Console::getStreamData(stream_e stream)
{
   return streams[stream];
}

Console::LogData& Console::getLogData(log_e log)
{
   return *logDatas[log];
}

//PendingData
Console::PendingData::PendingData(stream_e stream): stream(stream)
{

}

//LogFile
Console::LogData::LogData(string filename, bool enabled): filename(filename)
{
   setEnabled(enabled);
   file.open("logs/" + filename + ".log", std::ios::app);
}

void Console::LogData::setEnabled(bool enabled)
{
   this->enabled = enabled;
}
