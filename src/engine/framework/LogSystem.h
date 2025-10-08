#pragma once

/*
 * The log system takes log events from different sources and forwards them
 * to a number of targets. The event and the targets are decoupled so that
 * we can choose precisely where to send each event. Targets include
 * the TTY and graphical console and the HUD.
 *
 * A full list of the targets and "printing" facilities can be found in
 * common/Log
 */

namespace Log {

    // Dispatches the event to all the targets specified by targetControl (flags)
    // Can be called by any thread.
    void Dispatch(Log::Event event, int targetControl);

    // Open the log file and start writing to it
    void OpenLogFile();

    void FlushLogFile();

    class Target {
        public:
            Target();
            virtual ~Target() = default;

            // Should process all the logs in the batch given or none at all
            // return true iff the logs were processed (on false the log system
            // retains them for later).
            // Can be called by any thread.
            virtual bool Process(const std::vector<Log::Event>& events) = 0;

        protected:
            // Register itself as the target with this id
            void Register(TargetId id);
    };

    // Internal

    void RegisterTarget(TargetId id, Target* target);
}