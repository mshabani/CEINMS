/* -------------------------------------------------------------------------- *
 * CEINMS is a standalone toolbox for neuromusculoskeletal modelling and      *
 * simulation. CEINMS can also be used as a plugin for OpenSim either         *
 * through the OpenSim GUI or API. See https://simtk.org/home/ceinms and the  *
 * NOTICE file for more information. CEINMS development was coordinated       *
 * through Griffith University and supported by the Australian National       *
 * Health and Medical Research Council (NHMRC), the US National Institutes of *
 * Health (NIH), and the European Union Framework Programme 7 (EU FP7). Also  *
 * see the PROJECTS file for more information about the funding projects.     *
 *                                                                            *
 * Copyright (c) 2010-2015 Griffith University and the Contributors           *
 *                                                                            *
 * CEINMS Contributors: C. Pizzolato, M. Reggiani, M. Sartori,                *
 *                      E. Ceseracciu, and D.G. Lloyd                         *
 *                                                                            *
 * Author(s): C. Pizzolato, M. Reggiani                                       *
 *                                                                            *
 * CEINMS is licensed under the Apache License, Version 2.0 (the "License").  *
 * You may not use this file except in compliance with the License. You may   *
 * obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.*
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#ifndef ceinms_ModelEvaluationOffline_h
#define ceinms_ModelEvaluationOffline_h

#include <vector>
#include <list>
#include <string>
#include "ModelEvaluationBase.h"

namespace ceinms {

    template <typename NMSmodelT, typename Logger>
    class ModelEvaluationOffline : public ModelEvaluationBase < Logger > {

    public:
        ModelEvaluationOffline() = delete;
        ModelEvaluationOffline(InputConnectors& inputConnectors, OutputConnectors& outputConnectors, NMSmodelT& subject, const std::vector< std::string >& valuesToLog);
        ~ModelEvaluationOffline();

        void setOutputDirectory(const std::string& outputDir);
        void operator()();

    private:
        void readDataFromQueues();
        void initOfflineCurve();
        NMSmodelT& subject_;
        std::string outputDir_;
        std::vector< std::string > dofNames_;
        std::vector< std::string > dofNamesWithExtTorque_;

        std::list< InputConnectors::FrameType > lmtDataFromQueue_, emgDataFromQueue_, externalTorquesDataFromQueue_;
        std::list< std::vector < InputConnectors::FrameType > > maDataFromQueue_;
        unsigned noDof_;
        double globalEmDelay_;
    };
}

#include "ModelEvaluationOffline.cpp"
#endif
