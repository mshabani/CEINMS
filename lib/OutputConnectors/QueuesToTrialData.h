//__________________________________________________________________________
// Author(s): Claudio Pizzolato, Monica Reggiani, Elena Ceseracciu - June 2014
// email:  claudio.pizzolato@griffithuni.edu.au
//         monica.reggiani@gmail.com
//         elena.ceseracciu@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//


#ifndef ceinms_QueuesToTrialData_h
#define ceinms_QueuesToTrialData_h

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include "OutputConnectors.h"
#include "InputConnectors.h"
#include "TrialData.h"

namespace ceinms {
    class QueuesToTrialData {
    public:
        QueuesToTrialData() = delete;
        QueuesToTrialData(const QueuesToTrialData&) = delete;
        QueuesToTrialData& operator=(const QueuesToTrialData&) = delete;

        template<typename NMSmodel> QueuesToTrialData(InputConnectors& inputConnectors, OutputConnectors& outputConnectors, NMSmodel& subject, std::string id) : inputConnectors_(inputConnectors), outputConnectors_(outputConnectors)
        {
            data_.id = id;
            std::vector<std::string> muscleNames;
            subject.getMuscleNames(muscleNames);
            data_.emgData.setLabels(muscleNames);
            data_.lmtData.setLabels(muscleNames);

            subject.getDoFNames(data_.dofNames);
            data_.noDoF = data_.dofNames.size();
            std::vector<std::vector<std::string>> maMuscleNames;
            subject.getMuscleNamesOnDofs(maMuscleNames);
            data_.maData.resize(data_.noDoF);
            for (auto i(0); i < data_.noDoF; ++i)
                data_.maData.at(i).setLabels(maMuscleNames.at(i));
            data_.torqueData.setLabels(data_.dofNames);

        };
        ~QueuesToTrialData() = default;
        void operator()();
        TrialData getTrialData();

    private:
        TrialData data_;
        InputConnectors& inputConnectors_;
        OutputConnectors& outputConnectors_;
        //std::vector<std::string> valuesToWrite_;
        //std::vector<std::string> torqueNames_;
        //std::vector<std::string> muscleNames_;
    };
}
#endif