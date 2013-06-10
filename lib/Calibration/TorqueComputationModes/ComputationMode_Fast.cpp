#include <vector>
using std::vector;
#include "TrialData.h"


template<typename NMSmodelT>
ComputationMode_Fast<NMSmodelT>::ComputationMode_Fast(NMSmodelT& subject):
subject_(subject) { }


template<typename NMSmodelT>
void ComputationMode_Fast<NMSmodelT>::setTrials( const vector<TrialData>& trials) {

    trials_ = trials;
    unsigned nMuscles = subject_.getNoMuscles();
    parametersT1_.assign(nMuscles, MuscleParameters());
    musclesToUpdate_.assign(nMuscles, true);
    
    
    // resizing forceDataT1_
    // forceDataT1_ forces at previous calibration step
    forceDataT1_.resize(trials_.size());
    for(unsigned int i = 0; i < trials_.size(); ++i) {
        forceDataT1_.at(i).resize(trials_.at(i).noLmtSteps_);
        for(unsigned int j = 0; j < trials_.at(i).noLmtSteps_; ++j)
            forceDataT1_.at(i).at(j).resize(nMuscles);
    }
     
    normFiberVelDataT1_.resize(trials_.size());
    for(unsigned int i = 0; i < trials_.size(); ++i) {
        normFiberVelDataT1_.at(i).resize(trials_.at(i).noLmtSteps_);
        for(unsigned int j = 0; j < trials_.at(i).noLmtSteps_; ++j)
            normFiberVelDataT1_.at(i).at(j).resize(nMuscles);
    }
}


template<typename NMSmodelT>
void ComputationMode_Fast<NMSmodelT>::getMusclesToUpdate() {

    vector<MuscleParameters> currentParameters;
    musclesToUpdate_.clear();
    subject_.getMusclesParameters(currentParameters);
    for(unsigned int i = 0; i < currentParameters.size(); ++i)
        if(!(currentParameters.at(i) == parametersT1_.at(i)))
            musclesToUpdate_.push_back(i);
    parametersT1_ = currentParameters;
}


template<typename NMSmodelT>
void ComputationMode_Fast<NMSmodelT>::initFiberLengthTraceCurves(unsigned trialIndex) {
    
    unsigned const ct = trialIndex;
    subject_.resetFibreLengthTraces(musclesToUpdate_);
    
    for (unsigned j = 0; j <  trials_.at(ct).lmtTimeSteps_.size(); ++j) {
        double lmtTime = trials_.at(ct).lmtTimeSteps_.at(j);
        subject_.setTime(lmtTime);
        subject_.setMuscleTendonLengthsSelective(trials_.at(ct).lmtData_.at(j), musclesToUpdate_);
        subject_.updateFibreLengths_OFFLINEPREP(musclesToUpdate_);
    }
    subject_.updateFibreLengthTraces(musclesToUpdate_); 
}

template<typename NMSmodelT>
void ComputationMode_Fast<NMSmodelT>::computeTorques(vector< vector< std::vector<double> > >& torques) {

    getMusclesToUpdate();
    // i is for the number of trials
    for (unsigned int ct = 0 ; ct < trials_.size(); ++ct) {
        initFiberLengthTraceCurves(ct);
        unsigned k = 0; // k is the index for lmt and ma data
        double lmtTime = trials_.at(ct).lmtTimeSteps_.at(k);

        for (int i = 0; i < trials_.at(ct).noEmgSteps_; ++i) {
            double emgTime = trials_.at(ct).emgTimeSteps_.at(i);
            if(emgTime < lmtTime) 
                subject_.setTime_emgs_updateActivations_pushState_selective(emgTime, trials_.at(ct).emgData_.at(i), musclesToUpdate_);

            if ( (lmtTime <= emgTime) && (k < trials_.at(ct).noLmtSteps_)) {
                subject_.setMuscleForces(forceDataT1_.at(ct).at(k));
                subject_.setTime(emgTime);
                subject_.setEmgsSelective(trials_.at(ct).emgData_.at(i), musclesToUpdate_);
                subject_.setMuscleTendonLengths(trials_.at(ct).lmtData_.at(k));
                for (int j = 0 ; j < trials_.at(ct).noDoF_; ++j)
                    subject_.setMomentArms(trials_.at(ct).maData_.at(j).at(k), j);
   
                subject_.updateState_OFFLINE(musclesToUpdate_);
                subject_.pushState(musclesToUpdate_);
                vector<double> currentTorques, currentForces;
                subject_.getTorques(currentTorques);
                subject_.getMuscleForces(currentForces);
                for (int j = 0; j < trials_.at(ct).noDoF_; ++j)
                    torques.at(ct).at(j).at(k) = currentTorques.at(j); 
                for (int j = 0; j < subject_.getNoMuscles(); ++j)
                    forceDataT1_.at(ct).at(k).at(j) = currentForces.at(j);
            
                ++k;
                if (k < trials_.at(ct).noLmtSteps_)
                    lmtTime = trials_.at(ct).lmtTimeSteps_.at(k);  

#ifdef DEBUG
                cout << endl << endl << "EmgTime: " << emgTime << endl << "EMG" << endl;
        
                for(unsigned int l=0; l < trials_.at(ct).emgData_.at(i).size(); ++l)
                    cout << trials_.at(ct).emgData_.at(i).at(l) << "\t" ;
                cout << endl << "LmtTime: " << lmtTime << endl;
                
                for(unsigned int l=0; l < trials_.at(ct).lmtData_.at(k).size(); ++l)
                    cout << trials_.at(ct).lmtData_.at(k).at(l) << "\t";
                cout << endl << "MomentArms" << endl;
                
                for(unsigned int l=0; l < trials_.at(ct).maData_.at(0).at(k).size(); ++l)
                    cout << trials_.at(ct).maData_.at(0).at(k).at(l) << "\t";
                cout << "\ncurrent torque: " << currentTorques.at(0);
        
                cout << endl << "----------------------------------------" << endl;
#endif
            }
            
        } 
    }
    
  
}


template<typename NMSmodelT>
void ComputationMode_Fast<NMSmodelT>::computePenalties(std::vector< std::vector<double> >& penalties) {
 
    
    
}


template<typename NMSmodelT>
ComputationMode_Fast<NMSmodelT>& ComputationMode_Fast<NMSmodelT>::operator=(const ComputationMode_Fast<NMSmodelT>& orig) {
    
    subject_          = orig.subject_;
    trials_           = orig.trials_;
    parametersT1_     = orig.parametersT1_;
    forceDataT1_      = orig.forceDataT1_;
    normFiberVelDataT1_ = orig.normFiberVelDataT1_;
    musclesToUpdate_  = orig.musclesToUpdate_;
    return *this;
}
