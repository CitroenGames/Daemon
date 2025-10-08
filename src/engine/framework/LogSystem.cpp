#include <common/FileSystem.h>
#include "qcommon/q_shared.h"
#include "qcommon/qcommon.h"
#include "LogSystem.h"

namespace Log {
    static Target* targets[MAX_TARGET_ID];


    //TODO make me reentrant // or check it is actually reentrant when using for (Event e : events) do stuff
    //TODO think way more about thread safety
    void Dispatch(Log::Event event, int targetControl) {
        static std::vector<Log::Event> buffers[MAX_TARGET_ID];
        static std::recursive_mutex bufferLocks[MAX_TARGET_ID];

        if (Sys::IsProcessTerminating()) {
            return;
        }

        for (int i = 0; i < MAX_TARGET_ID; i++) {
            if ((targetControl >> i) & 1) {
                std::lock_guard<std::recursive_mutex> guard(bufferLocks[i]);
                auto& buffer = buffers[i];

                buffer.push_back(event);

                bool processed = false;
                if (targets[i]) {
                    processed = targets[i]->Process(buffer);
                }

                if (processed || buffer.size() > 512) {
                    buffer.clear();
                }
            }
        }
    }

    void RegisterTarget(TargetId id, Target* target) {
        targets[id] = target;
    }

    Target::Target() = default;

    void Target::Register(TargetId id) {
        Log::RegisterTarget(id, this);
    }

    //Log Targets
    //TODO: move them in their respective modules
    //TODO this one isn't multithreaded at all, need a rewrite of the consoles
    class TTYTarget : public Target {
        public:
            TTYTarget() {
                this->Register(TTY_CONSOLE);
            }

            virtual bool Process(const std::vector<Log::Event>& events) override {
                for (auto& event : events)  {
                    CON_Print(event.text.c_str());
                    CON_Print("\n");
                }
                return true;
            }
    };

    static TTYTarget tty;

    //TODO add a Callback on these that will make the logFile open a new file or something?
    static Cvar::Cvar<bool> useLogFile("logs.logFile.active", "are the logs sent in the logfile", Cvar::NONE, true);
    static Cvar::Cvar<std::string> logFileName("logs.logFile.filename", "the name of the logfile", Cvar::INIT | Cvar::TEMPORARY, "daemon.log");
    static Cvar::Cvar<bool> overwrite("logs.logFile.overwrite", "if true the logfile is deleted at each run else the logs are just appended", Cvar::INIT | Cvar::TEMPORARY, true);
    static Cvar::Cvar<bool> forceFlush("logs.logFile.forceFlush", "are all the logs flushed immediately (more accurate but slower)", Cvar::INIT | Cvar::TEMPORARY, false);
    class LogFileTarget: public Target {
        public:
            LogFileTarget() {
                this->Register(LOGFILE);
            }

            virtual bool Process(const std::vector<Log::Event>& events) override {
                //If we have no log file drop the events
                if (not useLogFile.Get()) {
                    return true;
                }

                if (logFile) {
                    for (auto& event : events) {
                        logFile.Printf("%s\n", event.text);
                    }
                    return true;
                } else {
                    return false;
                }
            }

            FS::File logFile;
    };

    static LogFileTarget logfile;

    void OpenLogFile() {
        //If we have no log file do nothing here
        if (not useLogFile.Get()) {
            return;
        }

        try {
            if (overwrite.Get()) {
                logfile.logFile = FS::HomePath::OpenWrite(logFileName.Get());
            } else {
                logfile.logFile = FS::HomePath::OpenAppend(logFileName.Get());
            }

            if (forceFlush.Get()) {
                logfile.logFile.SetLineBuffered(true);
            }
        } catch (std::system_error& err) {
            Sys::Error("Could not open log file %s: %s", logFileName.Get(), err.what());
        }
    }

    void FlushLogFile() {
        std::error_code err;
        logfile.logFile.Flush(err);
        if (err) {
            Log::Warn("Error flushing log file");
        }
    }
}
