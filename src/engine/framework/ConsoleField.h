#pragma once

#include "ConsoleHistory.h"
#include "common/LineEditData.h"
#include "common/String.h"

namespace Console {

    class Field : public Util::LineEditData {
        public:
            Field(int size);

            void HistoryPrev();
            void HistoryNext();

            void RunCommand(Str::StringRef defaultCommand = "");
            void AutoComplete();

        private:
            History hist;
    };

}